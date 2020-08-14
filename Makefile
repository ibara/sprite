# sprite Makefile

CC ?=		cc
CFLAGS ?=	-O2 -pipe
CFLAGS +=	-I/usr/local/include
CFLAGS +=	-DHAVE_GETPROGNAME -DHAVE_STRTONUM

PREFIX ?=	/usr/local
BINDIR ?=	${PREFIX}/bin
MANDIR ?=	${PREFIX}/man/man1

PROG =	sprite
OBJS =	getprogname.o sprite.o strtonum.o

all: ${OBJS}
	${CC} ${LDFLAGS} -o ${PROG} ${OBJS} -lcurses -L/usr/local/lib -lpng

install:
	install -d -m 755 ${BINDIR}
	install -d -m 755 ${MANDIR}
	install -c -s -m 555 ${PROG} ${BINDIR}
	install -c -m 444 ${PROG}.1 ${MANDIR}

clean:
	rm -f ${PROG} ${OBJS} ${PROG}.core
