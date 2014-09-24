/*
** sdl_visual_various.h
**
** Copyright 2001 Christer Gustavsson <cg@nocrew.org>
**
** This program is free software; you can redistribute it and/or modify
** it under the terms of the GNU General Public License as published by
** the Free Software Foundation; either version 2 of the License, or
** (at your option) any later version.
**  
** Read the file COPYING for more information.
**
*/

#include "ovdisis.h"

void
sdl_visual_inquire(void *        vis,
                   Visual_Attr * attr);

void
sdl_visual_put_pixel(VWKREF vwk,
                     int    x,
                     int    y,
                     int    c);

void
sdl_visual_put_pixels (VWKREF vwk,
		       int n,
		       Pixel *pixel );


int
sdl_visual_get_pixel (VWKREF vwk,
		      int    x,
		      int    y);

void
sdl_visual_hline(VWKREF vwk,
                 int    x1,
                 int    x2,
                 int    y,
                 int    c);

void
sdl_visual_line(VWKREF vwk,
                int    x1,
                int    y1,
                int    x2,
                int    y2,
                int    c);
