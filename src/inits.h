#ifndef _INITS_H_
#define _INITS_H_

void init_cmap(VDI_Workstation *);
void init_marker(VDI_Workstation *);
void init_line(VDI_Workstation *);
void init_fill(VDI_Workstation *);
void init_text(VDI_Workstation *);

void copy_cmap(VDI_Workstation *, VDI_Workstation *);
void copy_marker(VDI_Workstation *, VDI_Workstation *);
void copy_line(VDI_Workstation *, VDI_Workstation *);
void copy_fill(VDI_Workstation *, VDI_Workstation *);
void copy_text(VDI_Workstation *, VDI_Workstation *);

#endif /* _INITS_H_ */
