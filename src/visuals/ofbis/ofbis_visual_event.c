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

#include "ofbis_visual.h"
#include "ovdisis.h"

void
ofbis_visual_get_event (void *         fb,
                        Visual_Event * visual_event) {
  FBEVENT fe;

  FBgetevent (FB_T(fb), &fe);

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
  {
    if(fe.mouse.x || fe.mouse.y)
    {
      static int old_x = 0;
      static int old_y = 0;

      /* The mouse has been moved */
      old_x += fe.mouse.x;
      if(old_x < 0)
      {
        old_x = 0;
      }
      else if(old_x >= FB_T(fb)->vinf.xres)
      {
        old_x = FB_T(fb)->vinf.xres - 1;
      }

      old_y += fe.mouse.y;
      if(old_y < 0)
      {
        old_y = 0;
      }
      else if(old_y >= FB_T(fb)->vinf.yres)
      {
        old_y = FB_T(fb)->vinf.yres - 1;
      }

      visual_event->type = Visual_Mouse_Move_Event;
      visual_event->mouse_move.x = old_x;
      visual_event->mouse_move.y = old_y;
    }
    else
    {
      /* A button has been pressed or released */
      visual_event->type = Visual_Mouse_Button_Event;
      visual_event->mouse_button.state = fe.mouse.state;
      visual_event->mouse_button.buttons = fe.mouse.buttons;
    }
  }
  break;

  default :
    ;
  }
}
