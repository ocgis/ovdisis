/*
 * vdi_gdps.c
 *
 * Copyright 1998 Tomas Berndtsson <tomas@nocrew.org>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *  
 * Read the file COPYING for more information.
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <math.h> /* for v_arc */

#include "ovdisis.h"
#include "various.h"
#include "vdi_gdps.h"


/* Internal functions */
static inline void set_circle_pixels(VDI_Workstation *,int,int,int,int,RECT *,int);
static inline void list_circle_points(int,int,int,int *);


static void vdi_v_bar(VDI_Workstation *);
static void vdi_v_arc(VDI_Workstation *);
static void vdi_v_pieslice(VDI_Workstation *);
static void vdi_v_circle(VDI_Workstation *);
static void vdi_v_ellipse(VDI_Workstation *);
static void vdi_v_ellarc(VDI_Workstation *);
static void vdi_v_ellpie(VDI_Workstation *);
static void vdi_v_rbox(VDI_Workstation *);
static void vdi_v_rfbox(VDI_Workstation *);
static void vdi_v_justified(VDI_Workstation *);

VdiFunction *vdi_gdpfuncs[] = {
  UNUSED, vdi_v_bar, vdi_v_arc, vdi_v_pieslice, vdi_v_circle, vdi_v_ellipse,
  vdi_v_ellarc, vdi_v_ellpie, vdi_v_rbox, vdi_v_rfbox, vdi_v_justified
};


void vdi_gdps(VDI_Workstation *vwk)
{
  /* Check if specified subroutine is listed in vwk->cangdps[] */

  if(vdi_gdpfuncs[vdipb->contrl[SUBROUTINE]] && 
     (vdipb->contrl[SUBROUTINE] <= 10) && (vdipb->contrl[SUBROUTINE] >= 0))
    vdi_gdpfuncs[vdipb->contrl[SUBROUTINE]](vwk);
  else
    EDEBUG("vdi_gdps: Subroutine %d doesn't exist!\n",vdipb->contrl[SUBROUTINE]);
}


void vdi_v_bar(VDI_Workstation *vwk)
{
  int y,ni;
  unsigned long col;
  RECT cor;

  cor.x1 = vdipb->ptsin[0];
  cor.y1 = vdipb->ptsin[1];
  cor.x2 = vdipb->ptsin[2];
  cor.y2 = vdipb->ptsin[3];

  if(do_rectclip(&cor, &vwk->clip))
  {
    /* Lock visual before operation */
    VISUAL_MUTEX(vwk, VISUAL_MUTEX_LOCK);

    /* Setup write mode */
    VISUAL_SET_WRITE_MODE(vwk, vwk->write_mode);

    ni = gem2tos_color(vwk->inq.attr.planes, vwk->fill_a.color);
    col = get_color(vwk, ni);
    
    switch(vwk->fill_a.interior)
    {
    case FIS_HOLLOW:
      /* Just draw the frame. Maybe. */
      break;
    case FIS_SOLID:
      for(y=cor.y1 ; y<=cor.y2 ; y++)
        VISUAL_HLINE(vwk, cor.x1, cor.x2, y, col);
      break;
    case FIS_PATTERN:
    case FIS_HATCH:
    case FIS_USER:
      EDEBUG("v_bar: Unimplemented filling style, %d!\n", vwk->fill_a.interior);
      break;
    default:
      EDEBUG("v_bar: Unknown filling style, %d!\n", vwk->fill_a.interior);
      break;
    }
    
    /* Don't draw frame if FIS_SOLID */
    if(vwk->fill_a.perimeter && (vwk->fill_a.interior!=FIS_SOLID))
    {
      RECT lin;
      
      lin.x1 = vdipb->ptsin[0];
      lin.x2 = vdipb->ptsin[2];
      lin.y1 = lin.y2 = vdipb->ptsin[1];
      if(do_lineclip(&lin, &vwk->clip))
        VISUAL_HLINE(vwk, lin.x1, lin.x2, lin.y1, col);
      lin.x1 = vdipb->ptsin[0];
      lin.x2 = vdipb->ptsin[2];
      lin.y1 = lin.y2 = vdipb->ptsin[3];
      if(do_lineclip(&lin, &vwk->clip))
        VISUAL_HLINE(vwk, lin.x1, lin.x2, lin.y2, col);
      lin.x1 = lin.x2 = vdipb->ptsin[0];
      lin.y1 = vdipb->ptsin[1];
      lin.y2 = vdipb->ptsin[3];
      if(do_lineclip(&lin, &vwk->clip))
        VISUAL_LINE(vwk, lin.x1, lin.y1, lin.x1, lin.y2, col);
      lin.x1 = lin.x2 = vdipb->ptsin[2];
      lin.y1 = vdipb->ptsin[1];
      lin.y2 = vdipb->ptsin[3];
      if(do_lineclip(&lin, &vwk->clip))
        VISUAL_LINE(vwk, lin.x2, lin.y1, lin.x2, lin.y2, col);
    }

    /* Unock visual after operation */
    VISUAL_MUTEX(vwk, VISUAL_MUTEX_UNLOCK);
  }

  vdipb->contrl[N_PTSOUT] = 0;
  vdipb->contrl[N_INTOUT] = 0;
}

