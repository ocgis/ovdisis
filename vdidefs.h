#ifndef _VDIDEFS_H_
#define _VDIDEFS_H_

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


/****** Attribute definitions *****************************************/

#define IP_HOLLOW   0
#define IP_1PATT    1
#define IP_2PATT    2
#define IP_3PATT    3
#define IP_4PATT    4
#define IP_5PATT    5
#define IP_6PATT    6
#define IP_SOLID    7


/* gsx modes */

#define MD_REPLACE  1
#define MD_TRANS    2
#define MD_XOR      3
#define MD_ERASE    4


/* gsx styles */

#define FIS_HOLLOW  0
#define FIS_SOLID   1
#define FIS_PATTERN 2
#define FIS_HATCH   3
#define FIS_USER    4


/* bit blt rules */

#define ALL_WHITE   0
#define S_AND_D     1
#define S_AND_NOTD  2
#define S_ONLY      3
#define NOTS_AND_D  4
#define D_ONLY      5
#define S_XOR_D     6
#define S_OR_D      7
#define NOT_SORD    8
#define NOT_SXORD   9
#define NOT_D      10
#define S_OR_NOTD  11
#define NOT_S      12
#define NOTS_OR_D  13
#define NOT_SANDD  14
#define ALL_BLACK  15


/* linetypes */

#define SOLID       1
#define LONGDASH    2
#define DOT         3
#define DASHDOT     4
#define DASH        5
#define DASH2DOT    6
#define USERLINE    7

#define SQUARE      0
#define ARROWED     1
#define ROUND       2

#define M_DOT       1
#define M_PLUS      2
#define M_ASTERISK  3
#define M_SQUARE    4
#define M_CROSS     5
#define M_DIAMOND   6


/* text effect bitfields */

#define THICKENED   0x0001
#define LIGHT       0x0002
#define SKEWED      0x0004
#define UNDERLINED  0x0008
#define OUTLINED    0x0010
#define SHADOWED    0x0020



#define CLIP_OFF    0
#define CLIP_ON     1

#define COLOR_REQUESTED 0
#define COLOR_ACTUAL    1

/****** Raster definitions *********************************************/

typedef struct
{
        void            *fd_addr;
        int             fd_w;
        int             fd_h;
        int             fd_wdwidth;
        int             fd_stand;
        int             fd_nplanes;
        int             fd_r1;
        int             fd_r2;
        int             fd_r3;
} MFDB;

/***********************************************************/


#endif
