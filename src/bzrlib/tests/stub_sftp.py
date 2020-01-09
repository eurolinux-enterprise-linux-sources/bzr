# Copyright (C) 2005, 2006, 2008, 2009, 2010 Robey Pointer <robey@lag.net>, Canonical Ltd
#
# This program is free software; you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation; either version 2 of the License, or
# (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program; if not, write to the Free Software
# Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA

"""
A stub SFTP server for loopback SFTP testing.
Adapted from the one in paramiko's unit tests.
"""

import os
import paramiko
import select
import socket
import sys
import threading
import time

from bzrlib import (
    osutils,
    trace,
    urlutils,
    )
from bzrlib.transport import (
    local,
    Server,
    ssh,
    )

class StubServer (paramiko.ServerInterface):

    def __init__(self, test_case):
        paramiko.ServerInterface.__init__(self)
        self._test_case = test_case

    def check_auth_password(self, username, password):
        # all are allowed
        self._test_case.log('sftpserver - authorizing: %s' % (username,))
        return paramiko.AUTH_SUCCESSFUL

    def check_channel_request(self, kind, chanid):
        self._test_case.log(
            'sftpserver - channel request: %s, %s' % (kind, chanid))
        return paramiko.OPEN_SUCCEEDED


class StubSFTPHandle (paramiko.SFTPHandle):
    def stat(self):
        try:
            return paramiko.SFTPAttributes.from_stat(
                os.fstat(self.readfile.fileno()))
        except OSError, e:
            return paramiko.SFTPServer.convert_errno(e.errno)

    def chattr(self, attr):
        # python doesn't have equivalents to fchown or fchmod, so we have to
        # use the stored filename
        trace.mutter('Changing permissions on %s to %s', self.filename, attr)
        try:
            paramiko.SFTPServer.set_file_attr(self.filename, attr)
        except OSError, e:
            return paramiko.SFTPServer.convert_errno(e.errno)