/* an idea I had to use bresenham's circle algorithm for arcs
   it's a lot to write, and question is if it's really better */
#if 0
void vdi_v_arc(VDI_Workstation *vwk)
{
  int radius,x_center,y_center, angle_start,angle_end;
  int p,x,y,ni, start_x,end_x;
  unsigned long col;

  x_center = vdipb->ptsin[0];
  y_center = vdipb->ptsin[1];
  radius = vdipb->ptsin[6];
  angle_start = vdipb->intin[0] % 3600;
  angle_end = vdipb->intin[1] % 3600;

  ni = gem2tos_color(vwk->inq.attr.planes, vwk->fill_a.color);
  col = get_color(vwk, ni);


  if(angle_start <= 450)
    start_x = radius * sin((angle_start-0) * M_PI/3600);
  else if(angle_start <= 900)
    start_x = radius * sin((900-angle_start) * M_PI/3600);
  else if(angle_start <= 1350)
    start_x = radius * sin((angle_start-1350) * M_PI/3600);
  else if(angle_start <= 1800)
    start_x = radius * sin((1800-angle_start) * M_PI/3600);
  else if(angle_start <= 2250)
    start_x = radius * sin((angle_start-2250) * M_PI/3600);
  else if(angle_start <= 2700)
    start_x = radius * sin((2700-angle_start) * M_PI/3600);
  else if(angle_start <= 3150)
    start_x = radius * sin((angle_start-3150) * M_PI/3600);
  else
    start_x = radius * sin((3600-angle_start) * M_PI/3600);

  if(angle_end <= 450)
    end_x = radius * sin((angle_end-0) * M_PI/3600);
  else if(angle_end <= 900)
    end_x = radius * sin((900-angle_end) * M_PI/3600);
  else if(angle_end <= 1350)
    end_x = radius * sin((angle_end-1350) * M_PI/3600);
  else if(angle_end <= 1800)
    end_x = radius * sin((1800-angle_end) * M_PI/3600);
  else if(angle_end <= 2250)
    end_x = radius * sin((angle_end-2250) * M_PI/3600);
  else if(angle_end <= 2700)
    end_x = radius * sin((2700-angle_end) * M_PI/3600);
  else if(angle_end <= 3150)
    end_x = radius * sin((angle_end-3150) * M_PI/3600);
  else
    end_x = radius * sin((3600-angle_end) * M_PI/3600);

  x = 0;
  y = radius;
  p = 3-2*radius;
  while(x < y) {
    if(angle_start <= 450) {
      if(angle_end <= 450) {
	
      } else if(angle_end <= 900) {
      } else if(angle_end <= 1350) {
      } else if(angle_end <= 1800) {
      } else if(angle_end <= 2250) {
      } else if(angle_end <= 2700) {
      } else if(angle_end <= 3150) {
      } else {
      }
    } else if(angle_start <= 900) {
    } else if(angle_start <= 1350) {
    } else if(angle_start <= 1800) {
    } else if(angle_start <= 2250) {
    } else if(angle_start <= 2700) {
    } else if(angle_start <= 3150) {
    } else {
    }

    if(p < 0)
      p += 4*x+6;
    else {
      p += 4*(x-y)+10;
      y--;
    }
    x++;
  }
  if(x == y)
    list_circle_points(x, y, radius, x_values);

/*   EDEBUG("v_arc: Call not implemented!\n"); */
  vdipb->contrl[N_PTSOUT] = 0;
  vdipb->contrl[N_INTOUT] = 0;
}
#endif /* idea of bresenham */


