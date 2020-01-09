/* Generated by Pyrex 0.9.8.5 on Wed Mar 24 18:17:22 2010 */

#define PY_SSIZE_T_CLEAN
#include "Python.h"
#include "structmember.h"
#ifndef PY_LONG_LONG
  #define PY_LONG_LONG LONG_LONG
#endif
#if PY_VERSION_HEX < 0x02050000
  typedef int Py_ssize_t;
  #define PY_SSIZE_T_MAX INT_MAX
  #define PY_SSIZE_T_MIN INT_MIN
  #define PyInt_FromSsize_t(z) PyInt_FromLong(z)
  #define PyInt_AsSsize_t(o)	PyInt_AsLong(o)
#endif
#if !defined(WIN32) && !defined(MS_WINDOWS)
  #ifndef __stdcall
    #define __stdcall
  #endif
  #ifndef __cdecl
    #define __cdecl
  #endif
#endif
#ifdef __cplusplus
#define __PYX_EXTERN_C extern "C"
#else
#define __PYX_EXTERN_C extern
#endif
#include <math.h>
#include "python-compat.h"
#include "stdlib.h"
#include "string.h"


typedef struct {PyObject **p; int i; char *s; long n;} __Pyx_StringTabEntry; /*proto*/

static PyObject *__pyx_m;
static PyObject *__pyx_b;
static int __pyx_lineno;
static char *__pyx_filename;
static char **__pyx_f;

static char __pyx_mdoc[] = "Pyrex extensions for converting chunks to lines.";

static void __Pyx_Raise(PyObject *type, PyObject *value, PyObject *tb); /*proto*/

static int __Pyx_InitStrings(__Pyx_StringTabEntry *t); /*proto*/

static void __Pyx_AddTraceback(char *funcname); /*proto*/

/* Declarations from bzrlib._chunks_to_lines_pyx */


/* Declarations from implementation of bzrlib._chunks_to_lines_pyx */


static char __pyx_k1[] = "chunk is not a string";


static PyObject *__pyx_k1p;

static __Pyx_StringTabEntry __pyx_string_tab[] = {
  {&__pyx_k1p, 0, __pyx_k1, sizeof(__pyx_k1)},
  {0, 0, 0, 0}
};



/* Implementation of bzrlib._chunks_to_lines_pyx */

