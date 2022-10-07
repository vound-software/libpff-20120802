/*
 * Table block functions
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

#include "libpff_array_type.h"
#include "libpff_libbfio.h"
#include "libpff_libcerror.h"
#include "libpff_libcnotify.h"
#include "libpff_libfcache.h"
#include "libpff_libfdata.h"
#include "libpff_table_block.h"

#include "pff_table_block.h"

/* Creates a table block value
 * Returns 1 if successful or -1 on error
 */
int libpff_table_block_value_initialize(
     libpff_table_block_value_t **table_block_value,
     libcerror_error_t **error )
{
	static char *function = "libpff_table_block_value_initialize";

	if( table_block_value == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid table block value.",
		 function );

		return( -1 );
	}
	if( *table_block_value != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid table block value value already set.",
		 function );

		return( -1 );
	}
	*table_block_value = memory_allocate_structure(
	                      libpff_table_block_value_t );

	if( *table_block_value == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create table block value.",
		 function );

		goto on_error;
	}
	if( memory_set(
	     *table_block_value,
	     0,
	     sizeof( libpff_table_block_value_t ) ) == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_SET_FAILED,
		 "%s: unable to clear table block value.",
		 function );

		goto on_error;
	}
	return( 1 );

on_error:
	if( *table_block_value != NULL )
	{
		memory_free(
		 *table_block_value );

		*table_block_value = NULL;
	}
	return( -1 );
}

/* Frees the table block value
 * Returns 1 if successful or -1 on error
 */
int libpff_table_block_value_free(
     libpff_table_block_value_t **table_block_value,
     libcerror_error_t **error )
{
	static char *function = "libpff_table_block_value_free";

	if( table_block_value == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid table block value.",
		 function );

		return( -1 );
	}
	if( *table_block_value != NULL )
	{
		/* The data is referenced and freed elsewhere
		 */
		memory_free(
		 *table_block_value );

		*table_block_value = NULL;
	}
	return( 1 );
}

/* Creates a table_block
 * Returns 1 if successful or -1 on error
 */
int libpff_table_block_initialize(
     libpff_table_block_t **table_block,
     libcerror_error_t **error )
{
	static char *function = "libpff_table_block_initialize";

	if( table_block == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid table block.",
		 function );

		return( -1 );
	}
	if( *table_block != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid table block value already set.",
		 function );

		return( -1 );
	}
	*table_block = memory_allocate_structure(
	                libpff_table_block_t );

	if( *table_block == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create table block.",
		 function );

		goto on_error;
	}
	if( memory_set(
	     *table_block,
	     0,
	     sizeof( libpff_table_block_t ) ) == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_SET_FAILED,
		 "%s: unable to clear table block.",
		 function );

		goto on_error;
	}
	if( libpff_array_initialize(
	     &( ( *table_block )->values_array ),
	     0,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create table block values array.",
		 function );

		goto on_error;
	}
	return( 1 );

on_error:
	if( *table_block != NULL )
	{
		memory_free(
		 *table_block );

		*table_block = NULL;
	}
	return( -1 );
}

/* Frees table_block
 * Returns 1 if successful or -1 on error
 */
int libpff_table_block_free(
     libpff_table_block_t **table_block,
     libcerror_error_t **error )
{
	static char *function = "libpff_table_block_free";
	int result            = 1;

	if( table_block == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid table block.",
		 function );

		return( -1 );
	}
	if( *table_block != NULL )
	{
		if( libpff_array_free(
		     &( ( *table_block )->values_array ),
		     (int (*)(intptr_t **, libcerror_error_t **)) &libpff_table_block_value_free,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
			 "%s: unable to free the table block values array.",
			 function );

			result = -1;
		}
		memory_free(
		 *table_block );

		*table_block = NULL;
	}
	return( result );
}

/* Reads a table block and its values
 * Returns 1 if successful or -1 on error
 */
