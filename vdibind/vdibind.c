/*
 * vdibind.c
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

#include "../src/ovdisis.h"

/* set contrl[N_INTIN] & contrl[N_PTSIN] too ?? */

#define MSW(a) ((unsigned short)((unsigned long)(a) >> 16))
#define LSW(a) ((unsigned short)((unsigned long)(a) & 0xffffUL))

VDIPARBLK e_vdiparblk;
VDIPB o_vdipb = { e_vdiparblk.contrl,
		  e_vdiparblk.intin, e_vdiparblk.ptsin,
		  e_vdiparblk.intout, e_vdiparblk.ptsout };

extern void vdi_call(VDIPB *);

void v_opnwk(int work_in[11], int *handle, int work_out[57])
{
  int i;

  for(i=0 ; i<11 ; i++)
    o_vdipb.intin[i] = work_in[i];

  o_vdipb.contrl[ROUTINE] = 1;
  vdi_call(&o_vdipb);

  for(i=0 ; i<45 ; i++)
    work_out[i] = o_vdipb.intout[i];
  for(i=0 ; i<12 ; i++)
    work_out[i+45] = o_vdipb.ptsout[i];

  *handle = o_vdipb.contrl[VDI_HANDLE];
}

void v_clswk(int handle)
{
  o_vdipb.contrl[VDI_HANDLE] = handle;
  o_vdipb.contrl[ROUTINE] = 2;
  vdi_call(&o_vdipb);
}

void v_opnvwk(int work_in[11], int *handle, int work_out[57])
{
  int i;

  for(i=0 ; i<11 ; i++)
    o_vdipb.intin[i] = work_in[i];

  o_vdipb.contrl[VDI_HANDLE] = *handle;
  o_vdipb.contrl[ROUTINE] = 100;
  vdi_call(&o_vdipb);

  for(i=0 ; i<45 ; i++)
    work_out[i] = o_vdipb.intout[i];
  for(i=0 ; i<12 ; i++)
    work_out[i+45] = o_vdipb.ptsout[i];

  *handle = o_vdipb.contrl[VDI_HANDLE];
}

void v_clsvwk(int handle)
{
  o_vdipb.contrl[VDI_HANDLE] = handle;
  o_vdipb.contrl[ROUTINE] = 101;
  vdi_call(&o_vdipb);
}

void v_clrwk(int handle)
{
  o_vdipb.contrl[VDI_HANDLE] = handle;
  o_vdipb.contrl[ROUTINE] = 3;
  vdi_call(&o_vdipb);
}

void v_updwk(int handle)
{
  o_vdipb.contrl[VDI_HANDLE] = handle;
  o_vdipb.contrl[ROUTINE] = 4;
  vdi_call(&o_vdipb);
}

void vs_clip(int handle, int clipping, int pxy[4])
{
  int i;

  o_vdipb.intin[0] = clipping;

  if(clipping != 0) /* ignore pxy if clipping is off */
    for(i=0 ; i<4 ; i++)
      o_vdipb.ptsin[i] = pxy[i];

  o_vdipb.contrl[VDI_HANDLE] = handle;
  o_vdipb.contrl[ROUTINE] = 129;
  vdi_call(&o_vdipb);
}

int vswr_mode(int handle, int mode)
{
  o_vdipb.intin[0] = mode;

  o_vdipb.contrl[VDI_HANDLE] = handle;
  o_vdipb.contrl[ROUTINE] = 32;
  vdi_call(&o_vdipb);

  return o_vdipb.intout[0];
}

void vs_color(int handle, int index, int rgb[3])
{
  o_vdipb.intin[0] = index;
  o_vdipb.intin[1] = rgb[0];
  o_vdipb.intin[2] = rgb[1];
  o_vdipb.intin[3] = rgb[2];

  o_vdipb.contrl[VDI_HANDLE] = handle;
  o_vdipb.contrl[ROUTINE] = 14;
  vdi_call(&o_vdipb);
}

void v_gtext(int handle, int x, int y, char *string)
{
  int i=0;

  o_vdipb.ptsin[0] = x;
  o_vdipb.ptsin[1] = y;
  
  while((o_vdipb.intin[i++] = (int)*string++) != 0);

  o_vdipb.contrl[N_INTIN] = --i;

  o_vdipb.contrl[VDI_HANDLE] = handle;
  o_vdipb.contrl[ROUTINE] = 8;
  vdi_call(&o_vdipb);
}

