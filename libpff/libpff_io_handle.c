/*
 * Input/Output (IO) handle functions
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

#include "libpff_allocation_table.h"
#include "libpff_codepage.h"
#include "libpff_data_array.h"
#include "libpff_data_block.h"
#include "libpff_debug.h"
#include "libpff_definitions.h"
#include "libpff_index.h"
#include "libpff_index_node.h"
#include "libpff_index_tree.h"
#include "libpff_index_value.h"
#include "libpff_item_descriptor.h"
#include "libpff_item_tree.h"
#include "libpff_io_handle.h"
#include "libpff_libbfio.h"
#include "libpff_libcerror.h"
#include "libpff_libcnotify.h"
#include "libpff_libfcache.h"
#include "libpff_libfdata.h"
#include "libpff_libfmapi.h"
#include "libpff_local_descriptor_node.h"
#include "libpff_local_descriptor_value.h"
#include "libpff_local_descriptors.h"
#include "libpff_local_descriptors_tree.h"
#include "libpff_offset_list.h"
#include "libpff_tree_type.h"
#include "libpff_unused.h"

#include "pff_block.h"
#include "pff_file_header.h"
#include "pff_index_node.h"
#include "pff_local_descriptor_node.h"

#define LIBPFF_IO_HANDLE_DESCRIPTOR_INDEX_TREE_ROOT_OFFSET		1
#define LIBPFF_IO_HANDLE_OFFSET_INDEX_TREE_ROOT_OFFSET			2
#define LIBPFF_IO_HANDLE_RECOVERED_DESCRIPTOR_INDEX_TREE_ROOT_OFFSET	3
#define LIBPFF_IO_HANDLE_RECOVERED_OFFSET_INDEX_TREE_ROOT_OFFSET	4

const uint8_t pff_file_signature[ 4 ] = { 0x21, 0x42, 0x44, 0x4e };

/* Initialize an IO handle
 * Make sure the value io_handle is pointing to is set to NULL
 * Returns 1 if successful or -1 on error
 */
int libpff_io_handle_initialize(
     libpff_io_handle_t **io_handle,
     libcerror_error_t **error )
{
	static char *function = "libpff_io_handle_initialize";

	if( io_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid IO handle.",
		 function );

		return( -1 );
	}
	if( *io_handle != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid IO handle value already set.",
		 function );

		return( -1 );
	}
	*io_handle = memory_allocate_structure(
	              libpff_io_handle_t );

	if( *io_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create IO handle.",
		 function );

		goto on_error;
	}
	if( memory_set(
	     *io_handle,
	     0,
	     sizeof( libpff_io_handle_t ) ) == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_SET_FAILED,
		 "%s: unable to clear file.",
		 function );

		goto on_error;
	}
	( *io_handle )->ascii_codepage = LIBPFF_CODEPAGE_WINDOWS_1252;

	return( 1 );

on_error:
	if( *io_handle != NULL )
	{
		memory_free(
		 *io_handle );

		*io_handle = NULL;
	}
	return( -1 );
}

/* Frees an IO handle
 * Returns 1 if successful or -1 on error
 */
int libpff_io_handle_free(
     libpff_io_handle_t **io_handle,
     libcerror_error_t **error )
{
	static char *function = "libpff_io_handle_free";
	int result            = 1;

	if( io_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid IO handle.",
		 function );

		return( -1 );
	}
	if( *io_handle != NULL )
	{
		if( ( *io_handle )->index_nodes_vector != NULL )
		{
			if( libfdata_vector_free(
			     &( ( *io_handle )->index_nodes_vector ),
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
				 "%s: unable to free index nodes vector.",
				 function );

				result = -1;
			}
		}
		if( ( *io_handle )->index_nodes_cache != NULL )
		{
			if( libfcache_cache_free(
			     &( ( *io_handle )->index_nodes_cache ),
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
				 "%s: unable to free index nodes cache.",
				 function );

				result = -1;
			}
		}
		if( ( *io_handle )->descriptor_index_tree != NULL )
		{
			if( libfdata_tree_free(
			     &( ( *io_handle )->descriptor_index_tree ),
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
				 "%s: unable to free descriptor index tree.",
				 function );

				result = -1;
			}
		}
		if( ( *io_handle )->offset_index_tree != NULL )
		{
			if( libfdata_tree_free(
			     &( ( *io_handle )->offset_index_tree ),
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
				 "%s: unable to free offset index tree.",
				 function );

				result = -1;
			}
		}
		if( ( *io_handle )->recovered_descriptor_index_tree != NULL )
		{
			if( libfdata_tree_free(
			     &( ( *io_handle )->recovered_descriptor_index_tree ),
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
				 "%s: unable to free recovered descriptor index tree.",
				 function );

				result = -1;
			}
		}
		if( ( *io_handle )->recovered_offset_index_tree != NULL )
		{
			if( libfdata_tree_free(
			     &( ( *io_handle )->recovered_offset_index_tree ),
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
				 "%s: unable to free recovered offset index tree.",
				 function );

				result = -1;
			}
		}
		if( ( *io_handle )->descriptor_index_tree_cache != NULL )
		{
			if( libfcache_cache_free(
			     &( ( *io_handle )->descriptor_index_tree_cache ),
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
				 "%s: unable to free descriptor index tree cache.",
				 function );

				result = -1;
			}
		}
		if( ( *io_handle )->offset_index_tree_cache != NULL )
		{
			if( libfcache_cache_free(
			     &( ( *io_handle )->offset_index_tree_cache ),
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
				 "%s: unable to free offset index tree cache.",
				 function );

				result = -1;
			}
		}
		memory_free(
		 *io_handle );

		*io_handle = NULL;
	}
	return( result );
}

/* Reads the file header
 * Returns 1 if successful or -1 on error
 */
int libpff_io_handle_read_file_header(
     libpff_io_handle_t *io_handle,
     libbfio_handle_t *file_io_handle,
     int *file_content_type,
     libcerror_error_t **error )
{
	libpff_index_t *descriptor_index       = NULL;
	libpff_index_t *offset_index           = NULL;
	uint8_t *file_header                   = NULL;
	uint8_t *file_header_data              = NULL;
	static char *function                  = "libpff_io_handle_read_file_header";
	size_t data_size                       = 564;
	size_t read_size                       = 564;
	ssize_t read_count                     = 0;
	uint64_t descriptor_index_back_pointer = 0;
	uint64_t offset_index_back_pointer     = 0;
	uint32_t calculated_crc                = 0;
	uint32_t stored_crc                    = 0;
	uint16_t content_type                  = 0;
	uint16_t data_version                  = 0;
	uint8_t sentinal                       = 0;

#if defined( HAVE_DEBUG_OUTPUT )
	uint8_t *value_data                    = 0;
	uint64_t value_64bit                   = 0;
	uint32_t value_32bit                   = 0;
	uint16_t value_16bit                   = 0;
	int value_iterator                     = 0;
#endif

	if( io_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid IO handle.",
		 function );

		return( -1 );
	}
	if( io_handle->descriptor_index_tree != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid IO handle - descriptor index tree value already set.",
		 function );

		return( -1 );
	}
	if( io_handle->offset_index_tree != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid IO handle - offset index tree value already set.",
		 function );

		return( -1 );
	}
	if( file_content_type == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid file content type.",
		 function );

		return( -1 );
	}
#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		libcnotify_printf(
		 "%s: reading file header at offset: 0 (0x00000000)\n",
		 function );
	}
#endif
	if( libbfio_handle_seek_offset(
	     file_io_handle,
	     0,
	     SEEK_SET,
	     error ) == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_SEEK_FAILED,
		 "%s: unable to seek file header offset: 0.",
		 function );

		goto on_error;
	}
	file_header = (uint8_t *) memory_allocate(
	                           sizeof( uint8_t ) * read_size );

	if( file_header == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create file header.",
		 function );

		goto on_error;
	}
	read_count = libbfio_handle_read_buffer(
	              file_io_handle,
	              file_header,
	              read_size,
	              error );

	if( read_count != (ssize_t) read_size )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_READ_FAILED,
		 "%s: unable to read file header data.",
		 function );

		goto on_error;
	}
	file_header_data = &( file_header[ sizeof( pff_file_header_t ) ] );

#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		libcnotify_printf(
		 "%s: file header:\n",
		 function );
		libcnotify_print_data(
		 file_header,
		 sizeof( pff_file_header_t ),
		 0 );
	}
#endif
	if( memory_compare(
	     ( (pff_file_header_t *) file_header )->signature,
	     pff_file_signature,
	     4 ) != 0 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
		 "%s: invalid file signature.",
		 function );

		goto on_error;
	}
	byte_stream_copy_to_uint32_little_endian(
	 ( (pff_file_header_t *) file_header )->crc,
	 stored_crc );

	byte_stream_copy_to_uint16_little_endian(
	 ( (pff_file_header_t *) file_header )->content_type,
	 content_type );

	if( content_type == PFF_FILE_HEADER_CONTENT_TYPE_PAB )
	{
		*file_content_type = LIBPFF_FILE_CONTENT_TYPE_PAB;
	}
	else if( content_type == PFF_FILE_HEADER_CONTENT_TYPE_PST )
	{
		*file_content_type = LIBPFF_FILE_CONTENT_TYPE_PST;
	}
	else if( content_type == PFF_FILE_HEADER_CONTENT_TYPE_OST )
	{
		*file_content_type = LIBPFF_FILE_CONTENT_TYPE_OST;
	}
	else
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
		 "%s: unsupported content type: 0x%04" PRIx16 ".",
		 function,
		 content_type );

		goto on_error;
	}
	byte_stream_copy_to_uint16_little_endian(
	 ( (pff_file_header_t *) file_header )->data_version,
	 data_version );

	if( data_version <= 0x000f )
	{
		data_size            = sizeof( pff_file_header_data_32bit_t );
		io_handle->file_type = LIBPFF_FILE_TYPE_32BIT;
	}
	else if( data_version >= 0x0015 )
	{
		data_size            = sizeof( pff_file_header_data_64bit_t );
		io_handle->file_type = LIBPFF_FILE_TYPE_64BIT;
	}
	else
	{
		if( ( ( (pff_file_header_data_32bit_t *) file_header_data )->sentinal == 0x80 )
		 && ( ( (pff_file_header_data_64bit_t *) file_header_data )->sentinal != 0x80 ) )
		{
#if defined( HAVE_DEBUG_OUTPUT )
			if( libcnotify_verbose != 0 )
			{
				libcnotify_printf(
				 "%s: detected 32-bit file type for unsupported data version: 0x%04" PRIx16 ".\n",
				 function,
				 data_version );
			}
#endif
			data_size            = sizeof( pff_file_header_data_32bit_t );
			io_handle->file_type = LIBPFF_FILE_TYPE_32BIT;
		}
		else if( ( ( (pff_file_header_data_32bit_t *) file_header_data )->sentinal != 0x80 )
		      && ( ( (pff_file_header_data_64bit_t *) file_header_data )->sentinal == 0x80 ) )
		{
#if defined( HAVE_DEBUG_OUTPUT )
			if( libcnotify_verbose != 0 )
			{
				libcnotify_printf(
				 "%s: detected 64-bit file type for unsupported data version: 0x%04" PRIx16 ".\n",
				 function,
				 data_version );
			}
#endif
			data_size            = sizeof( pff_file_header_data_64bit_t );
			io_handle->file_type = LIBPFF_FILE_TYPE_64BIT;
		}
#if defined( HAVE_DEBUG_OUTPUT )
		else if( libcnotify_verbose != 0 )
		{
			libcnotify_printf(
			 "%s: unsupported data version: 0x%04" PRIx16 ".\n",
			 function,
			 data_version );
		}
#endif
	}
	if( ( io_handle->file_type != LIBPFF_FILE_TYPE_32BIT )
	 && ( io_handle->file_type != LIBPFF_FILE_TYPE_64BIT ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
		 "%s: unable to determine file type.",
		 function );

		goto on_error;
	}
#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		libcnotify_printf(
		 "%s: signature\t\t\t\t: %c%c%c%c\n",
		 function,
		 ( (pff_file_header_t *) file_header )->signature[ 0 ],
		 ( (pff_file_header_t *) file_header )->signature[ 1 ],
		 ( (pff_file_header_t *) file_header )->signature[ 2 ],
		 ( (pff_file_header_t *) file_header )->signature[ 3 ] );

		libcnotify_printf(
		 "%s: crc\t\t\t\t\t: 0x%08" PRIx32 "\n",
		 function,
		 stored_crc );

		libcnotify_printf(
		 "%s: content type\t\t\t\t: %c%c\n",
		 function,
		 ( (pff_file_header_t *) file_header )->content_type[ 0 ],
		 ( (pff_file_header_t *) file_header )->content_type[ 1 ] );

		libcnotify_printf(
		 "%s: data version\t\t\t\t: %" PRIu16 "\n",
		 function,
		 data_version );
		byte_stream_copy_to_uint16_little_endian(
		 ( (pff_file_header_t *) file_header )->content_version,
		 value_16bit );

		libcnotify_printf(
		 "%s: content version\t\t\t: %" PRIu16 "\n",
		 function,
		 value_16bit );

		libcnotify_printf(
		 "%s: creation platform\t\t\t: 0x%02" PRIx8 "\n",
		 function,
		 ( (pff_file_header_t *) file_header )->creation_platform );

		libcnotify_printf(
		 "%s: access platform\t\t\t: 0x%02" PRIx8 "\n",
		 function,
		 ( (pff_file_header_t *) file_header )->access_platform );

		byte_stream_copy_to_uint32_little_endian(
		 ( (pff_file_header_t *) file_header )->unknown1,
		 value_32bit );
		libcnotify_printf(
		 "%s: unknown1\t\t\t\t: 0x%08" PRIx32 " (%" PRIu32 ")\n",
		 function,
		 value_32bit,
		 value_32bit );

		byte_stream_copy_to_uint32_little_endian(
		 ( (pff_file_header_t *) file_header )->unknown2,
		 value_32bit );
		libcnotify_printf(
		 "%s: unknown2\t\t\t\t: 0x%08" PRIx32 " (%" PRIu32 ")\n",
		 function,
		 value_32bit,
		 value_32bit );

		libcnotify_printf(
		 "\n" );
	}
#endif
	if( libfmapi_crc32_weak_calculate(
	     &calculated_crc,
	     &( file_header[ 8 ] ),
	     471,
	     0,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GENERIC,
		 "%s: unable to calculate crc.",
		 function );

		goto on_error;
	}
	if( stored_crc != calculated_crc )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_INPUT,
		 LIBCERROR_INPUT_ERROR_CHECKSUM_MISMATCH,
		 "%s: mismatch in file header crc ( %" PRIu32 " != %" PRIu32 " ).",
		 function,
		 stored_crc,
		 calculated_crc );

		goto on_error;
	}
#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		libcnotify_printf(
		 "%s: file header data:\n",
		 function );
		libcnotify_print_data(
		 file_header_data,
		 data_size,
		 0 );
	}
#endif
	if( io_handle->file_type == LIBPFF_FILE_TYPE_32BIT )
	{
		byte_stream_copy_to_uint32_little_endian(
		 ( (pff_file_header_data_32bit_t *) file_header_data )->file_size,
		 io_handle->file_size );

		byte_stream_copy_to_uint32_little_endian(
		 ( (pff_file_header_data_32bit_t *) file_header_data )->descriptor_index_back_pointer,
		 descriptor_index_back_pointer );
		byte_stream_copy_to_uint32_little_endian(
		 ( (pff_file_header_data_32bit_t *) file_header_data )->descriptor_index_root_node_offset,
		 io_handle->descriptor_index_root_node_offset );
		byte_stream_copy_to_uint32_little_endian(
		 ( (pff_file_header_data_32bit_t *) file_header_data )->offset_index_back_pointer,
		 offset_index_back_pointer );
		byte_stream_copy_to_uint32_little_endian(
		 ( (pff_file_header_data_32bit_t *) file_header_data )->offset_index_root_node_offset,
		 io_handle->offset_index_root_node_offset );

		sentinal                   = ( (pff_file_header_data_32bit_t *) file_header_data )->sentinal;
		io_handle->encryption_type = ( (pff_file_header_data_32bit_t *) file_header_data )->encryption_type;
	}
	else if( io_handle->file_type == LIBPFF_FILE_TYPE_64BIT )
	{
		byte_stream_copy_to_uint64_little_endian(
		 ( (pff_file_header_data_64bit_t *) file_header_data )->file_size,
		 io_handle->file_size );

		byte_stream_copy_to_uint64_little_endian(
		 ( (pff_file_header_data_64bit_t *) file_header_data )->descriptor_index_back_pointer,
		 descriptor_index_back_pointer );
		byte_stream_copy_to_uint64_little_endian(
		 ( (pff_file_header_data_64bit_t *) file_header_data )->descriptor_index_root_node_offset,
		 io_handle->descriptor_index_root_node_offset );
		byte_stream_copy_to_uint64_little_endian(
		 ( (pff_file_header_data_64bit_t *) file_header_data )->offset_index_back_pointer,
		 offset_index_back_pointer );
		byte_stream_copy_to_uint64_little_endian(
		 ( (pff_file_header_data_64bit_t *) file_header_data )->offset_index_root_node_offset,
		 io_handle->offset_index_root_node_offset );

		sentinal                   = ( (pff_file_header_data_64bit_t *) file_header_data )->sentinal;
		io_handle->encryption_type = ( (pff_file_header_data_64bit_t *) file_header_data )->encryption_type;

		byte_stream_copy_to_uint32_little_endian(
		 ( (pff_file_header_data_64bit_t *) file_header_data )->crc,
		 stored_crc );
	}
