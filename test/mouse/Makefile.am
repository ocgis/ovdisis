all:test_mouse

noinst_PROGRAMS = test_mouse
test_mouse_SOURCES = test_mouse.c

ovdisisdir=../../src


INCLUDES = -I$(ovdisisdir)

if USE_LIBTOOL
  test_mouse_LDADD = $(ovdisisdir)/libovdisis.la
else
  test_mouse_LDADD = $(ovdisisdir)/libovdisis.a
endif

bin_SCRIPTS = ../../ovdisis-config

m4datadir = $(datadir)/aclocal

EXTRA_DIST = FUNCS OVDISIS-VERSION
