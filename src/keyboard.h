/*
** keyboard.h
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


#ifndef _KEYBOARD_H_
#define _KEYBOARD_H_

#include "ovdisis.h"


typedef struct {
  int ascii;
  int scancode;
} Ovdisis_Keyboard_Event;


extern pthread_cond_t key_cond;
extern pthread_mutex_t key_mutex;
extern int *key_scancode_buffer, *key_ascii_buffer;
extern int key_first_index, key_amount, key_buffer_length;


/** Returns 0 if buffers could not be allocated */
int start_keyboard_manager( );

void terminate_keyboard_manager( );
void key_event( const Visual_Event visual_event );

#endif
