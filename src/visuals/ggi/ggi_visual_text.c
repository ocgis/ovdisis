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

void
ggi_visual_put_char (VWKREF vwk,
		     int    x,
		     int    y,
		     int    fgcol,
		     int    ch)
{
  int ox, oy;
  register unsigned char *chardata =
    &fontdata_system_8x16[fontheight_system_8x16 * ch];
  register unsigned char data;

  for (oy = 0; oy < fontheight_system_8x16; oy++)
  {
    data = *chardata;
    chardata++;

    for (ox = 0; ox < fontwidth_system_8x16; ox++)
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
		     int    height) {
  /* fprintf(stderr, "Implement ggi_visual_set_font\n"); */
}
