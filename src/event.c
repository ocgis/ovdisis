/*
** event.c
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

#include <pthread.h>
#include <signal.h>
#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>

#define DEBUGLEVEL 3

#include "event.h"
#include "ovdisis.h"
#include "various.h"
#include "mouse.h"
#include "keyboard.h"
#include "timer.h"

/* This is just to remember the physical workstation that started the
 * event handler */
static VDI_Workstation *global_vwk;

/* This is needed for the timer handler */
static pthread_t        event_handler_thread;


/*
** Description
** This is the event handler that handles keyboard, mouse and timer events
*/
static 
void
event_handler (VDI_Workstation * vwk) {
  Visual_Event     visual_event;

  pthread_setcanceltype(PTHREAD_CANCEL_ASYNCHRONOUS, NULL);
  
  /* Endless loop that will pump events from the visual and pass them over
   * to the right manager (mouse, keyboard) */
  while (1) {
    VISUAL_GET_EVENT(vwk, &visual_event);

    switch(visual_event.type)
    {
    case Visual_Key_Press_Event  :
    case Visual_Key_Repeat_Event :
      key_event( visual_event );
      break;

    case Visual_Key_Release_Event :
      /* Do nothing */
      break;

    case Visual_Mouse_Button_Event :
      if (vwk->butv != NULL) {
        vwk->butv (visual_event.mouse_button.buttons);
      }
      set_mouse_buttons( visual_event.mouse_button.buttons );
      break;

    case Visual_Mouse_Move_Event :
      mouse_moved( visual_event.mouse_move.x,
		   visual_event.mouse_move.y );
      break;

    default:
      fprintf (stderr, "ovdisis: event.c: Unknown event\n");
    }
  }
}


/*
** Description
** Initialize the event handler and startup a thread to handle mouse,
** keyboard and timer events.
*/
void
start_event_handler (VDI_Workstation * vwk)
{
  int kb_ok    = 1;
  int mouse_ok = 1;
  int timer_ok = 1;

  /* Initialize global vwk that is used by timer and mouse handlers */
  global_vwk = vwk;

  if( !start_keyboard_manager( ) ) {
    fprintf( stderr, "ovdisis:event.c: Could not start keyboard manager !\n" );
    kb_ok = 0;
  }

  if( !start_mouse_manager( vwk )  ) {
    fprintf( stderr, "ovdisis:event.c: Could not start mouse manager !\n" );
    mouse_ok = 0;
  }

  if( !start_timer_manager( vwk )  ) {
    fprintf( stderr, "ovdisis:event.c: Could not start timer manager !\n" );
    timer_ok = 0;
  }

  if( !mouse_ok && !kb_ok && !timer_ok ) {
    fprintf( stderr, "ovdisis:event.c: Problem while starting event handler, aborting !\n" );
    if( kb_ok )
      terminate_keyboard_manager( );

    if( mouse_ok )
      terminate_mouse_manager( );

    if( timer_ok )
      terminate_timer_manager( );

    return; /* VB: is this the best thing to do ????? */
  }

  /* Create a new thread */
  if (pthread_create (&event_handler_thread,
                      NULL,
                      (void *) &event_handler,
                      vwk) != 0)
    {
      ;
    }
}


/*
** Description
** Kill event handler loop
*/
void
stop_event_handler (void)
{
  pthread_cancel(event_handler_thread);
  DEBUG2("event.c: event handler stopped\n");
}
