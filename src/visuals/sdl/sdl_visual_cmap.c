/*
** sdl_visual_cmap.c
**
** Copyright 2001 Christer Gustavsson <cg@nocrew.org>
**
** This program is free software; you can redistribute it and/or modify
** it under the terms of the GNU General Public License as published by
** the Free Software Foundation; either version 2 of the License, or
** (at your option) any later version.
**  
** Read the file COPYING for more information.
**
*/

#define DEBUGLEVEL 3

#include <SDL/SDL.h>

#include "ovdisis.h"
#include "sdl_visual.h"
#include "sdl_visual_cmap.h"

void
sdl_visual_set_cmap(VWKREF vwk,
                    int    index,
                    int    red,
                    int    green,
                    int    blue)
{
  DEBUG3( "sdl_visual_set_cmap entered\n" );
  DEBUG3( "sdl_visual_set_cmap exited\n" );
}


void
sdl_visual_put_cmap(VWKREF vwk)
{
#define NUMBER_OF_PENS 256

  int           pen;
  SDL_Surface * screen;

  DEBUG3( "sdl_visual_put_cmap entered\n" );

  screen = VISUAL_T(vwk->visual->private);

  for(pen = 0; pen < NUMBER_OF_PENS; pen++)
  {
    PENS(vwk->visual->private)[pen] =
      SDL_MapRGB(screen->format,
                 (vwk->vdi_cmap.red[pen]   * 255) / 1000,
                 (vwk->vdi_cmap.green[pen] * 255) / 1000,
                 (vwk->vdi_cmap.blue[pen]  * 255) / 1000);

  }

  DEBUG3( "sdl_visual_put_cmap exited\n" );
}


void
sdl_visual_get_cmap (void * vis,
		     int    index,
		     int *  red,
		     int *  green,
		     int *  blue)
{
  DEBUG0("Implement sdl_visual_get_cmap\n");
}


void
sdl_visual_free_cmap (VWKREF wk)
{
  /* Do nothing? */
}
