/*
 * vdibind.c
 *
 * Copyright 1998 Tomas Berndtsson <tomas@nocrew.org>
 * Copyright 1999 - 2000 Christer Gustavsson <cg@nocrew.org>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *  
 * Read the file COPYING for more information.
 *
 */

#include "ovdisis_types.h"
#include "vdibind.h"

#define VDIBIND_REENTRANT 1

/* set contrl[N_INTIN] & contrl[N_PTSIN] too ?? */

#define MSW(a) ((unsigned short)((unsigned long)(a) >> 16))
#define LSW(a) ((unsigned short)((unsigned long)(a) & 0xffffUL))


#define _DEFINE_VPB \
  VDIPARBLK e_vdiparblk; \
  VDIPB o_vdipb = { e_vdiparblk.contrl, \
		    e_vdiparblk.intin, e_vdiparblk.ptsin, \
		    e_vdiparblk.intout, e_vdiparblk.ptsout }

#ifdef VDIBIND_REENTRANT
#define DEFINE_VPB _DEFINE_VPB
#else
#define DEFINE_VPB
_DEFINE_VPB;
#endif

extern void vdi_call(VDIPB *);

void v_opnwk(int work_in[11], int *handle, int work_out[57])
{
  DEFINE_VPB;
  int i;

  for(i=0 ; i<11 ; i++)
    o_vdipb.intin[i] = work_in[i];

  o_vdipb.contrl[ROUTINE] = 1;
  o_vdipb.contrl[N_PTSIN] = 0;
  o_vdipb.contrl[N_INTIN] = 11;
  vdi_call(&o_vdipb);

  for(i=0 ; i<45 ; i++)
    work_out[i] = o_vdipb.intout[i];
  for(i=0 ; i<12 ; i++)
    work_out[i+45] = o_vdipb.ptsout[i];

  *handle = o_vdipb.contrl[VDI_HANDLE];
}

void v_clswk(int handle)
{
  DEFINE_VPB;
  o_vdipb.contrl[VDI_HANDLE] = handle;
  o_vdipb.contrl[ROUTINE] = 2;
  o_vdipb.contrl[N_PTSIN] = 0;
  o_vdipb.contrl[N_INTIN] = 0;
  vdi_call(&o_vdipb);
}

void v_opnvwk(int work_in[11], int *handle, int work_out[57])
{
  DEFINE_VPB;
  int i;

  for(i=0 ; i<11 ; i++)
    o_vdipb.intin[i] = work_in[i];

  o_vdipb.contrl[VDI_HANDLE] = *handle;
  o_vdipb.contrl[ROUTINE] = 100;
  o_vdipb.contrl[N_PTSIN] = 0;
  o_vdipb.contrl[N_INTIN] = 11;
  vdi_call(&o_vdipb);

  for(i=0 ; i<45 ; i++)
    work_out[i] = o_vdipb.intout[i];
  for(i=0 ; i<12 ; i++)
    work_out[i+45] = o_vdipb.ptsout[i];

  *handle = o_vdipb.contrl[VDI_HANDLE];
}

void v_clsvwk(int handle)
{
  DEFINE_VPB;
  o_vdipb.contrl[VDI_HANDLE] = handle;
  o_vdipb.contrl[ROUTINE] = 101;
  o_vdipb.contrl[N_PTSIN] = 0;
  o_vdipb.contrl[N_INTIN] = 0;
  vdi_call(&o_vdipb);
}

void v_clrwk(int handle)
{
  DEFINE_VPB;
  o_vdipb.contrl[VDI_HANDLE] = handle;
  o_vdipb.contrl[ROUTINE] = 3;
  o_vdipb.contrl[N_PTSIN] = 0;
  o_vdipb.contrl[N_INTIN] = 0;
  vdi_call(&o_vdipb);
}

void v_updwk(int handle)
{
  DEFINE_VPB;
  o_vdipb.contrl[VDI_HANDLE] = handle;
  o_vdipb.contrl[ROUTINE] = 4;
  o_vdipb.contrl[N_PTSIN] = 0;
  o_vdipb.contrl[N_INTIN] = 0;
  vdi_call(&o_vdipb);
}

void vs_clip(int handle, int clipping, int pxy[4])
{
  DEFINE_VPB;
  int i;

  o_vdipb.intin[0] = clipping;

  if(clipping != 0) /* ignore pxy if clipping is off */
    for(i=0 ; i<4 ; i++)
      o_vdipb.ptsin[i] = pxy[i];

  o_vdipb.contrl[VDI_HANDLE] = handle;
  o_vdipb.contrl[ROUTINE] = 129;
  o_vdipb.contrl[N_PTSIN] = 2;
  o_vdipb.contrl[N_INTIN] = 1;
  vdi_call(&o_vdipb);
}

int vswr_mode(int handle, int mode)
{
  DEFINE_VPB;
  o_vdipb.intin[0] = mode;

  o_vdipb.contrl[VDI_HANDLE] = handle;
  o_vdipb.contrl[ROUTINE] = 32;
  o_vdipb.contrl[N_PTSIN] = 0;
  o_vdipb.contrl[N_INTIN] = 1;
  vdi_call(&o_vdipb);

  return o_vdipb.intout[0];
}

void vs_color(int handle, int index, int rgb[3])
{
  DEFINE_VPB;
  o_vdipb.intin[0] = index;
  o_vdipb.intin[1] = rgb[0];
  o_vdipb.intin[2] = rgb[1];
  o_vdipb.intin[3] = rgb[2];

  o_vdipb.contrl[VDI_HANDLE] = handle;
  o_vdipb.contrl[ROUTINE] = 14;
  o_vdipb.contrl[N_PTSIN] = 0;
  o_vdipb.contrl[N_INTIN] = 4;
  vdi_call(&o_vdipb);
}

void v_gtext(int handle, int x, int y, char *string)
{
  DEFINE_VPB;
  int i=0;

  o_vdipb.ptsin[0] = x;
  o_vdipb.ptsin[1] = y;
  
  while((o_vdipb.intin[i++] = (int)*((unsigned char *)string)++) != 0);

  o_vdipb.contrl[N_PTSIN] = 1;
  o_vdipb.contrl[N_INTIN] = --i;

  o_vdipb.contrl[VDI_HANDLE] = handle;
  o_vdipb.contrl[ROUTINE] = 8;
  vdi_call(&o_vdipb);
}

int vst_color(int handle, int index)
{
  DEFINE_VPB;
  o_vdipb.intin[0] = index;

  o_vdipb.contrl[VDI_HANDLE] = handle;
  o_vdipb.contrl[ROUTINE] = 22;
  o_vdipb.contrl[N_PTSIN] = 0;
  o_vdipb.contrl[N_INTIN] = 1;
  vdi_call(&o_vdipb);

  return o_vdipb.intout[0];
}

