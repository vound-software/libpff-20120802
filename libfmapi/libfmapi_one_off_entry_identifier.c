/*
 * One-off entry identifier functions
 *
 * Copyright (c) 2009-2012, Joachim Metz <joachim.metz@gmail.com>
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

#include "libfmapi_definitions.h"
#include "libfmapi_libcerror.h"
#include "libfmapi_libcnotify.h"
#include "libfmapi_libuna.h"
#include "libfmapi_one_off_entry_identifier.h"

/* Initialize an one-off entry identifier
 * Returns 1 if successful or -1 on error
 */
int libfmapi_one_off_entry_identifier_initialize(
     libfmapi_one_off_entry_identifier_t **entry_identifier,
     libcerror_error_t **error )
{
	libfmapi_internal_one_off_entry_identifier_t *internal_entry_identifier = NULL;
	static char *function                                                   = "libfmapi_one_off_entry_identifier_initialize";

	if( entry_identifier == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid entry identifier.",
		 function );

		return( -1 );
	}
	if( *entry_identifier != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid entry identifier value already set.",
		 function );

		return( -1 );
	}
	internal_entry_identifier = memory_allocate_structure(
	                             libfmapi_internal_one_off_entry_identifier_t );

	if( internal_entry_identifier == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create entry identifier.",
		 function );

		goto on_error;
	}
	if( memory_set(
	     internal_entry_identifier,
	     0,
	     sizeof( libfmapi_internal_one_off_entry_identifier_t ) ) == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_SET_FAILED,
		 "%s: unable to clear entry identifier.",
		 function );

		goto on_error;
	}
	*entry_identifier = (libfmapi_one_off_entry_identifier_t *) internal_entry_identifier;

	return( 1 );

on_error:
	if( internal_entry_identifier != NULL )
	{
		memory_free(
		 internal_entry_identifier );
	}
	return( -1 );
}

/* Frees an one-off entry identifier
 * Returns 1 if successful or -1 on error
 */
int libfmapi_one_off_entry_identifier_free(
     libfmapi_one_off_entry_identifier_t **entry_identifier,
     libcerror_error_t **error )
{
	libfmapi_internal_one_off_entry_identifier_t *internal_entry_identifier = NULL;
	static char *function                                                           = "libfmapi_one_off_entry_identifier_free";

	if( entry_identifier == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid entry identifier.",
		 function );

		return( -1 );
	}
	if( *entry_identifier != NULL )
	{
		internal_entry_identifier = (libfmapi_internal_one_off_entry_identifier_t *) *entry_identifier;
		*entry_identifier         = NULL;

		if( internal_entry_identifier->display_name != NULL )
		{
			memory_free(
			 internal_entry_identifier->display_name );
		}
		if( internal_entry_identifier->address_type != NULL )
		{
			memory_free(
			 internal_entry_identifier->address_type );
		}
		if( internal_entry_identifier->email_address != NULL )
		{
			memory_free(
			 internal_entry_identifier->email_address );
		}
		memory_free(
		 internal_entry_identifier );
	}
	return( 1 );
}

/* Converts a byte stream into an one-off entry identifier
 * Returns 1 if successful or -1 on error
 */
