/*
 * Attachment functions
 *
 * Copyright (c) 2008-2012, Joachim Metz <joachim.metz@gmail.com>
 *
 * Refer to AUTHORS for acknowledgements.
 *
 * This software is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * This software is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU Lesser General Public License
 * along with this software.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <common.h>
#include <byte_stream.h>
#include <memory.h>
#include <types.h>

#include "libpff_attached_file_io_handle.h"
#include "libpff_attachment.h"
#include "libpff_debug.h"
#include "libpff_definitions.h"
#include "libpff_item.h"
#include "libpff_item_descriptor.h"
#include "libpff_item_tree.h"
#include "libpff_libcerror.h"
#include "libpff_libcnotify.h"
#include "libpff_libfcache.h"
#include "libpff_libfdata.h"
#include "libpff_libfmapi.h"
#include "libpff_local_descriptor_value.h"
#include "libpff_local_descriptors_tree.h"
#include "libpff_mapi.h"
#include "libpff_types.h"

/* Retrieves the attachment type
 * Returns 1 if successful or -1 on error
 */
int libpff_attachment_get_type(
     libpff_item_t *attachment,
     int *attachment_type,
     libcerror_error_t **error )
{
	uint8_t *value_data        = NULL;
	static char *function      = "libpff_attachment_get_type";
	uint32_t value_type        = 0;
	uint32_t attachment_method = 0;
	size_t value_data_size     = 0;

	if( attachment == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid attachment.",
		 function );

		return( -1 );
	}
	if( attachment_type == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid attachment type.",
		 function );

		return( -1 );
	}
	if( libpff_item_get_entry_value_32bit(
	     attachment,
	     0,
	     LIBPFF_ENTRY_TYPE_ATTACHMENT_METHOD,
	     &attachment_method,
	     0,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve attachment method.",
		 function );

		return( -1 );
	}
	if( ( attachment_method != LIBPFF_ATTACHMENT_METHOD_BY_VALUE )
	 && ( attachment_method != LIBPFF_ATTACHMENT_METHOD_BY_REFERENCE )
	 && ( attachment_method != LIBPFF_ATTACHMENT_METHOD_EMBEDDED_MESSAGE )
	 && ( attachment_method != LIBPFF_ATTACHMENT_METHOD_OLE ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
		 "%s: unsupported attachment method: 0x%08" PRIx32 ".",
		 function,
		 attachment_method );

		return( -1 );
	}
	if( attachment_method == LIBPFF_ATTACHMENT_METHOD_BY_REFERENCE )
	{
		*attachment_type = LIBPFF_ATTACHMENT_TYPE_REFERENCE;
	}
	else if( ( attachment_method == LIBPFF_ATTACHMENT_METHOD_BY_VALUE )
	      || ( attachment_method == LIBPFF_ATTACHMENT_METHOD_EMBEDDED_MESSAGE )
	      || ( attachment_method == LIBPFF_ATTACHMENT_METHOD_OLE ) )
	{
		if( libpff_item_get_entry_value(
		     attachment,
		     0,
		     LIBPFF_ENTRY_TYPE_ATTACHMENT_DATA_OBJECT,
		     &value_type,
		     &value_data,
		     &value_data_size,
		     LIBPFF_ENTRY_VALUE_FLAG_MATCH_ANY_VALUE_TYPE,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve entry value.",
			 function );

			return( -1 );
		}
		if( value_type == LIBPFF_VALUE_TYPE_BINARY_DATA )
		{
			*attachment_type = LIBPFF_ATTACHMENT_TYPE_DATA;
		}
		else if( value_type == LIBPFF_VALUE_TYPE_OBJECT )
		{
			if( attachment_method == LIBPFF_ATTACHMENT_METHOD_EMBEDDED_MESSAGE )
			{
				*attachment_type = LIBPFF_ATTACHMENT_TYPE_ITEM;
			}
			else if( attachment_method == LIBPFF_ATTACHMENT_METHOD_OLE )
			{
				*attachment_type = LIBPFF_ATTACHMENT_TYPE_DATA;
			}
			else
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
				 "%s: unsupported attachment method: 0x%08" PRIx32 " for object value type.",
				 function,
				 attachment_method );

				return( -1 );
			}
		}
		else
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
			 LIBCERROR_ARGUMENT_ERROR_UNSUPPORTED_VALUE,
			 "%s: unsupported entry value type: 0x%08" PRIx32 ".",
			 function,
			 value_type );

			return( -1 );
		}
	}
	return( 1 );
}

/* Retrieves the attachment data size
 * Returns 1 if successful, 0 if the item does not contain such value or -1 on error
 */