class StubSFTPServer (paramiko.SFTPServerInterface):

    def __init__(self, server, root, home=None):
        paramiko.SFTPServerInterface.__init__(self, server)
        # All paths are actually relative to 'root'.
        # this is like implementing chroot().
        self.root = root
        if home is None:
            self.home = ''
        else:
            if not home.startswith(self.root):
                raise AssertionError(
                    "home must be a subdirectory of root (%s vs %s)"
                    % (home, root))
            self.home = home[len(self.root):]
        if self.home.startswith('/'):
            self.home = self.home[1:]
        server._test_case.log('sftpserver - new connection')

    def _realpath(self, path):
        # paths returned from self.canonicalize() always start with
        # a path separator. So if 'root' is just '/', this would cause
        # a double slash at the beginning '//home/dir'.
        if self.root == '/':
            return self.canonicalize(path)
        return self.root + self.canonicalize(path)

    if sys.platform == 'win32':
        def canonicalize(self, path):
            # Win32 sftp paths end up looking like
            #     sftp://host@foo/h:/foo/bar
            # which means absolute paths look like:
            #     /h:/foo/bar
            # and relative paths stay the same:
            #     foo/bar
            # win32 needs to use the Unicode APIs. so we require the
            # paths to be utf8 (Linux just uses bytestreams)
            thispath = path.decode('utf8')
            if path.startswith('/'):
                # Abspath H:/foo/bar
                return os.path.normpath(thispath[1:])
            else:
                return os.path.normpath(os.path.join(self.home, thispath))
    else:
        def canonicalize(self, path):
            if os.path.isabs(path):
                return os.path.normpath(path)
            else:
                return os.path.normpath('/' + os.path.join(self.home, path))

    def chattr(self, path, attr):
        try:
            paramiko.SFTPServer.set_file_attr(path, attr)
        except OSError, e:
            return paramiko.SFTPServer.convert_errno(e.errno)
        return paramiko.SFTP_OK

    def list_folder(self, path):
        path = self._realpath(path)
        try:
            out = [ ]
            # TODO: win32 incorrectly lists paths with non-ascii if path is not
            # unicode. However on Linux the server should only deal with
            # bytestreams and posix.listdir does the right thing
            if sys.platform == 'win32':
                flist = [f.encode('utf8') for f in os.listdir(path)]
            else:
                flist = os.listdir(path)
            for fname in flist:
                attr = paramiko.SFTPAttributes.from_stat(
                    os.stat(osutils.pathjoin(path, fname)))
                attr.filename = fname
                out.append(attr)
            return out
        except OSError, e:
            return paramiko.SFTPServer.convert_errno(e.errno)

    def stat(self, path):
        path = self._realpath(path)
        try:
            return paramiko.SFTPAttributes.from_stat(os.stat(path))
        except OSError, e:
            return paramiko.SFTPServer.convert_errno(e.errno)

    def lstat(self, path):
        path = self._realpath(path)
        try:
            return paramiko.SFTPAttributes.from_stat(os.lstat(path))
        except OSError, e:
            return paramiko.SFTPServer.convert_errno(e.errno)

    def open(self, path, flags, attr):
        path = self._realpath(path)
        try:
            flags |= getattr(os, 'O_BINARY', 0)
            if getattr(attr, 'st_mode', None):
                fd = os.open(path, flags, attr.st_mode)
            else:
                # os.open() defaults to 0777 which is
                # an odd default mode for files
                fd = os.open(path, flags, 0666)
        except OSError, e:
            return paramiko.SFTPServer.convert_errno(e.errno)

        if (flags & os.O_CREAT) and (attr is not None):
            attr._flags &= ~attr.FLAG_PERMISSIONS
            paramiko.SFTPServer.set_file_attr(path, attr)
        if flags & os.O_WRONLY:
            fstr = 'wb'
        elif flags & os.O_RDWR:
            fstr = 'rb+'
        else:
            # O_RDONLY (== 0)
            fstr = 'rb'
        try:
            f = os.fdopen(fd, fstr)
        except (IOError, OSError), e:
            return paramiko.SFTPServer.convert_errno(e.errno)
        fobj = StubSFTPHandle()
        fobj.filename = path
        fobj.readfile = f
        fobj.writefile = f
        return fobj

    def remove(self, path):
        path = self._realpath(path)
        try:
            os.remove(path)
        except OSError, e:
            return paramiko.SFTPServer.convert_errno(e.errno)
        return paramiko.SFTP_OK

    def rename(self, oldpath, newpath):
        oldpath = self._realpath(oldpath)
        newpath = self._realpath(newpath)
        try:
            os.rename(oldpath, newpath)
        except OSError, e:
            return paramiko.SFTPServer.convert_errno(e.errno)
        return paramiko.SFTP_OK

    def mkdir(self, path, attr):
        path = self._realpath(path)
        try:
            # Using getattr() in case st_mode is None or 0
            # both evaluate to False
            if getattr(attr, 'st_mode', None):
                os.mkdir(path, attr.st_mode)
            else:
                os.mkdir(path)
            if attr is not None:
                attr._flags &= ~attr.FLAG_PERMISSIONS
                paramiko.SFTPServer.set_file_attr(path, attr)
        except OSError, e:
            return paramiko.SFTPServer.convert_errno(e.errno)
        return paramiko.SFTP_OK

    def rmdir(self, path):
        path = self._realpath(path)
        try:
            os.rmdir(path)
        except OSError, e:
            return paramiko.SFTPServer.convert_errno(e.errno)
        return paramiko.SFTP_OK

    # removed: chattr, symlink, readlink
    # (nothing in bzr's sftp transport uses those)

# ------------- server test implementation --------------

