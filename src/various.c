/*
 * various.c
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
#include <signal.h>

#include "ovdisis.h"

static int gem2tos_2bpl[4]  = { 0,3,1,2 };
static int gem2tos_4bpl[16] = { 0,15,1,2,4,6,3,5,7,8,9,10,12,14,11,13 };
static int tos2gem_2bpl[4]  = { 0,2,3,1 };
static int tos2gem_4bpl[16] = { 0,2,3,6,4,7,5,8,9,10,11,14,12,15,13,1 };

inline void fix_rect(RECT *cor)
{
  RECT t;

  t.x1 = min(cor->x1, cor->x2);
  t.y1 = min(cor->y1, cor->y2);
  cor->x2 = max(cor->x1, cor->x2);
  cor->y2 = max(cor->y1, cor->y2);
  cor->x1 = t.x1;
  cor->y1 = t.y1;
}

inline void fix_linex(RECT *cor)
{
  register int t;

  if(cor->x2 < cor->x1)
    {
      t = cor->x1;
      cor->x1 = cor->x2;
      cor->x2 = t;
      t = cor->y1;
      cor->y1 = cor->y2;
      cor->y2 = t;
    }
}

inline void fix_liney(RECT *cor)
{
  register int t;

  if(cor->y2 < cor->y1)
    {
      t = cor->y1;
      cor->y1 = cor->y2;
      cor->y2 = t;
      t = cor->x1;
      cor->x1 = cor->x2;
      cor->x2 = t;
    }
}

/* inline? Could be made as a #define! */
inline int do_pointclip(int x, int y, RECT *clip)
{
  if((x < clip->x1) || (x > clip->x2) ||
     (y < clip->y1) || (y > clip->y2))
    return 0;
  else
    return 1;
}

inline int do_rectclip(RECT *cor, RECT *clip)
{
  fix_rect(cor);

  if(((cor->x1 < clip->x1) && (cor->x2 < clip->x1)) ||
     ((cor->x1 > clip->x2) && (cor->x2 > clip->x2)) ||
     ((cor->y1 < clip->y1) && (cor->y2 < clip->y1)) ||
     ((cor->y1 > clip->y2) && (cor->y2 > clip->y2)))
    return 0;

  cor->x1 = max(cor->x1, clip->x1);
  cor->y1 = max(cor->y1, clip->y1);
  cor->x2 = min(cor->x2, clip->x2);
  cor->y2 = min(cor->y2, clip->y2);

  return 1;
}