static PyObject *__pyx_f_6bzrlib_20_chunks_to_lines_pyx_chunks_to_lines(PyObject *__pyx_self, PyObject *__pyx_args, PyObject *__pyx_kwds); /*proto*/
static char __pyx_doc_6bzrlib_20_chunks_to_lines_pyx_chunks_to_lines[] = "Re-split chunks into simple lines.\n\n    Each entry in the result should contain a single newline at the end. Except\n    for the last entry which may not have a final newline. If chunks is already\n    a simple list of lines, we return it directly.\n\n    :param chunks: An list/tuple of strings. If chunks is already a list of\n        lines, then we will return it as-is.\n    :return: A list of strings.\n    ";
static PyObject *__pyx_f_6bzrlib_20_chunks_to_lines_pyx_chunks_to_lines(PyObject *__pyx_self, PyObject *__pyx_args, PyObject *__pyx_kwds) {
  PyObject *__pyx_v_chunks = 0;
  char *__pyx_v_c_str;
  char *__pyx_v_newline;
  char *__pyx_v_c_last;
  Py_ssize_t __pyx_v_the_len;
  int __pyx_v_last_no_newline;
  PyObject *__pyx_v_chunk;
  PyObject *__pyx_v_lines;
  PyObject *__pyx_v_tail;
  PyObject *__pyx_v_line;
  PyObject *__pyx_r;
  PyObject *__pyx_1 = 0;
  PyObject *__pyx_2 = 0;
  int __pyx_3;
  PyObject *__pyx_4 = 0;
  static char *__pyx_argnames[] = {"chunks",0};
  if (!PyArg_ParseTupleAndKeywords(__pyx_args, __pyx_kwds, "O", __pyx_argnames, &__pyx_v_chunks)) return 0;
  Py_INCREF(__pyx_v_chunks);
  __pyx_v_chunk = Py_None; Py_INCREF(Py_None);
  __pyx_v_lines = Py_None; Py_INCREF(Py_None);
  __pyx_v_tail = Py_None; Py_INCREF(Py_None);
  __pyx_v_line = Py_None; Py_INCREF(Py_None);

  /* "/home/mbp/bzr/prepare-2.1/bzrlib/_chunks_to_lines_pyx.pyx":60 */
  __pyx_v_last_no_newline = 0;

  /* "/home/mbp/bzr/prepare-2.1/bzrlib/_chunks_to_lines_pyx.pyx":61 */
  __pyx_1 = PyObject_GetIter(__pyx_v_chunks); if (!__pyx_1) {__pyx_filename = __pyx_f[0]; __pyx_lineno = 61; goto __pyx_L1;}
  for (;;) {
    __pyx_2 = PyIter_Next(__pyx_1);
    if (!__pyx_2) {
      if (PyErr_Occurred()) {__pyx_filename = __pyx_f[0]; __pyx_lineno = 61; goto __pyx_L1;}
      break;
    }
    Py_DECREF(__pyx_v_chunk);
    __pyx_v_chunk = __pyx_2;
    __pyx_2 = 0;

    /* "/home/mbp/bzr/prepare-2.1/bzrlib/_chunks_to_lines_pyx.pyx":62 */
    __pyx_3 = __pyx_v_last_no_newline;
    if (__pyx_3) {
      goto __pyx_L3;
      goto __pyx_L4;
    }
    __pyx_L4:;

    /* "/home/mbp/bzr/prepare-2.1/bzrlib/_chunks_to_lines_pyx.pyx":70 */
    __pyx_3 = (!PyString_CheckExact(__pyx_v_chunk));
    if (__pyx_3) {
      __pyx_2 = PyTuple_New(1); if (!__pyx_2) {__pyx_filename = __pyx_f[0]; __pyx_lineno = 71; goto __pyx_L1;}
      Py_INCREF(__pyx_k1p);
      PyTuple_SET_ITEM(__pyx_2, 0, __pyx_k1p);
      __pyx_4 = PyObject_CallObject(PyExc_TypeError, __pyx_2); if (!__pyx_4) {__pyx_filename = __pyx_f[0]; __pyx_lineno = 71; goto __pyx_L1;}
      Py_DECREF(__pyx_2); __pyx_2 = 0;
      __Pyx_Raise(__pyx_4, 0, 0);
      Py_DECREF(__pyx_4); __pyx_4 = 0;
      {__pyx_filename = __pyx_f[0]; __pyx_lineno = 71; goto __pyx_L1;}
      goto __pyx_L5;
    }
    __pyx_L5:;

    /* "/home/mbp/bzr/prepare-2.1/bzrlib/_chunks_to_lines_pyx.pyx":72 */
    __pyx_v_the_len = PyString_GET_SIZE(__pyx_v_chunk);

    /* "/home/mbp/bzr/prepare-2.1/bzrlib/_chunks_to_lines_pyx.pyx":73 */
    __pyx_3 = (__pyx_v_the_len == 0);
    if (__pyx_3) {
      goto __pyx_L3;
      goto __pyx_L6;
    }
    __pyx_L6:;

    /* "/home/mbp/bzr/prepare-2.1/bzrlib/_chunks_to_lines_pyx.pyx":76 */
    __pyx_v_c_str = PyString_AS_STRING(__pyx_v_chunk);

    /* "/home/mbp/bzr/prepare-2.1/bzrlib/_chunks_to_lines_pyx.pyx":77 */
    __pyx_v_c_last = ((__pyx_v_c_str + __pyx_v_the_len) - 1);

    /* "/home/mbp/bzr/prepare-2.1/bzrlib/_chunks_to_lines_pyx.pyx":78 */
    __pyx_v_newline = ((char *)memchr(__pyx_v_c_str,'\n',__pyx_v_the_len));

    /* "/home/mbp/bzr/prepare-2.1/bzrlib/_chunks_to_lines_pyx.pyx":79 */
    __pyx_3 = (__pyx_v_newline != __pyx_v_c_last);
    if (__pyx_3) {
      __pyx_3 = (__pyx_v_newline == NULL);
      if (__pyx_3) {
        __pyx_v_last_no_newline = 1;
        goto __pyx_L8;
      }
      /*else*/ {
        goto __pyx_L3;
      }
      __pyx_L8:;
      goto __pyx_L7;
    }
    __pyx_L7:;
  }
  /*else*/ {
    Py_INCREF(__pyx_v_chunks);
    __pyx_r = __pyx_v_chunks;
    Py_DECREF(__pyx_1); __pyx_1 = 0;
    goto __pyx_L0;
  }
  __pyx_L3:;
  Py_DECREF(__pyx_1); __pyx_1 = 0;

  /* "/home/mbp/bzr/prepare-2.1/bzrlib/_chunks_to_lines_pyx.pyx":91 */
  __pyx_2 = PyList_New(0); if (!__pyx_2) {__pyx_filename = __pyx_f[0]; __pyx_lineno = 91; goto __pyx_L1;}
  Py_DECREF(__pyx_v_lines);
  __pyx_v_lines = __pyx_2;
  __pyx_2 = 0;

  /* "/home/mbp/bzr/prepare-2.1/bzrlib/_chunks_to_lines_pyx.pyx":92 */
  Py_INCREF(Py_None);
  Py_DECREF(__pyx_v_tail);
  __pyx_v_tail = Py_None;

  /* "/home/mbp/bzr/prepare-2.1/bzrlib/_chunks_to_lines_pyx.pyx":93 */
  __pyx_4 = PyObject_GetIter(__pyx_v_chunks); if (!__pyx_4) {__pyx_filename = __pyx_f[0]; __pyx_lineno = 93; goto __pyx_L1;}
  for (;;) {
    __pyx_1 = PyIter_Next(__pyx_4);
    if (!__pyx_1) {
      if (PyErr_Occurred()) {__pyx_filename = __pyx_f[0]; __pyx_lineno = 93; goto __pyx_L1;}
      break;
    }
    Py_DECREF(__pyx_v_chunk);
    __pyx_v_chunk = __pyx_1;
    __pyx_1 = 0;

    /* "/home/mbp/bzr/prepare-2.1/bzrlib/_chunks_to_lines_pyx.pyx":94 */
    __pyx_3 = __pyx_v_tail != Py_None;
    if (__pyx_3) {

      /* "/home/mbp/bzr/prepare-2.1/bzrlib/_chunks_to_lines_pyx.pyx":95 */
      __pyx_2 = PyNumber_Add(__pyx_v_tail, __pyx_v_chunk); if (!__pyx_2) {__pyx_filename = __pyx_f[0]; __pyx_lineno = 95; goto __pyx_L1;}
      Py_DECREF(__pyx_v_chunk);
      __pyx_v_chunk = __pyx_2;
      __pyx_2 = 0;

      /* "/home/mbp/bzr/prepare-2.1/bzrlib/_chunks_to_lines_pyx.pyx":96 */
      Py_INCREF(Py_None);
      Py_DECREF(__pyx_v_tail);
      __pyx_v_tail = Py_None;
      goto __pyx_L11;
    }
    __pyx_L11:;

    /* "/home/mbp/bzr/prepare-2.1/bzrlib/_chunks_to_lines_pyx.pyx":97 */
    __pyx_3 = (!PyString_CheckExact(__pyx_v_chunk));
    if (__pyx_3) {
      __pyx_1 = PyTuple_New(1); if (!__pyx_1) {__pyx_filename = __pyx_f[0]; __pyx_lineno = 98; goto __pyx_L1;}
      Py_INCREF(__pyx_k1p);
      PyTuple_SET_ITEM(__pyx_1, 0, __pyx_k1p);
      __pyx_2 = PyObject_CallObject(PyExc_TypeError, __pyx_1); if (!__pyx_2) {__pyx_filename = __pyx_f[0]; __pyx_lineno = 98; goto __pyx_L1;}
      Py_DECREF(__pyx_1); __pyx_1 = 0;
      __Pyx_Raise(__pyx_2, 0, 0);
      Py_DECREF(__pyx_2); __pyx_2 = 0;
      {__pyx_filename = __pyx_f[0]; __pyx_lineno = 98; goto __pyx_L1;}
      goto __pyx_L12;
    }
    __pyx_L12:;

    /* "/home/mbp/bzr/prepare-2.1/bzrlib/_chunks_to_lines_pyx.pyx":99 */
    __pyx_v_the_len = PyString_GET_SIZE(__pyx_v_chunk);

    /* "/home/mbp/bzr/prepare-2.1/bzrlib/_chunks_to_lines_pyx.pyx":100 */
    __pyx_3 = (__pyx_v_the_len == 0);
    if (__pyx_3) {
      goto __pyx_L9;
      goto __pyx_L13;
    }
    __pyx_L13:;

    /* "/home/mbp/bzr/prepare-2.1/bzrlib/_chunks_to_lines_pyx.pyx":104 */
    __pyx_v_c_str = PyString_AS_STRING(__pyx_v_chunk);

    /* "/home/mbp/bzr/prepare-2.1/bzrlib/_chunks_to_lines_pyx.pyx":105 */
    __pyx_v_c_last = ((__pyx_v_c_str + __pyx_v_the_len) - 1);

    /* "/home/mbp/bzr/prepare-2.1/bzrlib/_chunks_to_lines_pyx.pyx":106 */
    __pyx_v_newline = ((char *)memchr(__pyx_v_c_str,'\n',__pyx_v_the_len));

    /* "/home/mbp/bzr/prepare-2.1/bzrlib/_chunks_to_lines_pyx.pyx":107 */
    __pyx_3 = (__pyx_v_newline == __pyx_v_c_last);
    if (__pyx_3) {
      __pyx_3 = PyList_Append(__pyx_v_lines,__pyx_v_chunk); if (__pyx_3 == (-1)) {__pyx_filename = __pyx_f[0]; __pyx_lineno = 109; goto __pyx_L1;}
      goto __pyx_L14;
    }
    __pyx_3 = (__pyx_v_newline == NULL);
    if (__pyx_3) {
      Py_INCREF(__pyx_v_chunk);
      Py_DECREF(__pyx_v_tail);
      __pyx_v_tail = __pyx_v_chunk;
      goto __pyx_L14;
    }
    /*else*/ {
      while (1) {
        __pyx_3 = (__pyx_v_newline != NULL);
        if (!__pyx_3) break;

        /* "/home/mbp/bzr/prepare-2.1/bzrlib/_chunks_to_lines_pyx.pyx":118 */
        __pyx_1 = PyString_FromStringAndSize(__pyx_v_c_str,((__pyx_v_newline - __pyx_v_c_str) + 1)); if (!__pyx_1) {__pyx_filename = __pyx_f[0]; __pyx_lineno = 118; goto __pyx_L1;}
        Py_DECREF(__pyx_v_line);
        __pyx_v_line = __pyx_1;
        __pyx_1 = 0;

        /* "/home/mbp/bzr/prepare-2.1/bzrlib/_chunks_to_lines_pyx.pyx":119 */
        __pyx_3 = PyList_Append(__pyx_v_lines,__pyx_v_line); if (__pyx_3 == (-1)) {__pyx_filename = __pyx_f[0]; __pyx_lineno = 119; goto __pyx_L1;}

        /* "/home/mbp/bzr/prepare-2.1/bzrlib/_chunks_to_lines_pyx.pyx":120 */
        __pyx_v_c_str = (__pyx_v_newline + 1);

        /* "/home/mbp/bzr/prepare-2.1/bzrlib/_chunks_to_lines_pyx.pyx":121 */
        __pyx_3 = (__pyx_v_c_str > __pyx_v_c_last);
        if (__pyx_3) {
          goto __pyx_L16;
          goto __pyx_L17;
        }
        __pyx_L17:;

        /* "/home/mbp/bzr/prepare-2.1/bzrlib/_chunks_to_lines_pyx.pyx":123 */
        __pyx_v_the_len = ((__pyx_v_c_last - __pyx_v_c_str) + 1);

        /* "/home/mbp/bzr/prepare-2.1/bzrlib/_chunks_to_lines_pyx.pyx":124 */
        __pyx_v_newline = ((char *)memchr(__pyx_v_c_str,'\n',__pyx_v_the_len));

        /* "/home/mbp/bzr/prepare-2.1/bzrlib/_chunks_to_lines_pyx.pyx":125 */
        __pyx_3 = (__pyx_v_newline == NULL);
        if (__pyx_3) {

          /* "/home/mbp/bzr/prepare-2.1/bzrlib/_chunks_to_lines_pyx.pyx":126 */
          __pyx_2 = PyString_FromStringAndSize(__pyx_v_c_str,__pyx_v_the_len); if (!__pyx_2) {__pyx_filename = __pyx_f[0]; __pyx_lineno = 126; goto __pyx_L1;}
          Py_DECREF(__pyx_v_tail);
          __pyx_v_tail = __pyx_2;
          __pyx_2 = 0;

          /* "/home/mbp/bzr/prepare-2.1/bzrlib/_chunks_to_lines_pyx.pyx":127 */
          goto __pyx_L16;
          goto __pyx_L18;
        }
        __pyx_L18:;
      }
      __pyx_L16:;
    }
    __pyx_L14:;
    __pyx_L9:;
  }
  Py_DECREF(__pyx_4); __pyx_4 = 0;

  /* "/home/mbp/bzr/prepare-2.1/bzrlib/_chunks_to_lines_pyx.pyx":128 */
  __pyx_3 = __pyx_v_tail != Py_None;
  if (__pyx_3) {
    __pyx_3 = PyList_Append(__pyx_v_lines,__pyx_v_tail); if (__pyx_3 == (-1)) {__pyx_filename = __pyx_f[0]; __pyx_lineno = 129; goto __pyx_L1;}
    goto __pyx_L19;
  }
  __pyx_L19:;

  /* "/home/mbp/bzr/prepare-2.1/bzrlib/_chunks_to_lines_pyx.pyx":130 */
  Py_INCREF(__pyx_v_lines);
  __pyx_r = __pyx_v_lines;
  goto __pyx_L0;

  __pyx_r = Py_None; Py_INCREF(Py_None);
  goto __pyx_L0;
  __pyx_L1:;
  Py_XDECREF(__pyx_1);
  Py_XDECREF(__pyx_2);
  Py_XDECREF(__pyx_4);
  __Pyx_AddTraceback("bzrlib._chunks_to_lines_pyx.chunks_to_lines");
  __pyx_r = 0;
  __pyx_L0:;
  Py_DECREF(__pyx_v_chunk);
  Py_DECREF(__pyx_v_lines);
  Py_DECREF(__pyx_v_tail);
  Py_DECREF(__pyx_v_line);
  Py_DECREF(__pyx_v_chunks);
  return __pyx_r;
}

