SUBDIRS = src vdibind vdi_dump

CFLAGS = -Ivdibind -Isrc

noinst_PROGRAMS = otestis
otestis_SOURCES = otestis.c
otestis_LDADD = src/.libs/libovdisis.a vdibind/libvdibind.a
