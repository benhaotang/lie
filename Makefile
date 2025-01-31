srcdir := $(shell pwd)
Chelpers := c-helpers
LiE := LiE

CFLAGS= -g
py-dir := $(shell python3-config --includes | sed 's/-I//g' | tr ' ' '\n' | grep -m1 "python")
includes = -I$(srcdir) -I$(LiE) -I$(LiE)/box -I$(Chelpers) $(shell python3-config --includes)
all-C-flags:= -std=c99 $(includes) $(CFLAGS) -fPIC
non-ansi-flags := $(includes) $(CFLAGS)
py-link-flags = -shared -fPIC
py-libs := $(shell python3-config --libs) -lpython3.12 -lreadline -lpthread -ltinfo
CC = gcc
common_objects=$(LiE)/lexer.o $(LiE)/parser.o\
 $(LiE)/non-ANSI.o $(LiE)/bigint.o $(LiE)/binmat.o $(LiE)/creatop.o\
 $(LiE)/gettype.o $(LiE)/getvalue.o $(LiE)/init.o $(LiE)/learn.o\
 $(LiE)/main.o $(LiE)/mem.o $(LiE)/node.o $(LiE)/onoff.o $(LiE)/output.o\
 $(LiE)/poly.o $(LiE)/sym.o $(LiE)/date.o\

LiE_objects=$(common_objects) $(LiE)/print.o $(LiE)/getl.o $(LiE)/static/*.o $(LiE)/box/*.o

all: lie.so

# Get absolute paths
CURDIR := $(shell pwd)
CHELPERS_ABS := $(CURDIR)/$(Chelpers)

# Define c-helpers object files with absolute paths
CHELPERS_LIE_OBJECTS = $(CHELPERS_ABS)/from-static1.o $(CHELPERS_ABS)/from-static2.o $(CHELPERS_ABS)/from-static3.o $(CHELPERS_ABS)/gc.o
CHELPERS_PY_OBJECTS = $(CHELPERS_ABS)/py_error.o $(CHELPERS_ABS)/error.o

# Get Python include paths
PY_INCLUDES := $(shell python3-config --includes)

# C flags for different parts
LIE_CFLAGS = -std=c90 $(CFLAGS)
PY_CFLAGS = -std=c99 $(PY_INCLUDES) $(CFLAGS)

# Build Python objects first
$(CHELPERS_PY_OBJECTS):
	$(MAKE) -C $(Chelpers) error.o py_error.o
	test -f $(CHELPERS_ABS)/error.o
	test -f $(CHELPERS_ABS)/py_error.o

# Then build LiE objects
$(CHELPERS_LIE_OBJECTS): $(CHELPERS_PY_OBJECTS)
	$(MAKE) -C $(Chelpers) all
	test -f $(CHELPERS_ABS)/from-static1.o
	test -f $(CHELPERS_ABS)/from-static2.o
	test -f $(CHELPERS_ABS)/from-static3.o
	test -f $(CHELPERS_ABS)/gc.o

# Build LiE with non-Python objects first
$(LiE)/Lie.exe: $(CHELPERS_LIE_OBJECTS)
	$(MAKE) -C LiE math_functions
	$(MAKE) -C LiE binding_functions
	$(MAKE) -C LiE date.o
	$(MAKE) -C LiE Lie.exe CHELPERS_PATH=$(shell pwd)/$(Chelpers) \
		CHELPERS_OBJECTS="$(CHELPERS_LIE_OBJECTS)"

# Then add Python objects
$(LiE)/Lie.exe: $(CHELPERS_PY_OBJECTS)
	$(CC) -o $(LiE)/Lie.exe $(LiE_objects) $(LiE)/date.o $(LiE)/static/*.o $(LiE)/box/*.o \
		$(CHELPERS_LIE_OBJECTS) $(CHELPERS_PY_OBJECTS) -lreadline -lpthread $(py-libs)

# First build LiE without Python dependencies
$(LiE)/Lie.exe: $(CHELPERS_LIE_OBJECTS)
	$(MAKE) -C LiE math_functions
	$(MAKE) -C LiE binding_functions
	$(MAKE) -C LiE date.o
	$(MAKE) -C LiE Lie.exe CHELPERS_PATH=$(shell pwd)/$(Chelpers) CHELPERS_OBJECTS="$(CHELPERS_LIE_OBJECTS)"

# Generate C code from Cython
lie.c: lie.pyx
	cython lie.pyx

# Common include paths
LIE_INCLUDES = -I$(LiE) -I$(LiE)/box
ALL_INCLUDES = $(PY_INCLUDES) $(LIE_INCLUDES)

# Build Python extension with C99
lie.o: lie.c $(LiE)/Lie.exe $(LiE)/INFO.a
	$(CC) -c $(CPPFLAGS) -std=c99 $(ALL_INCLUDES) $(CFLAGS) -fPIC -o lie.o lie.c

# Build Python-specific c-helpers with C99
$(CHELPERS_PY_OBJECTS): $(CHELPERS_LIE_OBJECTS)
	$(MAKE) -C $(Chelpers) py_error.o PY_CFLAGS="-std=c99 $(ALL_INCLUDES) $(CFLAGS) -fPIC"

# Finally build Python extension
lie.so: lie.o $(CHELPERS_PY_OBJECTS) $(CHELPERS_LIE_OBJECTS)
	$(CC) $(py-link-flags) -std=c99 $(ALL_INCLUDES) -o lie.so $(LiE_objects) lie.o $(CHELPERS_LIE_OBJECTS) $(CHELPERS_PY_OBJECTS) $(py-libs)

$(LiE)/INFO.a: $(LiE)/Lie.exe
	cd $(LiE) && ./Lie.exe < progs/maxsub

clean:
	$(MAKE) -C LiE clean
	$(MAKE) -C c-helpers clean
	rm -f *~ *.o *.so INFO.* lie.c
