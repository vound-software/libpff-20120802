/*
 * Debug functions
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
#include <file_stream.h>
#include <memory.h>
#include <types.h>

#include "libpff_debug.h"
#include "libpff_definitions.h"
#include "libpff_libbfio.h"
#include "libpff_libcerror.h"
#include "libpff_libcnotify.h"
#include "libpff_libfcache.h"
#include "libpff_libfdata.h"
#include "libpff_libfguid.h"
#include "libpff_libfmapi.h"
#include "libpff_name_to_id_map.h"

#include "pff_value_data.h"

#if defined( HAVE_DEBUG_OUTPUT )

/* Retrieves a string containing the name to id map table property type identifier
 */
const char *libpff_debug_get_name_to_id_map_property_type_identifier(
             uint32_t entry_type,
             uint32_t value_type )
{
        if( entry_type == 0x0001 )
        {
		switch( value_type )
		{
			case 0x0003:
				return( "PidTagNameidBucketCount" );
		}
	}
        else if( entry_type == 0x0002 )
        {
		switch( value_type )
		{
			case 0x0102:
				return( "PidTagNameidStreamGuid" );
		}
	}
	else if( entry_type == 0x0003 )
	{
		switch( value_type )
		{
			case 0x0102:
				return( "PidTagNameidStreamEntry" );
		}
	}
	else if( entry_type == 0x0004 )
	{
		switch( value_type )
		{
			case 0x0102:
				return( "PidTagNameidStreamString" );
		}
	}
	if( entry_type >= 0x1000 )
	{
		switch( value_type )
		{
			case 0x0102:
				return( "PidTagNameidBucketBase" );
		}
	}
	return( "_UNKNOWN_" );
}

/* Retrieves a string containing the name to id map table property type description
 */
const char *libpff_debug_get_name_to_id_map_property_type_description(
             uint32_t entry_type,
             uint32_t value_type )
{
        if( entry_type == 0x0001 )
        {
		switch( value_type )
		{
			case 0x0003:
				return( "Name to ID Map Number of Validation Entries" );
		}
	}
        else if( entry_type == 0x0002 )
        {
		switch( value_type )
		{
			case 0x0102:
				return( "Name to ID Map Class Identifiers" );
		}
	}
	else if( entry_type == 0x0003 )
	{
		switch( value_type )
		{
			case 0x0102:
				return( "Name to ID Map Entries" );
		}
	}
	else if( entry_type == 0x0004 )
	{
		switch( value_type )
		{
			case 0x0102:
				return( "Name to ID Map Strings" );
		}
	}
	if( entry_type >= 0x1000 )
	{
		switch( value_type )
		{
			case 0x0102:
				return( "Name to ID Map Validation Entry" );
		}
	}
	return( "Unknown" );
}

/* Prints the node identifier type
 */
