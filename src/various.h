/*
 * various.h
 *
 * Copyright 1998 Tomas Berndtsson <tomas@nocrew.org>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *  
 * Read the file COPYING for more information.
 *
 */

#ifndef _VARIOUS_H_
#define _VARIOUS_H_

extern inline int do_pointclip(int, int, RECT *);
extern inline int do_rectclip(RECT *, RECT *);
extern int do_lineclip(RECT *, RECT *);
extern inline void fix_rect(RECT *);

extern inline int gem2tos_color(int, int);
extern inline int tos2gem_color(int, int);
extern inline unsigned long get_color(VDI_Workstation *, int);

#endif /* _VARIOUS_H_ */
