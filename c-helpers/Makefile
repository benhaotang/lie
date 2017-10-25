py-dir = /usr/local/Cellar/python3/3.6.1/Frameworks/Python.framework/Versions/3.6
includes = -I.. -I../LiE -I../LiE/box -I. -I$(py-dir)/include/python3.6m
sl-flags= -m64 -isysroot /Applications/Xcode.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX10.12.sdk -mmacosx-version-min=10.5
all-C-flags:= -ansi -D_ANSI_SOURCE $(includes) $(CFLAGS) $(sl-flags) -fPIC
$(CC) = gcc

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
