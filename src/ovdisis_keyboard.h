/*
** ovdisis_keyboard.h
**
** Copyright 1999 Christer Gustavsson <cg@nocrew.org>
**
** This program is free software; you can redistribute it and/or modify
** it under the terms of the GNU General Public License as published by
** the Free Software Foundation; either version 2 of the License, or
** (at your option) any later version.
**  
** Read the file COPYING for more information.
*/

#ifndef _OVDISIS_KEYBOARD_H
#define _OVDISIS_KEYBOARD_H

/* Decoding and encoding help functions */
#define VDI_KEY(scan,val) (((scan) << 8) | val)
#define VDI_KSCAN(x)      ((x) >> 8)
#define VDI_KVAL(x)       ((x) & 0xff)

/* Keys with Atari IKBD US scancodes */
#define VDI_K_BackSpace  0x0e08
#define VDI_K_Return     0x1c0d
#define VDI_K_Tab        0x0f09

/* Special keys */
#define VDI_K_Help       0x6200
#define VDI_K_Undo       0x6100
#define VDI_K_Insert     0x5200
#define VDI_K_ClearHome  0x4700
#define VDI_K_UpArrow    0x4800
#define VDI_K_DownArrow  0x5000
#define VDI_K_LeftArrow  0x4b00
#define VDI_K_RightArrow 0x4d00
#define VDI_K_F1         0x3b00
#define VDI_K_F2         0x3c00
#define VDI_K_F3         0x3d00
#define VDI_K_F4         0x3e00
#define VDI_K_F5         0x3f00
#define VDI_K_F6         0x4000
#define VDI_K_F7         0x4100
#define VDI_K_F8         0x4200
#define VDI_K_F9         0x4300
#define VDI_K_F10        0x4400
#define VDI_K_F11        0x5400
#define VDI_K_F12        0x5500
#define VDI_K_F13        0x5600
#define VDI_K_F14        0x5700
#define VDI_K_F15        0x5800
#define VDI_K_F16        0x5900
#define VDI_K_F17        0x5a00
#define VDI_K_F18        0x5b00
#define VDI_K_F19        0x5c00
#define VDI_K_F20        0x5d00


#endif /* _OVDISIS_KEYBOARD_H */
