/*
** sdl_visual.h
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

#ifndef SDL_VISUAL_H
#define SDL_VISUAL_H

#include <SDL/SDL.h>
#include <vdibind.h>

typedef struct
{
  SDL_Surface * visual;        /* This is the SDL handle */
  Uint32        pens[256];
} sdl_visual_private_t;

#define VISUAL_T(private)       (((sdl_visual_private_t *)private)->visual)
#define PENS(private)           (((sdl_visual_private_t *)private)->pens)

#endif
