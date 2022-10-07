AM_CPPFLAGS = \
	-I$(top_srcdir)/include \
	-I$(top_srcdir)/common \
	@LIBCSTRING_CPPFLAGS@ \
	@LIBCERROR_CPPFLAGS@ \
	@LIBCLOCALE_CPPFLAGS@ \
	@LIBCNOTIFY_CPPFLAGS@ \
	@LIBCSPLIT_CPPFLAGS@ \
	@LIBUNA_CPPFLAGS@ \
	@LIBCFILE_CPPFLAGS@ \
	@LIBCPATH_CPPFLAGS@ \
	@LIBBFIO_CPPFLAGS@ \
	@LIBFCACHE_CPPFLAGS@ \
	@LIBFDATA_CPPFLAGS@ \
	@LIBFDATETIME_CPPFLAGS@ \
	@LIBFGUID_CPPFLAGS@ \
	@LIBFVALUE_CPPFLAGS@ \
	@LIBFWNT_CPPFLAGS@ \
	@LIBFMAPI_CPPFLAGS@ \
	@LIBCSYSTEM_CPPFLAGS@ \
	@MEMWATCH_CPPFLAGS@ \
	@LIBPFF_DLL_IMPORT@

AM_LDFLAGS = @STATIC_LDFLAGS@

bin_PROGRAMS = \
	pffexport \
	pffinfo

pffexport_SOURCES = \
	export_handle.c export_handle.h \
	item_file.c item_file.h \
	log_handle.c log_handle.h \
	pffexport.c \
	pffinput.c pffinput.h \
	pfftools_libcerror.h \
	pfftools_libclocale.h \
	pfftools_libcnotify.h \
	pfftools_libcfile.h \
	pfftools_libcpath.h \
	pfftools_libcsplit.h \
	pfftools_libcstring.h \
	pfftools_libcsystem.h \
	pfftools_libfdatetime.h \
	pfftools_libfguid.h \
	pfftools_libfvalue.h \
	pfftools_libpff.h \
	pffoutput.c pffoutput.h

pffexport_LDADD = \
	@LIBCSYSTEM_LIBADD@ \
	@LIBFMAPI_LIBADD@ \
	@LIBFVALUE_LIBADD@ \
	@LIBFWNT_LIBADD@ \
	@LIBFGUID_LIBADD@ \
	@LIBFDATETIME_LIBADD@ \
	@LIBCPATH_LIBADD@ \
	@LIBCFILE_LIBADD@ \
	@LIBUNA_LIBADD@ \
	../libpff/libpff.la \
	@LIBCSPLIT_LIBADD@ \
	@LIBCNOTIFY_LIBADD@ \
	@LIBCLOCALE_LIBADD@ \
	@LIBCERROR_LIBADD@ \
	@LIBCSTRING_LIBADD@ \
	@LIBINTL@ \
	@MEMWATCH_LIBADD@

pffinfo_SOURCES = \
	info_handle.c info_handle.h \
	pffinfo.c \
	pffinput.c pffinput.h \
	pfftools_libcerror.h \
	pfftools_libclocale.h \
	pfftools_libcnotify.h \
	pfftools_libcstring.h \
	pfftools_libcsystem.h \
	pfftools_libpff.h \
	pffoutput.c pffoutput.h

pffinfo_LDADD = \
	@LIBCSYSTEM_LIBADD@ \
	@LIBCPATH_LIBADD@ \
	@LIBCFILE_LIBADD@ \
	@LIBUNA_LIBADD@ \
	../libpff/libpff.la \
	@LIBCSPLIT_LIBADD@ \
	@LIBCNOTIFY_LIBADD@ \
	@LIBCLOCALE_LIBADD@ \
	@LIBCERROR_LIBADD@ \
	@LIBCSTRING_LIBADD@ \
	@LIBINTL@ \
	@MEMWATCH_LIBADD@

MAINTAINERCLEANFILES = \
	Makefile.in

distclean: clean
	/bin/rm -f Makefile

splint:
	@echo "Running splint on pffexport ..." 
	-splint -preproc -redef $(DEFS) $(DEFAULT_INCLUDES) $(INCLUDES) $(AM_CPPFLAGS) $(pffexport_SOURCES) 
	@echo "Running splint on pffinfo ..." 
	-splint -preproc -redef $(DEFS) $(DEFAULT_INCLUDES) $(INCLUDES) $(AM_CPPFLAGS) $(pffinfo_SOURCES) 

