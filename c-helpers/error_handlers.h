#ifndef ERROR_HANDLERS_H
#define ERROR_HANDLERS_H

/* Forward declarations for Python API */
struct _object;
typedef struct _object PyObject;
extern PyObject* PyExc_RuntimeError;
extern void PyErr_SetString(PyObject *type, const char *message);

/* Error handling functions */
void error(char *format, ...);
void py_error(char *format, ...);

#endif /* ERROR_HANDLERS_H */