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

#undef DEBUGLEVEL

#include <stdio.h>
#include <string.h>

#include "ovdisis.h"
#include "various.h"
#include "vdi_text.h"


/* Note! No clipping performed! */

void vdi_v_gtext(VDI_Workstation *vwk)
{
  int ch, i,endchar, ni, x,y,ly, w;
  unsigned long col;

  /* Lock visual before operation */
  VISUAL_MUTEX(vwk, VISUAL_MUTEX_LOCK);

  /* Setup write mode */
  VISUAL_SET_WRITE_MODE(vwk, vwk->write_mode);

  ni = gem2tos_color(vwk->inq.attr.planes, vwk->text_a.color);
  col = get_color(vwk, ni);
 
  x = vdipb->ptsin[0];
  y = vdipb->ptsin[1] - vwk->text_a.font->top;

  w = vdipb->contrl[N_INTIN] * vwk->text_a.cellwidth;

  /* Do horizontal alignment */
  switch(vwk->text_a.h_alignment) {
  case 0: /* Left */
    break;
  case 1: /* Center */
    x -= w/2;
    break;
  case 2: /* Right */
    x -= w;
    break;
  default: /* Illegal horizontal alignment, shouldn't occur here! */
    break;
  }

  /* Do vertical alignment */
  switch(vwk->text_a.v_alignment) {
  case 0: /* Base line */
    break;
  case 1: /* Half line */
    y += vwk->text_a.font->half;
    break;
  case 2: /* Ascent line */
    y += vwk->text_a.font->ascent;
    break;
  case 3: /* Bottom line */
    y -= vwk->text_a.font->bottom;
    break;
  case 4: /* Descent line */
    y -= vwk->text_a.font->descent;
    break;
  case 5: /* Top line */
    y += vwk->text_a.font->top;
    break;
  default: /* Illegal vertical alignment, shouldn't occur here! */
    break;
  }

  /* y position for underline */
  ly = y + vwk->text_a.font->top + vwk->text_a.font->descent;


  i = 0;
  endchar = vdipb->contrl[N_INTIN];

  /* Poor mans clipping */
  /* Skips whole characters if they are outside the framebuffer */
  if((y < 0) || (y + vwk->text_a.cellheight > vwk->dev.attr.yres))
    return;
  while(x < 0) {
    i++;
    x += vwk->text_a.cellwidth;
    w--;
  }
  while(x+w > vwk->dev.attr.xres) {
    endchar--;
    w -= vwk->text_a.cellwidth;
  }

  /* Set the current font in the visual. */
  VISUAL_SET_FONT (vwk, vwk->text_a.font->data, vwk->text_a.font->wcell, 
		   vwk->text_a.font->formheight);

  /* Draw the text, one character at a time */
  for( ; i < endchar ; i++) {
    ch = vdipb->intin[i] & 0xff;
    VISUAL_PUT_CHAR(vwk, x, y, col, ch);

    if(vwk->text_a.effects & THICKENED)
      VISUAL_PUT_CHAR(vwk, x+1, y, col, ch);

    if(vwk->text_a.effects & UNDERLINED)
      VISUAL_HLINE (vwk, x, x+vwk->text_a.cellwidth, ly, col);

    x += vwk->text_a.cellwidth;
  }

  /* Unlock visual after operation */
  VISUAL_MUTEX(vwk, VISUAL_MUTEX_UNLOCK);

  vdipb->contrl[N_PTSOUT] = 0;
  vdipb->contrl[N_INTOUT] = 0;
}

void vdi_vst_color(VDI_Workstation *vwk)
{
  vwk->text_a.color=(int)vdipb->intin[0];
  vdipb->intout[0]=(short)vwk->text_a.color;

  vdipb->contrl[N_PTSOUT]=0;
  vdipb->contrl[N_INTOUT]=1;
}

void vdi_vst_height(VDI_Workstation *vwk)
{
  EDEBUG("vst_height: Call not implemented!\n");
  vdipb->contrl[N_PTSOUT]=2;
  vdipb->contrl[N_INTOUT]=0;
}


/*
** Exported
**
** 1998-12-26 CG
** 1999-05-22 CG
*/
void
vdi_vst_font (VDI_Workstation * vwk) {
  EDEBUG("vst_font: Call not implemented!\n");
  vdipb->contrl[N_PTSOUT]=0;
  vdipb->contrl[N_INTOUT]=1;
}


