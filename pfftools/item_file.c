/* 
 * Item file
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
#include <file_stream.h>
#include <memory.h>
#include <types.h>

#include "item_file.h"
#include "pfftools_libcerror.h"
#include "pfftools_libcfile.h"
#include "pfftools_libcstring.h"
#include "pfftools_libfdatetime.h"
#include "pfftools_libfguid.h"
#include "pfftools_libfvalue.h"
#include "pfftools_libpff.h"

/* Initializes the item file
 * Returns 1 if successful or -1 on error
 */
int item_file_initialize(
     item_file_t **item_file,
     libcerror_error_t **error )
{
	static char *function = "item_file_initialize";

	if( item_file == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid item file.",
		 function );

		return( -1 );
	}
	if( *item_file != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid item file value already set.",
		 function );

		return( -1 );
	}
	*item_file = memory_allocate_structure(
	              item_file_t );

	if( *item_file == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create item file.",
		 function );

		goto on_error;
	}
	if( memory_set(
	     *item_file,
	     0,
	     sizeof( item_file_t ) ) == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_SET_FAILED,
		 "%s: unable to clear item file.",
		 function );

		goto on_error;
	}
	if( libcfile_file_initialize(
	     &( ( *item_file )->file_handle ),
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create file handle.",
		 function );

		goto on_error;
	}
	return( 1 );

on_error:
	if( *item_file != NULL )
	{
		memory_free(
		 *item_file );

		*item_file = NULL;
	}
	return( -1 );
}

/* Frees the item file and its elements
 * Returns 1 if successful or -1 on error
 */
int item_file_free(
     item_file_t **item_file,
     libcerror_error_t **error )
{
	static char *function = "item_file_free";
	int result            = 1;

	if( item_file == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid item file.",
		 function );

		return( -1 );
	}
	if( *item_file != NULL )
	{
		if( libcfile_file_free(
		     &( ( *item_file )->file_handle ),
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
			 "%s: unable to free file handle.",
			 function );

			result = -1;
		}
		memory_free(
		 *item_file );

		*item_file = NULL;
	}
	return( result );
}

/* Opens the item file
 * Returns 1 if successful or -1 on error
 */
int item_file_open(
     item_file_t *item_file,
     const libcstring_system_character_t *filename,
     libcerror_error_t **error )
{
	static char *function = "item_file_open";

	if( item_file == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid item file.",
		 function );

		return( -1 );
	}
#if defined( LIBCSTRING_HAVE_WIDE_SYSTEM_CHARACTER )
	if( libcfile_file_open_wide(
	     item_file->file_handle,
	     filename,
	     LIBCFILE_OPEN_WRITE,
	     error ) != 1 )
#else
	if( libcfile_file_open(
	     item_file->file_handle,
	     filename,
	     LIBCFILE_OPEN_WRITE,
	     error ) != 1 )
#endif
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_OPEN_FAILED,
		 "%s: unable to open file handle.",
		 function );

		return( -1 );
	}
	return( 1 );
}

/* Closes the item file
 * Returns the 0 if succesful or -1 on error
 */
int item_file_close(
     item_file_t *item_file,
     libcerror_error_t **error )
{
	static char *function = "item_file_close";

	if( item_file == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid item file.",
		 function );

		return( -1 );
	}
	if( libcfile_file_close(
	     item_file->file_handle,
	     error ) != 0 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_CLOSE_FAILED,
		 "%s: unable to close file handle.",
		 function );

		return( -1 );
	}
	return( 0 );
}

/* Writes a buffer to the item file
 * Returns 1 if successful or -1 on error
 */
int item_file_write_buffer(
     item_file_t *item_file,
     const uint8_t *buffer,
     size_t buffer_size,
     libcerror_error_t **error )
{
	static char *function = "item_file_write_buffer";
	ssize_t write_count   = 0;

	if( item_file == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid item file.",
		 function );

		return( -1 );
	}
	if( buffer == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid buffer.",
		 function );

		return( -1 );
	}
	if( buffer_size > (size_t) SSIZE_MAX )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_EXCEEDS_MAXIMUM,
		 "%s: invalid buffer size value exceeds maximum.",
		 function );

		return( -1 );
	}
	write_count = libcfile_file_write_buffer(
		       item_file->file_handle,
		       buffer,
		       buffer_size,
		       error );

	if( write_count != (ssize_t) buffer_size )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_WRITE_FAILED,
		 "%s: unable to write buffer to file handle.",
		 function );

		return( -1 );
	}
	return( 1 );
}

/* Writes a buffer as a dump of hexadecimal values to the item file
 * Returns 1 if successful or -1 on error
 */
