/*
 * Windows 1251 codepage (Cyrillic) functions
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

#if !defined( _LIBUNA_CODEPAGE_WINDOWS_1251_H )
#define _LIBUNA_CODEPAGE_WINDOWS_1251_H

#include <common.h>
#include <types.h>

#if defined( __cplusplus )
extern "C" {
#endif

extern const uint16_t libuna_codepage_windows_1251_byte_stream_to_unicode_base_0x80[ 128 ];

extern const uint8_t libuna_codepage_windows_1251_unicode_to_byte_stream_base_0x00a0[ 32 ];
extern const uint8_t libuna_codepage_windows_1251_unicode_to_byte_stream_base_0x0400[ 96 ];
extern const uint8_t libuna_codepage_windows_1251_unicode_to_byte_stream_base_0x2010[ 24 ];

#if defined( __cplusplus )
}
#endif

#endif