#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		if( io_handle->file_type == LIBPFF_FILE_TYPE_32BIT )
		{
			byte_stream_copy_to_uint32_little_endian(
			 ( (pff_file_header_data_32bit_t *) file_header_data )->next_index_pointer,
			 value_32bit );
			libcnotify_printf(
			 "%s: next index pointer\t\t\t: %" PRIu32 "\n",
			 function,
			 value_32bit );

			byte_stream_copy_to_uint32_little_endian(
			 ( (pff_file_header_data_32bit_t *) file_header_data )->next_index_back_pointer,
			 value_32bit );
			libcnotify_printf(
			 "%s: next index back pointer\t\t: %" PRIu32 "\n",
			 function,
			 value_32bit );

			byte_stream_copy_to_uint32_little_endian(
			 ( (pff_file_header_data_32bit_t *) file_header_data )->seed_value,
			 value_32bit );
			libcnotify_printf(
			 "%s: seed value\t\t\t\t: 0x%08" PRIx32 "\n",
			 function,
			 value_32bit );

			value_data = ( (pff_file_header_data_32bit_t *) file_header_data )->descriptor_index_high_water_marks;

			for( value_iterator = 0;
			     value_iterator < 32;
			     value_iterator++ )
			{
				byte_stream_copy_to_uint32_little_endian(
				 value_data,
				 value_32bit );
				libcnotify_printf(
				 "%s: high water mark: %02d value\t\t: %" PRIu32 "\n",
				 function,
				 value_iterator,
				 value_32bit );

				value_data += 4;
			}
			libcnotify_printf(
			 "\n" );
		}
		else if( io_handle->file_type == LIBPFF_FILE_TYPE_64BIT )
		{
			libcnotify_printf(
			 "%s: unknown3:\n",
			 function );
			libcnotify_print_data(
			 ( (pff_file_header_data_64bit_t *) file_header_data )->unknown3,
			 8,
			 0 );

			byte_stream_copy_to_uint64_little_endian(
			 ( (pff_file_header_data_64bit_t *) file_header_data )->next_index_back_pointer,
			 value_64bit );
			libcnotify_printf(
			 "%s: next index back pointer\t\t: %" PRIu64 "\n",
			 function,
			 value_64bit );

			byte_stream_copy_to_uint32_little_endian(
			 ( (pff_file_header_data_64bit_t *) file_header_data )->seed_value,
			 value_32bit );
			libcnotify_printf(
			 "%s: seed value\t\t\t\t: 0x%08" PRIx32 "\n",
			 function,
			 value_32bit );

			value_data = ( (pff_file_header_data_64bit_t *) file_header_data )->descriptor_index_high_water_marks;

			for( value_iterator = 0;
			     value_iterator < 32;
			     value_iterator++ )
			{
				byte_stream_copy_to_uint32_little_endian(
				 value_data,
				 value_32bit );
				libcnotify_printf(
				 "%s: high water mark: %02d value\t\t: %" PRIu32 "\n",
				 function,
				 value_iterator,
				 value_32bit );

				value_data += 4;
			}
			libcnotify_printf(
			 "\n" );
			libcnotify_printf(
			 "%s: unknown4:\n",
			 function );
			libcnotify_print_data(
			 ( (pff_file_header_data_64bit_t *) file_header_data )->unknown4,
			 8,
			 0 );
		}
		libcnotify_printf(
		 "%s: file header data root:\n",
		 function );

		if( io_handle->file_type == LIBPFF_FILE_TYPE_32BIT )
		{
			libcnotify_printf(
			 "%s: unknown5:\n",
			 function );
			libcnotify_print_data(
			 ( (pff_file_header_data_32bit_t *) file_header_data )->unknown5,
			 4,
			 0 );
		}
		else if( io_handle->file_type == LIBPFF_FILE_TYPE_64BIT )
		{
			libcnotify_printf(
			 "%s: unknown5:\n",
			 function );
			libcnotify_print_data(
			 ( (pff_file_header_data_64bit_t *) file_header_data )->unknown5,
			 4,
			 0 );
		}
		libcnotify_printf(
		 "%s: file size\t\t\t\t: %" PRIu64 "\n",
		 function,
		 io_handle->file_size );

		if( io_handle->file_type == LIBPFF_FILE_TYPE_32BIT )
		{
			byte_stream_copy_to_uint32_little_endian(
			 ( (pff_file_header_data_32bit_t *) file_header_data )->last_data_allocation_table_offset,
			 value_32bit );
			libcnotify_printf(
			 "%s: last data allocation table offset\t: %" PRIu32 "\n",
			 function,
			 value_32bit );

			byte_stream_copy_to_uint32_little_endian(
			 ( (pff_file_header_data_32bit_t *) file_header_data )->total_available_data_size,
			 value_32bit );
			libcnotify_printf(
			 "%s: total available data size\t\t: %" PRIu32 "\n",
			 function,
			 value_32bit );

			byte_stream_copy_to_uint32_little_endian(
			 ( (pff_file_header_data_32bit_t *) file_header_data )->total_available_page_size,
			 value_32bit );
			libcnotify_printf(
			 "%s: total available page size\t\t: %" PRIu32 "\n",
			 function,
			 value_32bit );
		}
		else if( io_handle->file_type == LIBPFF_FILE_TYPE_64BIT )
		{
			byte_stream_copy_to_uint64_little_endian(
			 ( (pff_file_header_data_64bit_t *) file_header_data )->last_data_allocation_table_offset,
			 value_64bit );
			libcnotify_printf(
			 "%s: last data allocation table offset\t: %" PRIu64 "\n",
			 function,
			 value_64bit );

			byte_stream_copy_to_uint64_little_endian(
			 ( (pff_file_header_data_64bit_t *) file_header_data )->total_available_data_size,
			 value_64bit );
			libcnotify_printf(
			 "%s: total available data size\t\t: %" PRIu64 "\n",
			 function,
			 value_64bit );

			byte_stream_copy_to_uint64_little_endian(
			 ( (pff_file_header_data_64bit_t *) file_header_data )->total_available_page_size,
			 value_64bit );
			libcnotify_printf(
			 "%s: total available page size\t\t: %" PRIu64 "\n",
			 function,
			 value_64bit );
		}
		libcnotify_printf(
		 "%s: descriptor index root node offset\t: %" PRIi64 "\n",
		 function,
		 io_handle->descriptor_index_root_node_offset );

		libcnotify_printf(
		 "%s: descriptor index back pointer\t: %" PRIu64 "\n",
		 function,
		 descriptor_index_back_pointer );

		libcnotify_printf(
		 "%s: offset index root node offset\t: %" PRIi64 "\n",
		 function,
		 io_handle->offset_index_root_node_offset );

		libcnotify_printf(
		 "%s: offset index back pointer\t\t: %" PRIu64 "\n",
		 function,
		 offset_index_back_pointer );

		if( io_handle->file_type == LIBPFF_FILE_TYPE_32BIT )
		{
			libcnotify_printf(
			 "%s: allocation table validation type\t: 0x%02" PRIx8 "\n",
			 function,
			 ( (pff_file_header_data_32bit_t *) file_header_data )->allocation_table_validation_type );

			libcnotify_printf(
			 "%s: unknown6:\n",
			 function );
			libcnotify_print_data(
			 ( (pff_file_header_data_32bit_t *) file_header_data )->unknown6,
			 3,
			 0 );

			libcnotify_printf(
			 "%s: initial data free map:\n",
			 function );
			libcnotify_print_data(
			 ( (pff_file_header_data_32bit_t *) file_header_data )->initial_data_free_map,
			 128,
			 0 );

			libcnotify_printf(
			 "%s: initial page free map:\n",
			 function );
			libcnotify_print_data(
			 ( (pff_file_header_data_32bit_t *) file_header_data )->initial_page_free_map,
			 128,
			 0 );
		}
		else if( io_handle->file_type == LIBPFF_FILE_TYPE_64BIT )
		{
			libcnotify_printf(
			 "%s: allocation table validation type\t: 0x%02" PRIx8 "\n",
			 function,
			 ( (pff_file_header_data_64bit_t *) file_header_data )->allocation_table_validation_type );

			libcnotify_printf(
			 "%s: unknown6:\n",
			 function );
			libcnotify_print_data(
			 ( (pff_file_header_data_64bit_t *) file_header_data )->unknown6,
			 3,
			 0 );

			libcnotify_printf(
			 "%s: unknown7:\n",
			 function );
			libcnotify_print_data(
			 ( (pff_file_header_data_64bit_t *) file_header_data )->unknown7,
			 3,
			 0 );

			libcnotify_printf(
			 "%s: initial data free map:\n",
			 function );
			libcnotify_print_data(
			 ( (pff_file_header_data_64bit_t *) file_header_data )->initial_data_free_map,
			 128,
			 0 );

			libcnotify_printf(
			 "%s: initial page free map:\n",
			 function );
			libcnotify_print_data(
			 ( (pff_file_header_data_64bit_t *) file_header_data )->initial_page_free_map,
			 128,
			 0 );
		}
		libcnotify_printf(
		 "%s: sentinal\t\t\t\t: 0x%02" PRIx8 "\n",
		 function,
		 sentinal );

		libcnotify_printf(
		 "%s: encryption type\t\t\t: 0x%02" PRIx8 "\n",
		 function,
		 io_handle->encryption_type );

		if( io_handle->file_type == LIBPFF_FILE_TYPE_32BIT )
		{
			libcnotify_printf(
			 "%s: unknown8:\n",
			 function );
			libcnotify_print_data(
			 ( (pff_file_header_data_32bit_t *) file_header_data )->unknown8,
			 2,
			 0 );

			libcnotify_printf(
			 "%s: unknown9:\n",
			 function );
			libcnotify_print_data(
			 ( (pff_file_header_data_32bit_t *) file_header_data )->unknown9,
			 8,
			 0 );

			libcnotify_printf(
			 "%s: unknown10:\n",
			 function );
			libcnotify_print_data(
			 ( (pff_file_header_data_32bit_t *) file_header_data )->unknown10,
			 4,
			 0 );

			libcnotify_printf(
			 "%s: unknown11:\n",
			 function );
			libcnotify_print_data(
			 ( (pff_file_header_data_32bit_t *) file_header_data )->unknown11,
			 3,
			 0 );

			libcnotify_printf(
			 "%s: unknown12:\n",
			 function );
			libcnotify_print_data(
			 &( ( (pff_file_header_data_32bit_t *) file_header_data )->unknown12 ),
			 1,
			 0 );

			libcnotify_printf(
			 "%s: unknown13:\n",
			 function );
			libcnotify_print_data(
			 ( (pff_file_header_data_32bit_t *) file_header_data )->unknown13,
			 32,
			 0 );
		}
		else if( io_handle->file_type == LIBPFF_FILE_TYPE_64BIT )
		{
			libcnotify_printf(
			 "%s: unknown8:\n",
			 function );
			libcnotify_print_data(
			 ( (pff_file_header_data_64bit_t *) file_header_data )->unknown8,
			 2,
			 0 );

			byte_stream_copy_to_uint64_little_endian(
			 ( (pff_file_header_data_64bit_t *) file_header_data )->next_index_pointer,
			 value_32bit );
			libcnotify_printf(
			 "%s: next index pointer\t\t\t: %" PRIu64 "\n",
			 function,
			 value_64bit );

			libcnotify_printf(
			 "%s: crc\t\t\t\t\t: 0x%08" PRIx64 "\n",
			 function,
			 stored_crc );

			libcnotify_printf(
			 "%s: unknown11:\n",
			 function );
			libcnotify_print_data(
			 ( (pff_file_header_data_64bit_t *) file_header_data )->unknown11,
			 3,
			 0 );

			libcnotify_printf(
			 "%s: unknown12:\n",
			 function );
			libcnotify_print_data(
			 &( ( (pff_file_header_data_32bit_t *) file_header_data )->unknown12 ),
			 1,
			 0 );

			libcnotify_printf(
			 "%s: unknown13:\n",
			 function );
			libcnotify_print_data(
			 ( (pff_file_header_data_64bit_t *) file_header_data )->unknown13,
			 32,
			 0 );
		}
	}
#endif
	if( io_handle->file_type == LIBPFF_FILE_TYPE_64BIT )
	{
		if( libfmapi_crc32_weak_calculate(
		     &calculated_crc,
		     &( file_header[ 8 ] ),
		     516,
		     0,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GENERIC,
			 "%s: unable to calculate crc.",
			 function );

			goto on_error;
		}
		if( stored_crc != calculated_crc )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_INPUT,
			 LIBCERROR_INPUT_ERROR_CHECKSUM_MISMATCH,
			 "%s: mismatch in file header crc ( %" PRIu32 " != %" PRIu32 " ).",
			 function,
			 stored_crc,
			 calculated_crc );

			goto on_error;
		}
	}
	memory_free(
	 file_header );

	file_header = NULL;

	if( ( io_handle->encryption_type != LIBPFF_ENCRYPTION_TYPE_NONE )
	 && ( io_handle->encryption_type != LIBPFF_ENCRYPTION_TYPE_COMPRESSIBLE )
	 && ( io_handle->encryption_type != LIBPFF_ENCRYPTION_TYPE_HIGH ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
		 "%s: unsupported encryption type: 0x%02x",
		 function,
		 io_handle->encryption_type );

		goto on_error;
	}
	if( io_handle->index_nodes_vector == NULL )
	{
		/* TODO free and clone function ? */

		if( libfdata_vector_initialize(
		     &( io_handle->index_nodes_vector ),
		     512,
		     (intptr_t *) io_handle,
		     NULL,
		     NULL,
		     &libpff_io_handle_read_index_node,
		     LIBFDATA_FLAG_IO_HANDLE_NON_MANAGED,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
			 "%s: unable to create index nodes vector.",
			 function );

			goto on_error;
		}
		if( libfdata_vector_append_segment(
		     io_handle->index_nodes_vector,
		     0,
		     io_handle->file_size,
		     0,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_APPEND_FAILED,
			 "%s: unable to create append segment to nodes vector.",
			 function );

			goto on_error;
		}
	}
	if( io_handle->index_nodes_cache == NULL )
	{
		if( libfcache_cache_initialize(
		     &( io_handle->index_nodes_cache ),
		     LIBPFF_MAXIMUM_CACHE_ENTRIES_INDEX_NODES,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
			 "%s: unable to create index nodes cache.",
			 function );

			goto on_error;
		}
	}
	/* Create the descriptor index tree
	 */
	if( libpff_index_initialize(
	     &descriptor_index,
	     io_handle,
	     LIBPFF_INDEX_TYPE_DESCRIPTOR,
	     io_handle->descriptor_index_root_node_offset,
	     descriptor_index_back_pointer,
	     0,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create descriptor index.",
		 function );

		goto on_error;
	}
	if( libfdata_tree_initialize(
	     &( io_handle->descriptor_index_tree ),
	     (intptr_t *) descriptor_index,
	     (int (*)(intptr_t **, libcerror_error_t **)) &libpff_index_free,
	     (int (*)(intptr_t **, intptr_t *, libcerror_error_t **)) &libpff_index_clone,
	     &libpff_index_read_node_data,
	     &libpff_index_read_sub_nodes,
	     LIBFDATA_FLAG_IO_HANDLE_MANAGED,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create descriptor index tree",
		 function );

		goto on_error;
	}
	descriptor_index = NULL;

	/* Point the root node data offset to LIBPFF_IO_HANDLE_DESCRIPTOR_INDEX_TREE_ROOT_OFFSET
	 * otherwise it will mess up the caching of the first index value
	 */
	if( libfdata_tree_set_root_node(
	     io_handle->descriptor_index_tree,
	     LIBPFF_IO_HANDLE_DESCRIPTOR_INDEX_TREE_ROOT_OFFSET,
	     0,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to set descriptor index tree root node.",
		 function );

		goto on_error;
	}
	if( io_handle->descriptor_index_tree_cache == NULL )
	{
		if( libfcache_cache_initialize(
		     &( io_handle->descriptor_index_tree_cache ),
		     LIBPFF_MAXIMUM_CACHE_ENTRIES_DESCRIPTOR_INDEX_VALUES,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
			 "%s: unable to create descriptor index tree cache.",
			 function );

			goto on_error;
		}
	}
	/* Create the offset index tree
	 */
	if( libpff_index_initialize(
	     &offset_index,
	     io_handle,
	     LIBPFF_INDEX_TYPE_OFFSET,
	     io_handle->offset_index_root_node_offset,
	     offset_index_back_pointer,
	     0,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create offset index.",
		 function );

		goto on_error;
	}
	if( libfdata_tree_initialize(
	     &( io_handle->offset_index_tree ),
	     (intptr_t *) offset_index,
	     (int (*)(intptr_t **, libcerror_error_t **)) &libpff_index_free,
	     (int (*)(intptr_t **, intptr_t *, libcerror_error_t **)) &libpff_index_clone,
	     &libpff_index_read_node_data,
	     &libpff_index_read_sub_nodes,
	     LIBFDATA_FLAG_IO_HANDLE_MANAGED,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create offset index tree",
		 function );

		goto on_error;
	}
	offset_index = NULL;

	/* Point the root node data offset to LIBPFF_IO_HANDLE_OFFSET_INDEX_TREE_ROOT_OFFSET
	 * otherwise it will mess up the caching of the first index value
	 */
	if( libfdata_tree_set_root_node(
	     io_handle->offset_index_tree,
	     LIBPFF_IO_HANDLE_OFFSET_INDEX_TREE_ROOT_OFFSET,
	     0,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to set offset index tree root node.",
		 function );

		goto on_error;
	}
	if( io_handle->offset_index_tree_cache == NULL )
	{
		if( libfcache_cache_initialize(
		     &( io_handle->offset_index_tree_cache ),
		     LIBPFF_MAXIMUM_CACHE_ENTRIES_OFFSET_INDEX_VALUES,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
			 "%s: unable to create offset index tree cache.",
			 function );

			goto on_error;
		}
	}
	return( 1 );

on_error:
	if( offset_index != NULL )
	{
		libpff_index_free(
		 &offset_index,
		 NULL );
	}
	if( descriptor_index != NULL )
	{
		libpff_index_free(
		 &descriptor_index,
		 NULL );
	}
	if( file_header != NULL )
	{
		memory_free(
		 file_header );
	}
	return( -1 );
}

/* Reads the unallocated data blocks
 * Returns 1 if successful or -1 on error
 */
