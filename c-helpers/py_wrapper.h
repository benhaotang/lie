/* This wrapper handles Python.h C99 features in C90 mode */
#ifdef __STDC__
#undef __STDC__
#include "Python.h"
#define __STDC__ 1
#else
#include "Python.h"
#endif