void libpff_debug_print_node_identifier_type(
      uint8_t node_identifier_type )
{
	switch( node_identifier_type )
	{
		case LIBPFF_NODE_IDENTIFIER_TYPE_TABLE_VALUE:
			libcnotify_printf(
			 "(Table value)" );
			break;

		case LIBPFF_NODE_IDENTIFIER_TYPE_INTERNAL:
			libcnotify_printf(
			 "(Internal)" );
			break;

		case LIBPFF_NODE_IDENTIFIER_TYPE_FOLDER:
			libcnotify_printf(
			 "(Folder)" );
			break;

		case LIBPFF_NODE_IDENTIFIER_TYPE_SEARCH_FOLDER:
			libcnotify_printf(
			 "(Search folder)" );
			break;

		case LIBPFF_NODE_IDENTIFIER_TYPE_MESSAGE:
			libcnotify_printf(
			 "(Message)" );
			break;

		case LIBPFF_NODE_IDENTIFIER_TYPE_ATTACHMENT:
			libcnotify_printf(
			 "(Attachment)" );
			break;

		case LIBPFF_NODE_IDENTIFIER_TYPE_SEARCH_UPDATE_QUEUE:
			libcnotify_printf(
			 "(Search update queue)" );
			break;

		case LIBPFF_NODE_IDENTIFIER_TYPE_SEARCH_CRITERIA_OBJECT:
			libcnotify_printf(
			 "(Search criteria object)" );
			break;

		case LIBPFF_NODE_IDENTIFIER_TYPE_ASSOCIATED_CONTENT:
			libcnotify_printf(
			 "(Associated content)" );
			break;

		case LIBPFF_NODE_IDENTIFIER_TYPE_CONTENTS_TABLE_INDEX:
			libcnotify_printf(
			 "(Contents table index)" );
			break;

		case LIBPFF_NODE_IDENTIFIER_TYPE_INBOX:
			libcnotify_printf(
			 "(Inbox)" );
			break;

		case LIBPFF_NODE_IDENTIFIER_TYPE_OUTBOX:
			libcnotify_printf(
			 "(Outbox)" );
			break;

		case LIBPFF_NODE_IDENTIFIER_TYPE_SUB_FOLDERS:
			libcnotify_printf(
			 "(Sub folders)" );
			break;

		case LIBPFF_NODE_IDENTIFIER_TYPE_SUB_MESSAGES:
			libcnotify_printf(
			 "(Sub messages)" );
			break;

		case LIBPFF_NODE_IDENTIFIER_TYPE_SUB_ASSOCIATED_CONTENTS:
			libcnotify_printf(
			 "(Sub associated contents)" );
			break;

		case LIBPFF_NODE_IDENTIFIER_TYPE_SEARCH_CONTENTS_TABLE:
			libcnotify_printf(
			 "(Search contents table)" );
			break;

		case LIBPFF_NODE_IDENTIFIER_TYPE_ATTACHMENTS:
			libcnotify_printf(
			 "(Attachments)" );
			break;

		case LIBPFF_NODE_IDENTIFIER_TYPE_RECIPIENTS:
			libcnotify_printf(
			 "(Reciepients)" );
			break;

		case LIBPFF_NODE_IDENTIFIER_TYPE_SEARCH_TABLE:
			libcnotify_printf(
			 "(Search table)" );
			break;

		case LIBPFF_NODE_IDENTIFIER_TYPE_LOCAL_DESCRIPTOR_VALUE:
			libcnotify_printf(
			 "(Local descriptor value)" );
			break;

		default:
			libcnotify_printf(
			 "(Unknown)" );
			break;
	}
}

/* Prints the property type value
 * Returns 1 if successful or -1 on error
 */
int libpff_debug_property_type_value_reference_print(
     libpff_list_t *name_to_id_map_list,
     uint32_t entry_type,
     uint32_t value_type,
     libfdata_reference_t *value_data_reference,
     libbfio_handle_t *file_io_handle,
     libfcache_cache_t *descriptor_cache,
     int debug_item_type,
     int ascii_codepage,
     libcerror_error_t **error )
{
	uint8_t *value_data    = NULL;
	static char *function  = "libpff_debug_property_type_value_reference_print";
	size_t value_data_size = 0;

	if( libfdata_reference_get_data(
	     value_data_reference,
	     file_io_handle,
	     descriptor_cache,
	     &value_data,
	     &value_data_size,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve data reference data.",
		 function );

		return( -1 );
	}
	if( libpff_debug_property_type_value_print(
	     name_to_id_map_list,
	     entry_type,
	     value_type,
	     value_data,
	     value_data_size,
	     debug_item_type,
	     ascii_codepage,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_PRINT_FAILED,
		 "%s: unable to print property type value.",
		 function );

		return( -1 );
	}
	return( 1 );
}

/* Prints the property type value
 * Returns 1 if successful or -1 on error
 */