int libpff_io_handle_read_unallocated_data_blocks(
     libpff_io_handle_t *io_handle,
     libbfio_handle_t *file_io_handle,
     libpff_offset_list_t *unallocated_data_block_list,
     libcerror_error_t **error )
{
	static char *function           = "libpff_io_handle_read_unallocated_data_blocks";
	off64_t allocation_table_offset = 0;
	size64_t allocation_block_size  = 0;

	if( io_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid IO handle.",
		 function );

		return( -1 );
	}
	allocation_table_offset = 0x4400;
	allocation_block_size   = 496 * 512;

	while( allocation_table_offset < (off64_t) io_handle->file_size )
	{
		if( libpff_allocation_table_read(
		     unallocated_data_block_list,
		     file_io_handle,
		     allocation_table_offset,
		     (int) io_handle->file_type,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_IO,
			 LIBCERROR_IO_ERROR_READ_FAILED,
			 "%s: unable to read allocation table at offset: %" PRIi64 ".",
			 function,
			 allocation_table_offset );

			return( -1 );
		}
		allocation_table_offset += allocation_block_size;
	}
	return( 1 );
}

/* Reads the unallocated page blocks
 * Returns 1 if successful or -1 on error
 */
int libpff_io_handle_read_unallocated_page_blocks(
     libpff_io_handle_t *io_handle,
     libbfio_handle_t *file_io_handle,
     libpff_offset_list_t *unallocated_page_block_list,
     libcerror_error_t **error )
{
	static char *function           = "libpff_io_handle_read_unallocated_page_blocks";
	off64_t allocation_table_offset = 0;
	size64_t allocation_block_size  = 0;

	if( io_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid IO handle.",
		 function );

		return( -1 );
	}
	allocation_table_offset = 0x4600;
	allocation_block_size   = 496 * 4096;

	while( allocation_table_offset < (off64_t) io_handle->file_size )
	{
		if( libpff_allocation_table_read(
		     unallocated_page_block_list,
		     file_io_handle,
		     allocation_table_offset,
		     (int) io_handle->file_type,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_IO,
			 LIBCERROR_IO_ERROR_READ_FAILED,
			 "%s: unable to read allocation table at offset: %" PRIi64 ".",
			 function,
			 allocation_table_offset );

			return( -1 );
		}
		allocation_table_offset += allocation_block_size;
	}
	return( 1 );
}

/* Creates an item tree from the descriptor index
 * Returns 1 if successful, 0 if the item tree could not be created or -1 on error
 */
int libpff_io_handle_create_item_tree(
     libpff_io_handle_t *io_handle,
     libbfio_handle_t *file_io_handle,
     libpff_tree_node_t **item_tree_root_node,
     libpff_list_t *orphan_node_list,
     libpff_tree_node_t **root_folder_item_tree_node,
     libcerror_error_t **error )
{
	libfdata_tree_node_t *descriptor_index_tree_root_node = NULL;
	libpff_item_descriptor_t *item_descriptor             = NULL;
	static char *function                                 = "libpff_io_handle_create_item_tree";
	int result                                            = 0;

	if( io_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid IO handle.",
		 function );

		return( -1 );
	}
	if( item_tree_root_node == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid item tree root node.",
		 function );

		return( -1 );
	}
	if( *item_tree_root_node != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: item tree root node already set.",
		 function );

		return( -1 );
	}
	if( libfdata_tree_get_root_node(
	     io_handle->descriptor_index_tree,
	     &descriptor_index_tree_root_node,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to retrieve descriptor index tree root node.",
		 function );

		return( -1 );
	}
	if( libpff_item_descriptor_initialize(
	     &item_descriptor,
	     0,
	     0,
	     0,
	     0,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create item descriptor.",
		 function );

		return( -1 );
	}
	if( libpff_tree_node_initialize(
	     item_tree_root_node,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create item tree root node.",
		 function );

		libpff_item_descriptor_free(
		 &item_descriptor,
		 NULL );

		return( -1 );
	}
	if( libpff_tree_node_set_value(
	     *item_tree_root_node,
	     (intptr_t *) item_descriptor,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to set item descriptor in item tree root node.",
		 function );

		libpff_tree_node_free(
		 item_tree_root_node,
		 NULL,
		 NULL );
		libpff_item_descriptor_free(
		 &item_descriptor,
		 NULL );

		return( -1 );
	}
	result = libpff_item_tree_create_node(
	          *item_tree_root_node,
	          file_io_handle,
	          io_handle->descriptor_index_tree,
	          descriptor_index_tree_root_node,
	          io_handle->descriptor_index_tree_cache,
	          orphan_node_list,
	          root_folder_item_tree_node,
	          error );

	if( result == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create item tree.",
		 function );

		libpff_tree_node_free(
		 item_tree_root_node,
		 (int (*)(intptr_t **, libcerror_error_t **)) &libpff_item_descriptor_free,
		 NULL );

		return( -1 );
	}
	return( result );
}

/* Reads an index node
 * Returns 1 if successful or -1 on error
 */
int libpff_io_handle_read_index_node(
     intptr_t *io_handle,
     libbfio_handle_t *file_io_handle,
     libfdata_vector_t *vector,
     libfcache_cache_t *cache,
     int element_index,
     off64_t element_data_offset,
     size64_t element_data_size LIBPFF_ATTRIBUTE_UNUSED,
     uint8_t read_flags LIBPFF_ATTRIBUTE_UNUSED,
     libcerror_error_t **error )
{
	libpff_index_node_t *index_node = NULL;
	static char *function           = "libpff_io_handle_read_index_node";

	LIBPFF_UNREFERENCED_PARAMETER( element_data_size )
	LIBPFF_UNREFERENCED_PARAMETER( read_flags )

	if( io_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid IO handle.",
		 function );

		return( -1 );
	}
	/* TODO check if element data size matches index node size
	 * remove LIBPFF_ATTRIBUTE_UNUSED, LIBPFF_UNREFERENCED_PARAMETER
	 */
	if( libpff_index_node_initialize(
	     &index_node,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create index node.",
		 function );

		return( -1 );
	}
	if( libpff_index_node_read(
	     index_node,
	     file_io_handle,
	     element_data_offset,
	     ( (libpff_io_handle_t *) io_handle )->file_type,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_READ_FAILED,
		 "%s: unable to read index node at offset: %" PRIi64 ".",
		 function,
		 element_data_offset );

		libpff_index_node_free(
		 &index_node,
		 NULL );

		return( -1 );
	}
	if( libfdata_vector_set_element_value_by_index(
	     vector,
	     cache,
	     element_index,
	     (intptr_t *) index_node,
	     (int (*)(intptr_t **, libcerror_error_t **)) &libpff_index_node_free,
	     LIBFDATA_LIST_ELEMENT_VALUE_FLAG_MANAGED,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to set index node as element value.",
		 function );

		libpff_index_node_free(
		 &index_node,
		 NULL );

		return( -1 );
	}
	return( 1 );
}

/* Reads a local descriptor node
 * Returns 1 if successful or -1 on error
 */
int libpff_io_handle_read_local_descriptor_node(
     intptr_t *io_handle,
     libbfio_handle_t *file_io_handle,
     libfdata_list_element_t *list_element,
     libfcache_cache_t *cache,
     off64_t element_data_offset,
     size64_t element_data_size,
     uint32_t element_data_flags LIBPFF_ATTRIBUTE_UNUSED,
     uint8_t read_flags LIBPFF_ATTRIBUTE_UNUSED,
     libcerror_error_t **error )
{
	libpff_local_descriptor_node_t *local_descriptor_node = NULL;
	static char *function                                 = "libpff_io_handle_read_local_descriptor_node";

	LIBPFF_UNREFERENCED_PARAMETER( element_data_flags )
	LIBPFF_UNREFERENCED_PARAMETER( read_flags )

	if( io_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid IO handle.",
		 function );

		return( -1 );
	}
	if( element_data_size > (size64_t) UINT32_MAX )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_EXCEEDS_MAXIMUM,
		 "%s: element data size value exceeds maximum.",
		 function );

		return( -1 );
	}
	if( libpff_local_descriptor_node_initialize(
	     &local_descriptor_node,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create local descriptor node.",
		 function );

		return( -1 );
	}
	if( libpff_local_descriptor_node_read(
	     local_descriptor_node,
	     file_io_handle,
	     element_data_offset,
	     (size32_t) element_data_size,
	     ( (libpff_io_handle_t *) io_handle )->file_type,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_READ_FAILED,
		 "%s: unable to read local descriptor node at offset: %" PRIi64 ".",
		 function,
		 element_data_offset );

		libpff_local_descriptor_node_free(
		 &local_descriptor_node,
		 NULL );

		return( -1 );
	}
	if( libfdata_list_element_set_element_value(
	     list_element,
	     cache,
	     (intptr_t *) local_descriptor_node,
	     (int (*)(intptr_t **, libcerror_error_t **)) &libpff_local_descriptor_node_free,
	     LIBFDATA_LIST_ELEMENT_VALUE_FLAG_MANAGED,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to set local descriptor node as element value.",
		 function );

		libpff_local_descriptor_node_free(
		 &local_descriptor_node,
		 NULL );

		return( -1 );
	}
	return( 1 );
}

/* Retrieves a descriptor index value for a specific identifier
 * Returns 1 if successful, 0 if no index value was found or -1 on error
 */
int libpff_io_handle_get_descriptor_index_value_by_identifier(
     libpff_io_handle_t *io_handle,
     libbfio_handle_t *file_io_handle,
     uint32_t descriptor_identifier,
     uint8_t recovered,
     libpff_index_value_t **descriptor_index_value,
     libcerror_error_t **error )
{
	static char *function      = "libpff_io_handle_get_descriptor_index_value_by_identifier";
	int number_of_index_values = 0;
	int result                 = 0;

	if( io_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid IO handle.",
		 function );

		return( -1 );
	}
	if( recovered == 0 )
	{
		result = libpff_index_tree_get_value_by_identifier(
			  io_handle->descriptor_index_tree,
			  file_io_handle,
			  io_handle->descriptor_index_tree_cache,
			  descriptor_identifier,
			  0,
			  descriptor_index_value,
			  error );

		if( result == -1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve descriptor index value: %" PRIu32 ".",
			 function,
			 descriptor_identifier );

			return( -1 );
		}
	}
	else
	{
		if( libpff_index_tree_get_number_of_leaf_nodes_by_identifier(
		     io_handle->recovered_descriptor_index_tree,
		     file_io_handle,
		     io_handle->descriptor_index_tree_cache,
		     descriptor_identifier,
		     &number_of_index_values,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve number of recovered descriptor index values for identifier: %" PRIu32 ".",
			 function,
			 descriptor_identifier );

			return( -1 );
		}
#if defined( HAVE_DEBUG_OUTPUT )
fprintf( stderr, "NRDIV: %d\n", number_of_index_values );
#endif

		/* TODO is it necessary to lookup removed descriptors with a value index > 0 ? */
		result = libpff_index_tree_get_value_by_identifier(
			  io_handle->recovered_descriptor_index_tree,
			  file_io_handle,
			  io_handle->descriptor_index_tree_cache,
			  descriptor_identifier,
			  0,
			  descriptor_index_value,
			  error );

		if( result == -1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve recovered descriptor index value: %" PRIu32 ".",
			 function,
			 descriptor_identifier );

			return( -1 );
		}
	}
	return( result );
}

/* Retrieves an offset index value for a specific identifier
 * Returns 1 if successful, 0 if no index value was found or -1 on error
 */
int libpff_io_handle_get_offset_index_value_by_identifier(
     libpff_io_handle_t *io_handle,
     libbfio_handle_t *file_io_handle,
     uint64_t data_identifier,
     uint8_t recovered,
     int recovered_value_index,
     libpff_index_value_t **offset_index_value,
     libcerror_error_t **error )
{
	static char *function      = "libpff_io_handle_get_offset_index_value_by_identifier";
	int number_of_index_values = 0;
	int result                 = 0;

	if( io_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid IO handle.",
		 function );

		return( -1 );
	}
	if( recovered == 0 )
	{
		result = libpff_index_tree_get_value_by_identifier(
			  io_handle->offset_index_tree,
			  file_io_handle,
			  io_handle->offset_index_tree_cache,
			  data_identifier & (uint64_t) LIBPFF_OFFSET_INDEX_IDENTIFIER_MASK,
			  0,
			  offset_index_value,
			  error );

		if( result == -1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to find offset index value identifier: %" PRIu64 ".",
			 function,
			 data_identifier & (uint64_t) LIBPFF_OFFSET_INDEX_IDENTIFIER_MASK );

			return( -1 );
		}
	}
	else
	{
		if( libpff_index_tree_get_number_of_leaf_nodes_by_identifier(
		     io_handle->recovered_offset_index_tree,
		     file_io_handle,
		     io_handle->offset_index_tree_cache,
		     data_identifier & (uint64_t) LIBPFF_OFFSET_INDEX_IDENTIFIER_MASK,
		     &number_of_index_values,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve number of recovered offset index values for identifier: %" PRIu64 ".",
			 function,
			 data_identifier & (uint64_t) LIBPFF_OFFSET_INDEX_IDENTIFIER_MASK );

			return( -1 );
		}
		if( number_of_index_values <= 0 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
			 "%s: no recovered offset index values for identifier: %" PRIu64 ".",
			 function,
			 data_identifier & (uint64_t) LIBPFF_OFFSET_INDEX_IDENTIFIER_MASK );

			return( -1 );
		}
		if( ( recovered_value_index < 0 )
		 || ( recovered_value_index >= number_of_index_values ) )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
			 "%s: invalid recovered value index value out of bounds.",
			 function );

			return( -1 );
		}
#if defined( HAVE_DEBUG_OUTPUT )
fprintf( stderr, "NROIV: %d\n", number_of_index_values );
#endif
		result = libpff_index_tree_get_value_by_identifier(
			  io_handle->recovered_offset_index_tree,
			  file_io_handle,
			  io_handle->offset_index_tree_cache,
			  data_identifier & (uint64_t) LIBPFF_OFFSET_INDEX_IDENTIFIER_MASK,
			  recovered_value_index,
			  offset_index_value,
			  error );

		if( result == -1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to find recovered offset index value identifier: %" PRIu64 ".",
			 function,
			 data_identifier & (uint64_t) LIBPFF_OFFSET_INDEX_IDENTIFIER_MASK );

			return( -1 );
		}
	}
	return( result );
}

/* Reads the local descriptors of a descriptor
 * Returns 1 if successful or -1 on error
 */
int libpff_io_handle_read_descriptor_local_descriptors_tree(
     libpff_io_handle_t *io_handle,
     libbfio_handle_t *file_io_handle,
     uint64_t local_descriptors_identifier,
     uint8_t recovered,
     int recovered_value_index,
     libfdata_tree_t **local_descriptors_tree,
     libcerror_error_t **error )
{
	libpff_index_value_t *offset_index_value      = NULL;
	libpff_local_descriptors_t *local_descriptors = NULL;
	static char *function                         = "libpff_io_handle_read_descriptor_local_descriptors_tree";

	if( io_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid IO handle.",
		 function );

		return( -1 );
	}
	if( local_descriptors_tree == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid local descriptors tree.",
		 function );

		return( -1 );
	}
	if( *local_descriptors_tree != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: local descriptors tree already set.",
		 function );

		return( -1 );
	}
	if( libpff_io_handle_get_offset_index_value_by_identifier(
	     io_handle,
	     file_io_handle,
	     local_descriptors_identifier,
	     recovered,
	     recovered_value_index,
	     &offset_index_value,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to find offset index value identifier: %" PRIu64 ".",
		 function,
		 local_descriptors_identifier );

		return( -1 );
	}
	if( offset_index_value == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid offset index value.",
		 function );

		return( -1 );
	}
#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		libcnotify_printf(
		 "%s: identifier: %" PRIu64 " (%s) at offset: %" PRIi64 " of size: %" PRIu32 "\n",
		 function,
		 offset_index_value->identifier,
		 ( ( offset_index_value->identifier & LIBPFF_OFFSET_INDEX_IDENTIFIER_FLAG_INTERNAL ) ? "internal" : "external" ),
		 offset_index_value->file_offset,
		 offset_index_value->data_size );
	}
#endif
	/* Create the local descriptors tree
	 */
	if( libpff_local_descriptors_initialize(
	     &local_descriptors,
	     io_handle,
	     offset_index_value->file_offset,
	     offset_index_value->data_size,
	     recovered,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create local descriptors.",
		 function );

		return( -1 );
	}
	if( libfdata_tree_initialize(
	     local_descriptors_tree,
	     (intptr_t *) local_descriptors,
	     (int (*)(intptr_t **, libcerror_error_t **)) &libpff_local_descriptors_free,
	     (int (*)(intptr_t **, intptr_t *, libcerror_error_t **)) &libpff_local_descriptors_clone,
	     &libpff_local_descriptors_read_node_data,
	     &libpff_local_descriptors_read_sub_nodes,
	     LIBFDATA_FLAG_IO_HANDLE_MANAGED,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create local descriptors tree",
		 function );

		libpff_local_descriptors_free(
		 &local_descriptors,
		 NULL );

		return( -1 );
	}
	/* Point the root node data offset to 0
	 * otherwise it will mess up the caching of the first local descriptors value
	 */
	if( libfdata_tree_set_root_node(
	     *local_descriptors_tree,
	     0,
	     0,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to set local descriptors tree root node.",
		 function );

		libfdata_tree_free(
		 local_descriptors_tree,
		 NULL );

		return( -1 );
	}
	return( 1 );
}

/* Reads the data block of a descriptor
 * Returns 1 if successful or -1 on error
 */