int item_file_write_buffer_as_hexdump(
     item_file_t *item_file,
     const uint8_t *buffer,
     size_t buffer_size,
     libcerror_error_t **error )
{
	libcstring_system_character_t value_string[ 3 ];

	static char *function = "item_file_write_buffer_as_hexdump";
	size_t buffer_index   = 0;
	size_t byte_index     = 0;
	uint8_t byte_value    = 0;

	if( item_file == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid item file.",
		 function );

		return( -1 );
	}
	if( buffer != NULL )
	{
		if( buffer_size > (size_t) UINT32_MAX )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
			 LIBCERROR_ARGUMENT_ERROR_VALUE_EXCEEDS_MAXIMUM,
			 "%s: invalid buffer size value exceeds maximum.",
			 function );

			return( -1 );
		}
		while( buffer_index < buffer_size )
		{
			while( byte_index < buffer_size )
			{
				if( byte_index % 16 == 0 )
				{
					if( item_file_write_integer_32bit_as_hexadecimal(
					     item_file,
					     (uint32_t) byte_index,
					     error ) != 1 )
					{
						libcerror_error_set(
						 error,
						 LIBCERROR_ERROR_DOMAIN_IO,
						 LIBCERROR_IO_ERROR_WRITE_FAILED,
						 "%s: unable to write 32-bit value to item file.",
						 function );

						return( -1 );
					}
					if( item_file_write_string(
					     item_file,
					     _LIBCSTRING_SYSTEM_STRING( ": " ),
					     2,
					     error ) != 1 )
					{
						libcerror_error_set(
						 error,
						 LIBCERROR_ERROR_DOMAIN_IO,
						 LIBCERROR_IO_ERROR_WRITE_FAILED,
						 "%s: unable to write string to item file.",
						 function );

						return( -1 );
					}
				}
				byte_value = ( buffer[ byte_index ] >> 4 ) & 0x0f;

				if( byte_value < 10 )
				{
					value_string[ 0 ] = (libcstring_system_character_t) '0' + byte_value;
				}
				else
				{
					value_string[ 0 ] = (libcstring_system_character_t) 'a' + byte_value;
				}
				byte_value = buffer[ byte_index ] & 0x0f;

				if( byte_value < 10 )
				{
					value_string[ 1 ] = (libcstring_system_character_t) '0' + byte_value;
				}
				else
				{
					value_string[ 1 ] = (libcstring_system_character_t) 'a' + byte_value - 10;
				}
				value_string[ 2 ] = 0;

				if( item_file_write_string(
				     item_file,
				     value_string,
				     2,
				     error ) != 1 )
				{
					libcerror_error_set(
					 error,
					 LIBCERROR_ERROR_DOMAIN_IO,
					 LIBCERROR_IO_ERROR_WRITE_FAILED,
					 "%s: unable to write string to item file.",
					 function );

					return( -1 );
				}
				byte_index++;

				if( item_file_write_string(
				     item_file,
				     _LIBCSTRING_SYSTEM_STRING( " " ),
				     1,
				     error ) != 1 )
				{
					libcerror_error_set(
					 error,
					 LIBCERROR_ERROR_DOMAIN_IO,
					 LIBCERROR_IO_ERROR_WRITE_FAILED,
					 "%s: unable to write string to item file.",
					 function );

					return( -1 );
				}
				if( byte_index % 16 == 0 )
				{
					break;
				}
				else if( byte_index % 8 == 0 )
				{
					if( item_file_write_string(
					     item_file,
					     _LIBCSTRING_SYSTEM_STRING( " " ),
					     1,
					     error ) != 1 )
					{
						libcerror_error_set(
						 error,
						 LIBCERROR_ERROR_DOMAIN_IO,
						 LIBCERROR_IO_ERROR_WRITE_FAILED,
						 "%s: unable to write string to item file.",
						 function );

						return( -1 );
					}
				}
			}
			while( byte_index % 16 != 0 )
			{
				byte_index++;

				if( item_file_write_string(
				     item_file,
				     _LIBCSTRING_SYSTEM_STRING( "   " ),
				     3,
				     error ) != 1 )
				{
					libcerror_error_set(
					 error,
					 LIBCERROR_ERROR_DOMAIN_IO,
					 LIBCERROR_IO_ERROR_WRITE_FAILED,
					 "%s: unable to write string to item file.",
					 function );

					return( -1 );
				}
				if( ( byte_index % 8 == 0 )
				 && ( byte_index % 16 != 0 ) )
				{
					if( item_file_write_string(
					     item_file,
					     _LIBCSTRING_SYSTEM_STRING( " " ),
					     1,
					     error ) != 1 )
					{
						libcerror_error_set(
						 error,
						 LIBCERROR_ERROR_DOMAIN_IO,
						 LIBCERROR_IO_ERROR_WRITE_FAILED,
						 "%s: unable to write string to item file.",
						 function );

						return( -1 );
					}
				}
			}
			if( item_file_write_string(
			     item_file,
			     _LIBCSTRING_SYSTEM_STRING( "  " ),
			     2,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_IO,
				 LIBCERROR_IO_ERROR_WRITE_FAILED,
				 "%s: unable to write string to item file.",
				 function );

				return( -1 );
			}
			byte_index = buffer_index;

			while( byte_index < buffer_size )
			{
				if( ( buffer[ byte_index ] >= 0x20 )
				 && ( buffer[ byte_index ] <= 0x7e ) )
				{
					value_string[ 0 ] = (libcstring_system_character_t) buffer[ byte_index ];
				}
				else
				{
					value_string[ 0 ] = (libcstring_system_character_t) '.';
				}
				value_string[ 1 ] = 0;

				if( item_file_write_string(
				     item_file,
				     value_string,
				     1,
				     error ) != 1 )
				{
					libcerror_error_set(
					 error,
					 LIBCERROR_ERROR_DOMAIN_IO,
					 LIBCERROR_IO_ERROR_WRITE_FAILED,
					 "%s: unable to write string to item file.",
					 function );

					return( -1 );
				}
				byte_index++;

				if( byte_index % 16 == 0 )
				{
					break;
				}
				else if( byte_index % 8 == 0 )
				{
					if( item_file_write_string(
					     item_file,
					     _LIBCSTRING_SYSTEM_STRING( " " ),
					     1,
					     error ) != 1 )
					{
						libcerror_error_set(
						 error,
						 LIBCERROR_ERROR_DOMAIN_IO,
						 LIBCERROR_IO_ERROR_WRITE_FAILED,
						 "%s: unable to write string to item file.",
						 function );

						return( -1 );
					}
				}
			}
			if( item_file_write_new_line(
			     item_file,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_IO,
				 LIBCERROR_IO_ERROR_WRITE_FAILED,
				 "%s: unable to write new line to item file.",
				 function );

				return( -1 );
			}
			buffer_index = byte_index;
		}
		if( item_file_write_new_line(
		     item_file,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_IO,
			 LIBCERROR_IO_ERROR_WRITE_FAILED,
			 "%s: unable to write new line to item file.",
			 function );

			return( -1 );
		}
	}
	return( 1 );
}

