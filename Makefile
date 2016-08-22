CC = gcc
#CC = clang

CFLAGS = -Wall -Wextra -std=c11 -O2 -Wformat=2 -fstack-protector-all -fPIE -fdiagnostics-color=always -Wstrict-prototypes -Wunreachable-code  -Wwrite-strings -Wpointer-arith -Wbad-function-cast -Wcast-qual -Wcast-align $(shell pkg-config --cflags glib-2.0 gio-2.0)

DFLAGS = -D_FILE_OFFSET_BITS=64 -U_FORTIFY_SOURCE -D_FORTIFY_SOURCE=2

NOFLAGS = -Wno-missing-field-initializers -Wno-return-type -Wno-cast-qual

LDFLAGS = -Wl,-z,now -Wl,-z,relro

LIBS = -lgcrypt -lsqlite3 $(shell pkg-config --libs glib-2.0 gio-2.0)

SOURCES = $(wildcard src/*.c)
OBJS = ${SOURCES:.c=.o}

PROG = dupfinder

.SUFFIXES:.c .o

.c.o:
	$(CC) -c $(CFLAGS) $(NOFLAGS) $(DFLAGS) $< -o $@

all: $(PROG)


$(PROG) : $(OBJS)
	$(CC) $(CFLAGS) $(NOFLAGS) $(DFLAGS) $(OBJS) -o $@ $(LIBS)


.PHONY: clean

clean :
	rm -f $(PROG) $(OBJS)
