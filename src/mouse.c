/*
** mouse.c
**
** Copyright 1998 - 2000 Christer Gustavsson <cg@nocrew.org>
** Copyright 2003        Vincent Barrilliot <vincent_barrilliot@yahoo.com>
**
** This program is free software; you can redistribute it and/or modify
** it under the terms of the GNU General Public License as published by
** the Free Software Foundation; either version 2 of the License, or
** (at your option) any later version.
**  
** Read the file COPYING for more information.
**
*/

#include <stdio.h>

#if 0
#define DEBUGLEVEL 3
#else
#undef DEBUGLEVEL
#endif

#include "ovdisis.h"
#include "various.h"


/* Physical workstation */
static VDI_Workstation *global_vwk;


/* If this is positive the mouse cursor is visible */
static int mouse_visibility = 0;

/* Mouse cursor coordinates */
static int          mouse_x = 0;
static int          mouse_y = 0;
static unsigned int buttons = 0;  /* Mouse buttons status */

/* Maximum coordinates for the mouse */
static int max_x;
static int max_y;


/* Default mouse cursor (arrow) */
static const MFORM arrow_cursor =
{
  /* mf_xhot    */
  0,
  /* mf_yhot    */
  0,
  /* mf_nplanes */
  1,
  /* mf_fg      */
  1,
  /* mf_bg      */
  0,
  /* mf_mask    */
  {
    0xc000,
    0xe000,
    0xf000,
    0xf800,
    0xfc00,
    0xfe00,
    0xff00,
    0xff80,
    0xffc0,
    0xffe0,
    0xfe00,
    0xef00,
    0xcf00,
    0x8780,
    0x0780,
    0x0780
  },
  /* mf_data    */
  {
    0x0000,
    0x4000,
    0x6000,
    0x7000,
    0x7800,
    0x7c00,
    0x7e00,
    0x7f00,
    0x7f80,
    0x7c00,
    0x6c00,
    0x4600,
    0x0600,
    0x0300,
    0x0300,
    0x0000
  }
};


/* Mouse cursor to display */
static MFORM mouse_cursor;
static Pixel cursor_pixels[256]; /* Pixels used to draw the mouse cursor. They are already offset by the hot spot */
static Pixel offset_pixels[256]; /* Pixels computed at each redraw of the mouse cursor, offset by the mouse position */
static int   number_of_pixels;   /* Number of pixels to draw for the cursor of the mouse */


/*
** Description
** Get the position of the mouse.
*/
void get_mouse_position( short *x, short *y ) {
  *x = (short)mouse_x;
  *y = (short)mouse_y;
}


/*
** Description
** Set the status of the mouse's buttons.
*/
void set_mouse_buttons( unsigned int new_status ) {
  buttons = new_status;
}


/*
** Description
** Get the status of the mouse's buttons.
*/
void get_mouse_buttons( unsigned int *status ) {
  *status = buttons;
}


/*
** Description
** Draw the mouse cursor without locking the visual.
*/
static void draw_mouse_cursor(int x, int y) {
  int    i;
  int    pixels_to_draw = 0;
  Pixel *pixel;

  for( i = 0 ; i < number_of_pixels ; i++ ) {
    pixel    = &(offset_pixels[pixels_to_draw]);
    pixel->x = cursor_pixels[i].x + mouse_x; 
    pixel->y = cursor_pixels[i].y + mouse_y;

    /* We don't want to draw outside the limits of the display ! */
    if( (pixel->x < 0  ||  pixel->x > max_x) 
	||
	(pixel->y < 0  ||  pixel->y > max_y) )
      ; /* skip the pixel, it's outside the screen boundaries */
    else {
      pixel->color = cursor_pixels[i].color;
      pixels_to_draw++;
    }
  }

  VISUAL_PUT_PIXELS(global_vwk, pixels_to_draw, offset_pixels );
}


/*
** Description
** Set the default mouse drawing routine.
** This is used by vex_curv. Passing it NULL restores
** the default mouse drawing routine.
*/
void set_mouse_drawer(void (*curv)() ) {
  global_vwk->curv = (curv)?curv:draw_mouse_cursor;    
}


/*
** Decription
** Create an array of pixels that will correspond to the points to draw
** Only before drawing the mouse cursors, the coordinates will have to be offset
** by the hot point of the mouse
*/
void make_pixel_list_from_mform( VDI_Workstation *vwk, MFORM *cursor ) {
  int pixel_index;
  int xoff;
  int yoff;
  int fg_col, bg_col;
  int i;

  if( mouse_cursor.mf_nplanes > 1 ) {
    fg_col = gem2tos_color(vwk->inq.attr.planes, mouse_cursor.mf_fg);
    bg_col = gem2tos_color(vwk->inq.attr.planes, mouse_cursor.mf_bg);
  } else {
    fg_col = 1;
    bg_col = 0;
  }

  pixel_index = 0;
  for( yoff = 0  ;  yoff < 16  ;  yoff++ ) {
    unsigned short which = 0x8000;
    for( xoff = 0  ;  xoff < 16  ;  xoff++ ) {
      cursor_pixels[pixel_index].x = xoff - mouse_cursor.mf_xhot;
      cursor_pixels[pixel_index].y = yoff - mouse_cursor.mf_yhot;

      if( which & mouse_cursor.mf_data[yoff] )
	  cursor_pixels[pixel_index++].color = fg_col;
      else if( which & mouse_cursor.mf_mask[yoff] )
	  cursor_pixels[pixel_index++].color = bg_col;

      which >>= 1;
    }
  }

  number_of_pixels = pixel_index;
  /* We copy the color now, so that we don't have to do it afterwards at each redraw */
  for( i = 0 ; i < number_of_pixels ; i++ ) {
    offset_pixels[i].color = cursor_pixels[i].color;
  }
}


