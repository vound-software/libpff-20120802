/*
 * Message functions
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
#include <memory.h>
#include <types.h>

#include "libpff_debug.h"
#include "libpff_definitions.h"
#include "libpff_item.h"
#include "libpff_item_descriptor.h"
#include "libpff_item_tree.h"
#include "libpff_item_values.h"
#include "libpff_libcerror.h"
#include "libpff_libcnotify.h"
#include "libpff_libfcache.h"
#include "libpff_libfdata.h"
#include "libpff_libfmapi.h"
#include "libpff_mapi.h"
#include "libpff_message.h"
#include "libpff_item.h"
#include "libpff_libuna.h"
#include "libpff_value_type.h"

#define LIBPFF_MESSAGE_SUB_ITEM_ATTACHMENTS	0
#define LIBPFF_MESSAGE_SUB_ITEM_RECIPIENTS	1

/* Retrieves the UTF-8 string size of a specific entry from the referenced message item
 * Length includes the end of string character
 * Returns 1 if successful, 0 if the message does not contain such value or -1 on error
 */
int libpff_message_get_entry_value_utf8_string_size(
     libpff_item_t *message,
     uint32_t entry_type,
     size_t *utf8_string_size,
     libcerror_error_t **error )
{
	libpff_internal_item_t *internal_item = NULL;
	uint8_t *value_data                   = NULL;
	static char *function                 = "libpff_message_get_entry_value_utf8_string_size";
	size_t value_data_size                = 0;
	uint32_t ascii_codepage               = 0;
	uint32_t value_type                   = 0;
	uint8_t is_ascii_string               = 0;
	int result                            = 0;

	if( message == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid message.",
		 function );

		return( -1 );
	}
	internal_item = (libpff_internal_item_t *) message;

	if( internal_item->internal_file == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid item - missing internal file.",
		 function );

		return( -1 );
	}
	if( internal_item->internal_file->io_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid internal item - invalid internal file - missing IO handle.",
		 function );

		return( -1 );
	}
	result = libpff_item_get_entry_value(
		  message,
		  0,
		  entry_type,
		  &value_type,
		  &value_data,
		  &value_data_size,
		  1,
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
	if( ( value_type == LIBPFF_VALUE_TYPE_STRING_ASCII )
	 || ( value_type == LIBPFF_VALUE_TYPE_STRING_UNICODE ) )
	{
		if( value_type == LIBPFF_VALUE_TYPE_STRING_ASCII )
		{
			result = libpff_item_get_entry_value_32bit(
				  message,
				  0,
				  LIBPFF_ENTRY_TYPE_MESSAGE_CODEPAGE,
				  &ascii_codepage,
				  0,
				  error );

			if( result == -1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
				 "%s: unable to retrieve the message codepage.",
				 function );

				return( -1 );
			}
			else if( result == 0 )
			{
				ascii_codepage = (uint32_t) internal_item->internal_file->io_handle->ascii_codepage;
			}
			is_ascii_string = 1;
		}
		if( libpff_value_type_get_utf8_string_size(
		     value_data,
		     value_data_size,
		     is_ascii_string,
		     ascii_codepage,
		     utf8_string_size,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to determine UTF-8 string size.",
			 function );

			return( -1 );
		}
	}
	else
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_UNSUPPORTED_VALUE,
		 "%s: unsupported value type: 0x%04" PRIx32 ".",
		 function,
		 value_type );

		return( -1 );
	}
	return( 1 );
}

/* Retrieves the UTF-8 string value of a specific entry from the referenced message item
 * The function uses a codepage if necessary, it uses the codepage set for the library
 * Returns 1 if successful, 0 if the message does not contain such value or -1 on error
 */
int libpff_message_get_entry_value_utf8_string(
     libpff_item_t *message,
     uint32_t entry_type,
     uint8_t *utf8_string,
     size_t utf8_string_size,
     libcerror_error_t **error )
{
	libpff_internal_item_t *internal_item = NULL;
	uint8_t *value_data                   = NULL;
	static char *function                 = "libpff_message_get_entry_value_utf8_string";
	size_t value_data_size                = 0;
	uint32_t ascii_codepage               = 0;
	uint32_t value_type                   = 0;
	uint8_t is_ascii_string               = 0;
	int result                            = 0;

	if( message == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid message.",
		 function );

		return( -1 );
	}
	internal_item = (libpff_internal_item_t *) message;

	if( internal_item->internal_file == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid item - missing internal file.",
		 function );

		return( -1 );
	}
	if( internal_item->internal_file->io_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid internal item - invalid internal file - missing IO handle.",
		 function );

		return( -1 );
	}
	result = libpff_item_get_entry_value(
		  message,
		  0,
		  entry_type,
		  &value_type,
		  &value_data,
		  &value_data_size,
		  1,
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
	if( ( value_type == LIBPFF_VALUE_TYPE_STRING_ASCII )
	 || ( value_type == LIBPFF_VALUE_TYPE_STRING_UNICODE ) )
	{
		if( value_type == LIBPFF_VALUE_TYPE_STRING_ASCII )
		{
			result = libpff_item_get_entry_value_32bit(
				  message,
				  0,
				  LIBPFF_ENTRY_TYPE_MESSAGE_CODEPAGE,
				  &ascii_codepage,
				  0,
				  error );

			if( result == -1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
				 "%s: unable to retrieve the message codepage.",
				 function );

				return( -1 );
			}
			else if( result == 0 )
			{
				ascii_codepage = (uint32_t) internal_item->internal_file->io_handle->ascii_codepage;
			}
			is_ascii_string = 1;
		}
		if( libpff_value_type_copy_to_utf8_string(
		     value_data,
		     value_data_size,
		     is_ascii_string,
		     ascii_codepage,
		     utf8_string,
		     utf8_string_size,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_CONVERSION,
			 LIBCERROR_CONVERSION_ERROR_GENERIC,
			 "%s: unable to set UTF-8 string.",
			 function );

			return( -1 );
		}
	}
	else
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_UNSUPPORTED_VALUE,
		 "%s: unsupported value type: 0x%04" PRIx32 ".",
		 function,
		 value_type );

		return( -1 );
	}
	return( 1 );
}

/* Retrieves the UTF-16 string size of a specific entry from the referenced message item
 * Length includes the end of string character
 * Returns 1 if successful, 0 if the message does not contain such value or -1 on error
 */
