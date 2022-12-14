/*
 * Item tree functions
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

#if !defined( _LIBPFF_ITEM_TREE_H )
#define _LIBPFF_ITEM_TREE_H

#include <common.h>
#include <types.h>

#include "libpff_io_handle.h"
#include "libpff_libbfio.h"
#include "libpff_libcerror.h"
#include "libpff_libfcache.h"
#include "libpff_libfdata.h"
#include "libpff_list_type.h"
#include "libpff_tree_type.h"

#if defined( __cplusplus )
extern "C" {
#endif

int libpff_item_tree_node_free_recovered(
     libpff_tree_node_t **item_tree_node,
     libcerror_error_t **error );

int libpff_item_tree_get_tree_node_by_identifier(
     libpff_tree_node_t *item_tree_node,
     uint32_t item_identifier,
     libpff_tree_node_t **result_item_tree_node,
     libcerror_error_t **error );

int libpff_item_tree_get_sub_node(
     libpff_tree_node_t *item_tree_node,
     int sub_node_index,
     libpff_tree_node_t **sub_node,
     libcerror_error_t **error );

int libpff_item_tree_get_sub_node_by_identifier(
     libpff_tree_node_t *item_tree_node,
     uint32_t sub_node_identifier,
     libpff_tree_node_t **sub_node,
     libcerror_error_t **error );

int libpff_item_tree_create_node(
     libpff_tree_node_t *item_tree_root_node,
     libbfio_handle_t *file_io_handle,
     libfdata_tree_t *descriptor_index_tree,
     libfdata_tree_node_t *descriptor_index_tree_node,
     libfcache_cache_t *index_tree_cache,
     libpff_list_t *orphan_node_list,
     libpff_tree_node_t **root_folder_item_tree_node,
     libcerror_error_t **error );

int libpff_item_tree_get_identifier(
     libpff_tree_node_t *item_tree_node,
     uint32_t *identifier,
     libcerror_error_t **error );

int libpff_item_tree_append_identifier(
     libpff_tree_node_t *item_tree_node,
     uint32_t descriptor_identifier,
     uint64_t data_identifier,
     uint64_t local_descriptors_identifier,
     uint8_t recovered,
     libcerror_error_t **error );

#if defined( __cplusplus )
}
#endif

#endif

