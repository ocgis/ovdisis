/*
 * ovdi_more.c
 *
 * Copyright 1998 Tomas Berndtsson <tomas@nocrew.org>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *  
 * Read the file COPYING for more information.
 *
 */

#include <stdio.h>
#include <unistd.h>
#include <pthread.h>

#include "ovdisis.h"

extern VDIWK wk[MAX_HANDLES];    /* declared in vdi_control.c */
extern int wk_open[MAX_HANDLES]; /* ----------- " ----------- */ 

static pthread_mutex_t keyv_mutex = PTHREAD_MUTEX_INITIALIZER;
static pthread_cond_t keyv_cond = PTHREAD_COND_INITIALIZER;
static int keyv_state, keyv_ascii, keyv_keycode;

int ovdi_getchar(int handle)
{
  int ascii=0;

  pthread_mutex_lock(&keyv_mutex);
  pthread_cond_wait(&keyv_cond, &keyv_mutex);
  if(!(keyv_keycode & 0x80))
    ascii = keyv_ascii;
  pthread_mutex_unlock(&keyv_mutex);

  return ascii;

#if 0
  if(wk_open[handle-1]) {
    return FBgetchar(wk[handle-1].physical->fb);
  } else {
    return 0;
  }
#endif
}


void ovdi_keyv(int state, int ascii, int keycode)
{
  pthread_mutex_lock(&keyv_mutex);

  keyv_state = state;
  keyv_ascii = ascii;
  keyv_keycode = keycode;
  pthread_cond_signal(&keyv_cond);

  pthread_mutex_unlock(&keyv_mutex);  
}
