#ifndef _VARIOUS_H_
#define _VARIOUS_H_

extern inline int do_pointclip(int, int, RECT *);
extern inline int do_rectclip(RECT *, RECT *);
extern int do_lineclip(RECT *, RECT *);
extern inline void fix_rect(RECT *);

extern inline int gem2tos_color(int, int);
extern inline int tos2gem_color(int, int);
extern inline unsigned long get_color(VDI_Workstation *, int);

#endif /* _VARIOUS_H_ */
