/*
 * Item functions
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

#if !defined( _LIBPFF_INTERNAL_ITEM_H )
#define _LIBPFF_INTERNAL_ITEM_H

#include <common.h>
#include <types.h>

#include "libpff_extern.h"
#include "libpff_file.h"
#include "libpff_item_descriptor.h"
#include "libpff_item_values.h"
#include "libpff_libbfio.h"
#include "libpff_libcerror.h"
#include "libpff_libfcache.h"
#include "libpff_libfdata.h"
#include "libpff_multi_value.h"
#include "libpff_tree_type.h"
#include "libpff_types.h"

#if defined( __cplusplus )
extern "C" {
#endif

#if defined( HAVE_DEBUG_OUTPUT )
#define LIBPFF_ITEM_FLAGS_DEFAULT			LIBPFF_ITEM_FLAG_MANAGED_FILE_IO_HANDLE
#else
#define LIBPFF_ITEM_FLAGS_DEFAULT			LIBPFF_ITEM_FLAG_NON_MANAGED_FILE_IO_HANDLE
#endif

#define LIBPFF_ITEM_NUMBER_OF_SUB_ITEMS			4

typedef struct libpff_internal_item libpff_internal_item_t;

struct libpff_internal_item
{
	/* The file IO handle
	 */
	libbfio_handle_t *file_io_handle;

	/* The internal file
	 */
	libpff_internal_file_t *internal_file;

	/* The item tree node
	 */
	libpff_tree_node_t *item_tree_node;

	/* The item type
	 */
	uint8_t type;

	/* The item flags
	 */
	uint8_t flags;

	/* The item descriptor
	 */
        libpff_item_descriptor_t *item_descriptor;

	/* The item values
	 */
        libpff_item_values_t *item_values;

	/* Sub item tree nodes
	 */
	libpff_tree_node_t *sub_item_tree_node[ LIBPFF_ITEM_NUMBER_OF_SUB_ITEMS ];

	/* Sub item values
	 */
	libpff_item_values_t *sub_item_values[ LIBPFF_ITEM_NUMBER_OF_SUB_ITEMS ];

	/* Embedded object data reference
	 */
	libfdata_reference_t *embedded_object_data_reference;

	/* Embedded object data cache
	 */
	libfcache_cache_t *embedded_object_data_cache;
};

int libpff_item_initialize(
     libpff_item_t **item,
     libbfio_handle_t *file_io_handle,
     libpff_internal_file_t *internal_file,
     libpff_tree_node_t *item_tree_node,
     libpff_item_descriptor_t *item_descriptor,
     uint8_t flags,
     libcerror_error_t **error );

LIBPFF_EXTERN \
int libpff_item_free(
     libpff_item_t **item,
     libcerror_error_t **error );

LIBPFF_EXTERN \
int libpff_item_clone(
     libpff_item_t **destination_item,
     libpff_item_t *source_item,
     libcerror_error_t **error );

int libpff_item_determine_type(
     libpff_internal_item_t *internal_item,
     libcerror_error_t **error );

LIBPFF_EXTERN \
int libpff_item_get_identifier(
     libpff_item_t *item,
     uint32_t *identifier,
     libcerror_error_t **error );

LIBPFF_EXTERN \
int libpff_item_get_number_of_sets(
     libpff_item_t *item,
     uint32_t *number_of_sets,
     libcerror_error_t **error );

LIBPFF_EXTERN \
int libpff_item_get_number_of_entries(
     libpff_item_t *item,
     uint32_t *number_of_entries,
     libcerror_error_t **error );

LIBPFF_EXTERN \
int libpff_item_get_entry_type(
     libpff_item_t *item,
     int set_index,
     int entry_index,
     uint32_t *entry_type,
     uint32_t *value_type,
     libpff_name_to_id_map_entry_t **name_to_id_map_entry,
     libcerror_error_t **error );

LIBPFF_EXTERN \
int libpff_item_get_value_type(
     libpff_item_t *item,
     int set_index,
     uint32_t entry_type,
     uint32_t *value_type,
     uint8_t flags,
     libcerror_error_t **error );

LIBPFF_EXTERN \
int libpff_item_get_entry_value(
     libpff_item_t *item,
     int set_index,
     uint32_t entry_type,
     uint32_t *value_type,
     uint8_t **value_data,
     size_t *value_data_size,
     uint8_t flags,
     libcerror_error_t **error );

LIBPFF_EXTERN \
int libpff_item_get_entry_value_by_utf8_name(
     libpff_item_t *item,
     int set_index,
     uint8_t *utf8_entry_name,
     size_t utf8_entry_name_length,
     uint32_t *value_type,
     uint8_t **value_data,
     size_t *value_data_size,
     libcerror_error_t **error );

