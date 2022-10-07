/*
 * Local descriptors functions
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
#include "libpff_io_handle.h"
#include "libpff_libbfio.h"
#include "libpff_libcerror.h"
#include "libpff_libcnotify.h"
#include "libpff_libfcache.h"
#include "libpff_libfdata.h"
#include "libpff_local_descriptor_node.h"
#include "libpff_local_descriptor_value.h"
#include "libpff_local_descriptors.h"
#include "libpff_unused.h"

#include "pff_local_descriptor_node.h"

/* Initializes the local descriptors and its values
 * Returns 1 if successful or -1 on error
 */
int libpff_local_descriptors_initialize(
     libpff_local_descriptors_t **local_descriptors,
     libpff_io_handle_t *io_handle,
     off64_t root_node_offset,
     size32_t root_node_size,
     uint8_t recovered,
     libcerror_error_t **error )
{
	static char *function = "libpff_local_descriptors_initialize";
	int element_index     = 0;

	if( local_descriptors == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid local descriptors.",
		 function );

		return( -1 );
	}
	if( *local_descriptors != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid local descriptors value already set.",
		 function );

		return( -1 );
	}
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
	*local_descriptors = memory_allocate_structure(
	                      libpff_local_descriptors_t );

	if( *local_descriptors == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create local descriptors.",
		 function );

		goto on_error;
	}
	if( memory_set(
	     *local_descriptors,
	     0,
	     sizeof( libpff_local_descriptors_t ) ) == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_SET_FAILED,
		 "%s: unable to clear local descriptors.",
		 function );

		memory_free(
		 *local_descriptors );

		*local_descriptors = NULL;

		return( -1 );
	}
	if( libfdata_list_initialize(
	     &( ( *local_descriptors )->local_descriptor_nodes_list ),
	     (intptr_t* ) io_handle,
	     NULL,
	     NULL,
	     &libpff_io_handle_read_local_descriptor_node,
	     LIBFDATA_FLAG_IO_HANDLE_NON_MANAGED,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create local descriptor nodes list.",
		 function );

		goto on_error;
	}
	if( libfcache_cache_initialize(
	     &( ( *local_descriptors )->local_descriptor_nodes_cache ),
	     LIBPFF_MAXIMUM_CACHE_ENTRIES_LOCAL_DESCRIPTORS_NODES,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create local descriptor nodes cache.",
		 function );

		goto on_error;
	}
	if( libfdata_list_append_element(
	     ( *local_descriptors )->local_descriptor_nodes_list,
	     &element_index,
	     root_node_offset,
	     (size64_t) root_node_size,
	     0,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_APPEND_FAILED,
		 "%s: unable to append local descriptor nodes list element.",
		 function );

		goto on_error;
	}
	( *local_descriptors )->io_handle        = io_handle;
	( *local_descriptors )->root_node_offset = root_node_offset;
	( *local_descriptors )->root_node_size   = root_node_size;
	( *local_descriptors )->recovered        = recovered;

	return( 1 );

on_error:
	if( *local_descriptors != NULL )
	{
		if( ( *local_descriptors )->local_descriptor_nodes_cache != NULL )
		{
			libfcache_cache_free(
			 &( ( *local_descriptors )->local_descriptor_nodes_cache ),
			 NULL );
		}
		if( ( *local_descriptors )->local_descriptor_nodes_list != NULL )
		{
			libfdata_list_free(
			 &( ( *local_descriptors )->local_descriptor_nodes_list ),
			 NULL );
		}
		memory_free(
		 *local_descriptors );

		*local_descriptors = NULL;
	}
	return( -1 );
}

/* Frees the local descriptors and its values
 * Returns 1 if successful or -1 on error
 */
int libpff_local_descriptors_free(
     libpff_local_descriptors_t **local_descriptors,
     libcerror_error_t **error )
{
	static char *function = "libpff_local_descriptors_free";
	int result            = 1;

	if( local_descriptors == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid local descriptors.",
		 function );

		return( -1 );
	}
	if( *local_descriptors != NULL )
	{
		if( libfdata_list_free(
		     &( ( *local_descriptors )->local_descriptor_nodes_list ),
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
			 "%s: unable to free local descriptor nodes list.",
			 function );

			result = -1;
		}
		if( libfcache_cache_free(
		     &( ( *local_descriptors )->local_descriptor_nodes_cache ),
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
			 "%s: unable to free local descriptor nodes cache.",
			 function );

			result = -1;
		}
		memory_free(
		 *local_descriptors );

		*local_descriptors = NULL;
	}
	return( result );
}

