#ifndef _EXTERN_OVDISIS_H_
#define _EXTERN_OVDISIS_H_

typedef struct
{
    short contrl[15];
    short intin[132];
    short ptsin[145];
    short intout[140];
    short ptsout[145];
} VDIPARBLK;

typedef struct
{
  short *contrl;
  short *intin;
  short *ptsin;
  short *intout;
  short *ptsout;
} VDIPB;

extern void vdi_call( VDIPB * );

#endif /* _EXTERN_OVDISIS_H_ */
                                        