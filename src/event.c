/*
** event.c
**
** Copyright 1998 - 2000 Christer Gustavsson <cg@nocrew.org>
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
#include "various.h"

#ifndef TRUE
#define TRUE 1
#endif
static pthread_t event_handler_thread;

/* This is needed for the timer handler */
static VDI_Workstation * global_vwk;

/* If this is positive the mouse cursor is visible */
static int mouse_visibility = 0;

pthread_cond_t key_cond = PTHREAD_COND_INITIALIZER;
pthread_mutex_t key_mutex = PTHREAD_MUTEX_INITIALIZER;

/* Mouse cursor coordinates */
static int mouse_x = 0;
static int mouse_y = 0;


static
MFORM
mouse_cursor =
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


/* Character buffers from keyboard input 
 * These have to be global so that vsm_string can read them
 */
int *key_scancode_buffer, *key_ascii_buffer;
int key_first_index, key_amount, key_buffer_length;

#define KEY_BUFFER_LENGTH 4096


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
*/
static
inline
void
draw_mouse_cursor(VDI_Workstation * vwk,
                  int               x,
                  int               y)
{
  int xoff;
  int yoff;
  int fg_col, bg_col;
  
  fg_col = gem2tos_color(vwk->inq.attr.planes, mouse_cursor.mf_fg);
  bg_col = gem2tos_color(vwk->inq.attr.planes, mouse_cursor.mf_bg);

  for(yoff = 0; yoff < 16; yoff++)
  {
    unsigned short which = 0x8000;
    
    for(xoff = 0; xoff < 16; xoff++)
    {
      if(which & mouse_cursor.mf_data[yoff])
      {
	VISUAL_PUT_PIXEL(vwk, x + xoff, y + yoff, fg_col);
      }
      else if(which & mouse_cursor.mf_mask[yoff])
      {
        VISUAL_PUT_PIXEL(vwk, x + xoff, y + yoff, bg_col);
      }

      which >>= 1;
    }
  }
}


/*
** Description
** Set the mouse form
*/
void
set_mouse_form(MFORM * cursor)
{
  mouse_cursor = *cursor;
}


/*
** Description
** This is the event handler that handles keyboard, mouse and timer events
*/
static
void
event_handler (VDI_Workstation * vwk) {
  Visual_Event     visual_event;
  unsigned int     buttons = 0;
  int              enable_keyboard;
  int              key_next_index;
  static int       key_index_looped = 0;

  /* Save mouse background and draw mouse */
  if (mouse_visibility > 0)
  {
    /* Lock visual before operation */
    VISUAL_MUTEX(vwk, VISUAL_MUTEX_LOCK);

    VISUAL_SET_WRITE_MODE(vwk, MD_REPLACE);
    VISUAL_SAVE_MOUSE_BG(vwk, mouse_x, mouse_y);
    draw_mouse_cursor (vwk, mouse_x, mouse_y);

    /* Unlock visual after operation */
    VISUAL_MUTEX(vwk, VISUAL_MUTEX_UNLOCK);
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

    switch(visual_event.type)
    {
    case Visual_Key_Press_Event  :
    case Visual_Key_Repeat_Event :
      if(enable_keyboard)
      {
        /* We only care if the key is pressed down, not released */
        pthread_mutex_lock(&key_mutex);
        
        key_scancode_buffer[key_next_index] = visual_event.key.keycode;
        key_ascii_buffer[key_next_index] = visual_event.key.ascii;
        key_amount++;
        
        if(key_first_index == -1)
        {
          key_first_index = 0;
        }
        else if(key_index_looped && key_amount == KEY_BUFFER_LENGTH)
        {
          key_first_index = (key_next_index+1) % KEY_BUFFER_LENGTH;
        }
        
        if(key_amount >= KEY_BUFFER_LENGTH) {
          key_index_looped = 1;
          key_amount = KEY_BUFFER_LENGTH;
        }
        key_next_index = (key_next_index+1) % KEY_BUFFER_LENGTH;
        
        pthread_mutex_unlock(&key_mutex);
        pthread_cond_broadcast(&key_cond);
      }
      break;

    case Visual_Key_Release_Event :
      /* Do nothing */
      break;

    case Visual_Mouse_Button_Event :
      if (vwk->butv != NULL) {
        vwk->butv (visual_event.mouse_button.buttons);
      }
      buttons = visual_event.mouse_button.buttons;
      break;

    case Visual_Mouse_Move_Event :
      /* Make sure the visibility isn't being updated */
      VISUAL_MUTEX(vwk, VISUAL_MUTEX_LOCK);
      
      if (mouse_visibility > 0)
      {
        VISUAL_SET_WRITE_MODE(vwk, MD_REPLACE);
        VISUAL_RESTORE_MOUSE_BG (vwk);
      }
      
      mouse_x = visual_event.mouse_move.x;
      mouse_y = visual_event.mouse_move.y;
      
      /* Has a handler been installed? */
      if (vwk->motv != NULL) {
        vwk->motv (mouse_x, mouse_y);
      }
      
      if (mouse_visibility > 0) {
        VISUAL_SET_WRITE_MODE(vwk, MD_REPLACE);
        VISUAL_SAVE_MOUSE_BG (vwk, mouse_x, mouse_y);
        draw_mouse_cursor (vwk, mouse_x, mouse_y);
      }
        
      /* Unlock visual after operation */
      VISUAL_MUTEX(vwk, VISUAL_MUTEX_UNLOCK);
      break;

    default:
      fprintf (stderr, "ovdisis: event.c: Unknown event\n");
    }
  }
}


