srcdir := $(shell pwd)
Chelpers := c-helpers
LiE := LiE

CFLAGS= -g
py-dir := $(shell python3-config --includes | sed 's/-I//g' | tr ' ' '\n' | grep -m1 "python")
includes = -I$(srcdir) -I$(LiE) -I$(LiE)/box -I$(Chelpers) $(shell python3-config --includes)
all-C-flags:= -std=c99 $(includes) $(CFLAGS) -fPIC
non-ansi-flags := $(includes) $(CFLAGS)
py-link-flags = -shared -fPIC
py-libs := $(shell python3-config --libs) -lreadline -lpython3.12 -lreadline -lpthread
CC = gcc
common_objects=$(LiE)/lexer.o $(LiE)/parser.o\
 $(LiE)/non-ANSI.o $(LiE)/bigint.o $(LiE)/binmat.o $(LiE)/creatop.o\
 $(LiE)/gettype.o $(LiE)/getvalue.o $(LiE)/init.o $(LiE)/learn.o\
 $(LiE)/main.o $(LiE)/mem.o $(LiE)/node.o $(LiE)/onoff.o $(LiE)/output.o\
 $(LiE)/poly.o $(LiE)/sym.o $(LiE)/date.o\

LiE_objects=$(common_objects) $(LiE)/print.o $(LiE)/getl.o $(LiE)/static/*.o $(LiE)/box/*.o

all:
	$(MAKE) -C LiE math_functions
	$(MAKE) -C LiE binding_functions
	$(MAKE) -C LiE date.o
	$(MAKE) -C c-helpers
	$(MAKE) lie.o
	$(CC) $(py-link-flags) -o lie.so $(LiE_objects) lie.o $(Chelpers)/*.o $(py-libs)

clean:
	$(MAKE) -C c-helpers clean
	rm -f *~ *.o *.so

lie.o: lie.pyx
	cython lie.pyx
	$(CC) -c $(all-C-flags) -Dpreprocessor lie.c