int libfmapi_one_off_entry_identifier_copy_from_byte_stream(
     libfmapi_one_off_entry_identifier_t *entry_identifier,
     uint8_t *byte_stream,
     size_t byte_stream_size,
     libcerror_error_t **error )
{
	libfmapi_internal_one_off_entry_identifier_t *internal_entry_identifier = NULL;
	static char *function                                                   = "libfmapi_one_off_entry_identifier_copy_from_byte_stream";
	size_t byte_stream_index                                                = 0;
	uint16_t supported_flags                                                = 0;

	if( entry_identifier == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid entry identifier.",
		 function );

		return( -1 );
	}
	internal_entry_identifier = (libfmapi_internal_one_off_entry_identifier_t *) entry_identifier;

	if( byte_stream == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid byte stream.",
		 function );

		return( -1 );
	}
	if( byte_stream_size < 4 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_TOO_SMALL,
		 "%s: byte stream too small.",
		 function );

		return( -1 );
	}
	if( byte_stream_size > (size_t) SSIZE_MAX )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_EXCEEDS_MAXIMUM,
		 "%s: byte stream size exceeds maximum.",
		 function );

		return( -1 );
	}
	byte_stream_copy_to_uint16_little_endian(
	 byte_stream,
	 internal_entry_identifier->version );

	byte_stream      += 2;
	byte_stream_size -= 2;

	if( internal_entry_identifier->version != 0 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
		 "%s: unsupported version: %" PRIu16 ".",
		 function,
		 internal_entry_identifier->version );

		goto on_error;
	}
	byte_stream_copy_to_uint16_little_endian(
	 byte_stream,
	 internal_entry_identifier->flags );

	byte_stream      += 2;
	byte_stream_size -= 2;

	supported_flags = LIBFMAPI_ONE_OFF_ENTRY_IDENTIFIER_FLAG_NO_RICH_INFO
	                | LIBFMAPI_ONE_OFF_ENTRY_IDENTIFIER_FLAG_0x1000
	                | LIBFMAPI_ONE_OFF_ENTRY_IDENTIFIER_FLAG_UNICODE;

	if( ( internal_entry_identifier->flags & ~( supported_flags ) ) != 0 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
		 "%s: unsupported flags: 0x%04" PRIx16 ".",
		 function,
		 internal_entry_identifier->flags );

		goto on_error;
	}
	if( byte_stream_size == 0 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_TOO_SMALL,
		 "%s: byte stream too small.",
		 function );

		goto on_error;
	}
	byte_stream_index = 0;

	if( ( internal_entry_identifier->flags & LIBFMAPI_ONE_OFF_ENTRY_IDENTIFIER_FLAG_UNICODE ) != 0 )
	{
		while( ( byte_stream_index + 1 ) < byte_stream_size )
		{
			if( ( byte_stream[ byte_stream_index ] == 0 )
			 && ( byte_stream[ byte_stream_index + 1 ] == 0 ) )
			{
				break;
			}
			byte_stream_index += 2;
		}
		if( ( byte_stream[ byte_stream_index ] != 0 )
		 || ( byte_stream[ byte_stream_index + 1 ] != 0 ) )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
			 "%s: unsupported display name value in byte stream.",
			 function );

			goto on_error;
		}
		byte_stream_index += 2;
	}
	else
	{
		while( byte_stream_index < byte_stream_size )
		{
			if( byte_stream[ byte_stream_index ] == 0 )
			{
				break;
			}
			byte_stream_index += 1;
		}
		if( byte_stream[ byte_stream_index ] != 0 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
			 "%s: unsupported display name value in byte stream.",
			 function );

			return( -1 );
		}
		byte_stream_index += 1;
	}
	internal_entry_identifier->display_name = (uint8_t *) memory_allocate(
	                                                       sizeof( uint8_t ) * byte_stream_index );

	if( internal_entry_identifier->display_name == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create display name.",
		 function );

		goto on_error;
	}
	if( memory_copy(
	     internal_entry_identifier->display_name,
	     byte_stream,
	     byte_stream_index ) == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_COPY_FAILED,
		 "%s: unable to copy display name.",
		 function );

		goto on_error;
	}
	internal_entry_identifier->display_name_size = byte_stream_index;

	byte_stream      += byte_stream_index;
	byte_stream_size -= byte_stream_index;

	if( byte_stream_size == 0 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_TOO_SMALL,
		 "%s: byte stream too small.",
		 function );

		goto on_error;
	}
	byte_stream_index = 0;

	if( ( internal_entry_identifier->flags & LIBFMAPI_ONE_OFF_ENTRY_IDENTIFIER_FLAG_UNICODE ) != 0 )
	{
		while( ( byte_stream_index + 1 ) < byte_stream_size )
		{
			if( ( byte_stream[ byte_stream_index ] == 0 )
			 && ( byte_stream[ byte_stream_index + 1 ] == 0 ) )
			{
				break;
			}
			byte_stream_index += 2;
		}
		if( ( byte_stream[ byte_stream_index ] != 0 )
		 || ( byte_stream[ byte_stream_index + 1 ] != 0 ) )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
			 "%s: unsupported address type value in byte stream.",
			 function );

			goto on_error;
		}
		byte_stream_index += 2;
	}
	else
	{
		while( byte_stream_index < byte_stream_size )
		{
			if( byte_stream[ byte_stream_index ] == 0 )
			{
				break;
			}
			byte_stream_index += 1;
		}
		if( byte_stream[ byte_stream_index ] != 0 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
			 "%s: unsupported address type value in byte stream.",
			 function );

			goto on_error;
		}
		byte_stream_index += 1;
	}
	internal_entry_identifier->address_type = (uint8_t *) memory_allocate(
	                                                       sizeof( uint8_t ) * byte_stream_index );

	if( internal_entry_identifier->address_type == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create address type.",
		 function );

		goto on_error;
	}
	if( memory_copy(
	     internal_entry_identifier->address_type,
	     byte_stream,
	     byte_stream_index ) == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_COPY_FAILED,
		 "%s: unable to copy address type.",
		 function );

		goto on_error;
	}
	internal_entry_identifier->address_type_size = byte_stream_index;

	byte_stream      += byte_stream_index;
	byte_stream_size -= byte_stream_index;

	if( byte_stream_size == 0 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_TOO_SMALL,
		 "%s: byte stream too small.",
		 function );

		goto on_error;
	}
	byte_stream_index = 0;

	if( ( internal_entry_identifier->flags & LIBFMAPI_ONE_OFF_ENTRY_IDENTIFIER_FLAG_UNICODE ) != 0 )
	{
		while( ( byte_stream_index + 1 ) < byte_stream_size )
		{
			if( ( byte_stream[ byte_stream_index ] == 0 )
			 && ( byte_stream[ byte_stream_index + 1 ] == 0 ) )
			{
				break;
			}
			byte_stream_index += 2;
		}
		if( ( byte_stream[ byte_stream_index ] != 0 )
		 || ( byte_stream[ byte_stream_index + 1 ] != 0 ) )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
			 "%s: unsupported email address value in byte stream.",
			 function );

			goto on_error;
		}
		byte_stream_index += 2;
	}
	else
	{
		while( byte_stream_index < byte_stream_size )
		{
			if( byte_stream[ byte_stream_index ] == 0 )
			{
				break;
			}
			byte_stream_index += 1;
		}
		if( byte_stream[ byte_stream_index ] != 0 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
			 "%s: unsupported email address value in byte stream.",
			 function );

			goto on_error;
		}
		byte_stream_index += 1;
	}
	internal_entry_identifier->email_address = (uint8_t *) memory_allocate(
	                                                        sizeof( uint8_t ) * byte_stream_index );

	if( internal_entry_identifier->email_address == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create email address.",
		 function );

		goto on_error;
	}
	if( memory_copy(
	     internal_entry_identifier->email_address,
	     byte_stream,
	     byte_stream_index ) == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_COPY_FAILED,
		 "%s: unable to copy email address.",
		 function );

		goto on_error;
	}
	internal_entry_identifier->email_address_size = byte_stream_index;

	byte_stream      += byte_stream_index;
	byte_stream_size -= byte_stream_index;

