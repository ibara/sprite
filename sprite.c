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

	for (i = 4; i < 20; i++) {
		if (i % 4 == 0)
			k = switch_color(k);
		for (j = 32; j < 48; j++) {
			if (j % 4 == 0)
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

	for (i = 4; i < 20; i++) {
		for (j = 32; j < 48; j++) {
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

	for (i = 0; i < 16; i++) {
		for (j = 0; j < 16; j++) {
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

	move(2, 35);
	printw("Sprite 1.4");

	move(4, 50);
	printw("Key commands");
	move(5, 50);
	printw("============");
	move(6, 50);
	printw("Arrow keys: move");
	move(7, 50);
	printw("Spacebar: draw pixel");
	move(8, 50);
	printw("c: change color");
	move(9, 50);
	printw("d: delete pixel");
	move(10, 50);
	printw("e: export");
	move(11, 50);
	printw("s: save");
	move(12, 50);
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
	for (i = 0; i < 16; i++)
		addch(ACS_HLINE);
	addch(ACS_URCORNER);

	for (i = 4; i < 20; i++) {
		move(i, 31);
		addch(ACS_VLINE);
		move(i, 48);
		addch(ACS_VLINE);
	}

	move(i, 31);

	addch(ACS_LLCORNER);
	for (i = 0; i < 16; i++)
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

	move(21, 31);
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
file_export(int y, int x)
{
	FILE *fp;
	char buf[PATH_MAX];
	int code, height = 16, i, j, k = 0, width = 16;
	png_structp png_ptr = NULL;
	png_infop info_ptr = NULL;
	png_byte row[64];
	png_text title_text;

	memset(buf, 0, sizeof(buf));

	move(21, 31);
	printw("Name: ");
	echo();
	getnstr(buf, sizeof(buf) - 1);
	noecho();

	if ((fp = fopen(buf, "w+")) == NULL) {
		move(21, 31);
		printw("Error: could not open %s for writing");
		goto out;
	}

	if ((png_ptr = png_create_write_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL)) == NULL) {
		move(21, 31);
		printw("Error: could not allocate png write struct");
		(void) fclose(fp);
		goto out;
	}

	if ((info_ptr = png_create_info_struct(png_ptr)) == NULL) {
		move(21, 31);
		printw("Error: could not allocate png info struct");
		(void) fclose(fp);
		goto out;
	}

	if (setjmp(png_jmpbuf(png_ptr))) {
		move(21, 31);
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
			row[k++] = (colors[pixel[i][j].color] >> 16) & 0xff;
			row[k++] = (colors[pixel[i][j].color] >> 8) & 0xff;
			row[k++] = colors[pixel[i][j].color] & 0xff;
			if (pixel[i][j].color == -1)
				row[k++] = 0;
			else
				row[k++] = 0xff;
		}
		png_write_row(png_ptr, row);
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

	move(21, 31);
	printw("Name: ");
	echo();
	getnstr(buf, sizeof(buf) - 1);
	noecho();

	if ((fp = fopen(buf, "w+")) == NULL) {
		move(21, 31);
		printw("Error: could not open %s for writing");
		goto out;
	}

	for (i = 0; i < 16; i++) {
		for (j = 0; j < 16; j++) {
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

		y = strtonum(ybuf, 0, 15, &errstr);
		if (errstr != NULL)
			return;

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
	move(21, 31);
	printw("Save? [y/n]: ");
	echo();
	c = getch();
	noecho();

	move(21, 31);
	printw("              ");

	if (c == 'Y' || c == 'y')
		file_save(y, x);
	else if (c != 'N' && c != 'n')
		goto again;
}

static void
main_loop(void)
{
	int c, color = 0, dirty = 0, loop = 1, o, x = 39, y = 11;

	attron(COLOR_PAIR(color));
	mvaddch(y, x, ' ');
	attroff(COLOR_PAIR(color));
	refresh();

	while (loop) {
		switch ((c = getch())) {
		case KEY_UP:
		case 'K':
		case 'k':
			if (--y < 4)
				y = 4;
			break;
		case KEY_DOWN:
		case 'J':
		case 'j':
			if (++y > 19)
				y = 19;
			break;
		case KEY_LEFT:
		case 'H':
		case 'h':
			if (--x < 32)
				x = 32;
			break;
		case KEY_RIGHT:
		case 'L':
		case 'l':
			if (++x > 47)
				x = 47;
			break;
		case ' ':
			pixel[y - 4][x - 32].color = color;
			dirty = 1;
			break;
		case 'C':
		case 'c':
			color = change_color(y, x, color);
			break;
		case 'D':
		case 'd':
			pixel[y - 4][x - 32].color = -1;
			break;
		case 'E':
		case 'e':
			file_export(y, x);
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

int
main(int argc, char *argv[])
{

	if (argc > 2) {
		fprintf(stderr, "usage: %s [file]\n", getprogname());
		exit(1);
	}

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

	if (argc == 2)
		file_open(argv[1]);

	main_loop();

	endwin();

	return 0;
}