void vst_height(int handle, int height, int *t_width, int *t_height,
		int *c_width, int *c_height)
{
  DEFINE_VPB;
  o_vdipb.ptsin[0] = 0;
  o_vdipb.ptsin[1] = height;

  o_vdipb.contrl[VDI_HANDLE] = handle;
  o_vdipb.contrl[ROUTINE] = 12;
  o_vdipb.contrl[N_PTSIN] = 1;
  o_vdipb.contrl[N_INTIN] = 0;
  vdi_call(&o_vdipb);

  *t_width = o_vdipb.ptsout[0];
  *t_height = o_vdipb.ptsout[1];
  *c_width = o_vdipb.ptsout[2];
  *c_height = o_vdipb.ptsout[3];
}

int vst_point(int handle, int height, int *t_width, int *t_height,
		int *c_width, int *c_height)
{
  DEFINE_VPB;
  o_vdipb.intin[0] = height;

  o_vdipb.contrl[VDI_HANDLE] = handle;
  o_vdipb.contrl[ROUTINE] = 107;
  o_vdipb.contrl[N_PTSIN] = 0;
  o_vdipb.contrl[N_INTIN] = 1;
  vdi_call(&o_vdipb);

  *t_width = o_vdipb.ptsout[0];
  *t_height = o_vdipb.ptsout[1];
  *c_width = o_vdipb.ptsout[2];
  *c_height = o_vdipb.ptsout[3];

  return o_vdipb.intout[0];
}

int vst_effects(int handle, int attribute)
{
  DEFINE_VPB;
  o_vdipb.intin[0] = attribute;

  o_vdipb.contrl[VDI_HANDLE] = handle;
  o_vdipb.contrl[ROUTINE] = 106;
  o_vdipb.contrl[N_PTSIN] = 0;
  o_vdipb.contrl[N_INTIN] = 1;
  vdi_call(&o_vdipb);

  return o_vdipb.intout[0];
}

void vst_alignment(int handle, int h_in, int v_in, int *h_out, int *v_out)
{
  DEFINE_VPB;
  o_vdipb.intin[0] = h_in;
  o_vdipb.intin[1] = v_in;

  o_vdipb.contrl[VDI_HANDLE] = handle;
  o_vdipb.contrl[ROUTINE] = 39;
  o_vdipb.contrl[N_PTSIN] = 0;
  o_vdipb.contrl[N_INTIN] = 2;
  vdi_call(&o_vdipb);

  *h_out = o_vdipb.intout[0];
  *v_out = o_vdipb.intout[1];
}

int vst_rotation(int handle, int rot)
{
  DEFINE_VPB;
  o_vdipb.intin[0] = rot;

  o_vdipb.contrl[VDI_HANDLE] = handle;
  o_vdipb.contrl[ROUTINE] = 13;
  o_vdipb.contrl[N_PTSIN] = 0;
  o_vdipb.contrl[N_INTIN] = 1;
  vdi_call(&o_vdipb);

  return o_vdipb.intout[0];
}

int vst_font(int handle, int index)
{
  DEFINE_VPB;
  o_vdipb.intin[0] = index;

  o_vdipb.contrl[VDI_HANDLE] = handle;
  o_vdipb.contrl[ROUTINE] = 21;
  o_vdipb.contrl[N_PTSIN] = 0;
  o_vdipb.contrl[N_INTIN] = 1;
  vdi_call(&o_vdipb);

  return o_vdipb.intout[0];
}

int vst_load_fonts(int handle, int rsrvd)
{
  DEFINE_VPB;
  o_vdipb.intin[0] = rsrvd;

  o_vdipb.contrl[VDI_HANDLE] = handle;
  o_vdipb.contrl[ROUTINE] = 119;
  o_vdipb.contrl[N_PTSIN] = 0;
  o_vdipb.contrl[N_INTIN] = 1;
  vdi_call(&o_vdipb);

  return o_vdipb.intout[0];
}

void vst_unload_fonts(int handle, int select)
{
  DEFINE_VPB;
  o_vdipb.intin[0] = select;

  o_vdipb.contrl[VDI_HANDLE] = handle;
  o_vdipb.contrl[ROUTINE] = 120;
  o_vdipb.contrl[N_PTSIN] = 0;
  o_vdipb.contrl[N_INTIN] = 1;
  vdi_call(&o_vdipb);
}

int vst_arbpt (int   handle,
               int   point,
               int * wchar,
               int * hchar,
               int * wcell,
               int * hcell) {
  DEFINE_VPB;
  o_vdipb.intin[0] = point;

  o_vdipb.contrl[VDI_HANDLE] = handle;
  o_vdipb.contrl[ROUTINE] = 246;
  o_vdipb.contrl[N_PTSIN] = 0;
  o_vdipb.contrl[N_INTIN] = 1;
  vdi_call(&o_vdipb);

  *wchar = o_vdipb.ptsout[0];
  *hchar = o_vdipb.ptsout[1];
  *wcell = o_vdipb.ptsout[2];
  *hcell = o_vdipb.ptsout[3];

  return o_vdipb.intout[0];
}

void v_pline(int handle, int nr, int *coords)
{
  DEFINE_VPB;
  int i;

  for(i=0 ; i<nr ; i++)
  {
    o_vdipb.ptsin[i*2+0] = coords[i*2+0];
    o_vdipb.ptsin[i*2+1] = coords[i*2+1];
  }
  
  o_vdipb.contrl[N_PTSIN] = nr;
  o_vdipb.contrl[N_INTIN] = 0;
  o_vdipb.contrl[VDI_HANDLE] = handle;
  o_vdipb.contrl[ROUTINE] = 6;
  vdi_call(&o_vdipb);
}

int vsl_color(int handle, int index)
{
  DEFINE_VPB;
  o_vdipb.intin[0] = index;

  o_vdipb.contrl[VDI_HANDLE] = handle;
  o_vdipb.contrl[ROUTINE] = 17;
  o_vdipb.contrl[N_PTSIN] = 0;
  o_vdipb.contrl[N_INTIN] = 1;
  vdi_call(&o_vdipb);

  return o_vdipb.intout[0];
}

int vsl_width(int handle, int width)
{
  DEFINE_VPB;
  o_vdipb.ptsin[0] = width;
  o_vdipb.ptsin[1] = 0;

  o_vdipb.contrl[VDI_HANDLE] = handle;
  o_vdipb.contrl[ROUTINE] = 16;
  o_vdipb.contrl[N_PTSIN] = 1;
  o_vdipb.contrl[N_INTIN] = 0;
  vdi_call(&o_vdipb);

  return o_vdipb.ptsout[0];
}