/* Writes a 32-bit integer as a decimal to the item file
 * Returns 1 if successful or -1 on error
 */
int item_file_write_integer_32bit_as_decimal(
     item_file_t *item_file,
     uint32_t value_32bit,
     libcerror_error_t **error )
{
	libcstring_system_character_t integer_string[ 32 ];

	libfvalue_value_t *value     = NULL;
	static char *function        = "item_file_write_integer_as_decimal";
	size_t integer_string_length = 0;

	if( libfvalue_value_type_initialize(
	     &value,
	     LIBFVALUE_VALUE_TYPE_UNSIGNED_INTEGER_32BIT,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create value.",
		 function );

		goto on_error;
	}
	if( libfvalue_value_set_data(
	     value,
	     (uint8_t *) &value_32bit,
	     sizeof( uint32_t ),
	     LIBFVALUE_ENDIAN_NATIVE,
	     0,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to set data.",
		 function );

		goto on_error;
	}
	if( libfvalue_value_set_format_flags(
	     value,
	     LIBFVALUE_INTEGER_FORMAT_TYPE_DECIMAL_SIGNED,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to set format flags.",
		 function );

		goto on_error;
	}
#if defined( LIBCSTRING_HAVE_WIDE_SYSTEM_CHARACTER )
	if( libfvalue_value_copy_to_utf16_string(
	     value,
	     0,
	     (uint16_t *) integer_string,
	     32,
	     error ) != 1 )
#else
	if( libfvalue_value_copy_to_utf8_string(
	     value,
	     0,
	     (uint8_t *) integer_string,
	     32,
	     error ) != 1 )
#endif
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to copy integer value to string.",
		 function );

		goto on_error;
	}
	integer_string_length = libcstring_system_string_length(
	                         integer_string );

	if( item_file_write_string(
	     item_file,
	     integer_string,
	     integer_string_length,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_WRITE_FAILED,
		 "%s: unable to write string.",
		 function );

		goto on_error;
	}
	if( libfvalue_value_free(
	     &value,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
		 "%s: unable to free value.",
		 function );

		goto on_error;
	}
	return( 1 );

on_error:
	if( value != NULL )
	{
		libfvalue_value_free(
		 &value,
		 NULL );
	}
	return( -1 );
}

/* Writes a 32-bit integer as a hexadecimal to the item file
 * Returns 1 if successful or -1 on error
 */
