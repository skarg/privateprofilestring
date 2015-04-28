#Makefile to build PrivateProfileString tests
CC      = gcc
BASEDIR = .
#CFLAGS  = -Wall -I.
# -g for debugging with gdb
#CFLAGS  = -Wall -I. -g
#CFLAGS  = -Wall -I. -DTEST -DTEST_PROFILE -g -O2
CFLAGS  = -Wall -I. -DTEST -DTEST_PROFILE -g

OBJS    = profile.o rmspace.o stptok.o ctest.o

TARGET = profile.exe

all: ${TARGET}

${TARGET}: ${OBJS}
	${CC} -o $@ ${OBJS}

.c.o:
	${CC} -c ${CFLAGS} $*.c

depend:
	rm -f .depend
	${CC} -MM ${CFLAGS} *.c >> .depend

clean:
	rm -rf core ${TARGET} $(OBJS) *.bak *.1 *.ini

include: .depend