void vst_color(int handle, int index)
{
  o_vdipb.intin[0] = index;

  o_vdipb.contrl[VDI_HANDLE] = handle;
  o_vdipb.contrl[ROUTINE] = 22;
  vdi_call(&o_vdipb);
}

void vst_height(int handle, int height, int *t_width, int *t_height,
		int *c_width, int *c_height)
{
  o_vdipb.ptsin[0] = 0;
  o_vdipb.ptsin[1] = height;

  o_vdipb.contrl[VDI_HANDLE] = handle;
  o_vdipb.contrl[ROUTINE] = 12;
  vdi_call(&o_vdipb);

  *t_width = o_vdipb.ptsout[0];
  *t_height = o_vdipb.ptsout[1];
  *c_width = o_vdipb.ptsout[2];
  *c_height = o_vdipb.ptsout[3];
}

int vst_point(int handle, int height, int *t_width, int *t_height,
		int *c_width, int *c_height)
{
  o_vdipb.intin[0] = height;

  o_vdipb.contrl[VDI_HANDLE] = handle;
  o_vdipb.contrl[ROUTINE] = 107;
  vdi_call(&o_vdipb);

  *t_width = o_vdipb.ptsout[0];
  *t_height = o_vdipb.ptsout[1];
  *c_width = o_vdipb.ptsout[2];
  *c_height = o_vdipb.ptsout[3];

  return o_vdipb.intout[0];
}

int vst_effects(int handle, int attribute)
{
  o_vdipb.intin[0] = attribute;

  o_vdipb.contrl[VDI_HANDLE] = handle;
  o_vdipb.contrl[ROUTINE] = 106;
  vdi_call(&o_vdipb);

  return o_vdipb.intout[0];
}

void vst_alignment(int handle, int h_in, int v_in, int *h_out, int *v_out)
{
  o_vdipb.intin[0] = h_in;
  o_vdipb.intin[1] = v_in;

  o_vdipb.contrl[VDI_HANDLE] = handle;
  o_vdipb.contrl[ROUTINE] = 39;
  vdi_call(&o_vdipb);

  *h_out = o_vdipb.intout[0];
  *v_out = o_vdipb.intout[0];
}

int vst_rotation(int handle, int rot)
{
  o_vdipb.intin[0] = rot;

  o_vdipb.contrl[VDI_HANDLE] = handle;
  o_vdipb.contrl[ROUTINE] = 13;
  vdi_call(&o_vdipb);

  return o_vdipb.intout[0];
}

void v_pline(int handle, int nr, int *coords)
{
  int i;

  for(i=0 ; i<nr ; i++)
    {
      o_vdipb.ptsin[i*2+0] = coords[i*2+0];
      o_vdipb.ptsin[i*2+1] = coords[i*2+1];
    }

  o_vdipb.contrl[N_PTSIN] = nr;
  o_vdipb.contrl[VDI_HANDLE] = handle;
  o_vdipb.contrl[ROUTINE] = 6;
  vdi_call(&o_vdipb);
}

int vsl_color(int handle, int index)
{
  o_vdipb.intin[0] = index;

  o_vdipb.contrl[VDI_HANDLE] = handle;
  o_vdipb.contrl[ROUTINE] = 17;
  vdi_call(&o_vdipb);

  return o_vdipb.intout[0];
}

int vsl_width(int handle, int width)
{
  o_vdipb.ptsin[0] = width;
  o_vdipb.ptsin[1] = 0;

  o_vdipb.contrl[VDI_HANDLE] = handle;
  o_vdipb.contrl[ROUTINE] = 16;
  vdi_call(&o_vdipb);

  return o_vdipb.ptsout[0];
}

int vsl_type(int handle, int type)
{
  o_vdipb.intin[0] = type;

  o_vdipb.contrl[VDI_HANDLE] = handle;
  o_vdipb.contrl[ROUTINE] = 15;
  vdi_call(&o_vdipb);

  return o_vdipb.intout[0];
}

void vsl_udsty(int handle, int lmask)
{
  o_vdipb.intin[0] = (unsigned short)lmask;

  o_vdipb.contrl[VDI_HANDLE] = handle;
  o_vdipb.contrl[ROUTINE] = 113;
  vdi_call(&o_vdipb);
}

