/*
 * Binary data value functions
 *
 * Copyright (c) 2010-2012, Joachim Metz <joachim.metz@gmail.com>
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

#include "libfvalue_binary_data.h"
#include "libfvalue_definitions.h"
#include "libfvalue_libcerror.h"
#include "libfvalue_libuna.h"

/* Initialize a binary data
 * Returns 1 if successful or -1 on error
 */
int libfvalue_binary_data_initialize(
     libfvalue_binary_data_t **binary_data,
     libcerror_error_t **error )
{
	static char *function = "libfvalue_binary_data_initialize";

	if( binary_data == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid binary data.",
		 function );

		return( -1 );
	}
	if( *binary_data != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid binary data value already set.",
		 function );

		return( -1 );
	}
	*binary_data = memory_allocate_structure(
	                libfvalue_binary_data_t );

	if( *binary_data == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create binary data.",
		 function );

		goto on_error;
	}
	if( memory_set(
	     *binary_data,
	     0,
	     sizeof( libfvalue_binary_data_t ) ) == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_SET_FAILED,
		 "%s: unable to clear binary data.",
		 function );

		goto on_error;
	}
	return( 1 );

on_error:
	if( *binary_data != NULL )
	{
		memory_free(
		 *binary_data );

		*binary_data = NULL;
	}
	return( -1 );
}

/* Frees a binary data
 * Returns 1 if successful or -1 on error
 */
int libfvalue_binary_data_free(
     libfvalue_binary_data_t **binary_data,
     libcerror_error_t **error )
{
	static char *function = "libfvalue_binary_data_free";

	if( binary_data == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid binary data.",
		 function );

		return( -1 );
	}
	if( *binary_data != NULL )
	{
		memory_free(
		 *binary_data );

		*binary_data = NULL;
	}
	return( 1 );
}

/* Clones a binary data
 * Returns 1 if successful or -1 on error
 */
int libfvalue_binary_data_clone(
     libfvalue_binary_data_t **destination_binary_data,
     libfvalue_binary_data_t *source_binary_data,
     libcerror_error_t **error )
{
	static char *function = "libfvalue_binary_data_clone";

	if( destination_binary_data == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid destination binary data.",
		 function );

		return( -1 );
	}
	if( *destination_binary_data != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: destination binary data already set.",
		 function );

		return( -1 );
	}
	if( source_binary_data == NULL )
	{
		*destination_binary_data = NULL;

		return( 1 );
	}
	*destination_binary_data = memory_allocate_structure(
	                            libfvalue_binary_data_t );

	if( *destination_binary_data == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create destination binary data.",
		 function );

		goto on_error;
	}
	if( memory_copy(
	     *destination_binary_data,
	     source_binary_data,
	     sizeof( libfvalue_binary_data_t ) ) == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_COPY_FAILED,
		 "%s: unable to copy binary data.",
		 function );

		goto on_error;
	}
	return( 1 );

on_error:
	if( *destination_binary_data != NULL )
	{
		memory_free(
		 *destination_binary_data );

		*destination_binary_data = NULL;
	}
	return( -1 );
}

/* Copies the binary data from a byte stream
 * Returns 1 if successful or -1 on error
 */
int libfvalue_binary_data_copy_from_byte_stream(
     libfvalue_binary_data_t *binary_data,
     const uint8_t *byte_stream,
     size_t byte_stream_size,
     int encoding,
     libcerror_error_t **error )
{
	static char *function = "libfvalue_binary_data_copy_from_byte_stream";

	if( binary_data == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid binary data.",
		 function );

		return( -1 );
	}
	if( encoding != 0 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_UNSUPPORTED_VALUE,
		 "%s: unsupported encoding.",
		 function );

		return( -1 );
	}
	binary_data->data      = byte_stream;
	binary_data->data_size = byte_stream_size;

	return( 1 );
}

/* Retrieves the size of a string of the binary data
 * Returns 1 if successful or -1 on error
 */
