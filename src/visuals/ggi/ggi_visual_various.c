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
get_color_index(VWKREF vwk,
		const ggi_color * color) {
  short r, g, b;
  int i;

  r = (short)(((long)(color->r * 1000)) / 65535);
  g = (short)(((long)(color->g * 1000)) / 65535);
  b = (short)(((long)(color->b * 1000)) / 65535);
  for ( i = 0; i < vwk->dev.attr.colors; i++ ) {
    if ( r == vwk->vdi_cmap.red[i] &&
	 g == vwk->vdi_cmap.green[i] &&
	 b == vwk->vdi_cmap.blue[i] )
      return i;
  }
  return -1;
}


int
get_xorcolor_index(VWKREF vwk,
		   const ggi_color * color) {
  int i = get_color_index(vwk,color);
  if ( i >= 0 )
    i = vwk->dev.attr.colors - i;
  return i;
}


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

  if ( ( i = get_color_index( vwk, &color ) ) < 0 ) {
    fprintf(stderr, "ggi_visual_get_pixel: Couldn't match a VDI color\n");
    i = 0;
  }

  return i;
}


void
ggi_visual_put_pixel (VWKREF vwk,
		      int    x,
		      int    y,
		      int    c) {
  if( WRITE_MODE(vwk->visual->private) == MD_REPLACE ||
      WRITE_MODE(vwk->visual->private) == MD_TRANS )
    ggiPutPixel(VISUAL_T(vwk->visual->private),
		x,
		y,
		COLOR_MAPPED(vwk->visual->private)[c]);
  else if ( WRITE_MODE(vwk->visual->private) == MD_XOR ) {
    ggi_pixel pixel;
    ggi_color color;
    int i;
    ggiGetPixel(VISUAL_T(vwk->visual->private),x,y,&pixel);
    ggiUnmapPixel( VISUAL_T(vwk->visual->private), pixel, &color);
    i = get_xorcolor_index(vwk,&color);
    ggiPutPixel(VISUAL_T(vwk->visual->private),
		x,y,COLOR_MAPPED(vwk->visual->private)[i]);
  }
}


void
ggi_visual_hline (VWKREF vwk,
		  int    x1,
		  int    x2,
		  int    y,
		  int    c) {
  if( WRITE_MODE(vwk->visual->private) == MD_REPLACE ||
      WRITE_MODE(vwk->visual->private) == MD_TRANS ) {
    ggiSetGCForeground(VISUAL_T(vwk->visual->private),
		       COLOR_MAPPED(vwk->visual->private)[c]);
    ggiDrawHLine(VISUAL_T(vwk->visual->private), x1, y, x2 - x1 + 1);
  } else if ( WRITE_MODE(vwk->visual->private) == MD_XOR ) {
    /* There's a simple solution putting pixels, but there may be
     * a faster solution, searching for parts with the same color
     * and drawing the parts
     */
    int xs, xe;
    ggi_pixel cmppixel, pixel;
    ggi_color color;
    int i;
    for ( xs = x1; xs < x2; xs = xe ) {
      ggiGetPixel(VISUAL_T(vwk->visual->private),xs,y,&pixel);
      for ( cmppixel = pixel, xe = xs + 1; 
            xe < x2 && pixel == cmppixel; 
            xe++ ) {
        ggiGetPixel(VISUAL_T(vwk->visual->private),xe,y,&pixel);
      }
      ggiUnmapPixel(VISUAL_T(vwk->visual->private),cmppixel,&color);
      i = get_xorcolor_index(vwk,&color);
      ggiSetGCForeground(VISUAL_T(vwk->visual->private),
			 COLOR_MAPPED(vwk->visual->private)[i]);
      ggiDrawHLine(VISUAL_T(vwk->visual->private), xs, y, xe - xs);
    }
  }
}


void
ggi_visual_vline (VWKREF vwk,
		  int    x,
		  int    y1,
		  int    y2,
		  int    c) {
  if( WRITE_MODE(vwk->visual->private) == MD_REPLACE ||
      WRITE_MODE(vwk->visual->private) == MD_TRANS ) {
    ggiSetGCForeground(VISUAL_T(vwk->visual->private),
		       COLOR_MAPPED(vwk->visual->private)[c]);
    ggiDrawVLine(VISUAL_T(vwk->visual->private), x, y1, y2 - y1 + 1);
  } else if ( WRITE_MODE(vwk->visual->private) != MD_ERASE ) {
    /* There's a simple solution putting pixels, but there may be
     * a faster solution, searching for parts with the same color
     * and drawing the parts
     */
    int i;
    int ys, ye;
    ggi_pixel cmppixel, pixel;
    ggi_color color;
    for ( ys = y1; ys < y2; ys = ye ) {
      ggiGetPixel(VISUAL_T(vwk->visual->private),x,ys,&pixel);
      for ( cmppixel = pixel, ye = ys + 1; 
            ye < y2 && pixel == cmppixel; 
            ye++ ) {
        ggiGetPixel(VISUAL_T(vwk->visual->private),x,ye,&pixel);
      }
      ggiUnmapPixel(VISUAL_T(vwk->visual->private),cmppixel,&color);
      i = get_xorcolor_index(vwk,&color);
      ggiSetGCForeground(VISUAL_T(vwk->visual->private),
			 COLOR_MAPPED(vwk->visual->private)[i]);
      ggiDrawVLine(VISUAL_T(vwk->visual->private), x, ys, ye - ys);
    }
  }
}


void
ggi_visual_line (VWKREF vwk,
		 int    x1,
		 int    y1,
		 int    x2,
		 int    y2,
		 int    c) {
  if ( y1 == y2 ) {
    /* HLine */
    ggi_visual_hline( vwk, x1, x2, y1, c );
  } else if ( x1 == x2 ) {
    /* VLINE */
    ggi_visual_vline( vwk, x1, y1, y2, c );
  } else {
    if( WRITE_MODE(vwk->visual->private) == MD_REPLACE ||
	WRITE_MODE(vwk->visual->private) == MD_TRANS ) {
      ggiSetGCForeground(VISUAL_T(vwk->visual->private),
			 COLOR_MAPPED(vwk->visual->private)[c]);
      ggiDrawLine(VISUAL_T(vwk->visual->private), x1, y1, x2, y2);
    } else if ( WRITE_MODE(vwk->visual->private) != MD_ERASE ) {
      fprintf( stderr, "ggi_visual_line: implement draw-mode %s\n",
	       (WRITE_MODE(vwk->visual->private) == MD_XOR) ? "MD_XOR" :
	       "unknown" );
    }
  }
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

