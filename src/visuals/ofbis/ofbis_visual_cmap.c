/*
** ofbis_visual_cmap.c
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

#include <ofbis.h>
#include <unistd.h>

#include "ovdisis.h"
#include "various.h"

void
ofbis_visual_set_cmap (void * fb,
		       int    index,
		       int    red,
		       int    green,
		       int    blue) {
  ((FB *)fb)->cmap->red[index]   = (unsigned short)(red   * 65535) / 1000;
  ((FB *)fb)->cmap->green[index] = (unsigned short)(green * 65535) / 1000;
  ((FB *)fb)->cmap->blue[index]  = (unsigned short)(blue  * 65535) / 1000;
  
  ((FB *)fb)->cmap->start = index;
  ((FB *)fb)->cmap->end   = index;
  ((FB *)fb)->cmap->len   = 1;

  FBputcmap((FB *)fb, ((FB *)fb)->cmap);
}


void
ofbis_visual_get_cmap (void * fb,
		       int    index,
		       int *  red,
		       int *  green,
		       int *  blue) {
  *red = (short)((((FB *)fb)->cmap->red[index]   * 1000) / 65535);
  *green = (short)((((FB *)fb)->cmap->green[index] * 1000) / 65535);
  *blue = (short)((((FB *)fb)->cmap->blue[index]  * 1000) / 65535);
}


#define fb ((FB *)wk->visual->private)

void
ofbis_visual_put_cmap (VDI_Workstation * wk) {
  int i;

  switch (wk->inq.attr.planes) {
  case 1:
  case 2:
  case 4:
  case 8:
    /* fb->cmap is set by FBgetcmap */
    FBgetcmap(fb);
    ADEBUG("v_opnwk: FB cmap length %d allocated: %p\n",
	   fb->cmap->len, fb->cmap);

    for (i = 0; i < fb->cmap->len; i++) {
      int ti = gem2tos_color(wk->inq.attr.planes, i);

      fb->cmap->red[ti]   =
        (wk->vdi_cmap.red[i] * 65535) / 1000;
      fb->cmap->green[ti]   =
        (wk->vdi_cmap.green[i] * 65535) / 1000;
      fb->cmap->blue[ti]   =
        (wk->vdi_cmap.blue[i] * 65535) / 1000;
    }
    /* Put the changed cmap on to the FrameBuffer */
    fb->cmap->start = 0;
    fb->cmap->len = (1 << wk->inq.attr.planes);
    fb->cmap->end = (1 << wk->inq.attr.planes) - 1;

    sleep(1);			/* This is apparently needed in 8 bit mode (oFBis bug?) */

    FBputcmap(fb, fb->cmap);
    break;

  default:
    ;
  }
}

void
ofbis_visual_free_cmap (VDI_Workstation * wk) {
  /* cmap not used in TrueColor mode */
  if (wk->inq.attr.planes < 16) {
    FBfreecmap(((FB *)wk->visual->private)->cmap);
    ADEBUG("v_clswk: FB cmap freed\n");
  }
}

#undef fb