/* Clones the local descriptors
 * Returns 1 if successful or -1 on error
 */
int libpff_local_descriptors_clone(
     libpff_local_descriptors_t **destination_local_descriptors,
     libpff_local_descriptors_t *source_local_descriptors,
     libcerror_error_t **error )
{
	static char *function = "libpff_local_descriptors_clone";

	if( destination_local_descriptors == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid destination local descriptors.",
		 function );

		return( -1 );
	}
	if( *destination_local_descriptors != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid destination local descriptors already set.",
		 function );

		return( -1 );
	}
	if( source_local_descriptors == NULL )
	{
		*destination_local_descriptors = NULL;

		return( 1 );
	}
	if( libpff_local_descriptors_initialize(
	     destination_local_descriptors,
	     source_local_descriptors->io_handle,
	     source_local_descriptors->root_node_offset,
	     source_local_descriptors->root_node_size,
	     source_local_descriptors->recovered,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create destination local descriptors.",
		 function );

		return( -1 );
	}
	return( 1 );
}

/* Reads the local descriptor node
 * Returns 1 if successful or -1 on error
 */
int libpff_local_descriptors_read_node(
     libpff_local_descriptors_t *local_descriptors,
     libbfio_handle_t *file_io_handle,
     off64_t node_offset,
     libfdata_tree_node_t *local_descriptors_tree_node,
     libpff_local_descriptor_value_t *local_descriptor_value,
     libcerror_error_t **error )
{
	libfdata_list_element_t *local_descriptor_node_list_element = NULL;
	libpff_index_value_t *offset_index_value                    = NULL;
	libpff_local_descriptor_node_t *local_descriptor_node       = NULL;
	uint8_t *node_entry_data                                    = NULL;
	static char *function                                       = "libpff_local_descriptors_read_node";
	off64_t node_data_offset                                    = 0;
	uint64_t local_descriptor_sub_node_identifier               = 0;
	uint16_t entry_index                                        = 0;
	int element_index                                           = 0;
	int local_descriptor_node_list_element_index                = 0;

	if( local_descriptors == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid local_descriptors.",
		 function );

		return( -1 );
	}
	if( local_descriptors->io_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid local_descriptors - missing IO handle.",
		 function );

		return( -1 );
	}
	if( local_descriptor_value == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid local descriptor value.",
		 function );

		return( -1 );
	}
	if( libfdata_list_get_element_by_offset(
	     local_descriptors->local_descriptor_nodes_list,
	     node_offset,
	     &local_descriptor_node_list_element_index,
	     &local_descriptor_node_list_element,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve local descriptor node list element at offset: %" PRIi64 ".",
		 function,
		 node_offset );

		return( -1 );
	}
	if( libfdata_list_element_get_element_value(
	     local_descriptor_node_list_element,
	     file_io_handle,
	     local_descriptors->local_descriptor_nodes_cache,
	     (intptr_t **) &local_descriptor_node,
	     0,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve local descriptor node at offset: %" PRIi64 ".",
		 function,
		 node_offset );

		return( -1 );
	}
	if( local_descriptor_node == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: missing local descriptor node.",
		 function );

		return( -1 );
	}
	if( local_descriptor_node->number_of_entries > 0 )
	{
		if( libfdata_tree_node_resize_sub_nodes(
		     local_descriptors_tree_node,
		     (int) local_descriptor_node->number_of_entries,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_RESIZE_FAILED,
			 "%s: unable to resize number of sub nodes.",
			 function );

			return( -1 );
		}
		node_data_offset = node_offset;

		for( entry_index = 0;
		     entry_index < local_descriptor_node->number_of_entries;
		     entry_index++ )
		{
			if( local_descriptor_node->level != PFF_LOCAL_DESCRIPTOR_NODE_LEVEL_LEAF )
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
				if( local_descriptors->io_handle->file_type == LIBPFF_FILE_TYPE_32BIT )
				{
					byte_stream_copy_to_uint32_little_endian(
					 ( (pff_local_descriptor_branch_node_entry_type_32bit_t *) node_entry_data )->sub_node_identifier,
					 local_descriptor_sub_node_identifier );

					node_entry_data += 4;
				}
				else if( local_descriptors->io_handle->file_type == LIBPFF_FILE_TYPE_64BIT )
				{
					byte_stream_copy_to_uint64_little_endian(
					 ( (pff_local_descriptor_branch_node_entry_type_64bit_t *) node_entry_data )->sub_node_identifier,
					 local_descriptor_sub_node_identifier );

					node_entry_data += 8;
				}
				/* TODO handle multiple recovered offset index values */
				if( libpff_io_handle_get_offset_index_value_by_identifier(
				     local_descriptors->io_handle,
				     file_io_handle,
				     local_descriptor_sub_node_identifier,
				     local_descriptors->recovered,
				     0,
				     &offset_index_value,
				     error ) != 1 )
				{
					libcerror_error_set(
					 error,
					 LIBCERROR_ERROR_DOMAIN_RUNTIME,
					 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
					 "%s: unable to find sub node identifier: %" PRIu64 ".",
					 function,
					 local_descriptor_sub_node_identifier );

					return( -1 );
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
					 "%s: local descriptor entry: %03" PRIu16 " at level: %" PRIu8 " identifier: %" PRIu64 " (%s) at offset: %" PRIi64 " of size: %" PRIu32 "\n",
					 function,
					 entry_index,
					 local_descriptor_node->level,
					 offset_index_value->identifier,
					 ( ( offset_index_value->identifier & LIBPFF_OFFSET_INDEX_IDENTIFIER_FLAG_INTERNAL ) ? "internal" : "external" ),
					 offset_index_value->file_offset,
					 offset_index_value->data_size );
				}
#endif
				if( libfdata_list_append_element(
				     local_descriptors->local_descriptor_nodes_list,
				     &element_index,
				     offset_index_value->file_offset,
				     (off64_t) offset_index_value->data_size,
				     0,
				     error ) != 1 )
				{
					libcerror_error_set(
					 error,
					 LIBCERROR_ERROR_DOMAIN_RUNTIME,
					 LIBCERROR_RUNTIME_ERROR_APPEND_FAILED,
					 "%s: unable to append local descriptor nodes list element.",
					 function );

					return( -1 );
				}
			}
			if( libfdata_tree_node_set_sub_node_by_index(
			     local_descriptors_tree_node,
			     (int) entry_index,
			     node_data_offset,
			     (size64_t) entry_index,
			     0,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
				 "%s: unable to set local descriptor node entry: %" PRIu16 " as sub node.",
				 function,
				 entry_index );

				return( -1 );
			}
			node_data_offset += local_descriptor_node->entry_size;
		}
	}
	return( 1 );
}

