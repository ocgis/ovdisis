/*
** ofbis_visual_cmap.h
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

void
ofbis_visual_set_cmap (void * fb,
		       int    index,
		       int    red,
		       int    green,
		       int    blue);

void
ofbis_visual_get_cmap (void * fb,
		       int    index,
		       int *  red,
		       int *  green,
		       int *  blue);

void
ofbis_visual_put_cmap (VDI_Workstation * wk);

void
ofbis_visual_free_cmap (VDI_Workstation * wk);