int libpff_io_handle_read_descriptor_data_block(
     libpff_io_handle_t *io_handle,
     libbfio_handle_t *file_io_handle,
     uint32_t descriptor_identifier,
     uint64_t data_identifier,
     uint8_t recovered,
     int recovered_value_index,
     libfdata_block_t **descriptor_data_block,
     libfcache_cache_t **descriptor_data_cache,
     libcerror_error_t **error )
{
	libpff_data_array_t *data_array          = NULL;
	libpff_data_block_t *data_block          = NULL;
	libpff_index_value_t *offset_index_value = NULL;
	uint8_t *data_buffer                     = NULL;
	static char *function                    = "libpff_io_handle_read_descriptor_data_block";
	ssize_t read_count                       = 0;

	if( io_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid IO handle.",
		 function );

		return( -1 );
	}
	if( descriptor_data_block == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid descriptor data block.",
		 function );

		return( -1 );
	}
	if( *descriptor_data_block != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: descriptor data block already set.",
		 function );

		return( -1 );
	}
	if( descriptor_data_cache == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid descriptor data cache.",
		 function );

		return( -1 );
	}
	if( *descriptor_data_cache != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: descriptor data cache already set.",
		 function );

		return( -1 );
	}
	if( libpff_io_handle_get_offset_index_value_by_identifier(
	     io_handle,
	     file_io_handle,
	     data_identifier,
	     recovered,
	     recovered_value_index,
	     &offset_index_value,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to find offset index value identifier: %" PRIu64 ".",
		 function,
		 data_identifier );

		return( -1 );
	}
	if( offset_index_value == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid offset index value.",
		 function );

		return( -1 );
	}
#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		libcnotify_printf(
		 "%s: identifier: %" PRIu64 " (%s) at offset: %" PRIi64 " of size: %" PRIu32 "\n",
		 function,
		 offset_index_value->identifier,
		 ( ( offset_index_value->identifier & LIBPFF_OFFSET_INDEX_IDENTIFIER_FLAG_INTERNAL ) ? "internal" : "external" ),
		 offset_index_value->file_offset,
		 offset_index_value->data_size );
	}
#endif
	if( offset_index_value->file_offset <= 0 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: invalid offset index value - file offset value out of bounds.",
		 function );

		return( -1 );
	}
	if( offset_index_value->data_size == 0 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: invalid offset index value - data size value value out of bounds.",
		 function );

		return( -1 );
	}
#if UINT32_MAX > SSIZE_MAX
	if( offset_index_value->data_size > (size32_t) SSIZE_MAX )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_EXCEEDS_MAXIMUM,
		 "%s: invalid offset index value - data size value exceeds maximum.",
		 function );

		return( -1 );
	}
#endif
#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		libcnotify_printf(
		 "%s: reading descriptor data at offset: %" PRIi64 " (0x%08" PRIx64 ")\n",
		 function,
		 offset_index_value->file_offset,
		 offset_index_value->file_offset );
	}
#endif
	if( libbfio_handle_seek_offset(
	     file_io_handle,
	     offset_index_value->file_offset,
	     SEEK_SET,
	     error ) == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_SEEK_FAILED,
		 "%s: unable to seek value offset: %" PRIi64 ".",
		 function,
		 offset_index_value->file_offset );

		return( -1 );
	}
	data_buffer = (uint8_t *) memory_allocate(
	                           sizeof( uint8_t ) * (size_t) offset_index_value->data_size );

	if( data_buffer == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create data buffer.",
		 function );

		return( -1 );
	}
	if( libpff_data_block_initialize(
	     &data_block,
	     descriptor_identifier,
	     data_identifier,
	     io_handle,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create data block.",
		 function );

		goto on_error;
	}
	read_count = libpff_data_block_read(
	              file_io_handle,
	              io_handle->file_type,
	              data_identifier,
	              data_buffer,
	              (size_t) offset_index_value->data_size,
	              &( data_block->flags ),
	              error );

	if( read_count != (ssize_t) offset_index_value->data_size )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_READ_FAILED,
		 "%s: unable to read data block.",
		 function );

		goto on_error;
	}
	/* Check if the data block contains a data array
	 * The data array should have the internal flag set in the (data) offset index identifier
	 * The data array starts with 0x01 followed by either 0x01 or 0x02
	 */
	if( ( ( data_identifier & (uint64_t) LIBPFF_OFFSET_INDEX_IDENTIFIER_FLAG_INTERNAL ) != 0 )
	 && ( ( data_buffer[ 0 ] == 0x01 )
	  && ( ( data_buffer[ 1 ] == 0x01 )
	   || ( data_buffer[ 1 ] == 0x02 ) ) ) )
	{
		if( libpff_data_array_initialize(
		     &data_array,
		     descriptor_identifier,
		     data_identifier,
		     io_handle,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
			 "%s: unable to create data array.",
			 function );

			goto on_error;
		}
		if( libfdata_block_initialize(
		     descriptor_data_block,
		     (intptr_t *) data_array,
		     (int (*)(intptr_t **, libcerror_error_t **)) &libpff_data_array_free,
		     (int (*)(intptr_t **, intptr_t *, libcerror_error_t **)) &libpff_data_array_clone,
		     &libpff_data_array_read_entry_data,
		     LIBFDATA_FLAG_IO_HANDLE_MANAGED,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
			 "%s: unable to create descriptor data block.",
			 function );

			libpff_data_array_free(
			 &data_array,
			 NULL );

			goto on_error;
		}
		if( libpff_data_array_read(
		     data_array,
		     io_handle,
		     file_io_handle,
		     *descriptor_data_block,
		     recovered,
		     data_buffer,
		     (size_t) offset_index_value->data_size,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_IO,
			 LIBCERROR_IO_ERROR_READ_FAILED,
			 "%s: unable to read data array.",
			 function );

			goto on_error;
		}
		if( libpff_data_block_free(
		     &data_block,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
			 "%s: unable to free data block.",
			 function );

			goto on_error;
		}
		data_block = NULL;

		memory_free(
		 data_buffer );

		data_buffer = NULL;

		if( libfcache_cache_initialize(
		     descriptor_data_cache,
		     LIBPFF_MAXIMUM_CACHE_ENTRIES_DATA_ARRAY,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
			 "%s: unable to create descriptor data cache.",
			 function );

			goto on_error;
		}
	}
	else
	{
		if( libpff_data_block_decrypt_data(
		     data_block,
		     io_handle->encryption_type,
		     data_buffer,
		     (size_t) offset_index_value->data_size,
		     0,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_ENCRYPTION,
			 LIBCERROR_ENCRYPTION_ERROR_DECRYPT_FAILED,
			 "%s: unable to decrypt data block data.",
			 function );

			goto on_error;
		}
		if( libfdata_block_initialize(
		     descriptor_data_block,
		     (intptr_t *) data_block,
		     (int (*)(intptr_t **, libcerror_error_t **)) &libpff_data_block_free,
		     (int (*)(intptr_t **, intptr_t *, libcerror_error_t **)) &libpff_data_block_clone,
		     &libpff_data_block_read_data,
		     LIBFDATA_FLAG_IO_HANDLE_MANAGED,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
			 "%s: unable to create descriptor data block.",
			 function );

			goto on_error;
		}
		if( libfdata_block_append_segment(
		     *descriptor_data_block,
		     offset_index_value->file_offset,
		     (size64_t) offset_index_value->data_size,
		     0,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
			 "%s: unable to append data block segment.",
			 function );

			goto on_error;
		}
		if( libfcache_cache_initialize(
		     descriptor_data_cache,
		     LIBPFF_MAXIMUM_CACHE_ENTRIES_DATA_BLOCK,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
			 "%s: unable to create descriptor data cache.",
			 function );

			goto on_error;
		}
		if( libfdata_block_set_segment_data(
		     *descriptor_data_block,
		     *descriptor_data_cache,
		     0,
		     data_buffer,
		     (size_t) offset_index_value->data_size,
		     LIBFDATA_SEGMENT_DATA_FLAG_MANAGED,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
			 "%s: unable to set data block segment: 0.",
			 function );

			goto on_error;
		}
	}
	return( 1 );

on_error:
	if( descriptor_data_cache != NULL )
	{
		libfcache_cache_free(
		 descriptor_data_cache,
		 NULL );
	}
	if( descriptor_data_block != NULL )
	{
		libfdata_block_free(
		 descriptor_data_block,
		 NULL );
	}
	if( data_block != NULL )
	{
		libpff_data_block_free(
		 &data_block,
		 NULL );
	}
	if( data_buffer != NULL )
	{
		memory_free(
		 data_buffer );
	}
	return( -1 );
}

/* Reads the data reference of a descriptor
 * Returns 1 if successful or -1 on error
 */
int libpff_io_handle_read_descriptor_data_reference(
     libpff_io_handle_t *io_handle,
     libbfio_handle_t *file_io_handle,
     uint32_t descriptor_identifier,
     uint64_t data_identifier,
     uint8_t recovered,
     int recovered_value_index,
     libfdata_reference_t **descriptor_data_reference,
     libfcache_cache_t **descriptor_data_cache,
     libcerror_error_t **error )
{
	libfdata_block_t *descriptor_data_block = NULL;
	static char *function                   = "libpff_io_handle_read_descriptor_data_reference";

	if( libpff_io_handle_read_descriptor_data_block(
	     io_handle,
	     file_io_handle,
	     descriptor_identifier,
	     data_identifier,
	     recovered,
	     recovered_value_index,
	     &descriptor_data_block,
	     descriptor_data_cache,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_READ_FAILED,
		 "%s: unable to read descriptor data block.",
		 function );

		goto on_error;
	}
	if( libfdata_block_reference_initialize(
	     descriptor_data_reference,
	     descriptor_data_block,
	     LIBFDATA_REFERENCE_FLAG_MANAGED,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create block data reference.",
		 function );

		goto on_error;
	}
	return( 1 );

on_error:
	if( descriptor_data_cache != NULL )
	{
		libfcache_cache_free(
		 descriptor_data_cache,
		 NULL );
	}
	if( descriptor_data_block != NULL )
	{
		libfdata_block_free(
		 &descriptor_data_block,
		 NULL );
	}
	return( -1 );
}

/* Scans for recoverable items
 * By default only the unallocated space is checked for recoverable items
 * Returns 1 if successful or -1 on error
 */
int libpff_io_handle_recover_items(
     libpff_io_handle_t *io_handle,
     libbfio_handle_t *file_io_handle,
     libpff_offset_list_t *unallocated_data_block_list,
     libpff_offset_list_t *unallocated_page_block_list,
     libpff_list_t *recovered_item_list,
     uint8_t recovery_flags,
     libcerror_error_t **error )
{
	libfdata_tree_node_t *recovered_descriptor_index_leaf_node = NULL;
	libpff_index_t *recovered_descriptor_index                 = NULL;
	libpff_index_t *recovered_offset_index                     = NULL;
	libpff_index_value_t *descriptor_index_value               = NULL;
	libpff_index_value_t *offset_index_value                   = NULL;
	libpff_item_descriptor_t *item_descriptor                  = NULL;
	libpff_tree_node_t *item_tree_node                         = NULL;
	uint8_t *block_buffer                                      = NULL;
	static char *function                                      = "libpff_io_handle_recover_items";
	ssize_t read_count                                         = 0;
	uint8_t data_block_read_flags                              = 0;
	int data_identifier_value_index                            = 0;
	int index_value_iterator                                   = 0;
	int local_descriptors_identifier_value_index               = 0;
	int number_of_index_values                                 = 0;
	int number_of_recovered_descriptor_index_values            = 0;
	int recovered_descriptor_index_value_iterator              = 0;
	int recoverable                                            = 0;
	int result                                                 = 0;

	if( io_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid IO handle.",
		 function );

		return( -1 );
	}
	if( ( io_handle->file_type != LIBPFF_FILE_TYPE_32BIT )
	 && ( io_handle->file_type != LIBPFF_FILE_TYPE_64BIT ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
		 "%s: unsupported file type.",
		 function );

		return( -1 );
	}
	if( recovered_item_list == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid recovered item list.",
		 function );

		return( -1 );
	}
	if( io_handle->recovered_descriptor_index_tree == NULL )
	{
		/* Create the recovered descriptor index tree
		 */
		if( libpff_index_initialize(
		     &recovered_descriptor_index,
		     io_handle,
		     LIBPFF_INDEX_TYPE_DESCRIPTOR,
		     0,
		     0,
		     1,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
			 "%s: unable to create recovered descriptor index.",
			 function );

			return( -1 );
		}
		if( libfdata_tree_initialize(
		     &( io_handle->recovered_descriptor_index_tree ),
		     (intptr_t *) recovered_descriptor_index,
		     (int (*)(intptr_t **, libcerror_error_t **)) &libpff_index_free,
		     (int (*)(intptr_t **, intptr_t *, libcerror_error_t **)) &libpff_index_clone,
		     &libpff_index_read_node_data,
		     &libpff_index_read_sub_nodes,
		     LIBFDATA_FLAG_IO_HANDLE_MANAGED,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
			 "%s: unable to create recovered descriptor index tree",
			 function );

			libpff_index_free(
			 &recovered_descriptor_index,
			 NULL );

			return( -1 );
		}
		/* Point the root node data offset to LIBPFF_IO_HANDLE_RECOVERED_DESCRIPTOR_INDEX_TREE_ROOT_OFFSET
		 * otherwise it will mess up the caching of the first index value
		 */
		if( libfdata_tree_set_root_node(
		     io_handle->recovered_descriptor_index_tree,
		     LIBPFF_IO_HANDLE_RECOVERED_DESCRIPTOR_INDEX_TREE_ROOT_OFFSET,
		     0,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
			 "%s: unable to set recovered index index tree root node.",
			 function );

			libfdata_tree_free(
			 &( io_handle->recovered_descriptor_index_tree ),
			 NULL );

			return( -1 );
		}
	}
	if( io_handle->recovered_offset_index_tree == NULL )
	{
		/* Create the recovered offset index tree
		 */
		if( libpff_index_initialize(
		     &recovered_offset_index,
		     io_handle,
		     LIBPFF_INDEX_TYPE_OFFSET,
		     0,
		     0,
		     1,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
			 "%s: unable to create recovered offset index.",
			 function );

			return( -1 );
		}
		if( libfdata_tree_initialize(
		     &( io_handle->recovered_offset_index_tree ),
		     (intptr_t *) recovered_offset_index,
		     (int (*)(intptr_t **, libcerror_error_t **)) &libpff_index_free,
		     (int (*)(intptr_t **, intptr_t *, libcerror_error_t **)) &libpff_index_clone,
		     &libpff_index_read_node_data,
		     &libpff_index_read_sub_nodes,
		     LIBFDATA_FLAG_IO_HANDLE_MANAGED,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
			 "%s: unable to create recovered offset index tree",
			 function );

			libpff_index_free(
			 &recovered_offset_index,
			 NULL );

			return( -1 );
		}
		/* Point the root node data offset to LIBPFF_IO_HANDLE_RECOVERED_OFFSET_INDEX_TREE_ROOT_OFFSET
		 * otherwise it will mess up the caching of the first index value
		 */
		if( libfdata_tree_set_root_node(
		     io_handle->recovered_offset_index_tree,
		     LIBPFF_IO_HANDLE_RECOVERED_OFFSET_INDEX_TREE_ROOT_OFFSET,
		     0,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
			 "%s: unable to set recovered offset index tree root node.",
			 function );

			libfdata_tree_free(
			 &( io_handle->recovered_offset_index_tree ),
			 NULL );

			return( -1 );
		}
	}
#if defined( HAVE_DEBUG_OUTPUT )
	if( libbfio_handle_set_track_offsets_read(
	     file_io_handle,
	     0,
	     error ) != 1 )
	{
                libcerror_error_set(
                 error,
                 LIBCERROR_ERROR_DOMAIN_RUNTIME,
                 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
                 "%s: unable to set track offsets read in file IO handle.",
                 function );

                return( -1 );
	}
#endif
	if( libpff_io_handle_recover_index_nodes(
	     io_handle,
	     file_io_handle,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_READ_FAILED,
		 "%s: unable to recover index nodes.",
		 function );

		return( -1 );
	}
	if( libpff_io_handle_recover_data_blocks(
	     io_handle,
	     file_io_handle,
	     unallocated_data_block_list,
	     unallocated_page_block_list,
	     recovery_flags,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_READ_FAILED,
		 "%s: unable to recover data blocks.",
		 function );

		return( -1 );
	}
	block_buffer = (uint8_t *) memory_allocate(
	                            sizeof( uint8_t ) * 8192 );

	if( block_buffer == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create block buffer.",
		 function );

		return( -1 );
	}
	/* For the recovered descriptor index nodes check
	 * if the local descriptor and data offset index value still exists
	 */
	if( libfdata_tree_get_number_of_leaf_nodes(
	     io_handle->recovered_descriptor_index_tree,
	     file_io_handle,
	     io_handle->descriptor_index_tree_cache,
	     &number_of_recovered_descriptor_index_values,
	     0,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve number of recovered descriptor index values.",
		 function );

		memory_free(
		 block_buffer );

		return( -1 );
	}
	for( recovered_descriptor_index_value_iterator = 0;
	     recovered_descriptor_index_value_iterator < number_of_recovered_descriptor_index_values;
	     recovered_descriptor_index_value_iterator++ )
	{
		if( io_handle->abort != 0 )
		{
			memory_free(
			 block_buffer );

			return( -1 );
		}
		if( libfdata_tree_get_leaf_node_by_index(
		     io_handle->recovered_descriptor_index_tree,
		     file_io_handle,
		     io_handle->descriptor_index_tree_cache,
		     recovered_descriptor_index_value_iterator,
		     &recovered_descriptor_index_leaf_node,
		     0,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
			 "%s: unable to retrieve recovered descriptor leaf node: %d.",
			 function,
			 recovered_descriptor_index_value_iterator );

			libpff_list_empty(
			 recovered_item_list,
			 (int (*)(intptr_t **, libcerror_error_t **)) &libpff_item_tree_node_free_recovered,
			 NULL );
			memory_free(
			 block_buffer );

			return( -1 );
		}
		if( libfdata_tree_node_get_node_value(
		     recovered_descriptor_index_leaf_node,
		     file_io_handle,
		     io_handle->descriptor_index_tree_cache,
		     (intptr_t **) &descriptor_index_value,
		     0,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
			 "%s: unable to retrieve recovered descriptor index value: %d.",
			 function,
			 recovered_descriptor_index_value_iterator );

			libpff_list_empty(
			 recovered_item_list,
			 (int (*)(intptr_t **, libcerror_error_t **)) &libpff_item_tree_node_free_recovered,
			 NULL );
			memory_free(
			 block_buffer );

			return( -1 );
		}
		if( descriptor_index_value == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
			 "%s: missing descriptor index value: %d.",
			 function,
			 recovered_descriptor_index_value_iterator );

			libpff_list_empty(
			 recovered_item_list,
			 (int (*)(intptr_t **, libcerror_error_t **)) &libpff_item_tree_node_free_recovered,
			 NULL );
			memory_free(
			 block_buffer );

			return( -1 );
		}