int libpff_message_get_entry_value_utf16_string_size(
     libpff_item_t *message,
     uint32_t entry_type,
     size_t *utf16_string_size,
     libcerror_error_t **error )
{
	libpff_internal_item_t *internal_item = NULL;
	uint8_t *value_data                   = NULL;
	static char *function                 = "libpff_message_get_entry_value_utf16_string_size";
	size_t value_data_size                = 0;
	uint32_t ascii_codepage               = 0;
	uint32_t value_type                   = 0;
	uint8_t is_ascii_string               = 0;
	int result                            = 0;

	if( message == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid message.",
		 function );

		return( -1 );
	}
	internal_item = (libpff_internal_item_t *) message;

	if( internal_item->internal_file == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid item - missing internal file.",
		 function );

		return( -1 );
	}
	if( internal_item->internal_file->io_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid internal item - invalid internal file - missing IO handle.",
		 function );

		return( -1 );
	}
	result = libpff_item_get_entry_value(
		  message,
		  0,
		  entry_type,
		  &value_type,
		  &value_data,
		  &value_data_size,
		  1,
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
	if( ( value_type == LIBPFF_VALUE_TYPE_STRING_ASCII )
	 || ( value_type == LIBPFF_VALUE_TYPE_STRING_UNICODE ) )
	{
		if( value_type == LIBPFF_VALUE_TYPE_STRING_ASCII )
		{
			result = libpff_item_get_entry_value_32bit(
				  message,
				  0,
				  LIBPFF_ENTRY_TYPE_MESSAGE_CODEPAGE,
				  &ascii_codepage,
				  0,
				  error );

			if( result == -1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
				 "%s: unable to retrieve the message codepage.",
				 function );

				return( -1 );
			}
			else if( result == 0 )
			{
				ascii_codepage = (uint32_t) internal_item->internal_file->io_handle->ascii_codepage;
			}
			is_ascii_string = 1;
		}
		if( libpff_value_type_get_utf16_string_size(
		     value_data,
		     value_data_size,
		     is_ascii_string,
		     ascii_codepage,
		     utf16_string_size,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to determine UTF-16 string size.",
			 function );

			return( -1 );
		}
	}
	else
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_UNSUPPORTED_VALUE,
		 "%s: unsupported value type: 0x%04" PRIx32 ".",
		 function,
		 value_type );

		return( -1 );
	}
	return( 1 );
}

/* Retrieves the UTF-16 string value of a specific entry from the referenced message item
 * The function uses a codepage if necessary, it uses the codepage set for the library
 * Returns 1 if successful, 0 if the message does not contain such value or -1 on error
 */
int libpff_message_get_entry_value_utf16_string(
     libpff_item_t *message,
     uint32_t entry_type,
     uint16_t *utf16_string,
     size_t utf16_string_size,
     libcerror_error_t **error )
{
	libpff_internal_item_t *internal_item = NULL;
	uint8_t *value_data                   = NULL;
	static char *function                 = "libpff_message_get_entry_value_utf16_string";
	size_t value_data_size                = 0;
	uint32_t ascii_codepage               = 0;
	uint32_t value_type                   = 0;
	uint8_t is_ascii_string               = 0;
	int result                            = 0;

	if( message == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid message.",
		 function );

		return( -1 );
	}
	internal_item = (libpff_internal_item_t *) message;

	if( internal_item->internal_file == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid item - missing internal file.",
		 function );

		return( -1 );
	}
	if( internal_item->internal_file->io_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid internal item - invalid internal file - missing IO handle.",
		 function );

		return( -1 );
	}
	result = libpff_item_get_entry_value(
		  message,
		  0,
		  entry_type,
		  &value_type,
		  &value_data,
		  &value_data_size,
		  1,
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
	if( ( value_type == LIBPFF_VALUE_TYPE_STRING_ASCII )
	 || ( value_type == LIBPFF_VALUE_TYPE_STRING_UNICODE ) )
	{
		if( value_type == LIBPFF_VALUE_TYPE_STRING_ASCII )
		{
			result = libpff_item_get_entry_value_32bit(
				  message,
				  0,
				  LIBPFF_ENTRY_TYPE_MESSAGE_CODEPAGE,
				  &ascii_codepage,
				  0,
				  error );

			if( result == -1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
				 "%s: unable to retrieve the message codepage.",
				 function );

				return( -1 );
			}
			else if( result == 0 )
			{
				ascii_codepage = (uint32_t) internal_item->internal_file->io_handle->ascii_codepage;
			}
			is_ascii_string = 1;
		}
		if( libpff_value_type_copy_to_utf16_string(
		     value_data,
		     value_data_size,
		     is_ascii_string,
		     ascii_codepage,
		     utf16_string,
		     utf16_string_size,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_CONVERSION,
			 LIBCERROR_CONVERSION_ERROR_GENERIC,
			 "%s: unable to set UTF-16 string.",
			 function );

			return( -1 );
		}
	}
	else
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_UNSUPPORTED_VALUE,
		 "%s: unsupported value type: 0x%04" PRIx32 ".",
		 function,
		 value_type );

		return( -1 );
	}
	return( 1 );
}

/* Determine if the message item has attachments
 * Returns 1 if successful or -1 on error
 */
int libpff_message_determine_attachments(
     libpff_internal_item_t *internal_item,
     libcerror_error_t **error )
{
	libfcache_cache_t *value_data_cache                     = NULL;
	libfdata_reference_t *value_data_reference              = NULL;
	libpff_item_descriptor_t *item_descriptor               = NULL;
	libpff_item_descriptor_t *attachments_item_descriptor   = NULL;
	libpff_local_descriptor_value_t *local_descriptor_value = NULL;
	uint8_t *value_data                                     = NULL;
	static char *function                                   = "libpff_message_determine_attachments";
	size_t value_data_size                                  = 0;
	uint32_t attachment_descriptor_identifier               = 0;
	uint32_t value_type                                     = LIBPFF_VALUE_TYPE_INTEGER_32BIT_SIGNED;
	int attachment_iterator                                 = 0;
	int result                                              = 0;

	if( internal_item == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid internal item.",
		 function );

		return( -1 );
	}
	if( internal_item->internal_file == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid internal item - missing internal file.",
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
	item_descriptor = (libpff_item_descriptor_t *) internal_item->item_tree_node->value;

	if( internal_item->item_values == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid internal item - missing item values.",
		 function );

		return( -1 );
	}
	if( internal_item->sub_item_tree_node[ LIBPFF_MESSAGE_SUB_ITEM_ATTACHMENTS ] != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: attachments sub item tree node already set.",
		 function );

		return( -1 );
	}
	/* Make sure the item values have been read
	 */
