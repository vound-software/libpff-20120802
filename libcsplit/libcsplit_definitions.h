/*
 * The internal definitions
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

#if !defined( LIBCSPLIT_INTERNAL_DEFINITIONS_H )
#define LIBCSPLIT_INTERNAL_DEFINITIONS_H

#include <common.h>
#include <types.h>

/* Define HAVE_LOCAL_LIBCSPLIT for local use of libcsplit
 */
#if !defined( HAVE_LOCAL_LIBCSPLIT )
#include <libcsplit/definitions.h>

/* The definitions in <libcsplit/definitions.h> are copied here
 * for local use of libcsplit
 */
#else

#define LIBCSPLIT_VERSION			20120701

/* The libcstring version string
 */
#define LIBCSPLIT_VERSION_STRING		"20120701"

#endif /* !defined( HAVE_LOCAL_LIBCSPLIT ) */

#endif

