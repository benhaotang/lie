#include "lie-py.h"
#include <signal.h>

/* this function replaces LiE's error function in output.c */
void error(char *format, ...) {
  va_list ap;
  char str[128];

  va_start(ap, format);
  vsprintf(str, format, ap);
  va_end(ap);
  
  printf("%s", str);
  /* Raise Python error and signal */
  PyErr_SetString(PyExc_RuntimeError, str);
  raise(SIGFPE);
}