#if defined( HAVE_DEBUG_OUTPUT )
	if( ( libcnotify_verbose != 0 )
	 && ( byte_stream_size > 0 ) )
	{
		libcnotify_printf(
		 "%s: trailing data:\n",
		 function );
		libcnotify_print_data(
		 byte_stream,
		 byte_stream_size,
		 0 );
	}
#endif
	return( 1 );

on_error:
	if( internal_entry_identifier->email_address != NULL )
	{
		memory_free(
		 internal_entry_identifier->email_address );

		internal_entry_identifier->email_address = NULL;
	}
	if( internal_entry_identifier->address_type != NULL )
	{
		memory_free(
		 internal_entry_identifier->address_type );

		internal_entry_identifier->address_type = NULL;
	}
	if( internal_entry_identifier->display_name != NULL )
	{
		memory_free(
		 internal_entry_identifier->display_name );

		internal_entry_identifier->display_name = NULL;
	}
	return( -1 );
}

/* Retrieves the UTF-8 string size of the one-off entry identifier display name
 * The returned size includes the end of string character
 * Returns 1 if successful or -1 on error
 */
int libfmapi_one_off_entry_identifier_get_utf8_display_name_size(
     libfmapi_one_off_entry_identifier_t *entry_identifier,
     size_t *utf8_string_size,
     int ascii_codepage,
     libcerror_error_t **error )
{
	libfmapi_internal_one_off_entry_identifier_t *internal_entry_identifier = NULL;
	static char *function                                                   = "libfmapi_one_off_entry_identifier_get_utf8_display_name_size";
	int result                                                              = 0;

	if( entry_identifier == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid entry identifier.",
		 function );

		return( -1 );
	}
	internal_entry_identifier = (libfmapi_internal_one_off_entry_identifier_t *) entry_identifier;

	if( ( internal_entry_identifier->flags & LIBFMAPI_ONE_OFF_ENTRY_IDENTIFIER_FLAG_UNICODE ) != 0 )
	{
		result = libuna_utf8_string_size_from_utf16_stream(
			  internal_entry_identifier->display_name,
			  internal_entry_identifier->display_name_size,
			  LIBUNA_ENDIAN_LITTLE,
			  utf8_string_size,
			  error );
	}
	else
	{
		result = libuna_utf8_string_size_from_byte_stream(
			  internal_entry_identifier->display_name,
			  internal_entry_identifier->display_name_size,
			  ascii_codepage,
			  utf8_string_size,
			  error );
	}
	if( result != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve UTF-8 string size.",
		 function );

		return( -1 );
	}
	return( 1 );
}