int vsl_type(int handle, int type)
{
  DEFINE_VPB;
  o_vdipb.intin[0] = type;

  o_vdipb.contrl[VDI_HANDLE] = handle;
  o_vdipb.contrl[ROUTINE] = 15;
  o_vdipb.contrl[N_PTSIN] = 0;
  o_vdipb.contrl[N_INTIN] = 1;
  vdi_call(&o_vdipb);

  return o_vdipb.intout[0];
}

void vsl_udsty(int handle, int lmask)
{
  DEFINE_VPB;
  o_vdipb.intin[0] = (unsigned short)lmask;

  o_vdipb.contrl[VDI_HANDLE] = handle;
  o_vdipb.contrl[ROUTINE] = 113;
  o_vdipb.contrl[N_PTSIN] = 0;
  o_vdipb.contrl[N_INTIN] = 1;
  vdi_call(&o_vdipb);
}

void vsl_ends(int handle, int lbeg, int lend)
{
  DEFINE_VPB;
  o_vdipb.intin[0] = lbeg;
  o_vdipb.intin[1] = lend;

  o_vdipb.contrl[VDI_HANDLE] = handle;
  o_vdipb.contrl[ROUTINE] = 108;
  o_vdipb.contrl[N_PTSIN] = 0;
  o_vdipb.contrl[N_INTIN] = 2;
  vdi_call(&o_vdipb);
}

void v_pmarker(int handle, int nr, int *coords)
{
  DEFINE_VPB;
  int i;

  for(i=0 ; i<nr ; i++)
  {
    o_vdipb.ptsin[i*2+0] = coords[i*2+0];
    o_vdipb.ptsin[i*2+1] = coords[i*2+1];
  }
  
  o_vdipb.contrl[N_PTSIN] = nr;
  o_vdipb.contrl[N_INTIN] = 0;
  o_vdipb.contrl[VDI_HANDLE] = handle;
  o_vdipb.contrl[ROUTINE] = 7;
  vdi_call(&o_vdipb);
}

int vsm_color(int handle, int index)
{
  DEFINE_VPB;
  o_vdipb.intin[0] = index;

  o_vdipb.contrl[VDI_HANDLE] = handle;
  o_vdipb.contrl[ROUTINE] = 20;
  o_vdipb.contrl[N_PTSIN] = 0;
  o_vdipb.contrl[N_INTIN] = 1;
  vdi_call(&o_vdipb);

  return o_vdipb.intout[0];
}

int vsm_height(int handle, int height)
{
  DEFINE_VPB;
  o_vdipb.ptsin[0] = 0;
  o_vdipb.ptsin[1] = height;

  o_vdipb.contrl[VDI_HANDLE] = handle;
  o_vdipb.contrl[ROUTINE] = 19;
  o_vdipb.contrl[N_PTSIN] = 1;
  o_vdipb.contrl[N_INTIN] = 0;
  vdi_call(&o_vdipb);

  return o_vdipb.ptsout[1];
}

int vsm_type(int handle, int type)
{
  DEFINE_VPB;
  o_vdipb.intin[0] = type;

  o_vdipb.contrl[VDI_HANDLE] = handle;
  o_vdipb.contrl[ROUTINE] = 18;
  o_vdipb.contrl[N_PTSIN] = 0;
  o_vdipb.contrl[N_INTIN] = 1;
  vdi_call(&o_vdipb);

  return o_vdipb.intout[0];
}

void v_fillarea(int handle, int nr, int *coords)
{
  DEFINE_VPB;
  int i;

  for(i=0 ; i<nr ; i++)
  {
    o_vdipb.ptsin[i*2+0] = coords[i*2+0];
    o_vdipb.ptsin[i*2+1] = coords[i*2+1];
  }
  
  o_vdipb.contrl[VDI_HANDLE] = handle;
  o_vdipb.contrl[ROUTINE] = 9;
  o_vdipb.contrl[N_PTSIN] = nr;
  o_vdipb.contrl[N_INTIN] = 0;
  vdi_call(&o_vdipb);
}

void vr_recfl(int handle, int coords[4])
{
  DEFINE_VPB;
  int i;

  for(i=0 ; i<4 ; i++)
    o_vdipb.ptsin[i] = coords[i];

  o_vdipb.contrl[VDI_HANDLE] = handle;
  o_vdipb.contrl[ROUTINE] = 114;
  o_vdipb.contrl[N_PTSIN] = 2;
  o_vdipb.contrl[N_INTIN] = 0;
  vdi_call(&o_vdipb);
}

void v_contourfill(int handle, int x, int y, int index)
{
  DEFINE_VPB;
  o_vdipb.ptsin[0] = x;
  o_vdipb.ptsin[1] = y;
  o_vdipb.intin[0] = index;

  o_vdipb.contrl[VDI_HANDLE] = handle;
  o_vdipb.contrl[ROUTINE] = 103;
  o_vdipb.contrl[N_PTSIN] = 1;
  o_vdipb.contrl[N_INTIN] = 1;
  vdi_call(&o_vdipb);
}

int vsf_color(int handle, int index)
{
  DEFINE_VPB;
  o_vdipb.intin[0] = index;

  o_vdipb.contrl[VDI_HANDLE] = handle;
  o_vdipb.contrl[ROUTINE] = 25;
  o_vdipb.contrl[N_PTSIN] = 0;
  o_vdipb.contrl[N_INTIN] = 1;
  vdi_call(&o_vdipb);

  return o_vdipb.intout[0];
}

int vsf_interior(int handle, int interior)
{
  DEFINE_VPB;
  o_vdipb.intin[0] = interior;

  o_vdipb.contrl[VDI_HANDLE] = handle;
  o_vdipb.contrl[ROUTINE] = 23;
  o_vdipb.contrl[N_PTSIN] = 0;
  o_vdipb.contrl[N_INTIN] = 1;
  vdi_call(&o_vdipb);

  return o_vdipb.intout[0];
}

int vsf_style(int handle, int style)
{
  DEFINE_VPB;
  o_vdipb.intin[0] = style;

  o_vdipb.contrl[VDI_HANDLE] = handle;
  o_vdipb.contrl[ROUTINE] = 24;
  o_vdipb.contrl[N_PTSIN] = 0;
  o_vdipb.contrl[N_INTIN] = 1;
  vdi_call(&o_vdipb);

  return o_vdipb.intout[0];
}

void vsf_udpat(int handle, int *pat, int nbpl)
{
  DEFINE_VPB;
  int i,j;

  for(i=0 ; i<nbpl ; i++)
    for(j=0 ; j<16 ; j++)
      o_vdipb.intin[i*16+j] = (unsigned short)pat[i*16+j];

  o_vdipb.contrl[VDI_HANDLE] = handle;
  o_vdipb.contrl[ROUTINE] = 112;
  o_vdipb.contrl[N_PTSIN] = 0;
  o_vdipb.contrl[N_INTIN] = nbpl * 16;
  vdi_call(&o_vdipb);
}

