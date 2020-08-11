# sprite Makefile

CC ?=		cc
CFLAGS ?=	-O2 -pipe

PROG =	sprite
OBJS =	sprite.o

all: ${OBJS}
	${CC} ${LDFLAGS} -o ${PROG} ${OBJS} -lcurses

clean:
	rm -f ${PROG} ${OBJS} ${PROG}.core
