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
	@LIBFWNT_CPPFLAGS@ \
	@LIBFMAPI_CPPFLAGS@ \
	@MEMWATCH_CPPFLAGS@

lib_LTLIBRARIES = libpff.la

libpff_la_SOURCES = \
	libpff.c \
	libpff_allocation_table.c libpff_allocation_table.h \
	libpff_array_type.c libpff_array_type.h \
	libpff_attached_file_io_handle.c libpff_attached_file_io_handle.h \
	libpff_attachment.c libpff_attachment.h \
	libpff_codepage.h \
	libpff_column_definition.c libpff_column_definition.h \
	libpff_data_array.c libpff_data_array.h \
	libpff_data_array_entry.c libpff_data_array_entry.h \
	libpff_data_block.c libpff_data_block.h \
	libpff_debug.c libpff_debug.h \
	libpff_definitions.h \
	libpff_encryption.c libpff_encryption.h \
	libpff_error.c libpff_error.h \
	libpff_extern.h \
	libpff_file.c libpff_file.h \
	libpff_folder.c libpff_folder.h \
	libpff_free_map.c libpff_free_map.h \
	libpff_index.c libpff_index.h \
	libpff_index_node.c libpff_index_node.h \
	libpff_index_tree.c libpff_index_tree.h \
	libpff_index_value.c libpff_index_value.h \
	libpff_io_handle.c libpff_io_handle.h \
	libpff_item.c libpff_item.h \
	libpff_item_descriptor.c libpff_item_descriptor.h \
	libpff_item_tree.c libpff_item_tree.h \
	libpff_item_values.c libpff_item_values.h \
	libpff_libbfio.h \
	libpff_libcerror.h \
	libpff_libclocale.h \
	libpff_libcnotify.h \
	libpff_libcstring.h \
	libpff_libfcache.h \
	libpff_libfdata.h \
	libpff_libfguid.h \
	libpff_libfmapi.h \
	libpff_libfvalue.h \
	libpff_libuna.h \
	libpff_list_type.c libpff_list_type.h \
	libpff_local_descriptor_node.c libpff_local_descriptor_node.h \
	libpff_local_descriptor_value.c libpff_local_descriptor_value.h \
	libpff_local_descriptors.c libpff_local_descriptors.h \
	libpff_local_descriptors_tree.c libpff_local_descriptors_tree.h \
	libpff_mapi.h \
	libpff_message.c libpff_message.h \
	libpff_multi_value.c libpff_multi_value.h \
	libpff_name_to_id_map.c libpff_name_to_id_map.h \
	libpff_notify.c libpff_notify.h \
	libpff_offset_list.c libpff_offset_list.h \
	libpff_record_entry.h \
	libpff_support.c libpff_support.h \
	libpff_table.c libpff_table.h \
	libpff_table_block.c libpff_table_block.h \
	libpff_tree_type.c libpff_tree_type.h \
	libpff_types.h \
	libpff_unused.h \
	libpff_value_type.c libpff_value_type.h \
	pff_allocation_table.h \
	pff_array.h \
	pff_block.h \
	pff_file_header.h \
	pff_free_map.h \
	pff_index_node.h \
	pff_local_descriptor_node.h \
	pff_table.h \
	pff_table_block.h \
	pff_value_data.h

libpff_la_LIBADD = \
	@LIBCSTRING_LIBADD@ \
	@LIBCERROR_LIBADD@ \
	@LIBCLOCALE_LIBADD@ \
	@LIBCNOTIFY_LIBADD@ \
	@LIBCSPLIT_LIBADD@ \
	@LIBUNA_LIBADD@ \
	@LIBCFILE_LIBADD@ \
	@LIBCPATH_LIBADD@ \
	@LIBBFIO_LIBADD@ \
	@LIBFCACHE_LIBADD@ \
	@LIBFDATA_LIBADD@ \
	@LIBFDATETIME_LIBADD@ \
	@LIBFGUID_LIBADD@ \
	@LIBFWNT_LIBADD@ \
	@LIBFMAPI_LIBADD@

libpff_la_LDFLAGS = -no-undefined -version-info 1:0:0

EXTRA_DIST = \
	libpff_definitions.h.in \
	libpff.rc \
	libpff.rc.in

MAINTAINERCLEANFILES = \
	Makefile.in

distclean: clean
	/bin/rm -f libpff_definitions.h
	/bin/rm -f libpff.rc
	/bin/rm -f Makefile

splint:
	@echo "Running splint on libpff ..." 
	-splint -preproc -redef $(DEFS) $(DEFAULT_INCLUDES) $(INCLUDES) $(AM_CPPFLAGS) $(libpff_la_SOURCES) 

