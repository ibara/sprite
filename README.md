sprite
======
Sprite is an ncurses-based sprite editor.
You can use it to create and share pixel art.

By default, sprite sets up a 16x16 pixel canvas.
If you'd like the larger 32x32 pixel canvas,
start sprite with the -e flag.
If your terminal is not large enough to fit the 32x32 canvas,
it will use the 16x16 canvas instead.

Requirements
------------
You need ncurses with 256 color support.
While it will work with fewer than 256 colors it will not be as good.

You also need libpng for PNG export support.

Building
--------
Just run `make`.

Remove `-DHAVE_STRTONUM` if your system needs the strtonum(3) function.

License
-------
ISC License.
See `LICENSE` for details.
