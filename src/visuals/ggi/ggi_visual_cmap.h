/*
** ggi_visual_cmap.h
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

#include "ovdisis.h"

void
ggi_visual_set_cmap (VWKREF wk,
		     int    index,
		     int    red,
		     int    green,
		     int    blue);

void
ggi_visual_get_cmap (void * vis,
		     int    index,
		     int *  red,
		     int *  green,
		     int *  blue);

void
ggi_visual_put_cmap (VWKREF wk);

void
ggi_visual_free_cmap (VWKREF wk);
