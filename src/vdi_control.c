/*
 * vdi_control.c
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
#include <stdlib.h>

#include "ovdisis.h"
#include "various.h"
#include "inits.h"

void vdi_v_clrwk(VDI_Workstation *);	/* Used by v_opnwk */

VDIWK wk[MAX_HANDLES];
int wk_open[MAX_HANDLES];
int first=1; /* Used to clear the above array first time */

/* GDOS function */
/* vwk not used by this function */
void vdi_v_opnwk(VDI_Workstation *vwk)
{
  int w;

  if(first) {
    for(w=0 ; w<MAX_HANDLES ; w++)
      wk_open[w] = WS_NOTOPEN;
    first = 0;
  }

  /* find first free handle */
  w=0;
  while (w < MAX_HANDLES && wk_open[w] != WS_NOTOPEN)
    w++;

  if (w == MAX_HANDLES) {
    vdipb->contrl[VDI_HANDLE] = 0;	/* Could not open workstation */
    EDEBUG("v_opnwk: No handles left to open physical workstation, sorry!\n");
    return;
  }
  if ((wk[w].physical = (VDI_Workstation *) malloc((size_t) sizeof(VDI_Workstation))) == NULL) {
    vdipb->contrl[VDI_HANDLE] = 0;	/* Could not open workstation */
    EDEBUG("v_opnwk: Not enough memory for physical workstation!\n");
    return;
  }
  if ((wk[w].physical->fb = FBopen(NULL, FB_OPEN_NEW_VC | FB_NO_KBD)) == NULL) {
    free(wk[w].physical);
    vdipb->contrl[VDI_HANDLE] = 0;	/* Could not open workstation */
    EDEBUG("v_opnwk: Error opening FrameBuffer!\n");
    return;
  }

  vdipb->contrl[VDI_HANDLE] = w + 1;	/* new vdi handle */
  wk_open[w] = WS_PHYSICAL;
  wk[w].vwk = wk[w].physical;	/* Link workstation to physical */

  wk[w].physical->handle = w + 1;	/* used for debugging */

  ADEBUG("v_opnwk: Physical workstation, handle %d opened: %p\n", w + 1, wk[w].physical);
  ADEBUG("v_opnwk: FrameBuffer opened: %p\n", wk[w].physical->fb);

  /* Setting default values */

  /* v_opnvwk and vq_extnd values */
  init_workstation(wk[w].physical);

  wk[w].physical->write_mode = MD_REPLACE;
  wk[w].physical->clip.x1 = 0;
  wk[w].physical->clip.y1 = 0;
  wk[w].physical->clip.x2 = wk[w].physical->dev.attr.xres;
  wk[w].physical->clip.y2 = wk[w].physical->dev.attr.yres;

  IDEBUG("v_opnwk: Number of bitplanes: %d\n", wk[w].physical->inq.attr.planes);

  vdi_v_clrwk(wk[w].physical);	/* Clear screen */

  /* Set default things */
  init_cmap(wk[w].physical);
  init_marker(wk[w].physical);
  init_line(wk[w].physical);
  init_fill(wk[w].physical);
  init_text(wk[w].physical);

  ADEBUG("v_opnwk: Default palette set\n");

  vdipb->contrl[N_PTSOUT] = 6;
  vdipb->contrl[N_INTOUT] = 45;
}

/* GDOS function */
/* vwk not used */
void vdi_v_clswk(VDI_Workstation *vwk)
{
  int i, w;

  w = vdipb->contrl[VDI_HANDLE] - 1;

  if (wk_open[w] != WS_PHYSICAL) {
    if (wk_open[w])
      EDEBUG("v_clswk: Physical workstation, handle %d not open!", w + 1);
    else
      EDEBUG("v_clswk: Handle %d is not a physical workstation!", w + 1);
    return;
  }
  /* cmap not used in TrueColor mode */
  if (wk[w].physical->inq.attr.planes < 16) {
    FBfreecmap(wk[w].physical->fb->cmap);
    ADEBUG("v_clswk: FB cmap freed\n");
  }
  FBclose(wk[w].physical->fb);
  ADEBUG("v_clswk: FrameBuffer closed\n");

  /* Free virtual workstations left open */
  /* Will closing a physical workstation
     also close all its virtual ? */
  /* It will here! */
  for (i = 0; i < MAX_HANDLES; i++)
    if (wk_open[i] && (wk[i].physical == wk[w].physical)) {
      free(wk[i].vwk);
      wk_open[i] = WS_NOTOPEN;
      ADEBUG("v_clswk: Freed workstation, handle %d\n", i + 1);
    }
  ADEBUG("v_clswk: Freed physical workstation, handle %d\n", w + 1);

  /* Above will also free the actual physical ws */

  /* Free physical VDI_Workstation */
  /*
     free(wk[w].physical);
     ADEBUG("v_clswk: Freed physical workstation, handle %d\n",w+1);
   */

  wk_open[w] = WS_NOTOPEN;
}




