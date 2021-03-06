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
#include <stdio.h>

#include "ofbis_visual.h"
#include "ofbis_visual_cmap.h"
#include "ovdisis.h"
#include "various.h"

void
ofbis_visual_set_cmap (VWKREF wk,
		       int    index,
		       int    red,
		       int    green,
		       int    blue) {
  FBCMAP *fbcmap;
  switch (wk->inq.attr.planes) {
  case 1:
  case 2:
  case 4:
  case 8:
    fbcmap = FBgetcmap(FB_T(wk->visual->private));
    fbcmap->red[index]   = (unsigned short)(red   * 65535) / 1000;
    fbcmap->green[index] = (unsigned short)(green * 65535) / 1000;
    fbcmap->blue[index]  = (unsigned short)(blue  * 65535) / 1000;
    
    fbcmap->start = index;
    fbcmap->end   = index;
    fbcmap->len   = 1;

    FBputcmap(FB_T(wk->visual->private), fbcmap);
    FBfreecmap(fbcmap);
    break;
  case 15:
  case 16:
  case 24:
  case 32:
    COLOURS(wk->visual->private)[index] = (( ( (red * 65535/1000) >> 8) << 16) |
					   ( ( (green * 65535/1000) >> 8) << 8) |
					   ( ( (blue * 65535/1000) >> 8)));
    break;
  }
}


void
ofbis_visual_get_cmap (void * fb,
		       int    index,
		       int *  red,
		       int *  green,
		       int *  blue) {
  *red = (short)((FB_T(fb)->cmap->red[index]   * 1000) / 65535);
  *green = (short)((FB_T(fb)->cmap->green[index] * 1000) / 65535);
  *blue = (short)((FB_T(fb)->cmap->blue[index]  * 1000) / 65535);
}


void
ofbis_visual_put_cmap (VDI_Workstation * wk) {
  int i;
  FBCMAP *fbcmap;

  switch (wk->inq.attr.planes) {
  case 1:
  case 2:
  case 4:
  case 8:
    fbcmap = FBgetcmap(FB_T(wk->visual->private));
    ADEBUG("v_opnwk: FB cmap length %d allocated: %p\n",
	   fbcmap->len, fbcmap);

    for (i = 0; i < fbcmap->len; i++) {
      int ti = gem2tos_color(wk->inq.attr.planes, i);

      fbcmap->red[ti]   = (wk->vdi_cmap.red[i] * 65535) / 1000;
      fbcmap->green[ti] = (wk->vdi_cmap.green[i] * 65535) / 1000;
      fbcmap->blue[ti]  = (wk->vdi_cmap.blue[i] * 65535) / 1000;
    }
    /* Put the changed cmap on to the FrameBuffer */
    fbcmap->start = 0;
    fbcmap->len = (1 << wk->inq.attr.planes);
    fbcmap->end = (1 << wk->inq.attr.planes) - 1;
    FBputcmap(FB_T(wk->visual->private), fbcmap);
    FBfreecmap(fbcmap);
    break;
  case 15:
  case 16:
  case 24:
  case 32:
    for(i = 0; i < 256; i++) {
      COLOURS(wk->visual->private)[i] = (( ( (wk->vdi_cmap.red[i] * 65535/1000) >> 8) << 16) |
					 ( ( (wk->vdi_cmap.green[i] * 65535/1000) >> 8) << 8) |
					 ( ( (wk->vdi_cmap.blue[i] * 65535/1000) >> 8)));
    }
  default:
    ;
  }
}


void
ofbis_visual_free_cmap (VDI_Workstation * wk) {
#if 0
  /* cmap not used in TrueColor mode */
  if (wk->inq.attr.planes < 16) {
    FBfreecmap(FB_T(wk->visual->private)->cmap);
    ADEBUG("v_clswk: FB cmap freed\n");
  }
#else
    ADEBUG("FB cmap doesn't really need to be freed\n");
#endif
}


int 
ofbis_native_colour(VWKREF wk, int c) {
  switch(wk->inq.attr.planes) {
  case 1:
  case 2:
  case 4:
  case 8:
    return c;
  case 15:
  case 16:
  case 24:
  case 32:
  default:
    if(c>255) {
      fprintf(stderr, "Illegal colour value %d\n", c);
      return FBc24_to_cnative(FB_T(wk->visual->private), 
			      COLOURS(wk->visual->private)[255]);
    }
    return FBc24_to_cnative(FB_T(wk->visual->private), 
			    COLOURS(wk->visual->private)[c]);
  }
}