/* Retrieves the UTF-8 string of the one-off entry identifier display name
 * The size should include the end of string character
 * Returns 1 if successful or -1 on error
 */
int libfmapi_one_off_entry_identifier_get_utf8_display_name(
     libfmapi_one_off_entry_identifier_t *entry_identifier,
     uint8_t *utf8_string,
     size_t utf8_string_size,
     int ascii_codepage,
     libcerror_error_t **error )
{
	libfmapi_internal_one_off_entry_identifier_t *internal_entry_identifier = NULL;
	static char *function                                                   = "libfmapi_one_off_entry_identifier_get_utf8_display_name";
	int result                                                              = 0;

	if( entry_identifier == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid entry identifier.",
		 function );

		return( -1 );
	}
	internal_entry_identifier = (libfmapi_internal_one_off_entry_identifier_t *) entry_identifier;

	if( ( internal_entry_identifier->flags & LIBFMAPI_ONE_OFF_ENTRY_IDENTIFIER_FLAG_UNICODE ) != 0 )
	{
		result = libuna_utf8_string_copy_from_utf16_stream(
			  utf8_string,
			  utf8_string_size,
			  internal_entry_identifier->display_name,
			  internal_entry_identifier->display_name_size,
			  LIBUNA_ENDIAN_LITTLE,
			  error );
	}
	else
	{
		result = libuna_utf8_string_copy_from_byte_stream(
			  utf8_string,
			  utf8_string_size,
			  internal_entry_identifier->display_name,
			  internal_entry_identifier->display_name_size,
			  ascii_codepage,
			  error );
	}
	if( result != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to set UTF-8 string.",
		 function );

		return( -1 );
	}
	return( 1 );
}

/* Retrieves the UTF-16 string size of the one-off entry identifier display name
 * The returned size includes the end of string character
 * Returns 1 if successful or -1 on error
 */