void vdi_v_arc(VDI_Workstation *vwk)
{
  unsigned int radius,x_center,y_center;
  int ni;
  unsigned long col;
  double angle, abeg, aend, astep;
  RECT lin,linp;

  /* Lock visual before operation */
  VISUAL_MUTEX(vwk, VISUAL_MUTEX_LOCK);

  /* Setup write mode */
  VISUAL_SET_WRITE_MODE(vwk, vwk->write_mode);

  x_center = vdipb->ptsin[0];
  y_center = vdipb->ptsin[1];
  radius = vdipb->ptsin[6];
  abeg = (vdipb->intin[0]%3600) * 2*M_PI/3600.0;
  aend = (vdipb->intin[1]%3600) * 2*M_PI/3600.0;

  /* wrap around aend if abeg comes after aend on the circle */
  if(abeg > aend)
    aend += 2*M_PI;

  ni = gem2tos_color(vwk->inq.attr.planes, vwk->line_a.color);
  col = get_color(vwk, ni);

  linp.x1 = x_center + radius*cos(abeg);
  linp.y1 = y_center - radius*sin(abeg);
  astep = 2*M_PI / (radius+1); /* +1 to avoid division by zero */
  for(angle=abeg+astep ; angle<aend ; angle=min(aend,angle+astep)) {
    linp.x2 = x_center + radius*cos(angle);
    linp.y2 = y_center - radius*sin(angle);

    lin = linp;
    if(do_lineclip(&lin, &vwk->clip))
      VISUAL_LINE(vwk, lin.x1,lin.y1,lin.x2,lin.y2, col);

    linp.x1 = linp.x2;
    linp.y1 = linp.y2;
  }

  /* Unlock visual after operation */
  VISUAL_MUTEX(vwk, VISUAL_MUTEX_UNLOCK);

  vdipb->contrl[N_PTSOUT] = 0;
  vdipb->contrl[N_INTOUT] = 0;
}

void vdi_v_pieslice(VDI_Workstation *vwk)
{
  EDEBUG("v_pieslice: Call not implemented!\n");

  vdipb->contrl[N_PTSOUT] = 0;
  vdipb->contrl[N_INTOUT] = 0;
}


/* Uses Bresenham's Circle Algorithm */
/* Good for whole circles, but I guess I'll
   have to come up with something better
   for arcs and pieslices */
void vdi_v_circle(VDI_Workstation *vwk)
{
  int radius,x_center,y_center;
  int p,x,y,ni;
  int *x_values;
  unsigned long col;
  RECT lin_u, lin_l;

  /* Lock visual before operation */
  VISUAL_MUTEX(vwk, VISUAL_MUTEX_LOCK);

  /* Setup write mode */
  VISUAL_SET_WRITE_MODE(vwk, vwk->write_mode);

  x_center = vdipb->ptsin[0];
  y_center = vdipb->ptsin[1];
  radius = vdipb->ptsin[4];

  IDEBUG("v_circle: x_center: %d, y_center: %d, radius: %d\n",x_center,y_center,radius);

  ni = gem2tos_color(vwk->inq.attr.planes, vwk->fill_a.color);
  col = get_color(vwk, ni);

  switch(vwk->fill_a.interior)
  {
  case FIS_HOLLOW:
    /* Just draw the frame. Maybe. */
    break;
  case FIS_SOLID:
    x_values = calloc(radius+1, sizeof(int));
    if(!x_values)
    {
      EDEBUG("v_circle: Not enough memory to allocate circle buffers!\n");
      return;
    }
    
    x = 0;
    y = radius;
    p = 3-2*radius;
    while(x < y)
    {
      /* add the points to a buffer
         which is used for filling */
      list_circle_points(x, y, radius, x_values);
      if(p < 0)
        p += 4*x+6;
      else
      {
        p += 4*(x-y)+10;
        y--;
      }
      x++;
    }
    if(x == y)
      list_circle_points(x, y, radius, x_values);
    
    /* Draw horizontal lines for each line 
       calculated previously using the
       x_values list */
    /* Lines are mirrored around the x-axis
       and then mirrored around the y-axis */
    for(y=-radius ; y<0 ; y++)
    {
      lin_l.x1 = lin_u.x1 = -x_values[y+radius] + x_center;
      lin_l.x2 = lin_u.x2 =  x_values[y+radius] + x_center;
      lin_u.y1 = lin_u.y2 =  y + y_center;
      lin_l.y1 = lin_l.y2 = -y + y_center;
      if(do_lineclip(&lin_u, &vwk->clip))
        VISUAL_HLINE(vwk, lin_u.x1, lin_u.x2, lin_u.y1, col);
      if(do_lineclip(&lin_l, &vwk->clip))
        VISUAL_HLINE(vwk, lin_l.x1, lin_l.x2, lin_l.y1, col);
    }
    /* Finally the middle line which hasn't been drawn */
    lin_l.x1 = -x_values[radius] + x_center;
    lin_l.x2 =  x_values[radius] + x_center;
    lin_l.y1 = lin_l.y2 = y_center;
    if(do_lineclip(&lin_l, &vwk->clip))
      VISUAL_HLINE(vwk, lin_l.x1, lin_l.x2, lin_l.y1, col);
    
    free(x_values);
    
    break;
  case FIS_PATTERN:
  case FIS_HATCH:
  case FIS_USER:
    EDEBUG("v_circle: Unimplemented filling style, %d!\n", vwk->fill_a.interior);
    break;
  default:
    EDEBUG("v_circle: Unknown filling style, %d!\n", vwk->fill_a.interior);
    break;
  }
  
  /* Don't draw frame if FIS_SOLID */
  if(vwk->fill_a.perimeter && (vwk->fill_a.interior!=FIS_SOLID))
  {
    x = 0;
    y = radius;
    p = 3-2*radius;
    
    while(x < y)
    {
      set_circle_pixels(vwk, x, y, x_center, y_center, &vwk->clip, col);
      if(p < 0)
        p += 4*x + 6;
      else
      {
        p += 4*(x-y) + 10;
        y--;
      }
      x++;
    }
    if(x == y)
      set_circle_pixels(vwk, x, y, x_center, y_center, &vwk->clip, col);
  }

  /* Unlock visual before operation */
  VISUAL_MUTEX(vwk, VISUAL_MUTEX_UNLOCK);

  vdipb->contrl[N_PTSOUT] = 0;
  vdipb->contrl[N_INTOUT] = 0;
}

