/*
 * Copyright (c) 2020-2021 Brian Callahan <bcallah@openbsd.org>
 *
 * Permission to use, copy, modify, and distribute this software for any
 * purpose with or without fee is hereby granted, provided that the above
 * copyright notice and this permission notice appear in all copies.
 *
 * THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
 * WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
 * ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
 * WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
 * ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
 * OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 */

/*
 * sprite -- console sprite editor
 */

#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "curses.h"
#include "sprite.h"

static char small;

/*
 * color -1 == transparent
 */
typedef struct pixel {
	char x;
	char y;
	int color;
} pixel_t;
static pixel_t pixel[16][16];

static int
switch_color(int color)
{

	if (color == 7)
		return 8;

	return 7;
}

static void
draw_transparency(void)
{
	int i, j, k = 7;

	for (i = 4; i < 20 + (small ? -8 : 0); i++) {
		if ((i - 4) % (small ? 2 : 4) == 0)
			k = switch_color(k);
		for (j = 32; j < 48 + (small ? -8 : 0); j++) {
			if (j % (small ? 2 : 4) == 0)
				k = switch_color(k);
			attron(COLOR_PAIR(k));
			mvaddch(i, j, ' ');
			attroff(COLOR_PAIR(k));
		}
	}
}

static void
draw_screen(int y, int x, int color)
{
	int i, j;

	draw_transparency();

	for (i = 4; i < 20 + (small ? -8 : 0); i++) {
		for (j = 32; j < 48 + (small ? -8 : 0); j++) {
			if (pixel[i - 4][j - 32].color != -1) {
				attron(COLOR_PAIR(pixel[i - 4][j - 32].color));
				mvaddch(i, j, ' ');
				attroff(COLOR_PAIR(pixel[i - 4][j - 32].color));
			}
		}
	}

	attron(COLOR_PAIR(color));
	mvaddch(y, x, ' ');
	attroff(COLOR_PAIR(color));

	refresh();
}

static void
init_pixels(void)
{
	int i, j;

	for (i = 0; i < 16 + (small ? -8 : 0); i++) {
		for (j = 0; j < 16 + (small ? -8 : 0); j++) {
			pixel[i][j].x = j;
			pixel[i][j].y = i;
			pixel[i][j].color = -1;
		}
	}
}

static void
init_colors(void)
{
	int i;

	i = 0;
	init_pair(0, i, i);
	i = 4;
	init_pair(1, i, i);
	i = 2;
	init_pair(2, i, i);
	i = 6;
	init_pair(3, i, i);
	i = 1;
	init_pair(4, i, i);
	i = 5;
	init_pair(5, i, i);
	i = 3;
	init_pair(6, i, i);
	i = 7;
	init_pair(7, i, i);
	i = 8;
	init_pair(8, i, i);
	i = 12;
	init_pair(9, i, i);
	i = 10;
	init_pair(10, i, i);
	i = 14;
	init_pair(11, i, i);
	i = 9;
	init_pair(12, i, i);
	i = 13;
	init_pair(13, i, i);
	i = 11;
	init_pair(14, i, i);
	i = 15;
	init_pair(15, i, i);
}

static void
instructions(void)
{
	int i;

	move(2, 35);
	printw("Sprite 1.8");

	move(4, 50);
	printw("Key commands");
	move(5, 50);
	printw("============");
	move(6, 50);
	printw("Arrow keys: move");
	move(7, 50);
	printw("Spacebar: draw pixel");
	move(8, 50);
	printw("/: toggle spacebar lock");
	move(9, 50);
	printw("c: change color");
	move(10, 50);
	printw("d: delete pixel");
	move(11, 50);
	printw("f: fill region");
	move(12, 50);
	printw("u: undo");
	move(15, 50);
	printw("s: save");
	move(16, 50);
	printw("q: quit");
}

static void
color_panel(void)
{
	int i, j, k = 0;

	move(4, 14);
	for (i = 4; i < 20; i++) {
		for (j = 14; j < 30; j++) {
			attron(COLOR_PAIR(k));
			mvaddch(i, j, ' ');
			attroff(COLOR_PAIR(k++));
		}
	}
}