int libfvalue_binary_data_get_string_size(
     libfvalue_binary_data_t *binary_data,
     size_t *string_size,
     uint32_t string_format_flags,
     libcerror_error_t **error )
{
	static char *function       = "libfvalue_binary_data_get_string_size";
	uint32_t format_flags       = 0;
	uint32_t string_format_type = 0;
	uint32_t supported_flags    = 0;

	if( binary_data == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid binary data.",
		 function );

		return( -1 );
	}
	supported_flags = 0x000000ffUL
	                | LIBFVALUE_BINARY_DATA_FORMAT_FLAG_CASE_LOWER
	                | LIBFVALUE_BINARY_DATA_FORMAT_FLAG_CASE_UPPER;

	if( ( string_format_flags & ~( supported_flags ) ) != 0 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_UNSUPPORTED_VALUE,
		 "%s: unsupported string format flags: 0x%08" PRIx32 ".",
		 function,
		 string_format_flags );

		return( -1 );
	}
	string_format_type = string_format_flags & 0x000000ffUL;

	if( ( string_format_type != LIBFVALUE_BINARY_DATA_FORMAT_TYPE_BASE16 )
	 && ( string_format_type != LIBFVALUE_BINARY_DATA_FORMAT_TYPE_BASE32 ) 
	 && ( string_format_type != LIBFVALUE_BINARY_DATA_FORMAT_TYPE_BASE64 ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_UNSUPPORTED_VALUE,
		 "%s: unsupported string format type.",
		 function );

		return( -1 );
	}
	if( ( binary_data->data == NULL )
	 || ( binary_data->data_size == 0 ) )
	{
		if( string_size == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
			 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
			 "%s: invalid string size.",
			 function );

			return( -1 );
		}
		*string_size = 1;
	}
	else switch( string_format_type )
	{
		case LIBFVALUE_BINARY_DATA_FORMAT_TYPE_BASE16:
			format_flags = LIBUNA_BASE16_VARIANT_CASE_UPPER
				     | LIBUNA_BASE16_VARIANT_CHARACTER_LIMIT_NONE;

			if( libuna_base16_stream_size_from_byte_stream(
			     binary_data->data,
			     binary_data->data_size,
			     string_size,
			     format_flags,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
				 "%s: unable to determine size of string of base16 formatted binary data.",
				 function );

				return( -1 );
			}
			break;

		case LIBFVALUE_BINARY_DATA_FORMAT_TYPE_BASE32:
			format_flags = LIBUNA_BASE32_VARIANT_ALPHABET_NORMAL
			             | LIBUNA_BASE32_VARIANT_CHARACTER_LIMIT_NONE
			             | LIBUNA_BASE32_VARIANT_PADDING_REQUIRED;

			if( libuna_base32_stream_size_from_byte_stream(
			     binary_data->data,
			     binary_data->data_size,
			     string_size,
			     format_flags,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
				 "%s: unable to determine size of string of base32 formatted binary data.",
				 function );

				return( -1 );
			}
			break;

		case LIBFVALUE_BINARY_DATA_FORMAT_TYPE_BASE64:
			format_flags = LIBUNA_BASE64_VARIANT_ALPHABET_NORMAL
			             | LIBUNA_BASE64_VARIANT_CHARACTER_LIMIT_NONE
			             | LIBUNA_BASE64_VARIANT_PADDING_REQUIRED;

			if( libuna_base64_stream_size_from_byte_stream(
			     binary_data->data,
			     binary_data->data_size,
			     string_size,
			     format_flags,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
				 "%s: unable to determine size of string of base64 formatted binary data.",
				 function );

				return( -1 );
			}
			break;
	}
	return( 1 );
}

/* Copies the binary data to an UTF-8 encoded string
 * Returns 1 if successful or -1 on error
 */
