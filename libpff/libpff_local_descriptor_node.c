/*
 * Local descriptor node functions
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

#include "libpff_definitions.h"
#include "libpff_libbfio.h"
#include "libpff_libcerror.h"
#include "libpff_libcnotify.h"
#include "libpff_libfmapi.h"
#include "libpff_local_descriptor_node.h"

#include "pff_local_descriptor_node.h"

/* Initializes the local descriptor node
 * Returns 1 if successful or -1 on error
 */
int libpff_local_descriptor_node_initialize(
     libpff_local_descriptor_node_t **local_descriptor_node,
     libcerror_error_t **error )
{
	static char *function = "libpff_local_descriptor_node_initialize";

	if( local_descriptor_node == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid local descriptor node.",
		 function );

		return( -1 );
	}
	if( *local_descriptor_node != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid local descriptor node value already set.",
		 function );

		return( -1 );
	}
	*local_descriptor_node = memory_allocate_structure(
	                          libpff_local_descriptor_node_t );

	if( *local_descriptor_node == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create local descriptor node.",
		 function );

		goto on_error;
	}
	if( memory_set(
	     *local_descriptor_node,
	     0,
	     sizeof( libpff_local_descriptor_node_t ) ) == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_SET_FAILED,
		 "%s: unable to clear local descriptor node.",
		 function );

		goto on_error;
	}
	return( 1 );

on_error:
	if( *local_descriptor_node != NULL )
	{
		memory_free(
		 *local_descriptor_node );

		*local_descriptor_node = NULL;
	}
	return( -1 );
}

/* Frees the local descriptor node
 * Returns 1 if successful or -1 on error
 */
int libpff_local_descriptor_node_free(
     libpff_local_descriptor_node_t **local_descriptor_node,
     libcerror_error_t **error )
{
	static char *function = "libpff_local_descriptor_node_free";

	if( local_descriptor_node == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid local descriptor node.",
		 function );

		return( -1 );
	}
	if( *local_descriptor_node != NULL )
	{
		if( ( *local_descriptor_node )->data != NULL )
		{
			memory_free(
			 ( *local_descriptor_node )->data );
		}
		memory_free(
		 *local_descriptor_node );

		*local_descriptor_node = NULL;
	}
	return( 1 );
}

/* Retrieves the data of a specific entry
 * Returns 1 if successful or -1 on error
 */
int libpff_local_descriptor_node_get_entry_data(
     libpff_local_descriptor_node_t *local_descriptor_node,
     uint16_t entry_index,
     uint8_t **entry_data,
     libcerror_error_t **error )
{
	static char *function = "libpff_local_descriptor_node_get_entry_data";
	size_t entry_offset   = 0;

	if( local_descriptor_node == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid local descriptor node.",
		 function );

		return( -1 );
	}
	if( local_descriptor_node->entries_data == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid local descriptor node - missing entries data.",
		 function );

		return( -1 );
	}
	if( entry_data == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid entry data.",
		 function );

		return( -1 );
	}
	entry_offset = (size_t) ( local_descriptor_node->entry_size * entry_index );

	if( entry_offset > (size_t) local_descriptor_node->maximum_entries_data_size )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_EXCEEDS_MAXIMUM,
		 "%s: entry index value exceeds maximum.",
		 function );

		return( -1 );
	}
	*entry_data = &( local_descriptor_node->entries_data[ entry_offset ] );

	return( 1 );
}

/* Reads an local descriptor node
 * Returns 1 if successful or -1 on error
 */
int libpff_local_descriptor_node_read(
     libpff_local_descriptor_node_t *local_descriptor_node,
     libbfio_handle_t *file_io_handle,
     off64_t node_offset,
     size32_t node_size,
     uint8_t file_type,
     libcerror_error_t **error )
{
	uint8_t *local_descriptor_node_data       = NULL;
	static char *function                     = "libpff_local_descriptor_node_read";
	size_t local_descriptor_node_header_size  = 0;
	ssize_t read_count                        = 0;
	uint8_t node_signature                    = 0;

#if defined( HAVE_DEBUG_OUTPUT )
	uint8_t *local_descriptor_node_entry_data = NULL;
	uint64_t value_64bit                      = 0;
	uint16_t entry_index                      = 0;
#endif

	if( local_descriptor_node == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid local descriptor node.",
		 function );

		return( -1 );
	}
	if( local_descriptor_node->data != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid local descriptor node - data already set.",
		 function );

		return( -1 );
	}
	if( node_offset < 0 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: invalid node offset value out of bounds.",
		 function );

		return( -1 );
	}
	if( node_size > (size32_t) SSIZE_MAX )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_EXCEEDS_MAXIMUM,
		 "%s: node size value exceeds maximum.",
		 function );

		return( -1 );
	}
	if( ( file_type != LIBPFF_FILE_TYPE_32BIT )
	 && ( file_type != LIBPFF_FILE_TYPE_64BIT ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_UNSUPPORTED_VALUE,
		 "%s: unsupported file type.",
		 function );

		return( -1 );
	}
	local_descriptor_node->data_size = (size_t) node_size;

	local_descriptor_node->data = (uint8_t *) memory_allocate(
	                                           sizeof( uint8_t ) * local_descriptor_node->data_size );

	if( local_descriptor_node->data == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create local descriptor node data.",
		 function );

		return( -1 );
	}
