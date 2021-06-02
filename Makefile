# sprite Makefile
# Written by Brian Callahan <bcallah@openbsd.org>
# and released into the Public Domain

all
	wcl -0 -ox -ml -fe=sprite.exe sprite.c strtonum.c pdcurses.lib

clean
	-del sprite.exe
	-del sprite.err
	-del *.obj
