/*
 * The libfvalue header wrapper
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

#if !defined( _PFFTOOLS_LIBFVALUE_H )
#define _PFFTOOLS_LIBFVALUE_H

#include <common.h>

/* Define HAVE_LOCAL_LIBFVALUE for local use of libfvalue
 */
#if defined( HAVE_LOCAL_LIBFVALUE )

#include <libfvalue_codepage.h>
#include <libfvalue_definitions.h>
#include <libfvalue_floating_point.h>
#include <libfvalue_integer.h>
#include <libfvalue_split_string.h>
#include <libfvalue_string.h>
#include <libfvalue_table.h>
#include <libfvalue_types.h>
#include <libfvalue_value.h>
#include <libfvalue_value_type.h>

#else

/* If libtool DLL support is enabled set LIBFVALUE_DLL_IMPORT
 * before including libfvalue.h
 */
#if defined( _WIN32 ) && defined( DLL_IMPORT )
#define LIBFVALUE_DLL_IMPORT
#endif

#include <libfvalue.h>

#endif

#endif