#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		libcnotify_printf(
		 "%s: reading local descriptor node data at offset: %" PRIi64 " (0x%08" PRIx64 ")\n",
		 function,
		 node_offset,
		 node_offset );
	}
#endif
	if( libbfio_handle_seek_offset(
	     file_io_handle,
	     node_offset,
	     SEEK_SET,
	     error ) == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_SEEK_FAILED,
		 "%s: unable to seek node offset: %" PRIi64 ".",
		 function,
		 node_offset );

		return( -1 );
	}
	read_count = libbfio_handle_read_buffer(
	              file_io_handle,
	              local_descriptor_node->data,
	              local_descriptor_node->data_size,
	              error );

	if( read_count != (ssize_t) local_descriptor_node->data_size )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_READ_FAILED,
		 "%s: unable to read local descriptor node data.",
		 function );

		return( -1 );
	}
#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		libcnotify_printf(
		 "%s: local descriptor node data:\n",
		 function );
		libcnotify_print_data(
		 local_descriptor_node->data,
		 local_descriptor_node->data_size,
		 0 );
	}
#endif
	local_descriptor_node_data = local_descriptor_node->data;

	/* Parse the local descriptors node data
	 */
	if( file_type == LIBPFF_FILE_TYPE_32BIT )
	{
		node_signature               = ( (pff_local_descriptor_node_32bit_t *) local_descriptor_node_data )->signature;
		local_descriptor_node->level = ( (pff_local_descriptor_node_32bit_t *) local_descriptor_node_data )->level;

		byte_stream_copy_to_uint16_little_endian(
		 ( (pff_local_descriptor_node_32bit_t *) local_descriptor_node_data )->number_of_entries,
		 local_descriptor_node->number_of_entries );
	}
	else if( file_type == LIBPFF_FILE_TYPE_64BIT )
	{
		node_signature               = ( (pff_local_descriptor_node_64bit_t *) local_descriptor_node_data )->signature;
		local_descriptor_node->level = ( (pff_local_descriptor_node_64bit_t *) local_descriptor_node_data )->level;

		byte_stream_copy_to_uint16_little_endian(
		 ( (pff_local_descriptor_node_64bit_t *) local_descriptor_node_data )->number_of_entries,
		 local_descriptor_node->number_of_entries );
	}
#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		libcnotify_printf(
		 "%s: local descriptors node signature\t\t: 0x%02" PRIx8 "\n",
		 function,
		 node_signature );
		libcnotify_printf(
		 "%s: local descriptors node level\t\t\t: %" PRIu8 "\n",
		 function,
		 local_descriptor_node->level );
		libcnotify_printf(
		 "%s: number of entries\t\t\t\t: %" PRIu16 "\n",
		 function,
		 local_descriptor_node->number_of_entries );

		if( file_type == LIBPFF_FILE_TYPE_64BIT )
		{
			libcnotify_printf(
			 "%s: padding1:\n",
			 function );
			libcnotify_print_data(
			 ( (pff_local_descriptor_node_64bit_t *) local_descriptor_node_data )->padding1,
			 4,
			 0 );
		}
	}