/* TODO is this still needed ? */
	if( internal_item->item_values->table == NULL )
	{
#if defined( HAVE_DEBUG_OUTPUT )
		if( libcnotify_verbose != 0 )
		{
			libcnotify_printf(
			 "%s: reading item values of descriptor: %" PRIu32 "\n",
			 function,
			 item_descriptor->descriptor_identifier );
		}
#endif

		if( libpff_item_values_read(
		     internal_item->item_values,
		     internal_item->internal_file->name_to_id_map_list,
		     internal_item->internal_file->io_handle,
		     internal_item->file_io_handle,
		     LIBPFF_DEBUG_ITEM_TYPE_DEFAULT,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_IO,
			 LIBCERROR_IO_ERROR_READ_FAILED,
			 "%s: unable to read item values.",
			 function );

			return( -1 );
		}
		if( internal_item->item_values->table == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
			 "%s: invalid item values - missing table.",
			 function );

			return( -1 );
		}
	}
	/* Determine if the item has attachments
	 */
	result = libpff_item_values_get_local_descriptors_value_by_identifier(
		  internal_item->item_values,
		  internal_item->file_io_handle,
		  (uint32_t) LIBPFF_LOCAL_DESCRIPTOR_IDENTIFIER_ATTACHMENTS,
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
		 (uint32_t) LIBPFF_LOCAL_DESCRIPTOR_IDENTIFIER_ATTACHMENTS );

		return( -1 );
	}
	else if( result != 0 )
	{
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
		if( libpff_item_values_initialize(
		     &( internal_item->sub_item_values[ LIBPFF_MESSAGE_SUB_ITEM_ATTACHMENTS ] ),
		     (uint32_t) LIBPFF_LOCAL_DESCRIPTOR_IDENTIFIER_ATTACHMENTS,
		     local_descriptor_value->data_identifier,
		     local_descriptor_value->local_descriptors_identifier ,
		     item_descriptor->recovered,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
			 "%s: unable to create attachments item values.",
			 function );

			return( -1 );
		}
		/* Cache the attachments items values for successive usage
		 */
		if( libpff_item_values_read(
		     internal_item->sub_item_values[ LIBPFF_MESSAGE_SUB_ITEM_ATTACHMENTS ],
		     internal_item->internal_file->name_to_id_map_list,
		     internal_item->internal_file->io_handle,
		     internal_item->file_io_handle,
		     LIBPFF_DEBUG_ITEM_TYPE_DEFAULT,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_IO,
			 LIBCERROR_IO_ERROR_READ_FAILED,
			 "%s: unable to read attachments local descriptor recipiens identifier: %" PRIu32 ".",
			 function,
			 LIBPFF_LOCAL_DESCRIPTOR_IDENTIFIER_ATTACHMENTS );

			return( -1 );
		}
		if( internal_item->sub_item_values[ LIBPFF_MESSAGE_SUB_ITEM_ATTACHMENTS ]->table == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
			 "%s: invalid attachments item values - missing table.",
			 function );

			return( -1 );
		}
		/* Create an attachments item descriptor
		 */
		if( libpff_item_descriptor_initialize(
		     &attachments_item_descriptor,
		     (uint32_t) LIBPFF_LOCAL_DESCRIPTOR_IDENTIFIER_ATTACHMENTS,
		     local_descriptor_value->data_identifier,
		     local_descriptor_value->local_descriptors_identifier ,
		     item_descriptor->recovered,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
			 "%s: unable to create attachments item descriptor.",
			 function );

			return( -1 );
		}
		if( libpff_tree_node_initialize(
		     &( internal_item->sub_item_tree_node[ LIBPFF_MESSAGE_SUB_ITEM_ATTACHMENTS ] ),
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
			 "%s: unable to create attachments sub item tree node.",
			 function );

			memory_free(
			 attachments_item_descriptor );

			return( -1 );
		}
		if( libpff_tree_node_set_value(
		     internal_item->sub_item_tree_node[ LIBPFF_MESSAGE_SUB_ITEM_ATTACHMENTS ],
		     (intptr_t *) attachments_item_descriptor,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
			 "%s: unable to set attachments item descriptor in attachments sub item tree node.",
			 function );

			libpff_tree_node_free(
			 &( internal_item->sub_item_tree_node[ LIBPFF_MESSAGE_SUB_ITEM_ATTACHMENTS ] ),
			 NULL,
			 NULL );
			memory_free(
			 attachments_item_descriptor );

			return( -1 );
		}
		for( attachment_iterator = 0;
		     attachment_iterator < internal_item->sub_item_values[ LIBPFF_MESSAGE_SUB_ITEM_ATTACHMENTS ]->table->number_of_sets;
		     attachment_iterator++ )
		{
			if( libpff_item_values_get_entry_value(
			     internal_item->sub_item_values[ LIBPFF_MESSAGE_SUB_ITEM_ATTACHMENTS ],
			     internal_item->internal_file->name_to_id_map_list,
			     internal_item->internal_file->io_handle,
			     internal_item->file_io_handle,
			     attachment_iterator,
			     LIBPFF_ENTRY_TYPE_SUB_ITEM_IDENTIFIER,
			     &value_type,
			     &value_data_reference,
			     &value_data_cache,
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
				 "%s: unable to retrieve value data.",
				 function );

				return( -1 );
			}
			if( libpff_value_type_copy_to_32bit(
			     value_data,
			     value_data_size,
			     &attachment_descriptor_identifier,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_CONVERSION,
				 LIBCERROR_CONVERSION_ERROR_GENERIC,
				 "%s: unable to set 32-bit entry value.",
				 function );

				return( -1 );
			}
			result = libpff_item_values_get_local_descriptors_value_by_identifier(
				  internal_item->item_values,
				  internal_item->file_io_handle,
				  attachment_descriptor_identifier,
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
				 attachment_descriptor_identifier );

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
				 attachment_descriptor_identifier );

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
			/* Make the attachment item the sub item of the attachments
			 */
			if( libpff_item_tree_append_identifier(
			     internal_item->sub_item_tree_node[ LIBPFF_MESSAGE_SUB_ITEM_ATTACHMENTS ],
			     attachment_descriptor_identifier,
			     local_descriptor_value->data_identifier,
			     local_descriptor_value->local_descriptors_identifier,
			     item_descriptor->recovered,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_APPEND_FAILED,
				 "%s: unable to append attachment item: %" PRIu32 " to attachments item tree node.",
				 function );

				return( -1 );
			}
		}
	}
	return( 1 );
}

/* Determine if the message item has recipients
 * Returns 1 if successful or -1 on error
 */