int item_file_write_integer_32bit_as_hexadecimal(
     item_file_t *item_file,
     uint32_t value_32bit,
     libcerror_error_t **error )
{
	libcstring_system_character_t integer_string[ 32 ];

	libfvalue_value_t *value     = NULL;
	static char *function        = "item_file_write_integer_as_hexadecimal";
	size_t integer_string_length = 0;

	if( libfvalue_value_type_initialize(
	     &value,
	     LIBFVALUE_VALUE_TYPE_UNSIGNED_INTEGER_32BIT,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create value.",
		 function );

		goto on_error;
	}
	if( libfvalue_value_set_data(
	     value,
	     (uint8_t *) &value_32bit,
	     sizeof( uint32_t ),
	     LIBFVALUE_ENDIAN_NATIVE,
	     0,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to set data.",
		 function );

		goto on_error;
	}
	if( libfvalue_value_set_format_flags(
	     value,
	     LIBFVALUE_INTEGER_FORMAT_TYPE_HEXADECIMAL,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to set format flags.",
		 function );

		goto on_error;
	}
#if defined( LIBCSTRING_HAVE_WIDE_SYSTEM_CHARACTER )
	if( libfvalue_value_copy_to_utf16_string(
	     value,
	     0,
	     (uint16_t *) integer_string,
	     32,
	     error ) != 1 )
#else
	if( libfvalue_value_copy_to_utf8_string(
	     value,
	     0,
	     (uint8_t *) integer_string,
	     32,
	     error ) != 1 )
#endif
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to copy integer value to string.",
		 function );

		goto on_error;
	}
	integer_string_length = libcstring_system_string_length(
	                         integer_string );

	if( item_file_write_string(
	     item_file,
	     integer_string,
	     integer_string_length,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_WRITE_FAILED,
		 "%s: unable to write string.",
		 function );

		goto on_error;
	}
	if( libfvalue_value_free(
	     &value,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
		 "%s: unable to free value.",
		 function );

		goto on_error;
	}
	return( 1 );

on_error:
	if( value != NULL )
	{
		libfvalue_value_free(
		 &value,
		 NULL );
	}
	return( -1 );
}

/* Writes a floating point to the item file
 * Returns 1 if successful or -1 on error
 */
int item_file_write_floating_point(
     item_file_t *item_file,
     double floating_point,
     libcerror_error_t **error )
{
	libcstring_system_character_t floating_point_string[ 32 ];

	libfvalue_value_t *value            = NULL;
	static char *function               = "item_file_write_floating_point";
	size_t floating_point_string_length = 0;

	if( libfvalue_value_type_initialize(
	     &value,
	     LIBFVALUE_VALUE_TYPE_FLOATING_POINT_64BIT,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create value.",
		 function );

		goto on_error;
	}
	if( libfvalue_value_copy_from_double(
	     value,
	     0,
	     floating_point,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_COPY_FAILED,
		 "%s: unable to copy floating point value.",
		 function );

		goto on_error;
	}
#if defined( LIBCSTRING_HAVE_WIDE_SYSTEM_CHARACTER )
	if( libfvalue_value_copy_to_utf16_string(
	     value,
	     0,
	     (uint16_t *) floating_point_string,
	     32,
	     error ) != 1 )
#else
	if( libfvalue_value_copy_to_utf8_string(
	     value,
	     0,
	     (uint8_t *) floating_point_string,
	     32,
	     error ) != 1 )
#endif
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to copy integer value to string.",
		 function );

		goto on_error;
	}
	floating_point_string_length = libcstring_system_string_length(
	                                floating_point_string );

	if( item_file_write_string(
	     item_file,
	     floating_point_string,
	     floating_point_string_length,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_WRITE_FAILED,
		 "%s: unable to write string.",
		 function );

		goto on_error;
	}
	if( libfvalue_value_free(
	     &value,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
		 "%s: unable to free value.",
		 function );

		goto on_error;
	}
	return( 1 );

on_error:
	if( value != NULL )
	{
		libfvalue_value_free(
		 &value,
		 NULL );
	}
	return( -1 );
}

/* Writes a string to the item file
 * Returns 1 if successful or -1 on error
 */
int item_file_write_string(
     item_file_t *item_file,
     const libcstring_system_character_t *string,
     size_t string_length,
     libcerror_error_t **error )
{
	static char *function = "item_file_write_string";

	if( string == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid string.",
		 function );

		return( -1 );
	}
#if defined( LIBCSTRING_HAVE_WIDE_SYSTEM_CHARACTER )
	if( string_length > (size_t) SSIZE_MAX )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_EXCEEDS_MAXIMUM,
		 "%s: invalid string length value exceeds maximum.",
		 function );

		return( -1 );
	}
	string_length *= sizeof( libcstring_system_character_t );
#endif
	if( item_file_write_buffer(
	     item_file,
	     (uint8_t *) string,
	     string_length,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_WRITE_FAILED,
		 "%s: unable to write buffer.",
		 function );

		return( -1 );
	}
	return( 1 );
}