int libfvalue_binary_data_copy_to_utf8_string_with_index(
     libfvalue_binary_data_t *binary_data,
     uint8_t *utf8_string,
     size_t utf8_string_size,
     size_t *utf8_string_index,
     uint32_t string_format_flags,
     libcerror_error_t **error )
{
	static char *function       = "libfvalue_binary_data_copy_to_utf8_string_with_index";
	uint32_t format_flags       = 0;
	uint32_t string_format_type = 0;
	uint32_t supported_flags    = 0;

	if( binary_data == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid binary data.",
		 function );

		return( -1 );
	}
	supported_flags = 0x000000ffUL
	                | LIBFVALUE_BINARY_DATA_FORMAT_FLAG_CASE_LOWER
	                | LIBFVALUE_BINARY_DATA_FORMAT_FLAG_CASE_UPPER;

	if( ( string_format_flags & ~( supported_flags ) ) != 0 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_UNSUPPORTED_VALUE,
		 "%s: unsupported string format flags: 0x%08" PRIx32 ".",
		 function,
		 string_format_flags );

		return( -1 );
	}
	string_format_type = string_format_flags & 0x000000ffUL;

	if( ( string_format_type != LIBFVALUE_BINARY_DATA_FORMAT_TYPE_BASE16 )
	 && ( string_format_type != LIBFVALUE_BINARY_DATA_FORMAT_TYPE_BASE32 ) 
	 && ( string_format_type != LIBFVALUE_BINARY_DATA_FORMAT_TYPE_BASE64 ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_UNSUPPORTED_VALUE,
		 "%s: unsupported string format type.",
		 function );

		return( -1 );
	}
	if( ( binary_data->data == NULL )
	 || ( binary_data->data_size == 0 ) )
	{
		if( utf8_string == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
			 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
			 "%s: invalid UTF-8 string.",
			 function );

			return( -1 );
		}
		if( utf8_string_size > (size_t) SSIZE_MAX )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
			 LIBCERROR_ARGUMENT_ERROR_VALUE_EXCEEDS_MAXIMUM,
			 "%s: invalid UTF-8 string size value exceeds maximum.",
			 function );

			return( -1 );
		}
		if( utf8_string_index == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
			 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
			 "%s: invalid UTF-8 string index.",
			 function );

			return( -1 );
		}
		if( *utf8_string_index >= utf8_string_size )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
			 LIBCERROR_ARGUMENT_ERROR_VALUE_TOO_SMALL,
			 "%s: UTF-8 string is too small.",
			 function );

			return( -1 );
		}
		utf8_string[ *utf8_string_index ] = 0;

		*utf8_string_index += 1;
	}
	else switch( string_format_type )
	{
		case LIBFVALUE_BINARY_DATA_FORMAT_TYPE_BASE16:
			format_flags = LIBUNA_BASE16_VARIANT_CASE_UPPER
				     | LIBUNA_BASE16_VARIANT_CHARACTER_LIMIT_NONE;

			if( libuna_base16_stream_with_index_copy_from_byte_stream(
			     utf8_string,
			     utf8_string_size,
			     utf8_string_index,
			     binary_data->data,
			     binary_data->data_size,
			     format_flags,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_COPY_FAILED,
				 "%s: unable to copy base16 formatted binary data to UTF-8 string.",
				 function );

				return( -1 );
			}
			break;

		case LIBFVALUE_BINARY_DATA_FORMAT_TYPE_BASE32:
			format_flags = LIBUNA_BASE32_VARIANT_ALPHABET_NORMAL
			             | LIBUNA_BASE32_VARIANT_CHARACTER_LIMIT_NONE
			             | LIBUNA_BASE32_VARIANT_PADDING_REQUIRED;

			if( libuna_base32_stream_with_index_copy_from_byte_stream(
			     utf8_string,
			     utf8_string_size,
			     utf8_string_index,
			     binary_data->data,
			     binary_data->data_size,
			     format_flags,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_COPY_FAILED,
				 "%s: unable to copy base32 formatted binary data to UTF-8 string.",
				 function );

				return( -1 );
			}
			break;

		case LIBFVALUE_BINARY_DATA_FORMAT_TYPE_BASE64:
			format_flags = LIBUNA_BASE64_VARIANT_ALPHABET_NORMAL
			             | LIBUNA_BASE64_VARIANT_CHARACTER_LIMIT_NONE
			             | LIBUNA_BASE64_VARIANT_PADDING_REQUIRED;

			if( libuna_base64_stream_with_index_copy_from_byte_stream(
			     utf8_string,
			     utf8_string_size,
			     utf8_string_index,
			     binary_data->data,
			     binary_data->data_size,
			     format_flags,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_COPY_FAILED,
				 "%s: unable to copy base64 formatted binary data to UTF-8 string.",
				 function );

				return( -1 );
			}
			break;
	}
	return( 1 );
}

