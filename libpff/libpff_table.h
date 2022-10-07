/*
 * Table functions
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

#if !defined( _LIBPFF_TABLE_H )
#define _LIBPFF_TABLE_H

#include <common.h>
#include <types.h>

#include "libpff_array_type.h"
#include "libpff_io_handle.h"
#include "libpff_libbfio.h"
#include "libpff_libcerror.h"
#include "libpff_libfcache.h"
#include "libpff_libfdata.h"
#include "libpff_list_type.h"
#include "libpff_local_descriptor_value.h"
#include "libpff_name_to_id_map.h"
#include "libpff_record_entry.h"
#include "libpff_types.h"

#if defined( __cplusplus )
extern "C" {
#endif

enum LIBPFF_TABLE_FLAGS
{
	/* The data of one or more recrord entries could not be read
	 */
	LIBPFF_TABLE_FLAG_MISSING_RECORD_ENTRY_DATA		= 0x01,
};

typedef struct libpff_table_index_value libpff_table_index_value_t;

struct libpff_table_index_value
{
	/* The table data array entry of the value
	 */
	uint32_t array_entry;

	/* The offset of the value
	 */
	uint16_t offset;

	/* The size of the value
	 */
	uint16_t size;
};

typedef struct libpff_table_index_values libpff_table_index_values_t;

struct libpff_table_index_values
{
	/* The number of values
	 */
	uint16_t number_of_values;

	/* The table index values
	 */
	libpff_table_index_value_t *value;
};

typedef struct libpff_table libpff_table_t;

struct libpff_table
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

	/* Value to indicate if the table was recovered
	 */
	uint8_t recovered;

	/* The value index of the recovered data identifier
	 */
	int recovered_data_identifier_value_index;

	/* The value index of the recovered local descriptors identifier
	 */
	int recovered_local_descriptors_identifier_value_index;

	/* The descriptor data block
	 */
	libfdata_block_t *descriptor_data_block;

	/* The descriptor data cache
	 */
	libfcache_cache_t *descriptor_data_cache;

	/* The local descriptors tree
	 */
	libfdata_tree_t *local_descriptors_tree;

	/* The local descriptors cache
	 */
	libfcache_cache_t *local_descriptors_cache;

	/* The index (array)
	 */
	libpff_array_t *index;

	/* The record entries data references (array)
	 */
	libpff_array_t *record_entries_data_references;

/* TODO refactoring */

	/* The record entries
	 */
	libpff_record_entry_t **entry;

	/* The number of records
	 */
	int number_of_sets;

	/* The number of record entries
	 */
	int number_of_entries;

	/* The flags
	 */
	uint8_t flags;
};

typedef struct libpff_table_values_array_entry libpff_table_values_array_entry_t;

struct libpff_table_values_array_entry
{
	/* The identifier
	 */
	uint32_t identifier;

	/* The entry number
	 */
	uint32_t entry_number;
};

int libpff_table_index_values_initialize(
     libpff_table_index_values_t **table_index_values,
     uint16_t number_of_values,
     libcerror_error_t **error );

int libpff_table_index_values_free(
     libpff_table_index_values_t **table_index_values,
     libcerror_error_t **error );

int libpff_table_entries_resize(
     libpff_table_t *table,
     int number_of_sets,
     int number_of_entries,
     libcerror_error_t **error );

int libpff_table_entries_clone(
     libpff_table_t *destination,
     libpff_table_t *source,
     libcerror_error_t **error );

int libpff_table_initialize(
     libpff_table_t **table,
     uint32_t descriptor_identifier,
     uint64_t data_identifier,
     uint64_t local_descriptors_identifier,
     uint8_t recovered,
     libcerror_error_t **error );

int libpff_table_free(
     libpff_table_t **table,
     libcerror_error_t **error );

int libpff_table_clone(
     libpff_table_t **destination_table,
     libpff_table_t *source_table,
     libcerror_error_t **error );

