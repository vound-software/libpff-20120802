/*
 * The handle functions
 *
 * Copyright (c) 2009-2012, Joachim Metz <jbmetz@users.sourceforge.net>
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

#if !defined( _LIBBFIO_INTERNAL_HANDLE_H )
#define _LIBBFIO_INTERNAL_HANDLE_H

#include <common.h>
#include <types.h>

#include "libbfio_extern.h"
#include "libbfio_libcerror.h"
#include "libbfio_list_type.h"
#include "libbfio_offset_list.h"
#include "libbfio_types.h"

#if defined( _MSC_VER ) || defined( __BORLANDC__ ) || defined( __MINGW32_VERSION ) || defined( __MINGW64_VERSION_MAJOR )

/* This inclusion is needed otherwise some linkers
 * mess up exporting the legacy functions
 */
#include "libbfio_legacy.h"
#endif

#if defined( __cplusplus )
extern "C" {
#endif

typedef struct libbfio_internal_handle libbfio_internal_handle_t;

struct libbfio_internal_handle
{
	/* The IO handle
	 */
	intptr_t *io_handle;

	/* The flags
	 */
	uint8_t flags;

	/* The access flags
	 */
	int access_flags;

	/* The current offset
	 */
	off64_t offset;

	/* The size
	 */
	size64_t size;

	/* Value to indicate the size was set
	 */
	uint8_t size_set;

	/* Value to indicate to open and close
	 * the systems file descriptor or handle on demand
	 */
	uint8_t open_on_demand;

	/* Reference to the pool last used list element
	 */
	libbfio_list_element_t *pool_last_used_list_element;

	/* Value to indicate to track offsets read
	 */
	uint8_t track_offsets_read;

	/* The offset ranges that were read
	 */
	libbfio_offset_list_t *offsets_read;

	/* The narrow string codepage
	 */
	int narrow_string_codepage;

	/* The free IO handle function
	 */
	int (*free_io_handle)(
	       intptr_t **io_handle,
	       libcerror_error_t **error );

	/* The clone (duplicate) IO handle function
	 */
	int (*clone_io_handle)(
	       intptr_t **destination_io_handle,
	       intptr_t *source_io_handle,
	       libcerror_error_t **error );

	/* The open function
	 */
	int (*open)(
	       intptr_t *io_handle,
	       int access_flags,
	       libcerror_error_t **error );

	/* The close function
	 */
	int (*close)(
	       intptr_t *io_handle,
	       libcerror_error_t **error );

	/* The read function
	 */
	ssize_t (*read)(
	           intptr_t *io_handle,
	           uint8_t *buffer,
	           size_t size,
	           libcerror_error_t **error );

	/* The write function
	 */
	ssize_t (*write)(
	           intptr_t *io_handle,
	           const uint8_t *buffer,
	           size_t size,
	           libcerror_error_t **error );

	/* The seek offset function
	 */
	off64_t (*seek_offset)(
	           intptr_t *io_handle,
	           off64_t offset,
	           int whence,
	           libcerror_error_t **error );

	/* The exists function
	 */
	int (*exists)(
	       intptr_t *io_handle,
	       libcerror_error_t **error );

	/* The is open function
	 */
	int (*is_open)(
	       intptr_t *io_handle,
	       libcerror_error_t **error );

	/* The get size function
	 */
	int (*get_size)(
	       intptr_t *io_handle,
	       size64_t *size,
	       libcerror_error_t **error );

};

LIBBFIO_EXTERN \
int libbfio_handle_initialize(
     libbfio_handle_t **handle,
     intptr_t *io_handle,
     int (*free_io_handle)(
            intptr_t **io_handle,
            libcerror_error_t **error ),
     int (*clone_io_handle)(
            intptr_t **destination_io_handle,
            intptr_t *source_io_handle,
            libcerror_error_t **error ),
     int (*open)(
            intptr_t *io_handle,
            int access_flags,
            libcerror_error_t **error ),
     int (*close)(
            intptr_t *io_handle,
            libcerror_error_t **error ),
     ssize_t (*read)(
                intptr_t *io_handle,
                uint8_t *buffer,
                size_t size,
                libcerror_error_t **error ),
     ssize_t (*write)(
                intptr_t *io_handle,
                const uint8_t *buffer,
                size_t size,
                libcerror_error_t **error ),
     off64_t (*seek_offset)(
                intptr_t *io_handle,
                off64_t offset,
                int whence,
                libcerror_error_t **error ),
     int (*exists)(
            intptr_t *io_handle,
            libcerror_error_t **error ),
     int (*is_open)(
            intptr_t *io_handle,
            libcerror_error_t **error ),
     int (*get_size)(
            intptr_t *io_handle,
            size64_t *size,
            libcerror_error_t **error ),
     uint8_t flags,
     libcerror_error_t **error );

LIBBFIO_EXTERN \
int libbfio_handle_free(
     libbfio_handle_t **handle,
     libcerror_error_t **error );

LIBBFIO_EXTERN \
int libbfio_handle_clone(
     libbfio_handle_t **destination_handle,
     libbfio_handle_t *source_handle,
     libcerror_error_t **error );

LIBBFIO_EXTERN \
int libbfio_handle_open(
     libbfio_handle_t *handle,
     int access_flags,
     libcerror_error_t **error );

LIBBFIO_EXTERN \
int libbfio_handle_reopen(
     libbfio_handle_t *handle,
     int access_flags,
     libcerror_error_t **error );

LIBBFIO_EXTERN \
int libbfio_handle_close(
     libbfio_handle_t *handle,
     libcerror_error_t **error );

LIBBFIO_EXTERN \
ssize_t libbfio_handle_read_buffer(
         libbfio_handle_t *handle,
         uint8_t *buffer,
         size_t size,
         libcerror_error_t **error );

LIBBFIO_EXTERN \
ssize_t libbfio_handle_write_buffer(
         libbfio_handle_t *handle,
         const uint8_t *buffer,
         size_t size,
         libcerror_error_t **error );

LIBBFIO_EXTERN \
off64_t libbfio_handle_seek_offset(
         libbfio_handle_t *handle,
         off64_t offset,
         int whence,
         libcerror_error_t **error );

LIBBFIO_EXTERN \
int libbfio_handle_exists(
     libbfio_handle_t *handle,
     libcerror_error_t **error );

LIBBFIO_EXTERN \
int libbfio_handle_is_open(
     libbfio_handle_t *handle,
     libcerror_error_t **error );

LIBBFIO_EXTERN \
int libbfio_handle_get_io_handle(
     libbfio_handle_t *handle,
     intptr_t **io_handle,
     libcerror_error_t **error );

LIBBFIO_EXTERN \
int libbfio_handle_get_access_flags(
     libbfio_handle_t *handle,
     int *access_flags,
     libcerror_error_t **error );

LIBBFIO_EXTERN \
int libbfio_handle_set_access_flags(
     libbfio_handle_t *handle,
     int access_flags,
     libcerror_error_t **error );

LIBBFIO_EXTERN \
int libbfio_handle_get_size(
     libbfio_handle_t *handle,
     size64_t *size,
     libcerror_error_t **error );

LIBBFIO_EXTERN \
int libbfio_handle_get_offset(
     libbfio_handle_t *handle,
     off64_t *offset,
     libcerror_error_t **error );

LIBBFIO_EXTERN \
int libbfio_handle_set_open_on_demand(
     libbfio_handle_t *handle,
     uint8_t open_on_demand,
     libcerror_error_t **error );

LIBBFIO_EXTERN \
int libbfio_handle_set_track_offsets_read(
     libbfio_handle_t *handle,
     uint8_t track_offsets_read,
     libcerror_error_t **error );

LIBBFIO_EXTERN \
int libbfio_handle_get_number_of_offsets_read(
     libbfio_handle_t *handle,
     int *number_of_read_offsets,
     libcerror_error_t **error );

LIBBFIO_EXTERN \
int libbfio_handle_get_offset_read(
     libbfio_handle_t *handle,
     int index,
     off64_t *offset,
     size64_t *size,
     libcerror_error_t **error );

#if defined( __cplusplus )
}
#endif

#endif

