/*
** ggi_visual_mouse.h
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

/*
** Description
** Save background before drawing mouse
*/
void
ggi_visual_save_mouse_bg (void * fb,
			  int    x,
			  int    y,
			  int    w,
			  int    h);

/*
** Description
** Restore mouse background
*/
void
ggi_visual_restore_mouse_bg (void * fb);
