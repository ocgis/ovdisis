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
** This is the event handler that handles keyboard, mouse and timer events
**
** 1998-10-13 CG
** 1998-10-14 CG
** 1998-12-06 CG
** 1998-12-13 CG
** 1998-12-21 CG
** 1998-12-26 CG
*/
static
void
event_handler (VDI_Workstation * vwk) {
  FBEVENT          fe;
  int              x = 0;
  int              y = 0;
  unsigned int     buttons = 0;
  int              old = 0;
  struct itimerval timer_value = {{0, 50000}, {0, 50000}};
  struct itimerval old_timer_value;

  /* Install a timer handler */
  global_vwk = vwk;
  signal (SIGALRM, &timer_handler);
  setitimer (ITIMER_REAL, &timer_value, &old_timer_value);

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
        FBputpixel (vwk->fb, x, y, old);
        
        x += fe.mouse.x;
        y += fe.mouse.y;
        
        if (x < 0) {
          x = 0;
        } else if (x > vwk->dev.attr.xres) {
          x = vwk->dev.attr.xres;
        }
        
        if (y < 0) {
          y = 0;
        } else if (y > vwk->dev.attr.yres) {
          y = vwk->dev.attr.yres;
        }

        /* Has a handler been installed? */
        if (vwk->motv != NULL) {
          vwk->motv (x, y);
        }

        old = FBgetpixel (vwk->fb, x, y);
        FBputpixel (vwk->fb, x, y, 0xffff);
        
#if 0
        fprintf (stderr,
                 "ovdisis: event.c: FBMouseEvent: x = %d y = %d\n",
                 x,
                 y);
#endif
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
  fprintf (stderr, "ovdisis: event.c: start_event_handler: Trying to create thread\n");
  /* Create a new thread */
  if (pthread_create (&event_handler_thread,
                      NULL,
                      (void *) &event_handler,
                      (void *) vwk) < 0) {
    fprintf (stderr, "ovdisis: event.c: start_event_handler: Couldn't create event handler thread\n");
  }
  fprintf (stderr, "ovdisis: event.c: start_event_handler: Created thread\n");
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