int vsf_perimeter(int handle, int perimeter)
{
  DEFINE_VPB;
  o_vdipb.intin[0] = perimeter;

  o_vdipb.contrl[VDI_HANDLE] = handle;
  o_vdipb.contrl[ROUTINE] = 104;
  o_vdipb.contrl[N_PTSIN] = 0;
  o_vdipb.contrl[N_INTIN] = 1;
  vdi_call(&o_vdipb);

  return o_vdipb.intout[0];
}

void v_bar(int handle, int pxy[4])
{
  DEFINE_VPB;
  int i;

  for(i=0 ; i<4 ; i++)
    o_vdipb.ptsin[i] = pxy[i];

  o_vdipb.contrl[VDI_HANDLE] = handle;
  o_vdipb.contrl[ROUTINE] = 11;
  o_vdipb.contrl[SUBROUTINE] = 1;
  o_vdipb.contrl[N_PTSIN] = 2;
  o_vdipb.contrl[N_INTIN] = 0;
  vdi_call(&o_vdipb);
}

void v_arc(int handle, int x, int y, int rad, int abeg, int aend)
{
  DEFINE_VPB;
  o_vdipb.ptsin[0] = x;
  o_vdipb.ptsin[1] = y;
  o_vdipb.ptsin[2] = 0;
  o_vdipb.ptsin[3] = 0;
  o_vdipb.ptsin[4] = 0;
  o_vdipb.ptsin[5] = 0;
  o_vdipb.ptsin[6] = rad;
  o_vdipb.ptsin[7] = 0;

  o_vdipb.intin[0] = abeg;
  o_vdipb.intin[1] = aend;

  o_vdipb.contrl[VDI_HANDLE] = handle;
  o_vdipb.contrl[ROUTINE] = 11;
  o_vdipb.contrl[SUBROUTINE] = 2;
  o_vdipb.contrl[N_PTSIN] = 4;
  o_vdipb.contrl[N_INTIN] = 2;
  vdi_call(&o_vdipb);
}

void v_pieslice(int handle, int x, int y, int rad, int abeg, int aend)
{
  DEFINE_VPB;
  o_vdipb.ptsin[0] = x;
  o_vdipb.ptsin[1] = y;
  o_vdipb.ptsin[2] = 0;
  o_vdipb.ptsin[3] = 0;
  o_vdipb.ptsin[4] = 0;
  o_vdipb.ptsin[5] = 0;
  o_vdipb.ptsin[6] = rad;
  o_vdipb.ptsin[7] = 0;

  o_vdipb.intin[0] = abeg;
  o_vdipb.intin[1] = aend;

  o_vdipb.contrl[VDI_HANDLE] = handle;
  o_vdipb.contrl[ROUTINE] = 11;
  o_vdipb.contrl[SUBROUTINE] = 3;
  o_vdipb.contrl[N_PTSIN] = 4;
  o_vdipb.contrl[N_INTIN] = 2;
  vdi_call(&o_vdipb);
}

void v_circle(int handle, int x, int y, int rad)
{
  DEFINE_VPB;
  o_vdipb.ptsin[0] = x;
  o_vdipb.ptsin[1] = y;
  o_vdipb.ptsin[2] = 0;
  o_vdipb.ptsin[3] = 0;
  o_vdipb.ptsin[4] = rad;
  o_vdipb.ptsin[5] = 0;

  o_vdipb.contrl[VDI_HANDLE] = handle;
  o_vdipb.contrl[ROUTINE] = 11;
  o_vdipb.contrl[SUBROUTINE] = 4;
  o_vdipb.contrl[N_PTSIN] = 3;
  o_vdipb.contrl[N_INTIN] = 0;
  vdi_call(&o_vdipb);
}

void v_ellipse(int handle, int x, int y, int xrad, int yrad)
{
  DEFINE_VPB;
  o_vdipb.ptsin[0] = x;
  o_vdipb.ptsin[1] = y;
  o_vdipb.ptsin[2] = xrad;
  o_vdipb.ptsin[3] = yrad;

  o_vdipb.contrl[VDI_HANDLE] = handle;
  o_vdipb.contrl[ROUTINE] = 11;
  o_vdipb.contrl[SUBROUTINE] = 5;
  o_vdipb.contrl[N_PTSIN] = 2;
  o_vdipb.contrl[N_INTIN] = 0;
  vdi_call(&o_vdipb);
}

void v_ellarc(int handle, int x, int y, int xrad, int yrad, int abeg, int aend)
{
  DEFINE_VPB;
  o_vdipb.ptsin[0] = x;
  o_vdipb.ptsin[1] = y;
  o_vdipb.ptsin[2] = xrad;
  o_vdipb.ptsin[3] = yrad;

  o_vdipb.intin[0] = abeg;
  o_vdipb.intin[1] = aend;

  o_vdipb.contrl[VDI_HANDLE] = handle;
  o_vdipb.contrl[ROUTINE] = 11;
  o_vdipb.contrl[SUBROUTINE] = 6;
  o_vdipb.contrl[N_PTSIN] = 2;
  o_vdipb.contrl[N_INTIN] = 1;
  vdi_call(&o_vdipb);
}

void v_ellpie(int handle, int x, int y, int xrad, int yrad, int abeg, int aend)
{
  DEFINE_VPB;
  o_vdipb.ptsin[0] = x;
  o_vdipb.ptsin[1] = y;
  o_vdipb.ptsin[2] = xrad;
  o_vdipb.ptsin[3] = yrad;

  o_vdipb.intin[0] = abeg;
  o_vdipb.intin[1] = aend;

  o_vdipb.contrl[VDI_HANDLE] = handle;
  o_vdipb.contrl[ROUTINE] = 11;
  o_vdipb.contrl[SUBROUTINE] = 7;
  o_vdipb.contrl[N_PTSIN] = 2;
  o_vdipb.contrl[N_INTIN] = 1;
  vdi_call(&o_vdipb);
}

void v_rbox(int handle, int pxy[4])
{
  DEFINE_VPB;
  int i;

  for(i=0 ; i<4 ; i++)
    o_vdipb.ptsin[i] = pxy[i];

  o_vdipb.contrl[VDI_HANDLE] = handle;
  o_vdipb.contrl[ROUTINE] = 11;
  o_vdipb.contrl[SUBROUTINE] = 8;
  o_vdipb.contrl[N_PTSIN] = 2;
  o_vdipb.contrl[N_INTIN] = 0;
  vdi_call(&o_vdipb);
}