/* This function does not open a framebuffer,
   but only allocate a new virtual workstation. */
void vdi_v_opnvwk(VDI_Workstation *vwk)
{
  int i, v = 0, w;

  w = vdipb->contrl[VDI_HANDLE] - 1;	/* Physical workstation */

  if (wk_open[w] != WS_PHYSICAL) {
    vdipb->contrl[VDI_HANDLE] = 0;	/* Could not open virtual workstation */
    EDEBUG("v_opnvwk: Handle %d is not a physical workstation!\n", w + 1);
    return;
  }
  /* find first free handle */
  while (v < MAX_HANDLES && wk_open[v] != WS_NOTOPEN)
    v++;

  /* none free found */
  if (v == MAX_HANDLES) {
    vdipb->contrl[VDI_HANDLE] = 0;	/* Could not open virtual workstation */
    EDEBUG("v_opnvwk: We're all out of handles, I'm afraid!\n");
    return;
  }
  /* malloc space for new virtual workstation */
  if ((wk[v].vwk = (VDI_Workstation *) malloc((size_t) sizeof(VDI_Workstation))) == NULL) {
    vdipb->contrl[VDI_HANDLE] = 0;	/* Could not open virtual workstation */
    EDEBUG("v_opnvwk: Not enough memory for new workstation!\n");
    return;
  }
  ADEBUG("v_opnvwk: New virtual workstation, handle %d opened linked to %d\n", v + 1, w + 1);

  vdipb->contrl[VDI_HANDLE] = v + 1;	/* new vdi handle */
  wk_open[v] = WS_VIRTUAL;
  wk[v].physical = wk[w].physical;	/* link virtual to physical, sort of */
  wk[v].vwk->fb = wk[v].physical->fb;	/* same FB as physical workstation */

  wk[v].vwk->handle = v + 1;	/* used for debugging */

  /* Copy some things */
  copy_workstation(wk[v].physical, wk[v].vwk);
  copy_cmap(wk[v].physical, wk[v].vwk);
  copy_marker(wk[v].physical, wk[v].vwk);
  copy_line(wk[v].physical, wk[v].vwk);
  copy_fill(wk[v].physical, wk[v].vwk);
  copy_text(wk[v].physical, wk[v].vwk);

  /* make the changes the user wants */
  /* intin[0] - workstation (1-10 == screen) */
  wk[v].vwk->line_a.type = (int)vdipb->intin[1];
  wk[v].vwk->line_a.color = (int)vdipb->intin[2];
  wk[v].vwk->marker_a.type = (int)vdipb->intin[3];
  wk[v].vwk->marker_a.color = (int)vdipb->intin[4];
  wk[v].vwk->text_a.fontid = (int)vdipb->intin[5];
  wk[v].vwk->text_a.color = (int)vdipb->intin[6];
  wk[v].vwk->fill_a.interior = (int)vdipb->intin[7];
  wk[v].vwk->fill_a.style = (int)vdipb->intin[8];
  wk[v].vwk->fill_a.color = (int)vdipb->intin[9];
  /* intin[10] - Coordinate system.  Only used in GDOS */

  wk[v].vwk->write_mode = wk[v].physical->write_mode;
  wk[v].vwk->clip.x1 = wk[v].physical->clip.x1;
  wk[v].vwk->clip.y1 = wk[v].physical->clip.y1;
  wk[v].vwk->clip.x2 = wk[v].physical->clip.x2;
  wk[v].vwk->clip.y2 = wk[v].physical->clip.y2;

  /* v_opnvwk values to return */
  for (i = 0; i < 45; i++)
    vdipb->intout[i] = wk[v].vwk->dev.tab[i] = wk[v].physical->dev.tab[i];
  for (i = 0; i < 12; i++)
    vdipb->ptsout[i] = wk[v].vwk->dev.tab[i + 45] = wk[v].physical->dev.tab[i + 45];

  vdipb->contrl[N_PTSOUT] = 6;
  vdipb->contrl[N_INTOUT] = 45;
}

