/*
** ggi_visual_cmap.c
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

#include <ggi/ggi.h>
#include <stdio.h>

#include "ggi_visual.h"
#include "ggi_visual_cmap.h"
#include "ovdisis.h"
#include "various.h"

void
ggi_visual_set_cmap (VWKREF vwk,
		     int    index,
		     int    red,
		     int    green,
		     int    blue) {
  fprintf(stderr, "Implement ggi_visual_set_cmap\n");
}


void
ggi_visual_get_cmap (void * vis,
                     int    index,
                     int *  red,
                     int *  green,
                     int *  blue) {
  fprintf(stderr, "Implement ggi_visual_get_cmap\n");
}


void
ggi_visual_put_cmap (VWKREF wk)
{
  int       i;
  ggi_color cmap[256];
  int       number_of_pens;
  int       number_of_planes; /* FIXME */

  if(wk->inq.attr.planes >= 8)
  {
    number_of_pens = 256;
    number_of_planes = 8;
  }
  else
  {
    number_of_pens = 1 << wk->inq.attr.planes;
    number_of_planes = wk->inq.attr.planes;
  }

  for (i = 0; i < number_of_pens; i++) {
    int ti = gem2tos_color(number_of_planes, i);
    
    cmap[ti].r = (wk->vdi_cmap.red[i] * 65535) / 1000;
    cmap[ti].g = (wk->vdi_cmap.green[i] * 65535) / 1000;
    cmap[ti].b = (wk->vdi_cmap.blue[i] * 65535) / 1000;
    cmap[ti].a = 0xffff;
  }
  
  ggiSetPalette(VISUAL_T(wk->visual->private),
		0,
		number_of_pens - 1,
		cmap);
  
  for(i = 0; i < number_of_pens; i++)
  {
    COLOURS(wk->visual->private)[i] =
      ggiMapColor(VISUAL_T(wk->visual->private), &cmap[i]);
  }
}

void
ggi_visual_free_cmap (VWKREF wk)
{
  fprintf(stderr, "Implement ggi_visual_free_cmap\n");
}