int libpff_message_determine_recipients(
     libpff_internal_item_t *internal_item,
     libcerror_error_t **error )
{
	libpff_item_descriptor_t *item_descriptor               = NULL;
	libpff_item_descriptor_t *recipients_item_descriptor    = NULL;
	libpff_local_descriptor_value_t *local_descriptor_value = NULL;
	static char *function                                   = "libpff_message_determine_recipients";
	int result                                              = 0;

	if( internal_item == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid internal item.",
		 function );

		return( -1 );
	}
	if( internal_item->internal_file == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid internal item - missing internal file.",
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
	item_descriptor = (libpff_item_descriptor_t *) internal_item->item_tree_node->value;

	if( internal_item->item_values == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid internal item - missing item values.",
		 function );

		return( -1 );
	}
	if( internal_item->sub_item_tree_node[ LIBPFF_MESSAGE_SUB_ITEM_RECIPIENTS ] != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: recipients sub item tree node already set.",
		 function );

		return( -1 );
	}
	/* Make sure the item values have been read
	 */
	if( internal_item->item_values->table == NULL )
	{
#if defined( HAVE_DEBUG_OUTPUT )
		if( libcnotify_verbose != 0 )
		{
			libcnotify_printf(
			 "%s: reading item values of descriptor: %" PRIu32 "\n",
			 function,
			 item_descriptor->descriptor_identifier );
		}
#endif

		if( libpff_item_values_read(
		     internal_item->item_values,
		     internal_item->internal_file->name_to_id_map_list,
		     internal_item->internal_file->io_handle,
		     internal_item->file_io_handle,
		     LIBPFF_DEBUG_ITEM_TYPE_DEFAULT,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_IO,
			 LIBCERROR_IO_ERROR_READ_FAILED,
			 "%s: unable to read item values.",
			 function );

			return( -1 );
		}
		if( internal_item->item_values->table == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
			 "%s: invalid item values - missing table.",
			 function );

			return( -1 );
		}
	}
	/* Determine if the item has recipients
	 */
	result = libpff_item_values_get_local_descriptors_value_by_identifier(
		  internal_item->item_values,
		  internal_item->file_io_handle,
		  (uint32_t) LIBPFF_LOCAL_DESCRIPTOR_IDENTIFIER_RECIPIENTS,
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
		 (uint32_t) LIBPFF_LOCAL_DESCRIPTOR_IDENTIFIER_RECIPIENTS );

		return( -1 );
	}
	else if( result != 0 )
	{
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
		if( libpff_item_values_initialize(
		     &( internal_item->sub_item_values[ LIBPFF_MESSAGE_SUB_ITEM_RECIPIENTS ] ),
		     LIBPFF_LOCAL_DESCRIPTOR_IDENTIFIER_RECIPIENTS,
		     local_descriptor_value->data_identifier,
		     local_descriptor_value->local_descriptors_identifier ,
		     item_descriptor->recovered,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
			 "%s: unable to create recipients item values.",
			 function );

			return( -1 );
		}
		/* Cache the recipients items values for successive usage
		 */
		if( libpff_item_values_read(
		     internal_item->sub_item_values[ LIBPFF_MESSAGE_SUB_ITEM_RECIPIENTS ],
		     internal_item->internal_file->name_to_id_map_list,
		     internal_item->internal_file->io_handle,
		     internal_item->file_io_handle,
		     LIBPFF_DEBUG_ITEM_TYPE_DEFAULT,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_IO,
			 LIBCERROR_IO_ERROR_READ_FAILED,
			 "%s: unable to read recipients local descriptor identifier: %" PRIu32 ".",
			 function,
			 LIBPFF_LOCAL_DESCRIPTOR_IDENTIFIER_RECIPIENTS );

			return( -1 );
		}
		if( internal_item->sub_item_values[ LIBPFF_MESSAGE_SUB_ITEM_RECIPIENTS ]->table == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
			 "%s: invalid recipients item values - missing table.",
			 function );

			return( -1 );
		}
		/* Create an recipients item descriptor
		 */
		if( libpff_item_descriptor_initialize(
		     &recipients_item_descriptor,
		     LIBPFF_LOCAL_DESCRIPTOR_IDENTIFIER_RECIPIENTS,
		     local_descriptor_value->data_identifier,
		     local_descriptor_value->local_descriptors_identifier ,
		     item_descriptor->recovered,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
			 "%s: unable to create recipients item descriptor.",
			 function );

			return( -1 );
		}
		if( libpff_tree_node_initialize(
		     &( internal_item->sub_item_tree_node[ LIBPFF_MESSAGE_SUB_ITEM_RECIPIENTS ] ),
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
			 "%s: unable to create recipients sub item tree node.",
			 function );

			memory_free(
			 recipients_item_descriptor );

			return( -1 );
		}
		if( libpff_tree_node_set_value(
		     internal_item->sub_item_tree_node[ LIBPFF_MESSAGE_SUB_ITEM_RECIPIENTS ],
		     (intptr_t *) recipients_item_descriptor,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
			 "%s: unable to set recipients item descriptor in recipients sub item tree node.",
			 function );

			libpff_tree_node_free(
			 &( internal_item->sub_item_tree_node[ LIBPFF_MESSAGE_SUB_ITEM_RECIPIENTS ] ),
			 NULL,
			 NULL );
			memory_free(
			 recipients_item_descriptor );

			return( -1 );
		}
		/* All the recipient data is in the recipients item
		 * there are no sub items like for the attachments item
		 */
	}
	return( 1 );
}

/* Retrieves the number of attachments from a message item
 * Returns 1 if successful or -1 on error
 */
int libpff_message_get_number_of_attachments(
     libpff_item_t *message,
     int *number_of_attachments,
     libcerror_error_t **error )
{
	libpff_internal_item_t *internal_item = NULL;
	static char *function                 = "libpff_message_get_number_of_attachments";

	if( message == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid message.",
		 function );

		return( -1 );
	}
	internal_item = (libpff_internal_item_t *) message;

	if( internal_item->item_tree_node == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid message - missing item tree node.",
		 function );

		return( -1 );
	}
	if( internal_item->type == LIBPFF_ITEM_TYPE_UNDEFINED )
	{
		if( libpff_item_determine_type(
		     internal_item,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to determine item type.",
			 function );

			return( -1 );
		}
	}
	if( ( internal_item->type == LIBPFF_ITEM_TYPE_ATTACHMENT )
	 || ( internal_item->type == LIBPFF_ITEM_TYPE_ATTACHMENTS )
	 || ( internal_item->type == LIBPFF_ITEM_TYPE_FOLDER )
	 || ( internal_item->type == LIBPFF_ITEM_TYPE_RECIPIENTS ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_UNSUPPORTED_VALUE,
		 "%s: unsupported item type: 0x%08" PRIx32 "",
		 function,
		 internal_item->type );

		return( -1 );
	}
	if( number_of_attachments == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid number of attachments.",
		 function );

		return( -1 );
	}
	if( internal_item->sub_item_tree_node[ LIBPFF_MESSAGE_SUB_ITEM_ATTACHMENTS ] == NULL )
	{
		if( libpff_message_determine_attachments(
		     internal_item,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to determine attachments.",
			 function );

			return( -1 );
		}
	}
	*number_of_attachments = 0;

	if( internal_item->sub_item_tree_node[ LIBPFF_MESSAGE_SUB_ITEM_ATTACHMENTS ] != NULL )
	{
		*number_of_attachments = internal_item->sub_item_tree_node[ LIBPFF_MESSAGE_SUB_ITEM_ATTACHMENTS ]->number_of_sub_nodes;
	}
	return( 1 );
}

/* Retrieves the attachment for the specific index from a message item
 * Returns 1 if successful or -1 on error
 */