void vsl_ends(int handle, int lbeg, int lend)
{
  o_vdipb.intin[0] = lbeg;
  o_vdipb.intin[1] = lend;

  o_vdipb.contrl[VDI_HANDLE] = handle;
  o_vdipb.contrl[ROUTINE] = 108;
  vdi_call(&o_vdipb);
}

void v_pmarker(int handle, int nr, int *coords)
{
  int i;

  for(i=0 ; i<nr ; i++)
    {
      o_vdipb.ptsin[i*2+0] = coords[i*2+0];
      o_vdipb.ptsin[i*2+1] = coords[i*2+1];
    }

  o_vdipb.contrl[N_PTSIN] = nr;
  o_vdipb.contrl[VDI_HANDLE] = handle;
  o_vdipb.contrl[ROUTINE] = 7;
  vdi_call(&o_vdipb);
}

int vsm_color(int handle, int index)
{
  o_vdipb.intin[0] = index;

  o_vdipb.contrl[VDI_HANDLE] = handle;
  o_vdipb.contrl[ROUTINE] = 20;
  vdi_call(&o_vdipb);

  return o_vdipb.intout[0];
}

int vsm_height(int handle, int height)
{
  o_vdipb.ptsin[0] = 0;
  o_vdipb.ptsin[1] = height;

  o_vdipb.contrl[VDI_HANDLE] = handle;
  o_vdipb.contrl[ROUTINE] = 19;
  vdi_call(&o_vdipb);

  return o_vdipb.ptsout[1];
}

int vsm_type(int handle, int type)
{
  o_vdipb.intin[0] = type;

  o_vdipb.contrl[VDI_HANDLE] = handle;
  o_vdipb.contrl[ROUTINE] = 18;
  vdi_call(&o_vdipb);

  return o_vdipb.intout[0];
}

void v_fillarea(int handle, int nr, int *coords)
{
  int i;

  for(i=0 ; i<nr ; i++)
    {
      o_vdipb.ptsin[i*2+0] = coords[i*2+0];
      o_vdipb.ptsin[i*2+1] = coords[i*2+1];
    }

  o_vdipb.contrl[N_PTSIN] = nr;
  o_vdipb.contrl[VDI_HANDLE] = handle;
  o_vdipb.contrl[ROUTINE] = 9;
  vdi_call(&o_vdipb);
}

void vr_recfl(int handle, int coords[4])
{
  int i;

  for(i=0 ; i<4 ; i++)
    o_vdipb.ptsin[i] = coords[i];

  o_vdipb.contrl[VDI_HANDLE] = handle;
  o_vdipb.contrl[ROUTINE] = 114;
  vdi_call(&o_vdipb);
}

void v_contourfill(int handle, int x, int y, int index)
{
  o_vdipb.ptsin[0] = x;
  o_vdipb.ptsin[1] = y;
  o_vdipb.intin[0] = index;

  o_vdipb.contrl[VDI_HANDLE] = handle;
  o_vdipb.contrl[ROUTINE] = 103;
  vdi_call(&o_vdipb);
}

int vsf_color(int handle, int index)
{
  o_vdipb.intin[0] = index;

  o_vdipb.contrl[VDI_HANDLE] = handle;
  o_vdipb.contrl[ROUTINE] = 25;
  vdi_call(&o_vdipb);

  return o_vdipb.intout[0];
}

int vsf_interior(int handle, int interior)
{
  o_vdipb.intin[0] = interior;

  o_vdipb.contrl[VDI_HANDLE] = handle;
  o_vdipb.contrl[ROUTINE] = 23;
  vdi_call(&o_vdipb);

  return o_vdipb.intout[0];
}

int vsf_style(int handle, int style)
{
  o_vdipb.intin[0] = style;

  o_vdipb.contrl[VDI_HANDLE] = handle;
  o_vdipb.contrl[ROUTINE] = 24;
  vdi_call(&o_vdipb);

  return o_vdipb.intout[0];
}

void vsf_udpat(int handle, int *pat, int nbpl)
{
  int i,j;

  for(i=0 ; i<nbpl ; i++)
    for(j=0 ; j<16 ; j++)
      o_vdipb.intin[i*16+j] = (unsigned short)pat[i*16+j];

  o_vdipb.contrl[N_INTIN] = nbpl*16;
  o_vdipb.contrl[VDI_HANDLE] = handle;
  o_vdipb.contrl[ROUTINE] = 112;
  vdi_call(&o_vdipb);
}