/* Could probably be imlemented with some kind of Bresenham */
void vdi_v_ellipse(VDI_Workstation *vwk)
{
  unsigned int x_radius,y_radius,x_center,y_center;
  int ni;
  unsigned long col;
  double angle, abeg, aend, astep;
  RECT lin,linp;

  /* Lock visual before operation */
  VISUAL_MUTEX(vwk, VISUAL_MUTEX_LOCK);

  /* Setup write mode */
  VISUAL_SET_WRITE_MODE(vwk, vwk->write_mode);

  x_center = vdipb->ptsin[0];
  y_center = vdipb->ptsin[1];
  x_radius = vdipb->ptsin[2];
  y_radius = vdipb->ptsin[3];
  abeg = (0) * 2*M_PI/3600.0;
  aend = (3600) * 2*M_PI/3600.0;

  ni = gem2tos_color(vwk->inq.attr.planes, vwk->line_a.color);
  col = get_color(vwk, ni);

  linp.x1 = x_center + x_radius*cos(abeg);
  linp.y1 = y_center - y_radius*sin(abeg);
  astep = 2*M_PI / (((x_radius+y_radius)>>1)+1); /* +1 to avoid division by zero */
  for(angle=abeg+astep ; angle<aend ; angle=min(aend,angle+astep)) {
    linp.x2 = x_center + x_radius*cos(angle);
    linp.y2 = y_center - y_radius*sin(angle);

    lin = linp;
    if(do_lineclip(&lin, &vwk->clip))
      VISUAL_LINE(vwk, lin.x1,lin.y1,lin.x2,lin.y2, col);

    linp.x1 = linp.x2;
    linp.y1 = linp.y2;
  }

  /* Unlock visual after operation */
  VISUAL_MUTEX(vwk, VISUAL_MUTEX_UNLOCK);

  vdipb->contrl[N_PTSOUT] = 0;
  vdipb->contrl[N_INTOUT] = 0;
}

void vdi_v_ellarc(VDI_Workstation *vwk)
{
  unsigned int x_radius,y_radius,x_center,y_center;
  int ni;
  unsigned long col;
  double angle, abeg, aend, astep;
  RECT lin,linp;

  /* Lock visual before operation */
  VISUAL_MUTEX(vwk, VISUAL_MUTEX_LOCK);

  /* Setup write mode */
  VISUAL_SET_WRITE_MODE(vwk, vwk->write_mode);

  x_center = vdipb->ptsin[0];
  y_center = vdipb->ptsin[1];
  x_radius = vdipb->ptsin[2];
  y_radius = vdipb->ptsin[3];
  abeg = (vdipb->intin[0]%3600) * 2*M_PI/3600.0;
  aend = (vdipb->intin[1]%3600) * 2*M_PI/3600.0;

  /* wrap around aend if abeg comes after aend on the circle */
  if(abeg > aend)
    aend += 2*M_PI;

  ni = gem2tos_color(vwk->inq.attr.planes, vwk->line_a.color);
  col = get_color(vwk, ni);

  linp.x1 = x_center + x_radius*cos(abeg);
  linp.y1 = y_center - y_radius*sin(abeg);
  astep = 2*M_PI / (((x_radius+y_radius)>>1)+1); /* +1 to avoid division by zero */
  for(angle=abeg+astep ; angle<aend ; angle=min(aend,angle+astep)) {
    linp.x2 = x_center + x_radius*cos(angle);
    linp.y2 = y_center - y_radius*sin(angle);

    lin = linp;
    if(do_lineclip(&lin, &vwk->clip))
      VISUAL_LINE(vwk, lin.x1,lin.y1,lin.x2,lin.y2, col);

    linp.x1 = linp.x2;
    linp.y1 = linp.y2;
  }

  /* Unlock visual after operation */
  VISUAL_MUTEX(vwk, VISUAL_MUTEX_UNLOCK);

  vdipb->contrl[N_PTSOUT] = 0;
  vdipb->contrl[N_INTOUT] = 0;
}