int libpff_debug_property_type_value_print(
     libpff_list_t *name_to_id_map_list,
     uint32_t entry_type,
     uint32_t value_type,
     uint8_t *value_data,
     size_t value_data_size,
     int debug_item_type,
     int ascii_codepage,
     libcerror_error_t **error )
{
	static char *function = "libpff_debug_property_type_value_print";

	if( debug_item_type == LIBPFF_DEBUG_ITEM_TYPE_DEFAULT )
	{
		if( libpff_debug_mapi_value_print(
		     name_to_id_map_list,
		     entry_type,
		     value_type,
		     value_data,
		     value_data_size,
		     ascii_codepage,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_PRINT_FAILED,
			 "%s: unable to print mapi value.",
			 function );

			return( -1 );
		}
		return( 1 );
	}
	else if( debug_item_type == LIBPFF_DEBUG_ITEM_TYPE_NAME_TO_ID_MAP )
	{
		if( libpff_debug_name_to_id_map_value_print(
		     entry_type,
		     value_type,
		     value_data,
		     value_data_size,
		     ascii_codepage,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_PRINT_FAILED,
			 "%s: unable to print name to id map value.",
			 function );

			return( -1 );
		}
		return( 1 );
	}
	else if( value_data != NULL )
	{
		libcnotify_print_data(
		 value_data,
		 value_data_size,
		 0 );

		return( 1 );
	}
	return( 1 );
}

/* Prints the MAPI value
 * Returns 1 if successful or -1 on error
 */
int libpff_debug_mapi_value_print(
     libpff_list_t *name_to_id_map_list,
     uint32_t entry_type,
     uint32_t value_type,
     uint8_t *value_data,
     size_t value_data_size,
     int ascii_codepage,
     libcerror_error_t **error )
{
	libpff_list_element_t *list_element = NULL;
	static char *function               = "libpff_debug_mapi_value_print";
	int list_iterator                   = 0;

	if( name_to_id_map_list != NULL )
	{
		list_element = name_to_id_map_list->first_element;

		for( list_iterator = 0;
		     list_iterator < name_to_id_map_list->number_of_elements;
		     list_iterator++ )
		{
			if( list_element == NULL )
			{
				break;
			}
			if( list_element->value == NULL )
			{
				continue;
			}
			if( ( (libpff_internal_name_to_id_map_entry_t *) list_element->value )->identifier == entry_type )
			{
				if( ( (libpff_internal_name_to_id_map_entry_t *) list_element->value )->type == LIBPFF_NAME_TO_ID_MAP_ENTRY_TYPE_NUMERIC )
				{
					entry_type = ( (libpff_internal_name_to_id_map_entry_t *) list_element->value )->numeric_value;

					break;
				}
			}
			list_element = list_element->next_element;
		}
	}
	if( libfmapi_debug_print_value(
	     entry_type,
	     value_type,
	     value_data,
	     value_data_size,
	     ascii_codepage,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_PRINT_FAILED,
		 "%s: unable to print MAPI value.",
		 function );

		if( ( error != NULL )
		 && ( *error != NULL ) )
		{
			libcnotify_print_error_backtrace(
			 *error );
		}
		libcerror_error_free(
		 error );

		if( value_data != NULL )
		{
			libcnotify_print_data(
			 value_data,
			 value_data_size,
			 0 );
		}
		else
		{
			libcnotify_printf(
			 "<NULL>\n\n" );
		}
	}
	return( 1 );
}

/* Prints the name to id map value
 * Returns 1 if successful or -1 on error
 */
