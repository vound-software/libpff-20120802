/*
 * Data block functions
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

#if !defined( _LIBPFF_DATA_BLOCK_H )
#define _LIBPFF_DATA_BLOCK_H

#include <common.h>
#include <types.h>

#include "libpff_io_handle.h"
#include "libpff_libbfio.h"
#include "libpff_libcerror.h"

#if defined( __cplusplus )
extern "C" {
#endif

enum LIBPFF_DATA_BLOCK_FLAGS
{
	LIBPFF_DATA_BLOCK_FLAG_VALIDATED		= 0x01,
	LIBPFF_DATA_BLOCK_FLAG_DECRYPTION_FORCED	= 0x02,

	LIBPFF_DATA_BLOCK_FLAG_CRC_MISMATCH		= 0x10,
	LIBPFF_DATA_BLOCK_FLAG_SIZE_MISMATCH		= 0x20,
	LIBPFF_DATA_BLOCK_FLAG_IDENTIFIER_MISMATCH	= 0x40
};

typedef struct libpff_data_block libpff_data_block_t;

struct libpff_data_block
{
	/* Value to indicate if the descriptor identifier
	 */
	uint32_t descriptor_identifier;

	/* Value to indicate if the data identifier
	 */
	uint64_t data_identifier;

	/* A reference to the IO handle
	 */
	libpff_io_handle_t *io_handle;

	/* The flags
	 */
	uint8_t flags;
};

int libpff_data_block_initialize(
     libpff_data_block_t **data_block,
     uint32_t descriptor_identifier,
     uint64_t data_identifier,
     libpff_io_handle_t *io_handle,
     libcerror_error_t **error );

int libpff_data_block_free(
     libpff_data_block_t **data_block,
     libcerror_error_t **error );

int libpff_data_block_clone(
     libpff_data_block_t **destination_data_block,
     libpff_data_block_t *source_data_block,
     libcerror_error_t **error );

ssize_t libpff_data_block_read_data(
         intptr_t *io_handle,
         libbfio_handle_t *file_io_handle,
         int segment_index,
         uint8_t *data,
         size_t data_size,
         uint8_t flags,
         libcerror_error_t **error );

ssize_t libpff_data_block_read(
         libbfio_handle_t *file_io_handle,
         uint8_t file_type,
         uint64_t data_identifier,
         uint8_t *data,
         size_t data_size,
         uint8_t *flags,
         libcerror_error_t **error );

int libpff_data_block_decrypt_data(
     libpff_data_block_t *data_block,
     uint8_t encryption_type,
     uint8_t *data,
     size_t data_size,
     uint8_t flags,
     libcerror_error_t **error );

#if defined( __cplusplus )
}
#endif

#endif