STUB_SERVER_KEY = """
-----BEGIN RSA PRIVATE KEY-----
MIICWgIBAAKBgQDTj1bqB4WmayWNPB+8jVSYpZYk80Ujvj680pOTh2bORBjbIAyz
oWGW+GUjzKxTiiPvVmxFgx5wdsFvF03v34lEVVhMpouqPAYQ15N37K/ir5XY+9m/
d8ufMCkjeXsQkKqFbAlQcnWMCRnOoPHS3I4vi6hmnDDeeYTSRvfLbW0fhwIBIwKB
gBIiOqZYaoqbeD9OS9z2K9KR2atlTxGxOJPXiP4ESqP3NVScWNwyZ3NXHpyrJLa0
EbVtzsQhLn6rF+TzXnOlcipFvjsem3iYzCpuChfGQ6SovTcOjHV9z+hnpXvQ/fon
soVRZY65wKnF7IAoUwTmJS9opqgrN6kRgCd3DASAMd1bAkEA96SBVWFt/fJBNJ9H
tYnBKZGw0VeHOYmVYbvMSstssn8un+pQpUm9vlG/bp7Oxd/m+b9KWEh2xPfv6zqU
avNwHwJBANqzGZa/EpzF4J8pGti7oIAPUIDGMtfIcmqNXVMckrmzQ2vTfqtkEZsA
4rE1IERRyiJQx6EJsz21wJmGV9WJQ5kCQQDwkS0uXqVdFzgHO6S++tjmjYcxwr3g
H0CoFYSgbddOT6miqRskOQF3DZVkJT3kyuBgU2zKygz52ukQZMqxCb1fAkASvuTv
qfpH87Qq5kQhNKdbbwbmd2NxlNabazPijWuphGTdW0VfJdWfklyS2Kr+iqrs/5wV
HhathJt636Eg7oIjAkA8ht3MQ+XSl9yIJIS8gVpbPxSw5OMfw0PjVE7tBdQruiSc
nvuQES5C9BMHjF39LZiGH1iLQy7FgdHyoP+eodI7
-----END RSA PRIVATE KEY-----
"""


class SocketListener(threading.Thread):

    def __init__(self, callback):
        threading.Thread.__init__(self)
        self._callback = callback
        self._socket = socket.socket()
        self._socket.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
        self._socket.bind(('localhost', 0))
        self._socket.listen(1)
        self.host, self.port = self._socket.getsockname()[:2]
        self._stop_event = threading.Event()

    def stop(self):
        # called from outside this thread
        self._stop_event.set()
        # use a timeout here, because if the test fails, the server thread may
        # never notice the stop_event.
        self.join(5.0)
        self._socket.close()

    def run(self):
        while True:
            readable, writable_unused, exception_unused = \
                select.select([self._socket], [], [], 0.1)
            if self._stop_event.isSet():
                return
            if len(readable) == 0:
                continue
            try:
                s, addr_unused = self._socket.accept()
                # because the loopback socket is inline, and transports are
                # never explicitly closed, best to launch a new thread.
                threading.Thread(target=self._callback, args=(s,)).start()
            except socket.error, x:
                sys.excepthook(*sys.exc_info())
                trace.warning('Socket error during accept() '
                              'within unit test server thread: %r' % x)
            except Exception, x:
                # probably a failed test; unit test thread will log the
                # failure/error
                sys.excepthook(*sys.exc_info())
                trace.warning(
                    'Exception from within unit test server thread: %r' % x)


