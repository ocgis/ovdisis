/*
 * extern_ovdisis.h
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

#ifndef _EXTERN_OVDISIS_H_
#define _EXTERN_OVDISIS_H_

typedef struct
{
    short contrl[15];
    short intin[132];
    short ptsin[145];
    short intout[140];
    short ptsout[145];
} VDIPARBLK;

typedef struct
{
  short *contrl;
  short *intin;
  short *ptsin;
  short *intout;
  short *ptsout;
} VDIPB;

extern void vdi_call( VDIPB * );

#endif /* _EXTERN_OVDISIS_H_ */
                                        