int vsf_perimeter(int handle, int perimeter)
{
  o_vdipb.intin[0] = perimeter;

  o_vdipb.contrl[VDI_HANDLE] = handle;
  o_vdipb.contrl[ROUTINE] = 104;
  vdi_call(&o_vdipb);

  return o_vdipb.intout[0];
}

void v_bar(int handle, int pxy[4])
{
  int i;

  for(i=0 ; i<4 ; i++)
    o_vdipb.ptsin[i] = pxy[i];

  o_vdipb.contrl[VDI_HANDLE] = handle;
  o_vdipb.contrl[ROUTINE] = 11;
  o_vdipb.contrl[SUBROUTINE] = 1;
  vdi_call(&o_vdipb);
}

void v_arc(int handle, int x, int y, int rad, int abeg, int aend)
{
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
  vdi_call(&o_vdipb);
}

void v_pieslice(int handle, int x, int y, int rad, int abeg, int aend)
{
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
  vdi_call(&o_vdipb);
}

void v_circle(int handle, int x, int y, int rad)
{
  o_vdipb.ptsin[0] = x;
  o_vdipb.ptsin[1] = y;
  o_vdipb.ptsin[2] = 0;
  o_vdipb.ptsin[3] = 0;
  o_vdipb.ptsin[4] = rad;
  o_vdipb.ptsin[5] = 0;

  o_vdipb.contrl[VDI_HANDLE] = handle;
  o_vdipb.contrl[ROUTINE] = 11;
  o_vdipb.contrl[SUBROUTINE] = 4;
  vdi_call(&o_vdipb);
}

void v_ellipse(int handle, int x, int y, int xrad, int yrad)
{
  o_vdipb.ptsin[0] = x;
  o_vdipb.ptsin[1] = y;
  o_vdipb.ptsin[2] = xrad;
  o_vdipb.ptsin[3] = yrad;

  o_vdipb.contrl[VDI_HANDLE] = handle;
  o_vdipb.contrl[ROUTINE] = 11;
  o_vdipb.contrl[SUBROUTINE] = 5;
  vdi_call(&o_vdipb);
}

void v_ellarc(int handle, int x, int y, int xrad, int yrad, int abeg, int aend)
{
  o_vdipb.ptsin[0] = x;
  o_vdipb.ptsin[1] = y;
  o_vdipb.ptsin[2] = xrad;
  o_vdipb.ptsin[3] = yrad;

  o_vdipb.intin[0] = abeg;
  o_vdipb.intin[1] = aend;

  o_vdipb.contrl[VDI_HANDLE] = handle;
  o_vdipb.contrl[ROUTINE] = 11;
  o_vdipb.contrl[SUBROUTINE] = 6;
  vdi_call(&o_vdipb);
}

void v_ellpie(int handle, int x, int y, int xrad, int yrad, int abeg, int aend)
{
  o_vdipb.ptsin[0] = x;
  o_vdipb.ptsin[1] = y;
  o_vdipb.ptsin[2] = xrad;
  o_vdipb.ptsin[3] = yrad;

  o_vdipb.intin[0] = abeg;
  o_vdipb.intin[1] = aend;

  o_vdipb.contrl[VDI_HANDLE] = handle;
  o_vdipb.contrl[ROUTINE] = 11;
  o_vdipb.contrl[SUBROUTINE] = 7;
  vdi_call(&o_vdipb);
}

void v_rbox(int handle, int pxy[4])
{
  int i;

  for(i=0 ; i<4 ; i++)
    o_vdipb.ptsin[i] = pxy[i];

  o_vdipb.contrl[VDI_HANDLE] = handle;
  o_vdipb.contrl[ROUTINE] = 11;
  o_vdipb.contrl[SUBROUTINE] = 8;
  vdi_call(&o_vdipb);
}

void v_rfbox(int handle, int pxy[4])
{
  int i;

  for(i=0 ; i<4 ; i++)
    o_vdipb.ptsin[i] = pxy[i];

  o_vdipb.contrl[VDI_HANDLE] = handle;
  o_vdipb.contrl[ROUTINE] = 11;
  o_vdipb.contrl[SUBROUTINE] = 9;
  vdi_call(&o_vdipb);
}

void v_justified(int handle, int x, int y, char *string, int len, int wext, int cext)
{
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
  o_vdipb.contrl[N_INTIN] = i+2;

  o_vdipb.contrl[VDI_HANDLE] = handle;
  o_vdipb.contrl[ROUTINE] = 11;
  o_vdipb.contrl[SUBROUTINE] = 10;
  vdi_call(&o_vdipb);
}

