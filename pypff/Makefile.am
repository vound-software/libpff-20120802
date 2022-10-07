if HAVE_PYTHON
AM_CFLAGS = \
	-I$(top_srcdir)/include \
	-I$(top_srcdir)/common \
	@LIBCSTRING_CPPFLAGS@ \
	@LIBCERROR_CPPFLAGS@ \
	@LIBCLOCALE_CPPFLAGS@ \
	@LIBUNA_CPPFLAGS@ \
	@LIBCFILE_CPPFLAGS@ \
	@LIBCPATH_CPPFLAGS@ \
	@LIBBFIO_CPPFLAGS@ \
	@MEMWATCH_CPPFLAGS@

pyexec_LTLIBRARIES = pypff.la

pypff_la_SOURCES = \
	pypff.c pypff.h \
	pypff_codepage.c pypff_codepage.h \
	pypff_datetime.c pypff_datetime.h \
	pypff_file.c pypff_file.h \
	pypff_file_object_io_handle.c pypff_file_object_io_handle.h \
	pypff_folder.c pypff_folder.h \
	pypff_item.c pypff_item.h \
	pypff_libbfio.h \
	pypff_libcerror.h \
	pypff_libclocale.h \
	pypff_libcstring.h \
	pypff_libpff.h \
	pypff_message.c pypff_message.h \
	pypff_python.h

pypff_la_LIBADD = \
	@LIBCSTRING_LIBADD@ \
	@LIBCERROR_LIBADD@ \
	../libpff/libpff.la
	@LIBCLOCALE_LIBADD@ \
	@LIBCFILE_LIBADD@ \
	@LIBCPATH_LIBADD@ \
	@LIBBFIO_LIBADD@

pypff_la_CPPFLAGS = $(PYTHON_CPPFLAGS)
pypff_la_LDFLAGS  = -module -avoid-version $(PYTHON_LDFLAGS)

endif

MAINTAINERCLEANFILES = \
	Makefile.in

distclean: clean
	/bin/rm -f Makefile

