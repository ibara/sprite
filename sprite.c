/*
 * Copyright (c) 2020 Brian Callahan <bcallah@openbsd.org>
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

#include <curses.h>
#include <err.h>
#include <limits.h>
#include <png.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "colors.h"
#include "sprite.h"

static char extended;

/*
 * color -1 == transparent
 */
typedef struct pixel {
	char x;
	char y;
	int color;
} pixel_t;
static pixel_t pixel[32][32];

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

	for (i = 4; i < 20 + (extended ? 16 : 0); i++) {
		if ((i - 4) % (extended ? 8 : 4) == 0)
			k = switch_color(k);
		for (j = 32; j < 48 + (extended ? 16 : 0); j++) {
			if (j % (extended ? 8: 4) == 0)
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

	for (i = 4; i < 20 + (extended ? 16 : 0); i++) {
		for (j = 32; j < 48 + (extended ? 16 : 0); j++) {
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

	for (i = 0; i < 16 + (extended ? 16 : 0); i++) {
		for (j = 0; j < 16 + (extended ? 16 : 0); j++) {
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

	for (i = 0; i < 256; i++)
		init_pair(i, i, i);
}

static void
instructions(void)
{
	int i;

	move(2, 35 + (extended ? 8 : 0));
	printw("Sprite 1.6");

	move(4, 50 + (extended ? 16 : 0));
	printw("Key commands");
	move(5, 50 + (extended ? 16 : 0));
	printw("============");
	move(6, 50 + (extended ? 16 : 0));
	printw("Arrow keys: move");
	move(7, 50 + (extended ? 16 : 0));
	printw("Spacebar: draw pixel");
	move(8, 50 + (extended ? 16 : 0));
	printw("/: toggle spacebar lock");
	move(9, 50 + (extended ? 16 : 0));
	printw("c: change color");
	move(10, 50 + (extended ? 16 : 0));
	printw("d: delete pixel");
	move(11, 50 + (extended ? 16 : 0));
	printw("f: fill region");
	move(13, 50 + (extended ? 16 : 0));
	printw("e: export to PNG");
	move(14, 50 + (extended ? 16 : 0));
	printw("s: save");
	move(15, 50 + (extended ? 16 : 0));
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
	int i;

	if (has_colors() == FALSE) {
		endwin();
		errx(1, "sprite requires color support");
	}

	start_color();
	init_colors();

	move(3, 31);

	addch(ACS_ULCORNER);
	for (i = 0; i < 16 + (extended ? 16 : 0); i++)
		addch(ACS_HLINE);
	addch(ACS_URCORNER);

	for (i = 4; i < 20 + (extended ? 16 : 0); i++) {
		move(i, 31);
		addch(ACS_VLINE);
		move(i, 48 + (extended ? 16 : 0));
		addch(ACS_VLINE);
	}

	move(i, 31);

	addch(ACS_LLCORNER);
	for (i = 0; i < 16 + (extended ? 16 : 0); i++)
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

	move(21 + (extended ? 16 : 0), 31);
	printw("Color [0-255]: ");
	echo();
	getnstr(buf, sizeof(buf) - 1);
	noecho();

	clear();
	scrinit();
	color_panel();
	instructions();

	move(y, x);

	new_color = strtonum(buf, 0, 255, &errstr);
	if (errstr != NULL)
		return color;

	return new_color;
}

static void
fill_region(int y, int x, int color, int target)
{

	if (y < 4 || y > (extended ? 36 : 20))
		return;
	if (x < 32 || x > (extended ? 64 : 48))
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
file_export(int y, int x)
{
	FILE *fp;
	char buf[PATH_MAX];
	int code, height, i, j, k = 0, width;
	png_structp png_ptr = NULL;
	png_infop info_ptr = NULL;
	png_byte row[64], extended_row[128];
	png_text title_text;

	height = (extended ? 32 : 16);
	width = (extended ? 32 : 16);

	memset(buf, 0, sizeof(buf));

	move(21 + (extended ? 16 : 0), 31);
	printw("Name: ");
	echo();
	getnstr(buf, sizeof(buf) - 1);
	noecho();

	if ((fp = fopen(buf, "w+")) == NULL) {
		move(21 + (extended ? 16 : 0), 31);
		printw("Error: could not open %s for writing", buf);
		goto out;
	}

	if ((png_ptr = png_create_write_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL)) == NULL) {
		move(21 + (extended ? 16 : 0), 31);
		printw("Error: could not allocate png write struct");
		(void) fclose(fp);
		goto out;
	}

	if ((info_ptr = png_create_info_struct(png_ptr)) == NULL) {
		move(21 + (extended ? 16 : 0), 31);
		printw("Error: could not allocate png info struct");
		(void) fclose(fp);
		goto out;
	}

	if (setjmp(png_jmpbuf(png_ptr))) {
		move(21 + (extended ? 16 : 0), 31);
		printw("Error: could not create png");
		(void) fclose(fp);
		goto out;
	}

	png_init_io(png_ptr, fp);

	png_set_IHDR(png_ptr, info_ptr, width, height, 8, PNG_COLOR_TYPE_RGB_ALPHA, PNG_INTERLACE_NONE, PNG_COMPRESSION_TYPE_BASE, PNG_FILTER_TYPE_BASE);

	title_text.compression = PNG_TEXT_COMPRESSION_NONE;
	title_text.key = "Title";
	title_text.text = buf;
	png_set_text(png_ptr, info_ptr, &title_text, 1);

	png_write_info(png_ptr, info_ptr);

	for (i = 0; i < height; i++) {
		k = 0;
		for (j = 0; j < width; j++) {
			if (extended) {
				extended_row[k++] = (colors[pixel[i][j].color] >> 16) & 0xff;
				extended_row[k++] = (colors[pixel[i][j].color] >> 8) & 0xff;
				extended_row[k++] = colors[pixel[i][j].color] & 0xff;
				if (pixel[i][j].color == -1)
					extended_row[k++] = 0;
				else
					extended_row[k++] = 0xff;
			} else {
				row[k++] = (colors[pixel[i][j].color] >> 16) & 0xff;
				row[k++] = (colors[pixel[i][j].color] >> 8) & 0xff;
				row[k++] = colors[pixel[i][j].color] & 0xff;
				if (pixel[i][j].color == -1)
					row[k++] = 0;
				else
					row[k++] = 0xff;
			}
		}
		if (extended) {
			png_write_row(png_ptr, extended_row);
		} else {
			png_write_row(png_ptr, row);
		}
	}
	png_write_end(png_ptr, NULL);

	(void) fclose(fp);

out:
	if (info_ptr != NULL)
		png_free_data(png_ptr, info_ptr, PNG_FREE_ALL, -1);
	if (png_ptr != NULL)
		png_destroy_write_struct(&png_ptr, NULL);

	clear();
	scrinit();
	color_panel();
	instructions();

	move(y, x);
}

static void
file_save(int y, int x)
{
	FILE *fp;
	char buf[PATH_MAX];
	int i, j;

	memset(buf, 0, sizeof(buf));

	move(21 + (extended ? 16 : 0), 31);
	printw("Name: ");
	echo();
	getnstr(buf, sizeof(buf) - 1);
	noecho();

	if ((fp = fopen(buf, "w+")) == NULL) {
		move(21, 31);
		printw("Error: could not open %s for writing", buf);
		goto out;
	}

	for (i = 0; i < 16 + (extended ? 16 : 0); i++) {
		for (j = 0; j < 16 + (extended ? 16 : 0); j++) {
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

		if (extended)
			y = strtonum(ybuf, 0, 31, &errstr);
		else
			y = strtonum(ybuf, 0, 15, &errstr);
		if (errstr != NULL)
			return;

		if (extended)
			x = strtonum(xbuf, 0, 31, &errstr);
		else
			x = strtonum(xbuf, 0, 15, &errstr);
		if (errstr != NULL)
			return;

		color = strtonum(colorbuf, 0, 255, &errstr);
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
	move(21 + (extended ? 16 : 0), 31);
	printw("Save? [y/n]: ");
	echo();
	c = getch();
	noecho();

	move(21 + (extended ? 16 : 0), 31);
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

	x = (extended ? 47 : 39);
	y = (extended ? 19 : 11);

	attron(COLOR_PAIR(color));
	mvaddch(y, x, ' ');
	attroff(COLOR_PAIR(color));
	refresh();

	while (loop) {
		switch ((c = getch())) {
		case '/':
			if ((lock = !lock))
				goto print;
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
			if (++y > (extended ? 35: 19))
				y = (extended ? 35: 19);
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
			if (++x > (extended ? 63 : 47))
				x = (extended ? 63 : 47);
			if (lock)
				goto print;
			break;
		case ' ':
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
			pixel[y - 4][x - 32].color = -1;
			break;
		case 'E':
		case 'e':
			file_export(y, x);
			break;
		case 'F':
		case 'f':
			fill_region(y, x, color, pixel[y - 4][x - 32].color);
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
		}

		draw_screen(y, x, color);
	}
}

static void
usage(void)
{

	fprintf(stderr, "usage: %s [-e] [file]\n", getprogname());

	exit(1);
}

int
main(int argc, char *argv[])
{
	int ch;

	while ((ch = getopt(argc, argv, "e")) != -1) {
		switch (ch) {
		case 'e':
			extended = 1;
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

	if (argc == 1)
		file_open(*argv);

	main_loop();

	endwin();

	return 0;
}