static void
scrinit(void)
{
	int i, x, y;

	if (has_colors() == FALSE) {
		endwin();
		fprintf(stderr, "sprite requires color support\n");
		exit(1);
	}

	start_color();
	init_colors();

	getmaxyx(stdscr, y, x);
	if (y < 23 || x < 74) {
		endwin();
		fprintf(stderr, "terminal too small!\n");
		exit(1);
	}

	move(3, 31);

	addch(ACS_ULCORNER);
	for (i = 0; i < 16 + (small ? -8 : 0); i++)
		addch(ACS_HLINE);
	addch(ACS_URCORNER);

	for (i = 4; i < 20 + (small ? -8 : 0); i++) {
		move(i, 31);
		addch(ACS_VLINE);
		move(i, 48 + (small ? -8 : 0));
		addch(ACS_VLINE);
	}

	move(i, 31);

	addch(ACS_LLCORNER);
	for (i = 0; i < 16 + (small ? -8 : 0); i++)
		addch(ACS_HLINE);
	addch(ACS_LRCORNER);
}

static int
change_color(int y, int x, int color)
{
	const char *errstr;
	char buf[4];
	int i, new_color;

	memset(buf, 0, sizeof(buf));

	move(21 + (small ? -8 : 0), 31);
	printw("Color [0-15]: ");
	echo();
	getnstr(buf, sizeof(buf) - 1);
	noecho();

	clear();
	scrinit();
	color_panel();
	instructions();

	move(y, x);

	new_color = strtonum(buf, 0, 15, &errstr);
	if (errstr != NULL)
		return color;

	return new_color;
}

static void
do_undo(int go)
{
	static pixel_t temp[16][16], undo[16][16];

	if (go) {
		memset(&temp, 0, sizeof(temp));

		memmove(&temp, &pixel, sizeof(temp));
		memmove(&pixel, &undo, sizeof(pixel));
		memmove(&undo, &temp, sizeof(undo));

		return;
	}

	/* Otherwise, update the buffer */
	memmove(&undo, &pixel, sizeof(undo));
}

static void
fill_region(int y, int x, int color, int target)
{

	if (y < 4 || y > (small ? 12 : 20))
		return;
	if (x < 32 || x > (small ? 40 : 48))
		return;

	if (color == target)
		return;
	else if (pixel[y - 4][x - 32].color != target)
		return;
	else
		pixel[y - 4][x - 32].color = color;

	fill_region(y + 1, x, color, target);
	fill_region(y - 1, x, color, target);
	fill_region(y, x - 1, color, target);
	fill_region(y, x + 1, color, target);
}

static void
file_save(int y, int x)
{
	FILE *fp;
	char buf[PATH_MAX];
	int i, j;

	memset(buf, 0, sizeof(buf));

	move(21 + (small ? -8 : 0), 31);
	printw("Name: ");
	echo();
	getnstr(buf, sizeof(buf) - 1);
	noecho();

	if ((fp = fopen(buf, "w+")) == NULL) {
		move(21 + (small ? -8 : 0), 31);
		printw("Error: could not open %s for writing", buf);
		goto out;
	}

	for (i = 0; i < 16 + (small ? -8 : 0); i++) {
		for (j = 0; j < 16 + (small ? -8 : 0); j++) {
			if (pixel[i][j].color != -1)
				fprintf(fp, "%d,%d,%d\n", pixel[i][j].y, pixel[i][j].x, pixel[i][j].color);
		}
	}
	(void) fclose(fp);

out:
	clear();
	scrinit();
	color_panel();
	instructions();

	move(y, x);
}

