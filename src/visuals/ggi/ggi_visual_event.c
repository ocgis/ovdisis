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
    fprintf(stderr, "evPtrButtonPress\n");
    break;

  case evPtrButtonRelease:
    fprintf(stderr, "evPtrButtonRelease\n");
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
