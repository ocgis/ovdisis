/*
 * ovdisis.h
 *
 * Copyright 1998 Tomas Berndtsson <tomas@nocrew.org>
 * Copyright 1999 - 2001 Christer Gustavsson <cg@nocrew.org>
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

#include <stdio.h>

#include "ovdisis_types.h"
#include "vdibind.h" /* for VDI definitions */

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

typedef struct {
  int x;
  int y;
  int color;
} Pixel;

#define VISUAL_OPEN(vwk) (vwk->visual->private = vwk->visual->open())
#define VISUAL_CLOSE(vwk) vwk->visual->close(vwk->visual->private)
#define VISUAL_CLEAR(vwk) vwk->visual->clear(vwk)
#define VISUAL_INQUIRE(vwk, attr) \
        vwk->visual->inquire(vwk->visual->private, attr)
#define VISUAL_SET_WRITE_MODE(vwk, write_mode) \
        vwk->visual->set_write_mode(vwk->visual->private, write_mode)
#define VISUAL_FREE_CMAP(vwk) vwk->visual->free_cmap(vwk)
#define VISUAL_SET_CMAP(vwk, index, red, green, blue) \
        vwk->visual->set_cmap(vwk, index, red, green, blue)
#define VISUAL_GET_CMAP(vwk, index, red, green, blue) \
        vwk->visual->get_cmap(vwk->visual->private, index, red, green, blue)
#define VISUAL_GET_PIXEL(vwk, x, y) \
        vwk->visual->get_pixel(vwk, x, y)
#define VISUAL_PUT_PIXEL(vwk, x, y, c) \
        vwk->visual->put_pixel(vwk, x, y, c)
#define VISUAL_PUT_PIXELS(vwk, n, px) \
        vwk->visual->put_pixels(vwk, n, px)
#define VISUAL_HLINE(vwk, x1, x2, y, c) \
        vwk->visual->hline(vwk, x1, x2, y, c)
#define VISUAL_LINE(vwk, x1, y1, x2, y2, c) \
        vwk->visual->line(vwk, x1, y1, x2, y2, c)
#define VISUAL_BITBLT(vwk, mode, src_c, dst_c, src_m, dst_m) \
        vwk->visual->bitblt(vwk, mode, src_c, dst_c, src_m, dst_m)
#define VISUAL_BITBLTT(vwk, mode, fgcol, bgcol, src_c, dst_c, src_m, dst_m) \
        vwk->visual->bitbltt(vwk, mode, fgcol, bgcol, src_c, dst_c, src_m, dst_m)
#define VISUAL_PUT_CHAR(vwk, x, y, col, ch) \
        vwk->visual->put_char(vwk, x, y, col, ch)
#define VISUAL_SET_FONT(vwk, data, width, height) \
        vwk->visual->set_font(vwk->visual->private, data, width, height)
#define VISUAL_PUT_CMAP(vwk) vwk->visual->put_cmap(vwk)
#define VISUAL_SAVE_MOUSE_BG(vwk, x, y, w, h) \
        vwk->visual->save_mouse_bg(vwk->visual->private, x, y, w, h)
#define VISUAL_RESTORE_MOUSE_BG(vwk) \
        vwk->visual->restore_mouse_bg(vwk->visual->private)
#define VISUAL_GET_EVENT(vwk, event) \
        vwk->visual->get_event(vwk->visual->private, event)
#define VISUAL_MUTEX(vwk, mode) \
        vwk->visual->mutex(mode)
#define VISUAL_MUTEX_LOCK   0
#define VISUAL_MUTEX_UNLOCK 1

typedef struct {
  int x_res;
  int y_res;
  int palette_size;
  int number_of_colours;
  int bits_per_pixel;
} Visual_Attr;

typedef enum {
  Visual_No_Event,
  Visual_Key_Press_Event,
  Visual_Key_Release_Event,
  Visual_Key_Repeat_Event,
  Visual_Mouse_Move_Event,
  Visual_Mouse_Button_Event
} Visual_Event_Type;

typedef struct {
  Visual_Event_Type type;
  unsigned int      state;
  unsigned int      keycode;
  unsigned int      ascii;
} Visual_Key_Event_Type;

typedef struct {
  Visual_Event_Type type;
  int               x;
  int               y;
} Visual_Mouse_Move_Event_Type;

typedef struct {
  Visual_Event_Type type;
  unsigned int      state;
  unsigned int      buttons;
} Visual_Mouse_Button_Event_Type;