int libpff_table_block_read(
     libpff_table_block_t *table_block,
     libpff_io_handle_t *io_handle,
     libbfio_handle_t *file_io_handle,
     libfdata_block_t *table_array_data_block,
     libfcache_cache_t *table_array_data_cache,
     libcerror_error_t **error )
{
	uint8_t *table_block_data         = NULL;
	static char *function             = "libpff_table_block_read";
	size_t table_block_data_size      = 0;
	uint16_t table_block_index_offset = 0;

	if( table_block == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid table block.",
		 function );

		return( -1 );
	}
	if( libfdata_block_get_segment_data(
	     table_array_data_block,
	     file_io_handle,
	     table_array_data_cache,
	     table_block->table_array_entry,
	     &table_block_data,
	     &table_block_data_size,
	     0,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve table block data: %d.",
		 function,
		 table_block->table_array_entry );

		return( -1 );
	}
	if( table_block_data == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: missing table block data.",
		 function );

		return( -1 );
	}
	if( table_block_data_size < 2 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: invalid table block data size value out of bounds.",
		 function );

		return( -1 );
	}
	byte_stream_copy_to_uint16_little_endian(
	 table_block_data,
	 table_block_index_offset );

#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		libcnotify_printf(
		 "%s: table block index offset\t\t: %" PRIu16 "\n",
		 function,
		 table_block_index_offset );

		libcnotify_printf(
		 "\n" );
	}
#endif
	if( libpff_table_block_read_index(
	     table_block,
	     table_block_data,
	     table_block_data_size,
	     table_block_index_offset,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_READ_FAILED,
		 "%s: unable to read table block index.",
		 function );

		return( -1 );
	}
	return( 1 );
}

/* Reads the table block index
 * Returns 1 if successful or -1 on error
 */
int libpff_table_block_read_index(
     libpff_table_block_t *table_block,
     uint8_t *table_block_data,
     size_t table_block_data_size,
     uint16_t table_block_index_offset,
     libcerror_error_t **error )
{
	libpff_table_block_value_t *table_block_value = NULL;
	uint8_t *table_block_index_data               = NULL;
	static char *function                         = "libpff_table_block_read_index";
	uint16_t index_offset_iterator                = 0;
	uint16_t number_of_index_offsets              = 0;
	uint16_t number_of_unused_index_offsets       = 0;
	uint16_t value_end_offset                     = 0;
	uint16_t value_start_offset                   = 0;

	if( table_block == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid table block.",
		 function );

		return( -1 );
	}
	if( table_block_data == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid table block data.",
		 function );

		return( -1 );
	}
	if( table_block_data_size > (size_t) SSIZE_MAX )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_EXCEEDS_MAXIMUM,
		 "%s: invalid table block data size value exceeds maximum.",
		 function );

		return( -1 );
	}
	if( table_block_index_offset == 0 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: invalid table block index offset.",
		 function );

		return( -1 );
	}
	/* The table block index offset should point to an offset within the table block
	 */
	if( (size_t) table_block_index_offset >= table_block_data_size )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: table index block offset exceeds table block data size.",
		 function );

		return( -1 );
	}
	table_block_index_data = &( table_block_data[ table_block_index_offset ] );

#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		libcnotify_printf(
		 "%s: table block index:\n",
		 function );
		libcnotify_print_data(
		 table_block_index_data,
		 4,
		 0 );
	}
#endif
	byte_stream_copy_to_uint16_little_endian(
	 ( (pff_table_block_index_t *) table_block_index_data )->number_of_offsets,
	 number_of_index_offsets );

	byte_stream_copy_to_uint16_little_endian(
	 ( (pff_table_block_index_t *) table_block_index_data )->number_of_unused_offsets,
	 number_of_unused_index_offsets );

	table_block_index_data += 4;

#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		libcnotify_printf(
		 "%s: number of index offsets\t\t: %" PRIu16 "\n",
		 function,
		 number_of_index_offsets );

		libcnotify_printf(
		 "%s: number of unused index offsets\t: %" PRIu16 "\n",
		 function,
		 number_of_unused_index_offsets );

		libcnotify_printf(
		 "\n" );
	}
