AM_CPPFLAGS = \
	-I$(top_srcdir)/include \
	-I$(top_srcdir)/common \
	-I$(top_srcdir)/libcstring \
	-I$(top_srcdir)/liberror \
	-I$(top_srcdir)/libsystem \
	@MEMWATCH_CPPFLAGS@

TESTS = 

check_SCRIPTS =

EXTRA_DIST = \
	$(check_SCRIPTS)

check_PROGRAMS = \
	pff1

pff1_SOURCES = \
        pff1.c

pff1_LDADD = \
        ../libpff/libpff.la

MAINTAINERCLEANFILES = \
	Makefile.in

distclean: clean
	/bin/rm -f Makefile

