/*
** ofbis_visual_event.c
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

#include <ofbis.h>

#include "ovdisis.h"

void
ofbis_visual_get_event (void *         fb,
                        Visual_Event * visual_event) {
  FBEVENT fe;

  FBgetevent ((FB *)fb, &fe);

  switch (fe.type) {
  case FBNoEvent :
    visual_event->type = Visual_No_Event;
    break;

  case FBKeyEvent :
    visual_event->type = Visual_Key_Event;
    visual_event->key.state = fe.key.state;
    visual_event->key.keycode = fe.key.keycode;
    visual_event->key.ascii = fe.key.ascii;
    break;

  case FBMouseEvent :
    visual_event->type = Visual_Mouse_Event;
    visual_event->mouse.x = fe.mouse.x;
    visual_event->mouse.y = fe.mouse.y;
    visual_event->mouse.state = fe.mouse.state;
    visual_event->mouse.buttons = fe.mouse.buttons;
    break;

  default :
    ;
  }
}
