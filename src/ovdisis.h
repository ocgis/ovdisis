/*
 * ovdisis.h
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

#ifndef _OVDISIS_H_
#define _OVDISIS_H_

#include <ofbis.h>
#include "ovdisis_types.h"
#include "vdibind.h"        /* for VDI definitions */

#define MSW(a) ((unsigned short)((unsigned long)(a) >> 16))
#define LSW(a) ((unsigned short)((unsigned long)(a) & 0xffffUL))


extern VDIPB *vdipb;


typedef struct
{
  int fontid,color;
  int textheight,textwidth,cellheight,cellwidth;
  int effects,h_alignment,v_alignment,rotation;
  FontInfo *font;
} TextAttributes;

typedef struct
{
  int color,width,type,ends_beg,ends_end;
  unsigned short udsty;
} LineAttributes;

typedef struct
{
  int color,height,width,type;
} MarkerAttributes;

typedef struct
{
  int color,interior,style,perimeter,udpat_nbpl;
  unsigned short udpat[8][16];  /* [bpl][row] */
} FillAttributes;

typedef struct
{
  int x1,y1,x2,y2;
} RECT;

struct attributes_dev
{
  /* v_opnwk & v_opnvwk */
  /* intout[0]-intout[44] */
	short	xres, yres;
	short	noscale;
	short	wpixel, hpixel;
	short	cheights;
	short	linetypes, linewidths;
	short	markertypes, markersizes;
	short	faces,	patterns, hatches, colors;
	short	ngdps;
	short	cangdps[10];
	short	gdpattr[10];
	short	cancolor, cantextrot;
	short	canfillarea, cancellarray;
	short	palette;
	short	locators, valuators;
	short	choicedevs, stringdevs;
	short	wstype;

  /* ptsout[0]-ptsout[11] */
	short	minwchar, minhchar;
	short	maxwchar, maxhchar;
	short	minwline;
	short	zero5;
	short	maxwline;
	short	zero7;
	short	minwmark, minhmark;
	short	maxwmark, maxhmark;
};

struct attributes_inq
{
  /* v_extnd */
  /* intout[0]-intout[44] */
	short	screentype;
	short	bgcolors, textfx;
	short	canscale;
	short	planes, lut;
	short	rops;
	short	cancontourfill, textrot;
	short	writemodes;
	short	inputmodes;
	short	textalign, inking, rubberbanding;
	short	maxvertices, maxintin;
	short	mousebuttons;
	short	widestyles, widemodes;
        short   reserved[25+12]; /* reserved[25]-[36] == ptsout */
};

typedef union
{
  short tab[45+12];
  struct attributes_dev attr;
} Dev;
typedef union
{
  short tab[45+12];
  struct attributes_inq attr;
} Inq;

typedef struct
{
  int red[256],green[256],blue[256];
} Cmap;

typedef	struct
{
  FB *fb;
  int handle; /* Only used for debugging now */
              /* But it might be good for the workstation
		 to know who it is */

  Dev dev; /* Answers from v_opn[v]wk */
  Inq inq; /* Answers from vq_extnd */

  TextAttributes text_a;
  FillAttributes fill_a;
  LineAttributes line_a;
  MarkerAttributes marker_a;

  FontInfo *fonts;

  Cmap vdi_cmap;

  int write_mode;
  RECT clip;

  /* Event vectors */
  void (*butv) (int bstate);
  void (*curv) (int mx, int my);
  void (*motv) (int mx, int my);
  void (*timv) (void);
  void (*keyv) (int state, int ascii, int scan);

  /* Process ID for owner of this workstation */
  int pid;
} VDI_Workstation;

typedef struct
{
  VDI_Workstation *physical;  /* pointer to physical ws, containing default values */
  VDI_Workstation *vwk;       /* virtual workstation reserved by v_opnvwk */
} VDIWK;

#define MAX_HANDLES 128

/* kind of workstation */
#define WS_NOTOPEN 0
#define WS_PHYSICAL 1
#define WS_VIRTUAL 2

typedef void VdiFunction(VDI_Workstation *);
#define UNUSED (VdiFunction *)NULL

/* debug info defines */
#ifndef DEBUGLEVEL
#define DEBUGLEVEL 0
#endif

#if DEBUGLEVEL>=1
# define EDEBUG(fmt,args...) fprintf(stderr,fmt,##args)
# if DEBUGLEVEL>=2
#  define ADEBUG(fmt,args...) fprintf(stdout,fmt,##args)
#  if DEBUGLEVEL>=3
#   define IDEBUG(fmt,args...) fprintf(stdout,fmt,##args)
#  else  /* 3 */
#   define IDEBUG(fmt,args...) 
#  endif /* 3 */
# else  /* 2 */
#  define ADEBUG(fmt,args...) 
#  define IDEBUG(fmt,args...) 
# endif /* 2 */
#else  /* 1 */
# define EDEBUG(fmt,args...) 
# define ADEBUG(fmt,args...) 
# define IDEBUG(fmt,args...) 
#endif /* 1 */

#define max(a,b) ((a)>(b)?(a):(b))
#define min(a,b) ((a)<(b)?(a):(b))


#endif /* _OVDISIS_H_ */
