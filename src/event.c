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

#include "ovdisis.h"

static pthread_t event_handler_thread;

/*
** Description
** This is the event handler that handles keyboard, mouse and timer events
**
** 1998-10-13 CG
** 1998-10-14 CG
*/
static
void
event_handler (VDI_Workstation * vwk) {
  FBEVENT fe;
  int     x = 0;
  int     y = 0;

  while (1) {
    fprintf (stderr, "ovdisis: event.c: before FBgetevent %d\n", getpid ());
    FBgetevent (vwk->fb, &fe);
    fprintf (stderr, "ovdisis: event.c: after FBgetevent %d\n", getpid ());

    if (fe.type == FBKeyEvent) {
      fprintf (stderr,
               "ovdisis: event.c: FBKeyEvent: state = 0x%x ascii = 0x%x\n",
               fe.key.state,
               fe.key.ascii);
    } else if (fe.type == FBMouseEvent) {
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

      fprintf (stderr,
               "ovdisis: event.c: FBMouseEvent: x = %d y = %d\n",
               x,
               y);
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
*/
void
init_event_handler (VDI_Workstation * vwk)
{
#if 0
  /* Create a new thread */
  if (pthread_create (&event_handler_thread,
                      NULL,
                      (void *) &event_handler,
                      (void *) vwk) < 0) {
    fprintf (stderr, "ovdisis: init_event_handler: Couldn't create event handler thread\n");
  }
#endif
}


/*
** Description
** Kill event handler loop
**
** 1998-10-13 CG
*/
void
exit_event_handler (void) {
  fprintf (stderr, "ovdisis: event.c: killing handler thread\n");
  pthread_kill (event_handler_thread, SIGKILL);
  fprintf (stderr, "ovdisis: event.c: killed handler thread\n");
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
