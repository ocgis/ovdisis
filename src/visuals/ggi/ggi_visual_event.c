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
#include "ovdisis_keyboard.h"

static int old_buttons = 0; /* FIXME */

static
inline
void
map_key(gii_key_event *         ge,
        Visual_Key_Event_Type * ve)
{
  switch(GII_KTYP(ge->sym))
  {
  case GII_KT_FN :
    ve->state = 0;
    /* FIXME: CTRL and ALT */
    switch(ge->sym)
    {
    case GIIK_F1  :
    case GIIK_F2  :
    case GIIK_F3  :
    case GIIK_F4  :
    case GIIK_F5  :
    case GIIK_F6  :
    case GIIK_F7  :
    case GIIK_F8  :
    case GIIK_F9  :
    case GIIK_F10 :
      ve->keycode =
        GII_KVAL(ge->sym) - GII_KVAL(GIIK_F1) + VDI_KSCAN(VDI_K_F1);
      break;

    case GIIK_F11 :
    case GIIK_F12 :
    case GIIK_F13 :
    case GIIK_F14 :
    case GIIK_F15 :
    case GIIK_F16 :
    case GIIK_F17 :
    case GIIK_F18 :
    case GIIK_F19 :
    case GIIK_F20 :
      ve->keycode =
        GII_KVAL(ge->sym) - GII_KVAL(GIIK_F11) + VDI_KSCAN(VDI_K_F11);
      break;

    default :
      /* FIXME */
    }

    ve->ascii = 0;
    break;

  default :
    ve->state = 0;
    ve->keycode = ge->label;
    ve->ascii = GII_KVAL(ge->sym);
  }
}


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
    visual_event->type = Visual_Key_Press_Event;
    map_key(&event_buffer.key,
            &visual_event->key);
    break;

  case evKeyRelease:
    visual_event->type = Visual_Key_Release_Event;
    map_key(&event_buffer.key,
            &visual_event->key);
    break;

  case evKeyRepeat:
    visual_event->type = Visual_Key_Repeat_Event;
    map_key(&event_buffer.key,
            &visual_event->key);
    break;

  default:
    fprintf(stderr, "Unknown GII type %d in ggi_visual_get_event\n",
            event_buffer.any.type);
  }
}