static void
file_open(const char *fn)
{
	FILE *fp;
	const char *errstr;
	char xbuf[3], ybuf[3], colorbuf[4];
	int c, color, i, x, y;

	if ((fp = fopen(fn, "r")) == NULL)
		return;

	while (1) {
		i = 0;
		while ((c = fgetc(fp)) != ',') {
			if (c == EOF)
				goto out;
			ybuf[i++] = c;
			if (i == sizeof(ybuf))
				i = sizeof(ybuf) - 1;
		}
		ybuf[i] = '\0';

		i = 0;
		while ((c = fgetc(fp)) != ',') {
			if (c == EOF)
				goto out;
			xbuf[i++] = c;
			if (i == sizeof(xbuf))
				i = sizeof(xbuf) - 1;
		}
		xbuf[i] = '\0';

		i = 0;
		while ((c = fgetc(fp)) != '\n') {
			if (c == EOF)
				goto out;
			colorbuf[i++] = c;
			if (i == sizeof(colorbuf))
				i = sizeof(colorbuf) - 1;
		}
		colorbuf[i] = '\0';

		if (small)
			y = strtonum(ybuf, 0, 7, &errstr);
		else
			y = strtonum(ybuf, 0, 15, &errstr);
		if (errstr != NULL)
			return;

		if (small)
			x = strtonum(xbuf, 0, 7, &errstr);
		else
			x = strtonum(xbuf, 0, 15, &errstr);
		if (errstr != NULL)
			return;

		color = strtonum(colorbuf, 0, 15, &errstr);
		if (errstr != NULL)
			return;

		pixel[y][x].y = y;
		pixel[y][x].x = x;
		pixel[y][x].color = color;
	}

out:
	(void) fclose(fp);

	draw_screen(0, 0, 0);
}

static void
confirm_quit(int y, int x)
{
	int c;

again:
	move(21 + (small ? -8 : 0), 31);
	printw("Save? [y/n]: ");
	echo();
	c = getch();
	noecho();

	move(21 + (small ? -8 : 0), 31);
	printw("              ");

	if (c == 'Y' || c == 'y')
		file_save(y, x);
	else if (c != 'N' && c != 'n')
		goto again;
}

static void
main_loop(void)
{
	int c, color = 0, dirty = 0, lock = 0, loop = 1, o, x, y;

	x = (small ? 35 : 39);
	y = (small ? 7 : 11);

	attron(COLOR_PAIR(color));
	mvaddch(y, x, ' ');
	attroff(COLOR_PAIR(color));
	refresh();

	while (loop) {
		switch ((c = getch())) {
		case '/':
			if ((lock = !lock)) {
				do_undo(0);
				goto print;
			}
			break;
		case KEY_UP:
		case 'K':
		case 'k':
			if (--y < 4)
				y = 4;
			if (lock)
				goto print;
			break;
		case KEY_DOWN:
		case 'J':
		case 'j':
			if (++y > (small ? 11 : 19))
				y = (small ? 11 : 19);
			if (lock)
				goto print;
			break;
		case KEY_LEFT:
		case 'H':
		case 'h':
			if (--x < 32)
				x = 32;
			if (lock)
				goto print;
			break;
		case KEY_RIGHT:
		case 'L':
		case 'l':
			if (++x > (small ? 39 : 47))
				x = (small ? 39 : 47);
			if (lock)
				goto print;
			break;
		case ' ':
			do_undo(0);
print:
			pixel[y - 4][x - 32].color = color;
			dirty = 1;
			break;
		case 'C':
		case 'c':
			color = change_color(y, x, color);
			if (lock)
				goto print;
			break;
		case 'D':
		case 'd':
			do_undo(0);
			pixel[y - 4][x - 32].color = -1;
			dirty = 1;
			break;
		case 'F':
		case 'f':
			do_undo(0);
			fill_region(y, x, color, pixel[y - 4][x - 32].color);
			dirty = 1;
			break;
		case 'S':
		case 's':
			file_save(y, x);
			dirty = 0;
			break;
		case 'Q':
		case 'q':
			if (dirty == 1)
				confirm_quit(y, x);
			loop = 0;
			break;
		case 'U':
		case 'u':
			do_undo(1);
		}

		draw_screen(y, x, color);
	}
}

static void
usage(void)
{

	fprintf(stderr, "usage: sprite [-s] [file]\n");

	exit(1);
}

int
main(int argc, char *argv[])
{
	int ch;

	while ((ch = getopt(argc, argv, "s")) != -1) {
		switch (ch) {
		case 's':
			small = 1;
			break;
		default:
			usage();
		}
	}
	argc -= optind;
	argv += optind;

	if (argc > 1)
		usage();

	initscr();
	clear();
	keypad(stdscr, TRUE);
	cbreak();
	noecho();
	curs_set(0);

	scrinit();
	color_panel();
	instructions();

	init_pixels();

	draw_transparency();

	/* Set up undo structures */
	do_undo(0);

	if (argc == 1)
		file_open(*argv);

	main_loop();

	endwin();

	return 0;
}
