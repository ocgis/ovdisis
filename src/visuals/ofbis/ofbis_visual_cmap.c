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
  switch (wk->inq.attr.planes) {
  case 1:
  case 2:
  case 4:
  case 8:
    FB_T(wk->visual->private)->cmap->red[index]   = (unsigned short)(red   * 65535) / 1000;
    FB_T(wk->visual->private)->cmap->green[index] = (unsigned short)(green * 65535) / 1000;
    FB_T(wk->visual->private)->cmap->blue[index]  = (unsigned short)(blue  * 65535) / 1000;
    
    FB_T(wk->visual->private)->cmap->start = index;
    FB_T(wk->visual->private)->cmap->end   = index;
    FB_T(wk->visual->private)->cmap->len   = 1;

    FBputcmap(FB_T(wk->visual->private), FB_T(wk->visual->private)->cmap);
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

  switch (wk->inq.attr.planes) {
  case 1:
  case 2:
  case 4:
  case 8:
    /* fb->cmap is set by FBgetcmap */
    FBgetcmap(FB_T(wk->visual->private));
    ADEBUG("v_opnwk: FB cmap length %d allocated: %p\n",
	   FB_T(wk->visual->private)->cmap->len, FB_T(wk->visual->private)->cmap);

    for (i = 0; i < FB_T(wk->visual->private)->cmap->len; i++) {
      int ti = gem2tos_color(wk->inq.attr.planes, i);

      FB_T(wk->visual->private)->cmap->red[ti]   =
        (wk->vdi_cmap.red[i] * 65535) / 1000;
      FB_T(wk->visual->private)->cmap->green[ti]   =
        (wk->vdi_cmap.green[i] * 65535) / 1000;
      FB_T(wk->visual->private)->cmap->blue[ti]   =
        (wk->vdi_cmap.blue[i] * 65535) / 1000;
    }
    /* Put the changed cmap on to the FrameBuffer */
    FB_T(wk->visual->private)->cmap->start = 0;
    FB_T(wk->visual->private)->cmap->len = (1 << wk->inq.attr.planes);
    FB_T(wk->visual->private)->cmap->end = (1 << wk->inq.attr.planes) - 1;

    sleep(1);			/* This is apparently needed in 8 bit mode (oFBis bug?) */

    FBputcmap(FB_T(wk->visual->private), FB_T(wk->visual->private)->cmap);
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
  /* cmap not used in TrueColor mode */
  if (wk->inq.attr.planes < 16) {
    FBfreecmap(FB_T(wk->visual->private)->cmap);
    ADEBUG("v_clswk: FB cmap freed\n");
  }
}


int 
ofbis_native_colour(VWKREF wk,
                    int    c)
{
  switch(wk->inq.attr.planes)
  {
  case 1:
  case 2:
  case 4:
  case 8:
    return c;

  case 15:
  case 16:
  case 24:
  case 32:
    if(c > 255)
    {
      fprintf(stderr, "Illegal colour value %d\n", c);
      return FBc24_to_cnative(FB_T(wk->visual->private), 
			      COLOURS(wk->visual->private)[255]);
    }
    else
    {
      return FBc24_to_cnative(FB_T(wk->visual->private), 
                              COLOURS(wk->visual->private)[c]);
    }

  default:
    fprintf(stderr, "ofbis: Unknown colour depth: %d\n", wk->inq.attr.planes);

    return c;
  }
}
