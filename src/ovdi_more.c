#include "ovdisis.h"

extern VDIWK wk[MAX_HANDLES];    /* declared in vdi_control.c */
extern int wk_open[MAX_HANDLES]; /* ----------- " ----------- */ 


int ovdi_getchar(int handle)
{
  if(wk_open[handle-1])
    return FBgetchar(wk[handle-1].physical->fb);

  return 0;
}
