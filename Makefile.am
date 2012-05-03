SUBDIRS = src vdi_dump fntread test

INCLUDES = -Isrc

noinst_PROGRAMS = otestis
otestis_SOURCES = otestis.c

if USE_LIBTOOL
  otestis_LDADD = src/libovdisis.la
else
  otestis_LDADD = src/libovdisis.a
endif

bin_SCRIPTS = ovdisis-config

ACLOCAL_AMFLAGS = -I m4
m4datadir = $(datadir)/aclocal
m4data_DATA = ovdisis.m4

EXTRA_DIST = FUNCS Makefile.purec OVDISIS-VERSION
