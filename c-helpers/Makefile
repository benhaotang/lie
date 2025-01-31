# Get Python include paths
PY_INCLUDES := $(shell python3-config --includes)
INCLUDES = -I.. -I../LiE -I../LiE/box -I.
CC = gcc

# Default flags for LiE (C90)
LIE_CFLAGS = -std=c90 $(INCLUDES) $(CFLAGS) -fPIC

# Python flags can be overridden from parent Makefile
PY_CFLAGS ?= -std=c99 $(INCLUDES) $(PY_INCLUDES) $(CFLAGS) -fPIC

.SUFFIXES:

# Objects for LiE (non-Python)
lie_objects=from-static1.o from-static2.o from-static3.o gc.o

# Objects for Python extension
py_objects=py_error.o error.o

.PHONY: all clean

# Build Python error handlers
py_error.o: py_error.c error_handlers.h
	$(CC) -c $(CPPFLAGS) $(PY_CFLAGS) $(PY_INCLUDES) $<

error.o: error.c error_handlers.h
	$(CC) -c $(CPPFLAGS) $(PY_CFLAGS) $(PY_INCLUDES) $<

# Build LiE objects
%.o: %.c
	$(CC) -c $(CPPFLAGS) $(LIE_CFLAGS) $<

all: $(py_objects) $(lie_objects)

# Regular C files (C90)
from-static1.o: from-static1.c
	$(CC) -c $(CPPFLAGS) $(LIE_CFLAGS) $<

from-static2.o: from-static2.c
	$(CC) -c $(CPPFLAGS) $(LIE_CFLAGS) $<

from-static3.o: from-static3.c
	$(CC) -c $(CPPFLAGS) $(LIE_CFLAGS) $<

error.o: error.c
	$(CC) -c $(CPPFLAGS) $(LIE_CFLAGS) $<

gc.o: gc.c
	$(CC) -c $(CPPFLAGS) $(LIE_CFLAGS) $<

# Python-specific files (C99)
py_error.o: py_error.c
	$(CC) -c $(CPPFLAGS) $(PY_CFLAGS) $(PY_INCLUDES) $<

clean:
	rm -f *~ *.o