/* Writes a new line to the item file
 * Returns 1 if successful or -1 on error
 */
int item_file_write_new_line(
     item_file_t *item_file,
     libcerror_error_t **error )
{
	static char *function = "item_file_write_new_line";

#if defined( WINAPI )
	if( item_file_write_string(
	     item_file,
	     _LIBCSTRING_SYSTEM_STRING( "\r\n" ),
	     2,
	     error ) != 1 )
#else
	if( item_file_write_string(
	     item_file,
	     _LIBCSTRING_SYSTEM_STRING( "\n" ),
	     1,
	     error ) != 1 )
#endif
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_WRITE_FAILED,
		 "%s: unable to write string.",
		 function );

		return( -1 );
	}
	return( 1 );
}

/* Writes a filetime to the item file
 * Returns 1 if successful or -1 on error
 */
int item_file_write_filetime(
     item_file_t *item_file,
     libfdatetime_filetime_t *filetime,
     libcerror_error_t **error )
{
	libcstring_system_character_t filetime_string[ 32 ];

	static char *function         = "item_file_write_filetime";
	size_t filetime_string_length = 0;
	int result                    = 0;

#if defined( LIBCSTRING_HAVE_WIDE_SYSTEM_CHARACTER )
	result = libfdatetime_filetime_copy_to_utf16_string(
		  filetime,
		  (uint16_t *) filetime_string,
		  32,
		  LIBFDATETIME_STRING_FORMAT_TYPE_CTIME | LIBFDATETIME_STRING_FORMAT_FLAG_DATE_TIME_NANO_SECONDS,
		  error );
#else
	result = libfdatetime_filetime_copy_to_utf8_string(
		  filetime,
		  (uint8_t *) filetime_string,
		  32,
		  LIBFDATETIME_STRING_FORMAT_TYPE_CTIME | LIBFDATETIME_STRING_FORMAT_FLAG_DATE_TIME_NANO_SECONDS,
		  error );
#endif
	if( result != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_COPY_FAILED,
		 "%s: unable to copy filetime to string.",
		 function );

		return( -1 );
	}
	filetime_string_length = libcstring_system_string_length(
	                          filetime_string );

	if( item_file_write_string(
	     item_file,
	     filetime_string,
	     filetime_string_length,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_WRITE_FAILED,
		 "%s: unable to write string.",
		 function );

		return( -1 );
	}
	if( item_file_write_string(
	     item_file,
	     _LIBCSTRING_SYSTEM_STRING( " UTC" ),
	     4,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_WRITE_FAILED,
		 "%s: unable to write string.",
		 function );

		return( -1 );
	}
	return( 1 );
}

/* Writes a GUID to the item file
 * Returns 1 if successful or -1 on error
 */
int item_file_write_guid(
     item_file_t *item_file,
     libfguid_identifier_t *guid,
     libcerror_error_t **error )
{
	libcstring_system_character_t guid_string[ 48 ];

	static char *function     = "item_file_write_guid";
	size_t guid_string_length = 0;
	int result                = 0;

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

		return( -1 );
	}
	guid_string_length = libcstring_system_string_length(
	                      guid_string );

	if( item_file_write_string(
	     item_file,
	     guid_string,
	     guid_string_length,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_WRITE_FAILED,
		 "%s: unable to write string.",
		 function );

		return( -1 );
	}
	return( 1 );
}

/* Writes a value description to the item file
 * Returns 1 if successful or -1 on error
 */
int item_file_write_value_description(
     item_file_t *item_file,
     const libcstring_system_character_t *description,
     libcerror_error_t **error )
{
	static char *function     = "item_file_write_value_description";
	size_t description_length = 0;

	description_length = libcstring_system_string_length(
	                      description );
	
	if( item_file_write_string(
	     item_file,
	     description,
	     description_length,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_WRITE_FAILED,
		 "%s: unable to write string.",
		 function );

		return( -1 );
	}
	if( item_file_write_new_line(
	     item_file,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_WRITE_FAILED,
		 "%s: unable to write new line.",
		 function );

		return( -1 );
	}
	return( 1 );
}

/* Writes a 32-bit integer value as a decimal to the item file
 * Returns 1 if successful or -1 on error
 */
int item_file_write_value_integer_32bit_as_decimal(
     item_file_t *item_file,
     const libcstring_system_character_t *description,
     uint32_t value_32bit,
     libcerror_error_t **error )
{
	static char *function     = "item_file_write_value_integer_32bit_as_decimal";
	size_t description_length = 0;

	description_length = libcstring_system_string_length(
	                      description );
	
	if( item_file_write_string(
	     item_file,
	     description,
	     description_length,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_WRITE_FAILED,
		 "%s: unable to write string.",
		 function );

		return( -1 );
	}
	if( item_file_write_integer_32bit_as_decimal(
	     item_file,
	     value_32bit,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_WRITE_FAILED,
		 "%s: unable to write 32-bit value.",
		 function );

		return( -1 );
	}
	if( item_file_write_new_line(
	     item_file,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_WRITE_FAILED,
		 "%s: unable to write new line.",
		 function );

		return( -1 );
	}
	return( 1 );
}

