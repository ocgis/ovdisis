/*
 * vdi_various.c
 *
 * Copyright 1998 Tomas Berndtsson <tomas@nocrew.org>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *  
 * Read the file COPYING for more information.
 *
 */

#include <stdio.h>

#include "ovdisis.h"
#include "various.h"
#include "vdi_various.h"

void vdi_vs_color(VDI_Workstation *vwk)
{
  int i,ni;

  i = min(vdipb->intin[0], 255);

  ni = gem2tos_color(vwk->inq.attr.planes, i);
  
  vwk->vdi_cmap.red[i]   = vdipb->intin[1];
  vwk->vdi_cmap.green[i] = vdipb->intin[2];
  vwk->vdi_cmap.blue[i]  = vdipb->intin[3];

  /* Don't put cmap if in TrueColor mode */
#if 0
  if(vwk->inq.attr.planes < 16) {
#endif
    VISUAL_SET_CMAP(vwk,
		    ni,
		    vwk->vdi_cmap.red[i],
		    vwk->vdi_cmap.green[i],
		    vwk->vdi_cmap.blue[i]);
#if 0
    }
#endif

  ADEBUG("vs_color: VDI colour %d is set to: %d, %d, %d\n",
	 i,vwk->vdi_cmap.red[i],vwk->vdi_cmap.green[i],vwk->vdi_cmap.blue[i]);

  if(vwk->inq.attr.planes < 16)
    ADEBUG("vs_color: Which means cmap colour %d is set to: %u, %u, %u\n",
	   ni, vwk->fb->cmap->red[ni], vwk->fb->cmap->green[ni], vwk->fb->cmap->blue[ni]);
  vdipb->contrl[N_PTSOUT] = 0;
  vdipb->contrl[N_INTOUT] = 0;
}

void vdi_vq_color(VDI_Workstation *vwk)
{
  int i,ni;

  i = vdipb->intin[0];

  if(i > vwk->dev.attr.colors)
    vdipb->intout[0] = -1;    /* Color out of range */
  else
  {
    ni = gem2tos_color(vwk->inq.attr.planes, i);
    vdipb->intout[0] = i;
    
    /* Return the actual colour */
    if(vdipb->intin[1] && (vwk->inq.attr.planes < 16))
    {
      int red, green, blue;

      VISUAL_GET_CMAP(vwk, ni, &red, &green, &blue);
      vdipb->intout[1] = red;
      vdipb->intout[2] = green;
      vdipb->intout[3] = blue;
    }
    else /* Return the set value, or value of virtual pen if TC mode */
    {
      vdipb->intout[1] = (short)vwk->vdi_cmap.red[i];
      vdipb->intout[2] = (short)vwk->vdi_cmap.green[i];
      vdipb->intout[3] = (short)vwk->vdi_cmap.blue[i];
      
      ADEBUG("vq_color: VDI colour %d was set to %u, %u, %u\n",
             i,vdipb->intout[1],vdipb->intout[2],vdipb->intout[3]);
    }
  }
  
  vdipb->contrl[N_PTSOUT] = 0;
  vdipb->contrl[N_INTOUT] = 4;
}

void vdi_v_get_pixel(VDI_Workstation *vwk)
{
  unsigned long ti;
  unsigned short tc;
  unsigned char *pti;

  ti = VISUAL_GET_PIXEL(vwk, vdipb->ptsin[0], vdipb->ptsin[1]);

  if(vwk->inq.attr.planes < 16)
    {
      vdipb->intout[0] = (unsigned short)ti;
      vdipb->intout[1] = (unsigned short)tos2gem_color(vwk->inq.attr.planes, ti);
    }
  else /* TrueColor mode */
    {
      /* FBgetpixel returns 24 bit Truecolor, 0xrr00ggbb */

      pti = (unsigned char *)&ti;
      tc = (((pti[0] << 8) & 0xf800) |
	    ((pti[2] << 3) & 0x07e0) |
	    ((pti[3] >> 3) & 0x001f));

      vdipb->intout[0] = 0;
      vdipb->intout[1] = tc;
    }

  vdipb->contrl[N_PTSOUT] = 0;
  vdipb->contrl[N_INTOUT] = 2;
}


/*
** Description
** Implementation of vex_timv. Just fill in the handler address in the vdi
** workstation and let the handler do the calls.
**
** 1998-12-26 CG
** 1999-05-22 CG
*/
void
vdi_vex_timv (VDI_Workstation *vwk) {
  /* Return the old vector */
  vdipb->contrl[9] = MSW(vwk->timv);
  vdipb->contrl[10] = LSW(vwk->timv);
  
  /* Setup the new vector */
  vwk->timv = (void *)
    (((unsigned short)vdipb->contrl[7] << 16) +
     (unsigned short)vdipb->contrl[8]);

  /* Milliseconds per timer tick are hardcoded to 50 */
  vdipb->intout[0] = 50;

  /* Return number of returned values */
  vdipb->contrl[N_PTSOUT] = 0;
  vdipb->contrl[N_INTOUT] = 1;
}


void vdi_vq_key_s(VDI_Workstation *vwk)
{
  EDEBUG("vq_key_s: Call not implemented!\n");

  /* Return number of returned values */
  vdipb->contrl[N_PTSOUT] = 0;
  vdipb->contrl[N_INTOUT] = 1;
}

void vdi_vqin_mode(VDI_Workstation *vwk)
{
  EDEBUG("vqin_mode: Call not implemented!\n");

  vdipb->contrl[N_PTSOUT] = 0;
  vdipb->contrl[N_INTOUT] = 1;
}