int libpff_attachment_get_data_size(
     libpff_item_t *attachment,
     size64_t *size,
     libcerror_error_t **error )
{
	libfcache_cache_t *value_data_cache        = NULL;
	libfdata_reference_t *value_data_reference = NULL;
	libpff_internal_item_t *internal_item      = NULL;
	uint8_t *value_data                        = NULL;
	static char *function                      = "libpff_attachment_get_data_size";
	size_t value_data_size                     = 0;
	uint32_t value_type                        = 0;
	uint32_t embedded_object_data_identifier   = 0;
	int result                                 = 0;

	if( attachment == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid attachment.",
		 function );

		return( -1 );
	}
	internal_item = (libpff_internal_item_t *) attachment;

	if( internal_item->internal_file == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid attachment - missing internal file.",
		 function );

		return( -1 );
	}
	if( internal_item->item_values == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid attachment - missing item values.",
		 function );

		return( -1 );
	}
	if( internal_item->item_tree_node == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid internal item - missing item tree node.",
		 function );

		return( -1 );
	}
	if( internal_item->item_tree_node->value == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid internal item - invalid item tree node - missing value.",
		 function );

		return( -1 );
	}
	if( size == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid size.",
		 function );

		return( -1 );
	}
	result = libpff_item_values_get_entry_value(
	          internal_item->item_values,
	          internal_item->internal_file->name_to_id_map_list,
	          internal_item->internal_file->io_handle,
	          internal_item->file_io_handle,
	          0,
	          LIBPFF_ENTRY_TYPE_ATTACHMENT_DATA_OBJECT,
	          &value_type,
	          &value_data_reference,
	          &value_data_cache,
	          LIBPFF_ENTRY_VALUE_FLAG_MATCH_ANY_VALUE_TYPE,
	          error );

	if( result == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve entry value.",
		 function );

		return( -1 );
	}
	else if( result == 0 )
	{
		return( 0 );
	}
	/* The OLE attachment method could refer to an OLE embedded object
	 */
	if( value_type == LIBPFF_VALUE_TYPE_OBJECT )
	{
		if( internal_item->embedded_object_data_reference == NULL )
		{
			if( libfdata_reference_get_data(
			     value_data_reference,
			     internal_item->file_io_handle,
			     value_data_cache,
			     &value_data,
			     &value_data_size,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
				 "%s: unable to retrieve embedded object data.",
				 function );

				return( -1 );
			}
			if( value_data == NULL )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
				 "%s: missing embedded object data.",
				 function );

				return( -1 );
			}
			/* The descriptor identifier is located in the local descriptors tree
			 */
			byte_stream_copy_to_uint32_little_endian(
			 value_data,
			 embedded_object_data_identifier );

			if( libpff_item_values_read_local_descriptor_data(
			     internal_item->item_values,
			     internal_item->internal_file->io_handle,
			     internal_item->file_io_handle,
			     embedded_object_data_identifier,
			     &( internal_item->embedded_object_data_reference ),
			     &( internal_item->embedded_object_data_cache ),
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
				 "%s: unable to read embedded object: %" PRIu32 ".",
				 function,
				 embedded_object_data_identifier );

				return( -1 );
			}
		}
		value_data_reference = internal_item->embedded_object_data_reference;
		value_data_cache     = internal_item->embedded_object_data_cache;
	}
	if( libfdata_reference_get_size(
	     value_data_reference,
	     size,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve value data size.",
		 function );

		return( -1 );
	}
	return( 1 );
}

/* Reads attachment data from the current offset into a buffer
 * Returns the number of bytes read or -1 on error
 */
