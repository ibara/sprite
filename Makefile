# sprite Makefile

CC ?=		cc
CFLAGS ?=	-O2 -pipe

PREFIX ?=	/usr/local
BINDIR ?=	${PREFIX}/bin
MANDIR ?=	${PREFIX}/man/man1

PROG =	sprite
OBJS =	sprite.o

all: ${OBJS}
	${CC} ${LDFLAGS} -o ${PROG} ${OBJS} -lcurses

install:
	install -d -m 755 ${BINDIR}
	install -d -m 755 ${MANDIR}
	install -c -s -m 555 ${PROG} ${BINDIR}
	install -c -m 444 ${PROG}.1 ${MANDIR}

clean:
	rm -f ${PROG} ${OBJS} ${PROG}.core