int libfmapi_one_off_entry_identifier_get_utf16_display_name_size(
     libfmapi_one_off_entry_identifier_t *entry_identifier,
     size_t *utf16_string_size,
     int ascii_codepage,
     libcerror_error_t **error )
{
	libfmapi_internal_one_off_entry_identifier_t *internal_entry_identifier = NULL;
	static char *function                                                   = "libfmapi_one_off_entry_identifier_get_utf16_display_name_size";
	int result                                                              = 0;

	if( entry_identifier == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid entry identifier.",
		 function );

		return( -1 );
	}
	internal_entry_identifier = (libfmapi_internal_one_off_entry_identifier_t *) entry_identifier;

	if( ( internal_entry_identifier->flags & LIBFMAPI_ONE_OFF_ENTRY_IDENTIFIER_FLAG_UNICODE ) != 0 )
	{
		result = libuna_utf16_string_size_from_utf16_stream(
			  internal_entry_identifier->display_name,
			  internal_entry_identifier->display_name_size,
			  LIBUNA_ENDIAN_LITTLE,
			  utf16_string_size,
			  error );
	}
	else
	{
		result = libuna_utf16_string_size_from_byte_stream(
			  internal_entry_identifier->display_name,
			  internal_entry_identifier->display_name_size,
			  ascii_codepage,
			  utf16_string_size,
			  error );
	}
	if( result != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve UTF-16 string size.",
		 function );

		return( -1 );
	}
	return( 1 );
}

/* Retrieves the UTF-16 string of the one-off entry identifier display name
 * The size should include the end of string character
 * Returns 1 if successful or -1 on error
 */
int libfmapi_one_off_entry_identifier_get_utf16_display_name(
     libfmapi_one_off_entry_identifier_t *entry_identifier,
     uint16_t *utf16_string,
     size_t utf16_string_size,
     int ascii_codepage,
     libcerror_error_t **error )
{
	libfmapi_internal_one_off_entry_identifier_t *internal_entry_identifier = NULL;
	static char *function                                                   = "libfmapi_one_off_entry_identifier_get_utf16_display_name";
	int result                                                              = 0;

	if( entry_identifier == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid entry identifier.",
		 function );

		return( -1 );
	}
	internal_entry_identifier = (libfmapi_internal_one_off_entry_identifier_t *) entry_identifier;

	if( ( internal_entry_identifier->flags & LIBFMAPI_ONE_OFF_ENTRY_IDENTIFIER_FLAG_UNICODE ) != 0 )
	{
		result = libuna_utf16_string_copy_from_utf16_stream(
			  utf16_string,
			  utf16_string_size,
			  internal_entry_identifier->display_name,
			  internal_entry_identifier->display_name_size,
			  LIBUNA_ENDIAN_LITTLE,
			  error );
	}
	else
	{
		result = libuna_utf16_string_copy_from_byte_stream(
			  utf16_string,
			  utf16_string_size,
			  internal_entry_identifier->display_name,
			  internal_entry_identifier->display_name_size,
			  ascii_codepage,
			  error );
	}
	if( result != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to set UTF-16 string.",
		 function );

		return( -1 );
	}
	return( 1 );
}

/* Retrieves the UTF-8 string size of the one-off entry identifier address type
 * The returned size includes the end of string character
 * Returns 1 if successful or -1 on error
 */
int libfmapi_one_off_entry_identifier_get_utf8_address_type_size(
     libfmapi_one_off_entry_identifier_t *entry_identifier,
     size_t *utf8_string_size,
     int ascii_codepage,
     libcerror_error_t **error )
{
	libfmapi_internal_one_off_entry_identifier_t *internal_entry_identifier = NULL;
	static char *function                                                   = "libfmapi_one_off_entry_identifier_get_utf8_address_type_size";
	int result                                                              = 0;

	if( entry_identifier == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid entry identifier.",
		 function );

		return( -1 );
	}
	internal_entry_identifier = (libfmapi_internal_one_off_entry_identifier_t *) entry_identifier;

	if( ( internal_entry_identifier->flags & LIBFMAPI_ONE_OFF_ENTRY_IDENTIFIER_FLAG_UNICODE ) != 0 )
	{
		result = libuna_utf8_string_size_from_utf16_stream(
			  internal_entry_identifier->address_type,
			  internal_entry_identifier->address_type_size,
			  LIBUNA_ENDIAN_LITTLE,
			  utf8_string_size,
			  error );
	}
	else
	{
		result = libuna_utf8_string_size_from_byte_stream(
			  internal_entry_identifier->address_type,
			  internal_entry_identifier->address_type_size,
			  ascii_codepage,
			  utf8_string_size,
			  error );
	}
	if( result != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve UTF-8 string size.",
		 function );

		return( -1 );
	}
	return( 1 );
}

