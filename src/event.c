/*
** event.c
**
** Copyright 1998 Christer Gustavsson <cg@nocrew.org>
**
** This program is free software; you can redistribute it and/or modify
** it under the terms of the GNU General Public License as published by
** the Free Software Foundation; either version 2 of the License, or
** (at your option) any later version.
**  
** Read the file COPYING for more information.
**
*/

#include <pthread.h>
#include <signal.h>
#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>

#include "event.h"
#include "ovdisis.h"

static pthread_t event_handler_thread;

/* This is needed for the timer handler */
static VDI_Workstation * global_vwk;

/* Used when saving / restoring mouse background */
static unsigned short mouse_background_buffer[16*16*2];

/* If this is positive the mouse cursor is visible */
static int mouse_visibility = 0;

static pthread_mutex_t mouse_mutex = PTHREAD_MUTEX_INITIALIZER;

/* Mouse cursor coordinates */
static int mouse_x = 0;
static int mouse_y = 0;

/*
** Description
** Map buttons in vdi format
**
** 1998-12-13 CG
*/
static
unsigned int
map_buttons (unsigned int ofbis_buttons) {
  /*
  ** FIXME: I'm not sure if the mapping is correct for the middle and the right
  ** buttons.
  */
  return (((ofbis_buttons & 0x4) ? 0 : 1) |
          ((ofbis_buttons & 0x2) ? 0 : 2) |
          ((ofbis_buttons & 0x1) ? 0 : 4));
}


/*
** Description
** For each timer tick (20 ms) this routine is called and will call a
** callback routine that the user has setup.
**
** 1998-12-26 CG
*/
static
void
timer_handler (int signal_number) {
  if (global_vwk->timv != NULL) {
    global_vwk->timv ();
  }
}


/*
** Description
** Draw the mouse cursor
**
** 1999-01-02 CG
*/
inline
void
draw_mouse_cursor (VDI_Workstation * vwk,
                   int               x,
                   int               y) {
  int xoff;
  int yoff;

  unsigned short mask[] =
  {
    0xffff,
    0xfffe,
    0xfffc,
    0xfff8,
    0xfff0,
    0xffe0,
    0xffc0,
    0xff80,
    0xff00,
    0xfe00,
    0xfc00,
    0xf800,
    0xf000,
    0xe000,
    0xc000,
    0x8000
  };
  
  unsigned short cursor[] =
  {
    0x0000,
    0x7ffc,
    0x7ff8,
    0x7ff0,
    0x7fe0,
    0x7fc0,
    0x7f80,
    0x7f00,
    0x7e00,
    0x7c00,
    0x7800,
    0x7000,
    0x6000,
    0x4000,
    0x0000,
    0x0000
  };
  
  for (yoff = 0; yoff < 16; yoff++) {
    unsigned short which = 0x8000;

    for (xoff = 0; xoff < 16; xoff++) {
      if (which & cursor[yoff]) {
        FBputpixel (vwk->fb, x + xoff, y + yoff, 0xffff);
      } else if (which & mask[yoff]) {
        FBputpixel (vwk->fb, x + xoff, y + yoff, 0x0000);
      }

      which >>= 1;
    }
  }
}


/*
** Description
** Save background before drawing mouse
**
** 1999-01-02 CG
*/
inline
void
save_mouse_background (VDI_Workstation * vwk,
                       int               x,
                       int               y) {
  FBBLTPBLK * mouse_background;
  
  /* Setup mouse background blit */
  mouse_background = FBgetbltpblk (vwk->fb);

  mouse_background->s_xmin = x;
  mouse_background->s_ymin = y;
  mouse_background->b_wd = 16;
  mouse_background->b_ht = 16;
  mouse_background->d_form = mouse_background_buffer;
  mouse_background->d_nxln = 16; /* FIXME for current resolution */

  FBbitblt (vwk->fb, mouse_background);
}


/*
** Description
** Restore mouse background
**
** 1999-01-02 CG
*/
inline
void
restore_mouse_background (VDI_Workstation * vwk,
                          int               x,
                          int               y) {
  FBBLTPBLK * mouse_background;
  
  /* Setup mouse background blit */
  mouse_background = FBgetbltpblk (vwk->fb);
  
  mouse_background->d_xmin = x;
  mouse_background->d_ymin = y;
  mouse_background->b_wd = 16;
  mouse_background->b_ht = 16;
  mouse_background->s_form = mouse_background_buffer;
  mouse_background->s_nxln = 16; /* FIXME for current resolution */

  FBbitblt (vwk->fb, mouse_background);
}