void v_rfbox(int handle, int pxy[4])
{
  DEFINE_VPB;
  int i;

  for(i=0 ; i<4 ; i++)
    o_vdipb.ptsin[i] = pxy[i];

  o_vdipb.contrl[VDI_HANDLE] = handle;
  o_vdipb.contrl[ROUTINE] = 11;
  o_vdipb.contrl[SUBROUTINE] = 9;
  o_vdipb.contrl[N_PTSIN] = 2;
  o_vdipb.contrl[N_INTIN] = 0;
  vdi_call(&o_vdipb);
}

void v_justified(int handle, int x, int y, char *string, int len, int wext, int cext)
{
  DEFINE_VPB;
  int i;
  
  o_vdipb.ptsin[0] = x;
  o_vdipb.ptsin[1] = y;
  o_vdipb.ptsin[2] = len;
  o_vdipb.ptsin[3] = 0;
  
  o_vdipb.intin[0] = wext;
  o_vdipb.intin[1] = cext;
  
  i=0;
  while(string[i])
  {
    o_vdipb.intin[i+2] = (short)string[i];
    i++;
  }
  
  o_vdipb.contrl[VDI_HANDLE] = handle;
  o_vdipb.contrl[ROUTINE] = 11;
  o_vdipb.contrl[SUBROUTINE] = 10;
  o_vdipb.contrl[N_PTSIN] = 0;
  o_vdipb.contrl[N_INTIN] = i+2;
  vdi_call(&o_vdipb);
}

void vro_cpyfm(int handle, int mode, int coords[8], MFDB *src, MFDB *dest)
{
  DEFINE_VPB;
  int i;

  for(i=0 ; i<8 ; i++)
    o_vdipb.ptsin[i] = coords[i];

  o_vdipb.intin[0] = mode;

  o_vdipb.contrl[7] = MSW(src);
  o_vdipb.contrl[8] = LSW(src);
  o_vdipb.contrl[9] = MSW(dest);
  o_vdipb.contrl[10] = LSW(dest);

  o_vdipb.contrl[VDI_HANDLE] = handle;
  o_vdipb.contrl[ROUTINE] = 109;
  o_vdipb.contrl[N_PTSIN] = 4;
  o_vdipb.contrl[N_INTIN] = 1;
  vdi_call(&o_vdipb);
}

void vrt_cpyfm(int handle, int mode, int coords[8], MFDB *src, MFDB *dest, int index[2])
{
  DEFINE_VPB;
  int i;

  for(i=0 ; i<8 ; i++)
    o_vdipb.ptsin[i] = coords[i];

  o_vdipb.intin[0] = mode;
  o_vdipb.intin[1] = index[0];
  o_vdipb.intin[2] = index[1];

  o_vdipb.contrl[7] = MSW(src);
  o_vdipb.contrl[8] = LSW(src);
  o_vdipb.contrl[9] = MSW(dest);
  o_vdipb.contrl[10] = LSW(dest);

  o_vdipb.contrl[VDI_HANDLE] = handle;
  o_vdipb.contrl[ROUTINE] = 121;
  o_vdipb.contrl[N_PTSIN] = 4;
  o_vdipb.contrl[N_INTIN] = 3;
  vdi_call(&o_vdipb);
}

void vr_trnfm(int handle, MFDB *src, MFDB *dest)
{
  DEFINE_VPB;
  o_vdipb.contrl[7] = MSW(src);
  o_vdipb.contrl[8] = LSW(src);
  o_vdipb.contrl[9] = MSW(dest);
  o_vdipb.contrl[10] = LSW(dest);

  o_vdipb.contrl[VDI_HANDLE] = handle;
  o_vdipb.contrl[ROUTINE] = 110;
  o_vdipb.contrl[N_PTSIN] = 0;
  o_vdipb.contrl[N_INTIN] = 0;
  vdi_call(&o_vdipb);
}

void v_get_pixel(int handle, int x, int y, int *p_val, int *p_index)
{
  DEFINE_VPB;
  o_vdipb.ptsin[0] = x;
  o_vdipb.ptsin[1] = y;

  o_vdipb.contrl[VDI_HANDLE] = handle;
  o_vdipb.contrl[ROUTINE] = 105;
  o_vdipb.contrl[N_PTSIN] = 1;
  o_vdipb.contrl[N_INTIN] = 0;
  vdi_call(&o_vdipb);

  *p_val = o_vdipb.intout[0];
  *p_index = o_vdipb.intout[1];
}

void vsc_form(int handle, MFORM * data)
{
  DEFINE_VPB;
  int i;

  for(i = 0 ; i < sizeof(MFORM) / sizeof(WORD); i++)
  {
    o_vdipb.intin[i] = ((WORD *)data)[i];
  }

  o_vdipb.contrl[VDI_HANDLE] = handle;
  o_vdipb.contrl[ROUTINE] = 111;
  o_vdipb.contrl[N_PTSIN] = 0;
  o_vdipb.contrl[N_INTIN] = 37;
  vdi_call(&o_vdipb);
}

void vex_timv(int handle, void *newr, void *oldr, int *time)
{
  DEFINE_VPB;
  o_vdipb.contrl[7] = MSW(newr);
  o_vdipb.contrl[8] = LSW(newr);

  o_vdipb.contrl[VDI_HANDLE] = handle;
  o_vdipb.contrl[ROUTINE] = 118;
  o_vdipb.contrl[N_PTSIN] = 0;
  o_vdipb.contrl[N_INTIN] = 0;
  vdi_call(&o_vdipb);

  *time = o_vdipb.intout[0];
  *(long *)oldr = ((long)o_vdipb.contrl[9] << 16) +  o_vdipb.contrl[10];
}

void v_show_c(int handle, int reset)
{
  DEFINE_VPB;
  o_vdipb.intin[0] = reset;

  o_vdipb.contrl[VDI_HANDLE] = handle;
  o_vdipb.contrl[ROUTINE] = 122;
  o_vdipb.contrl[N_PTSIN] = 0;
  o_vdipb.contrl[N_INTIN] = 1;
  vdi_call(&o_vdipb);
}

void v_hide_c(int handle)
{
  DEFINE_VPB;
  o_vdipb.contrl[VDI_HANDLE] = handle;
  o_vdipb.contrl[ROUTINE] = 123;
  o_vdipb.contrl[N_PTSIN] = 0;
  o_vdipb.contrl[N_INTIN] = 0;
  vdi_call(&o_vdipb);
}

void vq_mouse(int handle, int *buttons, int *x, int *y)
{
  DEFINE_VPB;
  o_vdipb.contrl[VDI_HANDLE] = handle;
  o_vdipb.contrl[ROUTINE] = 124;
  o_vdipb.contrl[N_PTSIN] = 0;
  o_vdipb.contrl[N_INTIN] = 0;
  vdi_call(&o_vdipb);

  *buttons = o_vdipb.intout[0];
  *x = o_vdipb.ptsout[0];
  *y = o_vdipb.ptsout[1];
}