/* Copies the binary data to an UTF-16 encoded string
 * Returns 1 if successful or -1 on error
 */
int libfvalue_binary_data_copy_to_utf16_string_with_index(
     libfvalue_binary_data_t *binary_data,
     uint16_t *utf16_string,
     size_t utf16_string_size,
     size_t *utf16_string_index,
     uint32_t string_format_flags,
     libcerror_error_t **error )
{
	static char *function       = "libfvalue_binary_data_copy_to_utf16_string_with_index";
	size_t string_index         = 0;
	uint32_t format_flags       = 0;
	uint32_t string_format_type = 0;
	uint32_t supported_flags    = 0;

	if( binary_data == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid binary data.",
		 function );

		return( -1 );
	}
	if( utf16_string_size > (size_t) SSIZE_MAX )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_EXCEEDS_MAXIMUM,
		 "%s: invalid UTF-16 string size value exceeds maximum.",
		 function );

		return( -1 );
	}
	if( utf16_string_index == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid UTF-16 string index.",
		 function );

		return( -1 );
	}
	if( *utf16_string_index > utf16_string_size )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_TOO_SMALL,
		 "%s: UTF-16 string is too small.",
		 function );

		return( -1 );
	}
	supported_flags = 0x000000ffUL
	                | LIBFVALUE_BINARY_DATA_FORMAT_FLAG_CASE_LOWER
	                | LIBFVALUE_BINARY_DATA_FORMAT_FLAG_CASE_UPPER;

	if( ( string_format_flags & ~( supported_flags ) ) != 0 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_UNSUPPORTED_VALUE,
		 "%s: unsupported string format flags: 0x%08" PRIx32 ".",
		 function,
		 string_format_flags );

		return( -1 );
	}
	string_format_type = string_format_flags & 0x000000ffUL;

	if( ( string_format_type != LIBFVALUE_BINARY_DATA_FORMAT_TYPE_BASE16 )
	 && ( string_format_type != LIBFVALUE_BINARY_DATA_FORMAT_TYPE_BASE32 ) 
	 && ( string_format_type != LIBFVALUE_BINARY_DATA_FORMAT_TYPE_BASE64 ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_UNSUPPORTED_VALUE,
		 "%s: unsupported string format type.",
		 function );

		return( -1 );
	}
	if( ( binary_data->data == NULL )
	 || ( binary_data->data_size == 0 ) )
	{
		if( utf16_string == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
			 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
			 "%s: invalid UTF-16 string.",
			 function );

			return( -1 );
		}
		if( utf16_string_size > (size_t) SSIZE_MAX )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
			 LIBCERROR_ARGUMENT_ERROR_VALUE_EXCEEDS_MAXIMUM,
			 "%s: invalid UTF-16 string size value exceeds maximum.",
			 function );

			return( -1 );
		}
		if( utf16_string_index == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
			 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
			 "%s: invalid UTF-16 string index.",
			 function );

			return( -1 );
		}
		if( *utf16_string_index >= utf16_string_size )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
			 LIBCERROR_ARGUMENT_ERROR_VALUE_TOO_SMALL,
			 "%s: UTF-16 string is too small.",
			 function );

			return( -1 );
		}
		utf16_string[ *utf16_string_index ] = 0;

		*utf16_string_index += 1;
	}
	else switch( string_format_type )
	{
		case LIBFVALUE_BINARY_DATA_FORMAT_TYPE_BASE16:
			format_flags = LIBUNA_BASE16_VARIANT_CASE_UPPER
				     | LIBUNA_BASE16_VARIANT_CHARACTER_LIMIT_NONE;

			if( _BYTE_STREAM_HOST_IS_ENDIAN_BIG )
			{
				format_flags |= LIBUNA_BASE16_VARIANT_ENCODING_UTF32_BIG_ENDIAN;
			}
			else if( _BYTE_STREAM_HOST_IS_ENDIAN_LITTLE )
			{
				format_flags |= LIBUNA_BASE16_VARIANT_ENCODING_UTF32_LITTLE_ENDIAN;
			}
			else
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
				 LIBCERROR_ARGUMENT_ERROR_UNSUPPORTED_VALUE,
				 "%s: unsupported host byte order.",
				 function );

				return( -1 );
			}
			string_index = *utf16_string_index * sizeof( uint16_t );

			if( libuna_base16_stream_with_index_copy_from_byte_stream(
			     (uint8_t *) utf16_string,
			     utf16_string_size,
			     &string_index,
			     binary_data->data,
			     binary_data->data_size,
			     format_flags,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_COPY_FAILED,
				 "%s: unable to copy base16 formatted binary data to UTF-16 string.",
				 function );

				return( -1 );
			}
			*utf16_string_index = string_index / sizeof( uint16_t );

			break;

		case LIBFVALUE_BINARY_DATA_FORMAT_TYPE_BASE32:
			format_flags = LIBUNA_BASE32_VARIANT_ALPHABET_NORMAL
			             | LIBUNA_BASE32_VARIANT_CHARACTER_LIMIT_NONE
			             | LIBUNA_BASE32_VARIANT_PADDING_REQUIRED;

			if( _BYTE_STREAM_HOST_IS_ENDIAN_BIG )
			{
				format_flags |= LIBUNA_BASE32_VARIANT_ENCODING_UTF32_BIG_ENDIAN;
			}
			else if( _BYTE_STREAM_HOST_IS_ENDIAN_LITTLE )
			{
				format_flags |= LIBUNA_BASE32_VARIANT_ENCODING_UTF32_LITTLE_ENDIAN;
			}
			else
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
				 LIBCERROR_ARGUMENT_ERROR_UNSUPPORTED_VALUE,
				 "%s: unsupported host byte order.",
				 function );

				return( -1 );
			}
			string_index = *utf16_string_index * sizeof( uint16_t );

			if( libuna_base32_stream_with_index_copy_from_byte_stream(
			     (uint8_t *) utf16_string,
			     utf16_string_size,
			     &string_index,
			     binary_data->data,
			     binary_data->data_size,
			     format_flags,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_COPY_FAILED,
				 "%s: unable to copy base32 formatted binary data to UTF-16 string.",
				 function );

				return( -1 );
			}
			*utf16_string_index = string_index / sizeof( uint16_t );

			break;

		case LIBFVALUE_BINARY_DATA_FORMAT_TYPE_BASE64:
			format_flags = LIBUNA_BASE64_VARIANT_ALPHABET_NORMAL
			             | LIBUNA_BASE64_VARIANT_CHARACTER_LIMIT_NONE
			             | LIBUNA_BASE64_VARIANT_PADDING_REQUIRED;

			if( _BYTE_STREAM_HOST_IS_ENDIAN_BIG )
			{
				format_flags |= LIBUNA_BASE64_VARIANT_ENCODING_UTF32_BIG_ENDIAN;
			}
			else if( _BYTE_STREAM_HOST_IS_ENDIAN_LITTLE )
			{
				format_flags |= LIBUNA_BASE64_VARIANT_ENCODING_UTF32_LITTLE_ENDIAN;
			}
			else
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
				 LIBCERROR_ARGUMENT_ERROR_UNSUPPORTED_VALUE,
				 "%s: unsupported host byte order.",
				 function );

				return( -1 );
			}
			string_index = *utf16_string_index * sizeof( uint16_t );

			if( libuna_base64_stream_with_index_copy_from_byte_stream(
			     (uint8_t *) utf16_string,
			     utf16_string_size,
			     &string_index,
			     binary_data->data,
			     binary_data->data_size,
			     format_flags,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_COPY_FAILED,
				 "%s: unable to copy base64 formatted binary data to UTF-16 string.",
				 function );

				return( -1 );
			}
			*utf16_string_index = string_index / sizeof( uint16_t );

			break;
	}
	return( 1 );
}

