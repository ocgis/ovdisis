/*
** sdl_visual_control.h
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

#include "ovdisis.h"

void * sdl_visual_open(void);
void   sdl_visual_close(void * fb);
void   sdl_visual_clear(VWKREF vwk);
void   sdl_visual_set_write_mode(void * vis,
                                 int    write_mode);
