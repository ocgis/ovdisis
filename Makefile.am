SUBDIRS = src vdibind vdi_dump fntread

CFLAGS = -Ivdibind -Isrc

noinst_HEADERS = extern_ovdisis.h vdidefs.h
noinst_PROGRAMS = otestis
otestis_SOURCES = otestis.c
otestis_LDADD = src/.libs/libovdisis.a vdibind/libvdibind.a

EXTRA_DIST = FUNCS