/*
** Description
** Set the mouse form.
*/
void set_mouse_form( VDI_Workstation *vwk, MFORM * cursor) {
  mouse_cursor = *cursor;
  make_pixel_list_from_mform( vwk, &mouse_cursor );
}


/*
** Description
** Save mouse background and draw mouse cursor without locking the visual.
*/
void do_draw_mouse( ) { 
  int w,h;

  if (mouse_visibility > 0) {
    VISUAL_SET_WRITE_MODE(global_vwk, MD_REPLACE);

    w = min(16,global_vwk->dev.attr.xres-mouse_x+1);
    h = min(16,global_vwk->dev.attr.yres-mouse_y+1);

    /* DEBUG3("do_draw_mouse: Drawing cursor at %d,%d,%d,%d\n",mouse_x,mouse_y,w,h); */
    VISUAL_SAVE_MOUSE_BG(global_vwk, mouse_x, mouse_y, w, h);
    global_vwk->curv(mouse_x, mouse_y);
  }
}


/*
** Description
** Implementation of v_dspcur. Just draw the mouse cursor, without
** saving the background, but with locking the visual.
** VB 24 Nov 2003
*/
void just_draw_mouse_cursor(int x, int y) {
  VISUAL_MUTEX(global_vwk, VISUAL_MUTEX_LOCK);
  global_vwk->curv(x,y);
  VISUAL_MUTEX(global_vwk, VISUAL_MUTEX_UNLOCK);  
}


/*
** Description
** Handler for the event generated by the evet handler whenever
** the mouse is moved.
*/
void mouse_moved( unsigned int x, unsigned int y ) {
  DEBUG3("mouse_moved: Entering\n");

  /* Make sure the visibility isn't being updated */
  VISUAL_MUTEX(global_vwk, VISUAL_MUTEX_LOCK);
      
  if (mouse_visibility > 0) {
    VISUAL_SET_WRITE_MODE(global_vwk, MD_REPLACE);
    /*    DEBUG3("mouse_moved: Restoring background.\n"); */
    VISUAL_RESTORE_MOUSE_BG (global_vwk);
  }
      
  mouse_x = x;
  mouse_y = y;
      
  /* Has a handler been installed? */
  if (global_vwk->motv != NULL) {
    WORD mx, my;
    mx = (short)mouse_x;
    my = (short)mouse_y;
    DEBUG3("mouse_moved: Calling mouse move handler...");
    global_vwk->motv (&mx, &my);
    DEBUG3("done.\n");
    mouse_x = mx;
    mouse_y = my;
  }
      
  if (mouse_visibility > 0  &&  global_vwk->curv != NULL) {
    /* DEBUG3("mouse_moved: Calling mouse cursor drawer..."); */
    do_draw_mouse();
    /* DEBUG3("done.\n"); */
  }

  /* Unlock visual after operation */
  VISUAL_MUTEX(global_vwk, VISUAL_MUTEX_UNLOCK);
  DEBUG3("mouse_moved: Exiting\n");
}


/*
** Description
** Increase the mouse visibility. If the visibility is superior to
** 0, then the mouse cursor is shown.
*/
void increase_mouse_visibility ()
{
  DEBUG3("increase_mouse_visibility: Entering\n");

  /* Lock visual before operation */
  VISUAL_MUTEX(global_vwk, VISUAL_MUTEX_LOCK);

  mouse_visibility++;
  do_draw_mouse();

  /* Unlock visual after operation */
  VISUAL_MUTEX(global_vwk, VISUAL_MUTEX_UNLOCK);

  DEBUG3("increase_mouse_visibility: Exiting\n");
}


/*
** Description
** Decrease the mouse visibilty. If the visibility is 0 or less,
** the mouse cursor is hidden.
*/
void decrease_mouse_visibility () {
  DEBUG3("decrease_mouse_visibility: Entering\n");

  /* Lock visual before operation */
  VISUAL_MUTEX(global_vwk, VISUAL_MUTEX_LOCK);

  mouse_visibility--;

  if (mouse_visibility == 0) {
    VISUAL_SET_WRITE_MODE(global_vwk, MD_REPLACE);
    VISUAL_RESTORE_MOUSE_BG(global_vwk);
  }

  /* Unlock visual after operation */
  VISUAL_MUTEX(global_vwk, VISUAL_MUTEX_UNLOCK);

  DEBUG3("decrease_mouse_visibility: Exiting\n");
}


/*
** Description
** Initializes the mouse manager
*/
int start_mouse_manager( VDI_Workstation * vwk ) {
  global_vwk = vwk;
  set_mouse_form( vwk, &mouse_cursor );
  mouse_visibility = 0;

#if 0
  max_x = global_vwk->dev.attr.xres-mouse_x+1;
  max_y = global_vwk->dev.attr.xres-mouse_y+1;
#else
  max_x = global_vwk->dev.attr.xres;
  max_y = global_vwk->dev.attr.yres;
#endif   

  set_mouse_form(global_vwk,&arrow_cursor);
  set_mouse_drawer(NULL);
  return 1;
}

/*
** Description
** Terminate the mouse manager
*/
void terminate_mouse_manager( ) {
  return;
}
