/*
 * Date and time functions
 *
 * Copyright (c) 2008-2012, Joachim Metz <jbmetz@users.sourceforge.net>
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

#include <common.h>
#include <memory.h>
#include <types.h>

#if defined( HAVE_STRING_H ) || defined( WINAPI )
#include <string.h>
#endif

#if defined( TIME_WITH_SYS_TIME )
#include <sys/time.h>
#include <time.h>
#elif defined( HAVE_SYS_TIME_H )
#include <sys/time.h>
#else
#include <time.h>
#endif

#include "libcsystem_date_time.h"
#include "libcsystem_libcerror.h"
#include "libcsystem_libcstring.h"

#if defined( HAVE_CTIME ) || defined( HAVE_CTIME_R ) || defined( WINAPI )

/* Retrieves a ctime formatted string
 * The string must be at least 32 characters of size including the end of string character
 * Returns 1 if successful or -1 on error
 */
int libcsystem_date_time_get_ctime_string(
     const time_t *timestamp,
     libcstring_system_character_t *string,
     size_t string_size,
     libcerror_error_t **error )
{
	static char *function                                    = "libcsystem_date_time_get_ctime_string";

#if ( defined( HAVE_CTIME ) && !defined( HAVE_CTIME_R ) ) || ( defined( WINAPI ) && !defined( _MSC_VER ) )
	const libcstring_system_character_t *static_ctime_string = NULL;
	size_t static_ctime_string_length                        = 0;
#endif

	if( timestamp == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid timestamp.",
		 function );

		return( -1 );
	}
	if( string == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid string.",
		 function );

		return( -1 );
	}
	if( string_size > (size_t) SSIZE_MAX )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_EXCEEDS_MAXIMUM,
		 "%s: invalid string size value exceeds maximum.",
		 function );

		return( -1 );
	}
	if( string_size < 32 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_TOO_SMALL,
		 "%s: string too small.",
		 function );

		return( -1 );
	}
#if defined( _MSC_VER )
#if defined( LIBCSTRING_HAVE_WIDE_SYSTEM_CHARACTER )
	if( _wctime_s(
	     string,
	     string_size,
	     timestamp ) != 0 )
#else
	if( ctime_s(
	     string,
	     string_size,
	     timestamp ) != 0 )
#endif
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to set string.",
		 function );

		return( -1 );
	}

#elif defined( HAVE_CTIME_R )
/* Sanity check
 */
#if defined( LIBCSTRING_HAVE_WIDE_SYSTEM_CHARACTER )
#error Missing wide character ctime_r function
#endif

#if defined( HAVE_CTIME_R_SIZE )
	if( ctime_r(
	     timestamp,
	     string,
	     string_size ) == NULL )
#else
	if( ctime_r(
	     timestamp,
	     string ) == NULL )
#endif
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to set string.",
		 function );

		return( -1 );
	}

#else
/* Sanity check
 */
#if defined( LIBCSTRING_HAVE_WIDE_SYSTEM_CHARACTER ) && !defined( WINAPI )
#error Missing wide character ctime function
#endif

#if defined( LIBCSTRING_HAVE_WIDE_SYSTEM_CHARACTER )
	static_ctime_string = _wctime(
	                       timestamp );
#else
	static_ctime_string = ctime(
	                       timestamp );
#endif
	if( static_ctime_string == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to create static ctime string.",
		 function );

		return( -1 );
	}
	static_ctime_string_length = libcstring_system_string_length(
	                              static_ctime_string );

	if( libcstring_system_string_copy(
	     string,
	     static_ctime_string,
	     static_ctime_string_length ) == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to set string.",
		 function );

		return( -1 );
	}
	string[ static_ctime_string_length ] = 0;
#endif

	return( 1 );
}

#endif

#if defined( HAVE_LOCALTIME ) || defined( HAVE_LOCALTIME_R ) || defined( WINAPI )

/* Retrieves time elements in the local time
 * Returns 1 if successful or -1 on error
 */
int libcsystem_date_time_get_time_elements_in_localtime(
     const time_t *timestamp,
     struct tm *time_elements,
     libcerror_error_t **error )
{
	static char *function           = "libcsystem_date_time_get_time_elements_in_localtime";

#if !defined( HAVE_LOCALTIME_R ) && !defined( _MSC_VER )
	struct tm *static_time_elements = NULL;
#endif

	if( timestamp == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid timestamp.",
		 function );

		return( -1 );
	}
	if( time_elements == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid time elements.",
		 function );

		return( -1 );
	}
#if defined( _MSC_VER )
	if( localtime_s(
	     time_elements,
	     timestamp ) != 0 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to set time elements.",
		 function );

		return( -1 );
	}

#elif defined( HAVE_LOCALTIME_R )
	if( localtime_r(
	     timestamp,
	     time_elements ) == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to set time elements.",
		 function );

		return( -1 );
	}

#else
	static_time_elements = localtime(
	                        timestamp );

	if( static_time_elements == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to create static time elements.",
		 function );

		return( -1 );
	}
	if( memory_copy(
	     time_elements,
	     static_time_elements,
	     sizeof( struct tm ) ) == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_COPY_FAILED,
		 "%s: unable to set time elements.",
		 function );

		return( -1 );
	}
#endif

	return( 1 );
}

#endif

#if defined( HAVE_GMTIME ) || defined( HAVE_GMTIME_R ) || defined( WINAPI )

/* Retrieves time elements in UTC (GMT)
 * Returns 1 if successful or -1 on error
 */
int libcsystem_date_time_get_time_elements_in_utc(
     const time_t *timestamp,
     struct tm *time_elements,
     libcerror_error_t **error )
{
	static char *function           = "libcsystem_date_time_get_time_elements_in_utc";

#if !defined( HAVE_GMTIME_R ) && !defined( _MSC_VER )
	struct tm *static_time_elements = NULL;
#endif

	if( timestamp == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid timestamp.",
		 function );

		return( -1 );
	}
	if( time_elements == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid time elements.",
		 function );

		return( -1 );
	}
#if defined( _MSC_VER )
	if( gmtime_s(
	     time_elements,
	     timestamp ) != 0 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to set time elements.",
		 function );

		return( -1 );
	}

#elif defined( HAVE_GMTIME_R )
	if( gmtime_r(
	     timestamp,
	     time_elements ) == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to set time elements.",
		 function );

		return( -1 );
	}

#else
	static_time_elements = gmtime(
	                        timestamp );

	if( static_time_elements == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to create static time elements.",
		 function );

		return( -1 );
	}
	if( memory_copy(
	     time_elements,
	     static_time_elements,
	     sizeof( struct tm ) ) == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_COPY_FAILED,
		 "%s: unable to set time elements.",
		 function );

		return( -1 );
	}
#endif

	return( 1 );
}

#endif