/* Reads the local descriptor node entry
 * Returns 1 if successful or -1 on error
 */
int libpff_local_descriptors_read_node_entry(
     libpff_local_descriptors_t *local_descriptors,
     libbfio_handle_t *file_io_handle,
     off64_t node_offset,
     uint16_t entry_index,
     libfdata_tree_node_t *local_descriptors_tree_node,
     libpff_local_descriptor_value_t *local_descriptor_value,
     libcerror_error_t **error )
{
	libfdata_list_element_t *local_descriptor_node_list_element = NULL;
	libpff_index_value_t *offset_index_value                    = NULL;
	libpff_local_descriptor_node_t *local_descriptor_node       = NULL;
	uint8_t *node_entry_data                                    = NULL;
	static char *function                                       = "libpff_local_descriptors_read_node_entry";
	int local_descriptor_node_list_element_index                = 0;

	if( local_descriptors == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid local_descriptors.",
		 function );

		return( -1 );
	}
	if( local_descriptors->io_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid local_descriptors - missing IO handle.",
		 function );

		return( -1 );
	}
	if( ( local_descriptors->io_handle->file_type != LIBPFF_FILE_TYPE_32BIT )
	 && ( local_descriptors->io_handle->file_type != LIBPFF_FILE_TYPE_64BIT ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_UNSUPPORTED_VALUE,
		 "%s: unsupported file type.",
		 function );

		return( -1 );
	}
	if( local_descriptor_value == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid local descriptor value.",
		 function );

		return( -1 );
	}
	if( libfdata_list_get_element_by_offset(
	     local_descriptors->local_descriptor_nodes_list,
	     node_offset,
	     &local_descriptor_node_list_element_index,
	     &local_descriptor_node_list_element,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve local descriptor node list element at offset: %" PRIi64 ".",
		 function,
		 node_offset );

		return( -1 );
	}
	if( libfdata_list_element_get_element_value(
	     local_descriptor_node_list_element,
	     file_io_handle,
	     local_descriptors->local_descriptor_nodes_cache,
	     (intptr_t **) &local_descriptor_node,
	     0,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve local descriptor node at offset: %" PRIi64 ".",
		 function,
		 node_offset );

		return( -1 );
	}
	if( local_descriptor_node == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: missing local descriptor node.",
		 function );

		return( -1 );
	}
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
	if( local_descriptors->io_handle->file_type == LIBPFF_FILE_TYPE_32BIT )
	{
		byte_stream_copy_to_uint32_little_endian(
		 node_entry_data,
		 local_descriptor_value->identifier );

		node_entry_data += 4;
	}
	else if( local_descriptors->io_handle->file_type == LIBPFF_FILE_TYPE_64BIT )
	{
		byte_stream_copy_to_uint64_little_endian(
		 node_entry_data,
		 local_descriptor_value->identifier );

		node_entry_data += 8;
	}
	/* Ignore the upper 32-bit of local descriptor identifiers
	 */
	local_descriptor_value->identifier &= 0xffffffffUL;

	if( local_descriptor_node->level == PFF_LOCAL_DESCRIPTOR_NODE_LEVEL_LEAF )
	{
		if( local_descriptors->io_handle->file_type == LIBPFF_FILE_TYPE_32BIT )
		{
			byte_stream_copy_to_uint32_little_endian(
			 node_entry_data,
			 local_descriptor_value->data_identifier );

			node_entry_data += 4;

			byte_stream_copy_to_uint32_little_endian(
			 node_entry_data,
			 local_descriptor_value->local_descriptors_identifier );

			node_entry_data += 4;
		}
		else if( local_descriptors->io_handle->file_type == LIBPFF_FILE_TYPE_64BIT )
		{
			byte_stream_copy_to_uint64_little_endian(
			 node_entry_data,
			 local_descriptor_value->data_identifier );

			node_entry_data += 8;

			byte_stream_copy_to_uint64_little_endian(
			 node_entry_data,
			 local_descriptor_value->local_descriptors_identifier );

			node_entry_data += 8;
		}
	}
	else
	{
		if( local_descriptors->io_handle->file_type == LIBPFF_FILE_TYPE_32BIT )
		{
			byte_stream_copy_to_uint32_little_endian(
			 node_entry_data,
			 local_descriptor_value->sub_node_identifier );

			node_entry_data += 4;
		}
		else if( local_descriptors->io_handle->file_type == LIBPFF_FILE_TYPE_64BIT )
		{
			byte_stream_copy_to_uint64_little_endian(
			 node_entry_data,
			 local_descriptor_value->sub_node_identifier );

			node_entry_data += 8;
		}
	}
	if( local_descriptor_node->level == PFF_LOCAL_DESCRIPTOR_NODE_LEVEL_LEAF )
	{
		if( libfdata_tree_node_set_leaf(
		     local_descriptors_tree_node,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
			 "%s: unable to set leaf in local descriptors tree node.",
			 function );

			return( -1 );
		}
	}
	else
	{
		/* TODO handle multiple recovered offset index values */
		if( libpff_io_handle_get_offset_index_value_by_identifier(
		     local_descriptors->io_handle,
		     file_io_handle,
		     local_descriptor_value->sub_node_identifier,
		     local_descriptors->recovered,
		     0,
		     &offset_index_value,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to find sub node identifier: %" PRIu64 ".",
			 function,
			 local_descriptor_value->sub_node_identifier );

			return( -1 );
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
			 "%s: local descriptor entry: %03" PRIu16 " at level: %" PRIu8 " identifier: %" PRIu64 " (%s) at offset: %" PRIi64 " of size: %" PRIu32 "\n",
			 function,
			 entry_index,
			 local_descriptor_node->level,
			 offset_index_value->identifier,
			 ( ( offset_index_value->identifier & LIBPFF_OFFSET_INDEX_IDENTIFIER_FLAG_INTERNAL ) ? "internal" : "external" ),
			 offset_index_value->file_offset,
			 offset_index_value->data_size );
		}
