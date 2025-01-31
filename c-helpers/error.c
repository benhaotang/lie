#include <stdio.h>
#include <stdarg.h>
#include "error_handlers.h"

/* this function replaces LiE's error function in output.c */
void error(char *format, ...) {
  va_list ap;
  char str[128];

  va_start(ap, format);
  vsprintf(str, format, ap);
  va_end(ap);
  
  printf("%s", str);
  /* Call Python error handler with the format string */
  py_error("%s", str);
}