int libpff_table_get_local_descriptors_value_by_identifier(
     libpff_table_t *table,
     libbfio_handle_t *file_io_handle,
     uint32_t descriptor_identifier,
     libpff_local_descriptor_value_t **local_descriptor_value,
     libcerror_error_t **error );

int libpff_table_get_index_value_by_reference(
     libpff_table_t *table,
     uint32_t table_index_reference,
     libpff_table_index_value_t **table_index_value,
     libcerror_error_t **error );

int libpff_table_get_number_of_sets(
     libpff_table_t *table,
     int *number_of_sets,
     libcerror_error_t **error );

int libpff_table_get_number_of_entries(
     libpff_table_t *table,
     int *number_of_entries,
     libcerror_error_t **error );

int libpff_table_get_entry_type_by_index(
     libpff_table_t *table,
     int set_index,
     int entry_index,
     uint32_t *entry_type,
     uint32_t *value_type,
     libpff_name_to_id_map_entry_t **name_to_id_map_entry,
     libcerror_error_t **error );

int libpff_table_get_entry_value_by_index(
     libpff_table_t *table,
     int set_index,
     int entry_type,
     libfdata_reference_t **value_data_reference,
     libfcache_cache_t **value_data_cache,
     libcerror_error_t **error );

int libpff_table_get_entry_value_by_entry_type(
     libpff_table_t *table,
     int set_index,
     uint32_t entry_type,
     uint32_t *value_type,
     libfdata_reference_t **value_data_reference,
     libfcache_cache_t **value_data_cache,
     uint8_t flags,
     libcerror_error_t **error );

int libpff_table_get_entry_value_by_utf8_entry_name(
     libpff_table_t *table,
     int set_index,
     uint8_t *utf8_entry_name,
     size_t utf8_entry_name_length,
     uint32_t *value_type,
     libfdata_reference_t **value_data_reference,
     libfcache_cache_t **value_data_cache,
     libcerror_error_t **error );

int libpff_table_get_entry_value_by_utf16_entry_name(
     libpff_table_t *table,
     int set_index,
     uint16_t *utf16_entry_name,
     size_t utf16_entry_name_length,
     uint32_t *value_type,
     libfdata_reference_t **value_data_reference,
     libfcache_cache_t **value_data_cache,
     libcerror_error_t **error );

int libpff_table_read(
     libpff_table_t *table,
     libpff_io_handle_t *io_handle,
     libbfio_handle_t *file_io_handle,
     libpff_list_t *name_to_id_map_list,
     int debug_item_type,
     libcerror_error_t **error );

int libpff_table_read_index(
     libpff_table_t *table,
     libbfio_handle_t *file_io_handle,
     libcerror_error_t **error );

int libpff_table_read_record_entries_data_references(
     libpff_table_t *table,
     uint8_t record_entries_level,
     uint8_t record_entry_identifier_size,
     uint32_t record_entries_reference,
     libbfio_handle_t *file_io_handle,
     libcerror_error_t **error );

int libpff_table_read_values(
     libpff_table_t *table,
     uint8_t table_type,
     uint32_t table_value_reference,
     libpff_io_handle_t *io_handle,
     libpff_list_t *name_to_id_map_list,
     libbfio_handle_t *file_io_handle,
     int debug_item_type,
     libcerror_error_t **error );

int libpff_table_read_6c_values(
     libpff_table_t *table,
     uint32_t table_header_reference,
     libbfio_handle_t *file_io_handle,
     libcerror_error_t **error );

int libpff_table_read_7c_values(
     libpff_table_t *table,
     uint32_t table_header_reference,
     libpff_io_handle_t *io_handle,
     libpff_list_t *name_to_id_map_list,
     libbfio_handle_t *file_io_handle,
     libcerror_error_t **error );

int libpff_table_read_8c_values(
     libpff_table_t *table,
     uint32_t table_header_reference,
     libbfio_handle_t *file_io_handle,
     libcerror_error_t **error );