int libpff_debug_name_to_id_map_value_print(
     uint32_t entry_type,
     uint32_t value_type,
     uint8_t *value_data,
     size_t value_data_size,
     int ascii_codepage,
     libcerror_error_t **error )
{
	libcstring_system_character_t guid_string[ 48 ];

	libfguid_identifier_t *guid = NULL;
	static char *function       = "libpff_debug_name_to_id_map_value_print";
	uint32_t number_of_values   = 0;
	uint32_t value_32bit        = 0;
	uint32_t value_iterator     = 0;
	uint16_t value_16bit        = 0;
	int result                  = 0;

	if( value_type != 0x0102 )
	{
		if( value_data == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
			 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
			 "%s: invalid value data.",
			 function );

			goto on_error;
		}
		libcnotify_print_data(
		 value_data,
		 value_data_size,
		 0 );
	}
	else if( entry_type == 0x0002 )
	{
		if( value_data == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
			 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
			 "%s: invalid value data.",
			 function );

			goto on_error;
		}
		if( ( value_data_size % 16 ) == 0 )
		{
			number_of_values = (uint32_t) ( value_data_size / 16 );

			if( number_of_values > 0 )
			{
				if( libfguid_identifier_initialize(
				     &guid,
				     error ) != 1 )
				{
					libcerror_error_set(
					 error,
					 LIBCERROR_ERROR_DOMAIN_RUNTIME,
					 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
					 "%s: unable to create GUID.",
					 function );

					goto on_error;
				}
				for( value_iterator = 0;
				     value_iterator < number_of_values;
				     value_iterator++ )
				{
					if( libfguid_identifier_copy_from_byte_stream(
					     guid,
					     value_data,
					     16,
					     LIBFGUID_ENDIAN_LITTLE,
					     error ) != 1 )
					{
						libcerror_error_set(
						 error,
						 LIBCERROR_ERROR_DOMAIN_RUNTIME,
						 LIBCERROR_RUNTIME_ERROR_COPY_FAILED,
						 "%s: unable to copy byte stream to GUID.",
						 function );

						goto on_error;
					}
#if defined( LIBCSTRING_HAVE_WIDE_SYSTEM_CHARACTER )
					result = libfguid_identifier_copy_to_utf16_string(
						  guid,
						  (uint16_t *) guid_string,
						  48,
						  LIBFGUID_STRING_FORMAT_USE_LOWER_CASE,
						  error );
#else
					result = libfguid_identifier_copy_to_utf8_string(
						  guid,
						  (uint8_t *) guid_string,
						  48,
						  LIBFGUID_STRING_FORMAT_USE_LOWER_CASE,
						  error );
#endif
					if( result != 1 )
					{
						libcerror_error_set(
						 error,
						 LIBCERROR_ERROR_DOMAIN_RUNTIME,
						 LIBCERROR_RUNTIME_ERROR_COPY_FAILED,
						 "%s: unable to copy GUID to string.",
						 function );

						goto on_error;
					}
					libcnotify_printf(
					 "Class identifier: %02" PRIu32 "\t: %" PRIs_LIBCSTRING_SYSTEM " (%s)\n",
					 value_iterator,
					 guid_string,
					 libfmapi_class_identifier_get_name(
					  value_data ) );

					value_data += 16;
				}
				if( libfguid_identifier_free(
				     &guid,
				     error ) != 1 )
				{
					libcerror_error_set(
					 error,
					 LIBCERROR_ERROR_DOMAIN_RUNTIME,
					 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
					 "%s: unable to free GUID.",
					 function );

					goto on_error;
				}
			}
			libcnotify_printf(
			 "\n" );
		}
		else
		{
			libcnotify_print_data(
			 value_data,
			 value_data_size,
			 0 );
		}
	}
	else if( entry_type == 0x003 )
	{
		if( value_data == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
			 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
			 "%s: invalid value data.",
			 function );

			goto on_error;
		}
		if( ( value_data_size % 8 ) == 0 )
		{
			number_of_values = (uint32_t) ( value_data_size / 8 );

			for( value_iterator = 0;
			     value_iterator < number_of_values;
			     value_iterator++ )
			{
				byte_stream_copy_to_uint32_little_endian(
				 value_data,
				 value_32bit );

				value_data += 4;

				byte_stream_copy_to_uint16_little_endian(
				 value_data,
				 value_16bit );

				value_data += 2;

				if( ( value_16bit & 0x00000001 ) == 0x00000001 )
				{
					libcnotify_printf(
					 "Entry: %04" PRIu32 " reference\t: 0x%08" PRIx32 "\n",
					 value_iterator,
					 value_32bit );
				}
				else
				{
					libcnotify_printf(
					 "Entry: %04" PRIu32 " value\t: 0x%08" PRIx32 "\n",
					 value_iterator,
					 value_32bit );
				}
				libcnotify_printf(
				 "Entry: %04" PRIu32 " type\t: %" PRIu16 "\n",
				 value_iterator,
				 value_16bit );

				byte_stream_copy_to_uint16_little_endian(
				 value_data,
				 value_16bit );

				libcnotify_printf(
				 "Entry: %04" PRIu32 " number\t: 0x%04" PRIx16 "\n",
				 value_iterator,
				 value_16bit );

				value_data += 2;
			}
			libcnotify_printf(
			 "\n" );
		}
		else
		{
			libcnotify_print_data(
			 value_data,
			 value_data_size,
			 0 );
		}
	}
	else if( entry_type >= 0x1000 )
	{
		if( value_data == NULL )
		{
			libcnotify_printf(
			 "\n" );
		}
		else if( ( value_data_size % 8 ) == 0 )
		{
			number_of_values = (uint32_t) ( value_data_size / 8 );

			for( value_iterator = 0;
			     value_iterator < number_of_values;
			     value_iterator++ )
			{
				byte_stream_copy_to_uint32_little_endian(
				 value_data,
				 value_32bit );

				value_data += 4;

				byte_stream_copy_to_uint16_little_endian(
				 value_data,
				 value_16bit );

				value_data += 2;

				if( ( value_16bit & 0x00000001 ) == 0x00000001 )
				{
					libcnotify_printf(
					 "Validation entry: %02" PRIu32 " crc\t: 0x%08" PRIx32 "\n",
					 value_iterator,
					 value_32bit );
				}
				else
				{
					libcnotify_printf(
					 "Validation entry: %02" PRIu32 " value\t: 0x%08" PRIx32 "\n",
					 value_iterator,
					 value_32bit );
				}
				libcnotify_printf(
				 "Validation entry: %02" PRIu32 " type\t: %" PRIu16 "\n",
				 value_iterator,
				 value_16bit );

				byte_stream_copy_to_uint16_little_endian(
				 value_data,
				 value_16bit );

				libcnotify_printf(
				 "Validation entry: %02" PRIu32 " number\t: 0x%04" PRIx16 "\n",
				 value_iterator,
				 value_16bit );

				value_data += 2;
			}
			libcnotify_printf(
			 "\n" );
		}
		else
		{
			libcnotify_print_data(
			 value_data,
			 value_data_size,
			 0 );
		}
	}
	else
	{
		libcnotify_print_data(
		 value_data,
		 value_data_size,
		 0 );
	}
	return( 1 );

