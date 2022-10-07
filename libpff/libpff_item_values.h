/*
 * Item values functions
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

#if !defined( _LIBPFF_ITEM_VALUES_H )
#define _LIBPFF_ITEM_VALUES_H

#include <common.h>
#include <types.h>

#include "libpff_io_handle.h"
#include "libpff_libbfio.h"
#include "libpff_libcerror.h"
#include "libpff_libfcache.h"
#include "libpff_libfdata.h"
#include "libpff_list_type.h"
#include "libpff_local_descriptor_value.h"
#include "libpff_table.h"
#include "libpff_tree_type.h"
#include "libpff_types.h"

#if defined( __cplusplus )
extern "C" {
#endif

typedef struct libpff_item_values libpff_item_values_t;

struct libpff_item_values
{
	/* The descriptor identifier value
	 */
	uint32_t descriptor_identifier;

	/* The data identifier value
	 */
	uint64_t data_identifier;

	/* The local descriptors identifier value
	 */
	uint64_t local_descriptors_identifier;

	/* Value to indicate if the item was recovered
	 */
	uint8_t recovered;

	/* The value index of the recovered data identifier
	 */
	int recovered_data_identifier_value_index;

	/* The value index of the recovered local descriptors identifier
	 */
	int recovered_local_descriptors_identifier_value_index;

	/* The item table
	 */
	libpff_table_t *table;
};

int libpff_item_values_initialize(
     libpff_item_values_t **item_values,
     uint32_t descriptor_identifier,
     uint64_t data_identifier,
     uint64_t local_descriptors_identifier,
     uint8_t recovered,
     libcerror_error_t **error );

int libpff_item_values_free(
     libpff_item_values_t **item_values,
     libcerror_error_t **error );

int libpff_item_values_clone_copy(
     libpff_item_values_t *destination_item_values,
     libpff_item_values_t *source_item_values,
     libcerror_error_t **error );

int libpff_item_values_read(
     libpff_item_values_t *item_values,
     libpff_list_t *name_to_id_map_list,
     libpff_io_handle_t *io_handle,
     libbfio_handle_t *file_io_handle,
     int debug_item_type,
     libcerror_error_t **error );

int libpff_item_values_get_local_descriptors_value_by_identifier(
     libpff_item_values_t *item_values,
     libbfio_handle_t *file_io_handle,
     uint32_t descriptor_identifier,
     libpff_local_descriptor_value_t **local_descriptor_value,
     libcerror_error_t **error );

int libpff_item_values_read_local_descriptor_data(
     libpff_item_values_t *item_values,
     libpff_io_handle_t *io_handle,
     libbfio_handle_t *file_io_handle,
     uint32_t descriptor_identifier,
     libfdata_reference_t **descriptor_data_reference,
     libfcache_cache_t **descriptor_data_cache,
     libcerror_error_t **error );

int libpff_item_values_get_number_of_sets(
     libpff_item_values_t *item_values,
     libpff_list_t *name_to_id_map_list,
     libpff_io_handle_t *io_handle,
     libbfio_handle_t *file_io_handle,
     uint32_t *number_of_sets,
     libcerror_error_t **error );

int libpff_item_values_get_entry_value(
     libpff_item_values_t *item_values,
     libpff_list_t *name_to_id_map_list,
     libpff_io_handle_t *io_handle,
     libbfio_handle_t *file_io_handle,
     int table_set_index,
     uint32_t entry_type,
     uint32_t *value_type,
     libfdata_reference_t **value_data_reference,
     libfcache_cache_t **value_data_cache,
     uint8_t flags,
     libcerror_error_t **error );

#if defined( __cplusplus )
}
#endif

#endif

