SUBDIRS = src vdibind vdi_dump fntread

CFLAGS = -Ivdibind -Isrc

noinst_HEADERS = extern_ovdisis.h vdidefs.h
noinst_PROGRAMS = otestis
otestis_SOURCES = otestis.c
otestis_LDADD = vdibind/libvdibind.a src/.libs/libovdisis.a

bin_SCRIPTS = ovdisis-config

m4datadir = $(datadir)/aclocal
m4data_DATA = ovdisis.m4

EXTRA_DIST = FUNCS
