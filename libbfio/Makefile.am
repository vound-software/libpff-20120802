if HAVE_LOCAL_LIBBFIO
AM_CPPFLAGS = \
	-I$(top_srcdir)/include \
	-I$(top_srcdir)/common \
	@LIBCSTRING_CPPFLAGS@ \
	@LIBCERROR_CPPFLAGS@ \
	@LIBCLOCALE_CPPFLAGS@ \
	@LIBUNA_CPPFLAGS@ \
	@LIBCFILE_CPPFLAGS@ \
	@LIBCPATH_CPPFLAGS@ \
	@MEMWATCH_CPPFLAGS@

noinst_LTLIBRARIES = libbfio.la

libbfio_la_SOURCES = \
	libbfio_codepage.h \
	libbfio_definitions.h \
	libbfio_error.c libbfio_error.h \
	libbfio_extern.h \
	libbfio_file.c libbfio_file.h \
	libbfio_file_pool.c libbfio_file_pool.h \
	libbfio_file_range.c libbfio_file_range.h \
	libbfio_handle.c libbfio_handle.h \
	libbfio_legacy.c libbfio_legacy.h \
	libbfio_libcerror.h \
	libbfio_libcfile.h \
	libbfio_libclocale.h \
	libbfio_libcpath.h \
	libbfio_libcstring.h \
	libbfio_libuna.h \
	libbfio_list_type.c libbfio_list_type.h \
	libbfio_memory_range.c libbfio_memory_range.h \
	libbfio_offset_list.c libbfio_offset_list.h \
	libbfio_pool.c libbfio_pool.h \
	libbfio_support.c libbfio_support.h \
	libbfio_types.h \
	libbfio_unused.h
endif

MAINTAINERCLEANFILES = \
	Makefile.in

distclean: clean
	/bin/rm -f Makefile

splint:
	@echo "Running splint on libbfio ..." 
	-splint -preproc -redef $(DEFS) $(DEFAULT_INCLUDES) $(INCLUDES) $(AM_CPPFLAGS) $(libbfio_la_SOURCES) 