#if defined( HAVE_DEBUG_OUTPUT )
		if( libcnotify_verbose != 0 )
		{
			libcnotify_printf(
			 "%s: analyzing identifier: %" PRIu64 ", data: %" PRIu64 ", local descriptors: %" PRIu64 ", parent: %" PRIu32 "\n",
			 function,
			 descriptor_index_value->identifier,
			 descriptor_index_value->data_identifier,
			 descriptor_index_value->local_descriptors_identifier,
			 descriptor_index_value->parent_identifier );
		}
#endif
		recoverable = 1;

		/* Check if the data identifier is recoverable
		 */
		if( recoverable != 0 )
		{
			if( libpff_index_tree_get_number_of_leaf_nodes_by_identifier(
			     io_handle->recovered_offset_index_tree,
			     file_io_handle,
			     io_handle->offset_index_tree_cache,
			     descriptor_index_value->data_identifier & (uint64_t) LIBPFF_OFFSET_INDEX_IDENTIFIER_MASK,
			     &number_of_index_values,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
				 "%s: unable to retrieve number of recovered offset index values for data identifier: %" PRIu64 ".",
				 function,
				 descriptor_index_value->data_identifier & (uint64_t) LIBPFF_OFFSET_INDEX_IDENTIFIER_MASK );

				libpff_list_empty(
				 recovered_item_list,
				 (int (*)(intptr_t **, libcerror_error_t **)) &libpff_item_tree_node_free_recovered,
				 NULL );
				memory_free(
				 block_buffer );

				return( -1 );
			}
			/* TODO what if more than 1 identifier is recoverable ? now uses first come first serve */
			result = 0;

			for( index_value_iterator = 0;
			     index_value_iterator < number_of_index_values;
			     index_value_iterator++ )
			{
				result = libpff_index_tree_get_value_by_identifier(
					  io_handle->recovered_offset_index_tree,
					  file_io_handle,
					  io_handle->offset_index_tree_cache,
					  descriptor_index_value->data_identifier & (uint64_t) LIBPFF_OFFSET_INDEX_IDENTIFIER_MASK,
					  index_value_iterator,
					  &offset_index_value,
					  error );

				if( result == -1 )
				{
					libcerror_error_set(
					 error,
					 LIBCERROR_ERROR_DOMAIN_RUNTIME,
					 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
					 "%s: unable to retrieve recovered offset index value for data identifier: %" PRIu64 ".",
					 function,
					 descriptor_index_value->data_identifier & (uint64_t) LIBPFF_OFFSET_INDEX_IDENTIFIER_MASK );

					libpff_list_empty(
					 recovered_item_list,
					 (int (*)(intptr_t **, libcerror_error_t **)) &libpff_item_tree_node_free_recovered,
					 NULL );
					memory_free(
					 block_buffer );

					return( -1 );
				}
				else if( result != 0 )
				{
					/* Check if the data block is readable
					 */
					if( libbfio_handle_seek_offset(
					     file_io_handle,
					     offset_index_value->file_offset,
					     SEEK_SET,
					     error ) == -1 )
					{
						libcerror_error_set(
						 error,
						 LIBCERROR_ERROR_DOMAIN_IO,
						 LIBCERROR_IO_ERROR_SEEK_FAILED,
						 "%s: unable to seek data block offset: %" PRIi64 ".",
						 function,
						 offset_index_value->file_offset );

						libpff_list_empty(
						 recovered_item_list,
						 (int (*)(intptr_t **, libcerror_error_t **)) &libpff_item_tree_node_free_recovered,
						 NULL );
						memory_free(
						 block_buffer );

						return( -1 );
					}
					data_block_read_flags = 0;

					read_count = libpff_data_block_read(
						      file_io_handle,
						      io_handle->file_type,
						      offset_index_value->identifier,
						      block_buffer,
						      offset_index_value->data_size,
						      &data_block_read_flags,
						      error );

					if( read_count != (ssize_t) offset_index_value->data_size )
					{
#if defined( HAVE_DEBUG_OUTPUT )
						if( ( libcnotify_verbose != 0 )
						 && ( error != NULL )
						 && ( *error != NULL ) )
						{
							libcnotify_print_error_backtrace(
							 *error );
						}
#endif
						libcerror_error_free(
						 error );

						/* TODO delete unreadable offset identifier in io_handle->recovered_offset_index_tree */
					}
					else
					{
						break;
					}
					result = 0;
				}
			}
			if( result == 0 )
			{
#if defined( HAVE_DEBUG_OUTPUT )
				if( libcnotify_verbose != 0 )
				{
					libcnotify_printf(
					 "%s: recovered offset index value for data identifier: %" PRIu64 " not available.\n",
					 function,
					 descriptor_index_value->data_identifier & (uint64_t) LIBPFF_OFFSET_INDEX_IDENTIFIER_MASK );
				}
#endif
				recoverable = 0;
			}
			else
			{
				data_identifier_value_index = index_value_iterator;
			}
		}
		/* Check if the local descriptors are also recoverable
		 */
		if( recoverable != 0 )
		{
			/* Allow desciptors to have a zero local descriptors value
			 */
			if( descriptor_index_value->local_descriptors_identifier > 0 )
			{
				if( libpff_index_tree_get_number_of_leaf_nodes_by_identifier(
				     io_handle->recovered_offset_index_tree,
				     file_io_handle,
				     io_handle->offset_index_tree_cache,
				     descriptor_index_value->local_descriptors_identifier & (uint64_t) LIBPFF_OFFSET_INDEX_IDENTIFIER_MASK,
				     &number_of_index_values,
				     error ) != 1 )
				{
					libcerror_error_set(
					 error,
					 LIBCERROR_ERROR_DOMAIN_RUNTIME,
					 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
					 "%s: unable to retrieve number of recovered offset index values for local descriptors identifier: %" PRIu64 ".",
					 function,
					 descriptor_index_value->local_descriptors_identifier & (uint64_t) LIBPFF_OFFSET_INDEX_IDENTIFIER_MASK );

					libpff_list_empty(
					 recovered_item_list,
					 (int (*)(intptr_t **, libcerror_error_t **)) &libpff_item_tree_node_free_recovered,
					 NULL );
					memory_free(
					 block_buffer );

					return( -1 );
				}
				/* TODO what if more than 1 identifier is recoverable ? now uses first come first serve */
				result = 0;

				for( index_value_iterator = 0;
				     index_value_iterator < number_of_index_values;
				     index_value_iterator++ )
				{
					result = libpff_index_tree_get_value_by_identifier(
						  io_handle->recovered_offset_index_tree,
						  file_io_handle,
						  io_handle->offset_index_tree_cache,
						  descriptor_index_value->local_descriptors_identifier & (uint64_t) LIBPFF_OFFSET_INDEX_IDENTIFIER_MASK,
						  index_value_iterator,
						  &offset_index_value,
						  error );

					if( result == -1 )
					{
						libcerror_error_set(
						 error,
						 LIBCERROR_ERROR_DOMAIN_RUNTIME,
						 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
						 "%s: unable to retrieve recovered offset index value for local descriptors identifier: %" PRIu64 ".",
						 function,
						 descriptor_index_value->local_descriptors_identifier & (uint64_t) LIBPFF_OFFSET_INDEX_IDENTIFIER_MASK );

						libpff_list_empty(
						 recovered_item_list,
						 (int (*)(intptr_t **, libcerror_error_t **)) &libpff_item_tree_node_free_recovered,
						 NULL );
						memory_free(
						 block_buffer );

						return( -1 );
					}
					else if( result != 0 )
					{
						/* Check if local descriptors are readable
						 */
						result = libpff_io_handle_recover_local_descriptors(
							  io_handle,
							  file_io_handle,
							  descriptor_index_value->local_descriptors_identifier,
							  error );

						if( result == -1 )
						{
							libcerror_error_set(
							 error,
							 LIBCERROR_ERROR_DOMAIN_IO,
							 LIBCERROR_IO_ERROR_READ_FAILED,
							 "%s: unable to read local descriptors with identifier: %" PRIu64 ".",
							 function,
							 descriptor_index_value->local_descriptors_identifier );

							libpff_list_empty(
							 recovered_item_list,
							 (int (*)(intptr_t **, libcerror_error_t **)) &libpff_item_tree_node_free_recovered,
							 NULL );
							memory_free(
							 block_buffer );

							return( -1 );
						}
						else if( result != 0 )
						{
							break;
						}
					}
				}
				if( result == 0 )
				{
#if defined( HAVE_DEBUG_OUTPUT )
					if( libcnotify_verbose != 0 )
					{
						libcnotify_printf(
						 "%s: recovered offset index value for local descriptors identifier: %" PRIu64 " not available.\n",
						 function,
						 descriptor_index_value->local_descriptors_identifier & (uint64_t) LIBPFF_OFFSET_INDEX_IDENTIFIER_MASK );
					}
#endif
					recoverable = 0;
				}
				if( recoverable != 0 )
				{
					local_descriptors_identifier_value_index = index_value_iterator;
				}
			}
		}
		if( recoverable == 0 )
		{
			if( libfdata_tree_node_set_deleted(
			     recovered_descriptor_index_leaf_node,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
				 "%s: unable to set deleted in recovered index tree leaf node: %d.",
				 function,
				 recovered_descriptor_index_value_iterator );

				libpff_list_empty(
				 recovered_item_list,
				 (int (*)(intptr_t **, libcerror_error_t **)) &libpff_item_tree_node_free_recovered,
				 NULL );
				memory_free(
				 block_buffer );

				return( -1 );
			}
			number_of_recovered_descriptor_index_values--;
			recovered_descriptor_index_value_iterator--;
		}
		else
		{
#if defined( HAVE_DEBUG_OUTPUT )
			if( libcnotify_verbose != 0 )
			{
				libcnotify_printf(
				 "%s: item descriptor: %" PRIu64 " is recoverable.\n",
				 function,
				 descriptor_index_value->identifier );
			}
#endif
			/* Create a new item descriptor
			 */
			if( libpff_item_descriptor_initialize(
			     &item_descriptor,
			     (uint32_t) descriptor_index_value->identifier,
			     descriptor_index_value->data_identifier,
			     descriptor_index_value->local_descriptors_identifier,
			     1,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
				 "%s: unable to create item descriptor.",
				 function );

				libpff_list_empty(
				 recovered_item_list,
				 (int (*)(intptr_t **, libcerror_error_t **)) &libpff_item_tree_node_free_recovered,
				 NULL );
				memory_free(
				 block_buffer );

				return( -1 );
			}
			if( item_descriptor == NULL )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
				 "%s: missing item descriptor.",
				 function );

				return( -1 );
			}
			/* TODO add to initialize function or create a separate function for setting these value in the item descriptor */
			item_descriptor->recovered_data_identifier_value_index              = data_identifier_value_index;
			item_descriptor->recovered_local_descriptors_identifier_value_index = local_descriptors_identifier_value_index;

			/* Create a new tree node with item tree values
			 */
			if( libpff_tree_node_initialize(
			     &item_tree_node,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
				 "%s: unable to create item tree node.",
				 function );

				libpff_item_descriptor_free(
				 &item_descriptor,
				 NULL );
				libpff_list_empty(
				 recovered_item_list,
				 (int (*)(intptr_t **, libcerror_error_t **)) &libpff_item_tree_node_free_recovered,
				 NULL );
				memory_free(
				 block_buffer );

				return( -1 );
			}
			if( libpff_tree_node_set_value(
			     item_tree_node,
			     (intptr_t *) item_descriptor,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
				 "%s: unable to set item descriptor in item tree node.",
				 function );

				libpff_tree_node_free(
				 &item_tree_node,
				 NULL,
				 NULL );
				libpff_item_descriptor_free(
				 &item_descriptor,
				 NULL );
				libpff_list_empty(
				 recovered_item_list,
				 (int (*)(intptr_t **, libcerror_error_t **)) &libpff_item_tree_node_free_recovered,
				 NULL );
				memory_free(
				 block_buffer );

				return( -1 );
			}
			item_descriptor = NULL;

			if( libpff_list_append_value(
			     recovered_item_list,
			     (intptr_t *) item_tree_node,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_APPEND_FAILED,
				 "%s: unable to append tree node to recovered item list.",
				 function );

				libpff_tree_node_free(
				 &item_tree_node,
				 (int (*)(intptr_t **, libcerror_error_t **)) &libpff_item_descriptor_free,
				 NULL );
				libpff_list_empty(
				 recovered_item_list,
				 (int (*)(intptr_t **, libcerror_error_t **)) &libpff_item_tree_node_free_recovered,
				 NULL );
				memory_free(
				 block_buffer );

				return( -1 );
			}
			item_tree_node = NULL;
		}
	}
	memory_free(
	 block_buffer );

#if defined( HAVE_DEBUG_OUTPUT )
	if( libbfio_handle_set_track_offsets_read(
	     file_io_handle,
	     0,
	     error ) != 1 )
	{
                libcerror_error_set(
                 error,
                 LIBCERROR_ERROR_DOMAIN_RUNTIME,
                 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
                 "%s: unable to set track offsets read in file IO handle.",
                 function );

                return( -1 );
	}
#endif
	/* TODO
	 * link recovered descriptors to parent? and add to item hierarchy?
	 * handle scanning without index data
	 * what about 'encryption' ?
	 */
	return( 1 );
}

/* Scans for recoverable index nodes
 * Returns 1 if successful or -1 on error
 */
int libpff_io_handle_recover_index_nodes(
     libpff_io_handle_t *io_handle,
     libbfio_handle_t *file_io_handle,
     libcerror_error_t **error )
{
	libfdata_tree_node_t *deleted_index_leaf_node = NULL;
	libpff_index_value_t *deleted_index_value     = NULL;
	libpff_index_value_t *index_value             = NULL;
	static char *function                         = "libpff_io_handle_recover_index_nodes";
	off64_t node_data_offset                      = 0;
	size64_t node_data_size                       = 0;
	uint32_t node_data_flags                      = 0;
	int deleted_index_value_iterator              = 0;
	int index_value_iterator                      = 0;
	int number_of_index_values                    = 0;
	int number_of_deleted_index_values            = 0;
	int result                                    = 0;

	if( io_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid IO handle.",
		 function );

		return( -1 );
	}
	/* Scan the existing descriptor index nodes for remnant values
	 */
	if( libfdata_tree_get_number_of_deleted_leaf_nodes(
	     io_handle->descriptor_index_tree,
	     file_io_handle,
	     io_handle->descriptor_index_tree_cache,
	     &number_of_deleted_index_values,
	     0,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve number of deleted descriptor index values.",
		 function );

		return( -1 );
	}
	for( deleted_index_value_iterator = 0;
	     deleted_index_value_iterator < number_of_deleted_index_values;
	     deleted_index_value_iterator++ )
	{
		if( io_handle->abort != 0 )
		{
			return( -1 );
		}
		if( libfdata_tree_get_deleted_leaf_node_by_index(
		     io_handle->descriptor_index_tree,
		     file_io_handle,
		     io_handle->descriptor_index_tree_cache,
		     deleted_index_value_iterator,
		     &deleted_index_leaf_node,
		     0,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
			 "%s: unable to retrieve deleted descriptor leaf node: %d.",
			 function,
			 deleted_index_value_iterator );

			return( -1 );
		}
		if( libfdata_tree_node_get_node_value(
		     deleted_index_leaf_node,
		     file_io_handle,
		     io_handle->descriptor_index_tree_cache,
		     (intptr_t **) &deleted_index_value,
		     0,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
			 "%s: unable to retrieve deleted descriptor index value: %d.",
			 function,
			 deleted_index_value_iterator );

			return( -1 );
		}
		if( deleted_index_value == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
			 "%s: missing deleted descriptor index value: %d.",
			 function,
			 deleted_index_value_iterator );

			return( -1 );
		}
#if defined( HAVE_DEBUG_OUTPUT )
		if( libcnotify_verbose != 0 )
		{
			libcnotify_printf(
			 "%s: analyzing identifier: %" PRIu64 ", data: %" PRIu64 ", local descriptors: %" PRIu64 ", parent: %" PRIu32 "\n",
			 function,
			 deleted_index_value->identifier,
			 deleted_index_value->data_identifier,
			 deleted_index_value->local_descriptors_identifier,
			 deleted_index_value->parent_identifier );
		}