void vdi_v_ellpie(VDI_Workstation *vwk)
{
  EDEBUG("v_ellpie: Call not implemented!\n");

  vdipb->contrl[N_PTSOUT] = 0;
  vdipb->contrl[N_INTOUT] = 0;
}


#define RBOX_RADIUS(rec) (min(rec.x2-rec.x1,rec.y2-rec.y1) / 5)

void vdi_v_rbox(VDI_Workstation *vwk)
{
  int x,y,p,radius, ni;
  int ul_xc,ul_yc, ur_xc,ur_yc, ll_xc,ll_yc, lr_xc,lr_yc;
  unsigned long col;
  RECT cor,lin,org;

  /* Lock visual before operation */
  VISUAL_MUTEX(vwk, VISUAL_MUTEX_LOCK);

  /* Setup write mode */
  VISUAL_SET_WRITE_MODE(vwk, vwk->write_mode);

  cor.x1 = vdipb->ptsin[0];
  cor.y1 = vdipb->ptsin[1];
  cor.x2 = vdipb->ptsin[2];
  cor.y2 = vdipb->ptsin[3];

  org = cor;
  fix_rect(&org);

  /* check if draw at all */
  if(do_rectclip(&cor, &vwk->clip))
  {
    ni = gem2tos_color(vwk->inq.attr.planes, vwk->line_a.color);
    col = get_color(vwk, ni);
    
    radius = RBOX_RADIUS(org);
    
    lin.x1 = org.x1 + radius;
    lin.x2 = org.x2 - radius;
    lin.y1 = lin.y2 = org.y1;
    if(do_lineclip(&lin, &vwk->clip))
      VISUAL_HLINE(vwk, lin.x1, lin.x2, lin.y1, col);
    lin.x1 = org.x1 + radius;
    lin.x2 = org.x2 - radius;
    lin.y1 = lin.y2 = org.y2;
    if(do_lineclip(&lin, &vwk->clip))
      VISUAL_HLINE(vwk, lin.x1, lin.x2, lin.y2, col);
    lin.x1 = lin.x2 = org.x1;
    lin.y1 = org.y1 + radius;
    lin.y2 = org.y2 - radius;
    if(do_lineclip(&lin, &vwk->clip))
      VISUAL_LINE(vwk, lin.x1, lin.y1, lin.x1, lin.y2, col);
    lin.x1 = lin.x2 = org.x2;
    lin.y1 = org.y1+ radius;
    lin.y2 = org.y2 - radius;
    if(do_lineclip(&lin, &vwk->clip))
      VISUAL_LINE(vwk, lin.x2, lin.y1, lin.x2, lin.y2, col);
    
    
    if(radius > 0) {
      /* do the rounded corners */
      ul_xc = org.x1 + radius;
      ul_yc = org.y1 + radius;
      ur_xc = org.x2 - radius;
      ur_yc = org.y1 + radius;
      ll_xc = org.x1 + radius;
      ll_yc = org.y2 - radius;
      lr_xc = org.x2 - radius;
      lr_yc = org.y2 - radius;
      
      x = 0;
      y = radius;
      p = 3-2*radius;
      
      while(x < y) {
        /* upper left corner */
        if(do_pointclip(ul_xc - x, ul_yc - y, &vwk->clip))
          VISUAL_PUT_PIXEL(vwk, ul_xc - x, ul_yc - y, col);
        if(do_pointclip(ul_xc - y, ul_yc - x, &vwk->clip))
          VISUAL_PUT_PIXEL(vwk, ul_xc - y, ul_yc - x, col);
        
        /* upper right corner */
        if(do_pointclip(ur_xc + x, ur_yc - y, &vwk->clip))
          VISUAL_PUT_PIXEL(vwk, ur_xc + x, ur_yc - y, col);
        if(do_pointclip(ur_xc + y, ur_yc - x, &vwk->clip))
          VISUAL_PUT_PIXEL(vwk, ur_xc + y, ur_yc - x, col);
        
        /* lower left corner */
        if(do_pointclip(ll_xc - x, ll_yc + y, &vwk->clip))
          VISUAL_PUT_PIXEL(vwk, ll_xc - x, ll_yc + y, col);
        if(do_pointclip(ll_xc - y, ll_yc + x, &vwk->clip))
	    VISUAL_PUT_PIXEL(vwk, ll_xc - y, ll_yc + x, col);
        
        /* lower right corner */
        if(do_pointclip(lr_xc + x, lr_yc + y, &vwk->clip))
          VISUAL_PUT_PIXEL(vwk, lr_xc + x, lr_yc + y, col);
        if(do_pointclip(lr_xc + y, lr_yc + x, &vwk->clip))
	    VISUAL_PUT_PIXEL(vwk, lr_xc + y, lr_yc + x, col);
        
        if(p < 0)
          p += 4*x + 6;
        else {
          p += 4*(x-y) + 10;
          y--;
        }
        x++;
      }
      if(x == y) {
        /* upper left corner */
        if(do_pointclip(ul_xc - x, ul_yc - y, &vwk->clip))
          VISUAL_PUT_PIXEL(vwk, ul_xc - x, ul_yc - y, col);
        if(do_pointclip(ul_xc - y, ul_yc - x, &vwk->clip))
          VISUAL_PUT_PIXEL(vwk, ul_xc - y, ul_yc - x, col);
        
        /* upper right corner */
        if(do_pointclip(ur_xc + x, ur_yc - y, &vwk->clip))
          VISUAL_PUT_PIXEL(vwk, ur_xc + x, ur_yc - y, col);
        if(do_pointclip(ur_xc + y, ur_yc - x, &vwk->clip))
          VISUAL_PUT_PIXEL(vwk, ur_xc + y, ur_yc - x, col);
        
        /* lower left corner */
        if(do_pointclip(ll_xc - x, ll_yc + y, &vwk->clip))
          VISUAL_PUT_PIXEL(vwk, ll_xc - x, ll_yc + y, col);
        if(do_pointclip(ll_xc - y, ll_yc + x, &vwk->clip))
          VISUAL_PUT_PIXEL(vwk, ll_xc - y, ll_yc + x, col);
        
        /* lower right corner */
        if(do_pointclip(lr_xc + x, lr_yc + y, &vwk->clip))
          VISUAL_PUT_PIXEL(vwk, lr_xc + x, lr_yc + y, col);
        if(do_pointclip(lr_xc + y, lr_yc + x, &vwk->clip))
          VISUAL_PUT_PIXEL(vwk, lr_xc + y, lr_yc + x, col);
      }
    }
  }
  
  /* Unlock visual after operation */
  VISUAL_MUTEX(vwk, VISUAL_MUTEX_UNLOCK);

  vdipb->contrl[N_PTSOUT] = 0;
  vdipb->contrl[N_INTOUT] = 0;
}