/* Retrieves the UTF-8 string of the one-off entry identifier address type
 * The size should include the end of string character
 * Returns 1 if successful or -1 on error
 */
int libfmapi_one_off_entry_identifier_get_utf8_address_type(
     libfmapi_one_off_entry_identifier_t *entry_identifier,
     uint8_t *utf8_string,
     size_t utf8_string_size,
     int ascii_codepage,
     libcerror_error_t **error )
{
	libfmapi_internal_one_off_entry_identifier_t *internal_entry_identifier = NULL;
	static char *function                                                   = "libfmapi_one_off_entry_identifier_get_utf8_address_type";
	int result                                                              = 0;

	if( entry_identifier == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid entry identifier.",
		 function );

		return( -1 );
	}
	internal_entry_identifier = (libfmapi_internal_one_off_entry_identifier_t *) entry_identifier;

	if( ( internal_entry_identifier->flags & LIBFMAPI_ONE_OFF_ENTRY_IDENTIFIER_FLAG_UNICODE ) != 0 )
	{
		result = libuna_utf8_string_copy_from_utf16_stream(
			  utf8_string,
			  utf8_string_size,
			  internal_entry_identifier->address_type,
			  internal_entry_identifier->address_type_size,
			  LIBUNA_ENDIAN_LITTLE,
			  error );
	}
	else
	{
		result = libuna_utf8_string_copy_from_byte_stream(
			  utf8_string,
			  utf8_string_size,
			  internal_entry_identifier->address_type,
			  internal_entry_identifier->address_type_size,
			  ascii_codepage,
			  error );
	}
	if( result != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to set UTF-8 string.",
		 function );

		return( -1 );
	}
	return( 1 );
}

/* Retrieves the UTF-16 string size of the one-off entry identifier address type
 * The returned size includes the end of string character
 * Returns 1 if successful or -1 on error
 */
int libfmapi_one_off_entry_identifier_get_utf16_address_type_size(
     libfmapi_one_off_entry_identifier_t *entry_identifier,
     size_t *utf16_string_size,
     int ascii_codepage,
     libcerror_error_t **error )
{
	libfmapi_internal_one_off_entry_identifier_t *internal_entry_identifier = NULL;
	static char *function                                                   = "libfmapi_one_off_entry_identifier_get_utf16_address_type_size";
	int result                                                              = 0;

	if( entry_identifier == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid entry identifier.",
		 function );

		return( -1 );
	}
	internal_entry_identifier = (libfmapi_internal_one_off_entry_identifier_t *) entry_identifier;

	if( ( internal_entry_identifier->flags & LIBFMAPI_ONE_OFF_ENTRY_IDENTIFIER_FLAG_UNICODE ) != 0 )
	{
		result = libuna_utf16_string_size_from_utf16_stream(
			  internal_entry_identifier->address_type,
			  internal_entry_identifier->address_type_size,
			  LIBUNA_ENDIAN_LITTLE,
			  utf16_string_size,
			  error );
	}
	else
	{
		result = libuna_utf16_string_size_from_byte_stream(
			  internal_entry_identifier->address_type,
			  internal_entry_identifier->address_type_size,
			  ascii_codepage,
			  utf16_string_size,
			  error );
	}
	if( result != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve UTF-16 string size.",
		 function );

		return( -1 );
	}
	return( 1 );
}

/* Retrieves the UTF-16 string of the one-off entry identifier address type
 * The size should include the end of string character
 * Returns 1 if successful or -1 on error
 */
