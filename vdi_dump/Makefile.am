INCLUDES = -I../src/

if USE_LIBTOOL
  vdi_dump_LDADD = ../src/.libs/libovdisis.a
else
  vdi_dump_LDADD = ../src/libovdisis.a
endif

noinst_PROGRAMS = vdi_dump

vdi_dump_SOURCES = vdi_dump.c

EXTRA_DIST = 16_dump 256_dump outputs tc_dump
