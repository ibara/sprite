sprite
======
Sprite is an ncurses-based sprite editor.
You can use it to create and share pixel art.

By default, sprite sets up a 16x16 pixel canvas.
If you'd like the larger 32x32 pixel canvas,
start sprite with the -e flag.
It is the responsibility of the user to ensure the terminal is properly sized.

Requirements
------------
You need ncurses with 256 color support.
While it will work with fewer than 256 colors it will not be as good.

You also need libpng for PNG export support.

Building
--------
Just run `make`.

License
-------
ISC License.
See `LICENSE` for details.

Todo
----
* Fill command (f)
* Line drawing (hold Shift + move new location)
