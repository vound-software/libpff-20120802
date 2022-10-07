/*
 * Allocation table functions
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

#if !defined( _LIBPFF_ALLOCATION_TABLE_H )
#define _LIBPFF_ALLOCATION_TABLE_H

#include <common.h>
#include <types.h>

#include "libpff_libbfio.h"
#include "libpff_libcerror.h"
#include "libpff_offset_list.h"

#if defined( __cplusplus )
extern "C" {
#endif

#define LIBPFF_ALLOCATION_TABLE_TYPE_PAGE	0x83
#define LIBPFF_ALLOCATION_TABLE_TYPE_DATA	0x84

int libpff_allocation_table_read(
     libpff_offset_list_t *unallocated_block_list,
     libbfio_handle_t *file_io_handle,
     off64_t allocation_table_offset,
     uint8_t file_type,
     libcerror_error_t **error );

#if defined( __cplusplus )
}
#endif

#endif