#endif
		/* Check if the item value matches the existing item value
		 */
		result = libpff_index_tree_get_value_by_identifier(
			  io_handle->descriptor_index_tree,
			  file_io_handle,
			  io_handle->descriptor_index_tree_cache,
			  deleted_index_value->identifier,
			  0,
			  &index_value,
			  error );

		if( result == -1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve descriptor index value for identifier: %" PRIu64 ".",
			 function,
			 deleted_index_value->identifier );

			return( -1 );
		}
		else if( result != 0 )
		{
			/* TODO what about parent changes ? */
			if( ( deleted_index_value->data_identifier == index_value->data_identifier )
			 && ( deleted_index_value->local_descriptors_identifier == index_value->local_descriptors_identifier ) )
			{
#if defined( HAVE_DEBUG_OUTPUT )
				if( libcnotify_verbose != 0 )
				{
					libcnotify_printf(
					 "%s: deleted descriptor index value: %" PRIu64 " matches existing item value.\n",
					 function,
					 deleted_index_value->identifier );
				}
#endif
				continue;
			}
		}
		/* Check for duplicates
		 */
		if( libpff_index_tree_get_number_of_leaf_nodes_by_identifier(
		     io_handle->recovered_descriptor_index_tree,
		     file_io_handle,
		     io_handle->descriptor_index_tree_cache,
		     deleted_index_value->identifier,
		     &number_of_index_values,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve number of recovered descriptor index values for identifier: %" PRIu64 ".",
			 function,
			 deleted_index_value->identifier );

			return( -1 );
		}
		result = 0;

		for( index_value_iterator = 0;
		     index_value_iterator < number_of_index_values;
		     index_value_iterator++ )
		{
			result = libpff_index_tree_get_value_by_identifier(
				  io_handle->recovered_descriptor_index_tree,
				  file_io_handle,
				  io_handle->descriptor_index_tree_cache,
				  deleted_index_value->identifier,
				  index_value_iterator,
				  &index_value,
				  error );

			if( result == -1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
				 "%s: unable to retrieve recovered descriptor index value for identifier: %" PRIu64 ".",
				 function,
				 deleted_index_value->identifier );

				return( -1 );
			}
			else if( result != 0 )
			{
				/* TODO what about parent changes ? */
				if( ( deleted_index_value->data_identifier == index_value->data_identifier )
				 && ( deleted_index_value->local_descriptors_identifier == index_value->local_descriptors_identifier ) )
				{
					break;
				}
				result = 0;
			}
		}
		if( result != 0 )
		{
#if defined( HAVE_DEBUG_OUTPUT )
			if( libcnotify_verbose != 0 )
			{
				libcnotify_printf(
				 "%s: deleted descriptor index value: %" PRIu64 " matches existing recovered item value.\n",
				 function,
				 deleted_index_value->identifier );
			}
#endif
			continue;
		}
		/* Add the recovered descriptor index values to the index tree
		 */
#if defined( HAVE_DEBUG_OUTPUT )
		if( libcnotify_verbose != 0 )
		{
			libcnotify_printf(
			 "%s: decriptor index value: %d identifier: %" PRIu64 " is recoverable.\n",
			 function,
			 deleted_index_value_iterator,
			 deleted_index_value->identifier );
		}
#endif
		if( libfdata_tree_node_get_data_range(
		     deleted_index_leaf_node,
		     &node_data_offset,
		     &node_data_size,
		     &node_data_flags,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve deleted descriptor leaf node: %d range.",
			 function,
			 deleted_index_value_iterator );

			return( -1 );
		}
		if( libpff_index_tree_insert_value(
		     io_handle->recovered_descriptor_index_tree,
		     file_io_handle,
		     io_handle->descriptor_index_tree_cache,
		     deleted_index_value->identifier,
		     node_data_offset,
		     node_data_size,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_APPEND_FAILED,
			 "%s: unable to insert descriptor index value: %" PRIu64 " to recovered index tree.",
			 function,
			 deleted_index_value->identifier );

			return( -1 );
		}
	}
#ifdef TODO
fprintf( stderr, "O:\n" );
	/* Scan the existing offset index nodes for remnant values
	 */
	if( libfdata_tree_get_number_of_deleted_leaf_nodes(
	     io_handle->offset_index_tree,
	     file_io_handle,
	     io_handle->offset_index_tree_cache,
	     &number_of_deleted_index_values,
	     0,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve number of deleted offset index values.",
		 function );

		return( -1 );
	}
	for( deleted_index_value_iterator = 0;
	     deleted_index_value_iterator < number_of_deleted_index_values;
	     deleted_index_value_iterator++ )
	{
		if( io_handle->abort != 0 )
		{
			return( -1 );
		}
fprintf( stderr, "O: %d out of %d\n",
 deleted_index_value_iterator, number_of_deleted_index_values );
		if( libfdata_tree_get_deleted_leaf_node_by_index(
		     io_handle->offset_index_tree,
		     file_io_handle,
		     io_handle->offset_index_tree_cache,
		     deleted_index_value_iterator,
		     &deleted_index_leaf_node,
		     0,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
			 "%s: unable to retrieve deleted offset leaf node: %d.",
			 function,
			 deleted_index_value_iterator );

			return( -1 );
		}
		if( libfdata_tree_node_get_node_value(
		     deleted_index_leaf_node,
		     file_io_handle,
		     io_handle->offset_index_tree_cache,
		     (intptr_t **) &deleted_index_value,
		     0,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
			 "%s: unable to retrieve deleted offset index value: %d.",
			 function,
			 deleted_index_value_iterator );

			return( -1 );
		}
		if( deleted_index_value == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
			 "%s: missing deleted offset index value: %d.",
			 function,
			 deleted_index_value_iterator );

			return( -1 );
		}
#if defined( HAVE_DEBUG_OUTPUT )
		if( libcnotify_verbose != 0 )
		{
			libcnotify_printf(
			 "%s: analyzing identifier: %" PRIu64 " (%s) at offset: %" PRIi64 " of size: %" PRIu32 "\n",
			 function,
			 deleted_index_value->identifier,
			 ( ( deleted_index_value->identifier & LIBPFF_OFFSET_INDEX_IDENTIFIER_FLAG_INTERNAL ) ? "internal" : "external" ),
			 deleted_index_value->file_offset,
			 deleted_index_value->data_size );
		}
#endif
		/* Ignore index values without a valid file offset
		 */
		if( deleted_index_value->file_offset <= 0 )
		{
#if defined( HAVE_DEBUG_OUTPUT )
			if( libcnotify_verbose != 0 )
			{
				libcnotify_printf(
				 "%s: deleted offset index value: %" PRIu64 " has an invalid file offset: %" PRIi64 ".\n",
				 function,
				 deleted_index_value->identifier,
				 deleted_index_value->file_offset );
			}
#endif
			continue;
		}
		/* Ignore index values without a valid data size
		 */
		if( ( deleted_index_value->data_size == 0 )
		 || ( deleted_index_value->data_size > 8192 ) )
		{
#if defined( HAVE_DEBUG_OUTPUT )
			if( libcnotify_verbose != 0 )
			{
				libcnotify_printf(
				 "%s: deleted offset index value: %" PRIu64 " has an invalid data size: %" PRIu32 ".\n",
				 function,
				 deleted_index_value->identifier,
				 deleted_index_value->data_size );
			}
#endif
			continue;
		}
		/* Check if the item value matches the existing item value
		 */
		result = libpff_index_tree_get_value_by_identifier(
			  io_handle->offset_index_tree,
			  file_io_handle,
			  io_handle->offset_index_tree_cache,
			  deleted_index_value->identifier,
			  0,
			  &index_value,
			  error );

		if( result == -1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve offset index value for identifier: %" PRIu64 ".",
			 function,
			 deleted_index_value->identifier );

			return( -1 );
		}
		else if( result != 0 )
		{
			if( ( deleted_index_value->file_offset == index_value->file_offset )
			 && ( deleted_index_value->data_size == index_value->data_size ) )
			{
#if defined( HAVE_DEBUG_OUTPUT )
				if( libcnotify_verbose != 0 )
				{
					libcnotify_printf(
					 "%s: deleted offset index value: %" PRIu64 " matches existing item value.\n",
					 function,
					 deleted_index_value->identifier );
				}
#endif
				continue;
			}
		}
		/* Check for duplicates
		 */
		if( libpff_index_tree_get_number_of_leaf_nodes_by_identifier(
		     io_handle->recovered_offset_index_tree,
		     file_io_handle,
		     io_handle->offset_index_tree_cache,
		     deleted_index_value->identifier,
		     &number_of_index_values,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve number of recovered offset index values for identifier: %" PRIu64 ".",
			 function,
			 deleted_index_value->identifier );

			return( -1 );
		}
		result = 0;

		for( index_value_iterator = 0;
		     index_value_iterator < number_of_index_values;
		     index_value_iterator++ )
		{
			result = libpff_index_tree_get_value_by_identifier(
				  io_handle->recovered_offset_index_tree,
				  file_io_handle,
				  io_handle->offset_index_tree_cache,
				  deleted_index_value->identifier,
				  index_value_iterator,
				  &index_value,
				  error );

			if( result == -1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
				 "%s: unable to retrieve recovered offset index value for identifier: %" PRIu64 ".",
				 function,
				 deleted_index_value->identifier );

				return( -1 );
			}
			else if( result != 0 )
			{
				if( ( deleted_index_value->file_offset == index_value->file_offset )
				 && ( deleted_index_value->data_size == index_value->data_size ) )
				{
					break;
				}
				result = 0;
			}
		}
		if( result != 0 )
		{
#if defined( HAVE_DEBUG_OUTPUT )
			if( libcnotify_verbose != 0 )
			{
				libcnotify_printf(
				 "%s: deleted offset index value: %" PRIu64 " matches existing recovered item value.\n",
				 function,
				 deleted_index_value->identifier );
			}
#endif
			continue;
		}
		/* Add the recovered offset index values to the index tree
		 */
#if defined( HAVE_DEBUG_OUTPUT )
		if( libcnotify_verbose != 0 )
		{
			libcnotify_printf(
			 "%s: offset index value: %d identifier: %" PRIu64 " is recoverable.\n",
			 function,
			 deleted_index_value_iterator,
			 deleted_index_value->identifier );
		}
#endif
		if( libfdata_tree_node_get_data_range(
		     deleted_index_leaf_node,
		     &node_data_offset,
		     &node_data_size,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve deleted offset leaf node: %d range.",
			 function,
			 deleted_index_value_iterator );

			return( -1 );
		}
		if( libpff_index_tree_insert_value(
		     io_handle->recovered_offset_index_tree,
		     file_io_handle,
		     io_handle->offset_index_tree_cache,
		     deleted_index_value->identifier,
		     node_data_offset,
		     node_data_size,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_APPEND_FAILED,
			 "%s: unable to insert offset index value: %" PRIu64 " to recovered index tree.",
			 function,
			 deleted_index_value->identifier );

			return( -1 );
		}
	}
#endif
	return( 1 );
}

/* Scans for recoverable data blocks
 * Returns 1 if successful or -1 on error
 */