#endif
		if( libfdata_tree_node_set_sub_nodes_range(
		     local_descriptors_tree_node,
		     offset_index_value->file_offset,
		     (size64_t) entry_index,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
			 "%s: unable to set sub nodes range.",
			 function );

			return( -1 );
		}
	}
	return( 1 );
}

/* Reads the local descriptor value
 * Returns 1 if successful or -1 on error
 */
int libpff_local_descriptors_read_node_data(
     intptr_t *io_handle,
     libbfio_handle_t *file_io_handle,
     libfdata_tree_node_t *node,
     libfcache_cache_t *cache,
     off64_t node_data_offset,
     size64_t node_data_size,
     uint8_t read_flags LIBPFF_ATTRIBUTE_UNUSED,
     libcerror_error_t **error )
{
	libpff_local_descriptors_t *local_descriptors           = NULL;
	libpff_local_descriptor_value_t *local_descriptor_value = NULL;
	static char *function                                   = "libpff_local_descriptors_read_node_data";
	int result                                              = 0;

	LIBPFF_UNREFERENCED_PARAMETER( read_flags )

	if( io_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid local_descriptors IO handle.",
		 function );

		return( -1 );
	}
	local_descriptors = (libpff_local_descriptors_t *) io_handle;

	if( node_data_offset < 0 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_LESS_THAN_ZERO,
		 "%s: invalid node data offset value less than zero.",
		 function );

		return( -1 );
	}
	if( node_data_size > (off64_t) UINT16_MAX )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_EXCEEDS_MAXIMUM,
		 "%s: invalid node data size value exceeds maximum.",
		 function );

		return( -1 );
	}
	if( libpff_local_descriptor_value_initialize(
	     &local_descriptor_value,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create local descriptor value.",
		 function );

		return( -1 );
	}
	result = libfdata_tree_node_is_root(
	          node,
	          error );

	if( result == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to determine if node is the root.",
		 function );

		libpff_local_descriptor_value_free(
		 &local_descriptor_value,
		 NULL );

		return( -1 );
	}
	else if( result != 0 )
	{
		/* The local descriptors tree root node is virtual
		 */
		if( libfdata_tree_node_set_sub_nodes_range(
		     node,
		     local_descriptors->root_node_offset,
		     0,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
			 "%s: unable to set root node as sub nodes range.",
			 function );

			libpff_local_descriptor_value_free(
			 &local_descriptor_value,
			 NULL );

			return( -1 );
		}
	}
	else
	{
		/* node_data_size contains the local descriptor node entry
		 */
		if( libpff_local_descriptors_read_node_entry(
		     local_descriptors,
		     file_io_handle,
		     node_data_offset,
		     (uint16_t) node_data_size,
		     node,
		     local_descriptor_value,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_IO,
			 LIBCERROR_IO_ERROR_READ_FAILED,
			 "%s: unable to read local descriptor node entry at offset: %" PRIi64 ".",
			 function,
			 node_data_offset );

			libpff_local_descriptor_value_free(
			 &local_descriptor_value,
			 NULL );

			return( -1 );
		}
	}
	if( libfdata_tree_node_set_node_value(
	     node,
	     cache,
	     (intptr_t *) local_descriptor_value,
	     (int (*)(intptr_t **, libcerror_error_t **)) &libpff_local_descriptor_value_free,
	     LIBFDATA_TREE_NODE_VALUE_FLAG_MANAGED,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to set local descriptor value as node value.",
		 function );

		libpff_local_descriptor_value_free(
		 &local_descriptor_value,
		 NULL );

		return( -1 );
	}
	return( 1 );
}

