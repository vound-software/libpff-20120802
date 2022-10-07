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

#if !defined( _LIBPFF_IO_HANDLE_H )
#define _LIBPFF_IO_HANDLE_H

#include <common.h>
#include <types.h>

#include "libpff_index_value.h"
#include "libpff_libbfio.h"
#include "libpff_libcerror.h"
#include "libpff_libfcache.h"
#include "libpff_libfdata.h"
#include "libpff_list_type.h"
#include "libpff_offset_list.h"
#include "libpff_tree_type.h"

#if defined( __cplusplus )
extern "C" {
#endif

extern const uint8_t pff_file_signature[ 4 ];

typedef struct libpff_io_handle libpff_io_handle_t;

struct libpff_io_handle
{
	/* The index nodes vector
	 */
	libfdata_vector_t *index_nodes_vector;

	/* The index nodes cache
	 */
	libfcache_cache_t *index_nodes_cache;

	/* The descriptor index (data) tree
	 */
	libfdata_tree_t *descriptor_index_tree;

	/* The offset index (data) tree
	 */
	libfdata_tree_t *offset_index_tree;

	/* The recovered descriptor index (data) tree
	 */
	libfdata_tree_t *recovered_descriptor_index_tree;

	/* The recovered offset index (data) tree
	 */
	libfdata_tree_t *recovered_offset_index_tree;

	/* The descriptor index tree (value) cache
	 */
	libfcache_cache_t *descriptor_index_tree_cache;

	/* The offset index tree (value) cache
	 */
	libfcache_cache_t *offset_index_tree_cache;

	/* The file size
	 */
	size64_t file_size;

	/* The descriptor index root node offset
	 */
	off64_t descriptor_index_root_node_offset;

	/* The offset index root node offset
	 */
	off64_t offset_index_root_node_offset;

	/* The encryption type
	 */
	uint8_t encryption_type;

	/* Value to indicate decryption should be forced
	 */
	uint8_t force_decryption;

	/* The file type
	 */
	uint8_t file_type;

	/* The codepage of the extended ASCII strings
	 */
	int ascii_codepage;

	/* Value to indicate if abort was signalled
	 */
	int abort;
};

int libpff_io_handle_initialize(
     libpff_io_handle_t **io_handle,
     libcerror_error_t **error );

int libpff_io_handle_free(
     libpff_io_handle_t **io_handle,
     libcerror_error_t **error );

int libpff_io_handle_read_file_header(
     libpff_io_handle_t *io_handle,
     libbfio_handle_t *file_io_handle,
     int *file_content_type,
     libcerror_error_t **error );

int libpff_io_handle_read_unallocated_data_blocks(
     libpff_io_handle_t *io_handle,
     libbfio_handle_t *file_io_handle,
     libpff_offset_list_t *unallocated_data_block_list,
     libcerror_error_t **error );

int libpff_io_handle_read_unallocated_page_blocks(
     libpff_io_handle_t *io_handle,
     libbfio_handle_t *file_io_handle,
     libpff_offset_list_t *unallocated_page_block_list,
     libcerror_error_t **error );

int libpff_io_handle_create_item_tree(
     libpff_io_handle_t *io_handle,
     libbfio_handle_t *file_io_handle,
     libpff_tree_node_t **item_tree_root_node,
     libpff_list_t *orphan_node_list,
     libpff_tree_node_t **root_folder_item_tree_node,
     libcerror_error_t **error );

int libpff_io_handle_read_index_node(
     intptr_t *io_handle,
     libbfio_handle_t *file_io_handle,
     libfdata_vector_t *vector,
     libfcache_cache_t *cache,
     int element_index,
     off64_t element_data_offset,
     size64_t element_data_size,
     uint8_t read_flags,
     libcerror_error_t **error );

int libpff_io_handle_read_local_descriptor_node(
     intptr_t *io_handle,
     libbfio_handle_t *file_io_handle,
     libfdata_list_element_t *list_element,
     libfcache_cache_t *cache,
     off64_t element_data_offset,
     size64_t element_data_size,
     uint32_t element_data_flags,
     uint8_t read_flags,
     libcerror_error_t **error );

int libpff_io_handle_get_descriptor_index_value_by_identifier(
     libpff_io_handle_t *io_handle,
     libbfio_handle_t *file_io_handle,
     uint32_t descriptor_identifier,
     uint8_t recovered,
     libpff_index_value_t **descriptor_index_value,
     libcerror_error_t **error );

int libpff_io_handle_get_offset_index_value_by_identifier(
     libpff_io_handle_t *io_handle,
     libbfio_handle_t *file_io_handle,
     uint64_t data_identifier,
     uint8_t recovered,
     int recovered_value_index,
     libpff_index_value_t **offset_index_value,
     libcerror_error_t **error );

int libpff_io_handle_read_descriptor_local_descriptors_tree(
     libpff_io_handle_t *io_handle,
     libbfio_handle_t *file_io_handle,
     uint64_t local_descriptors_identifier,
     uint8_t recovered,
     int recovered_value_index,
     libfdata_tree_t **local_descriptors_tree,
     libcerror_error_t **error );

int libpff_io_handle_read_descriptor_data_block(
     libpff_io_handle_t *io_handle,
     libbfio_handle_t *file_io_handle,
     uint32_t descriptor_identifier,
     uint64_t data_identifier,
     uint8_t recovered,
     int recovered_value_index,
     libfdata_block_t **descriptor_data_block,
     libfcache_cache_t **descriptor_data_cache,
     libcerror_error_t **error );

int libpff_io_handle_read_descriptor_data_reference(
     libpff_io_handle_t *io_handle,
     libbfio_handle_t *file_io_handle,
     uint32_t descriptor_identifier,
     uint64_t data_identifier,
     uint8_t recovered,
     int recovered_value_index,
     libfdata_reference_t **descriptor_data_reference,
     libfcache_cache_t **descriptor_data_cache,
     libcerror_error_t **error );

int libpff_io_handle_recover_items(
     libpff_io_handle_t *io_handle,
     libbfio_handle_t *file_io_handle,
     libpff_offset_list_t *unallocated_data_block_list,
     libpff_offset_list_t *unallocated_page_block_list,
     libpff_list_t *recovered_item_list,
     uint8_t recovery_flags,
     libcerror_error_t **error );

int libpff_io_handle_recover_index_nodes(
     libpff_io_handle_t *io_handle,
     libbfio_handle_t *file_io_handle,
     libcerror_error_t **error );

int libpff_io_handle_recover_data_blocks(
     libpff_io_handle_t *io_handle,
     libbfio_handle_t *file_io_handle,
     libpff_offset_list_t *unallocated_data_block_list,
     libpff_offset_list_t *unallocated_page_block_list,
     uint8_t recovery_flags,
     libcerror_error_t **error );

int libpff_io_handle_recover_index_values(
     libpff_io_handle_t *io_handle,
     libbfio_handle_t *file_io_handle,
     libpff_offset_list_t *unallocated_data_block_list,
     size64_t node_offset,
     uint8_t recovery_flags,
     libcerror_error_t **error );

int libpff_io_handle_recover_local_descriptors(
     libpff_io_handle_t *io_handle,
     libbfio_handle_t *file_io_handle,
     uint64_t local_descriptors_identifier,
     libcerror_error_t **error );

#if defined( __cplusplus )
}
#endif

#endif