static struct PyMethodDef __pyx_methods[] = {
  {"chunks_to_lines", (PyCFunction)__pyx_f_6bzrlib_20_chunks_to_lines_pyx_chunks_to_lines, METH_VARARGS|METH_KEYWORDS, __pyx_doc_6bzrlib_20_chunks_to_lines_pyx_chunks_to_lines},
  {0, 0, 0, 0}
};

static void __pyx_init_filenames(void); /*proto*/

PyMODINIT_FUNC init_chunks_to_lines_pyx(void); /*proto*/
PyMODINIT_FUNC init_chunks_to_lines_pyx(void) {
  __pyx_init_filenames();
  __pyx_m = Py_InitModule4("_chunks_to_lines_pyx", __pyx_methods, __pyx_mdoc, 0, PYTHON_API_VERSION);
  if (!__pyx_m) {__pyx_filename = __pyx_f[0]; __pyx_lineno = 18; goto __pyx_L1;};
  Py_INCREF(__pyx_m);
  __pyx_b = PyImport_AddModule("__builtin__");
  if (!__pyx_b) {__pyx_filename = __pyx_f[0]; __pyx_lineno = 18; goto __pyx_L1;};
  if (PyObject_SetAttrString(__pyx_m, "__builtins__", __pyx_b) < 0) {__pyx_filename = __pyx_f[0]; __pyx_lineno = 18; goto __pyx_L1;};
  if (__Pyx_InitStrings(__pyx_string_tab) < 0) {__pyx_filename = __pyx_f[0]; __pyx_lineno = 18; goto __pyx_L1;};

  /* "/home/mbp/bzr/prepare-2.1/bzrlib/_chunks_to_lines_pyx.pyx":42 */
  return;
  __pyx_L1:;
  __Pyx_AddTraceback("bzrlib._chunks_to_lines_pyx");
}

