/*
 * ovdisis_types.h
 *
 * Copyright 1999 Christer Gustavsson <cg@nocrew.org>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *  
 * Read the file COPYING for more information.
 *
 */

#ifndef _OVDISIS_TYPES_H_
#define _OVDISIS_TYPES_H_

/* Defines for vdipb->contrl[] */
#define ROUTINE 0
#define N_PTSIN 1
#define N_PTSOUT 2
#define N_INTIN 3
#define N_INTOUT 4
#define SUBROUTINE 5
#define VDI_HANDLE 6

typedef struct _fontinfo
{
  short id,size;
  char name[32];
  short first,last;
  short top,ascent,half,descent,bottom;
  short wchar,wcell;
  short left,right,thick,underline;
  unsigned short light,skew;
  short flags;
  short *character_offset;
  short *horizontal_offset;
  unsigned char *data;
  short formwidth,formheight;
  struct _fontinfo *next;
} FontInfo;


#endif /* _OVDISIS_TYPES_H_ */