/* This HAS to be possible in a more simple way! */
/* Hmm, maybe not. */
int do_lineclip(RECT *cor, RECT *clip)
{
  fix_linex(cor);

  /* check if totally out of it */
  if(((cor->x1 < clip->x1) && (cor->x2 < clip->x1)) ||
     ((cor->x1 > clip->x2) && (cor->x2 > clip->x2)) ||
     ((cor->y1 < clip->y1) && (cor->y2 < clip->y1)) ||
     ((cor->y1 > clip->y2) && (cor->y2 > clip->y2)))
    return 0;

  if(cor->x1 == cor->x2)
    { /* Vertical line */
      if(cor->y1 < clip->y1)
	{
	  if(cor->y2 < clip->y1)
	    return 0;
	  else
	    {
	      cor->y1 = clip->y1;
	      cor->y2 = min(cor->y2, clip->y2);
	    }
	}
      else
	{
	  if(cor->y1 > clip->y2)
	    return 0;
	  else
	    cor->y2 = min(cor->y2, clip->y2);
	}
    }
  else if(cor->y1 == cor->y2)
    { /* Horizontal line */
      if(cor->x1 < clip->x1)
	{
	  if(cor->x2 < clip->x1)
	    return 0;
	  else
	    {
	      cor->x1 = clip->x1;
	      cor->x2 = min(cor->x2, clip->x2);
	    }
	}
      else
	{
	  if(cor->x1 > clip->x2)
	    return 0;
	  else
	    cor->x2 = min(cor->x2, clip->x2);
	}
    }
  else
    { /* Non straight line */
      int t1,t2;
      double kx,ky;

      /* Check again if outside. There are cases which aren't
	 covered by the first check. */
      fix_liney(cor);
      ky = ((double)(cor->x2 - cor->x1) / (double)(cor->y2 - cor->y1));
      t1 = cor->x1 - (int)((double)(cor->y1 - clip->y1) * ky);
      t2 = cor->x2 - (int)((double)(cor->y2 - clip->y2) * ky);
      if(((t1 > clip->x2) && (t2 > clip->x2)) ||
	 ((t1 < clip->x1) && (t2 < clip->x1)))
	return 0;

      fix_linex(cor);
      kx = ((double)(cor->y2 - cor->y1) / (double)(cor->x2 - cor->x1));
      t1 = cor->y1 - (int)((double)(cor->x1 - clip->x1) * kx);
      t2 = cor->y2 - (int)((double)(cor->x2 - clip->x2) * kx);
      if(((t1 > clip->y2) && (t2 > clip->y2)) ||
	 ((t1 < clip->y1) && (t2 < clip->y1)))
	return 0;


      if(cor->x1 < clip->x1)
	{
	  cor->y1 += (int)((double)(clip->x1 - cor->x1) * kx);
	  cor->x1 = clip->x1;
	}
      if(cor->y1 < clip->y1)
	{
	  cor->x1 += (int)((double)(clip->y1 - cor->y1) * ky);
	  cor->y1 = clip->y1;
	}
      else if(cor->y1 > clip->y2)
	{
	  cor->x1 += (int)((double)(clip->y2 - cor->y1) * ky);
	  cor->y1 = clip->y2;
	}
      
      if(cor->x2 > clip->x2)
	{
	  cor->y2 += (int)((double)(clip->x2 - cor->x2) * kx);
	  cor->x2 = clip->x2;
	}
      if(cor->y2 > clip->y2)
	{
	  cor->x2 += (int)((double)(clip->y2 - cor->y2) * ky);
	  cor->y2 = clip->y2;
	}
      else if(cor->y2 < clip->y1)
	{
	  cor->x2 += (int)((double)(clip->y1 - cor->y2) * ky);
	  cor->y2 = clip->y1;
	}

    }
  return 1;
}


/* Converts from GEM to physical (TOS) colour index. */
inline int gem2tos_color(int nbpl, int index)
{
  switch(nbpl)
    {
    case 1:
      return ((unsigned int)index<1  ? index : 1);
    case 2:
      return ((unsigned int)index<4  ? gem2tos_2bpl[index] : 3);
    case 4:
      return ((unsigned int)index<16 ? gem2tos_4bpl[index] : 15);
    case 8:
      return ((unsigned int)index<16 ? gem2tos_4bpl[index] : (index<256 ? index : 255));
    default: /* TrueColor mode, use 256 virtual pens */
      return ((unsigned int)index<256 ? index : 255);
    }
}

/* Converts from physical index to GEM. */
inline int tos2gem_color(int nbpl, int index)
{
  switch(nbpl)
    {
    case 1:
      return ((unsigned int)index<1 ? index : 1);
    case 2:
      return ((unsigned int)index<4 ? tos2gem_2bpl[index] : 3);
    case 4:
      return ((unsigned int)index<16 ? tos2gem_4bpl[index] : 15);
    case 8:
      return ((unsigned int)index<16 ? tos2gem_4bpl[index] : (index<256 ? index : 255));
    default: /* TrueColor mode, use 256 virtual pens */
      return ((unsigned int)index<256 ? index : 255);
    }
}

inline unsigned long get_color(VDI_Workstation *vwk, int index)
{
  unsigned long col;
  unsigned char *c;

  if(vwk->inq.attr.planes < 16)
    return index;
  else /* TrueColor mode */
    {
      c = (unsigned char *)(&col);

      c[0] = 0;
      c[1] = (unsigned char)((vwk->vdi_cmap.red[index]  *255)/1000);
      c[2] = (unsigned char)((vwk->vdi_cmap.green[index]*255)/1000);
      c[3] = (unsigned char)((vwk->vdi_cmap.blue[index] *255)/1000);

      IDEBUG("get_color: Color %4u, %4u, %4u makes %08lx\n",
	     vwk->vdi_cmap.red[index],vwk->vdi_cmap.green[index],
	     vwk->vdi_cmap.blue[index],col);

      /* FIXME
      return FBc24_to_cnative(vwk->fb, col);
      */
      return index;
    }
}


