/*
** ggi_visual_text.h
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

#include "ovdisis.h"

void
ggi_visual_put_char (VWKREF vwk,
		     int    x,
		     int    y,
		     int    col,
		     int    ch);

void
ggi_visual_set_font (void * fb,
		     void * data,
		     int    width,
		     int    height);