void
vex_butv (int     handle,
          void *  newr,
          void ** oldr)
{
  DEFINE_VPB;
  o_vdipb.contrl[7] = MSW(newr);
  o_vdipb.contrl[8] = LSW(newr);

  o_vdipb.contrl[VDI_HANDLE] = handle;
  o_vdipb.contrl[ROUTINE] = 125;
  o_vdipb.contrl[N_PTSIN] = 0;
  o_vdipb.contrl[N_INTIN] = 0;
  vdi_call(&o_vdipb);

  *(long *)oldr = ((long)o_vdipb.contrl[9] << 16) +  o_vdipb.contrl[10];
}

void vex_motv(int handle, void *newr, void *oldr)
{
  DEFINE_VPB;
  o_vdipb.contrl[7] = MSW(newr);
  o_vdipb.contrl[8] = LSW(newr);

  o_vdipb.contrl[VDI_HANDLE] = handle;
  o_vdipb.contrl[N_PTSIN] = 0;
  o_vdipb.contrl[N_INTIN] = 0;
  o_vdipb.contrl[ROUTINE] = 126;
  vdi_call(&o_vdipb);

  *(long *)oldr = ((long)o_vdipb.contrl[9] << 16) +  o_vdipb.contrl[10];
}

void vex_curv(int handle, void *newr, void *oldr)
{
  DEFINE_VPB;
  o_vdipb.contrl[7] = MSW(newr);
  o_vdipb.contrl[8] = LSW(newr);

  o_vdipb.contrl[VDI_HANDLE] = handle;
  o_vdipb.contrl[N_PTSIN] = 0;
  o_vdipb.contrl[N_INTIN] = 0;
  o_vdipb.contrl[ROUTINE] = 127;
  vdi_call(&o_vdipb);

  *(long *)oldr = ((long)o_vdipb.contrl[9] << 16) +  o_vdipb.contrl[10];
}

void vq_extnd(int handle, int ext, int work_out[57])
{
  DEFINE_VPB;
  int i;

  o_vdipb.intin[0] = ext;

  o_vdipb.contrl[VDI_HANDLE] = handle;
  o_vdipb.contrl[ROUTINE] = 102;
  o_vdipb.contrl[N_PTSIN] = 0;
  o_vdipb.contrl[N_INTIN] = 1;
  vdi_call(&o_vdipb);

  for(i=0 ; i<45 ; i++)
    work_out[i] = o_vdipb.intout[i];
  for(i=0 ; i<12 ; i++)
    work_out[i+45] = o_vdipb.ptsout[i];
}

void vq_key_s(int handle, int *keys)
{
  DEFINE_VPB;
  o_vdipb.contrl[VDI_HANDLE] = handle;
  o_vdipb.contrl[ROUTINE] = 128;
  o_vdipb.contrl[N_PTSIN] = 0;
  o_vdipb.contrl[N_INTIN] = 0;
  vdi_call(&o_vdipb);

  *keys = o_vdipb.intout[0];
}

int
vq_color(int handle, int index, int flag, int rgb[3])
{
  DEFINE_VPB;
  o_vdipb.intin[0] = index;
  o_vdipb.intin[1] = flag;

  o_vdipb.contrl[VDI_HANDLE] = handle;
  o_vdipb.contrl[ROUTINE] = 26;
  o_vdipb.contrl[N_PTSIN] = 0;
  o_vdipb.contrl[N_INTIN] = 2;
  vdi_call(&o_vdipb);

  rgb[0] = o_vdipb.intout[1];
  rgb[1] = o_vdipb.intout[2];
  rgb[2] = o_vdipb.intout[3];

  return o_vdipb.intout[0];
}

void vql_attributes(int handle, int attr[6])
{
  DEFINE_VPB;
  o_vdipb.contrl[VDI_HANDLE] = handle;
  o_vdipb.contrl[ROUTINE] = 35;
  o_vdipb.contrl[N_PTSIN] = 0;
  o_vdipb.contrl[N_INTIN] = 0;
  vdi_call(&o_vdipb);

  attr[0] = o_vdipb.intout[0];
  attr[1] = o_vdipb.intout[1];
  attr[2] = o_vdipb.intout[2];
  attr[3] = o_vdipb.intout[3];
  attr[4] = o_vdipb.intout[4];

  attr[5] = o_vdipb.ptsout[0];
}

void vqm_attributes(int handle, int attr[5])
{
  DEFINE_VPB;
  o_vdipb.contrl[VDI_HANDLE] = handle;
  o_vdipb.contrl[ROUTINE] = 36;
  o_vdipb.contrl[N_PTSIN] = 0;
  o_vdipb.contrl[N_INTIN] = 0;
  vdi_call(&o_vdipb);

  attr[0] = o_vdipb.intout[0];
  attr[1] = o_vdipb.intout[1];
  attr[2] = o_vdipb.intout[2];

  attr[3] = o_vdipb.ptsout[0];
  attr[4] = o_vdipb.ptsout[1];
}

void vqf_attributes(int handle, int attr[5])
{
  DEFINE_VPB;
  o_vdipb.contrl[VDI_HANDLE] = handle;
  o_vdipb.contrl[ROUTINE] = 37;
  o_vdipb.contrl[N_PTSIN] = 0;
  o_vdipb.contrl[N_INTIN] = 0;
  vdi_call(&o_vdipb);

  attr[0] = o_vdipb.intout[0];
  attr[1] = o_vdipb.intout[1];
  attr[2] = o_vdipb.intout[2];
  attr[3] = o_vdipb.intout[3];
  attr[4] = o_vdipb.intout[4];
}

void vqt_attributes(int handle, int attr[10])
{
  DEFINE_VPB;
  o_vdipb.contrl[VDI_HANDLE] = handle;
  o_vdipb.contrl[ROUTINE] = 38;
  o_vdipb.contrl[N_PTSIN] = 0;
  o_vdipb.contrl[N_INTIN] = 0;
  vdi_call(&o_vdipb);

  attr[0] = o_vdipb.intout[0];
  attr[1] = o_vdipb.intout[1];
  attr[2] = o_vdipb.intout[2];
  attr[3] = o_vdipb.intout[3];
  attr[4] = o_vdipb.intout[4];
  attr[5] = o_vdipb.intout[5];

  attr[6] = o_vdipb.ptsout[0];
  attr[7] = o_vdipb.ptsout[1];
  attr[8] = o_vdipb.ptsout[2];
  attr[9] = o_vdipb.ptsout[3];
}

void vqt_extent(int handle, char *string, int coords[8])
{
  DEFINE_VPB;
  int i;
  
  i=0;
  while(string[i])
  {
    o_vdipb.intin[i] = (short)string[i];
    i++;
  }
  
  o_vdipb.contrl[VDI_HANDLE] = handle;
  o_vdipb.contrl[ROUTINE] = 116;
  o_vdipb.contrl[N_PTSIN] = 0;
  o_vdipb.contrl[N_INTIN] = i;
  vdi_call(&o_vdipb);
  
  for(i=0 ; i<8 ; i++)
    coords[i] = o_vdipb.ptsout[i];
}