/* Reads the local_descriptors sub nodes
 * Returns 1 if successful or -1 on error
 */
int libpff_local_descriptors_read_sub_nodes(
     intptr_t *io_handle,
     libbfio_handle_t *file_io_handle,
     libfdata_tree_node_t *node,
     libfcache_cache_t *cache,
     off64_t node_data_offset,
     size64_t node_data_size LIBPFF_ATTRIBUTE_UNUSED,
     uint8_t read_flags LIBPFF_ATTRIBUTE_UNUSED,
     libcerror_error_t **error )
{
	libpff_local_descriptors_t *local_descriptors           = NULL;
	libpff_local_descriptor_value_t *local_descriptor_value = NULL;
	static char *function                                   = "libpff_local_descriptors_read_sub_nodes";

	LIBPFF_UNREFERENCED_PARAMETER( node_data_size )
	LIBPFF_UNREFERENCED_PARAMETER( read_flags )

	if( io_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid local_descriptors IO handle.",
		 function );

		return( -1 );
	}
	local_descriptors = (libpff_local_descriptors_t *) io_handle;

	if( node_data_offset < 0 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_LESS_THAN_ZERO,
		 "%s: invalid node data offset value less than zero.",
		 function );

		return( -1 );
	}
	if( libfdata_tree_node_get_node_value(
	     node,
	     file_io_handle,
	     cache,
	     (intptr_t **) &local_descriptor_value,
	     0,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve node value.",
		 function );

		return( -1 );
	}
	if( local_descriptor_value == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: missing local descriptor value.",
		 function );

		return( -1 );
	}
	if( libpff_local_descriptors_read_node(
	     local_descriptors,
	     file_io_handle,
	     node_data_offset,
	     node,
	     local_descriptor_value,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_READ_FAILED,
		 "%s: unable to read local descriptor node at offset: %" PRIi64 ".",
		 function,
		 node_data_offset );

		return( -1 );
	}
	return( 1 );
}

