/*
** sdl_visual_control.c
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

#define DEBUGLEVEL 0

#include <SDL/SDL.h>
#include <stdio.h>
#include <stdlib.h>

#include "ovdisis.h"
#include "sdl_visual.h"
#include "sdl_visual_control.h"

void *
sdl_visual_open (void)
{
  sdl_visual_private_t * returnValue;

  DEBUG3("sdl_visual_open entered\n");

  if(SDL_Init(SDL_INIT_VIDEO) < 0)
  {
    fprintf(stderr, "Unable to init SDL: %s\n", SDL_GetError());

    returnValue = NULL;
  }
  else
  {
    SDL_Surface * screen;

    int bitDepth;

    bitDepth = SDL_VideoModeOK(640, 480, 16, SDL_HWSURFACE | SDL_ANYFORMAT);
    
    printf("bitDepth = %d\n", bitDepth);

    if(bitDepth == 0)
    {
      fprintf(stderr, "No matching video mode: %s\n", SDL_GetError());
      returnValue = NULL;
    }
    else
    {
      screen = SDL_SetVideoMode(640, 480, bitDepth, SDL_HWSURFACE);
      if(screen == NULL)
      {
        fprintf(stderr, "Unable to set 640x480 video: %s\n", SDL_GetError());
        returnValue = NULL;
      }
      else
      {
        returnValue =
          (sdl_visual_private_t *)malloc(sizeof(sdl_visual_private_t));
        
        if(returnValue != NULL)
        {
          returnValue->visual = screen;                                 
        }
      }      
    }
  }

  DEBUG3("sdl_visual_open exited\n");

  return (void *)returnValue;
}


void
sdl_visual_close(void * fb)
{
  DEBUG0("Implement sdl_visual_close\n");
}


void
sdl_visual_clear(VWKREF vwk)
{
  SDL_Surface * screen;

  DEBUG3("sdl_visual_clear entered\n");

  screen = VISUAL_T(vwk->visual->private);

  if(SDL_MUSTLOCK(screen))
  {
    if(SDL_LockSurface(screen) < 0)
    {
      return;
    }
  }

  printf("FillRect\n");
  SDL_FillRect(screen,
               NULL,
               SDL_MapRGB(screen->format, 255, 255, 255));

  if(SDL_MUSTLOCK(screen))
  {
    SDL_UnlockSurface(screen);
  }

  SDL_UpdateRect(screen, 0, 0, 0, 0);

  DEBUG3("sdl_visual_clear exited\n");
}


void
sdl_visual_set_write_mode(void * vis,
                          int    write_mode)
{
  DEBUG3("sdl_visual_set_write_mode entered\n");
  WRITE_MODE(vis) = write_mode;
  DEBUG3("sdl_visual_set_write_mode exited\n");
}
