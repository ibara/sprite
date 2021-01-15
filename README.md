sprite
======
Sprite is an ncurses-based sprite editor.
You can use it to create and share pixel art.

By default, sprite sets up a 16x16 pixel canvas.
If you'd like a larger 32x32 pixel canvas, start sprite with the -e
flag.
If your terminal is not large enough to fit a 32x32 canvas, it will
use a 16x16 canvas instead.
If you'd like a smaller 8x8 pixel canvas, start sprite with the -s
flag.

Most terminals do not have 1:1 square character cells.
This may cause the canvas to have a different aspect ratio than
expectations.
PNG export does create standard 1:1 square pixels; exported images
will look correct.

Save file format
----------------
Images are saved as a flat text file, one pixel per line, in the form
```
y,x,color
```
Color is an index number from 0-255 that matches XTerm color.
Only pixels with color are saved; transparent pixels are left out.
The .spr and .txt file extensions are used by convention but any file
extension can be used for save files.

Images can be exported to PNG, but sprite is not able to open PNGs,
so please save images rather than export if you want to work on them
over multiple sessions. The .png file extension is recommended for
export; sprite does not add the file extension automatically when
exporting.

Requirements
------------
You need ncurses with 256 color support.
While it will work with fewer than 256 colors it will not be as good.

You also need libpng for PNG export support.

Building
--------
Just run `make`.

Remove `-DHAVE_STRTONUM` if your system needs the strtonum(3) function.

Remove `-DHAVE_GETPROGNAME` if your system needs the getprogname(3)
function.

License
-------
ISC License.
See `LICENSE` for details.
