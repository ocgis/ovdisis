/*
** ggi_visual_event.c
**
** Copyright 1999 Christer Gustavsson <cg@nocrew.org>
**
** This program is free software; you can redistribute it and/or modify
** it under the terms of the GNU General Public License as published by
** the Free Software Foundation; either version 2 of the License, or
** (at your option) any later version.
**  
** Read the file COPYING for more information.
**
*/

#include <ggi/ggi.h>
#include <stdio.h>

#include "ggi_visual.h"
#include "ovdisis.h"

static int old_buttons = 0; /* FIXME */

void
ggi_visual_get_event (void *         vis,
		      Visual_Event * visual_event) {
  gii_event event_buffer;
  int       err;

  err = ggiEventRead(VISUAL_T(vis),
                     &event_buffer,
                     emKey | emPointer);

  switch(event_buffer.any.type)
  {
  case evPtrAbsolute:
    visual_event->type = Visual_Mouse_Move_Event;
    visual_event->mouse_move.x = event_buffer.pmove.x;
    visual_event->mouse_move.y = event_buffer.pmove.y;
    break;

  case evPtrRelative:
    fprintf(stderr, "evPtrRelative\n");
    break;

  case evPtrButtonPress:
    old_buttons |= (1 << (event_buffer.pbutton.button - 1));
    visual_event->type = Visual_Mouse_Button_Event;
    visual_event->mouse_button.buttons = old_buttons;
    visual_event->mouse_button.state = 0;
    break;

  case evPtrButtonRelease:
    old_buttons &= ~(1 << (event_buffer.pbutton.button - 1));
    visual_event->type = Visual_Mouse_Button_Event;
    visual_event->mouse_button.buttons = old_buttons;
    visual_event->mouse_button.state = 0;
    break;

  case evKeyPress:
    fprintf(stderr, "evKeyPress\n");
    break;

  case evKeyRelease:
    fprintf(stderr, "evKeyRelease\n");
    break;

  case evKeyRepeat:
    fprintf(stderr, "evKeyRepeat\n");
    break;

  default:
    fprintf(stderr, "Unknown GII type %d in ggi_visual_get_event\n",
            event_buffer.any.type);
  }
}