void vro_cpyfm(int handle, int mode, int coords[8], MFDB *src, MFDB *dest)
{
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
  vdi_call(&o_vdipb);
}

void vrt_cpyfm(int handle, int mode, int coords[8], MFDB *src, MFDB *dest, int index[2])
{
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
  vdi_call(&o_vdipb);
}

void vr_trnfm(int handle, MFDB *src, MFDB *dest)
{
  o_vdipb.contrl[7] = MSW(src);
  o_vdipb.contrl[8] = LSW(src);
  o_vdipb.contrl[9] = MSW(dest);
  o_vdipb.contrl[10] = LSW(dest);

  o_vdipb.contrl[VDI_HANDLE] = handle;
  o_vdipb.contrl[ROUTINE] = 110;
  vdi_call(&o_vdipb);
}

void v_get_pixel(int handle, int x, int y, int *p_val, int *p_index)
{
  o_vdipb.ptsin[0] = x;
  o_vdipb.ptsin[1] = y;

  o_vdipb.contrl[VDI_HANDLE] = handle;
  o_vdipb.contrl[ROUTINE] = 105;
  vdi_call(&o_vdipb);

  *p_val = o_vdipb.intout[0];
  *p_index = o_vdipb.intout[1];
}

void vsc_form(int handle, int data[37])
{
  int i;

  for(i=0 ; i<37 ; i++)
    o_vdipb.intin[i] = data[i];

  o_vdipb.contrl[VDI_HANDLE] = handle;
  o_vdipb.contrl[ROUTINE] = 111;
  vdi_call(&o_vdipb);
}

void vex_timv(int handle, void *newr, void *oldr, int *time)
{
  o_vdipb.contrl[7] = MSW(newr);
  o_vdipb.contrl[8] = LSW(newr);
  o_vdipb.contrl[9] = MSW(oldr);
  o_vdipb.contrl[10] = LSW(oldr);

  o_vdipb.contrl[VDI_HANDLE] = handle;
  o_vdipb.contrl[ROUTINE] = 118;
  vdi_call(&o_vdipb);

  *time = o_vdipb.intout[0];
}

void v_show_c(int handle, int reset)
{
  o_vdipb.intin[0] = reset;

  o_vdipb.contrl[VDI_HANDLE] = handle;
  o_vdipb.contrl[ROUTINE] = 122;
  vdi_call(&o_vdipb);
}

void v_hide_c(int handle)
{
  o_vdipb.contrl[VDI_HANDLE] = handle;
  o_vdipb.contrl[ROUTINE] = 123;
  vdi_call(&o_vdipb);
}

void vq_mouse(int handle, int *buttons, int *x, int *y)
{
  o_vdipb.contrl[VDI_HANDLE] = handle;
  o_vdipb.contrl[ROUTINE] = 124;
  vdi_call(&o_vdipb);

  *buttons = o_vdipb.intout[0];
  *x = o_vdipb.ptsout[0];
  *y = o_vdipb.ptsout[1];
}

void vex_butv(int handle, void *newr, void *oldr)
{
  o_vdipb.contrl[7] = MSW(newr);
  o_vdipb.contrl[8] = LSW(newr);
  o_vdipb.contrl[9] = MSW(oldr);
  o_vdipb.contrl[10] = LSW(oldr);

  o_vdipb.contrl[VDI_HANDLE] = handle;
  o_vdipb.contrl[ROUTINE] = 125;
  vdi_call(&o_vdipb);
}

void vex_motv(int handle, void *newr, void *oldr)
{
  o_vdipb.contrl[7] = MSW(newr);
  o_vdipb.contrl[8] = LSW(newr);
  o_vdipb.contrl[9] = MSW(oldr);
  o_vdipb.contrl[10] = LSW(oldr);

  o_vdipb.contrl[VDI_HANDLE] = handle;
  o_vdipb.contrl[ROUTINE] = 126;
  vdi_call(&o_vdipb);
}

void vex_curv(int handle, void *newr, void *oldr)
{
  o_vdipb.contrl[7] = MSW(newr);
  o_vdipb.contrl[8] = LSW(newr);
  o_vdipb.contrl[9] = MSW(oldr);
  o_vdipb.contrl[10] = LSW(oldr);

  o_vdipb.contrl[VDI_HANDLE] = handle;
  o_vdipb.contrl[ROUTINE] = 127;
  vdi_call(&o_vdipb);
}