void vdi_v_rfbox(VDI_Workstation *vwk)
{
  int x,y,p,radius, ni;
  int ul_xc,ul_yc, ur_xc,ur_yc, ll_xc,ll_yc, lr_xc,lr_yc;
  int *x_values;
  unsigned long col;
  RECT cor,lin,org, lin_l, lin_u;

  /* Lock visual before operation */
  VISUAL_MUTEX(vwk, VISUAL_MUTEX_LOCK);

  /* Setup write mode */
  VISUAL_SET_WRITE_MODE(vwk, vwk->write_mode);

  cor.x1 = vdipb->ptsin[0];
  cor.y1 = vdipb->ptsin[1];
  cor.x2 = vdipb->ptsin[2];
  cor.y2 = vdipb->ptsin[3];

  org = cor;
  fix_rect(&org);

  /* check if draw at all */
  if(do_rectclip(&cor, &vwk->clip)) {
  
    radius = RBOX_RADIUS(org);

    ni = gem2tos_color(vwk->inq.attr.planes, vwk->fill_a.color);
    col = get_color(vwk, ni);

    switch(vwk->fill_a.interior)
    {
    case FIS_HOLLOW:
      /* Just draw the frame. Maybe. */
      break;
    case FIS_SOLID:
      x_values = calloc(radius+1, sizeof(int));
      if(!x_values)
      {
        EDEBUG("v_rfbox: Not enough memory to allocate circle buffers!\n");
        return;
      }
      
      ul_xc = org.x1 + radius;
      ul_yc = org.y1 + radius;
      ur_xc = org.x2 - radius;
      ur_yc = org.y1 + radius;
      ll_xc = org.x1 + radius;
      ll_yc = org.y2 - radius;
      lr_xc = org.x2 - radius;
      lr_yc = org.y2 - radius;
      
      x = 0;
      y = radius;
      p = 3-2*radius;
      while(x < y)
      {
        /* add the points to a buffer
           which is used for filling */
        list_circle_points(x, y, radius, x_values);
        if(p < 0)
          p += 4*x+6;
        else
        {
          p += 4*(x-y)+10;
          y--;
        }
        x++;
      }
      if(x == y)
        list_circle_points(x, y, radius, x_values);
      
      /* Draw horizontal lines for each line 
         calculated previously using the
         x_values list */
      /* Lines are mirrored around the x-axis
         and then mirrored around the y-axis */
      /* this is only the "corner" scanlines */
      for(y=-radius ; y<0 ; y++)
      {
        lin_l.x1 = lin_u.x1 = -x_values[y+radius] + ul_xc;
        lin_l.x2 = lin_u.x2 =  x_values[y+radius] + ur_xc;
        lin_u.y1 = lin_u.y2 =  y + ul_yc;
        lin_l.y1 = lin_l.y2 = -y + ll_yc;
        if(do_lineclip(&lin_u, &vwk->clip))
          VISUAL_HLINE(vwk, lin_u.x1, lin_u.x2, lin_u.y1, col);
        if(do_lineclip(&lin_l, &vwk->clip))
          VISUAL_HLINE(vwk, lin_l.x1, lin_l.x2, lin_l.y1, col);
      }
      /* Draw the middle lines */
      for(y=ul_yc ; y<=ll_yc ; y++) {
        /* have to set these for every line, because of clipping */
        lin_l.x1 = org.x1;
        lin_l.x2 = org.x2;
        lin_l.y1 = lin_l.y2 = y;
        if(do_lineclip(&lin_l, &vwk->clip))
          VISUAL_HLINE(vwk, lin_l.x1, lin_l.x2, lin_l.y1, col);
      } 
      
      free(x_values);
      
      break;
    case FIS_PATTERN:
    case FIS_HATCH:
    case FIS_USER:
      EDEBUG("v_circle: Unimplemented filling style, %d!\n", vwk->fill_a.interior);
      break;
    default:
      EDEBUG("v_circle: Unknown filling style, %d!\n", vwk->fill_a.interior);
      break;
    }
    
    /* Don't draw frame if FIS_SOLID */
    if(vwk->fill_a.perimeter && (vwk->fill_a.interior!=FIS_SOLID))
    {
      lin.x1 = org.x1 + radius;
      lin.x2 = org.x2 - radius;
      lin.y1 = lin.y2 = org.y1;
      if(do_lineclip(&lin, &vwk->clip))
        VISUAL_HLINE(vwk, lin.x1, lin.x2, lin.y1, col);
      lin.x1 = org.x1 + radius;
      lin.x2 = org.x2 - radius;
      lin.y1 = lin.y2 = org.y2;
      if(do_lineclip(&lin, &vwk->clip))
        VISUAL_HLINE(vwk, lin.x1, lin.x2, lin.y2, col);
      lin.x1 = lin.x2 = org.x1;
      lin.y1 = org.y1 + radius;
      lin.y2 = org.y2 - radius;
      if(do_lineclip(&lin, &vwk->clip))
        VISUAL_LINE(vwk, lin.x1, lin.y1, lin.x1, lin.y2, col);
      lin.x1 = lin.x2 = org.x2;
      lin.y1 = org.y1+ radius;
      lin.y2 = org.y2 - radius;
      if(do_lineclip(&lin, &vwk->clip))
        VISUAL_LINE(vwk, lin.x2, lin.y1, lin.x2, lin.y2, col);
      
      
      if(radius > 0) {
        /* do the rounded corners */
        ul_xc = org.x1 + radius;
        ul_yc = org.y1 + radius;
        ur_xc = org.x2 - radius;
        ur_yc = org.y1 + radius;
        ll_xc = org.x1 + radius;
        ll_yc = org.y2 - radius;
        lr_xc = org.x2 - radius;
        lr_yc = org.y2 - radius;
        
        x = 0;
        y = radius;
        p = 3-2*radius;
        
        while(x < y) {
          /* upper left corner */
          if(do_pointclip(ul_xc - x, ul_yc - y, &vwk->clip))
            VISUAL_PUT_PIXEL(vwk, ul_xc - x, ul_yc - y, col);
          if(do_pointclip(ul_xc - y, ul_yc - x, &vwk->clip))
            VISUAL_PUT_PIXEL(vwk, ul_xc - y, ul_yc - x, col);
          
          /* upper right corner */
          if(do_pointclip(ur_xc + x, ur_yc - y, &vwk->clip))
            VISUAL_PUT_PIXEL(vwk, ur_xc + x, ur_yc - y, col);
          if(do_pointclip(ur_xc + y, ur_yc - x, &vwk->clip))
            VISUAL_PUT_PIXEL(vwk, ur_xc + y, ur_yc - x, col);
          
          /* lower left corner */
          if(do_pointclip(ll_xc - x, ll_yc + y, &vwk->clip))
            VISUAL_PUT_PIXEL(vwk, ll_xc - x, ll_yc + y, col);
          if(do_pointclip(ll_xc - y, ll_yc + x, &vwk->clip))
            VISUAL_PUT_PIXEL(vwk, ll_xc - y, ll_yc + x, col);
          
          /* lower right corner */
          if(do_pointclip(lr_xc + x, lr_yc + y, &vwk->clip))
            VISUAL_PUT_PIXEL(vwk, lr_xc + x, lr_yc + y, col);
          if(do_pointclip(lr_xc + y, lr_yc + x, &vwk->clip))
            VISUAL_PUT_PIXEL(vwk, lr_xc + y, lr_yc + x, col);
          
          if(p < 0)
            p += 4*x + 6;
          else {
            p += 4*(x-y) + 10;
            y--;
          }
          x++;
        }
        if(x == y) {
          /* upper left corner */
          if(do_pointclip(ul_xc - x, ul_yc - y, &vwk->clip))
            VISUAL_PUT_PIXEL(vwk, ul_xc - x, ul_yc - y, col);
          if(do_pointclip(ul_xc - y, ul_yc - x, &vwk->clip))
            VISUAL_PUT_PIXEL(vwk, ul_xc - y, ul_yc - x, col);
          
          /* upper right corner */
          if(do_pointclip(ur_xc + x, ur_yc - y, &vwk->clip))
            VISUAL_PUT_PIXEL(vwk, ur_xc + x, ur_yc - y, col);
          if(do_pointclip(ur_xc + y, ur_yc - x, &vwk->clip))
            VISUAL_PUT_PIXEL(vwk, ur_xc + y, ur_yc - x, col);
          
          /* lower left corner */
          if(do_pointclip(ll_xc - x, ll_yc + y, &vwk->clip))
            VISUAL_PUT_PIXEL(vwk, ll_xc - x, ll_yc + y, col);
          if(do_pointclip(ll_xc - y, ll_yc + x, &vwk->clip))
            VISUAL_PUT_PIXEL(vwk, ll_xc - y, ll_yc + x, col);
          
          /* lower right corner */
          if(do_pointclip(lr_xc + x, lr_yc + y, &vwk->clip))
            VISUAL_PUT_PIXEL(vwk, lr_xc + x, lr_yc + y, col);
          if(do_pointclip(lr_xc + y, lr_yc + x, &vwk->clip))
            VISUAL_PUT_PIXEL(vwk, lr_xc + y, lr_yc + x, col);
        }
      }
    }
  }
  
  /* Unlock visual after operation */
  VISUAL_MUTEX(vwk, VISUAL_MUTEX_UNLOCK);

  vdipb->contrl[N_PTSOUT] = 0;
  vdipb->contrl[N_INTOUT] = 0;
}

