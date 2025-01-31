#include <stdarg.h>
#include <stdio.h>
#include "error_handlers.h"

/* Python-specific error handling */
void py_error(char *format, ...) {
  va_list ap;
  char str[128];

  va_start(ap, format);
  vsprintf(str, format, ap);
  va_end(ap);
  
  printf("%s", str);
  /* Set Python error and return */
  PyErr_SetString(PyExc_RuntimeError, str);
  /* Don't raise SIGFPE, let Python handle the error */
  return;
}