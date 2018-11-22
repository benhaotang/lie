srcdir := $(shell pwd)
Chelpers := c-helpers
LiE := LiE

CFLAGS= -g
py-dir = /usr/local/Cellar/python3/3.7.1/Frameworks/Python.framework/Versions/3.7
includes = -I$(srcdir) -I$(LiE) -I$(LiE)/box -I$(Chelpers) -I$(py-dir)/include/python3.7m
all-C-flags:= -ansi -D_ANSI_SOURCE $(includes) $(CFLAGS) -fPIC
non-ansi-flags :=  $(includes) $(CFLAGS)
py-link-flags = -shared -dynamiclib -lreadline -lpython3.7 -L$(py-dir)/lib/python3.7/config-3.7m-darwin
SnowLeopard-flags= -m64 -isysroot /Applications/Xcode.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX10.14.sdk -mmacosx-version-min=10.14

CC = gcc $(SnowLeopard-flags) # some compiler for ANSI/ISO C

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
	$(CC) $(py-link-flags) -o lie.so $(LiE_objects) lie.o $(Chelpers)/*.o

clean:
	$(MAKE) -C c-helpers clean
	rm -f *~ *.o *.so

lie.o: lie.pyx
	cython lie.pyx $(fixed-flags)
	$(CC) -c $(all-C-flags) lie.c
