/*
** keyboard.c
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
#include <sys/time.h>
#include <stdlib.h>

#include "event.h"
#include "ovdisis.h"
#include "various.h"
#include "keyboard.h"

/* Character buffers from keyboard input 
 * These have to be global so that vsm_string can read them
 */
int *key_scancode_buffer, *key_ascii_buffer;
int key_first_index, key_amount, key_buffer_length;

#define KEY_BUFFER_LENGTH 4096

pthread_cond_t  key_cond  = PTHREAD_COND_INITIALIZER;
pthread_mutex_t key_mutex = PTHREAD_MUTEX_INITIALIZER;


static int  key_next_index;

char pipe_name[FILENAME_MAX];

#if 0
Ovdisis_Keyboard_Event pump_keyboard_event( ) {
  
}
#endif

int start_keyboard_manager( ) {
  int buffers_allocated_ok = 1;

#if 0
  char pipe_name[FILENAME_MAX];

  sprintf( pipe_name, "/tmp/ovdisis-kbd.%d", getpid( ) );
  
  if( mknod( pipe_name, S_FIFO, 0 ) != E_OK ) {
    fprintf( stderr, "ovdisis:event.c: Can't get pipes for keyboard events\n" );
    return;
  }
#endif
  

  key_scancode_buffer = (int *)malloc(KEY_BUFFER_LENGTH);
  if(key_scancode_buffer == NULL) {
    fprintf(stderr, "ovdisis: event.c: Incredible, not even %d bytes to "
	    "spare for the scancode buffer\n", KEY_BUFFER_LENGTH);
    buffers_allocated_ok = 0;
  }
  key_ascii_buffer = (int *)malloc(KEY_BUFFER_LENGTH);
  if(key_ascii_buffer == NULL) {
    fprintf(stderr, "ovdisis: event.c: Incredible, not even %d bytes to "
	    "spare for the ascii buffer\n", KEY_BUFFER_LENGTH);
    buffers_allocated_ok = 0;
  }
  key_first_index = 0;
  key_next_index = 0;
  key_amount = 0;
  key_buffer_length = KEY_BUFFER_LENGTH;

  return buffers_allocated_ok;
}

void terminate_keyboard_manager( ) {

  unlink( pipe_name );

  if( key_scancode_buffer )
    free( key_scancode_buffer );

  if( key_ascii_buffer )
    free( key_ascii_buffer );

  key_scancode_buffer = NULL;
  key_ascii_buffer = NULL;

}


void key_event( const Visual_Event visual_event ) {
  /* We only care if the key is pressed down, not released */
  pthread_mutex_lock(&key_mutex);
  
  key_scancode_buffer[key_next_index] = visual_event.key.keycode;
  key_ascii_buffer[key_next_index] = visual_event.key.ascii;
  
  /* We always write the new events (good policy?) */
  key_next_index = (key_next_index + 1) % KEY_BUFFER_LENGTH;
  
  key_amount++;
  
  /* But we cannot store more than the size of the buffer, so we
   * discard the older event to make room for the new event to occur*/
  if( key_amount == KEY_BUFFER_LENGTH ) {
    key_amount = KEY_BUFFER_LENGTH-1;
    key_first_index = (key_first_index + 1) % KEY_BUFFER_LENGTH;
  }
  
  pthread_mutex_unlock(&key_mutex);
  pthread_cond_broadcast(&key_cond);
}
