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
#include <sys/time.h>
#include <stdlib.h>

#include "event.h"
#include "ovdisis.h"

#ifndef TRUE
#define TRUE 1
#endif
static pthread_t event_handler_thread;

/* This is needed for the timer handler */
static VDI_Workstation * global_vwk;

/* If this is positive the mouse cursor is visible */
static int mouse_visibility = 0;

static pthread_mutex_t mouse_mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t key_cond = PTHREAD_COND_INITIALIZER;
pthread_mutex_t key_mutex = PTHREAD_MUTEX_INITIALIZER;

/* Mouse cursor coordinates */
static int mouse_x = 0;
static int mouse_y = 0;

/* Character buffers from keyboard input 
 * These have to be global so that vsm_string can read them
 */
int *key_scancode_buffer, *key_ascii_buffer;
int key_first_index, key_amount, key_buffer_length;

#define KEY_BUFFER_LENGTH 4096


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
** 1999-08-29 CG
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
        VISUAL_PUT_PIXEL (vwk, x + xoff, y + yoff, 0xffff);
      } else if (which & mask[yoff]) {
        VISUAL_PUT_PIXEL (vwk, x + xoff, y + yoff, 0x0000);
      }

      which >>= 1;
    }
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
** 1999-01-02 CG
** 1999-05-20 CG
** 1999-08-29 CG
*/
static
void
event_handler (VDI_Workstation * vwk) {
  Visual_Event     visual_event;
  unsigned int     buttons = 0;
  struct itimerval timer_value = {{0, 50000}, {0, 50000}};
  struct itimerval old_timer_value;
  int              enable_keyboard;
  int              key_next_index;
  static int       key_index_looped = 0;

  /* Install a timer handler */
  signal (SIGALRM, &timer_handler);
  setitimer (ITIMER_REAL, &timer_value, &old_timer_value);

  /* Save mouse background and draw mouse */
  if (mouse_visibility > 0) {
    VISUAL_SAVE_MOUSE_BG (vwk, mouse_x, mouse_y);
    draw_mouse_cursor (vwk, mouse_x, mouse_y);
  }

  enable_keyboard = 1;
  key_scancode_buffer = (int *)malloc(KEY_BUFFER_LENGTH);
  if(key_scancode_buffer == NULL) {
    fprintf(stderr, "ovdisis: event.c: Incredible, not even %d bytes to "
	    "spare for the scancode buffer\n", KEY_BUFFER_LENGTH);
    enable_keyboard = 0;
  }
  key_ascii_buffer = (int *)malloc(KEY_BUFFER_LENGTH);
  if(key_ascii_buffer == NULL) {
    fprintf(stderr, "ovdisis: event.c: Incredible, not even %d bytes to "
	    "spare for the ascii buffer\n", KEY_BUFFER_LENGTH);
    enable_keyboard = 0;
  }
  key_first_index = -1;
  key_next_index = 0;
  key_amount = 0;
  key_buffer_length = KEY_BUFFER_LENGTH;

  while (TRUE) {
    VISUAL_GET_EVENT(vwk, &visual_event);

    if (visual_event.type == Visual_Key_Event && enable_keyboard) {
      /* We only care if the key is pressed down, not released
       * Figure out how to make repeat if a key is held down.
       */
      if(visual_event.key.keycode & 0x80) {
	pthread_mutex_lock(&key_mutex);
	
	key_scancode_buffer[key_next_index] = visual_event.key.keycode;
	key_ascii_buffer[key_next_index] = visual_event.key.ascii;
	key_amount++;
	if(key_first_index == -1)
	  key_first_index = 0;
	else if(key_index_looped && key_amount == KEY_BUFFER_LENGTH)
	  key_first_index = (key_next_index+1) % KEY_BUFFER_LENGTH;
	if(key_amount >= KEY_BUFFER_LENGTH) {
	  key_index_looped = 1;
	  key_amount = KEY_BUFFER_LENGTH;
	}
	key_next_index = (key_next_index+1) % KEY_BUFFER_LENGTH;
	
	pthread_mutex_unlock(&key_mutex);
	pthread_cond_broadcast(&key_cond);
      } 
    } else if (visual_event.type == Visual_Mouse_Event) {
      /* Has one or more of the buttons changed? */
      if (visual_event.mouse.buttons != buttons) {
        if (vwk->butv != NULL) {
          vwk->butv (map_buttons (visual_event.mouse.buttons));
        }
        buttons = visual_event.mouse.buttons;
      }

      /* Has the mouse been moved? */
      if ((visual_event.mouse.x != 0) || (visual_event.mouse.y != 0)) {
        /* Make sure the visibility isn't being updated */
        pthread_mutex_lock (&mouse_mutex);

        if (mouse_visibility > 0) {
          VISUAL_RESTORE_MOUSE_BG (vwk);
        }
        
        mouse_x += visual_event.mouse.x;
        mouse_y += visual_event.mouse.y;
        
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
          VISUAL_SAVE_MOUSE_BG (vwk, mouse_x, mouse_y);
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
** 1999-05-20 CG
*/
void
start_event_handler (VDI_Workstation * vwk)
{
  /* Initialize global vwk that is used by timer and mouse handlers */
  global_vwk = vwk;

  /* Create a new thread */
  if (pthread_create (&event_handler_thread,
                      NULL,
                      (void *) &event_handler,
                      vwk) < 0) {
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
  if(key_scancode_buffer)
    free(key_scancode_buffer);
  if(key_ascii_buffer)
    free(key_ascii_buffer);
  key_scancode_buffer = NULL;
  key_ascii_buffer = NULL;

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
** 1999-08-29 CG
*/
void
increase_mouse_visibility (void) {
  pthread_mutex_lock (&mouse_mutex);

  if (mouse_visibility == 0) {
    VISUAL_SAVE_MOUSE_BG (global_vwk, mouse_x, mouse_y);
    draw_mouse_cursor (global_vwk, mouse_x, mouse_y);
  }

  mouse_visibility++;

  pthread_mutex_unlock (&mouse_mutex);
}


/*
** Exported
**
** 1999-01-03 CG
** 1999-08-29 CG
*/
void
decrease_mouse_visibility (void) {
  pthread_mutex_lock (&mouse_mutex);

  mouse_visibility--;

  if (mouse_visibility == 0) {
    VISUAL_RESTORE_MOUSE_BG(global_vwk);
  }

  pthread_mutex_unlock (&mouse_mutex);
}