void vq_extnd(int handle, int ext, int work_out[57])
{
  int i;

  o_vdipb.intin[0] = ext;

  o_vdipb.contrl[VDI_HANDLE] = handle;
  o_vdipb.contrl[ROUTINE] = 102;
  vdi_call(&o_vdipb);

  for(i=0 ; i<45 ; i++)
    work_out[i] = o_vdipb.intout[i];
  for(i=0 ; i<12 ; i++)
    work_out[i+45] = o_vdipb.ptsout[i];
}

void vq_key_s(int handle, int *keys)
{
  o_vdipb.contrl[VDI_HANDLE] = handle;
  o_vdipb.contrl[ROUTINE] = 128;
  vdi_call(&o_vdipb);

  *keys = o_vdipb.intout[0];
}

void vq_color(int handle, int index, int flag, int rgb[3])
{
  o_vdipb.intin[0] = index;
  o_vdipb.intin[1] = flag;

  o_vdipb.contrl[VDI_HANDLE] = handle;
  o_vdipb.contrl[ROUTINE] = 26;
  vdi_call(&o_vdipb);

  rgb[0] = o_vdipb.intout[1];
  rgb[1] = o_vdipb.intout[2];
  rgb[2] = o_vdipb.intout[3];
}

void vql_attributes(int handle, int attr[6])
{
  o_vdipb.contrl[VDI_HANDLE] = handle;
  o_vdipb.contrl[ROUTINE] = 35;
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
  o_vdipb.contrl[VDI_HANDLE] = handle;
  o_vdipb.contrl[ROUTINE] = 36;
  vdi_call(&o_vdipb);

  attr[0] = o_vdipb.intout[0];
  attr[1] = o_vdipb.intout[1];
  attr[2] = o_vdipb.intout[2];

  attr[3] = o_vdipb.ptsout[0];
  attr[4] = o_vdipb.ptsout[1];
}

void vqf_attributes(int handle, int attr[5])
{
  o_vdipb.contrl[VDI_HANDLE] = handle;
  o_vdipb.contrl[ROUTINE] = 37;
  vdi_call(&o_vdipb);

  attr[0] = o_vdipb.intout[0];
  attr[1] = o_vdipb.intout[1];
  attr[2] = o_vdipb.intout[2];
  attr[3] = o_vdipb.intout[3];
  attr[4] = o_vdipb.intout[4];
}

void vqt_attributes(int handle, int attr[10])
{
  o_vdipb.contrl[VDI_HANDLE] = handle;
  o_vdipb.contrl[ROUTINE] = 38;
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
  int i;

  i=0;
  while(string[i])
    {
      o_vdipb.intin[i] = (short)string[i];
      i++;
    }
  o_vdipb.contrl[N_INTIN] = i;

  o_vdipb.contrl[VDI_HANDLE] = handle;
  o_vdipb.contrl[ROUTINE] = 116;
  vdi_call(&o_vdipb);

  for(i=0 ; i<8 ; i++)
    coords[i] = o_vdipb.ptsin[i];
}

void vqt_width(int handle, char ch, int *cwidth, int *v_offset, int *h_offset)
{
  o_vdipb.intin[0] = (short)ch;

  o_vdipb.contrl[VDI_HANDLE] = handle;
  o_vdipb.contrl[ROUTINE] = 117;
  vdi_call(&o_vdipb);

  *cwidth = o_vdipb.ptsout[0];
  *v_offset = o_vdipb.ptsout[2];
  *h_offset = o_vdipb.ptsout[4];
}

void vqin_mode(int handle, int dev, int *mode)
{
  o_vdipb.intin[0] = dev;

  o_vdipb.contrl[VDI_HANDLE] = handle;
  o_vdipb.contrl[ROUTINE] = 115;
  vdi_call(&o_vdipb);

  *mode = o_vdipb.intout[0];
}

void vq_chcells(int handle, int *rows, int *cols)
{
  o_vdipb.contrl[VDI_HANDLE] = handle;
  o_vdipb.contrl[ROUTINE] = 5;
  o_vdipb.contrl[SUBROUTINE] = 1;
  vdi_call(&o_vdipb);

  *rows = o_vdipb.intout[0];
  *cols = o_vdipb.intout[1];
}