int libpff_message_get_attachment(
     libpff_item_t *message,
     int attachment_index,
     libpff_item_t **attachment,
     libcerror_error_t **error )
{
	libpff_internal_item_t *internal_item    = NULL;
	libpff_tree_node_t *attachment_tree_node = NULL;
	static char *function                    = "libpff_message_get_attachment";

	if( message == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid message.",
		 function );

		return( -1 );
	}
	internal_item = (libpff_internal_item_t *) message;

	if( internal_item->item_tree_node == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid message - missing item tree node.",
		 function );

		return( -1 );
	}
	if( internal_item->internal_file == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid message - missing internal file.",
		 function );

		return( -1 );
	}
	if( internal_item->item_values == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid message - missing item values.",
		 function );

		return( -1 );
	}
	if( internal_item->type == LIBPFF_ITEM_TYPE_UNDEFINED )
	{
		if( libpff_item_determine_type(
		     internal_item,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to determine item type.",
			 function );

			return( -1 );
		}
	}
	if( ( internal_item->type == LIBPFF_ITEM_TYPE_ATTACHMENT )
	 || ( internal_item->type == LIBPFF_ITEM_TYPE_ATTACHMENTS )
	 || ( internal_item->type == LIBPFF_ITEM_TYPE_FOLDER )
	 || ( internal_item->type == LIBPFF_ITEM_TYPE_RECIPIENTS ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_UNSUPPORTED_VALUE,
		 "%s: unsupported item type: 0x%08" PRIx32 "",
		 function,
		 internal_item->type );

		return( -1 );
	}
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
	if( *attachment != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: attachment already set.",
		 function );

		return( -1 );
	}
	if( internal_item->sub_item_tree_node[ LIBPFF_MESSAGE_SUB_ITEM_ATTACHMENTS ] == NULL )
	{
		if( libpff_message_determine_attachments(
		     internal_item,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to determine attachments.",
			 function );

			return( -1 );
		}
	}
	if( internal_item->sub_item_tree_node[ LIBPFF_MESSAGE_SUB_ITEM_ATTACHMENTS ] != NULL )    
	{
		if( ( attachment_index < 0 )
		 || ( (uint64_t) attachment_index > (uint64_t) UINT32_MAX )
		 || ( attachment_index >= internal_item->sub_item_tree_node[ LIBPFF_MESSAGE_SUB_ITEM_ATTACHMENTS ]->number_of_sub_nodes ) )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
			 "%s: invalid attachment index value out of bounds.",
			 function );

			return( -1 );
		}
		if( libpff_tree_node_get_sub_node_by_index(
		     internal_item->sub_item_tree_node[ LIBPFF_MESSAGE_SUB_ITEM_ATTACHMENTS ],
		     attachment_index,
		     &attachment_tree_node,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve attachment item tree node.",
			 function );

			return( -1 );
		}
		if( attachment_tree_node == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
			 "%s: invalid attachment item tree node.",
			 function );

			return( -1 );
		}
		if( libpff_item_initialize(
		     attachment,
		     internal_item->file_io_handle,
		     internal_item->internal_file,
		     attachment_tree_node,
		     (libpff_item_descriptor_t *) attachment_tree_node->value,
		     LIBPFF_ITEM_FLAGS_DEFAULT | LIBPFF_ITEM_FLAG_MANAGED_ITEM_TREE_NODE,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
			 "%s: unable to create attachment.",
			 function );

			return( -1 );
		}
		if( *attachment == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
			 "%s: invalid attachment.",
			 function );

			return( -1 );
		}
		( (libpff_internal_item_t *) *attachment )->type = LIBPFF_ITEM_TYPE_ATTACHMENT;
	}
	return( 1 );
}

/* Retrieves the attachments from a message item
 * Returns 1 if successful, 0 if the message item does not contain such value or -1 on error
 */
int libpff_message_get_attachments(
     libpff_item_t *message,
     libpff_item_t **attachments,
     libcerror_error_t **error )
{
	libpff_internal_item_t *internal_item = NULL;
	static char *function                 = "libpff_message_get_attachments";

	if( message == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid message.",
		 function );

		return( -1 );
	}
	internal_item = (libpff_internal_item_t *) message;

	if( internal_item->item_tree_node == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid message - missing item tree node.",
		 function );

		return( -1 );
	}
	if( internal_item->internal_file == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid message - missing internal file.",
		 function );

		return( -1 );
	}
	if( internal_item->item_values == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid message - missing item values.",
		 function );

		return( -1 );
	}
	if( internal_item->type == LIBPFF_ITEM_TYPE_UNDEFINED )
	{
		if( libpff_item_determine_type(
		     internal_item,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to determine item type.",
			 function );

			return( -1 );
		}
	}
	if( ( internal_item->type == LIBPFF_ITEM_TYPE_ATTACHMENT )
	 || ( internal_item->type == LIBPFF_ITEM_TYPE_ATTACHMENTS )
	 || ( internal_item->type == LIBPFF_ITEM_TYPE_FOLDER )
	 || ( internal_item->type == LIBPFF_ITEM_TYPE_RECIPIENTS ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_UNSUPPORTED_VALUE,
		 "%s: unsupported item type: 0x%08" PRIx32 "",
		 function,
		 internal_item->type );

		return( -1 );
	}
	if( attachments == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid attachments.",
		 function );

		return( -1 );
	}
	if( *attachments != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: attachments already set.",
		 function );

		return( -1 );
	}
	if( internal_item->sub_item_tree_node[ LIBPFF_MESSAGE_SUB_ITEM_ATTACHMENTS ] == NULL )
	{
		if( libpff_message_determine_attachments(
		     internal_item,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to determine attachments.",
			 function );

			return( -1 );
		}
	}
	if( internal_item->sub_item_tree_node[ LIBPFF_MESSAGE_SUB_ITEM_ATTACHMENTS ] == NULL )
	{
		return( 0 );
	}
	if( libpff_item_initialize(
	     attachments,
	     internal_item->file_io_handle,
	     internal_item->internal_file,
	     internal_item->sub_item_tree_node[ LIBPFF_MESSAGE_SUB_ITEM_ATTACHMENTS ],
	     (libpff_item_descriptor_t *) internal_item->sub_item_tree_node[ LIBPFF_MESSAGE_SUB_ITEM_ATTACHMENTS ]->value,
	     LIBPFF_ITEM_FLAGS_DEFAULT | LIBPFF_ITEM_FLAG_MANAGED_ITEM_TREE_NODE,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create attachments.",
		 function );

		return( -1 );
	}
	if( *attachments == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid attachments.",
		 function );

		return( -1 );
	}
	( (libpff_internal_item_t *) *attachments )->type = LIBPFF_ITEM_TYPE_ATTACHMENTS;

	/* Clones the item values sub elements from the cached sub item values
	 */
	if( libpff_item_values_clone_copy(
	     ( (libpff_internal_item_t *) *attachments )->item_values,
	     internal_item->sub_item_values[ LIBPFF_MESSAGE_SUB_ITEM_ATTACHMENTS ],
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_COPY_FAILED,
		 "%s: unable to copy attachments item values.",
		 function );

		libpff_item_free(
		 attachments,
		 NULL );

		return( -1 );
	}
	return( 1 );
}

/* Retrieves the recipients from a message item
 * Returns 1 if successful, 0 if the message item does not contain such value or -1 on error
 */