int vqt_width(int handle, char ch, int *cwidth, int *v_offset, int *h_offset)
{
  DEFINE_VPB;
  o_vdipb.intin[0] = (short)ch;

  o_vdipb.contrl[VDI_HANDLE] = handle;
  o_vdipb.contrl[ROUTINE] = 117;
  o_vdipb.contrl[N_PTSIN] = 0;
  o_vdipb.contrl[N_INTIN] = 1;
  vdi_call(&o_vdipb);

  *cwidth = o_vdipb.ptsout[0];
  *v_offset = o_vdipb.ptsout[2];
  *h_offset = o_vdipb.ptsout[4];

  return o_vdipb.intout[0];
}

int vqt_name(int handle, int index, char *fontname)
{
  DEFINE_VPB;
  int i;

  o_vdipb.intin[0] = (short)index;

  o_vdipb.contrl[VDI_HANDLE] = handle;
  o_vdipb.contrl[ROUTINE] = 130;
  o_vdipb.contrl[N_PTSIN] = 0;
  o_vdipb.contrl[N_INTIN] = 1;
  vdi_call(&o_vdipb);

  for(i=0 ; i<33 ; i++)
    fontname[i] = o_vdipb.intout[i + 1];

  return o_vdipb.intout[0];
}

void vqt_fontinfo(int handle, int *first, int *last, 
		  int *dist, int *width, int *effects)
{
  DEFINE_VPB;
  o_vdipb.contrl[VDI_HANDLE] = handle;
  o_vdipb.contrl[ROUTINE] = 131;
  o_vdipb.contrl[N_PTSIN] = 0;
  o_vdipb.contrl[N_INTIN] = 0;
  vdi_call(&o_vdipb);

  *first = o_vdipb.intout[0];
  *last = o_vdipb.intout[1];
  *width = o_vdipb.ptsout[0];
  dist[0] = o_vdipb.ptsout[1];
  dist[1] = o_vdipb.ptsout[3];
  dist[2] = o_vdipb.ptsout[5];
  dist[3] = o_vdipb.ptsout[7];
  dist[4] = o_vdipb.ptsout[7]; /* FIXME: Atari compendium is incomplete */
  effects[0] = o_vdipb.ptsout[2];
  effects[1] = o_vdipb.ptsout[4];
  effects[2] = o_vdipb.ptsout[6];
}

void vqin_mode(int handle, int dev, int *mode)
{
  DEFINE_VPB;
  o_vdipb.intin[0] = dev;

  o_vdipb.contrl[VDI_HANDLE] = handle;
  o_vdipb.contrl[ROUTINE] = 115;
  o_vdipb.contrl[N_PTSIN] = 0;
  o_vdipb.contrl[N_INTIN] = 1;
  vdi_call(&o_vdipb);

  *mode = o_vdipb.intout[0];
}

void vq_chcells(int handle, int *rows, int *cols)
{
  DEFINE_VPB;
  o_vdipb.contrl[VDI_HANDLE] = handle;
  o_vdipb.contrl[ROUTINE] = 5;
  o_vdipb.contrl[SUBROUTINE] = 1;
  o_vdipb.contrl[N_PTSIN] = 0;
  o_vdipb.contrl[N_INTIN] = 0;
  vdi_call(&o_vdipb);

  *rows = o_vdipb.intout[0];
  *cols = o_vdipb.intout[1];
}

void v_exit_cur(int handle)
{
  DEFINE_VPB;
  o_vdipb.contrl[VDI_HANDLE] = handle;
  o_vdipb.contrl[ROUTINE] = 5;
  o_vdipb.contrl[SUBROUTINE] = 2;
  o_vdipb.contrl[N_PTSIN] = 0;
  o_vdipb.contrl[N_INTIN] = 0;
  vdi_call(&o_vdipb);
}

void v_enter_cur(int handle)
{
  DEFINE_VPB;
  o_vdipb.contrl[VDI_HANDLE] = handle;
  o_vdipb.contrl[ROUTINE] = 5;
  o_vdipb.contrl[SUBROUTINE] = 3;
  o_vdipb.contrl[N_PTSIN] = 0;
  o_vdipb.contrl[N_INTIN] = 0;
  vdi_call(&o_vdipb);
}

void v_curup(int handle)
{
  DEFINE_VPB;
  o_vdipb.contrl[VDI_HANDLE] = handle;
  o_vdipb.contrl[ROUTINE] = 5;
  o_vdipb.contrl[SUBROUTINE] = 4;
  o_vdipb.contrl[N_PTSIN] = 0;
  o_vdipb.contrl[N_INTIN] = 0;
  vdi_call(&o_vdipb);
}

void v_curdown(int handle)
{
  DEFINE_VPB;
  o_vdipb.contrl[VDI_HANDLE] = handle;
  o_vdipb.contrl[ROUTINE] = 5;
  o_vdipb.contrl[SUBROUTINE] = 5;
  o_vdipb.contrl[N_PTSIN] = 0;
  o_vdipb.contrl[N_INTIN] = 0;
  vdi_call(&o_vdipb);
}

void v_curright(int handle)
{
  DEFINE_VPB;
  o_vdipb.contrl[VDI_HANDLE] = handle;
  o_vdipb.contrl[ROUTINE] = 5;
  o_vdipb.contrl[SUBROUTINE] = 6;
  o_vdipb.contrl[N_PTSIN] = 0;
  o_vdipb.contrl[N_INTIN] = 0;
  vdi_call(&o_vdipb);
}

void v_curleft(int handle)
{
  DEFINE_VPB;
  o_vdipb.contrl[VDI_HANDLE] = handle;
  o_vdipb.contrl[ROUTINE] = 5;
  o_vdipb.contrl[SUBROUTINE] = 7;
  o_vdipb.contrl[N_PTSIN] = 0;
  o_vdipb.contrl[N_INTIN] = 0;
  vdi_call(&o_vdipb);
}

void v_curhome(int handle)
{
  DEFINE_VPB;
  o_vdipb.contrl[VDI_HANDLE] = handle;
  o_vdipb.contrl[ROUTINE] = 5;
  o_vdipb.contrl[SUBROUTINE] = 8;
  o_vdipb.contrl[N_PTSIN] = 0;
  o_vdipb.contrl[N_INTIN] = 0;
  vdi_call(&o_vdipb);
}

