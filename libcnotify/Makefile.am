AM_CPPFLAGS = \
	-I$(top_srcdir)/include \
	-I$(top_srcdir)/common \
	@LIBCSTRING_CPPFLAGS@ \
	@LIBCERROR_CPPFLAGS@ \
	@MEMWATCH_CPPFLAGS@

noinst_LTLIBRARIES = libcnotify.la

libcnotify_la_SOURCES = \
	libcnotify_definitions.h \
	libcnotify_extern.h \
	libcnotify_libcerror.h \
	libcnotify_libcstring.h \
	libcnotify_print.c libcnotify_print.h \
	libcnotify_stream.c libcnotify_stream.h \
	libcnotify_unused.h \
	libcnotify_verbose.c libcnotify_verbose.h

MAINTAINERCLEANFILES = \
	Makefile.in

distclean: clean
	/bin/rm -f Makefile

splint:
	@echo "Running splint on libcnotify ..." 
	-splint -preproc -redef $(DEFS) $(DEFAULT_INCLUDES) $(INCLUDES) $(AM_CPPFLAGS) $(libcnotify_la_SOURCES) 

