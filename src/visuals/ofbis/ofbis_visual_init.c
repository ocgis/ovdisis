/*
** ofbis_visual_init.c
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

#include <stdio.h>

#include "ovdisis.h"
#include "ofbis_visual_cmap.h"
#include "ofbis_visual_control.h"
#include "ofbis_visual_event.h"
#include "ofbis_visual_mouse.h"
#include "ofbis_visual_various.h"

VDI_Visual *
init (void)
{
  static VDI_Visual visual = {
    ofbis_visual_open,
    ofbis_visual_close,
    ofbis_visual_clear,
    ofbis_visual_inquire,
    ofbis_visual_set_write_mode,
    ofbis_visual_free_cmap,
    ofbis_visual_set_cmap,
    ofbis_visual_get_cmap,
    ofbis_visual_put_cmap,
    ofbis_visual_get_pixel,
    ofbis_visual_put_pixel,
    ofbis_visual_hline,
    ofbis_visual_line,
    ofbis_visual_bitblt,
    ofbis_visual_bitbltt,
    ofbis_visual_put_char,
    ofbis_visual_set_font,
    ofbis_visual_save_mouse_bg,
    ofbis_visual_restore_mouse_bg,
    ofbis_visual_get_event
  };

  return &visual;
}
