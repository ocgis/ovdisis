/*
** ggi_visual_various.h
**
** Copyright 1999 - 2000 Christer Gustavsson <cg@nocrew.org>
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

int
ggi_visual_get_pixel (VWKREF vwk,
		      int    x,
		      int    y);

void
ggi_visual_put_pixel (VWKREF vwk,
		      int    x,
		      int    y,
		      int    c);

void
ggi_visual_put_pixels (VWKREF vwk,
		       int n,
		       Pixel *pixel );


void
ggi_visual_hline (VWKREF vwk,
		  int    x1,
		  int    x2,
		  int    y,
		  int    c);

void
ggi_visual_line (VWKREF vwk,
		 int    x1,
		 int    y1,
		 int    x2,
		 int    y2,
		 int    c);

void
ggi_visual_inquire (void *        fb,
		    Visual_Attr * attr);
