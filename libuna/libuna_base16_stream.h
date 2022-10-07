/*
 * Base16 stream functions
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

#if !defined( _LIBUNA_BASE16_STREAM_H )
#define _LIBUNA_BASE16_STREAM_H

#include <common.h>
#include <types.h>

#include "libuna_extern.h"
#include "libuna_libcerror.h"
#include "libuna_types.h"

#if defined( __cplusplus )
extern "C" {
#endif

LIBUNA_EXTERN \
int libuna_base16_stream_size_to_byte_stream(
     const uint8_t *base16_stream,
     size_t base16_stream_size,
     size_t *byte_stream_size,
     uint32_t base16_variant,
     uint8_t flags,
     libcerror_error_t **error );

LIBUNA_EXTERN \
int libuna_base16_stream_copy_to_byte_stream(
     const uint8_t *base16_stream,
     size_t base16_stream_size,
     uint8_t *byte_stream,
     size_t byte_stream_size,
     uint32_t base16_variant,
     uint8_t flags,
     libcerror_error_t **error );

LIBUNA_EXTERN \
int libuna_base16_stream_size_from_byte_stream(
     const uint8_t *byte_stream,
     size_t byte_stream_size,
     size_t *base16_stream_size,
     uint32_t base16_variant,
     libcerror_error_t **error );

LIBUNA_EXTERN \
int libuna_base16_stream_copy_from_byte_stream(
     uint8_t *base16_stream,
     size_t base16_stream_size,
     const uint8_t *byte_stream,
     size_t byte_stream_size,
     uint32_t base16_variant,
     libcerror_error_t **error );

LIBUNA_EXTERN \
int libuna_base16_stream_with_index_copy_from_byte_stream(
     uint8_t *base16_stream,
     size_t base16_stream_size,
     size_t *base16_stream_index,
     const uint8_t *byte_stream,
     size_t byte_stream_size,
     uint32_t base16_variant,
     libcerror_error_t **error );

#if defined( __cplusplus )
}
#endif

#endif