/*
** Description
** This is the event handler that handles keyboard, mouse and timer events
**
** 1998-10-13 CG
** 1998-10-14 CG
** 1998-12-06 CG
** 1998-12-13 CG
** 1998-12-21 CG
** 1998-12-26 CG
** 1999-01-02 CG
*/
static
void
event_handler (VDI_Workstation * vwk) {
  FBEVENT          fe;
  unsigned int     buttons = 0;
  struct itimerval timer_value = {{0, 50000}, {0, 50000}};
  struct itimerval old_timer_value;

  /* Install a timer handler */
  global_vwk = vwk;
  signal (SIGALRM, &timer_handler);
  setitimer (ITIMER_REAL, &timer_value, &old_timer_value);

  /* Save mouse background and draw mouse */
  if (mouse_visibility > 0) {
    save_mouse_background (vwk, mouse_x, mouse_y);
    draw_mouse_cursor (vwk, mouse_x, mouse_y);
  }

  while (TRUE) {
    FBgetevent (vwk->fb, &fe);

    if (fe.type == FBKeyEvent) {
      fprintf (stderr,
               "ovdisis: event.c: FBKeyEvent: state = 0x%x ascii = 0x%x\n",
               fe.key.state,
               fe.key.ascii);
    } else if (fe.type == FBMouseEvent) {
      /* Has one or more of the buttons changed? */
      if (fe.mouse.buttons != buttons) {
        if (vwk->butv != NULL) {
          vwk->butv (map_buttons (fe.mouse.buttons));
        }
        buttons = fe.mouse.buttons;
      }

      /* Has the mouse been moved? */
      if ((fe.mouse.x != 0) || (fe.mouse.y != 0)) {
        /* Make sure the visibility isn't being updated */
        pthread_mutex_lock (&mouse_mutex);

        if (mouse_visibility > 0) {
          restore_mouse_background (vwk, mouse_x, mouse_y);
        }
        
        mouse_x += fe.mouse.x;
        mouse_y += fe.mouse.y;
        
        if (mouse_x < 0) {
          mouse_x = 0;
        } else if (mouse_x > vwk->dev.attr.xres) {
          mouse_x = vwk->dev.attr.xres;
        }
        
        if (mouse_y < 0) {
          mouse_y = 0;
        } else if (mouse_y > vwk->dev.attr.yres) {
          mouse_y = vwk->dev.attr.yres;
        }

        /* Has a handler been installed? */
        if (vwk->motv != NULL) {
          vwk->motv (mouse_x, mouse_y);
        }

        if (mouse_visibility > 0) {
          save_mouse_background (vwk, mouse_x, mouse_y);
          draw_mouse_cursor (vwk, mouse_x, mouse_y);
        }

        pthread_mutex_unlock (&mouse_mutex);
      }
    } else {
      fprintf (stderr, "ovdisis: event.c: Unknown event\n");
    }
  }
}


/*
** Description
** Initialize the event handler and startup a thread to handle mouse,
** keyboard and timer events.
**
** 1998-10-13 CG
** 1998-10-14 CG
** 1998-12-07 CG
*/
void
start_event_handler (VDI_Workstation * vwk)
{
  /* Create a new thread */
  if (pthread_create (&event_handler_thread,
                      NULL,
                      (void *) &event_handler,
                      (void *) vwk) < 0) {
  }
}


/*
** Description
** Kill event handler loop
**
** 1998-10-13 CG
** 1998-12-07 CG
*/
void
stop_event_handler (void) {
#if 0
  fprintf (stderr, "ovdisis: event.c: killing handler thread\n");
  pthread_kill (event_handler_thread, SIGKILL);
  fprintf (stderr, "ovdisis: event.c: killed handler thread\n");
#endif
}


/*
** Exported
**
** 1999-01-03 CG
*/
void
increase_mouse_visibility (void) {
  pthread_mutex_lock (&mouse_mutex);

  if (mouse_visibility == 0) {
    save_mouse_background (global_vwk, mouse_x, mouse_y);
    draw_mouse_cursor (global_vwk, mouse_x, mouse_y);
  }

  mouse_visibility++;

  pthread_mutex_unlock (&mouse_mutex);
}


/*
** Exported
**
** 1999-01-03 CG
*/
void
decrease_mouse_visibility (void) {
  pthread_mutex_lock (&mouse_mutex);

  mouse_visibility--;

  if (mouse_visibility == 0) {
    restore_mouse_background (global_vwk, mouse_x, mouse_y);
  }

  pthread_mutex_unlock (&mouse_mutex);
}