ssize_t libpff_attachment_data_read_buffer(
         libpff_item_t *attachment,
         uint8_t *buffer,
         size_t buffer_size,
         libcerror_error_t **error )
{
	libfcache_cache_t *value_data_cache        = NULL;
	libfdata_reference_t *value_data_reference = NULL;
	libpff_internal_item_t *internal_item      = NULL;
	uint8_t *value_data                        = NULL;
	static char *function                      = "libpff_attachment_data_read_buffer";
	size_t value_data_size                     = 0;
	ssize_t read_count                         = 0;
	uint32_t embedded_object_data_identifier   = 0;
	uint32_t value_type                        = 0;
	int result                                 = 0;

	if( attachment == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid attachment.",
		 function );

		return( -1 );
	}
	internal_item = (libpff_internal_item_t *) attachment;

	if( internal_item->internal_file == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid attachment - missing internal file.",
		 function );

		return( -1 );
	}
	if( internal_item->item_values == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid attachment - missing item values.",
		 function );

		return( -1 );
	}
	if( internal_item->item_tree_node == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid internal item - missing item tree node.",
		 function );

		return( -1 );
	}
	if( internal_item->item_tree_node->value == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid internal item - invalid item tree node - missing value.",
		 function );

		return( -1 );
	}
	result = libpff_item_values_get_entry_value(
	          internal_item->item_values,
	          internal_item->internal_file->name_to_id_map_list,
	          internal_item->internal_file->io_handle,
	          internal_item->file_io_handle,
	          0,
	          LIBPFF_ENTRY_TYPE_ATTACHMENT_DATA_OBJECT,
	          &value_type,
	          &value_data_reference,
	          &value_data_cache,
	          LIBPFF_ENTRY_VALUE_FLAG_MATCH_ANY_VALUE_TYPE,
	          error );

	if( result == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve entry value.",
		 function );

		return( -1 );
	}
	else if( result == 0 )
	{
		return( 0 );
	}
	/* The OLE attachment method could refer to an OLE embedded object
	 */
	if( value_type == LIBPFF_VALUE_TYPE_OBJECT )
	{
		if( internal_item->embedded_object_data_reference == NULL )
		{
			if( libfdata_reference_get_data(
			     value_data_reference,
			     internal_item->file_io_handle,
			     value_data_cache,
			     &value_data,
			     &value_data_size,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
				 "%s: unable to retrieve embedded object data.",
				 function );

				return( -1 );
			}
			if( value_data == NULL )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
				 "%s: invalid entry embedded object data.",
				 function );

				return( -1 );
			}
			/* The descriptor identifier is located in the local descriptors tree
			 */
			byte_stream_copy_to_uint32_little_endian(
			 value_data,
			 embedded_object_data_identifier );

			if( libpff_item_values_read_local_descriptor_data(
			     internal_item->item_values,
			     internal_item->internal_file->io_handle,
			     internal_item->file_io_handle,
			     embedded_object_data_identifier,
			     &( internal_item->embedded_object_data_reference ),
			     &( internal_item->embedded_object_data_cache ),
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
				 "%s: unable to read embedded object: %" PRIu32 ".",
				 function,
				 embedded_object_data_identifier );

				return( -1 );
			}
		}
		value_data_reference = internal_item->embedded_object_data_reference;
		value_data_cache     = internal_item->embedded_object_data_cache;
	}
	read_count = libfdata_reference_read_buffer(
	              value_data_reference,
	              internal_item->file_io_handle,
	              value_data_cache,
	              buffer,
	              buffer_size,
	              error );

	if( read_count <= -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_READ_FAILED,
		 "%s: unable to read buffer from value data reference.",
		 function );

		return( -1 );
	}
	return( read_count );
}

/* Seeks a certain offset of the attachment data
 * Returns the offset if seek is successful or -1 on error
 */
off64_t libpff_attachment_data_seek_offset(
         libpff_item_t *attachment,
         off64_t offset,
         int whence,
         libcerror_error_t **error )
{
	libfcache_cache_t *value_data_cache        = NULL;
	libfdata_reference_t *value_data_reference = NULL;
	libpff_internal_item_t *internal_item      = NULL;
	uint8_t *value_data                        = NULL;
	static char *function                      = "libpff_attachment_data_seek_offset";
	size_t value_data_size                     = 0;
	uint32_t embedded_object_data_identifier   = 0;
	uint32_t value_type                        = 0;
	int result                                 = 0;

	if( attachment == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid attachment.",
		 function );

		return( -1 );
	}
	internal_item = (libpff_internal_item_t *) attachment;

	if( internal_item->internal_file == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid attachment - missing internal file.",
		 function );

		return( -1 );
	}
	if( internal_item->item_values == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid attachment - missing item values.",
		 function );

		return( -1 );
	}
	if( internal_item->item_tree_node == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid internal item - missing item tree node.",
		 function );

		return( -1 );
	}
	if( internal_item->item_tree_node->value == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid internal item - invalid item tree node - missing value.",
		 function );

		return( -1 );
	}
	result = libpff_item_values_get_entry_value(
	          internal_item->item_values,
	          internal_item->internal_file->name_to_id_map_list,
	          internal_item->internal_file->io_handle,
	          internal_item->file_io_handle,
	          0,
	          LIBPFF_ENTRY_TYPE_ATTACHMENT_DATA_OBJECT,
	          &value_type,
	          &value_data_reference,
	          &value_data_cache,
	          LIBPFF_ENTRY_VALUE_FLAG_MATCH_ANY_VALUE_TYPE,
	          error );

	if( result == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve entry value.",
		 function );

		return( -1 );
	}
	else if( result == 0 )
	{
		return( 0 );
	}
	/* The OLE attachment method could refer to an OLE embedded object
	 */
	if( value_type == LIBPFF_VALUE_TYPE_OBJECT )
	{
		if( internal_item->embedded_object_data_reference == NULL )
		{
			if( libfdata_reference_get_data(
			     value_data_reference,
			     internal_item->file_io_handle,
			     value_data_cache,
			     &value_data,
			     &value_data_size,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
				 "%s: unable to retrieve embedded object data.",
				 function );

				return( -1 );
			}
			if( value_data == NULL )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
				 "%s: invalid entry embedded object data.",
				 function );

				return( -1 );
			}
			/* The descriptor identifier is located in the local descriptors tree
			 */
			byte_stream_copy_to_uint32_little_endian(
			 value_data,
			 embedded_object_data_identifier );

			if( libpff_item_values_read_local_descriptor_data(
			     internal_item->item_values,
			     internal_item->internal_file->io_handle,
			     internal_item->file_io_handle,
			     embedded_object_data_identifier,
			     &( internal_item->embedded_object_data_reference ),
			     &( internal_item->embedded_object_data_cache ),
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
				 "%s: unable to read embedded object: %" PRIu32 ".",
				 function,
				 embedded_object_data_identifier );

				return( -1 );
			}
		}
		value_data_reference = internal_item->embedded_object_data_reference;
		value_data_cache     = internal_item->embedded_object_data_cache;
	}
	offset = libfdata_reference_seek_offset(
	          value_data_reference,
	          offset,
	          whence,
	          error );

	if( offset == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_SEEK_FAILED,
		 "%s: unable to seek offset in value data reference.",
		 function );

		return( -1 );
	}
	return( offset );
}