int libpff_message_get_recipients(
     libpff_item_t *message,
     libpff_item_t **recipients,
     libcerror_error_t **error )
{
	libpff_internal_item_t *internal_item = NULL;
	static char *function                 = "libpff_message_get_recipients";

	if( message == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid message.",
		 function );

		return( -1 );
	}
	internal_item = (libpff_internal_item_t *) message;

	if( internal_item->item_tree_node == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid message - missing item tree node.",
		 function );

		return( -1 );
	}
	if( internal_item->internal_file == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid message - missing internal file.",
		 function );

		return( -1 );
	}
	if( internal_item->item_values == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid message - missing item values.",
		 function );

		return( -1 );
	}
	if( internal_item->type == LIBPFF_ITEM_TYPE_UNDEFINED )
	{
		if( libpff_item_determine_type(
		     internal_item,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to determine item type.",
			 function );

			return( -1 );
		}
	}
	if( ( internal_item->type == LIBPFF_ITEM_TYPE_ATTACHMENT )
	 || ( internal_item->type == LIBPFF_ITEM_TYPE_ATTACHMENTS )
	 || ( internal_item->type == LIBPFF_ITEM_TYPE_FOLDER )
	 || ( internal_item->type == LIBPFF_ITEM_TYPE_RECIPIENTS ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_UNSUPPORTED_VALUE,
		 "%s: unsupported item type: 0x%08" PRIx32 "",
		 function,
		 internal_item->type );

		return( -1 );
	}
	if( recipients == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid recipients.",
		 function );

		return( -1 );
	}
	if( *recipients != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: recipients already set.",
		 function );

		return( -1 );
	}
	if( internal_item->sub_item_tree_node[ LIBPFF_MESSAGE_SUB_ITEM_RECIPIENTS ] == NULL )
	{
		if( libpff_message_determine_recipients(
		     internal_item,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to determine recipients.",
			 function );

			return( -1 );
		}
	}
	if( internal_item->sub_item_tree_node[ LIBPFF_MESSAGE_SUB_ITEM_RECIPIENTS ] == NULL )
	{
		return( 0 );
	}
	if( libpff_item_initialize(
	     recipients,
	     internal_item->file_io_handle,
	     internal_item->internal_file,
	     internal_item->sub_item_tree_node[ LIBPFF_MESSAGE_SUB_ITEM_RECIPIENTS ],
	     (libpff_item_descriptor_t *) internal_item->sub_item_tree_node[ LIBPFF_MESSAGE_SUB_ITEM_RECIPIENTS ]->value,
	     LIBPFF_ITEM_FLAGS_DEFAULT | LIBPFF_ITEM_FLAG_MANAGED_ITEM_TREE_NODE,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create recipients.",
		 function );

		return( -1 );
	}
	if( *recipients == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid recipients.",
		 function );

		return( -1 );
	}
	( (libpff_internal_item_t *) *recipients )->type = LIBPFF_ITEM_TYPE_RECIPIENTS;

	/* Clones the item values sub elements from the cached sub item values
	 */
	if( libpff_item_values_clone_copy(
	     ( (libpff_internal_item_t *) *recipients )->item_values,
	     internal_item->sub_item_values[ LIBPFF_MESSAGE_SUB_ITEM_RECIPIENTS ],
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_COPY_FAILED,
		 "%s: unable to copy recipients item values.",
		 function );

		libpff_item_free(
		 recipients,
		 NULL );

		return( -1 );
	}
	return( 1 );
}

/* Retrieves the plain text message body size
 * Size includes the end of string character
 * Returns 1 if successful, 0 if the item does not contain such value or -1 on error
 */
int libpff_message_get_plain_text_body_size(
     libpff_item_t *message,
     size_t *size,
     libcerror_error_t **error )
{
	libpff_internal_item_t *internal_item = NULL;
	uint8_t *value_data                   = NULL;
	static char *function                 = "libpff_message_get_plain_text_body_size";
	size_t value_data_size                = 0;
	uint32_t ascii_codepage               = 0;
	uint32_t message_codepage             = 0;
	uint32_t message_body_codepage        = 0;
	uint32_t value_type                   = 0;
	uint8_t is_ascii_string               = 0;
	int result                            = 0;

	if( message == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid message.",
		 function );

		return( -1 );
	}
	internal_item = (libpff_internal_item_t *) message;

	if( internal_item->internal_file == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid item - missing internal file.",
		 function );

		return( -1 );
	}
	if( internal_item->internal_file->io_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid internal item - invalid internal file - missing IO handle.",
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
	result = libpff_item_get_entry_value(
		  message,
		  0,
		  LIBPFF_ENTRY_TYPE_MESSAGE_BODY_PLAIN_TEXT,
		  &value_type,
		  &value_data,
		  &value_data_size,
		  1,
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
	if( ( value_type == LIBPFF_VALUE_TYPE_STRING_ASCII )
	 || ( value_type == LIBPFF_VALUE_TYPE_STRING_UNICODE ) )
	{
		if( value_type == LIBPFF_VALUE_TYPE_STRING_ASCII )
		{
			if( libpff_item_get_entry_value_32bit(
			     message,
			     0,
			     LIBPFF_ENTRY_TYPE_MESSAGE_BODY_CODEPAGE,
			     &message_body_codepage,
			     0,
			     error ) == -1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
				 "%s: unable to retrieve the message body codepage.",
				 function );

				return( -1 );
			}
			if( libpff_item_get_entry_value_32bit(
			     message,
			     0,
			     LIBPFF_ENTRY_TYPE_MESSAGE_CODEPAGE,
			     &message_codepage,
			     0,
			     error ) == -1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
				 "%s: unable to retrieve the message codepage.",
				 function );

				return( -1 );
			}
/* TODO ignore the message body codepage for now
			if( message_body_codepage != 0 )
			{
				ascii_codepage = message_body_codepage;
			}
			else if( message_codepage != 0 )
*/
			if( message_body_codepage != 0 )
			{
				ascii_codepage = message_codepage;
			}
			else
			{
				ascii_codepage = (uint32_t) internal_item->internal_file->io_handle->ascii_codepage;
			}
			is_ascii_string = 1;
		}
		if( libpff_value_type_get_utf8_string_size(
		     value_data,
		     value_data_size,
		     is_ascii_string,
		     ascii_codepage,
		     size,
		     error ) != 1 )
		{
			/* Sometimes the message codepage is not available
			 */
			if( ( message_codepage == message_body_codepage )
			 || ( message_codepage == 0 ) )
			{
				message_codepage = internal_item->internal_file->io_handle->ascii_codepage;
			}
			/* Sometimes the message body codepade is not the one used to encode
			 * the message body, so try the message codepage as well
			 */
			if( ( ascii_codepage == message_codepage )
			 || ( libpff_value_type_get_utf8_string_size(
			       value_data,
			       value_data_size,
			       is_ascii_string,
			       message_codepage,
			       size,
			       error ) != 1 ) )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
				 "%s: unable to determine message body size.",
				 function );

				return( -1 );
			}
			libcerror_error_free(
			 error );
		}
	}
	else if( value_type == LIBPFF_VALUE_TYPE_BINARY_DATA )
	{
		/* Add the end of string byte
		 */
		*size = value_data_size + 1;
	}
	else
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_UNSUPPORTED_VALUE,
		 "%s: unsupported value type: 0x%04" PRIx32 ".",
		 function,
		 value_type );

		return( -1 );
	}
	return( 1 );
}

