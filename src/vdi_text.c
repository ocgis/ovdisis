/*
 * vdi_text.c
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

/* Note! No clipping performed! */

void vdi_v_gtext(VDI_Workstation *vwk)
{
  int ch, i, ni, x,y;
  unsigned long col;

  ni = gem2tos_color(vwk->inq.attr.planes, vwk->text_a.color);
  col = get_color(vwk, ni);
 
  x = vdipb->ptsin[0];
  y = vdipb->ptsin[1];

  /* Do alignment stuff here */

  for(i = 0 ; i < vdipb->contrl[N_INTIN] ; i++) {
    ch = vdipb->intin[i];
    FBputchar(vwk->fb, x, y, col, 0, ch&0xff);

    if(vwk->text_a.effects & UNDERLINED) {
      FBhline(vwk->fb, x, x+vwk->text_a.cellwidth, 
	      y+vwk->text_a.cellheight - vwk->text_a.font->descent, col);
    }

    x += vwk->text_a.cellwidth;
  }
}

void vdi_vst_color(VDI_Workstation *vwk)
{
  vwk->text_a.color=(int)vdipb->intin[0];
  vdipb->intout[0]=(short)vwk->text_a.color;

  vdipb->contrl[N_INTOUT]=1;
}

void vdi_vst_height(VDI_Workstation *vwk)
{
  EDEBUG("vst_height: Call not implemented!\n");
}

/* This should really only check within the same font ID. */
void vdi_vst_point(VDI_Workstation *vwk)
{
  FontInfo *fp=vwk->fonts;
  FBFONT fnt;

  /* It might be necessary later to go through the whole list
     in order to find the best match. */
  while(fp->next && fp->next->size <= vdipb->intin[0]) {
    fp = fp->next;
  }

  /* We have found a best match in fp, now tell oFBis */
  fnt.data = fp->data;
  fnt.width = fp->wcell;
  fnt.height = fp->formheight;
  FBsetfont(vwk->fb, &fnt);

  vdipb->ptsout[0] = vwk->text_a.textwidth = fp->wchar;
  vdipb->ptsout[1] = vwk->text_a.textheight = fp->top + fp->bottom;
  vdipb->ptsout[2] = vwk->text_a.cellwidth = fp->wcell;
  vdipb->ptsout[3] = vwk->text_a.cellheight = fp->formheight;
  vwk->text_a.font = fp;

  vdipb->intout[0] = fp->size;

  IDEBUG("vst_point: Changed point size to %d\n",fp->size);

  vdipb->contrl[N_INTOUT]=1;
  vdipb->contrl[N_PTSOUT]=4;
}

void vdi_vst_effects(VDI_Workstation *vwk)
{
  vwk->text_a.effects=(int)vdipb->intin[0];
  vdipb->intout[0]=(short)vwk->text_a.effects;

  vdipb->contrl[N_INTOUT]=1;
}

void vdi_vst_alignment(VDI_Workstation *vwk)
{
  if(vdipb->intin[0] < 0 || vdipb->intin[0] > 2)
    vwk->text_a.h_alignment=0; /* left alignment if illegal specified */
  else
    vwk->text_a.h_alignment=(int)vdipb->intin[0];

  if(vdipb->intin[1] < 0 || vdipb->intin[1] > 5)
    vwk->text_a.v_alignment=0; /* baseline alignment if illegal specified */
  else
    vwk->text_a.v_alignment=(int)vdipb->intin[1];

  vdipb->intout[0]=(short)vwk->text_a.h_alignment;
  vdipb->intout[1]=(short)vwk->text_a.v_alignment;

  vdipb->contrl[N_INTOUT]=2;
}

void vdi_vst_rotation(VDI_Workstation *vwk)
{
  vwk->text_a.rotation=(int)vdipb->intin[0];
  vdipb->intout[0]=(short)vwk->text_a.rotation;

  vdipb->contrl[N_INTOUT]=1;
}

void vdi_vqt_attributes(VDI_Workstation *vwk)
{
  vdipb->ptsout[0]=(short)vwk->text_a.textwidth;
  vdipb->ptsout[1]=(short)vwk->text_a.textheight;
  vdipb->ptsout[2]=(short)vwk->text_a.cellwidth;
  vdipb->ptsout[3]=(short)vwk->text_a.cellheight;

  vdipb->intout[0]=(short)vwk->text_a.fontid;
  vdipb->intout[1]=(short)vwk->text_a.color;
  vdipb->intout[2]=(short)vwk->text_a.rotation;
  vdipb->intout[3]=(short)vwk->text_a.h_alignment;
  vdipb->intout[4]=(short)vwk->text_a.v_alignment;
  vdipb->intout[5]=(short)vwk->write_mode;

  vdipb->contrl[N_PTSOUT]=2;
  vdipb->contrl[N_INTOUT]=6;
}

void vdi_vqt_extent(VDI_Workstation *vwk)
{
  EDEBUG("vqt_extent: Call not implemented!\n");
}

void vdi_vqt_width(VDI_Workstation *vwk)
{
  EDEBUG("vqt_width: Call not implemented!\n");
}








