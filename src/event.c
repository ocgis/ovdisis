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

/*
** Description
** This is the event handler that handles keyboard, mouse and timer events
**
** 1998-10-13 CG
** 1998-10-14 CG
** 1998-12-06 CG
*/
static
void
event_handler (VDI_Workstation * vwk) {
  FBEVENT fe;
  unsigned int x = 0;
  unsigned int y = 0;
  unsigned int buttons = 0;
  int          old = 0;

  while (1) {
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
          vwk->butv(fe.mouse.buttons);
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
        } else if (x > 1023) {
          x = 1023;
        }
        
        if (y < 0) {
          y = 0;
        } else if (y > 767) {
          y = 767;
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
init_event_handler (VDI_Workstation * vwk)
{
  fprintf (stderr, "ovdisis: event.c: init_event_handler: Trying to create thread\n");
  /* Create a new thread */
  if (pthread_create (&event_handler_thread,
                      NULL,
                      (void *) &event_handler,
                      (void *) vwk) < 0) {
    fprintf (stderr, "ovdisis: init_event_handler: Couldn't create event handler thread\n");
  }
  fprintf (stderr, "ovdisis: event.c: init_event_handler: Created thread\n");
}


/*
** Description
** Kill event handler loop
**
** 1998-10-13 CG
** 1998-12-07 CG
*/
void
exit_event_handler (void) {
#if 0
  fprintf (stderr, "ovdisis: event.c: killing handler thread\n");
  pthread_kill (event_handler_thread, SIGKILL);
  fprintf (stderr, "ovdisis: event.c: killed handler thread\n");
#endif
}


/*
** Revision history
**
** 1998-10-13 CG
**            Created original
**
** 1998-10-14 CG
**            Started to use pthread_create instead of clone.
*/
