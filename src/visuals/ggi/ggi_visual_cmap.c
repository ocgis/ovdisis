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
#include "ovdisis.h"
#include "various.h"

void
ggi_visual_set_cmap (void * fb,
		     int    index,
		     int    red,
		     int    green,
		     int    blue) {
  fprintf(stderr, "Implement ggi_visual_set_cmap\n");
}


void
ggi_visual_get_cmap (void * fb,
		       int    index,
		       int *  red,
		       int *  green,
		       int *  blue) {
  fprintf(stderr, "Implement ggi_visual_get_cmap\n");
}


void
ggi_visual_put_cmap (VDI_Workstation * wk) {
  int i;
  ggi_color cmap[1 << wk->inq.attr.planes];

  switch (wk->inq.attr.planes) {
  case 1:
  case 2:
  case 4:
  case 8:
    for (i = 0; i < (1 << wk->inq.attr.planes); i++) {
      int ti = gem2tos_color(wk->inq.attr.planes, i);

      cmap[ti].r = (wk->vdi_cmap.red[i] * 65535) / 1000;
      cmap[ti].g = (wk->vdi_cmap.green[i] * 65535) / 1000;
      cmap[ti].b = (wk->vdi_cmap.blue[i] * 65535) / 1000;
      cmap[ti].a = 0xffff;
    }

    ggiSetPalette(VISUAL_T(wk->visual->private),
                  0,
                  1 << wk->inq.attr.planes,
                  cmap);

    for (i = 0; i < (1 << wk->inq.attr.planes); i++) {
      COLOURS(wk->visual->private)[i] =
        ggiMapColor(VISUAL_T(wk->visual->private), &cmap[i]);
    }

    break;

  default:
    ;
  }
}

void
ggi_visual_free_cmap (VDI_Workstation * wk) {
  fprintf(stderr, "Implement ggi_visual_free_cmap\n");
}