#endif
	if( (size_t) ( table_block_index_offset + 4 + ( ( number_of_index_offsets + 1 ) * 2 ) ) > table_block_data_size )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: number of index offsets do not fit in table block.",
		 function );

		return( -1 );
	}
	if( number_of_index_offsets > 0 )
	{
		if( libpff_array_resize(
		     table_block->values_array,
		     (int) number_of_index_offsets,
		     (int (*)(intptr_t **, libcerror_error_t **)) &libpff_table_block_value_free,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_RESIZE_FAILED,
			 "%s: unable to resize table block index array.",
			 function );

			return( -1 );
		}
#if defined( HAVE_DEBUG_OUTPUT )
		if( libcnotify_verbose != 0 )
		{
			libcnotify_printf(
			 "%s: table block index data:\n",
			 function );
			libcnotify_print_data(
			 table_block_index_data,
			 ( number_of_index_offsets + 1 ) * 2,
			 0 );
		}
#endif
		byte_stream_copy_to_uint16_little_endian(
		 table_block_index_data,
		 value_start_offset );

		table_block_index_data += 2;

		for( index_offset_iterator = 0;
		     index_offset_iterator < number_of_index_offsets;
		     index_offset_iterator++ )
		{
			byte_stream_copy_to_uint16_little_endian(
			 table_block_index_data,
			 value_end_offset );

			table_block_index_data += 2;

#if defined( HAVE_DEBUG_OUTPUT )
			if( libcnotify_verbose != 0 )
			{
				libcnotify_printf(
				 "%s: table block value: %03" PRIu16 " index offset range\t: %" PRIu16 " - %" PRIu16 "\n",
				 function,
				 index_offset_iterator,
				 value_start_offset,
				 value_end_offset );
			}
#endif
			if( value_start_offset > value_end_offset )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
				 "%s: table block index start offset: %" PRIu16 " exceeds end offset: %" PRIu16 ".",
				 function,
				 value_start_offset,
				 value_end_offset );

				return( -1 );
			}
			if( libpff_table_block_value_initialize(
			     &table_block_value,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
				 "%s: unable to create table block value.",
				 function );

				return( -1 );
			}
			/* TODO check sanity of size */

			table_block_value->offset = value_start_offset;
			table_block_value->size   = value_end_offset - value_start_offset;

			if( libpff_array_set_entry_by_index(
			     table_block->values_array,
			     (int) index_offset_iterator,
			     (intptr_t *) table_block_value,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
				 "%s: unable to set table block value: %" PRIu16 ".",
				 function,
				 index_offset_iterator );

				libpff_table_block_value_free(
				 &table_block_value,
				 NULL );

				return( -1 );
			}
			table_block_value = NULL;

			value_start_offset = value_end_offset;
		}
		if( value_end_offset > table_block_index_offset )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
			 "%s: last index value end offset: %" PRIu16 " exceeds table block index offset: %" PRIu16 ".",
			 function,
			 value_end_offset,
			 table_block_index_offset );

			return( -1 );
		}
	}
#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		libcnotify_printf(
		 "\n" );

		if( value_end_offset < table_block_index_offset )
		{
			libcnotify_printf(
			 "%s: last index value end offset: %" PRIu16 " does not match table block index offset: %" PRIu16 "\n",
			 function,
			 value_start_offset,
			 table_block_index_offset );
			libcnotify_print_data(
			 &( table_block_data[ value_end_offset ] ),
			 table_block_index_offset - value_end_offset,
			 0 );
		}
		if( (size_t) ( table_block_index_data - table_block_data ) != table_block_data_size )
		{
			libcnotify_printf(
			 "%s: trailing data of size: %" PRIzd "\n",
			 function,
			 table_block_data_size - (size_t) ( table_block_index_data - table_block_data ) );
			libcnotify_print_data(
			 table_block_index_data,
			 table_block_data_size - (size_t) ( table_block_index_data - table_block_data ),
			 0 );
		}
	}
#endif
	return( 1 );
}

/* Retrieves the number of table block values
 * Returns 1 if successful or -1 on error
 */
int libpff_table_block_get_number_of_values(
     libpff_table_block_t *table_block,
     int *number_of_values,
     libcerror_error_t **error )
{
	static char *function = "libpff_table_block_get_number_of_values";

	if( table_block == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid table block.",
		 function );

		return( -1 );
	}
	if( libpff_array_get_number_of_entries(
	     table_block->values_array,
	     number_of_values,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve number of values.",
		 function );

		return( -1 );
	}
	return( 1 );
}

/* Retrieves the table block value at the index
 * Returns 1 if successful or -1 on error
 */
int libpff_table_block_get_value(
     libpff_table_block_t *table_block,
     int value_index,
     libpff_table_block_value_t **table_block_value,
     libcerror_error_t **error )
{
	static char *function = "libpff_table_block_get_value";

	if( table_block == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid table block.",
		 function );

		return( -1 );
	}
	if( libpff_array_get_entry_by_index(
	     table_block->values_array,
	     value_index,
	     (intptr_t **) table_block_value,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve table block value: %d.",
		 function,
		 value_index );

		return( -1 );
	}
	return( 1 );
}

