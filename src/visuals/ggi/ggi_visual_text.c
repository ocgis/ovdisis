/*
** ggi_visual_text.c
**
** Copyright 2000 Christer Gustavsson <cg@nocrew.org>
**
** This program is free software; you can redistribute it and/or modify
** it under the terms of the GNU General Public License as published by
** the Free Software Foundation; either version 2 of the License, or
** (at your option) any later version.
**  
** Read the file COPYING for more information.
**
*/

#include <ggi/ggi.h>
#include <stdio.h>

#include "ggi_visual.h"
#include "ggi_visual_text.h"
#include "ggi_visual_various.h"
#include "ovdisis.h"
#include "various.h"


extern int  fontheight_system_8x16;
extern int  fontwidth_system_8x16;

extern unsigned char fontdata_system_8x16[];

static char * set_chardata = NULL;
static int    set_charwidth = 0;
static int    set_charheight = 0;

void
ggi_visual_put_char (VWKREF vwk,
		     int    x,
		     int    y,
		     int    fgcol,
		     int    ch)
{
  int ox, oy;
  register unsigned char * chardata = &set_chardata[set_charheight * ch];
  register unsigned char   data;

  for (oy = 0; oy < set_charheight; oy++)
  {
    data = *chardata;
    chardata++;

    for (ox = 0; ox < set_charwidth; ox++)
    {
      if (data & 0x80)
      {
        ggi_visual_put_pixel(vwk, x + ox, y + oy, fgcol);
      }

      data <<= 1;
    }
  }
}


void
ggi_visual_set_font (void * vis,
		     void * data,
		     int    width,
		     int    height)
{
  set_chardata = data;
  set_charwidth = width;
  set_charheight = height;
}