int libfmapi_one_off_entry_identifier_get_utf16_address_type(
     libfmapi_one_off_entry_identifier_t *entry_identifier,
     uint16_t *utf16_string,
     size_t utf16_string_size,
     int ascii_codepage,
     libcerror_error_t **error )
{
	libfmapi_internal_one_off_entry_identifier_t *internal_entry_identifier = NULL;
	static char *function                                                   = "libfmapi_one_off_entry_identifier_get_utf16_address_type";
	int result                                                              = 0;

	if( entry_identifier == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid entry identifier.",
		 function );

		return( -1 );
	}
	internal_entry_identifier = (libfmapi_internal_one_off_entry_identifier_t *) entry_identifier;

	if( ( internal_entry_identifier->flags & LIBFMAPI_ONE_OFF_ENTRY_IDENTIFIER_FLAG_UNICODE ) != 0 )
	{
		result = libuna_utf16_string_copy_from_utf16_stream(
			  utf16_string,
			  utf16_string_size,
			  internal_entry_identifier->address_type,
			  internal_entry_identifier->address_type_size,
			  LIBUNA_ENDIAN_LITTLE,
			  error );
	}
	else
	{
		result = libuna_utf16_string_copy_from_byte_stream(
			  utf16_string,
			  utf16_string_size,
			  internal_entry_identifier->address_type,
			  internal_entry_identifier->address_type_size,
			  ascii_codepage,
			  error );
	}
	if( result != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to set UTF-16 string.",
		 function );

		return( -1 );
	}
	return( 1 );
}

/* Retrieves the UTF-8 string size of the one-off entry identifier email address
 * The returned size includes the end of string character
 * Returns 1 if successful or -1 on error
 */
int libfmapi_one_off_entry_identifier_get_utf8_email_address_size(
     libfmapi_one_off_entry_identifier_t *entry_identifier,
     size_t *utf8_string_size,
     int ascii_codepage,
     libcerror_error_t **error )
{
	libfmapi_internal_one_off_entry_identifier_t *internal_entry_identifier = NULL;
	static char *function                                                   = "libfmapi_one_off_entry_identifier_get_utf8_email_address_size";
	int result                                                              = 0;

	if( entry_identifier == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid entry identifier.",
		 function );

		return( -1 );
	}
	internal_entry_identifier = (libfmapi_internal_one_off_entry_identifier_t *) entry_identifier;

	if( ( internal_entry_identifier->flags & LIBFMAPI_ONE_OFF_ENTRY_IDENTIFIER_FLAG_UNICODE ) != 0 )
	{
		result = libuna_utf8_string_size_from_utf16_stream(
			  internal_entry_identifier->email_address,
			  internal_entry_identifier->email_address_size,
			  LIBUNA_ENDIAN_LITTLE,
			  utf8_string_size,
			  error );
	}
	else
	{
		result = libuna_utf8_string_size_from_byte_stream(
			  internal_entry_identifier->email_address,
			  internal_entry_identifier->email_address_size,
			  ascii_codepage,
			  utf8_string_size,
			  error );
	}
	if( result != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve UTF-8 string size.",
		 function );

		return( -1 );
	}
	return( 1 );
}

/* Retrieves the UTF-8 string of the one-off entry identifier email address
 * The size should include the end of string character
 * Returns 1 if successful or -1 on error
 */
int libfmapi_one_off_entry_identifier_get_utf8_email_address(
     libfmapi_one_off_entry_identifier_t *entry_identifier,
     uint8_t *utf8_string,
     size_t utf8_string_size,
     int ascii_codepage,
     libcerror_error_t **error )
{
	libfmapi_internal_one_off_entry_identifier_t *internal_entry_identifier = NULL;
	static char *function                                                   = "libfmapi_one_off_entry_identifier_get_utf8_email_address";
	int result                                                              = 0;

	if( entry_identifier == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid entry identifier.",
		 function );

		return( -1 );
	}
	internal_entry_identifier = (libfmapi_internal_one_off_entry_identifier_t *) entry_identifier;

	if( ( internal_entry_identifier->flags & LIBFMAPI_ONE_OFF_ENTRY_IDENTIFIER_FLAG_UNICODE ) != 0 )
	{
		result = libuna_utf8_string_copy_from_utf16_stream(
			  utf8_string,
			  utf8_string_size,
			  internal_entry_identifier->email_address,
			  internal_entry_identifier->email_address_size,
			  LIBUNA_ENDIAN_LITTLE,
			  error );
	}
	else
	{
		result = libuna_utf8_string_copy_from_byte_stream(
			  utf8_string,
			  utf8_string_size,
			  internal_entry_identifier->email_address,
			  internal_entry_identifier->email_address_size,
			  ascii_codepage,
			  error );
	}
	if( result != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to set UTF-8 string.",
		 function );

		return( -1 );
	}
	return( 1 );
}

