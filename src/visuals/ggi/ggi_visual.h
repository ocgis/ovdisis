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
  ggi_visual_t vis;          /* This is the GGI visual */
  ggi_pixel    color_mapped[256];
  ggi_color    color_unmapped[256];
  int          write_mode;   /* Takes values defined by the VDI */
} private_t;


#define VISUAL_T(private)       (((private_t *)private)->vis)
#define COLOR_MAPPED(private)   (((private_t *)private)->color_mapped)
#define COLOR_UNMAPPED(private) (((private_t *)private)->color_unmapped)
#define WRITE_MODE(private)     (((private_t *)private)->write_mode)