/* Retrieves the plain text message body
 * The body is encoded in UTF-8
 * The function uses a codepage if necessary, it tries to read PidTagInternetCodepage first,
 * after that PidTagMessageCodepage and if not available it uses the codepage set for the library
 * Size should include the end of string character
 * Returns 1 if successful, 0 if the item does not contain such value or -1 on error
 */
int libpff_message_get_plain_text_body(
     libpff_item_t *message,
     uint8_t *message_body,
     size_t size,
     libcerror_error_t **error )
{
	libpff_internal_item_t *internal_item = NULL;
	uint8_t *value_data                   = NULL;
	static char *function                 = "libpff_message_get_plain_text_body";
	size_t value_data_size                = 0;
	uint32_t ascii_codepage               = 0;
	uint32_t message_codepage             = 0;
	uint32_t message_body_codepage        = 0;
	uint32_t value_type                   = 0;
	uint8_t is_ascii_string               = 0;
	int result                            = 0;

	if( message == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid message.",
		 function );

		return( -1 );
	}
	internal_item = (libpff_internal_item_t *) message;

	if( internal_item->internal_file == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid item - missing internal file.",
		 function );

		return( -1 );
	}
	if( internal_item->internal_file->io_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid internal item - invalid internal file - missing IO handle.",
		 function );

		return( -1 );
	}
	if( message_body == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid message body.",
		 function );

		return( -1 );
	}
	result = libpff_item_get_entry_value(
		  message,
		  0,
		  LIBPFF_ENTRY_TYPE_MESSAGE_BODY_PLAIN_TEXT,
		  &value_type,
		  &value_data,
		  &value_data_size,
		  1,
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
	if( ( value_type == LIBPFF_VALUE_TYPE_STRING_ASCII )
	 || ( value_type == LIBPFF_VALUE_TYPE_STRING_UNICODE ) )
	{
		if( value_type == LIBPFF_VALUE_TYPE_STRING_ASCII )
		{
			if( libpff_item_get_entry_value_32bit(
			     message,
			     0,
			     LIBPFF_ENTRY_TYPE_MESSAGE_BODY_CODEPAGE,
			     &message_body_codepage,
			     0,
			     error ) == -1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
				 "%s: unable to retrieve the message body codepage.",
				 function );

				return( -1 );
			}
			if( libpff_item_get_entry_value_32bit(
			     message,
			     0,
			     LIBPFF_ENTRY_TYPE_MESSAGE_CODEPAGE,
			     &message_codepage,
			     0,
			     error ) == -1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
				 "%s: unable to retrieve the message codepage.",
				 function );

				return( -1 );
			}
/* TODO ignore the message body codepage for now
			if( message_body_codepage != 0 )
			{
				ascii_codepage = message_body_codepage;
			}
			else if( message_codepage != 0 )
*/
			if( message_codepage != 0 )
			{
				ascii_codepage = message_codepage;
			}
			else
			{
				ascii_codepage = (uint32_t) internal_item->internal_file->io_handle->ascii_codepage;
			}
			is_ascii_string = 1;
		}
		if( libpff_value_type_copy_to_utf8_string(
		     value_data,
		     value_data_size,
		     is_ascii_string,
		     ascii_codepage,
		     message_body,
		     size,
		     error ) != 1 )
		{
			/* Sometimes the message codepage is not available
			 */
			if( ( message_codepage == message_body_codepage )
			 || ( message_codepage == 0 ) )
			{
				message_codepage = internal_item->internal_file->io_handle->ascii_codepage;
			}
			/* Sometimes the message body codepade is not the one used to encode
			 * the message body, so try the message codepage as well
			 */
			if( ( ascii_codepage == message_codepage )
			 || ( libpff_value_type_copy_to_utf8_string(
			       value_data,
			       value_data_size,
			       is_ascii_string,
			       message_codepage,
			       message_body,
			       size,
			       error ) != 1 ) )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_CONVERSION,
				 LIBCERROR_CONVERSION_ERROR_GENERIC,
				 "%s: unable to set message body.",
				 function );

				return( -1 );
			}
			libcerror_error_free(
			 error );
		}
	}
	else if( value_type == LIBPFF_VALUE_TYPE_BINARY_DATA )
	{
		if( size < value_data_size )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
			 LIBCERROR_ARGUMENT_ERROR_VALUE_TOO_SMALL,
			 "%s: message body size too small.",
			 function );

			return( -1 );
		}
		if( memory_copy(
		     message_body,
		     value_data,
		     value_data_size ) == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_MEMORY,
			 LIBCERROR_MEMORY_ERROR_COPY_FAILED,
			 "%s: unable to set message body .",
			 function );

			return( -1 );
		}
		/* Add the end of string byte
		 */
		message_body[ value_data_size ] = 0;
	}
	else
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_UNSUPPORTED_VALUE,
		 "%s: unsupported value type: 0x%04" PRIx32 ".",
		 function,
		 value_type );

		return( -1 );
	}
	return( 1 );
}

/* Retrieves the RTF message body size
 * Size includes the end of string character
 * Returns 1 if successful, 0 if the item does not contain such value or -1 on error
 */