void vdi_v_clsvwk(VDI_Workstation *vwk)
{
  int v;

  v = vdipb->contrl[VDI_HANDLE] - 1;

  if (wk_open[v] != WS_VIRTUAL) {
    if (wk_open[v])
      EDEBUG("v_clsvwk: Handle %d is not a virtual workstation\n", v + 1);
    else
      EDEBUG("v_clsvwk: Handle %d is not open\n", v + 1);
    return;
  }
  free(wk[v].vwk);
  wk_open[v] = WS_NOTOPEN;

  ADEBUG("v_clsvwk: Virtual workstation, handle %d freed\n", v + 1);
}

void vdi_v_clrwk(VDI_Workstation *vwk)
{
  unsigned long fill, *mem, len, *end;

  if (vwk->inq.attr.planes < 16)
    fill = 0L;
  else				/* TrueColor mode */
    fill = 0xffdfffdfL;		/* TOS way, I'd rather have 0xffffffffL */

  mem = (unsigned long *)vwk->fb->sbuf;

  len = ((unsigned long)(vwk->dev.attr.xres + 1) *
	 (unsigned long)(vwk->dev.attr.yres + 1) *
	 (unsigned long)vwk->inq.attr.planes) / 8;

  end = (unsigned long *)((unsigned long)vwk->fb->sbuf + len);

  ADEBUG("v_clrwk: Filling screen %p to %p (%ld) with %08lx\n", mem, end, len, fill);

  for (; mem < end; mem++)
    *mem = fill;
}

void vdi_v_updwk(VDI_Workstation *vwk)
{
  EDEBUG("v_updwk: Call not implemented!\n");
}

void vdi_vs_clip(VDI_Workstation *vwk)
{
  if (vdipb->intin[0]) {	/* Clipping on */
    vwk->clip.x1 = vdipb->ptsin[0];
    vwk->clip.y1 = vdipb->ptsin[1];
    vwk->clip.x2 = vdipb->ptsin[2];
    vwk->clip.y2 = vdipb->ptsin[3];
    fix_rect(&vwk->clip);
    vwk->clip.x1 = max(0, vdipb->ptsin[0]);
    vwk->clip.y1 = max(0, vdipb->ptsin[1]);
    vwk->clip.x2 = min(vwk->dev.attr.xres, vdipb->ptsin[2]);
    vwk->clip.y2 = min(vwk->dev.attr.yres, vdipb->ptsin[3]);
  } else {			/* Clipping off */
    vwk->clip.x1 = 0;
    vwk->clip.y1 = 0;
    vwk->clip.x2 = vwk->dev.attr.xres;
    vwk->clip.y2 = vwk->dev.attr.yres;
  }
  ADEBUG("vs_clip: Clipping for handle %d set to: %d, %d, %d, %d.\n",
    vwk->handle, vwk->clip.x1, vwk->clip.y1, vwk->clip.x2, vwk->clip.y2);
}

void vdi_vswr_mode(VDI_Workstation *vwk)
{
  vwk->write_mode = vdipb->intin[0];
  vdipb->intout[0] = vwk->write_mode;

  vdipb->contrl[N_INTOUT] = 1;
}

void vdi_vq_extnd(VDI_Workstation *vwk)
{
  int i;

  if (vdipb->intin[0]) {	/* extended answers */
    for (i = 0; i < 45; i++)
      vdipb->intout[i] = vwk->inq.tab[i];
    for (i = 0; i < 12; i++)
      vdipb->ptsout[i] = vwk->inq.tab[i + 45];
  } else {			/* same answers as from v_opnvwk */
    for (i = 0; i < 45; i++)
      vdipb->intout[i] = vwk->dev.tab[i];
    for (i = 0; i < 12; i++)
      vdipb->ptsout[i] = vwk->dev.tab[i + 45];
  }

  vdipb->contrl[N_PTSOUT] = 6;
  vdipb->contrl[N_INTOUT] = 45;
}