class SocketDelay(object):
    """A socket decorator to make TCP appear slower.

    This changes recv, send, and sendall to add a fixed latency to each python
    call if a new roundtrip is detected. That is, when a recv is called and the
    flag new_roundtrip is set, latency is charged. Every send and send_all
    sets this flag.

    In addition every send, sendall and recv sleeps a bit per character send to
    simulate bandwidth.

    Not all methods are implemented, this is deliberate as this class is not a
    replacement for the builtin sockets layer. fileno is not implemented to
    prevent the proxy being bypassed.
    """

    simulated_time = 0
    _proxied_arguments = dict.fromkeys([
        "close", "getpeername", "getsockname", "getsockopt", "gettimeout",
        "setblocking", "setsockopt", "settimeout", "shutdown"])

    def __init__(self, sock, latency, bandwidth=1.0,
                 really_sleep=True):
        """
        :param bandwith: simulated bandwith (MegaBit)
        :param really_sleep: If set to false, the SocketDelay will just
        increase a counter, instead of calling time.sleep. This is useful for
        unittesting the SocketDelay.
        """
        self.sock = sock
        self.latency = latency
        self.really_sleep = really_sleep
        self.time_per_byte = 1 / (bandwidth / 8.0 * 1024 * 1024)
        self.new_roundtrip = False

    def sleep(self, s):
        if self.really_sleep:
            time.sleep(s)
        else:
            SocketDelay.simulated_time += s

    def __getattr__(self, attr):
        if attr in SocketDelay._proxied_arguments:
            return getattr(self.sock, attr)
        raise AttributeError("'SocketDelay' object has no attribute %r" %
                             attr)

    def dup(self):
        return SocketDelay(self.sock.dup(), self.latency, self.time_per_byte,
                           self._sleep)

    def recv(self, *args):
        data = self.sock.recv(*args)
        if data and self.new_roundtrip:
            self.new_roundtrip = False
            self.sleep(self.latency)
        self.sleep(len(data) * self.time_per_byte)
        return data

    def sendall(self, data, flags=0):
        if not self.new_roundtrip:
            self.new_roundtrip = True
            self.sleep(self.latency)
        self.sleep(len(data) * self.time_per_byte)
        return self.sock.sendall(data, flags)

    def send(self, data, flags=0):
        if not self.new_roundtrip:
            self.new_roundtrip = True
            self.sleep(self.latency)
        bytes_sent = self.sock.send(data, flags)
        self.sleep(bytes_sent * self.time_per_byte)
        return bytes_sent


class SFTPServer(Server):
    """Common code for SFTP server facilities."""

    def __init__(self, server_interface=StubServer):
        self._original_vendor = None
        self._homedir = None
        self._server_homedir = None
        self._listener = None
        self._root = None
        self._vendor = ssh.ParamikoVendor()
        self._server_interface = server_interface
        # sftp server logs
        self.logs = []
        self.add_latency = 0

    def _get_sftp_url(self, path):
        """Calculate an sftp url to this server for path."""
        return 'sftp://foo:bar@%s:%d/%s' % (self._listener.host,
                                            self._listener.port, path)

    def log(self, message):
        """StubServer uses this to log when a new server is created."""
        self.logs.append(message)

    def _run_server_entry(self, sock):
        """Entry point for all implementations of _run_server.

        If self.add_latency is > 0.000001 then sock is given a latency adding
        decorator.
        """
        if self.add_latency > 0.000001:
            sock = SocketDelay(sock, self.add_latency)
        return self._run_server(sock)

    def _run_server(self, s):
        ssh_server = paramiko.Transport(s)
        key_file = osutils.pathjoin(self._homedir, 'test_rsa.key')
        f = open(key_file, 'w')
        f.write(STUB_SERVER_KEY)
        f.close()
        host_key = paramiko.RSAKey.from_private_key_file(key_file)
        ssh_server.add_server_key(host_key)
        server = self._server_interface(self)
        ssh_server.set_subsystem_handler('sftp', paramiko.SFTPServer,
                                         StubSFTPServer, root=self._root,
                                         home=self._server_homedir)
        event = threading.Event()
        ssh_server.start_server(event, server)
        event.wait(5.0)

    def start_server(self, backing_server=None):
        # XXX: TODO: make sftpserver back onto backing_server rather than local
        # disk.
        if not (backing_server is None or
                isinstance(backing_server, local.LocalURLServer)):
            raise AssertionError(
                'backing_server should not be %r, because this can only serve '
                'the local current working directory.' % (backing_server,))
        self._original_vendor = ssh._ssh_vendor_manager._cached_ssh_vendor
        ssh._ssh_vendor_manager._cached_ssh_vendor = self._vendor
        # FIXME: the following block should certainly just be self._homedir =
        # osutils.getcwd() but that fails badly on Unix -- vila 20100224
        if sys.platform == 'win32':
            # Win32 needs to use the UNICODE api
            self._homedir = os.getcwdu()
        else:
            # But Linux SFTP servers should just deal in bytestreams
            self._homedir = os.getcwd()
        if self._server_homedir is None:
            self._server_homedir = self._homedir
        self._root = '/'
        if sys.platform == 'win32':
            self._root = ''
        self._listener = SocketListener(self._run_server_entry)
        self._listener.setDaemon(True)
        self._listener.start()

    def stop_server(self):
        self._listener.stop()
        ssh._ssh_vendor_manager._cached_ssh_vendor = self._original_vendor

    def get_bogus_url(self):
        """See bzrlib.transport.Server.get_bogus_url."""
        # this is chosen to try to prevent trouble with proxies, wierd dns, etc
        # we bind a random socket, so that we get a guaranteed unused port
        # we just never listen on that port
        s = socket.socket()
        s.bind(('localhost', 0))
        return 'sftp://%s:%s/' % s.getsockname()