LIBPFF_EXTERN \
int libpff_item_get_entry_value_by_utf16_name(
     libpff_item_t *item,
     int set_index,
     uint16_t *utf16_entry_name,
     size_t utf16_entry_name_length,
     uint32_t *value_type,
     uint8_t **value_data,
     size_t *value_data_size,
     libcerror_error_t **error );

LIBPFF_EXTERN \
int libpff_item_get_entry_value_boolean(
     libpff_item_t *item,
     int set_index,
     uint32_t entry_type,
     uint8_t *entry_value,
     uint8_t flags,
     libcerror_error_t **error );

LIBPFF_EXTERN \
int libpff_item_get_entry_value_16bit(
     libpff_item_t *item,
     int set_index,
     uint32_t entry_type,
     uint16_t *entry_value,
     uint8_t flags,
     libcerror_error_t **error );

LIBPFF_EXTERN \
int libpff_item_get_entry_value_32bit(
     libpff_item_t *item,
     int set_index,
     uint32_t entry_type,
     uint32_t *entry_value,
     uint8_t flags,
     libcerror_error_t **error );

LIBPFF_EXTERN \
int libpff_item_get_entry_value_64bit(
     libpff_item_t *item,
     int set_index,
     uint32_t entry_type,
     uint64_t *entry_value,
     uint8_t flags,
     libcerror_error_t **error );

LIBPFF_EXTERN \
int libpff_item_get_entry_value_filetime(
     libpff_item_t *item,
     int set_index,
     uint32_t entry_type,
     uint64_t *entry_value,
     uint8_t flags,
     libcerror_error_t **error );

LIBPFF_EXTERN \
int libpff_item_get_entry_value_size(
     libpff_item_t *item,
     int set_index,
     uint32_t entry_type,
     size_t *entry_value,
     uint8_t flags,
     libcerror_error_t **error );

LIBPFF_EXTERN \
int libpff_item_get_entry_value_floating_point(
     libpff_item_t *item,
     int set_index,
     uint32_t entry_type,
     double *entry_value,
     uint8_t flags,
     libcerror_error_t **error );

LIBPFF_EXTERN \
int libpff_item_get_entry_value_utf8_string_size(
     libpff_item_t *item,
     int set_index,
     uint32_t entry_type,
     size_t *utf8_string_size,
     uint8_t flags,
     libcerror_error_t **error );

LIBPFF_EXTERN \
int libpff_item_get_entry_value_utf8_string(
     libpff_item_t *item,
     int set_index,
     uint32_t entry_type,
     uint8_t *utf8_string,
     size_t utf8_string_size,
     uint8_t flags,
     libcerror_error_t **error );

LIBPFF_EXTERN \
int libpff_item_get_entry_value_utf16_string_size(
     libpff_item_t *item,
     int set_index,
     uint32_t entry_type,
     size_t *utf16_string_size,
     uint8_t flags,
     libcerror_error_t **error );

LIBPFF_EXTERN \
int libpff_item_get_entry_value_utf16_string(
     libpff_item_t *item,
     int set_index,
     uint32_t entry_type,
     uint16_t *utf16_string,
     size_t utf16_string_size,
     uint8_t flags,
     libcerror_error_t **error );

LIBPFF_EXTERN \
int libpff_item_get_entry_value_binary_data_size(
     libpff_item_t *item,
     int set_index,
     uint32_t entry_type,
     size_t *size,
     uint8_t flags,
     libcerror_error_t **error );

LIBPFF_EXTERN \
int libpff_item_get_entry_value_binary_data(
     libpff_item_t *item,
     int set_index,
     uint32_t entry_type,
     uint8_t *binary_data,
     size_t size,
     uint8_t flags,
     libcerror_error_t **error );

LIBPFF_EXTERN \
int libpff_item_get_entry_value_guid(
     libpff_item_t *item,
     int set_index,
     uint32_t entry_type,
     uint8_t *guid,
     size_t size,
     uint8_t flags,
     libcerror_error_t **error );

LIBPFF_EXTERN \
int libpff_item_get_entry_multi_value(
     libpff_item_t *item,
     int set_index,
     uint32_t entry_type,
     libpff_multi_value_t **multi_value,
     uint8_t flags,
     libcerror_error_t **error );

LIBPFF_EXTERN \
int libpff_item_get_type(
     libpff_item_t *item,
     uint8_t *item_type,
     libcerror_error_t **error );

LIBPFF_EXTERN \
int libpff_item_get_number_of_sub_items(
     libpff_item_t *item,
     int *number_of_sub_items,
     libcerror_error_t **error );

LIBPFF_EXTERN \
int libpff_item_get_sub_item(
     libpff_item_t *item,
     int sub_item_index,
     libpff_item_t **sub_item,
     libcerror_error_t **error );

LIBPFF_EXTERN \
int libpff_item_get_sub_item_by_identifier(
     libpff_item_t *item,
     uint32_t sub_item_identifier,
     libpff_item_t **sub_item,
     libcerror_error_t **error );

#if defined( __cplusplus )
}
#endif

#endif