/* Copies the binary data to an UTF-32 encoded string
 * Returns 1 if successful or -1 on error
 */
int libfvalue_binary_data_copy_to_utf32_string_with_index(
     libfvalue_binary_data_t *binary_data,
     uint32_t *utf32_string,
     size_t utf32_string_size,
     size_t *utf32_string_index,
     uint32_t string_format_flags,
     libcerror_error_t **error )
{
	static char *function       = "libfvalue_binary_data_copy_to_utf32_string_with_index";
	size_t string_index         = 0;
	uint32_t format_flags       = 0;
	uint32_t string_format_type = 0;
	uint32_t supported_flags    = 0;

	if( binary_data == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid binary data.",
		 function );

		return( -1 );
	}
	if( utf32_string_size > (size_t) SSIZE_MAX )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_EXCEEDS_MAXIMUM,
		 "%s: invalid UTF-32 string size value exceeds maximum.",
		 function );

		return( -1 );
	}
	if( utf32_string_index == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid UTF-32 string index.",
		 function );

		return( -1 );
	}
	if( *utf32_string_index > utf32_string_size )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_TOO_SMALL,
		 "%s: UTF-32 string is too small.",
		 function );

		return( -1 );
	}
	supported_flags = 0x000000ffUL
	                | LIBFVALUE_BINARY_DATA_FORMAT_FLAG_CASE_LOWER
	                | LIBFVALUE_BINARY_DATA_FORMAT_FLAG_CASE_UPPER;

	if( ( string_format_flags & ~( supported_flags ) ) != 0 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_UNSUPPORTED_VALUE,
		 "%s: unsupported string format flags: 0x%08" PRIx32 ".",
		 function,
		 string_format_flags );

		return( -1 );
	}
	string_format_type = string_format_flags & 0x000000ffUL;

	if( ( string_format_type != LIBFVALUE_BINARY_DATA_FORMAT_TYPE_BASE16 )
	 && ( string_format_type != LIBFVALUE_BINARY_DATA_FORMAT_TYPE_BASE32 ) 
	 && ( string_format_type != LIBFVALUE_BINARY_DATA_FORMAT_TYPE_BASE64 ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_UNSUPPORTED_VALUE,
		 "%s: unsupported string format type.",
		 function );

		return( -1 );
	}
	if( ( binary_data->data == NULL )
	 || ( binary_data->data_size == 0 ) )
	{
		if( utf32_string == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
			 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
			 "%s: invalid UTF-32 string.",
			 function );

			return( -1 );
		}
		if( utf32_string_size > (size_t) SSIZE_MAX )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
			 LIBCERROR_ARGUMENT_ERROR_VALUE_EXCEEDS_MAXIMUM,
			 "%s: invalid UTF-32 string size value exceeds maximum.",
			 function );

			return( -1 );
		}
		if( utf32_string_index == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
			 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
			 "%s: invalid UTF-32 string index.",
			 function );

			return( -1 );
		}
		if( *utf32_string_index >= utf32_string_size )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
			 LIBCERROR_ARGUMENT_ERROR_VALUE_TOO_SMALL,
			 "%s: UTF-32 string is too small.",
			 function );

			return( -1 );
		}
		utf32_string[ *utf32_string_index ] = 0;

		*utf32_string_index += 1;
	}
	else switch( string_format_type )
	{
		case LIBFVALUE_BINARY_DATA_FORMAT_TYPE_BASE16:
			format_flags = LIBUNA_BASE16_VARIANT_CASE_UPPER
				     | LIBUNA_BASE16_VARIANT_CHARACTER_LIMIT_NONE;

			if( _BYTE_STREAM_HOST_IS_ENDIAN_BIG )
			{
				format_flags |= LIBUNA_BASE16_VARIANT_ENCODING_UTF32_BIG_ENDIAN;
			}
			else if( _BYTE_STREAM_HOST_IS_ENDIAN_LITTLE )
			{
				format_flags |= LIBUNA_BASE16_VARIANT_ENCODING_UTF32_LITTLE_ENDIAN;
			}
			else
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
				 LIBCERROR_ARGUMENT_ERROR_UNSUPPORTED_VALUE,
				 "%s: unsupported host byte order.",
				 function );

				return( -1 );
			}
			string_index = *utf32_string_index * sizeof( uint32_t );

			if( libuna_base16_stream_with_index_copy_from_byte_stream(
			     (uint8_t *) utf32_string,
			     utf32_string_size,
			     &string_index,
			     binary_data->data,
			     binary_data->data_size,
			     format_flags,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_COPY_FAILED,
				 "%s: unable to copy base16 formatted binary data to UTF-32 string.",
				 function );

				return( -1 );
			}
			*utf32_string_index = string_index / sizeof( uint32_t );

			break;

		case LIBFVALUE_BINARY_DATA_FORMAT_TYPE_BASE32:
			format_flags = LIBUNA_BASE32_VARIANT_ALPHABET_NORMAL
			             | LIBUNA_BASE32_VARIANT_CHARACTER_LIMIT_NONE
			             | LIBUNA_BASE32_VARIANT_PADDING_REQUIRED;

			if( _BYTE_STREAM_HOST_IS_ENDIAN_BIG )
			{
				format_flags |= LIBUNA_BASE32_VARIANT_ENCODING_UTF32_BIG_ENDIAN;
			}
			else if( _BYTE_STREAM_HOST_IS_ENDIAN_LITTLE )
			{
				format_flags |= LIBUNA_BASE32_VARIANT_ENCODING_UTF32_LITTLE_ENDIAN;
			}
			else
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
				 LIBCERROR_ARGUMENT_ERROR_UNSUPPORTED_VALUE,
				 "%s: unsupported host byte order.",
				 function );

				return( -1 );
			}
			string_index = *utf32_string_index * sizeof( uint32_t );

			if( libuna_base32_stream_with_index_copy_from_byte_stream(
			     (uint8_t *) utf32_string,
			     utf32_string_size,
			     &string_index,
			     binary_data->data,
			     binary_data->data_size,
			     format_flags,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_COPY_FAILED,
				 "%s: unable to copy base32 formatted binary data to UTF-32 string.",
				 function );

				return( -1 );
			}
			*utf32_string_index = string_index / sizeof( uint32_t );

			break;

		case LIBFVALUE_BINARY_DATA_FORMAT_TYPE_BASE64:
			format_flags = LIBUNA_BASE64_VARIANT_ALPHABET_NORMAL
			             | LIBUNA_BASE64_VARIANT_CHARACTER_LIMIT_NONE
			             | LIBUNA_BASE64_VARIANT_PADDING_REQUIRED;

			if( _BYTE_STREAM_HOST_IS_ENDIAN_BIG )
			{
				format_flags |= LIBUNA_BASE64_VARIANT_ENCODING_UTF32_BIG_ENDIAN;
			}
			else if( _BYTE_STREAM_HOST_IS_ENDIAN_LITTLE )
			{
				format_flags |= LIBUNA_BASE64_VARIANT_ENCODING_UTF32_LITTLE_ENDIAN;
			}
			else
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
				 LIBCERROR_ARGUMENT_ERROR_UNSUPPORTED_VALUE,
				 "%s: unsupported host byte order.",
				 function );

				return( -1 );
			}
			string_index = *utf32_string_index * sizeof( uint32_t );

			if( libuna_base64_stream_with_index_copy_from_byte_stream(
			     (uint8_t *) utf32_string,
			     utf32_string_size,
			     &string_index,
			     binary_data->data,
			     binary_data->data_size,
			     format_flags,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_COPY_FAILED,
				 "%s: unable to copy base64 formatted binary data to UTF-32 string.",
				 function );

				return( -1 );
			}
			*utf32_string_index = string_index / sizeof( uint32_t );

			break;
	}
	return( 1 );
}