int libpff_table_read_9c_values(
     libpff_table_t *table,
     uint32_t table_header_reference,
     libbfio_handle_t *file_io_handle,
     libcerror_error_t **error );

int libpff_table_read_a5_values(
     libpff_table_t *table,
     uint32_t table_header_reference,
     libbfio_handle_t *file_io_handle,
     libcerror_error_t **error );

int libpff_table_read_ac_values(
     libpff_table_t *table,
     uint32_t table_header_reference,
     libpff_io_handle_t *io_handle,
     libpff_list_t *name_to_id_map_list,
     libbfio_handle_t *file_io_handle,
     libcerror_error_t **error );

int libpff_table_read_bc_values(
     libpff_table_t *table,
     uint32_t table_header_reference,
     libpff_io_handle_t *io_handle,
     libpff_list_t *name_to_id_map_list,
     libbfio_handle_t *file_io_handle,
     int debug_item_type,
     libcerror_error_t **error );

int libpff_table_read_b5_header(
     libpff_table_t *table,
     uint32_t table_header_reference,
     uint8_t *record_entry_identifier_size,
     uint8_t *record_entry_value_size,
     uint8_t *record_entries_level,
     uint32_t *record_entries_reference,
     libbfio_handle_t *file_io_handle,
     libcerror_error_t **error );

int libpff_table_read_6c_record_entries(
     libpff_table_t *table,
     uint32_t values_array_reference,
     libbfio_handle_t *file_io_handle,
     libcerror_error_t **error );

int libpff_table_read_7c_column_definitions(
     libpff_array_t *column_definitions_array,
     libfdata_reference_t *column_definitions_data_reference,
     int number_of_column_definitions,
     libbfio_handle_t *file_io_handle,
     libpff_list_t *name_to_id_map_list,
     libcerror_error_t **error );

int libpff_table_read_8c_record_entries(
     libpff_table_t *table,
     libbfio_handle_t *file_io_handle,
     libcerror_error_t **error );

int libpff_table_read_9c_record_entries(
     libpff_table_t *table,
     libbfio_handle_t *file_io_handle,
     libcerror_error_t **error );

int libpff_table_read_a5_record_entries(
     libpff_table_t *table,
     uint32_t record_entries_reference,
     libbfio_handle_t *file_io_handle,
     libcerror_error_t **error );

int libpff_table_read_ac_column_definitions(
     libpff_table_t *table,
     libpff_array_t *column_definitions_array,
     uint32_t column_definitions_reference,
     int number_of_column_definitions,
     libpff_io_handle_t *io_handle,
     libbfio_handle_t *file_io_handle,
     libpff_list_t *name_to_id_map_list,
     libcerror_error_t **error );

int libpff_table_read_bc_record_entries(
     libpff_table_t *table,
     libpff_io_handle_t *io_handle,
     libbfio_handle_t *file_io_handle,
     libpff_list_t *name_to_id_map_list,
     int debug_item_type,
     libcerror_error_t **error );

int libpff_table_read_values_array(
     libpff_table_t *table,
     uint32_t record_entries_reference,
     uint32_t values_array_reference,
     uint8_t record_entry_identifier_size,
     uint8_t record_entry_value_size,
     uint16_t values_array_entry_size,
     libpff_array_t *column_definitions_array,
     libpff_io_handle_t *io_handle,
     libbfio_handle_t *file_io_handle,
     libcerror_error_t **error );

int libpff_table_read_entry_value(
     libpff_table_t *table,
     int set_index,
     int entry_index,
     uint32_t record_entry_type,
     uint32_t record_entry_value_type,
     uint8_t *record_entry_value,
     size_t record_entry_value_size,
     libpff_io_handle_t *io_handle,
     libbfio_handle_t *file_io_handle,
     libpff_list_t *name_to_id_map_list,
     libpff_internal_name_to_id_map_entry_t *name_to_id_map_entry,
     libpff_table_t *record_entry_values_table,
     int debug_item_type,
     libcerror_error_t **error );

#if defined( __cplusplus )
}
#endif

#endif

