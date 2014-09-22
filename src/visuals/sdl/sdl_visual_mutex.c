/*
** sdl_visual_mutex.c
**
** Copyright 2001 Christer Gustavsson <cg@nocrew.org>
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

#include "sdl_visual_mutex.h"
#include "ovdisis.h"

static pthread_mutex_t visual_mutex = PTHREAD_MUTEX_INITIALIZER;

void
sdl_visual_mutex(int mode)
{
  if(mode == VISUAL_MUTEX_LOCK)
  {
    pthread_mutex_lock(&visual_mutex);
  }
  else
  {
    pthread_mutex_unlock(&visual_mutex);
  }
}