/* This should really only check within the same font ID. */
void vdi_vst_point(VDI_Workstation *vwk)
{
  FontInfo *fp;

  ADEBUG ("ovdisis: vdi_text.c: vdi_vst_point: enter\n");
  fp = vwk->fonts;

  /* It might be necessary later to go through the whole list
     in order to find the best match. */
  while(fp->next && fp->next->size <= vdipb->intin[0]) {
    fp = fp->next;
  }

  vdipb->ptsout[0] = vwk->text_a.textwidth = fp->wchar;
  vdipb->ptsout[1] = vwk->text_a.textheight = fp->top + fp->bottom;
  vdipb->ptsout[2] = vwk->text_a.cellwidth = fp->wcell;
  vdipb->ptsout[3] = vwk->text_a.cellheight = fp->formheight;
  vwk->text_a.font = fp;

  vdipb->intout[0] = fp->size;

  IDEBUG("vst_point: Changed point size to %d\n",fp->size);

  vdipb->contrl[N_PTSOUT]=2;
  vdipb->contrl[N_INTOUT]=1;
}

void vdi_vst_effects(VDI_Workstation *vwk)
{
  vwk->text_a.effects=(int)vdipb->intin[0];
  vdipb->intout[0]=(short)vwk->text_a.effects;

  vdipb->contrl[N_PTSOUT]=0;
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

  vdipb->contrl[N_PTSOUT]=0;
  vdipb->contrl[N_INTOUT]=2;
}

void vdi_vst_rotation(VDI_Workstation *vwk)
{
  vwk->text_a.rotation=(int)vdipb->intin[0];
  vdipb->intout[0]=(short)vwk->text_a.rotation;

  vdipb->contrl[N_PTSOUT]=0;
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

  vdipb->contrl[N_PTSOUT] = 2;
  vdipb->contrl[N_INTOUT] = 6;
}

/* This is made really simple now, since we only use monospaced fonts */
void vdi_vqt_extent(VDI_Workstation *vwk)
{
  int width,height;

  /* Doesn't take rotation into acount yet! */

  width = vwk->text_a.font->wcell * vdipb->contrl[N_INTIN];
  height = vwk->text_a.font->formheight;

  vdipb->ptsout[0] = 0;
  vdipb->ptsout[1] = 0;
  vdipb->ptsout[2] = width;
  vdipb->ptsout[3] = 0;
  vdipb->ptsout[4] = width;
  vdipb->ptsout[5] = height;
  vdipb->ptsout[6] = 0;
  vdipb->ptsout[7] = height;

  vdipb->contrl[N_PTSOUT] = 4;
  vdipb->contrl[N_INTOUT] = 0;
}

/* Simple for monospaced fonts */
void vdi_vqt_width(VDI_Workstation *vwk)
{
  vdipb->ptsout[0] = vwk->text_a.font->wcell;
  vdipb->ptsout[2] = 0;
  vdipb->ptsout[4] = 0;

  vdipb->contrl[N_PTSOUT] = 3;
  vdipb->contrl[N_INTOUT] = 1;
  /* FIXME: Something should be written in intout[0] */
}

/* I'm not sure I'm doing this the correct way. 
   Will have to check it in TOS at some point! */
void vdi_vqt_name(VDI_Workstation *vwk)
{
  int i;
  FontInfo *fp = vwk->fonts;

  for(i=0 ; i<vdipb->intin[0] ; i++) {
    if(fp->next)
      fp = fp->next;
  }

  for(i=0 ; i<32 ; i++)
    vdipb->intout[i + 1] = (short)fp->name[i];

  /* Bitmap font, actually only used by FSMGDOS and SpeedoGDOS,
     but I set it anyway, just for the fun of it. */
  vdipb->intout[32 + 1] = 0;

  vdipb->intout[0] = fp->id;

  vdipb->contrl[N_PTSOUT] = 0;
  vdipb->contrl[N_INTOUT] = 34;
}

void vdi_vqt_fontinfo(VDI_Workstation *vwk)
{
  vdipb->intout[0] = vwk->text_a.font->first;
  vdipb->intout[1] = vwk->text_a.font->last;
  vdipb->ptsout[0] = vwk->text_a.font->wcell;
  vdipb->ptsout[1] = vwk->text_a.font->bottom;
  vdipb->ptsout[3] = vwk->text_a.font->descent;
  vdipb->ptsout[5] = vwk->text_a.font->half;
  vdipb->ptsout[7] = vwk->text_a.font->ascent;
  vdipb->ptsout[2] = vwk->text_a.font->left;
  vdipb->ptsout[4] = vwk->text_a.font->right;
  vdipb->ptsout[6] = vdipb->ptsout[2] + vdipb->ptsout[4];

  vdipb->contrl[N_PTSOUT] = 4;
  vdipb->contrl[N_INTOUT] = 2;
}



