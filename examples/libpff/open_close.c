/*
 * C application to open and close a Personal Folder File using libpff
 *
 * Copyright (c) 2010, Joachim Metz <joachim.metz@gmail.com>
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

#include <stdlib.h>
#include <stdio.h>

#include <libpff.h>

int main( int argc, char * const argv[] )
{
	libpff_error_t *error = NULL;
	libpff_file_t *file   = NULL;

	if( argc != 2 )
	{
		fprintf(
		 stderr,
		 "Usage: ./open_close filename\n" );

		return( EXIT_FAILURE );
	}
	/* The function will return 1 if successful or -1 on error.
	 * On error the error 'object' is created by the library.
	 *
	 * file must refer to NULL to create a new libpff file 'object'.
	 *
	 * If error is NULL e.g. libpff_file_initialize( &file, NULL )
	 * no error 'object' is created
	 *
	 * The error 'object' can be freed by libpff_error_free()
	 */
	if( libpff_file_initialize(
	     &file,
	     &error ) != 1 )
	{
		fprintf(
		 stderr,
		 "Unable to initialize file.\n" );

		goto exit_failure;
	}
	if( libpff_file_open(
	     file,
	     argv[ 1 ],
	     LIBPFF_OPEN_READ,
	     &error ) != 1 )
	{
		fprintf(
		 stderr,
		 "Unable to open file.\n" );

		goto exit_failure_free_file;
	}
	if( libpff_file_close(
	     file,
	     &error ) != 0 )
	{
		fprintf(
		 stderr,
		 "Unable to close file.\n" );

		goto exit_failure_free_file;
	}
	if( libpff_file_free(
	     &file,
	     &error ) != 1 )
	{
		fprintf(
		 stderr,
		 "Unable to free file.\n" );

		goto exit_failure;
	}
	return( EXIT_SUCCESS );

exit_failure_free_file:
	libpff_file_free(
	 &file,
	 NULL );

exit_failure:
	libpff_error_backtrace_fprint(
	 error,
	 stderr );
	libpff_error_free(
	 &error );

	return( EXIT_FAILURE );
}

