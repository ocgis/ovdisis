/*
** ggi_visual_cmap.c
**
** Copyright 1999 - 2000 Christer Gustavsson <cg@nocrew.org>
**           2000     Vincent Barrilliot <vincent_barrilliot@yahoo.com>
**
** Change marks:
**  MK: Markus Kohm <markus.kohm@gmx.de>
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
#include "ggi_visual_cmap.h"
#include "ovdisis.h"
#include "various.h"

static inline void do_set_unmapped (VWKREF vwk,
		     int    index,
		     int    red,
		     int    green,
		     int    blue) {
  ggi_color *color_unmapped = COLOR_UNMAPPED(vwk->visual->private);

  color_unmapped[index].r = (red   * 65535) / 1000;
  color_unmapped[index].g = (green * 65535) / 1000;
  color_unmapped[index].b = (blue  * 65535) / 1000;
  color_unmapped[index].a = 0xffff;

}

static inline void do_set_mapped( VWKREF vwk, int index) {
  COLOR_MAPPED(vwk->visual->private)[index] = 
    ggiMapColor(VISUAL_T(vwk->visual->private),
		&COLOR_UNMAPPED(vwk->visual->private)[index]);
}

void
ggi_visual_set_cmap (VWKREF vwk,
		     int    index,
		     int    red,
		     int    green,
		     int    blue) {
#if DEBUGLEVEL>=1
  int i;
#endif
  do_set_unmapped( vwk, index, red, green, blue );

#if DEBUGLEVEL>=1
  for(i=0; i<16 ; i++) {
    ggi_color *color_unmapped = COLOR_UNMAPPED(vwk->visual->private);

    EDEBUG("Color %d: R:%d  G:%d  B:%d\n",i,color_unmapped[i].r, color_unmapped[i].g, color_unmapped[i].b);
  }
#endif

  ggiSetPalette( VISUAL_T(vwk->visual->private),
		 index,
		 1,
		 &COLOR_UNMAPPED(vwk->visual->private)[index] );

  do_set_mapped( vwk, index );
}


void
ggi_visual_get_cmap (void * vis,
                     int    index,
                     int  * red,
                     int  * green,
                     int  * blue) {

  *red =   (short)((COLOR_UNMAPPED(vis)[index].r   * 1000) / 65535);
  *green = (short)((COLOR_UNMAPPED(vis)[index].g   * 1000) / 65535);
  *blue =  (short)((COLOR_UNMAPPED(vis)[index].b   * 1000) / 65535);

  EDEBUG("ggi_visual_get_cmap: index %d is R:%d G:%d B:%d\n",index,*red,*green,*blue);
}


void
ggi_visual_put_cmap(VWKREF vwk)
{
  int       i;
  int       number_of_pens;
  ggi_color *color_unmapped;

  color_unmapped = COLOR_UNMAPPED(vwk->visual->private);
  number_of_pens = min( vwk->dev.attr.colors, 256 );
  
  for( i=0 ; i<number_of_pens ; i++) {
    do_set_unmapped( vwk, i, 
		     vwk->vdi_cmap.red [i],
		     vwk->vdi_cmap.green[i],
		     vwk->vdi_cmap.blue[i] );
  }
  
  ggiSetPalette(VISUAL_T(vwk->visual->private),
		0,
		number_of_pens - 1,
		color_unmapped);

  for( i=0 ; i<number_of_pens ; i++)
    do_set_mapped( vwk, i );
}

void
ggi_visual_free_cmap (VWKREF wk)
{
#if 0 /* 2000-12-07 MK */
  fprintf(stderr, "Implement ggi_visual_free_cmap\n");
#else
  EDEBUG("ggi cmap doesn't really need to be freed\n");
#endif
}