static char *__pyx_filenames[] = {
  "_chunks_to_lines_pyx.pyx",
};

/* Runtime support code */

static void __pyx_init_filenames(void) {
  __pyx_f = __pyx_filenames;
}

static void __Pyx_Raise(PyObject *type, PyObject *value, PyObject *tb) {
    Py_XINCREF(type);
    Py_XINCREF(value);
    Py_XINCREF(tb);
    /* First, check the traceback argument, replacing None with NULL. */
    if (tb == Py_None) {
        Py_DECREF(tb);
        tb = 0;
    }
    else if (tb != NULL && !PyTraceBack_Check(tb)) {
        PyErr_SetString(PyExc_TypeError,
            "raise: arg 3 must be a traceback or None");
        goto raise_error;
    }
    /* Next, replace a missing value with None */
    if (value == NULL) {
        value = Py_None;
        Py_INCREF(value);
    }
    #if PY_VERSION_HEX < 0x02050000
    if (!PyClass_Check(type))
    #else
    if (!PyType_Check(type))
    #endif
    {
        /* Raising an instance.  The value should be a dummy. */
        if (value != Py_None) {
            PyErr_SetString(PyExc_TypeError,
                "instance exception may not have a separate value");
            goto raise_error;
        }
        /* Normalize to raise <class>, <instance> */
        Py_DECREF(value);
        value = type;
        #if PY_VERSION_HEX < 0x02050000
            if (PyInstance_Check(type)) {
                type = (PyObject*) ((PyInstanceObject*)type)->in_class;
                Py_INCREF(type);
            }
            else {
                PyErr_SetString(PyExc_TypeError,
                    "raise: exception must be an old-style class or instance");
                goto raise_error;
            }
        #else
            type = (PyObject*) type->ob_type;
            Py_INCREF(type);
            if (!PyType_IsSubtype((PyTypeObject *)type, (PyTypeObject *)PyExc_BaseException)) {
                PyErr_SetString(PyExc_TypeError,
                    "raise: exception class must be a subclass of BaseException");
                goto raise_error;
            }
        #endif
    }
    PyErr_Restore(type, value, tb);
    return;
raise_error:
    Py_XDECREF(value);
    Py_XDECREF(type);
    Py_XDECREF(tb);
    return;
}