typedef union {
  Visual_Event_Type              type;
  Visual_Key_Event_Type          key;
  Visual_Mouse_Move_Event_Type   mouse_move;
  Visual_Mouse_Button_Event_Type mouse_button;
} Visual_Event;

typedef struct vdi_workstation * VWKREF;

typedef struct
{
  void * (*open)(void);                   /* Open the visual */
  void   (*close)(void *);                /* Close the visual */
  void   (*clear)(VWKREF vwk);            /* Clear visual */
  /* Inquire visual attributes */
  void   (*inquire)(void * private, Visual_Attr * attr);
  void   (*set_write_mode)(void *, int);  /* Set write mode */
  void   (*free_cmap)(VWKREF vwk);        /* Free colour map */
  void   (*set_cmap)(VWKREF, int, int, int, int); /* Set colour map entry */
  void   (*get_cmap)(void *, int, int *, int *, int *); /* Set colour map entry */
  void   (*put_cmap)(VWKREF vwk);         /* Put colourmap from workstation */
  int    (*get_pixel)(VWKREF, int, int);  /* Get pixel */
  void   (*put_pixel)(VWKREF, int, int, int);  /* Put pixel */
  void   (*put_pixels)(VWKREF, int, Pixel*);  /* Put several pixels */
  void   (*hline)(VWKREF, int, int, int, int); /* Draw a horizontal line */
  void   (*line)(VWKREF, int, int, int, int, int); /* Draw a line */
  void   (*bitblt)(VWKREF vwk,
		   int    mode,
		   RECT * src_c,
		   RECT * dest_c,
		   MFDB * src_m,
		   MFDB * dst_m);
  void   (*bitbltt)(VWKREF vwk,
		    int    mode,
		    int    fgcol,
		    int    bgcol,
		    RECT * src_c,
		    RECT * dest_c,
		    MFDB * src_m,
		    MFDB * dst_m);
  void   (*put_char)(VWKREF vwk,
		     int    x,
		     int    y,
		     int    col,
		     int    ch);
  void   (*set_font)(void * private,
		     void * data,
		     int    width,
		     int    height);
  void   (*save_mouse_bg)(void * private,
                          int    x,
                          int    y,
			  int    w,
			  int    h);
  void   (*restore_mouse_bg)(void * private);
  void   (*get_event)(void *         private,
                      Visual_Event * event);
  void   (*mutex)(int mode);

  /* Visual specific data is stored here */
  void * private;
} VDI_Visual;

typedef	struct vdi_workstation
{
  VDI_Visual * visual; /* Visual dependent callbacks and data */
  int handle;          /* Only used for debugging now */
                       /* But it might be good for the workstation
		          to know who it is */

  Dev dev;             /* Answers from v_opn[v]wk */
  Inq inq;             /* Answers from vq_extnd */

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

  /* Device modes */
  int device_mode[5];

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
# define DEBUG1(fmt,args...) fprintf(stderr,fmt,##args)
# if DEBUGLEVEL>=2
#  define ADEBUG(fmt,args...) fprintf(stderr,fmt,##args)
#  define DEBUG2(fmt,args...) fprintf(stderr,fmt,##args)
#  if DEBUGLEVEL>=3
#   define IDEBUG(fmt,args...) fprintf(stderr,fmt,##args)
#   define DEBUG3(fmt,args...) fprintf(stderr,fmt,##args)
#  else  /* 3 */
#   define IDEBUG(fmt,args...) 
#   define DEBUG3(fmt,args...) 
#  endif /* 3 */
# else  /* 2 */
#  define ADEBUG(fmt,args...) 
#  define DEBUG2(fmt,args...) 
#  define IDEBUG(fmt,args...) 
#  define DEBUG3(fmt,args...) 
# endif /* 2 */
#else  /* 1 */
# define EDEBUG(fmt,args...) 
# define DEBUG1(fmt,args...) 
# define ADEBUG(fmt,args...) 
# define DEBUG2(fmt,args...) 
# define IDEBUG(fmt,args...) 
# define DEBUG3(fmt,args...) 
#endif /* 1 */

#define DEBUG0(fmt,args...) fprintf(stderr,fmt,##args)
#define DEBUG1 EDEBUG 
#define DEBUG2 ADEBUG 
#define DEBUG3 IDEBUG 

#define max(a,b) ((a)>(b)?(a):(b))
#define min(a,b) ((a)<(b)?(a):(b))


#endif /* _OVDISIS_H_ */