void vdi_v_justified(VDI_Workstation *vwk)
{
  EDEBUG("v_justified: Call not implemented!\n");

  vdipb->contrl[N_PTSOUT] = 0;
  vdipb->contrl[N_INTOUT] = 0;
}




/***********************
 * Internal functions! *
 ***********************/

/* Used by v_circle. Puts pixels around the circle */
static inline void set_circle_pixels(VDI_Workstation * vwk,
				     int               x,
				     int               y,
				     int               xc,
				     int               yc,
				     RECT *            clip,
				     int               col)
{
  if(do_pointclip(xc + x, yc + y, clip))
    VISUAL_PUT_PIXEL(vwk, xc + x, yc + y, col);
  if(do_pointclip(xc - x, yc + y, clip))
    VISUAL_PUT_PIXEL(vwk, xc - x, yc + y, col);
  if(do_pointclip(xc + x, yc - y, clip))
    VISUAL_PUT_PIXEL(vwk, xc + x, yc - y, col);
  if(do_pointclip(xc - x, yc - y, clip))
    VISUAL_PUT_PIXEL(vwk, xc - x, yc - y, col);
  if(do_pointclip(xc + y, yc + x, clip))
    VISUAL_PUT_PIXEL(vwk, xc + y, yc + x, col);
  if(do_pointclip(xc - y, yc + x, clip))
    VISUAL_PUT_PIXEL(vwk, xc - y, yc + x, col);
  if(do_pointclip(xc + y, yc - x, clip))
    VISUAL_PUT_PIXEL(vwk, xc + y, yc - x, col);
  if(do_pointclip(xc - y, yc - x, clip))
    VISUAL_PUT_PIXEL(vwk, xc - y, yc - x, col);
}

/* Used by v_circle. Puts calculated
   x values in a list ordered by the
   y value. Used for filling */
static inline void list_circle_points(int x, int y, int radius, int *x_values)
{
  /* Only save one quarter of the circle, the
     rest is mirrored when drawing */

  x_values[-y+radius] = max(x_values[-y+radius], x);        /* x, -y */
  x_values[-x+radius] = max(x_values[-x+radius], y);        /* y, -x */
}