static int __Pyx_InitStrings(__Pyx_StringTabEntry *t) {
    while (t->p) {
        *t->p = PyString_FromStringAndSize(t->s, t->n - 1);
        if (!*t->p)
            return -1;
        if (t->i)
            PyString_InternInPlace(t->p);
        ++t;
    }
    return 0;
}

#include "compile.h"
#include "frameobject.h"
#include "traceback.h"

static void __Pyx_AddTraceback(char *funcname) {
    PyObject *py_srcfile = 0;
    PyObject *py_funcname = 0;
    PyObject *py_globals = 0;
    PyObject *empty_tuple = 0;
    PyObject *empty_string = 0;
    PyCodeObject *py_code = 0;
    PyFrameObject *py_frame = 0;
    
    py_srcfile = PyString_FromString(__pyx_filename);
    if (!py_srcfile) goto bad;
    py_funcname = PyString_FromString(funcname);
    if (!py_funcname) goto bad;
    py_globals = PyModule_GetDict(__pyx_m);
    if (!py_globals) goto bad;
    empty_tuple = PyTuple_New(0);
    if (!empty_tuple) goto bad;
    empty_string = PyString_FromString("");
    if (!empty_string) goto bad;
    py_code = PyCode_New(
        0,            /*int argcount,*/
        0,            /*int nlocals,*/
        0,            /*int stacksize,*/
        0,            /*int flags,*/
        empty_string, /*PyObject *code,*/
        empty_tuple,  /*PyObject *consts,*/
        empty_tuple,  /*PyObject *names,*/
        empty_tuple,  /*PyObject *varnames,*/
        empty_tuple,  /*PyObject *freevars,*/
        empty_tuple,  /*PyObject *cellvars,*/
        py_srcfile,   /*PyObject *filename,*/
        py_funcname,  /*PyObject *name,*/
        __pyx_lineno,   /*int firstlineno,*/
        empty_string  /*PyObject *lnotab*/
    );
    if (!py_code) goto bad;
    py_frame = PyFrame_New(
        PyThreadState_Get(), /*PyThreadState *tstate,*/
        py_code,             /*PyCodeObject *code,*/
        py_globals,          /*PyObject *globals,*/
        0                    /*PyObject *locals*/
    );
    if (!py_frame) goto bad;
    py_frame->f_lineno = __pyx_lineno;
    PyTraceBack_Here(py_frame);
bad:
    Py_XDECREF(py_srcfile);
    Py_XDECREF(py_funcname);
    Py_XDECREF(empty_tuple);
    Py_XDECREF(empty_string);
    Py_XDECREF(py_code);
    Py_XDECREF(py_frame);
}
