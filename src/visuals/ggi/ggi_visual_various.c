/*
** ggi_visual_various.c
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

#include <stdio.h>

#include "ovdisis.h"

int
ggi_visual_get_pixel (void * fb,
		      int    x,
		      int    y) {
  fprintf(stderr, "Implement ggi_visual_get_pixel\n");
  return 0;
}


void
ggi_visual_put_pixel (void * fb,
		      int    x,
		      int    y,
		      int    c) {
  fprintf(stderr, "Implement ggi_visual_put_pixel\n");
}


void
ggi_visual_hline (void * fb,
		  int    x1,
		  int    x2,
		  int    y,
		  int    c) {
  fprintf(stderr, "Implement ggi_visual_hline\n");
}


void
ggi_visual_line (void * fb,
		 int    x1,
		 int    y1,
		 int    x2,
		 int    y2,
		 int    c) {
  fprintf(stderr, "Implement ggi_visual_line\n");
}


void
ggi_visual_bitblt (VDI_Workstation * vwk,
		   int               mode,
		   RECT *            srccor,
		   RECT *            dstcor,
		   MFDB *            src,
		   MFDB *            dst) {
  fprintf(stderr, "Implement ggi_visual_bitblt\n");
}


void
ggi_visual_bitbltt (VDI_Workstation * vwk,
		    int               mode,
		    int               fgcol,
		    int               bgcol,
		    RECT *            srccor,
		    RECT *            dstcor,
		    MFDB *            src,
		    MFDB *            dst) {
  fprintf(stderr, "Implement ggi_visual_bitbltt\n");
}


void
ggi_visual_put_char (void * fb,
		     int    x,
		     int    y,
		     int    col,
		     int    ch) {
  fprintf(stderr, "Implement ggi_visual_put_char\n");
}


void
ggi_visual_set_font (void * fb,
		     void * data,
		     int    width,
		     int    height) {
  fprintf(stderr, "Implement ggi_visual_set_font\n");
}


void
ggi_visual_inquire (void *        fb,
		    Visual_Attr * attr) {
  fprintf(stderr, "Implement ggi_visual_inquire\n");
}