int libpff_io_handle_recover_data_blocks(
     libpff_io_handle_t *io_handle,
     libbfio_handle_t *file_io_handle,
     libpff_offset_list_t *unallocated_data_block_list,
     libpff_offset_list_t *unallocated_page_block_list,
     uint8_t recovery_flags,
     libcerror_error_t **error )
{
	libpff_index_value_t *index_value                          = NULL;
	libpff_list_element_t *unallocated_data_block_list_element = NULL;
	libpff_list_element_t *unallocated_page_block_list_element = NULL;
	uint8_t *block_buffer                                      = NULL;
	uint8_t *data_block_footer                                 = NULL;
	static char *function                                      = "libpff_io_handle_recover_data_blocks";
	off64_t block_buffer_data_offset                           = 0;
	off64_t block_offset                                       = 0;
	off64_t data_block_offset                                  = 0;
	off64_t page_block_offset                                  = 0;
	size64_t block_size                                        = 0;
	size64_t data_block_size                                   = 0;
	size64_t page_block_size                                   = 0;
	size_t block_buffer_offset                                 = 0;
	size_t block_buffer_size_available                         = 0;
	size_t data_block_data_offset                              = 0;
	size_t read_size                                           = 0;
	size_t scan_block_size                                     = 0;
	ssize_t read_count                                         = 0;
	uint64_t data_block_back_pointer                           = 0;
	uint32_t data_block_calculated_crc                         = 0;
	uint32_t data_block_stored_crc                             = 0;
	uint16_t data_block_data_size                              = 0;
	uint8_t supported_recovery_flags                           = 0;
	int index_value_iterator                                   = 0;
	int number_of_index_values                                 = 0;
	int number_of_unallocated_data_blocks                      = 0;
	int number_of_unallocated_page_blocks                      = 0;
	int result                                                 = 0;
	int unallocated_data_block_iterator                        = 0;
	int unallocated_page_block_iterator                        = 0;

	if( io_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid IO handle.",
		 function );

		return( -1 );
	}
	supported_recovery_flags = LIBPFF_RECOVERY_FLAG_IGNORE_ALLOCATION_DATA
	                         | LIBPFF_RECOVERY_FLAG_SCAN_FOR_FRAGMENTS;

	if( ( recovery_flags & ~( supported_recovery_flags ) ) != 0 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_UNSUPPORTED_VALUE,
		 "%s: unsupported recovery flags.",
		 function );

		return( -1 );
	}
	if( ( recovery_flags & LIBPFF_RECOVERY_FLAG_IGNORE_ALLOCATION_DATA ) == 0 )
	{
		if( unallocated_data_block_list == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
			 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
			 "%s: invalid unallocated data block list.",
			 function );

			return( -1 );
		}
		if( unallocated_page_block_list == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
			 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
			 "%s: invalid unallocated page block list.",
			 function );

			return( -1 );
		}
	}
	/* Scan the unallocated page block list or all blocks for index nodes
	 */
	if( ( recovery_flags & LIBPFF_RECOVERY_FLAG_IGNORE_ALLOCATION_DATA ) == 0 )
	{
		if( libpff_offset_list_get_number_of_elements(
		     unallocated_data_block_list,
		     &number_of_unallocated_data_blocks,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve number of unallocated data block list elements.",
			 function );

			return( -1 );
		}
		unallocated_data_block_list_element = unallocated_data_block_list->first_element;

		if( libpff_offset_list_get_number_of_elements(
		     unallocated_page_block_list,
		     &number_of_unallocated_page_blocks,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve number of unallocated page block list elements.",
			 function );

			return( -1 );
		}
		unallocated_page_block_list_element = unallocated_page_block_list->first_element;
	}
	if( ( recovery_flags & LIBPFF_RECOVERY_FLAG_SCAN_FOR_FRAGMENTS ) == 0 )
	{
		scan_block_size = 512;
	}
	else
	{
		scan_block_size = 64;
	}
	block_buffer = (uint8_t *) memory_allocate(
	                            sizeof( uint8_t ) * ( 8192 * 2 ) );

	if( block_buffer == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create block buffer.",
		 function );

		return( -1 );
	}
	if( ( number_of_unallocated_data_blocks > 0 )
	 || ( number_of_unallocated_page_blocks > 0 )
	 || ( ( recovery_flags & LIBPFF_RECOVERY_FLAG_IGNORE_ALLOCATION_DATA ) != 0 ) )
	{
		block_offset      = 0;
		data_block_offset = -1;
		page_block_offset = -1;

		while( block_offset < (off64_t) io_handle->file_size )
		{
			if( io_handle->abort != 0 )
			{
				memory_free(
				 block_buffer );

				return( -1 );
			}
			if( ( recovery_flags & LIBPFF_RECOVERY_FLAG_IGNORE_ALLOCATION_DATA ) == 0 )
			{
				if( data_block_offset < block_offset )
				{
					if( unallocated_data_block_iterator < number_of_unallocated_data_blocks )
					{
						/* Retrieve the next unallocated data range
						 */
						if( unallocated_data_block_list_element == NULL )
						{
							libcerror_error_set(
							 error,
							 LIBCERROR_ERROR_DOMAIN_RUNTIME,
							 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
							 "%s: corruption detected for unallocated data block list element: %d.",
							 function,
							 unallocated_data_block_iterator );

							memory_free(
							 block_buffer );

							return( -1 );
						}
						if( unallocated_data_block_list_element->value == NULL )
						{
							libcerror_error_set(
							 error,
							 LIBCERROR_ERROR_DOMAIN_RUNTIME,
							 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
							 "%s: missing offset values for unallocate data block list element: %d.",
							 function,
							 unallocated_data_block_iterator );

							memory_free(
							 block_buffer );

							return( -1 );
						}
						data_block_offset = ( (libpff_offset_list_value_t *) unallocated_data_block_list_element->value )->offset;
						data_block_size   = ( (libpff_offset_list_value_t *) unallocated_data_block_list_element->value )->size;

						unallocated_data_block_list_element = unallocated_data_block_list_element->next_element;

						unallocated_data_block_iterator++;
					}
					else
					{
						data_block_offset = (off64_t) io_handle->file_size;
						data_block_size   = 0;
					}
				}
				if( page_block_offset < block_offset )
				{
					if( unallocated_page_block_iterator < number_of_unallocated_page_blocks )
					{
						/* Retrieve the next unallocated data range
						 */
						if( unallocated_page_block_list_element == NULL )
						{
							libcerror_error_set(
							 error,
							 LIBCERROR_ERROR_DOMAIN_RUNTIME,
							 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
							 "%s: corruption detected for unallocated page block list element: %d.",
							 function,
							 unallocated_page_block_iterator );

							memory_free(
							 block_buffer );

							return( -1 );
						}
						if( unallocated_page_block_list_element->value == NULL )
						{
							libcerror_error_set(
							 error,
							 LIBCERROR_ERROR_DOMAIN_RUNTIME,
							 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
							 "%s: missing offset values for unallocate page block list element: %d.",
							 function,
							 unallocated_page_block_iterator );

							memory_free(
							 block_buffer );

							return( -1 );
						}
						page_block_offset = ( (libpff_offset_list_value_t *) unallocated_page_block_list_element->value )->offset;
						page_block_size   = ( (libpff_offset_list_value_t *) unallocated_page_block_list_element->value )->size;

						unallocated_page_block_list_element = unallocated_page_block_list_element->next_element;

						unallocated_page_block_iterator++;
					}
					else
					{
						page_block_offset = (off64_t) io_handle->file_size;
						page_block_size   = 0;
					}
				}
			}
			if( ( recovery_flags & LIBPFF_RECOVERY_FLAG_IGNORE_ALLOCATION_DATA ) == 0 )
			{
				if( ( data_block_offset >= (off64_t) io_handle->file_size )
				 && ( page_block_offset >= (off64_t) io_handle->file_size ) )
				{
					break;
				}
				/* Process the smallest offset
				 */
				else if( ( data_block_offset < page_block_offset )
				      && ( data_block_size > scan_block_size ) )
				{
					block_offset = data_block_offset;
					block_size   = data_block_size;
				}
				else if( ( page_block_offset < data_block_offset )
				      && ( page_block_size > scan_block_size ) )
				{
					block_offset = page_block_offset;
					block_size   = page_block_size;
				}
				/* Process the largest range
				 */
				else if( data_block_size > page_block_size )
				{
					block_offset = data_block_offset;
					block_size   = data_block_size;
				}
				else
				{
					block_offset = page_block_offset;
					block_size   = page_block_size;
				}
			}
			else
			{
				block_size = scan_block_size;
			}
			if( ( block_offset % scan_block_size ) != 0 )
			{
				block_offset  = ( ( block_offset / scan_block_size ) + 1 ) * scan_block_size;
				block_size   -= block_size % scan_block_size;
			}
			if( block_size < scan_block_size )
			{
				block_offset += block_size;

				continue;
			}
			while( block_size >= scan_block_size )
			{
				/* The index nodes are 512 bytes of size and 512 bytes aligned
				 */
				if( ( block_size >= 512 )
				 && ( ( block_offset % 512 ) == 0 ) )
				{
					/* Scan for index values in the index node
					 */
					result = libpff_io_handle_recover_index_values(
						  io_handle,
						  file_io_handle,
						  unallocated_data_block_list,
						  block_offset,
						  recovery_flags,
						  error );

					if( result == -1 )
					{
						libcerror_error_set(
						 error,
						 LIBCERROR_ERROR_DOMAIN_RUNTIME,
						 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
						 "%s: unable to recover index node at offset: %" PRIi64 ".\n",
						 function,
						 block_offset );

						memory_free(
						 block_buffer );

						return( -1 );
					}
					else if( result == 1 )
					{
						block_offset += 512;
						block_size   -= 512;

						continue;
					}
				}
				if( ( recovery_flags & LIBPFF_RECOVERY_FLAG_SCAN_FOR_FRAGMENTS ) == 0 )
				{
					block_offset += scan_block_size;
					block_size   -= scan_block_size;

					continue;
				}
				/* TODO optimize by minimizing amount of reads */
				if( block_buffer_size_available == 0 )
				{
					block_buffer_data_offset = block_offset;

					if( block_buffer_offset > 0 )
					{
						/* TODO optimize by copying the needed data to the front of the buffer */

						block_buffer_data_offset -= 64;
						block_buffer_offset       = 8192 - 64;
					}
					read_size = (size_t) block_size;

					if( read_size > 8192 )
					{
						read_size = 8192;
					}
#if defined( HAVE_DEBUG_OUTPUT )
					if( libcnotify_verbose != 0 )
					{
						libcnotify_printf(
						 "%s: reading data block at offset: %" PRIi64 " (0x%08" PRIx64 ") of size: %" PRIzd "\n",
						 function,
						 block_buffer_data_offset,
						 block_buffer_data_offset,
						 read_size );
					}
#endif
					if( libbfio_handle_seek_offset(
					     file_io_handle,
					     block_buffer_data_offset,
					     SEEK_SET,
					     error ) == -1 )
					{
						libcerror_error_set(
						 error,
						 LIBCERROR_ERROR_DOMAIN_IO,
						 LIBCERROR_IO_ERROR_SEEK_FAILED,
						 "%s: unable to seek data block offset: %" PRIi64 ".",
						 function,
						 block_buffer_data_offset );

						memory_free(
						 block_buffer );

						return( -1 );
					}
					read_count = libbfio_handle_read_buffer(
						      file_io_handle,
						      &( block_buffer[ block_buffer_offset ] ),
						      read_size,
						      error );

					if( read_count != (ssize_t) read_size )
					{
						libcerror_error_set(
						 error,
						 LIBCERROR_ERROR_DOMAIN_IO,
						 LIBCERROR_IO_ERROR_READ_FAILED,
						 "%s: unable to read data block.",
						 function );

						memory_free(
						 block_buffer );

						return( -1 );
					}
					block_buffer_size_available = read_size;
				}
				if( block_buffer_size_available >= 64 )
				{
					/* Scan the block for a data block footer
					 */
					data_block_footer = &( block_buffer[ block_buffer_offset ] );

					if( io_handle->file_type == LIBPFF_FILE_TYPE_32BIT )
					{
						data_block_footer += 64 - sizeof( pff_block_footer_32bit_t );

						byte_stream_copy_to_uint16_little_endian(
						 ( (pff_block_footer_32bit_t *) data_block_footer )->size,
						 data_block_data_size );
						byte_stream_copy_to_uint32_little_endian(
						 ( (pff_block_footer_32bit_t *) data_block_footer )->back_pointer,
						 data_block_back_pointer );
						byte_stream_copy_to_uint32_little_endian(
						 ( (pff_block_footer_32bit_t *) data_block_footer )->crc,
						 data_block_stored_crc );
					}
					else if( io_handle->file_type == LIBPFF_FILE_TYPE_64BIT )
					{
						data_block_footer += 64 - sizeof( pff_block_footer_64bit_t );

						byte_stream_copy_to_uint16_little_endian(
						 ( (pff_block_footer_64bit_t *) data_block_footer )->size,
						 data_block_data_size );
						byte_stream_copy_to_uint32_little_endian(
						 ( (pff_block_footer_64bit_t *) data_block_footer )->crc,
						 data_block_stored_crc );
						byte_stream_copy_to_uint64_little_endian(
						 ( (pff_block_footer_64bit_t *) data_block_footer )->back_pointer,
						 data_block_back_pointer );
					}
					/* Check if back pointer itself is not empty but the upper 32-bit are
					 */
					if( ( data_block_back_pointer != 0 )
					 && ( ( data_block_back_pointer >> 32 ) == 0 ) )
					{
						data_block_data_offset = block_buffer_offset - ( ( data_block_data_size / 64 ) * 64 );

						if( (size_t) data_block_data_size < read_size )
						{
							/* Check for duplicates
							 */
							if( libpff_index_tree_get_number_of_leaf_nodes_by_identifier(
							     io_handle->recovered_offset_index_tree,
							     file_io_handle,
							     io_handle->offset_index_tree_cache,
							     data_block_back_pointer,
							     &number_of_index_values,
							     error ) != 1 )
							{
								libcerror_error_set(
								 error,
								 LIBCERROR_ERROR_DOMAIN_RUNTIME,
								 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
								 "%s: unable to retrieve number of recovered offset index values for identifier: %" PRIu64 ".",
								 function,
								 data_block_back_pointer );

								memory_free(
								 block_buffer );

								return( -1 );
							}
							result = 0;

							for( index_value_iterator = 0;
							     index_value_iterator < number_of_index_values;
							     index_value_iterator++ )
							{
								result = libpff_index_tree_get_value_by_identifier(
									  io_handle->recovered_offset_index_tree,
									  file_io_handle,
									  io_handle->offset_index_tree_cache,
									  data_block_back_pointer,
									  index_value_iterator,
									  &index_value,
									  error );

								if( result == -1 )
								{
									libcerror_error_set(
									 error,
									 LIBCERROR_ERROR_DOMAIN_RUNTIME,
									 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
									 "%s: unable to retrieve recovered offset index value for identifier: %" PRIu64 ".",
									 function,
									 data_block_back_pointer );

									memory_free(
									 block_buffer );

									return( -1 );
								}
								else if( result != 0 )
								{
									if( ( (off64_t) ( block_buffer_data_offset + data_block_data_offset ) == index_value->file_offset )
									 && ( (size32_t) data_block_data_size == index_value->data_size ) )
									{
										break;
									}
									result = 0;
								}
							}
							if( result != 0 )
							{
#if defined( HAVE_DEBUG_OUTPUT )
								if( libcnotify_verbose != 0 )
								{
									libcnotify_printf(
									 "%s: recovered data block with identifier: %" PRIu64 " matches existing recovered item value.\n",
									 function,
									 data_block_back_pointer );
								}
#endif
								block_offset += 64;
								block_size   -= 64;

								/* TODO reset block_buffer_offset and block_buffer_size_available ? */

								continue;
							}
							if( data_block_stored_crc != 0 )
							{
								if( libfmapi_crc32_weak_calculate(
								     &data_block_calculated_crc,
								     &( block_buffer[ data_block_data_offset ] ),
								     data_block_data_size,
								     0,
								     error ) != 1 )
								{
									libcerror_error_set(
									 error,
									 LIBCERROR_ERROR_DOMAIN_RUNTIME,
									 LIBCERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
									 "%s: unable to calculate CRC-32.",
									 function );

									memory_free(
									 block_buffer );

									return( -1 );
								}
								if( data_block_stored_crc != data_block_calculated_crc )
								{
#if defined( HAVE_DEBUG_OUTPUT )
									if( libcnotify_verbose != 0 )
									{
										libcnotify_printf(
										 "%s: mismatch in data block: %" PRIu64 " CRC-32 ( %" PRIu32 " != %" PRIu32 " ).\n",
										 function,
										 data_block_back_pointer,
										 data_block_stored_crc,
										 data_block_calculated_crc );
									}
#endif
									block_offset += 64;
									block_size   -= 64;

									continue;
								}
							}
							/* TODO consider data block as fragment */

							data_block_data_size   = ( ( data_block_data_size / 64 ) + 1 ) * 64;

#if defined( HAVE_DEBUG_OUTPUT )
							if( libcnotify_verbose != 0 )
							{
								libcnotify_printf(
								 "%s: data block back pointer: 0x%08" PRIx64 "\n",
								 function,
								 data_block_back_pointer );

								libcnotify_printf(
								 "%s: data block data at offset: %" PRIi64 " (0x%08" PRIx64 ") of size: %" PRIzd "\n",
								 function,
								 block_buffer_data_offset + data_block_data_offset,
								 block_buffer_data_offset + data_block_data_offset,
								 data_block_data_size );
								libcnotify_print_data(
								 &( block_buffer[ data_block_data_offset ] ),
								 data_block_data_size,
								 0 );
							}
#endif
						}
					}
					block_buffer_offset         += 64;
					block_buffer_size_available -= 64;
				}
				else
				{
					block_buffer_offset         = 0;
					block_buffer_size_available = 0;
				}
				block_offset += scan_block_size;
				block_size   -= scan_block_size;
			}
		}
	}
	memory_free(
	 block_buffer );

	return( 1 );
}

/* Scans for recoverable index values in an index node
 * Returns 1 if successful, returns 0 if no valid index node could be found or -1 on error
 */
int libpff_io_handle_recover_index_values(
     libpff_io_handle_t *io_handle,
     libbfio_handle_t *file_io_handle,
     libpff_offset_list_t *unallocated_data_block_list,
     size64_t node_offset,
     uint8_t recovery_flags,
     libcerror_error_t **error )
{
	libpff_index_value_t *index_value                 = NULL;
	libpff_index_node_t *index_node                   = NULL;
	uint8_t *node_entry_data                          = NULL;
	const char *index_string                          = NULL;
	static char *function                             = "libpff_io_handle_recover_index_values";
	off64_t index_value_file_offset                   = 0;
	uint64_t index_value_data_identifier              = 0;
	uint64_t index_value_identifier                   = 0;
	uint64_t index_value_local_descriptors_identifier = 0;
	uint16_t index_value_data_size                    = 0;
	uint8_t entry_index                               = 0;
	int index_value_iterator                          = 0;
	int number_of_index_values                        = 0;
	int result                                        = 0;

	if( io_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid IO handle.",
		 function );

		return( -1 );
	}
	if( libpff_index_node_initialize(
	     &index_node,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create index node.",
		 function );

		return( -1 );
	}
	if( libpff_index_node_read(
	     index_node,
	     file_io_handle,
	     node_offset,
	     ( (libpff_io_handle_t *) io_handle )->file_type,
	     error ) != 1 )
	{
#if defined( HAVE_DEBUG_OUTPUT )
		if( ( libcnotify_verbose != 0 )
		 && ( error != NULL )
		 && ( *error != NULL ) )
		{
			libcnotify_print_error_backtrace(
			 *error );
		}
#endif
		libcerror_error_free(
		 error );

		libpff_index_node_free(
		 &index_node,
		 NULL );

		return( 0 );
	}
	if( index_node->type == LIBPFF_INDEX_TYPE_DESCRIPTOR )
	{
		index_string = "descriptor";
	}
	else if( index_node->type == LIBPFF_INDEX_TYPE_OFFSET )
	{
		index_string = "offset";
	}
	if( ( index_node->type != LIBPFF_INDEX_TYPE_DESCRIPTOR )
	 && ( index_node->type != LIBPFF_INDEX_TYPE_OFFSET ) )
	{
#if defined( HAVE_DEBUG_OUTPUT )
		if( libcnotify_verbose != 0 )
		{
			libcnotify_printf(
			 "%s: unsupported index type: 0x%02" PRIx8 ".\n",
			 function,
			 index_node->type );
		}
#endif
	}
	else if( index_node->level != PFF_INDEX_NODE_LEVEL_LEAF )
	{
#if defined( HAVE_DEBUG_OUTPUT )
		if( libcnotify_verbose != 0 )
		{
			libcnotify_printf(
			 "%s: skipping %s index branch node at level: %" PRIu8 ".\n",
			 function,
			 index_string,
			 index_node->level );
		}
#endif
	}
	else
	{
		/* Check if the index leaf entries are recoverable
		 */
		for( entry_index = 0;
		     entry_index < index_node->maximum_number_of_entries;
		     entry_index++ )
		{
			if( libpff_index_node_get_entry_data(
			     index_node,
			     entry_index,
			     &node_entry_data,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
				 "%s: unable to retrieve node entry: %" PRIu8 " data.",
				 function,
				 entry_index );

				libpff_index_node_free(
				 &index_node,
				 NULL );

				return( -1 );
			}
			if( node_entry_data == NULL )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
				 "%s: missing node entry: %" PRIu8 " data.",
				 function,
				 entry_index );

				libpff_index_node_free(
				 &index_node,
				 NULL );

				return( -1 );
			}
			if( io_handle->file_type == LIBPFF_FILE_TYPE_32BIT )
			{
				byte_stream_copy_to_uint32_little_endian(
				 node_entry_data,
				 index_value_identifier );
			}
			else if( io_handle->file_type == LIBPFF_FILE_TYPE_64BIT )
			{
				byte_stream_copy_to_uint64_little_endian(
				 node_entry_data,
				 index_value_identifier );
			}
			/* Ignore the upper 32-bit of descriptor identifiers
			 */
			if( index_node->type == LIBPFF_INDEX_TYPE_DESCRIPTOR )
			{
				index_value_identifier &= 0xffffffffUL;
			}
#if defined( HAVE_DEBUG_OUTPUT )
			if( libcnotify_verbose != 0 )
			{
				libcnotify_printf(
				 "%s: analyzing %s index entry: %" PRIu8 " identifier: %" PRIu64 ".\n",
				 function,
				 index_string,
				 entry_index,
				 index_value_identifier );
			}