void v_eeos(int handle)
{
  DEFINE_VPB;
  o_vdipb.contrl[VDI_HANDLE] = handle;
  o_vdipb.contrl[ROUTINE] = 5;
  o_vdipb.contrl[SUBROUTINE] = 9;
  o_vdipb.contrl[N_PTSIN] = 0;
  o_vdipb.contrl[N_INTIN] = 0;
  vdi_call(&o_vdipb);
}

void v_eeol(int handle)
{
  DEFINE_VPB;
  o_vdipb.contrl[VDI_HANDLE] = handle;
  o_vdipb.contrl[ROUTINE] = 5;
  o_vdipb.contrl[SUBROUTINE] = 10;
  o_vdipb.contrl[N_PTSIN] = 0;
  o_vdipb.contrl[N_INTIN] = 0;
  vdi_call(&o_vdipb);
}

void vs_curaddress(int handle, int row, int col)
{
  DEFINE_VPB;
  o_vdipb.intin[0] = row;
  o_vdipb.intin[1] = col;

  o_vdipb.contrl[VDI_HANDLE] = handle;
  o_vdipb.contrl[ROUTINE] = 5;
  o_vdipb.contrl[SUBROUTINE] = 11;
  o_vdipb.contrl[N_PTSIN] = 0;
  o_vdipb.contrl[N_INTIN] = 2;
  vdi_call(&o_vdipb);
}

void v_curtext(int handle, char *string)
{
  DEFINE_VPB;
  int i;

  i=0;
  while(string[i])
  {
    o_vdipb.intin[i] = (short)string[i];
    i++;
  }
  
  o_vdipb.contrl[VDI_HANDLE] = handle;
  o_vdipb.contrl[ROUTINE] = 5;
  o_vdipb.contrl[SUBROUTINE] = 12;
  o_vdipb.contrl[N_PTSIN] = 0;
  o_vdipb.contrl[N_INTIN] = i;
  vdi_call(&o_vdipb);
}

void v_rvon(int handle)
{
  DEFINE_VPB;
  o_vdipb.contrl[VDI_HANDLE] = handle;
  o_vdipb.contrl[ROUTINE] = 5;
  o_vdipb.contrl[SUBROUTINE] = 13;
  o_vdipb.contrl[N_PTSIN] = 0;
  o_vdipb.contrl[N_INTIN] = 0;
  vdi_call(&o_vdipb);
}

void v_rvoff(int handle)
{
  DEFINE_VPB;
  o_vdipb.contrl[VDI_HANDLE] = handle;
  o_vdipb.contrl[ROUTINE] = 5;
  o_vdipb.contrl[SUBROUTINE] = 14;
  o_vdipb.contrl[N_PTSIN] = 0;
  o_vdipb.contrl[N_INTIN] = 0;
  vdi_call(&o_vdipb);
}

void vq_curaddress(int handle, int *row, int *col)
{
  DEFINE_VPB;
  o_vdipb.contrl[VDI_HANDLE] = handle;
  o_vdipb.contrl[ROUTINE] = 5;
  o_vdipb.contrl[SUBROUTINE] = 15;
  o_vdipb.contrl[N_PTSIN] = 0;
  o_vdipb.contrl[N_INTIN] = 0;
  vdi_call(&o_vdipb);

  *row = o_vdipb.intout[0];
  *col = o_vdipb.intout[1];
}

void v_hardcopy(int handle)
{
  DEFINE_VPB;
  o_vdipb.contrl[VDI_HANDLE] = handle;
  o_vdipb.contrl[ROUTINE] = 5;
  o_vdipb.contrl[SUBROUTINE] = 17;
  o_vdipb.contrl[N_PTSIN] = 0;
  o_vdipb.contrl[N_INTIN] = 0;
  vdi_call(&o_vdipb);
}

int vq_gdos(void) {
  /* Return false since we haven't implemented gdos yet */
  return 0;
}

int vq_vgdos(void) {
  return GDOS_NONE;
}


void
vrq_string (int    handle,
	    int    maxlen,
	    int    echo,
	    int *  outxy,
	    char * str) {
  DEFINE_VPB;
  int i;

  o_vdipb.contrl[VDI_HANDLE] = handle;
  o_vdipb.contrl[ROUTINE] = 31;
  o_vdipb.contrl[N_PTSIN] = 1;
  o_vdipb.contrl[N_INTIN] = 2;

  o_vdipb.intin[0] = maxlen;
  o_vdipb.intin[1] = echo;
  
  o_vdipb.ptsin[0] = outxy[0];
  o_vdipb.ptsin[0] = outxy[1];

  vdi_call(&o_vdipb);

  for (i = 0; i < o_vdipb.contrl[N_INTOUT]; i++) {
    str[i] = (char)o_vdipb.intout[i];
  }
}

int
vsm_string (int   handle,
	    int   maxlen,
	    int   echo,
	    int * outxy,
	    char * str) {
  DEFINE_VPB;
  int i;

  o_vdipb.contrl[VDI_HANDLE] = handle;
  o_vdipb.contrl[ROUTINE] = 31;
  o_vdipb.contrl[N_PTSIN] = 1;
  o_vdipb.contrl[N_INTIN] = 2;

  o_vdipb.intin[0] = maxlen;
  o_vdipb.intin[1] = echo;
  
  o_vdipb.ptsin[0] = outxy[0];
  o_vdipb.ptsin[0] = outxy[1];

  vdi_call(&o_vdipb);

  for (i = 0; i < o_vdipb.contrl[N_INTOUT]; i++) {
    str[i] = (char)o_vdipb.intout[i];
  }

  return o_vdipb.contrl[N_INTOUT];
}


int
vsm_string_raw (int   handle,
		int   maxlen,
		int   echo,
		int * outxy,
		int * str) {
  DEFINE_VPB;
  int i;

  o_vdipb.contrl[VDI_HANDLE] = handle;
  o_vdipb.contrl[ROUTINE] = 31;
  o_vdipb.contrl[N_PTSIN] = 1;
  o_vdipb.contrl[N_INTIN] = 2;

  o_vdipb.intin[0] = -maxlen;
  o_vdipb.intin[1] = echo;
  
  o_vdipb.ptsin[0] = outxy[0];
  o_vdipb.ptsin[0] = outxy[1];

  vdi_call(&o_vdipb);

  for (i = 0; i < o_vdipb.contrl[N_INTOUT]; i++) {
    str[i] = o_vdipb.intout[i];
  }

  return o_vdipb.contrl[N_INTOUT];
}


void
vsin_mode (int handle,
	   int device,
	   int mode) {
  DEFINE_VPB;
  o_vdipb.contrl[VDI_HANDLE] = handle;
  o_vdipb.contrl[ROUTINE] = 33;
  o_vdipb.contrl[N_PTSIN] = 0;
  o_vdipb.contrl[N_INTIN] = 2;

  o_vdipb.intin[0] = device;
  o_vdipb.intin[1] = mode;
  
  vdi_call(&o_vdipb);
}