/* Retrieves the UTF-16 string size of the one-off entry identifier email address
 * The returned size includes the end of string character
 * Returns 1 if successful or -1 on error
 */
int libfmapi_one_off_entry_identifier_get_utf16_email_address_size(
     libfmapi_one_off_entry_identifier_t *entry_identifier,
     size_t *utf16_string_size,
     int ascii_codepage,
     libcerror_error_t **error )
{
	libfmapi_internal_one_off_entry_identifier_t *internal_entry_identifier = NULL;
	static char *function                                                   = "libfmapi_one_off_entry_identifier_get_utf16_email_address_size";
	int result                                                              = 0;

	if( entry_identifier == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid entry identifier.",
		 function );

		return( -1 );
	}
	internal_entry_identifier = (libfmapi_internal_one_off_entry_identifier_t *) entry_identifier;

	if( ( internal_entry_identifier->flags & LIBFMAPI_ONE_OFF_ENTRY_IDENTIFIER_FLAG_UNICODE ) != 0 )
	{
		result = libuna_utf16_string_size_from_utf16_stream(
			  internal_entry_identifier->email_address,
			  internal_entry_identifier->email_address_size,
			  LIBUNA_ENDIAN_LITTLE,
			  utf16_string_size,
			  error );
	}
	else
	{
		result = libuna_utf16_string_size_from_byte_stream(
			  internal_entry_identifier->email_address,
			  internal_entry_identifier->email_address_size,
			  ascii_codepage,
			  utf16_string_size,
			  error );
	}
	if( result != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve UTF-16 string size.",
		 function );

		return( -1 );
	}
	return( 1 );
}

/* Retrieves the UTF-16 string of the one-off entry identifier email address
 * The size should include the end of string character
 * Returns 1 if successful or -1 on error
 */
int libfmapi_one_off_entry_identifier_get_utf16_email_address(
     libfmapi_one_off_entry_identifier_t *entry_identifier,
     uint16_t *utf16_string,
     size_t utf16_string_size,
     int ascii_codepage,
     libcerror_error_t **error )
{
	libfmapi_internal_one_off_entry_identifier_t *internal_entry_identifier = NULL;
	static char *function                                                   = "libfmapi_one_off_entry_identifier_get_utf16_email_address";
	int result                                                              = 0;

	if( entry_identifier == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid entry identifier.",
		 function );

		return( -1 );
	}
	internal_entry_identifier = (libfmapi_internal_one_off_entry_identifier_t *) entry_identifier;

	if( ( internal_entry_identifier->flags & LIBFMAPI_ONE_OFF_ENTRY_IDENTIFIER_FLAG_UNICODE ) != 0 )
	{
		result = libuna_utf16_string_copy_from_utf16_stream(
			  utf16_string,
			  utf16_string_size,
			  internal_entry_identifier->email_address,
			  internal_entry_identifier->email_address_size,
			  LIBUNA_ENDIAN_LITTLE,
			  error );
	}
	else
	{
		result = libuna_utf16_string_copy_from_byte_stream(
			  utf16_string,
			  utf16_string_size,
			  internal_entry_identifier->email_address,
			  internal_entry_identifier->email_address_size,
			  ascii_codepage,
			  error );
	}
	if( result != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to set UTF-16 string.",
		 function );

		return( -1 );
	}
	return( 1 );
}

