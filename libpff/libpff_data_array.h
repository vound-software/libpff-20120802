/*
 * Data array functions
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

#if !defined( _LIBPFF_DATA_ARRAY_H )
#define _LIBPFF_DATA_ARRAY_H

#include <common.h>
#include <types.h>

#include "libpff_array_type.h"
#include "libpff_io_handle.h"
#include "libpff_libbfio.h"
#include "libpff_libcerror.h"
#include "libpff_libfdata.h"

#if defined( __cplusplus )
extern "C" {
#endif

enum LIBPFF_DATA_ARRAY_FLAGS
{
	LIBPFF_DATA_ARRAY_FLAG_DECRYPTION_FORCED	= 0x02,
};

typedef struct libpff_data_array libpff_data_array_t;

struct libpff_data_array
{
	/* Value to indicate if the data array descriptor identifier
	 */
	uint32_t descriptor_identifier;

	/* Value to indicate if the data array data identifier
	 */
	uint64_t data_identifier;

	/* A reference to the IO handle
	 */
	libpff_io_handle_t *io_handle;

	/* The total size of the data in the array
	 */
	size64_t data_size;

	/* The data array entries array
	 */
	libpff_array_t *entries;

	/* The (data array) flags
	 */
	uint8_t flags;
};

int libpff_data_array_initialize(
     libpff_data_array_t **data_array,
     uint32_t descriptor_identifier,
     uint64_t data_identifier,
     libpff_io_handle_t *io_handle,
     libcerror_error_t **error );

int libpff_data_array_free(
     libpff_data_array_t **data_array,
     libcerror_error_t **error );

int libpff_data_array_clone(
     libpff_data_array_t **destination_data_array,
     libpff_data_array_t *source_data_array,
     libcerror_error_t **error );

int libpff_data_array_read(
     libpff_data_array_t *data_array,
     libpff_io_handle_t *io_handle,
     libbfio_handle_t *file_io_handle,
     libfdata_block_t *descriptor_data_block,
     uint8_t recovered,
     uint8_t *array_data,
     size_t array_data_size,
     libcerror_error_t **error );

int libpff_data_array_read_entries(
     libpff_data_array_t *data_array,
     libpff_io_handle_t *io_handle,
     libbfio_handle_t *file_io_handle,
     libfdata_block_t *descriptor_data_block,
     uint8_t recovered,
     uint8_t *array_data,
     size_t array_data_size,
     uint32_t *total_data_size,
     libcerror_error_t **error );

ssize_t libpff_data_array_read_entry_data(
         intptr_t *io_handle,
         libbfio_handle_t *file_io_handle,
         int array_entry_index,
         uint8_t *data,
         size_t data_size,
         uint8_t flags,
         libcerror_error_t **error );

int libpff_data_array_decrypt_entry_data(
     libpff_data_array_t *data_array,
     int array_entry_index,
     uint8_t encryption_type,
     uint8_t *data,
     size_t data_size,
     uint8_t flags,
     libcerror_error_t **error );

#if defined( __cplusplus )
}
#endif

#endif