/* Retrieves the attachment data file IO handle
 * Returns 1 if successful, 0 if the item does not contain such value or -1 on error
 */
int libpff_attachment_get_data_file_io_handle(
     libpff_item_t *attachment,
     libbfio_handle_t **file_io_handle,
     libcerror_error_t **error )
{
	libpff_attached_file_io_handle_t *io_handle = NULL;
	static char *function                       = "libpff_attachment_get_data_file_io_handle";

	if( libpff_attached_file_io_handle_initialize(
	     &io_handle,
	     attachment,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create attached file IO handle.",
		 function );

		goto on_error;
	}
	if( libbfio_handle_initialize(
	     file_io_handle,
	     (intptr_t *) io_handle,
	     (int (*)(intptr_t **, libcerror_error_t **)) libpff_attached_file_io_handle_free,
	     (int (*)(intptr_t **, intptr_t *, libcerror_error_t **)) libpff_attached_file_io_handle_clone,
	     (int (*)(intptr_t *, int flags, libcerror_error_t **)) libpff_attached_file_io_handle_open,
	     (int (*)(intptr_t *, libcerror_error_t **)) libpff_attached_file_io_handle_close,
	     (ssize_t (*)(intptr_t *, uint8_t *, size_t, libcerror_error_t **)) libpff_attached_file_io_handle_read,
	     (ssize_t (*)(intptr_t *, const uint8_t *, size_t, libcerror_error_t **)) libpff_attached_file_io_handle_write,
	     (off64_t (*)(intptr_t *, off64_t, int, libcerror_error_t **)) libpff_attached_file_io_handle_seek_offset,
	     (int (*)(intptr_t *, libcerror_error_t **)) libpff_attached_file_io_handle_exists,
	     (int (*)(intptr_t *, libcerror_error_t **)) libpff_attached_file_io_handle_is_open,
	     (int (*)(intptr_t *, size64_t *, libcerror_error_t **)) libpff_attached_file_io_handle_get_size,
	     LIBBFIO_FLAG_IO_HANDLE_MANAGED | LIBBFIO_FLAG_IO_HANDLE_CLONE_BY_FUNCTION,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create file IO handle.",
		 function );

		goto on_error;
	}
	return( 1 );

on_error:
	if( io_handle != NULL )
	{
		libpff_attached_file_io_handle_free(
		 &io_handle,
		 NULL );
	}
	return( -1 );
}

/* Retrieves the attachment item
 * Returns 1 if successful, 0 if the item does not contain such value or -1 on error
 */
