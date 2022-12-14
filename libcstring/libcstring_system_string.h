/*
 * System character type string functions
 *
 * Copyright (c) 2010-2012, Joachim Metz <joachim.metz@gmail.com>
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

#if !defined( _LIBCSTRING_SYSTEM_STRING_H )
#define _LIBCSTRING_SYSTEM_STRING_H

#include <common.h>
#include <types.h>

#include "libcstring_definitions.h"
#include "libcstring_narrow_string.h"
#include "libcstring_types.h"
#include "libcstring_wide_string.h"

#if defined( _cplusplus )
extern "C" {
#endif

#if defined( LIBCSTRING_HAVE_WIDE_SYSTEM_CHARACTER )

#if SIZEOF_WCHAR_T != 2
#error Unsupported wide system character size
#endif

/* Intermediate version of the macro required
 * for correct evaluation predefined string
 */
#define _LIBCSTRING_SYSTEM_STRING_INTERMEDIATE( string ) \
	L ## string

#define _LIBCSTRING_SYSTEM_STRING( string ) \
	_LIBCSTRING_SYSTEM_STRING_INTERMEDIATE( string )

#define libcstring_system_string_allocate( size ) \
	libcstring_wide_string_allocate( size )

#define libcstring_system_string_reallocate( string, size ) \
	libcstring_wide_string_reallocate( string, size )

#define libcstring_system_string_compare( destination, source, size ) \
	libcstring_wide_string_compare( destination, source, size )

#define libcstring_system_string_compare_no_case( destination, source, size ) \
	libcstring_wide_string_compare_no_case( destination, source, size )

#define libcstring_system_string_copy( destination, source, size ) \
	libcstring_wide_string_copy( destination, source, size )

#define libcstring_system_string_length( string ) \
	libcstring_wide_string_length( string )

#define libcstring_system_string_search_character( string, character, size ) \
	libcstring_wide_string_search_character( string, character, size )

#define libcstring_system_string_search_character_reverse( string, character, size ) \
	libcstring_wide_string_search_character_reverse( string, character, size )

#define libcstring_system_string_search_string( string, substring, size ) \
	libcstring_wide_string_search_string( string, substring, size )

#if defined( __BORLANDC__ ) && ( __BORLANDC__ < 0x0560 )
#define libcstring_system_string_sprintf \
	libcstring_wide_string_snwprintf

#else
#define libcstring_system_string_sprintf( string, size, format, ... ) \
	libcstring_wide_string_snwprintf( string, size, format, __VA_ARGS__ )
#endif

#if defined( __BORLANDC__ ) && ( __BORLANDC__ < 0x0560 )
#define libcstring_system_string_vsprintf \
	libcstring_wide_string_vsnwprintf

#else
#define libcstring_system_string_vsprintf( string, size, format, ... ) \
	libcstring_wide_string_vsnwprintf( string, size, format, __VA_ARGS__ )
#endif

#else

#define _LIBCSTRING_SYSTEM_STRING( string ) \
	string

#define libcstring_system_string_allocate( size ) \
	libcstring_narrow_string_allocate( size )

#define libcstring_system_string_reallocate( string, size ) \
	libcstring_narrow_string_reallocate( string, size )

#define libcstring_system_string_compare( destination, source, size ) \
	libcstring_narrow_string_compare( destination, source, size )

#define libcstring_system_string_compare_no_case( destination, source, size ) \
	libcstring_narrow_string_compare_no_case( destination, source, size )

#define libcstring_system_string_copy( destination, source, size ) \
	libcstring_narrow_string_copy( destination, source, size )

#define libcstring_system_string_length( string ) \
	libcstring_narrow_string_length( string )

#define libcstring_system_string_search_character( string, character, size ) \
	libcstring_narrow_string_search_character( string, character, size )

#define libcstring_system_string_search_character_reverse( string, character, size ) \
	libcstring_narrow_string_search_character_reverse( string, character, size )

#define libcstring_system_string_search_string( string, substring, size ) \
	libcstring_narrow_string_search_string( string, substring, size )

#if defined( __BORLANDC__ ) && ( __BORLANDC__ < 0x0560 )
#define libcstring_system_string_sprintf \
	libcstring_narrow_string_snprintf

#else
#define libcstring_system_string_sprintf( string, size, format, ... ) \
	libcstring_narrow_string_snprintf( string, size, format, __VA_ARGS__ )
#endif

#if defined( __BORLANDC__ ) && ( __BORLANDC__ < 0x0560 )
#define libcstring_system_string_vsprintf \
	libcstring_narrow_string_vsnprintf

#else
#define libcstring_system_string_vsprintf( string, size, format, ... ) \
	libcstring_narrow_string_vsnprintf( string, size, format, __VA_ARGS__ )
#endif

#endif /* defined( LIBCSTRING_HAVE_WIDE_SYSTEM_CHARACTER ) */

#if defined( _cplusplus )
}
#endif

#endif

