/*
** ggi_visual_various.c
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

#include <ggi/ggi.h>
#include <stdio.h>

#include "ggi_visual.h"
#include "ggi_visual_various.h"
#include "ovdisis.h"
#include "various.h"

int
ggi_visual_get_pixel (VWKREF vwk,
		      int    x,
		      int    y) {
  ggi_pixel pixel;
  ggi_color color;
  int i;

  /* Get the pixel */
  ggiGetPixel( VISUAL_T(vwk->visual->private), x, y, &pixel);

  /* Get the color of the pixel */
  ggiUnmapPixel( VISUAL_T(vwk->visual->private), pixel, &color);

  /* Scale values */
  color.r = ((long)(color.r*1000)) / 65535;
  color.g = ((long)(color.g*1000)) / 65535;
  color.b = ((long)(color.b*1000)) / 65535;

  /* Try to find the corresponding index in the colors of the workstation */
  for( i=0 ; i<vwk->dev.attr.colors ; i++ ) {
    if( color.r==vwk->vdi_cmap.red[i] &&
	color.g==vwk->vdi_cmap.green[i] &&
	color.r==vwk->vdi_cmap.blue[i]
	) {
      return i;
    }
  }

  fprintf(stderr, "ggi_visual_get_pixel: Couldn't match a VDI color\n");
  return 0;
}


void
ggi_visual_put_pixel (VWKREF vwk,
		      int    x,
		      int    y,
		      int    c) {
  if( WRITE_MODE(vwk->visual->private) == MD_REPLACE )
    ggiPutPixel(VISUAL_T(vwk->visual->private),
		x,
		y,
		COLOR_MAPPED(vwk->visual->private)[c]);
  else
    fprintf( stderr, "ggi_visual_put_pixel: Implement write_modes\n");
}


void
ggi_visual_hline (VWKREF vwk,
		  int    x1,
		  int    x2,
		  int    y,
		  int    c) {
  ggiSetGCForeground(VISUAL_T(vwk->visual->private),
                     COLOR_MAPPED(vwk->visual->private)[c]);
  ggiDrawHLine(VISUAL_T(vwk->visual->private), x1, y, x2 - x1 + 1);
}


void
ggi_visual_line (VWKREF vwk,
		 int    x1,
		 int    y1,
		 int    x2,
		 int    y2,
		 int    c) {
  ggiSetGCForeground(VISUAL_T(vwk->visual->private),
                     COLOR_MAPPED(vwk->visual->private)[c]);
  ggiDrawLine(VISUAL_T(vwk->visual->private), x1, y1, x2, y2);
}


void
ggi_visual_inquire (void *        vis,
		    Visual_Attr * attr) {
  ggi_mode graphical_mode;

  ggiGetMode(VISUAL_T(vis), &graphical_mode);

  attr->x_res = graphical_mode.virt.x;
  attr->y_res = graphical_mode.virt.y;
  attr->palette_size = 1 << GT_DEPTH(graphical_mode.graphtype);
  attr->number_of_colours = 1 << GT_DEPTH(graphical_mode.graphtype);
  attr->bits_per_pixel = GT_DEPTH(graphical_mode.graphtype);
}