/* Writes a 32-bit integer value as a hexadecimal to the item file
 * Returns 1 if successful or -1 on error
 */
int item_file_write_value_integer_32bit_as_hexadecimal(
     item_file_t *item_file,
     const libcstring_system_character_t *description,
     uint32_t value_32bit,
     libcerror_error_t **error )
{
	static char *function     = "item_file_write_value_integer_32bit_as_hexadecimal";
	size_t description_length = 0;

	description_length = libcstring_system_string_length(
	                      description );
	
	if( item_file_write_string(
	     item_file,
	     description,
	     description_length,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_WRITE_FAILED,
		 "%s: unable to write string.",
		 function );

		return( -1 );
	}
	if( item_file_write_integer_32bit_as_hexadecimal(
	     item_file,
	     value_32bit,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_WRITE_FAILED,
		 "%s: unable to write 32-bit value.",
		 function );

		return( -1 );
	}
	if( item_file_write_new_line(
	     item_file,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_WRITE_FAILED,
		 "%s: unable to write new line.",
		 function );

		return( -1 );
	}
	return( 1 );
}

/* Writes a value string to the item file
 * Returns 1 if successful or -1 on error
 */
int item_file_write_value_string(
     item_file_t *item_file,
     const libcstring_system_character_t *description,
     const libcstring_system_character_t *string,
     size_t string_length,
     libcerror_error_t **error )
{
	static char *function     = "item_file_write_value_description";
	size_t description_length = 0;

	description_length = libcstring_system_string_length(
	                      description );
	
	if( item_file_write_string(
	     item_file,
	     description,
	     description_length,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_WRITE_FAILED,
		 "%s: unable to write string.",
		 function );

		return( -1 );
	}
	if( item_file_write_string(
	     item_file,
	     string,
	     string_length,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_WRITE_FAILED,
		 "%s: unable to write string.",
		 function );

		return( -1 );
	}
	if( item_file_write_new_line(
	     item_file,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_WRITE_FAILED,
		 "%s: unable to write new line.",
		 function );

		return( -1 );
	}
	return( 1 );
}

/* Writes a filetime value to the item file
 * Returns 1 if successful or -1 on error
 */
int item_file_write_value_filetime(
     item_file_t *item_file,
     const libcstring_system_character_t *description,
     libfdatetime_filetime_t *filetime,
     libcerror_error_t **error )
{
	static char *function     = "item_file_write_value_filetime";
	size_t description_length = 0;

	description_length = libcstring_system_string_length(
	                      description );
	
	if( item_file_write_string(
	     item_file,
	     description,
	     description_length,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_WRITE_FAILED,
		 "%s: unable to write string.",
		 function );

		return( -1 );
	}
	if( item_file_write_filetime(
	     item_file,
	     filetime,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_WRITE_FAILED,
		 "%s: unable to write filetime.",
		 function );

		return( -1 );
	}
	if( item_file_write_new_line(
	     item_file,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_WRITE_FAILED,
		 "%s: unable to write new line.",
		 function );

		return( -1 );
	}
	return( 1 );
}

/* Writes a specific item value to the item file
 * Returns 1 if successful or -1 on error
 */