on_error:
	if( guid != NULL )
	{
		libfguid_identifier_free(
		 &guid,
		 NULL );
	}
	return( -1 );
}

/* Prints the read offsets
 * Returns 1 if successful or -1 on error
 */
int libpff_debug_print_read_offsets(
     libbfio_handle_t *file_io_handle,
     libcerror_error_t **error )
{
	static char *function = "libpff_debug_print_read_offsets";
	off64_t offset        = 0;
	size64_t size         = 0;
	int number_of_offsets = 0;
	int offset_iterator   = 0;

	if( file_io_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid file IO handle.",
		 function );

		return( -1 );
	}
	if( libbfio_handle_get_number_of_offsets_read(
	     file_io_handle,
	     &number_of_offsets,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve number of offsets read.",
		 function );

		return( -1 );
	}
	libcnotify_printf(
	 "Offsets read:\n" );

	for( offset_iterator = 0;
	     offset_iterator < number_of_offsets;
	     offset_iterator++ )
	{
		if( libbfio_handle_get_offset_read(
		     file_io_handle,
		     offset_iterator,
		     &offset,
		     &size,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve offset: %d.",
			 function,
			 offset_iterator );

			return( -1 );
		}
		libcnotify_printf(
		 "%08" PRIu64 " ( 0x%08" PRIx64 " ) - %08" PRIu64 " ( 0x%08" PRIx64 " ) size: %" PRIu64 "\n",
		 offset,
		 offset,
		 offset + size,
		 offset + size,
		 size );
	}
	libcnotify_printf(
	 "\n" );

	return( 1 );
}

#endif

