/*
 * vdi_raster.c
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

#define DEBUGLEVEL 0

#include <stdio.h>

#include "ovdisis.h"
#include "various.h"

#define USHORTS_TO_POINTER(m,l) ((void *)(((unsigned long)(m)<<16) | ((unsigned long)(l&0xffff))))

void vdi_vro_cpyfm(VDI_Workstation *vwk)
{
  MFDB *src,*dst;
  RECT srccor, dstcor;

  IDEBUG("vdi_vro_cpyfm entered\n");
  srccor.x1 = vdipb->ptsin[0];
  srccor.y1 = vdipb->ptsin[1];
  srccor.x2 = vdipb->ptsin[2];
  srccor.y2 = vdipb->ptsin[3];
  dstcor.x1 = vdipb->ptsin[4];
  dstcor.y1 = vdipb->ptsin[5];
  dstcor.x2 = vdipb->ptsin[6];
  dstcor.y2 = vdipb->ptsin[7];

  src = (MFDB *)USHORTS_TO_POINTER(vdipb->contrl[7],vdipb->contrl[8]);
  dst = (MFDB *)USHORTS_TO_POINTER(vdipb->contrl[9],vdipb->contrl[10]);

  VISUAL_BITBLT(vwk, vdipb->intin[0], &srccor, &dstcor, src, dst);

  vdipb->contrl[N_PTSOUT] = 0;
  vdipb->contrl[N_INTOUT] = 0;
  IDEBUG("vdi_vro_cpyfm exited\n");
}

void vdi_vrt_cpyfm(VDI_Workstation *vwk)
{
  MFDB *src,*dst;
  RECT srccor, dstcor;

  srccor.x1 = vdipb->ptsin[0];
  srccor.y1 = vdipb->ptsin[1];
  srccor.x2 = vdipb->ptsin[2];
  srccor.y2 = vdipb->ptsin[3];
  dstcor.x1 = vdipb->ptsin[4];
  dstcor.y1 = vdipb->ptsin[5];
  dstcor.x2 = vdipb->ptsin[6];
  dstcor.y2 = vdipb->ptsin[7];

  /* order the coordinates as I like them. */
  fix_rect(&srccor);
  fix_rect(&dstcor);

  src = (MFDB *)USHORTS_TO_POINTER(vdipb->contrl[7],vdipb->contrl[8]);
  dst = (MFDB *)USHORTS_TO_POINTER(vdipb->contrl[9],vdipb->contrl[10]);

  VISUAL_BITBLTT(vwk,
                 vdipb->intin[0],
                 vdipb->intin[1],
                 vdipb->intin[2],
                 &srccor,
                 &dstcor,
                 src,
                 dst);

  vdipb->contrl[N_PTSOUT] = 0;
  vdipb->contrl[N_INTOUT] = 0;
}

void vdi_vr_trnfm(VDI_Workstation *vwk)
{
  EDEBUG("vr_trnfm: Call not implemented!\n");

  vdipb->contrl[N_PTSOUT] = 0;
  vdipb->contrl[N_INTOUT] = 0;
}