int item_file_write_item_value(
     item_file_t *item_file,
     libpff_item_t *item,
     int set_index,
     uint32_t entry_type,
     const libcstring_system_character_t *description,
     uint32_t format_flags,
     libcerror_error_t **error )
{
	libcstring_system_character_t *value_string = NULL;
	libfdatetime_filetime_t *filetime           = NULL;
	static char *function                       = "item_file_write_item_value";
	size_t description_length                   = 0;
	size_t value_string_size                    = 0;
	double value_double                         = 0.0;
	uint64_t value_64bit                        = 0;
	uint32_t value_32bit                        = 0;
	uint32_t value_type                         = LIBPFF_VALUE_TYPE_UNSPECIFIED;
	uint8_t value_boolean                       = 0;
	int result                                  = 0;

	result = libpff_item_get_value_type(
	          item,
	          set_index,
	          entry_type,
	          &value_type,
	          0,
	          error );

	if( result == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve entry type of set: %d entry type: 0x%08" PRIx32 ".",
		 function,
		 set_index,
		 entry_type );

		goto on_error;
	}
	else if( result != 0 )
	{
		description_length = libcstring_system_string_length(
		                      description );
		
		if( item_file_write_string(
		     item_file,
		     description,
		     description_length,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_IO,
			 LIBCERROR_IO_ERROR_WRITE_FAILED,
			 "%s: unable to write description string.",
			 function );

			goto on_error;
		}
		switch( value_type )
		{
			case LIBPFF_VALUE_TYPE_BOOLEAN:
				if( libpff_item_get_entry_value_boolean(
				     item,
				     set_index,
				     entry_type,
				     &value_boolean,
				     0,
				     error ) != 1 )
				{
					libcerror_error_set(
					 error,
					 LIBCERROR_ERROR_DOMAIN_RUNTIME,
					 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
					 "%s: unable to retrieve boolean.",
					 function );

					goto on_error;
				}
				if( value_boolean == 0 )
				{
					value_string      = _LIBCSTRING_SYSTEM_STRING( "no" );
					value_string_size = 3;
				}
				else
				{
					value_string      = _LIBCSTRING_SYSTEM_STRING( "yes" );
					value_string_size = 4;
				}
				if( item_file_write_string(
				     item_file,
				     value_string,
				     value_string_size - 1,
				     error ) != 1 )
				{
					libcerror_error_set(
					 error,
					 LIBCERROR_ERROR_DOMAIN_IO,
					 LIBCERROR_IO_ERROR_WRITE_FAILED,
					 "%s: unable to write string.",
					 function );

					value_string = NULL;

					goto on_error;
				}
				break;

			case LIBPFF_VALUE_TYPE_INTEGER_32BIT_SIGNED:
				if( libpff_item_get_entry_value_32bit(
				     item,
				     set_index,
				     entry_type,
				     &value_32bit,
				     0,
				     error ) != 1 )
				{
					libcerror_error_set(
					 error,
					 LIBCERROR_ERROR_DOMAIN_RUNTIME,
					 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
					 "%s: unable to retrieve 32-bit integer.",
					 function );

					goto on_error;
				}
				if( ( format_flags & ITEM_FILE_FORMAT_FLAG_HEXADECIMAL ) != 0 )
				{
					if( item_file_write_integer_32bit_as_hexadecimal(
					     item_file,
					     value_32bit,
					     error ) != 1 )
					{
						libcerror_error_set(
						 error,
						 LIBCERROR_ERROR_DOMAIN_IO,
						 LIBCERROR_IO_ERROR_WRITE_FAILED,
						 "%s: unable to write 32-bit integer.",
						 function );

						goto on_error;
					}
				}
				else
				{
					if( item_file_write_integer_32bit_as_decimal(
					     item_file,
					     value_32bit,
					     error ) != 1 )
					{
						libcerror_error_set(
						 error,
						 LIBCERROR_ERROR_DOMAIN_IO,
						 LIBCERROR_IO_ERROR_WRITE_FAILED,
						 "%s: unable to write 32-bit integer.",
						 function );

						goto on_error;
					}
				}
				if( ( format_flags & ITEM_FILE_FORMAT_FLAG_DURATION_IN_MINUTES ) != 0 )
				{
					if( value_32bit >= 1 )
					{
						if( value_32bit == 1 )
						{
							value_string      = _LIBCSTRING_SYSTEM_STRING( " minute" );
							value_string_size = 8;
						}
						else if( value_32bit > 1 )
						{
							value_string      = _LIBCSTRING_SYSTEM_STRING( " minutes" );
							value_string_size = 9;
						}
						if( item_file_write_string(
						     item_file,
						     value_string,
						     value_string_size - 1,
						     error ) != 1 )
						{
							libcerror_error_set(
							 error,
							 LIBCERROR_ERROR_DOMAIN_IO,
							 LIBCERROR_IO_ERROR_WRITE_FAILED,
							 "%s: unable to write string.",
							 function );

							value_string = NULL;

							goto on_error;
						}
					}
				}
				break;

			case LIBPFF_VALUE_TYPE_FLOAT_32BIT:
			case LIBPFF_VALUE_TYPE_DOUBLE_64BIT:
				if( libpff_item_get_entry_value_floating_point(
				     item,
				     set_index,
				     entry_type,
				     &value_double,
				     0,
				     error ) != 1 )
				{
					libcerror_error_set(
					 error,
					 LIBCERROR_ERROR_DOMAIN_RUNTIME,
					 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
					 "%s: unable to retrieve boolean.",
					 function );

					goto on_error;
				}
				if( item_file_write_floating_point(
				     item_file,
				     value_double,
				     error ) != 1 )
				{
					libcerror_error_set(
					 error,
					 LIBCERROR_ERROR_DOMAIN_IO,
					 LIBCERROR_IO_ERROR_WRITE_FAILED,
					 "%s: unable to write floating point.",
					 function );

					goto on_error;
				}
				break;

			case LIBPFF_VALUE_TYPE_FILETIME:
				if( libpff_item_get_entry_value_filetime(
				     item,
				     set_index,
				     entry_type,
				     &value_64bit,
				     0,
				     error ) != 1 )
				{
					libcerror_error_set(
					 error,
					 LIBCERROR_ERROR_DOMAIN_RUNTIME,
					 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
					 "%s: unable to retrieve filetime.",
					 function );

					goto on_error;
				}
				if( libfdatetime_filetime_initialize(
				     &filetime,
				     error ) != 1 )
				{
					libcerror_error_set(
					 error,
					 LIBCERROR_ERROR_DOMAIN_RUNTIME,
					 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
					 "%s: unable to create filetime.",
					 function );

					goto on_error;
				}
				if( libfdatetime_filetime_copy_from_64bit(
				     filetime,
				     value_64bit,
				     error ) != 1 )
				{
					libcerror_error_set(
					 error,
					 LIBCERROR_ERROR_DOMAIN_MEMORY,
					 LIBCERROR_MEMORY_ERROR_COPY_FAILED,
					 "%s: unable to copy filetime from 64-bit value.",
					 function );

					goto on_error;
				}
				if( item_file_write_filetime(
				     item_file,
				     filetime,
				     error ) != 1 )
				{
					libcerror_error_set(
					 error,
					 LIBCERROR_ERROR_DOMAIN_IO,
					 LIBCERROR_IO_ERROR_WRITE_FAILED,
					 "%s: unable to write filetime.",
					 function );

					goto on_error;
				}
				if( libfdatetime_filetime_free(
				     &filetime,
				     error ) != 1 )
				{
					libcerror_error_set(
					 error,
					 LIBCERROR_ERROR_DOMAIN_RUNTIME,
					 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
					 "%s: unable to free filetime.",
					 function );

					goto on_error;
				}
				break;

			case LIBPFF_VALUE_TYPE_STRING_ASCII:
			case LIBPFF_VALUE_TYPE_STRING_UNICODE:
#if defined( LIBCSTRING_HAVE_WIDE_SYSTEM_CHARACTER )
				result = libpff_item_get_entry_value_utf16_string_size(
				          item,
				          set_index,
				          entry_type,
				          &value_string_size,
				          0,
				          error );
#else
				result = libpff_item_get_entry_value_utf8_string_size(
				          item,
				          set_index,
				          entry_type,
				          &value_string_size,
				          0,
				          error );
#endif
				if( result != 1 )
				{
					libcerror_error_set(
					 error,
					 LIBCERROR_ERROR_DOMAIN_RUNTIME,
					 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
					 "%s: unable to retrieve string size.",
					 function );

					goto on_error;
				}
				else if( value_string_size > 0 )
				{
					value_string = libcstring_system_string_allocate(
					                value_string_size );

					if( value_string == NULL )
					{
						libcerror_error_set(
						 error,
						 LIBCERROR_ERROR_DOMAIN_MEMORY,
						 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
						 "%s: unable to create value string.",
						 function );

						goto on_error;
					}
#if defined( LIBCSTRING_HAVE_WIDE_SYSTEM_CHARACTER )
					result = libpff_item_get_entry_value_utf16_string(
					          item,
					          set_index,
					          entry_type,
					          (uint16_t *) value_string,
					          value_string_size,
					          0,
					          error );
#else
					result = libpff_item_get_entry_value_utf8_string(
					          item,
					          set_index,
					          entry_type,
					          (uint8_t *) value_string,
					          value_string_size,
					          0,
					          error );
#endif
					if( result != 1 )
					{
						libcerror_error_set(
						 error,
						 LIBCERROR_ERROR_DOMAIN_RUNTIME,
						 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
						 "%s: unable to retrieve string.",
						 function );

						goto on_error;
					}
					if( item_file_write_string(
					     item_file,
					     value_string,
					     value_string_size - 1,
					     error ) != 1 )
					{
						libcerror_error_set(
						 error,
						 LIBCERROR_ERROR_DOMAIN_IO,
						 LIBCERROR_IO_ERROR_WRITE_FAILED,
						 "%s: unable to write string.",
						 function );

						goto on_error;
					}
					memory_free(
					 value_string );

					value_string = NULL;
				}
				break;
		}
		if( item_file_write_new_line(
		     item_file,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_IO,
			 LIBCERROR_IO_ERROR_WRITE_FAILED,
			 "%s: unable to write new line.",
			 function );

			goto on_error;
		}
	}
	return( 1 );

on_error:
	if( value_string != NULL )
	{
		memory_free(
		 value_string );
	}
	if( filetime != NULL )
	{
		libfdatetime_filetime_free(
		 &filetime,
		 NULL );
	}
	return( -1 );
}