#endif
			/* Ignore index values without an identifier
			 */
			if( index_value_identifier == 0 )
			{
#if defined( HAVE_DEBUG_OUTPUT )
				if( libcnotify_verbose != 0 )
				{
					libcnotify_printf(
					 "%s: %s index entry: %" PRIu8 " indentifier: %" PRIu64 " has an empty identifier.\n",
					 function,
					 index_string,
					 entry_index,
					 index_value_identifier );
				}
#endif
				continue;
			}
			if( index_node->type == LIBPFF_INDEX_TYPE_DESCRIPTOR )
			{
				if( io_handle->file_type == LIBPFF_FILE_TYPE_32BIT )
				{
					byte_stream_copy_to_uint32_little_endian(
					 ( (pff_index_node_descriptor_entry_32bit_t *) node_entry_data )->data_identifier,
					 index_value_data_identifier );
					byte_stream_copy_to_uint32_little_endian(
					 ( (pff_index_node_descriptor_entry_32bit_t *) node_entry_data )->local_descriptors_identifier,
					 index_value_local_descriptors_identifier );
				}
				else
				{
					byte_stream_copy_to_uint64_little_endian(
					 ( (pff_index_node_descriptor_entry_64bit_t *) node_entry_data )->data_identifier,
					 index_value_data_identifier );
					byte_stream_copy_to_uint64_little_endian(
					 ( (pff_index_node_descriptor_entry_64bit_t *) node_entry_data )->local_descriptors_identifier,
					 index_value_local_descriptors_identifier );
				}
			}
			else if( index_node->type == LIBPFF_INDEX_TYPE_OFFSET )
			{
				if( io_handle->file_type == LIBPFF_FILE_TYPE_32BIT )
				{
					byte_stream_copy_to_uint32_little_endian(
					 ( (pff_index_node_offset_entry_32bit_t *) node_entry_data )->file_offset,
					 index_value_file_offset );
					byte_stream_copy_to_uint16_little_endian(
					 ( (pff_index_node_offset_entry_32bit_t *) node_entry_data )->data_size,
					 index_value_data_size );
				}
				else if( io_handle->file_type == LIBPFF_FILE_TYPE_64BIT )
				{
					byte_stream_copy_to_uint64_little_endian(
					 ( (pff_index_node_offset_entry_64bit_t *) node_entry_data )->file_offset,
					 index_value_file_offset );
					byte_stream_copy_to_uint16_little_endian(
					 ( (pff_index_node_offset_entry_64bit_t *) node_entry_data )->data_size,
					 index_value_data_size );
				}
			}
			if( index_node->type == LIBPFF_INDEX_TYPE_DESCRIPTOR )
			{
				/* Ignore descriptor index values without a data identifier
				 */
				if( index_value_data_identifier == 0 )
				{
#if defined( HAVE_DEBUG_OUTPUT )
					if( libcnotify_verbose != 0 )
					{
						libcnotify_printf(
						 "%s: %s index entry: %" PRIu8 " identifier: %" PRIu64 " has an empty data identifier.\n",
						 function,
						 index_string,
						 entry_index,
						 index_value_identifier );
					}
#endif
					continue;
				}
			}
			else if( index_node->type == LIBPFF_INDEX_TYPE_OFFSET )
			{
				/* Ignore index values without a valid file offset
				 */
				if( index_value_file_offset <= 0 )
				{
#if defined( HAVE_DEBUG_OUTPUT )
					if( libcnotify_verbose != 0 )
					{
						libcnotify_printf(
						 "%s: %s index entry: %" PRIu8 " identifier: %" PRIu64 " has an invalid file offset: %" PRIi64 ".\n",
						 function,
						 index_string,
						 entry_index,
						 index_value_identifier,
						 index_value_file_offset );
					}
#endif
					continue;
				}
				/* Ignore index values without a valid data size
				 */
				if( ( index_value_data_size == 0 )
				 || ( index_value_data_size > 8192 ) )
				{
#if defined( HAVE_DEBUG_OUTPUT )
					if( libcnotify_verbose != 0 )
					{
						libcnotify_printf(
						 "%s: %s index entry: %" PRIu8 " identifier: %" PRIu64 " has an invalid data size: %" PRIu16 ".\n",
						 function,
						 index_string,
						 entry_index,
						 index_value_identifier,
						 index_value_data_size );
					}
#endif
					continue;
				}
			}
			/* Check if the recovered item value matches the existing item value
			 */
			if( index_node->type == LIBPFF_INDEX_TYPE_DESCRIPTOR )
			{
				result = libpff_index_tree_get_value_by_identifier(
					  io_handle->descriptor_index_tree,
					  file_io_handle,
					  io_handle->descriptor_index_tree_cache,
					  index_value_identifier,
					  0,
					  &index_value,
					  error );
			}
			else if( index_node->type == LIBPFF_INDEX_TYPE_OFFSET )
			{
				result = libpff_index_tree_get_value_by_identifier(
					  io_handle->offset_index_tree,
					  file_io_handle,
					  io_handle->offset_index_tree_cache,
					  index_value_identifier,
					  0,
					  &index_value,
					  error );
			}
			if( result == -1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
				 "%s: unable to retrieve %s index value for identifier: %" PRIu64 ".",
				 function,
				 index_string,
				 index_value_identifier );

				return( -1 );
			}
			else if( result != 0 )
			{
				if( index_node->type == LIBPFF_INDEX_TYPE_DESCRIPTOR )
				{
					/* TODO what about parent changes ? */
					if( ( index_value_data_identifier == index_value->data_identifier )
					 && ( index_value_local_descriptors_identifier == index_value->local_descriptors_identifier ) )
					{
#if defined( HAVE_DEBUG_OUTPUT )
						if( libcnotify_verbose != 0 )
						{
							libcnotify_printf(
							 "%s: recovered descriptor index value: %" PRIu64 " matches existing item value.\n",
							 function,
							 index_value_identifier );
						}
#endif
						continue;
					}
				}
				else if( index_node->type == LIBPFF_INDEX_TYPE_OFFSET )
				{
					if( ( index_value_file_offset == index_value->file_offset )
					 && ( (size32_t) index_value_data_size == index_value->data_size ) )
					{
#if defined( HAVE_DEBUG_OUTPUT )
						if( libcnotify_verbose != 0 )
						{
							libcnotify_printf(
							 "%s: recovered offset index value: %" PRIu64 " matches existing item value.\n",
							 function,
							 index_value_identifier );
						}
#endif
						continue;
					}
				}
			}
			/* Check for duplicates
			 */
			if( index_node->type == LIBPFF_INDEX_TYPE_DESCRIPTOR )
			{
				result = libpff_index_tree_get_number_of_leaf_nodes_by_identifier(
					  io_handle->recovered_descriptor_index_tree,
					  file_io_handle,
					  io_handle->descriptor_index_tree_cache,
					  index_value_identifier,
					  &number_of_index_values,
					  error );
			}
			else if( index_node->type == LIBPFF_INDEX_TYPE_OFFSET )
			{
				result = libpff_index_tree_get_number_of_leaf_nodes_by_identifier(
					  io_handle->recovered_offset_index_tree,
					  file_io_handle,
					  io_handle->offset_index_tree_cache,
					  index_value_identifier,
					  &number_of_index_values,
					  error );
			}
			if( result != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
				 "%s: unable to retrieve number of recovered %s index values for identifier: %" PRIu64 ".",
				 function,
				 index_string,
				 index_value_identifier );

				return( -1 );
			}
			result = 0;

			for( index_value_iterator = 0;
			     index_value_iterator < number_of_index_values;
			     index_value_iterator++ )
			{
				if( index_node->type == LIBPFF_INDEX_TYPE_DESCRIPTOR )
				{
					result = libpff_index_tree_get_value_by_identifier(
						  io_handle->recovered_descriptor_index_tree,
						  file_io_handle,
						  io_handle->descriptor_index_tree_cache,
						  index_value_identifier,
						  index_value_iterator,
						  &index_value,
						  error );
				}
				else if( index_node->type == LIBPFF_INDEX_TYPE_OFFSET )
				{
					result = libpff_index_tree_get_value_by_identifier(
						  io_handle->recovered_offset_index_tree,
						  file_io_handle,
						  io_handle->offset_index_tree_cache,
						  index_value_identifier,
						  index_value_iterator,
						  &index_value,
						  error );
				}
				if( result == -1 )
				{
					libcerror_error_set(
					 error,
					 LIBCERROR_ERROR_DOMAIN_RUNTIME,
					 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
					 "%s: unable to retrieve recovered %s index value for identifier: %" PRIu64 ".",
					 function,
					 index_string,
					 index_value_identifier );

					return( -1 );
				}
				else if( result != 0 )
				{
					if( index_node->type == LIBPFF_INDEX_TYPE_DESCRIPTOR )
					{
						/* TODO what about parent changes ? */
						if( ( index_value_data_identifier == index_value->data_identifier )
						 && ( index_value_local_descriptors_identifier == index_value->local_descriptors_identifier ) )
						{
							break;
						}
					}
					else if( index_node->type == LIBPFF_INDEX_TYPE_OFFSET )
					{
						if( ( index_value_file_offset == index_value->file_offset )
						 && ( (size32_t) index_value_data_size == index_value->data_size ) )
						{
							break;
						}
					}
					result = 0;
				}
			}
			if( result != 0 )
			{
#if defined( HAVE_DEBUG_OUTPUT )
				if( libcnotify_verbose != 0 )
				{
					libcnotify_printf(
					 "%s: recovered %s index value: %" PRIu64 " matches existing item value.\n",
					 function,
					 index_string,
					 index_value_identifier );
				}
#endif
				continue;
			}
			/* Check if the offset index value is unallocated according to the
			 * unallocated data block list
			 */
			if( ( index_node->type == LIBPFF_INDEX_TYPE_OFFSET )
			 && ( ( recovery_flags & LIBPFF_RECOVERY_FLAG_IGNORE_ALLOCATION_DATA ) == 0 ) )
			{
				result = libpff_offset_list_range_is_present(
					  unallocated_data_block_list,
					  index_value_file_offset,
					  (off64_t) index_value_data_size,
					  error );

				if( result == -1 )
				{
					libcerror_error_set(
					 error,
					 LIBCERROR_ERROR_DOMAIN_RUNTIME,
					 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
					 "%s: error determining if offset range is unallocated.",
					 function );

					libpff_index_node_free(
					 &index_node,
					 NULL );

					return( -1 );
				}
				else if( result == 0 )
				{
#if defined( HAVE_DEBUG_OUTPUT )
					if( libcnotify_verbose != 0 )
					{
						libcnotify_printf(
						 "%s: %s index entry: %" PRIu8 " identifier: %" PRIu64 " refers to allocated range: %" PRIi64 " - %" PRIi64 ".\n",
						 function,
						 index_string,
						 entry_index,
						 index_value_identifier,
						 index_value_file_offset,
						 index_value_file_offset + (off64_t) index_value_data_size );
					}
#endif
					continue;
				}
#if defined( HAVE_DEBUG_OUTPUT )
				if( libcnotify_verbose != 0 )
				{
					libcnotify_printf(
					 "%s: %s index entry: %" PRIu8 " identifier: %" PRIu64 " refers to unallocated range: %" PRIi64 " - %" PRIi64 ".\n",
					 function,
					 index_string,
					 entry_index,
					 index_value_identifier,
					 index_value_file_offset,
					 index_value_file_offset + (off64_t) index_value_data_size );
				}
#endif
			}
			/* Move the recovered node to the index
			 */
#if defined( HAVE_DEBUG_OUTPUT )
			if( libcnotify_verbose != 0 )
			{
				libcnotify_printf(
				 "%s: %s index entry: %" PRIu8 " identifier: %" PRIu64 " is recoverable.\n",
				 function,
				 index_string,
				 entry_index,
				 index_value_identifier );
			}
#endif
			if( index_node->type == LIBPFF_INDEX_TYPE_DESCRIPTOR )
			{
				result = libpff_index_tree_insert_value(
					  io_handle->recovered_descriptor_index_tree,
					  file_io_handle,
					  io_handle->descriptor_index_tree_cache,
					  index_value_identifier,
					  node_offset,
				          (size64_t) entry_index,
					  error );
			}
			else if( index_node->type == LIBPFF_INDEX_TYPE_OFFSET )
			{
				result = libpff_index_tree_insert_value(
					  io_handle->recovered_offset_index_tree,
					  file_io_handle,
					  io_handle->offset_index_tree_cache,
					  index_value_identifier,
					  node_offset,
				          (size64_t) entry_index,
					  error );
			}
			if( result != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_APPEND_FAILED,
				 "%s: unable to insert %s index value: %" PRIu64 " to recovered index tree.",
				 function,
				 index_string,
				 index_value_identifier );

				libpff_index_node_free(
				 &index_node,
				 NULL );

				return( -1 );
			}
			node_offset += index_node->entry_size;
		}
	}
	if( libpff_index_node_free(
	     &index_node,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
		 "%s: unable to free index node.",
		 function );

		return( -1 );
	}
#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		libcnotify_printf(
		 "\n" );
	}
#endif
	return( 1 );
}

/* Scans for recoverable local descriptors
 * Returns 1 if successful, returns 0 if no valid local descriptors node could be found or -1 on error
 */
int libpff_io_handle_recover_local_descriptors(
     libpff_io_handle_t *io_handle,
     libbfio_handle_t *file_io_handle,
     uint64_t local_descriptors_identifier,
     libcerror_error_t **error )
{
	libpff_index_value_t *offset_index_value                     = NULL;
	libpff_local_descriptor_node_t *local_descriptor_node        = NULL;
	uint8_t *node_entry_data                                     = NULL;
	static char *function                                        = "libpff_io_handle_recover_local_descriptors";
	uint64_t local_descriptor_value_identifier                   = 0;
	uint64_t local_descriptor_value_data_identifier              = 0;
	uint64_t local_descriptor_value_local_descriptors_identifier = 0;
	uint64_t local_descriptor_value_sub_node_identifier          = 0;
	uint16_t entry_index                                         = 0;
	int result                                                   = 1;

	if( io_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid IO handle.",
		 function );

		return( -1 );
	}
	if( libpff_io_handle_get_offset_index_value_by_identifier(
	     io_handle,
	     file_io_handle,
	     local_descriptors_identifier,
	     1,
	     0,
	     &offset_index_value,
	     error ) != 1 )
	{
#if defined( HAVE_DEBUG_OUTPUT )
		if( ( libcnotify_verbose != 0 )
		 && ( error != NULL )
		 && ( *error != NULL ) )
		{
			libcnotify_print_error_backtrace(
			 *error );
		}
#endif
		libcerror_error_free(
		 error );

		return( 0 );
	}
	if( offset_index_value == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: missing offset index value.",
		 function );

		return( -1 );
	}
#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		libcnotify_printf(
		 "%s: local descriptor node identifier: %" PRIu64 " (%s) at offset: %" PRIi64 " of size: %" PRIu32 "\n",
		 function,
		 offset_index_value->identifier,
		 ( ( offset_index_value->identifier & LIBPFF_OFFSET_INDEX_IDENTIFIER_FLAG_INTERNAL ) ? "internal" : "external" ),
		 offset_index_value->file_offset,
		 offset_index_value->data_size );
	}
#endif
	if( libpff_local_descriptor_node_initialize(
	     &local_descriptor_node,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create local descriptor node.",
		 function );

		return( -1 );
	}
	if( libpff_local_descriptor_node_read(
	     local_descriptor_node,
	     file_io_handle,
	     offset_index_value->file_offset,
	     offset_index_value->data_size,
	     ( (libpff_io_handle_t *) io_handle )->file_type,
	     error ) != 1 )
	{
#if defined( HAVE_DEBUG_OUTPUT )
		if( ( libcnotify_verbose != 0 )
		 && ( error != NULL )
		 && ( *error != NULL ) )
		{
			libcnotify_print_error_backtrace(
			 *error );
		}
#endif
		libcerror_error_free(
		 error );

		libpff_local_descriptor_node_free(
		 &local_descriptor_node,
		 NULL );

		return( 0 );
	}
	for( entry_index = 0;
	     entry_index < local_descriptor_node->number_of_entries;
	     entry_index++ )
	{
		if( libpff_local_descriptor_node_get_entry_data(
		     local_descriptor_node,
		     entry_index,
		     &node_entry_data,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve node entry: %" PRIu16 " data.",
			 function,
			 entry_index );

			return( -1 );
		}
		if( node_entry_data == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
			 "%s: missing node entry: %" PRIu16 " data.",
			 function,
			 entry_index );

			return( -1 );
		}
		if( io_handle->file_type == LIBPFF_FILE_TYPE_32BIT )
		{
			byte_stream_copy_to_uint32_little_endian(
			 node_entry_data,
			 local_descriptor_value_identifier );

			node_entry_data += 4;
		}
		else if( io_handle->file_type == LIBPFF_FILE_TYPE_64BIT )
		{
			byte_stream_copy_to_uint64_little_endian(
			 node_entry_data,
			 local_descriptor_value_identifier );

			node_entry_data += 8;
		}
		/* Ignore the upper 32-bit of local descriptor identifiers
		 */
		local_descriptor_value_identifier &= 0xffffffffUL;

		/* Ignore local descriptor values without a data identifier
		 */
		if( local_descriptor_value_identifier == 0 )
		{
#if defined( HAVE_DEBUG_OUTPUT )
			if( libcnotify_verbose != 0 )
			{
				libcnotify_printf(
				 "%s: local descriptor entry: %" PRIu8 " identifier: %" PRIu64 " has an empty identifier.\n",
				 function,
				 entry_index,
				 local_descriptor_value_identifier );
			}
#endif
			result = 0;

			break;
		}
		if( local_descriptor_node->level == PFF_LOCAL_DESCRIPTOR_NODE_LEVEL_LEAF )
		{
			if( io_handle->file_type == LIBPFF_FILE_TYPE_32BIT )
			{
				byte_stream_copy_to_uint32_little_endian(
				 node_entry_data,
				 local_descriptor_value_data_identifier );

				node_entry_data += 4;

				byte_stream_copy_to_uint32_little_endian(
				 node_entry_data,
				 local_descriptor_value_local_descriptors_identifier );

				node_entry_data += 4;
			}
			else if( io_handle->file_type == LIBPFF_FILE_TYPE_64BIT )
			{
				byte_stream_copy_to_uint64_little_endian(
				 node_entry_data,
				 local_descriptor_value_data_identifier );

				node_entry_data += 8;

				byte_stream_copy_to_uint64_little_endian(
				 node_entry_data,
				 local_descriptor_value_local_descriptors_identifier );

				node_entry_data += 8;
			}
			/* Ignore local descriptor values without a data identifier
			 */
			if( local_descriptor_value_data_identifier == 0 )
			{
#if defined( HAVE_DEBUG_OUTPUT )
				if( libcnotify_verbose != 0 )
				{
					libcnotify_printf(
					 "%s: local descriptor entry: %" PRIu8 " identifier: %" PRIu64 " has an empty data identifier.\n",
					 function,
					 entry_index,
					 local_descriptor_value_identifier );
				}
#endif
				result = 0;

				break;
			}
		}
		else
		{
			if( io_handle->file_type == LIBPFF_FILE_TYPE_32BIT )
			{
				byte_stream_copy_to_uint32_little_endian(
				 node_entry_data,
				 local_descriptor_value_sub_node_identifier );

				node_entry_data += 4;
			}
			else if( io_handle->file_type == LIBPFF_FILE_TYPE_64BIT )
			{
				byte_stream_copy_to_uint64_little_endian(
				 node_entry_data,
				 local_descriptor_value_sub_node_identifier );

				node_entry_data += 8;
			}
			/* Ignore local descriptor values without a sub node identifier
			 */
			if( local_descriptor_value_sub_node_identifier == 0 )
			{
#if defined( HAVE_DEBUG_OUTPUT )
				if( libcnotify_verbose != 0 )
				{
					libcnotify_printf(
					 "%s: local descriptor entry: %" PRIu8 " identifier: %" PRIu64 " has an empty sub node identifier.\n",
					 function,
					 entry_index,
					 local_descriptor_value_identifier );
				}
#endif
				result = 0;

				break;
			}
			result = libpff_io_handle_recover_local_descriptors(
				  io_handle,
				  file_io_handle,
				  local_descriptor_value_sub_node_identifier,
				  error );

			if( result == -1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_IO,
				 LIBCERROR_IO_ERROR_READ_FAILED,
				 "%s: unable to read local descriptors with identifier: %" PRIu64 ".",
				 function,
				 local_descriptor_value_sub_node_identifier );

				libpff_local_descriptor_node_free(
				 &local_descriptor_node,
				 NULL );

				return( -1 );
			}
			else if( result == 0 )
			{
				break;
			}
		}
	}
	if( libpff_local_descriptor_node_free(
	     &local_descriptor_node,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
		 "%s: unable to free local descriptor node.",
		 function );

		return( -1 );
	}
#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		libcnotify_printf(
		 "\n" );
	}
#endif
	return( result );
}

