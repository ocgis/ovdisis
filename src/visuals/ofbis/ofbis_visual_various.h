/*
** ofbis_visual_various.h
**
** Copyright 1999 Christer Gustavsson <cg@nocrew.org>
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
ofbis_visual_get_pixel (VWKREF wk,
			int    x,
			int    y);

void
ofbis_visual_put_pixel (VWKREF wk,
			int    x,
			int    y,
			int    c);

void
ofbis_visual_hline (VWKREF wk,
		    int    x1,
		    int    x2,
		    int    y,
		    int    c);

void
ofbis_visual_line (VWKREF wk,
		   int    x1,
		   int    y1,
		   int    x2,
		   int    y2,
		   int    c);

void
ofbis_visual_bitblt (VWKREF vwk,
		     int    mode,
		     RECT * srccor,
		     RECT * dstcor,
		     MFDB * src,
		     MFDB * dst);

void
ofbis_visual_bitbltt (VWKREF wk,
		      int    mode,
		      int    fgcol,
		      int    bgcol,
		      RECT * srccor,
		      RECT * dstcor,
		      MFDB * src,
		      MFDB * dst);

void
ofbis_visual_put_char (VWKREF wk,
                       int    x,
                       int    y,
                       int    col,
                       int    ch);

void
ofbis_visual_set_font (void * fb,
		       void * data,
		       int    width,
		       int    height);

void
ofbis_visual_inquire (void *        fb,
                      Visual_Attr * attr);
