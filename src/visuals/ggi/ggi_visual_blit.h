/*
** ggi_visual_blit.h
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
ggi_visual_bitblt (VWKREF vwk,
		   int    mode,
		   RECT * srccor,
		   RECT * dstcor,
		   MFDB * src,
		   MFDB * dst);

void
ggi_visual_bitbltt (VWKREF vwk,
		    int    mode,
		    int    fgcol,
		    int    bgcol,
		    RECT * srccor,
		    RECT * dstcor,
		    MFDB * src,
		    MFDB * dst);