int libpff_attachment_get_item(
     libpff_item_t *attachment,
     libpff_item_t **attached_item,
     libcerror_error_t **error )
{
	libpff_internal_item_t *internal_item                   = NULL;
	libpff_local_descriptor_value_t *local_descriptor_value = NULL;
	libpff_tree_node_t *embedded_item_tree_node             = NULL;
	uint8_t *value_data                                     = NULL;
	static char *function                                   = "libpff_attachment_get_item";
	uint32_t value_type                                     = LIBPFF_VALUE_TYPE_OBJECT;
	uint32_t embedded_object_item_identifier                = 0;
	size_t value_data_size                                  = 0;
	int result                                              = 0;

	if( attachment == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid attachment.",
		 function );

		return( -1 );
	}
	internal_item = (libpff_internal_item_t *) attachment;

	if( internal_item->internal_file == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid attachment - missing internal file.",
		 function );

		return( -1 );
	}
	if( internal_item->item_values == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid attachment - missing item values.",
		 function );

		return( -1 );
	}
	if( internal_item->item_tree_node == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid internal item - missing item tree node.",
		 function );

		return( -1 );
	}
	if( internal_item->item_tree_node->value == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid internal item - invalid item tree node - missing value.",
		 function );

		return( -1 );
	}
	if( attached_item == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid attached item.",
		 function );

		return( -1 );
	}
	if( *attached_item != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid attached item already set.",
		 function );

		return( -1 );
	}
	if( libpff_item_get_entry_value(
	     attachment,
	     0,
	     LIBPFF_ENTRY_TYPE_ATTACHMENT_DATA_OBJECT,
	     &value_type,
	     &value_data,
	     &value_data_size,
	     0,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve entry value.",
		 function );

		return( -1 );
	}
	if( value_data == NULL )
	{
		return( 0 );
	}
	/* The descriptor identifier is located in the local descriptors tree
	 */
	byte_stream_copy_to_uint32_little_endian(
	 value_data,
	 embedded_object_item_identifier );

	/* TODO add support for recovered embedded items */

	result = libpff_item_tree_get_tree_node_by_identifier(
		  internal_item->internal_file->item_tree_root_node,
		  embedded_object_item_identifier,
		  &embedded_item_tree_node,
		  error );

	if( result == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve descriptor index value of attached item: %" PRIu32 ".",
		 function,
		 embedded_object_item_identifier );

		return( -1 );
	}
	if( result == 0 )
	{
		result = libpff_item_values_get_local_descriptors_value_by_identifier(
			  internal_item->item_values,
			  internal_item->file_io_handle,
			  embedded_object_item_identifier,
			  &local_descriptor_value,
			  error );

		if( result == -1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve local descriptor identifier: %" PRIu32 ".",
			 function,
			 embedded_object_item_identifier );

			return( -1 );
		}
/* TODO error tollerability flag an attachment as missing if result == 0 */
		else if( result == 0 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
			 "%s: missing local descriptor identifier: %" PRIu32 ".",
			 function,
			 embedded_object_item_identifier );

			return( -1 );
		}
		if( local_descriptor_value == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
			 "%s: invalid local descriptor value.",
			 function );

			return( -1 );
		}
#if defined( HAVE_DEBUG_OUTPUT )
		if( libcnotify_verbose != 0 )
		{
			libcnotify_printf(
			 "%s: local descriptor identifier: %" PRIu64 " ",
			 function,
			 local_descriptor_value->identifier );
			libpff_debug_print_node_identifier_type(
			 (uint8_t) ( local_descriptor_value->identifier & 0x0000001fUL ) );
			libcnotify_printf(
			 ", data: %" PRIu64 ", local descriptors: %" PRIu64 "\n",
			 local_descriptor_value->data_identifier,
			 local_descriptor_value->local_descriptors_identifier );
		}
#endif
		/* Make the embedded item the sub item of the attachment
		 */
		if( libpff_item_tree_append_identifier(
		     internal_item->item_tree_node,
		     embedded_object_item_identifier,
		     local_descriptor_value->data_identifier,
		     local_descriptor_value->local_descriptors_identifier,
		     internal_item->item_values->recovered,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_APPEND_FAILED,
			 "%s: unable to append attached item: %" PRIu32 " to attachment item tree node.",
			 function,
			 embedded_object_item_identifier );

			return( -1 );
		}
		/* Only a single embedded item per attachment should exists
		 */
		embedded_item_tree_node = internal_item->item_tree_node->first_sub_node;
	}
	if( embedded_item_tree_node == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid attached item tree node.",
		 function );

		return( -1 );
	}
	if( libpff_item_initialize(
	     attached_item,
	     internal_item->file_io_handle,
	     internal_item->internal_file,
	     embedded_item_tree_node,
	     (libpff_item_descriptor_t *) embedded_item_tree_node->value,
	     LIBPFF_ITEM_FLAGS_DEFAULT,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create attached item.",
		 function );

		return( -1 );
	}
	if( *attached_item == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid attached item.",
		 function );

		return( -1 );
	}
	return( 1 );
}