class SFTPFullAbsoluteServer(SFTPServer):
    """A test server for sftp transports, using absolute urls and ssh."""

    def get_url(self):
        """See bzrlib.transport.Server.get_url."""
        homedir = self._homedir
        if sys.platform != 'win32':
            # Remove the initial '/' on all platforms but win32
            homedir = homedir[1:]
        return self._get_sftp_url(urlutils.escape(homedir))


class SFTPServerWithoutSSH(SFTPServer):
    """An SFTP server that uses a simple TCP socket pair rather than SSH."""

    def __init__(self):
        super(SFTPServerWithoutSSH, self).__init__()
        self._vendor = ssh.LoopbackVendor()

    def _run_server(self, sock):
        # Re-import these as locals, so that they're still accessible during
        # interpreter shutdown (when all module globals get set to None, leading
        # to confusing errors like "'NoneType' object has no attribute 'error'".
        class FakeChannel(object):
            def get_transport(self):
                return self
            def get_log_channel(self):
                return 'paramiko'
            def get_name(self):
                return '1'
            def get_hexdump(self):
                return False
            def close(self):
                pass

        server = paramiko.SFTPServer(
            FakeChannel(), 'sftp', StubServer(self), StubSFTPServer,
            root=self._root, home=self._server_homedir)
        try:
            server.start_subsystem(
                'sftp', None, ssh.SocketAsChannelAdapter(sock))
        except socket.error, e:
            if (len(e.args) > 0) and (e.args[0] == errno.EPIPE):
                # it's okay for the client to disconnect abruptly
                # (bug in paramiko 1.6: it should absorb this exception)
                pass
            else:
                raise
        except Exception, e:
            # This typically seems to happen during interpreter shutdown, so
            # most of the useful ways to report this error are won't work.
            # Writing the exception type, and then the text of the exception,
            # seems to be the best we can do.
            import sys
            sys.stderr.write('\nEXCEPTION %r: ' % (e.__class__,))
            sys.stderr.write('%s\n\n' % (e,))
        server.finish_subsystem()


class SFTPAbsoluteServer(SFTPServerWithoutSSH):
    """A test server for sftp transports, using absolute urls."""

    def get_url(self):
        """See bzrlib.transport.Server.get_url."""
        homedir = self._homedir
        if sys.platform != 'win32':
            # Remove the initial '/' on all platforms but win32
            homedir = homedir[1:]
        return self._get_sftp_url(urlutils.escape(homedir))


class SFTPHomeDirServer(SFTPServerWithoutSSH):
    """A test server for sftp transports, using homedir relative urls."""

    def get_url(self):
        """See bzrlib.transport.Server.get_url."""
        return self._get_sftp_url("~/")


class SFTPSiblingAbsoluteServer(SFTPAbsoluteServer):
    """A test server for sftp transports where only absolute paths will work.

    It does this by serving from a deeply-nested directory that doesn't exist.
    """

    def start_server(self, backing_server=None):
        self._server_homedir = '/dev/noone/runs/tests/here'
        super(SFTPSiblingAbsoluteServer, self).start_server(backing_server)