int libpff_message_get_rtf_body_size(
     libpff_item_t *message,
     size_t *size,
     libcerror_error_t **error )
{
	uint8_t *value_data   = NULL;
	static char *function = "libpff_message_get_rtf_body_size";
	uint32_t value_type   = LIBPFF_VALUE_TYPE_BINARY_DATA;
	int result            = 0;

	if( message == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid message.",
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
	result = libpff_item_get_entry_value(
		  message,
		  0,
		  LIBPFF_ENTRY_TYPE_MESSAGE_BODY_COMPRESSED_RTF,
		  &value_type,
		  &value_data,
		  size,
		  0,
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
	if( value_data == NULL )
	{
		*size = 0;
	}
	else if( libfmapi_lzfu_get_uncompressed_data_size(
	          value_data,
	          *size,
	          size,
	          error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_COMPRESSION,
		 LIBCERROR_COMPRESSION_ERROR_DECOMPRESS_FAILED,
		 "%s: unable to determine uncompressed RTF body size.",
		 function );

		return( -1 );
	}
	return( 1 );
}

/* Retrieves the RTF message body
 * The body is encoded in UTF-8
 * Size should include the end of string character
 * Returns 1 if successful, 0 if the item does not contain such value or -1 on error
 */
int libpff_message_get_rtf_body(
     libpff_item_t *message,
     uint8_t *message_body,
     size_t size,
     libcerror_error_t **error )
{
	uint8_t *value_data    = NULL;
	static char *function  = "libpff_message_get_rtf_body";
	size_t value_data_size = 0;
	uint32_t value_type    = LIBPFF_VALUE_TYPE_BINARY_DATA;
	int result             = 0;

	if( message == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid message.",
		 function );

		return( -1 );
	}
	if( message_body == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid message body.",
		 function );

		return( -1 );
	}
	result = libpff_item_get_entry_value(
		  message,
		  0,
		  LIBPFF_ENTRY_TYPE_MESSAGE_BODY_COMPRESSED_RTF,
		  &value_type,
		  &value_data,
		  &value_data_size,
		  0,
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
	if( libfmapi_lzfu_decompress(
	     message_body,
	     &size,
	     value_data,
	     value_data_size,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_COMPRESSION,
		 LIBCERROR_COMPRESSION_ERROR_DECOMPRESS_FAILED,
		 "%s: unable to decompress message body.",
		 function );

		return( -1 );
	}
	return( 1 );
}

/* Retrieves the HTML message body size
 * Size includes the end of string character
 * Returns 1 if successful, 0 if the item does not contain such value or -1 on error
 */
int libpff_message_get_html_body_size(
     libpff_item_t *message,
     size_t *size,
     libcerror_error_t **error )
{
	libpff_internal_item_t *internal_item = NULL;
	uint8_t *value_data                   = NULL;
	static char *function                 = "libpff_message_get_html_body_size";
	size_t value_data_size                = 0;
	uint32_t value_type                   = 0;
	int result                            = 0;

	if( message == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid message.",
		 function );

		return( -1 );
	}
	internal_item = (libpff_internal_item_t *) message;

	if( internal_item->internal_file == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid item - missing internal file.",
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
	result = libpff_item_get_entry_value(
		  message,
		  0,
		  LIBPFF_ENTRY_TYPE_MESSAGE_BODY_HTML,
		  &value_type,
		  &value_data,
		  &value_data_size,
		  1,
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
	if( value_data == NULL )
	{
		return( 0 );
	}
	if( value_type == LIBPFF_VALUE_TYPE_STRING_ASCII )
	{
		*size = value_data_size;

		if( value_data[ value_data_size - 1 ] != 0 )
		{
			/* Add the end of string byte
			 */
			*size += 1;
		}
	}
	else if( value_type == LIBPFF_VALUE_TYPE_STRING_UNICODE )
	{
		*size = value_data_size;

		if( ( value_data[ value_data_size - 1 ] != 0 )
		 || ( value_data[ value_data_size - 2 ] != 0 ) )
		{
			/* Add the end of string byte
			 */
			*size += 2;
		}
	}
	else if( value_type == LIBPFF_VALUE_TYPE_BINARY_DATA )
	{
		*size = value_data_size;

		if( value_data[ value_data_size - 1 ] != 0 )
		{
			/* Add the end of string byte
			 */
			*size += 1;
		}
	}
	else
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_UNSUPPORTED_VALUE,
		 "%s: unsupported value type: 0x%04" PRIx32 ".",
		 function,
		 value_type );

		return( -1 );
	}
	return( 1 );
}

/* Retrieves the HTML message body
 * Size should include the end of string character
 * Returns 1 if successful, 0 if the item does not contain such value or -1 on error
 */
int libpff_message_get_html_body(
     libpff_item_t *message,
     uint8_t *message_body,
     size_t size,
     libcerror_error_t **error )
{
	libpff_internal_item_t *internal_item = NULL;
	uint8_t *value_data                   = NULL;
	static char *function                 = "libpff_message_get_html_body";
	size_t value_data_size                = 0;
	uint32_t value_type                   = 0;
	int result                            = 0;

	if( message == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid message.",
		 function );

		return( -1 );
	}
	internal_item = (libpff_internal_item_t *) message;

	if( internal_item->internal_file == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid item - missing internal file.",
		 function );

		return( -1 );
	}
	if( message_body == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid message body.",
		 function );

		return( -1 );
	}
	result = libpff_item_get_entry_value(
		  message,
		  0,
		  LIBPFF_ENTRY_TYPE_MESSAGE_BODY_HTML,
		  &value_type,
		  &value_data,
		  &value_data_size,
		  1,
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
	if( value_data == NULL )
	{
		return( 0 );
	}
	if( ( value_type == LIBPFF_VALUE_TYPE_STRING_ASCII )
	 || ( value_type == LIBPFF_VALUE_TYPE_STRING_UNICODE )
	 || ( value_type == LIBPFF_VALUE_TYPE_BINARY_DATA ) )
	{
		if( size < value_data_size )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
			 LIBCERROR_ARGUMENT_ERROR_VALUE_TOO_SMALL,
			 "%s: message body size too small.",
			 function );

			return( -1 );
		}
		if( memory_copy(
		     message_body,
		     value_data,
		     value_data_size ) == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_MEMORY,
			 LIBCERROR_MEMORY_ERROR_COPY_FAILED,
			 "%s: unable to set message body .",
			 function );

			return( -1 );
		}
		if( value_type == LIBPFF_VALUE_TYPE_STRING_ASCII )
		{
			if( value_data[ value_data_size - 1 ] != 0 )
			{
				if( ( size + 1 ) < value_data_size )
				{
					libcerror_error_set(
					 error,
					 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
					 LIBCERROR_ARGUMENT_ERROR_VALUE_TOO_SMALL,
					 "%s: message body size too small.",
					 function );

					return( -1 );
				}
				message_body[ value_data_size ] = 0;
			}
		}
		else if( value_type == LIBPFF_VALUE_TYPE_STRING_UNICODE )
		{
			if( ( value_data[ value_data_size - 1 ] != 0 )
			 || ( value_data[ value_data_size - 2 ] != 0 ) )
			{
				if( ( size + 2 ) < value_data_size )
				{
					libcerror_error_set(
					 error,
					 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
					 LIBCERROR_ARGUMENT_ERROR_VALUE_TOO_SMALL,
					 "%s: message body size too small.",
					 function );

					return( -1 );
				}
				message_body[ value_data_size     ] = 0;
				message_body[ value_data_size + 1 ] = 0;
			}
		}
		else if( value_type == LIBPFF_VALUE_TYPE_BINARY_DATA )
		{
			if( value_data[ value_data_size - 1 ] != 0 )
			{
				if( ( size + 1 ) < value_data_size )
				{
					libcerror_error_set(
					 error,
					 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
					 LIBCERROR_ARGUMENT_ERROR_VALUE_TOO_SMALL,
					 "%s: message body size too small.",
					 function );

					return( -1 );
				}
				message_body[ value_data_size ] = 0;
			}
		}
	}
	else
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_UNSUPPORTED_VALUE,
		 "%s: unsupported value type: 0x%04" PRIx32 ".",
		 function,
		 value_type );

		return( -1 );
	}
	return( 1 );
}

