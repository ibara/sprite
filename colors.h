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
 * From https://jonasjacek.github.io/colors/
 */

static const int colors[256] = {
	0x00000000,	/* Black	0 */
	0x00800000,	/* Maroon	1 */
	0x00008000,	/* Green	2 */
	0x00808000,	/* Olive	3 */
	0x00000080,	/* Navy		4 */
	0x00800080,	/* Purple	5 */
	0x00008080,	/* Teal		6 */
	0x00c0c0c0,	/* Silver	7 (Transparent 1) */
	0x00808080,	/* Grey		8 (Transparent 2) */
	0x00ff0000,	/* Red		9 */
	0x0000ff00,	/* Lime		10 */
	0x00ffff00,	/* Yellow	11 */
	0x000000ff,	/* Blue		12 */
	0x00ff00ff,	/* Fuchsia	13 */
	0x0000ffff,	/* Aqua		14 */
	0x00ffffff,	/* White	15 */
	0x00000000,	/* Grey0	16 (Same as Black) */
	0x0000005f,	/* NavyBlue	17 */
	0x00000087,	/* DarkBlue	18 */
	0x000000af,	/* Blue3	19 */
	0x000000d7,	/* Blue3	20 */
	0x000000ff,	/* Blue1	21 (Same as Blue) */
	0x00005f00,	/* DarkGreen	22 */
	0x00005f5f,	/* DeepSkyBlue4	23 */
	0x00005f87,	/* DeepSkyBlue4	24 */
	0x00005faf,	/* DeepSkyBlue4	25 */
	0x00005fd7,	/* DodgerBlue3	26 */
	0x00005fff,	/* DodgerBlue2	27 */
	0x00008700,	/* Green4	28 */
	0x0000875f,	/* SpringGreen4	29 */
	0x00008787,	/* Turquoise4	30 */
	0x000087af,	/* DeepSkyBlue3	31 */
	0x000087d7,	/* DeepSkyBlue3	32 */
	0x000087ff,	/* DodgerBlue1	33 */
	0x0000af00,	/* Green3	34 */
	0x0000af5f,	/* SpringGreen3	35 */
	0x0000af87,	/* DarkCyan	36 */
	0x0000afaf,	/* LightSeaGreen 37 */
	0x0000afd7,	/* DeepSkyBlue2	38 */
	0x0000afff,	/* DeepSkyBlue1	39 */
	0x0000d700,	/* Green3	40 */
	0x0000d75f,	/* SpringGreen3	41 */
	0x0000d787,	/* SpringGreen2	42 */
	0x0000d7af,	/* Cyan3	43 */
	0x0000d7d7,	/* DarkTurquoise 44 */
	0x0000d7ff,	/* Turquoise2	45 */
	0x0000ff00,	/* Green1	46 (Same as Lime) */
	0x0000ff5f,	/* SpringGreen2	47 */
	0x0000ff87,	/* SpringGreen1	48 */
	0x0000ffaf,	/* MediumSpringGreen 49 */
	0x0000ffd7,	/* Cyan2	50 */
	0x0000ffff,	/* Cyan1	51 (Same as Aqua) */
	0x005f0000,	/* DarkRed	52 */
	0x005f005f,	/* DeepPink4	53 */
	0x005f0087,	/* Purple4	54 */
	0x005f00af,	/* Purple4	55 */
	0x005f00d7,	/* Purple3	56 */
	0x005f00ff,	/* BlueViolet	57 */
	0x005f5f00,	/* Orange4	58 */
	0x005f5f5f,	/* Grey37	59 */
	0x005f5f87,	/* MediumPurple4 60 */
	0x005f5faf,	/* SlateBlue3	61 */
	0x005f5fd7,	/* SlateBlue3	62 */
	0x005f5fff,	/* RoyalBlue1	63 */
	0x005f8700,	/* Chartreuse4	64 */
	0x005f875f,	/* DarkSeaGreen4 65 */
	0x005f8787,	/* PaleTurquoise4 66 */
	0x005f87af,	/* SteelBlue	67 */
	0x005f87d7,	/* SteelBlue3	68 */
	0x005f87ff,	/* CornflowerBlue 69 */
	0x005faf00,	/* Chartreuse3	70 */
	0x005faf5f,	/* DarkSeaGreen4 71 */
	0x005faf87,	/* CadetBlue	72 */
	0x005fafaf,	/* CadetBlue	73 */
	0x005fafd7,	/* SkyBlue3	74 */
	0x005fafff,	/* SteelBlue1	75 */
	0x005fd700,	/* Chartreuse3	76 */
	0x005fd75f,	/* PaleGreen3	77 */
	0x005fd787,	/* SeaGreen3	78 */
	0x005fd7af,	/* Aquamarine3	79 */
	0x005fd7d7,	/* MediumTurquoise 80 */
	0x005fd7ff,	/* SteelBlue1	81 */
	0x005fff00,	/* Chartreuse2	82 */
	0x005fff5f,	/* SeaGreen2	83 */
	0x005fff87,	/* SeaGreen1	84 */
	0x005fffaf,	/* SeaGreen1	85 */
	0x005fffd7,	/* Aquamarine1	86 */
	0x005fffff,	/* DarkSlateGray2 87 */
	0x00870000,	/* DarkRed	88 */
	0x0087005f,	/* DeepPink4	89 */
	0x00870087,	/* DarkMagenta	90 */
	0x008700af,	/* DarkMagenta	91 */
	0x008700d7,	/* DarkViolet	92 */
	0x008700ff,	/* Purple	93 */
	0x00875f00,	/* Orange4	94 */
	0x00875f5f,	/* LightPink4	95 */
	0x00875f87,	/* Plum4	96 */
	0x00875faf,	/* MediumPurple3 97 */
	0x00875fd7,	/* MediumPurple3 98 */
	0x00875fff,	/* SlateBlue1	99 */
	0x00878700,	/* Yellow4	100 */
	0x0087875f,	/* Wheat4	101 */
	0x00878787,	/* Grey53	102 */
	0x008787af,	/* LightSlateGrey 103 */
	0x008787d7,	/* MediumPurple	104 */
	0x008787ff,	/* LightSlateBlue 105 */
	0x0087af00,	/* Yellow4	106 */
	0x0087af5f,	/* DarkOliveGreen3 107 */
	0x0087af87,	/* DarkSeaGreen	108 */
	0x0087afaf,	/* LightSkyBlue3 109 */
	0x0087afd7,	/* LightSkyBlue3 110 */
	0x0087afff,	/* SkyBlue2	111 */
	0x0087d700,	/* Chartreuse2	112 */
	0x0087d75f,	/* DarkOliveGree3 113 */
	0x0087d787,	/* PaleGreen3	114 */
	0x0087d7af,	/* DarkSeaGreen3 115 */
	0x0087d7d7,	/* DarkSlateGray3 116 */
	0x0087d7ff,	/* SkyBlue1	117 */
	0x0087ff00,	/* Chartreuse1	118 */
	0x0087ff5f,	/* LightGreen	119 */
	0x0087ff87,	/* LightGreen	120 */
	0x0087ffaf,	/* PaleGreen1	121 */
	0x0087ffd7,	/* Aquamarine1	122 */
	0x0087ffff,	/* DarkSlateGray1 123 */
	0x00af0000,	/* Red3		124 */
	0x00af005f,	/* DeepPink4	125 */
	0x00af0087,	/* MediumVioletRed 126 */
	0x00af00af,	/* Magenta3	127 */
	0x00af00d7,	/* DarkViolet	128 */
	0x00af00ff,	/* Purple	129 */
	0x00af5f00,	/* DarkOrange3	130 */
	0x00af5f5f,	/* IndianRed	131 */
	0x00af5f87,	/* HotPink3	132 */
	0x00af5faf,	/* MediumOrchid3 133 */
	0x00af5fd7,	/* MediumOrchid	134 */
	0x00af5fff,	/* MediumPurple2 135 */
	0x00af8700,	/* DarkGoldenrod 136 */
	0x00af875f,	/* LightSalmon3	137 */
	0x00af8787,	/* RosyBrown	138 */
	0x00af87af,	/* Grey63	139 */
	0x00af87d7,	/* MediumPurple2 140 */
	0x00af87ff,	/* MediumPurple1 141 */
	0x00afaf00,	/* Gold3	142 */
	0x00afaf5f,	/* DarkKhaki	143 */
	0x00afaf87,	/* NavajoWhite3	144 */
	0x00afafaf,	/* Grey69	145 */
	0x00afafd7,	/* LightSteelBlue3 146 */
	0x00afafff,	/* LightSteelBlue 147 */
	0x00afd700,	/* Yellow3	148 */
	0x00afd75f,	/* DarkOliveGreen3 149 */
	0x00afd787,	/* DarkSeaGreen3 150 */
	0x00afd7af,	/* DarkSeaGreen2 151 */
	0x00afd7d7,	/* LightCyan3	152 */
	0x00afd7ff,	/* LightSkyBlue1 153 */
	0x00afff00,	/* GreenYellow	154 */
	0x00afff5f,	/* DarkOliveGreen2 155 */
	0x00afff87,	/* PaleGreen1	156 */
	0x00afffaf,	/* DarkSeaGreen2 157 */
	0x00afffd7,	/* DarkSeaGreen1 158 */
	0x00afffff,	/* PaleTurquoise1 159 */
	0x00d70000,	/* Red3		160 */
	0x00d7005f,	/* DeepPink3	161 */
	0x00d70087,	/* DeepPink3	162 */
	0x00d700af,	/* Magenta3	163 */
	0x00d700d7,	/* Magenta3	164 */
	0x00d700ff,	/* Magenta2	165 */
	0x00d75f00,	/* DarkOrange3	166 */
	0x00d75f5f,	/* IndianRed	167 */
	0x00d75f87,	/* HotPink3	168 */
	0x00d75faf,	/* HotPink2	169 */
	0x00d75fd7,	/* Orchid	170 */
	0x00d75fff,	/* MediumOrchid1 171 */
	0x00d78700,	/* Orange3	172 */
	0x00d7875f,	/* LightSalmon3	173 */
	0x00d78787,	/* LightPink3	174 */
	0x00d787af,	/* Pink3	175 */
	0x00d787d7,	/* Plum3	176 */
	0x00d787ff,	/* Violet	177 */
	0x00d7af00,	/* Gold3	178 */
	0x00d7af5f,	/* LightGoldenrod3 179 */
	0x00d7af87,	/* Tan		180 */
	0x00d7afaf,	/* MistyRose3	181 */
	0x00d7afd7,	/* Thistle3	182 */
	0x00d7afff,	/* Plum2	183 */
	0x00d7d700,	/* Yellow3	184 */
	0x00d7d75f,	/* Khaki3	185 */
	0x00d7d787,	/* LightGoldenrod2 186 */
	0x00d7d7af,	/* LightYellow3	187 */
	0x00d7d7d7,	/* Grey84	188 */
	0x00d7d7ff,	/* LightSteelBlue1 189 */
	0x00d7ff00,	/* Yellow2	190 */
	0x00d7ff5f,	/* DarkOliveGreen1 191 */
	0x00d7ff87,	/* DarkOliveGreen1 192 */
	0x00d7ffaf,	/* DarkSeaGreen1 193 */
	0x00d7ffd7,	/* Honeydew2	194 */
	0x00d7ffff,	/* LightCyan1	195 */
	0x00ff0000,	/* Red1		196 */
	0x00ff005f,	/* DeepPink2	197 */
	0x00ff0087,	/* DeepPink1	198 */
	0x00ff00af,	/* DeepPink1	199 */
	0x00ff00d7,	/* Magenta2	200 */
	0x00ff00ff,	/* Magenta1	201 */
	0x00ff5f00,	/* OrangeRed1	202 */
	0x00ff5f5f,	/* IndianRed1	203 */
	0x00ff5f87,	/* IndianRed1	204 */
	0x00ff5faf,	/* HotPink	205 */
	0x00ff5fd7,	/* HotPink	206 */
	0x00ff5fff,	/* MediumOrchid1 207 */
	0x00ff8700,	/* DarkOrange	208 */
	0x00ff875f,	/* Salmon1	209 */
	0x00ff8787,	/* LightCoral	210 */
	0x00ff87af,	/* PaleVioletRed1 211 */
	0x00ff87d7,	/* Orchid2	212 */
	0x00ff87ff,	/* Orchid1	213 */
	0x00ffaf00,	/* Orange1	214 */
	0x00ffaf5f,	/* SandyBrown	215 */
	0x00ffaf87,	/* LightSalmon1	216 */
	0x00ffafaf,	/* LightPink1	217 */
	0x00ffafd7,	/* Pink1	218 */
	0x00ffafff,	/* Plum1	219 */
	0x00ffd700,	/* Gold1	220 */
	0x00ddf75f,	/* LightGoldenrod2 221 */
	0x00ffd787,	/* LightGoldenrod2 222 */
	0x00ffd7af,	/* NavajoWhite1	223 */
	0x00ffd7d7,	/* MistyRose1	224 */
	0x00ffd7ff,	/* Thistle1	225 */
	0x00ffff00,	/* Yellow1	226 */
	0x00ffff5f,	/* LightGoldenrod1 227 */
	0x00ffff87,	/* Khaki1	228 */
	0x00ffffaf,	/* Wheat1	229 */
	0x00ffffd7,	/* Cornsilk1	230 */
	0x00ffffff,	/* Grey100	231 */
	0x00080808,	/* Grey3	232 */
	0x00121212,	/* Grey7	233 */
	0x001c1c1c,	/* Grey11	234 */
	0x00262626,	/* Grey15	235 */
	0x00303030,	/* Grey19	236 */
	0x003a3a3a,	/* Grey23	237 */
	0x00444444,	/* Grey27	238 */
	0x004e4e4e,	/* Grey30	239 */
	0x00585858,	/* Grey35	240 */
	0x00626262,	/* Grey39	241 */
	0x006c6c6c,	/* Grey42	242 */
	0x00767676,	/* Grey46	243 */
	0x00808080,	/* Grey50	244 */
	0x008a8a8a,	/* Grey54	245 */
	0x00949494,	/* Grey58	246 */
	0x009e9e9e,	/* Grey62	247 */
	0x00a8a8a8,	/* Grey66	248 */
	0x00b2b2b2,	/* Grey70	249 */
	0x00bcbcbc,	/* Grey74	250 */
	0x00c6c6c6,	/* Grey78	251 */
	0x00d0d0d0,	/* Grey82	252 */
	0x00dadada,	/* Grey85	253 */
	0x00e4e4e4,	/* Grey89	254 */
	0x00eeeeee	/* Grey93	255 */
};
