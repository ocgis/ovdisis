/*
** ggi_visual.h
**
** Copyright 1999 Christer Gustavsson <cg@nocrew.org>
**
** This program is free software; you can redistribute it and/or modify
** it under the terms of the GNU General Public License as published by
** the Free Software Foundation; either version 2 of the License, or
** (at your option) any later version.
**  
** Read the file COPYING for more information.
**
*/

typedef struct {
  ggi_visual_t vis;
  ggi_pixel    colours[256];
} private_t;

#define VISUAL_T(private) (((private_t *)private)->vis)
#define COLOURS(private) (((private_t *)private)->colours)