void v_exit_cur(int handle)
{
  o_vdipb.contrl[VDI_HANDLE] = handle;
  o_vdipb.contrl[ROUTINE] = 5;
  o_vdipb.contrl[SUBROUTINE] = 2;
  vdi_call(&o_vdipb);
}

void v_enter_cur(int handle)
{
  o_vdipb.contrl[VDI_HANDLE] = handle;
  o_vdipb.contrl[ROUTINE] = 5;
  o_vdipb.contrl[SUBROUTINE] = 3;
  vdi_call(&o_vdipb);
}

void v_curup(int handle)
{
  o_vdipb.contrl[VDI_HANDLE] = handle;
  o_vdipb.contrl[ROUTINE] = 5;
  o_vdipb.contrl[SUBROUTINE] = 4;
  vdi_call(&o_vdipb);
}

void v_curdown(int handle)
{
  o_vdipb.contrl[VDI_HANDLE] = handle;
  o_vdipb.contrl[ROUTINE] = 5;
  o_vdipb.contrl[SUBROUTINE] = 5;
  vdi_call(&o_vdipb);
}

void v_curright(int handle)
{
  o_vdipb.contrl[VDI_HANDLE] = handle;
  o_vdipb.contrl[ROUTINE] = 5;
  o_vdipb.contrl[SUBROUTINE] = 6;
  vdi_call(&o_vdipb);
}

void v_curleft(int handle)
{
  o_vdipb.contrl[VDI_HANDLE] = handle;
  o_vdipb.contrl[ROUTINE] = 5;
  o_vdipb.contrl[SUBROUTINE] = 7;
  vdi_call(&o_vdipb);
}

void v_curhome(int handle)
{
  o_vdipb.contrl[VDI_HANDLE] = handle;
  o_vdipb.contrl[ROUTINE] = 5;
  o_vdipb.contrl[SUBROUTINE] = 8;
  vdi_call(&o_vdipb);
}

void v_eeos(int handle)
{
  o_vdipb.contrl[VDI_HANDLE] = handle;
  o_vdipb.contrl[ROUTINE] = 5;
  o_vdipb.contrl[SUBROUTINE] = 9;
  vdi_call(&o_vdipb);
}

void v_eeol(int handle)
{
  o_vdipb.contrl[VDI_HANDLE] = handle;
  o_vdipb.contrl[ROUTINE] = 5;
  o_vdipb.contrl[SUBROUTINE] = 10;
  vdi_call(&o_vdipb);
}

void vs_curaddress(int handle, int row, int col)
{
  o_vdipb.intin[0] = row;
  o_vdipb.intin[1] = col;

  o_vdipb.contrl[VDI_HANDLE] = handle;
  o_vdipb.contrl[ROUTINE] = 5;
  o_vdipb.contrl[SUBROUTINE] = 11;
  vdi_call(&o_vdipb);
}

void v_curtext(int handle, char *string)
{
  int i;

  i=0;
  while(string[i])
    {
      o_vdipb.intin[i] = (short)string[i];
      i++;
    }
  o_vdipb.contrl[N_INTIN] = i;

  o_vdipb.contrl[VDI_HANDLE] = handle;
  o_vdipb.contrl[ROUTINE] = 5;
  o_vdipb.contrl[SUBROUTINE] = 12;
  vdi_call(&o_vdipb);
}

void v_rvon(int handle)
{
  o_vdipb.contrl[VDI_HANDLE] = handle;
  o_vdipb.contrl[ROUTINE] = 5;
  o_vdipb.contrl[SUBROUTINE] = 13;
  vdi_call(&o_vdipb);
}

void v_rvoff(int handle)
{
  o_vdipb.contrl[VDI_HANDLE] = handle;
  o_vdipb.contrl[ROUTINE] = 5;
  o_vdipb.contrl[SUBROUTINE] = 14;
  vdi_call(&o_vdipb);
}

void vq_curaddress(int handle, int *row, int *col)
{
  o_vdipb.contrl[VDI_HANDLE] = handle;
  o_vdipb.contrl[ROUTINE] = 5;
  o_vdipb.contrl[SUBROUTINE] = 15;
  vdi_call(&o_vdipb);

  *row = o_vdipb.intout[0];
  *col = o_vdipb.intout[1];
}

void v_hardcopy(int handle)
{
  o_vdipb.contrl[VDI_HANDLE] = handle;
  o_vdipb.contrl[ROUTINE] = 5;
  o_vdipb.contrl[SUBROUTINE] = 17;
  vdi_call(&o_vdipb);
}



