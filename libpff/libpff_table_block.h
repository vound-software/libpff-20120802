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

#if !defined( _LIBPFF_TABLE_BLOCK_H )
#define _LIBPFF_TABLE_BLOCK_H

#include <common.h>
#include <types.h>

#include "libpff_array_type.h"
#include "libpff_io_handle.h"
#include "libpff_libbfio.h"
#include "libpff_libcerror.h"
#include "libpff_libfcache.h"
#include "libpff_libfdata.h"

#if defined( __cplusplus )
extern "C" {
#endif

typedef struct libpff_table_block_value libpff_table_block_value_t;

struct libpff_table_block_value
{
	/* The table_block value size
	 */
	uint16_t size;

	/* The offset of the table block value
	 */
	uint16_t offset;
};

typedef struct libpff_table_block libpff_table_block_t;

struct libpff_table_block
{
	/* The table array entry
	 */
	int table_array_entry;

	/* The table block values array
	 */
	libpff_array_t *values_array;
};

int libpff_table_block_value_initialize(
     libpff_table_block_value_t **table_block_value,
     libcerror_error_t **error );

int libpff_table_block_value_free(
     libpff_table_block_value_t **table_block_value,
     libcerror_error_t **error );

int libpff_table_block_initialize(
     libpff_table_block_t **table_block,
     libcerror_error_t **error );

int libpff_table_block_free(
     libpff_table_block_t **table_block,
     libcerror_error_t **error );

int libpff_table_block_read(
     libpff_table_block_t *table_block,
     libpff_io_handle_t *io_handle,
     libbfio_handle_t *file_io_handle,
     libfdata_block_t *table_array_data_block,
     libfcache_cache_t *table_array_data_cache,
     libcerror_error_t **error );

int libpff_table_block_read_index(
     libpff_table_block_t *table_block,
     uint8_t *table_block_data,
     size_t table_block_data_size,
     uint16_t table_block_index_offset,
     libcerror_error_t **error );

int libpff_table_block_get_number_of_values(
     libpff_table_block_t *table_block,
     int *number_of_values,
     libcerror_error_t **error );

int libpff_table_block_get_value(
     libpff_table_block_t *table_block,
     int value_index,
     libpff_table_block_value_t **table_block_value,
     libcerror_error_t **error );

#if defined( __cplusplus )
}
#endif

#endif