#endif
	if( node_signature != 0x02 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: unsupported local descriptors node signature: 0x%02" PRIx8 ".",
		 function,
		 node_signature );

		return( -1 );
	}
	if( file_type == LIBPFF_FILE_TYPE_32BIT )
	{
		local_descriptor_node_header_size = sizeof( pff_local_descriptor_node_32bit_t );

		if( local_descriptor_node->level == PFF_LOCAL_DESCRIPTOR_NODE_LEVEL_LEAF )
		{
			local_descriptor_node->entry_size = sizeof( pff_local_descriptor_leaf_node_entry_type_32bit_t );
		}
		else
		{
			local_descriptor_node->entry_size = sizeof( pff_local_descriptor_branch_node_entry_type_32bit_t );
		}
	}
	else if( file_type == LIBPFF_FILE_TYPE_64BIT )
	{
		local_descriptor_node_header_size = sizeof( pff_local_descriptor_node_64bit_t );

		if( local_descriptor_node->level == PFF_LOCAL_DESCRIPTOR_NODE_LEVEL_LEAF )
		{
			local_descriptor_node->entry_size = sizeof( pff_local_descriptor_leaf_node_entry_type_64bit_t );
		}
		else
		{
			local_descriptor_node->entry_size = sizeof( pff_local_descriptor_branch_node_entry_type_64bit_t );
		}
	}
	/* TODO additional sanity checks */

	if( local_descriptor_node->number_of_entries == 0 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: invalid number of entries.",
		 function );

		/* TODO implement error tollerance
		 */
		return( -1 );
	}
	local_descriptor_node->maximum_entries_data_size = local_descriptor_node->number_of_entries * (uint16_t) local_descriptor_node->entry_size;

	if( (size_t) local_descriptor_node->maximum_entries_data_size != ( local_descriptor_node->data_size - local_descriptor_node_header_size ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: mismatch in total size of the entries data and the size of the entries.",
		 function );

		/* TODO implement error tollerance
		 */
		return( -1 );
	}
	local_descriptor_node->entries_data = &( local_descriptor_node_data[ local_descriptor_node_header_size ] );

#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		local_descriptor_node_entry_data = local_descriptor_node->entries_data;

		for( entry_index = 0;
		     entry_index < local_descriptor_node->number_of_entries;
		     entry_index++ )
		{
			if( file_type == LIBPFF_FILE_TYPE_32BIT )
			{
				byte_stream_copy_to_uint32_little_endian(
				 local_descriptor_node_entry_data,
				 value_64bit );

				local_descriptor_node_entry_data += 4;
			}
			else if( file_type == LIBPFF_FILE_TYPE_64BIT )
			{
				byte_stream_copy_to_uint64_little_endian(
				 local_descriptor_node_entry_data,
				 value_64bit );

				local_descriptor_node_entry_data += 8;
			}
			libcnotify_printf(
			 "%s: entry: %03d identifier\t\t\t: 0x%08" PRIx64 " (%" PRIu64 ")\n",
			 function,
			 entry_index,
			 value_64bit,
			 value_64bit );

			if( local_descriptor_node->level == PFF_LOCAL_DESCRIPTOR_NODE_LEVEL_LEAF )
			{
				if( file_type == LIBPFF_FILE_TYPE_32BIT )
				{
					byte_stream_copy_to_uint32_little_endian(
					 local_descriptor_node_entry_data,
					 value_64bit );

					local_descriptor_node_entry_data += 4;
				}
				else if( file_type == LIBPFF_FILE_TYPE_64BIT )
				{
					byte_stream_copy_to_uint64_little_endian(
					 local_descriptor_node_entry_data,
					 value_64bit );

					local_descriptor_node_entry_data += 8;
				}
				libcnotify_printf(
				 "%s: entry: %03d data identifier\t\t\t: 0x%08" PRIx64 " (%" PRIu64 ")\n",
				 function,
				 entry_index,
				 value_64bit,
				 value_64bit );
			}
			if( file_type == LIBPFF_FILE_TYPE_32BIT )
			{
				byte_stream_copy_to_uint32_little_endian(
				 local_descriptor_node_entry_data,
				 value_64bit );

				local_descriptor_node_entry_data += 4;
			}
			else if( file_type == LIBPFF_FILE_TYPE_64BIT )
			{
				byte_stream_copy_to_uint64_little_endian(
				 local_descriptor_node_entry_data,
				 value_64bit );

				local_descriptor_node_entry_data += 8;
			}
			if( local_descriptor_node->level == PFF_LOCAL_DESCRIPTOR_NODE_LEVEL_LEAF )
			{
				libcnotify_printf(
				 "%s: entry: %03d local descriptors identifier\t: 0x%08" PRIx64 " (%" PRIu64 ")\n",
				 function,
				 entry_index,
				 value_64bit,
				 value_64bit );
			}
			else
			{
				libcnotify_printf(
				 "%s: entry: %03d sub node identifier\t\t: 0x%08" PRIx64 " (%" PRIu64 ")\n",
				 function,
				 entry_index,
				 value_64bit,
				 value_64bit );
			}
		}
		libcnotify_printf(
		 "\n" );
	}
#endif
	return( 1 );
}

