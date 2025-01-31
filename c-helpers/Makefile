py-dir := $(shell python3-config --includes | sed 's/-I//g' | tr ' ' '\n' | grep -m1 "python")
includes = -I.. -I../LiE -I../LiE/box -I. $(shell python3-config --includes)
all-C-flags:= -std=c99 $(includes) $(CFLAGS) -fPIC
CC = gcc

.SUFFIXES:

%.o: %.c
	$(CC) -c $(CPPFLAGS) $(all-C-flags) $<

objects=from-static1.o from-static2.o from-static3.o error.o gc.o

.PHONY: all clean

all:	.last_compiled

.last_compiled:	$(objects)
	touch .last_compiled

clean:
	rm -f *~ *.o .last_compiled