static struct itimerval timer_value = {{0, 50000}, {0, 50000}};
static struct itimerval old_timer_value;

/*
** Description
** Initialize the event handler and startup a thread to handle mouse,
** keyboard and timer events.
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
                      vwk) != 0)
  {
    ;
  }

  /* Install a timer handler */
  signal(SIGALRM, &timer_handler);
  setitimer(ITIMER_REAL, &timer_value, &old_timer_value);
}


/*
** Description
** Kill event handler loop
*/
void
stop_event_handler (void)
{
  if(key_scancode_buffer)
    free(key_scancode_buffer);
  if(key_ascii_buffer)
    free(key_ascii_buffer);
  key_scancode_buffer = NULL;
  key_ascii_buffer = NULL;

  signal(SIGALRM, SIG_DFL);
  setitimer(ITIMER_REAL, &old_timer_value, &old_timer_value);

  pthread_cancel(event_handler_thread);
}


/*
** Exported
*/
void
increase_mouse_visibility (void)
{
  /* Lock visual before operation */
  VISUAL_MUTEX(global_vwk, VISUAL_MUTEX_LOCK);

  if (mouse_visibility == 0)
  {
    VISUAL_SET_WRITE_MODE(global_vwk, MD_REPLACE);
    VISUAL_SAVE_MOUSE_BG (global_vwk, mouse_x, mouse_y);
    draw_mouse_cursor (global_vwk, mouse_x, mouse_y);
  }

  mouse_visibility++;

  /* Unlock visual after operation */
  VISUAL_MUTEX(global_vwk, VISUAL_MUTEX_UNLOCK);
}


/*
** Exported
*/
void
decrease_mouse_visibility (void)
{
  /* Lock visual before operation */
  VISUAL_MUTEX(global_vwk, VISUAL_MUTEX_LOCK);

  mouse_visibility--;

  if (mouse_visibility == 0) {
    VISUAL_SET_WRITE_MODE(global_vwk, MD_REPLACE);
    VISUAL_RESTORE_MOUSE_BG(global_vwk);
  }

  /* Unlock visual after operation */
  VISUAL_MUTEX(global_vwk, VISUAL_MUTEX_UNLOCK);
}
