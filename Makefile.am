SUBDIRS = src vdi_dump fntread

INCLUDES = -Isrc

noinst_PROGRAMS = otestis
otestis_SOURCES = otestis.c
otestis_LDADD = src/.libs/libovdisis.a

bin_SCRIPTS = ovdisis-config

m4datadir = $(datadir)/aclocal
m4data_DATA = ovdisis.m4

EXTRA_DIST = FUNCS
