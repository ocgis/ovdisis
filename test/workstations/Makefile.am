all:test_workstations

noinst_PROGRAMS = test_workstations
test_workstations_SOURCES = test_workstations.c

ovdisisdir=../../src



INCLUDES = -I$(ovdisisdir)

if USE_LIBTOOL
  test_workstations_LDADD = $(ovdisisdir)/libovdisis.la
else
  test_workstations_LDADD = $(ovdisisdir)/libovdisis.a
endif

bin_SCRIPTS = ../../ovdisis-config

m4datadir = $(datadir)/aclocal

EXTRA_DIST = FUNCS OVDISIS-VERSION
