/*
** sdl_visual_event.c
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
#include <unistd.h>

#include "ovdisis.h"
#include "sdl_visual_event.h"


static
inline
void
map_key(SDL_keysym *            keysym,
        Visual_Key_Event_Type * ve)
{
  ve->state = 0;
  ve->keycode = keysym->scancode;
  ve->ascii = keysym->sym;

  DEBUG3("ascii = 0x%x\n", ve->ascii);
}


void
sdl_visual_get_event (void *         vis,
		      Visual_Event * visual_event)
{
  int       ok;
  SDL_Event event;

  DEBUG3("sdl_visual_get_event entered\n");

  ok = SDL_WaitEvent(&event);

  if(ok)
  {
    switch(event.type)
    {
    case SDL_MOUSEBUTTONDOWN:
    case SDL_MOUSEBUTTONUP:
    {
      static int oldButtons = 0;
      int        mappedButton;

      DEBUG3("button event\n");

      switch(event.button.button)
      {
      case SDL_BUTTON_LEFT:
        mappedButton = 0x1 /* LEFT_BUTTON */;
        break;

      case SDL_BUTTON_MIDDLE:
        mappedButton = 0x4 /* MIDDLE_BUTTON */;
        break;

      case SDL_BUTTON_RIGHT:
        mappedButton = 0x2 /* RIGHT_BUTTON */;
        break;

      default:
        DEBUG0("Unknown mouse button!\n");
        mappedButton = 0;
      }

      if(event.button.state == SDL_PRESSED)
      {
        oldButtons |= mappedButton;
      }
      else
      {
        oldButtons &= ~mappedButton;
      }

      visual_event->type = Visual_Mouse_Button_Event;
      visual_event->mouse_button.buttons = oldButtons;
      visual_event->mouse_button.state = 0;
    }
    break;

    case SDL_MOUSEMOTION:
      DEBUG3("motion event %d x %d\n", event.motion.x, event.motion.y);
      visual_event->type = Visual_Mouse_Move_Event;
      visual_event->mouse_move.x = event.motion.x;
      visual_event->mouse_move.y = event.motion.y;
      break;

    case SDL_KEYDOWN:
      DEBUG3("keydown event\n");
      visual_event->type = Visual_Key_Press_Event;
      map_key(&event.key.keysym,
              &visual_event->key);
      break;

    case SDL_KEYUP:
      DEBUG3("keyup event\n");
      visual_event->type = Visual_Key_Release_Event;
      map_key(&event.key.keysym,
              &visual_event->key);
      break;

    default:
      DEBUG3("SDL_WaitEvent returned %d\n", event.type);
    }
  }
  else
  {
    DEBUG1("SDL_WaitEvent failed!\n");
  }

  DEBUG3("sdl_visual_get_event exited\n");
}
