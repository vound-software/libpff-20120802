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

#include <common.h>
#include <memory.h>
#include <types.h>

#include "libbfio_definitions.h"
#include "libbfio_handle.h"
#include "libbfio_libcerror.h"
#include "libbfio_list_type.h"
#include "libbfio_offset_list.h"

/* Initializes the handle
 * Returns 1 if successful or -1 on error
 */
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
      libcerror_error_t **error )
{
	libbfio_internal_handle_t *internal_handle = NULL;
	static char *function                      = "libbfio_handle_initialize";

	if( handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid handle.",
		 function );

		return( -1 );
	}
	if( *handle != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid handle value already set.",
		 function );

		return( -1 );
	}
	internal_handle = memory_allocate_structure(
	                   libbfio_internal_handle_t );

	if( internal_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create handle.",
		 function );

		goto on_error;
	}
	if( memory_set(
	     internal_handle,
	     0,
	     sizeof( libbfio_internal_handle_t ) ) == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_SET_FAILED,
		 "%s: unable to clear handle.",
		 function );

		goto on_error;
	}
	if( libbfio_offset_list_initialize(
	     &( internal_handle->offsets_read ),
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create read offsets list.",
		 function );

		goto on_error;
	}
	internal_handle->io_handle       = io_handle;
	internal_handle->flags           = flags;
	internal_handle->free_io_handle  = free_io_handle;
	internal_handle->clone_io_handle = clone_io_handle;
	internal_handle->open            = open;
	internal_handle->close           = close;
	internal_handle->read            = read;
	internal_handle->write           = write;
	internal_handle->seek_offset     = seek_offset;
	internal_handle->exists          = exists;
	internal_handle->is_open         = is_open;
	internal_handle->get_size        = get_size;

	*handle = (libbfio_handle_t *) internal_handle;

	return( 1 );

on_error:
	if( internal_handle != NULL )
	{
		memory_free(
		 internal_handle );
	}
	return( -1 );
}

/* Frees the handle
 * Returns 1 if successful or -1 on error
 */
int libbfio_handle_free(
     libbfio_handle_t **handle,
     libcerror_error_t **error )
{
	libbfio_internal_handle_t *internal_handle = NULL;
	static char *function                      = "libbfio_handle_free";
	int is_open                                = 0;
	int result                                 = 1;

	if( handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid handle.",
		 function );

		return( -1 );
	}
	if( *handle != NULL )
	{
		internal_handle = (libbfio_internal_handle_t *) *handle;
		*handle         = NULL;

		is_open = internal_handle->is_open(
			   internal_handle->io_handle,
		           error );

		if( is_open == -1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_IO,
			 LIBCERROR_IO_ERROR_OPEN_FAILED,
			 "%s: unable to determine if handle is open.",
			 function );

			result = -1;
		}
		else if( is_open != 0 )
		{
			if( internal_handle->close(
			     internal_handle->io_handle,
			     error ) != 0 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_IO,
				 LIBCERROR_IO_ERROR_CLOSE_FAILED,
				 "%s: unable to close handle.",
				 function );

				result = -1;
			}
		}
		if( ( internal_handle->flags & LIBBFIO_FLAG_IO_HANDLE_MANAGED ) != 0 )
		{
			if( internal_handle->io_handle != NULL )
			{
				if( internal_handle->free_io_handle == NULL )
				{
					memory_free(
					 internal_handle->io_handle );
				}
				else if( internal_handle->free_io_handle(
					  &( internal_handle->io_handle ),
					  error ) != 1 )
				{
					libcerror_error_set(
					 error,
					 LIBCERROR_ERROR_DOMAIN_RUNTIME,
					 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
					 "%s: unable to free IO handle.",
					 function );

					result = -1;
				}
			}
		}
		if( internal_handle->offsets_read != NULL )
		{
			if( libbfio_offset_list_free(
			     &( internal_handle->offsets_read ),
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
				 "%s: unable to free read offsets list.",
				 function );

				result = -1;
			}
		}
		memory_free(
		 internal_handle );
	}
	return( result );
}

/* Clones (duplicates) the handle
 * The values in the offsets read list are not duplicated
 * Returns 1 if successful or -1 on error
 */
int libbfio_handle_clone(
     libbfio_handle_t **destination_handle,
     libbfio_handle_t *source_handle,
     libcerror_error_t **error )
{
	libbfio_internal_handle_t *internal_source_handle = NULL;
	intptr_t *destination_io_handle                   = NULL;
	static char *function                             = "libbfio_handle_clone";
	uint8_t destination_flags                         = 0;

	if( destination_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid destination handle.",
		 function );

		return( -1 );
	}
	if( *destination_handle != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: destination handle already set.",
		 function );

		return( -1 );
	}
	if( source_handle == NULL )
	{
		*destination_handle = NULL;

		return( 1 );
	}
	internal_source_handle = (libbfio_internal_handle_t *) source_handle;

	if( internal_source_handle->io_handle != NULL )
	{
		if( ( internal_source_handle->flags & LIBBFIO_FLAG_IO_HANDLE_CLONE_BY_REFERENCE ) != 0 )
		{
			destination_io_handle = internal_source_handle->io_handle;

			destination_flags = LIBBFIO_FLAG_IO_HANDLE_CLONE_BY_REFERENCE;
		}
		else
		{
			if( internal_source_handle->clone_io_handle == NULL )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
				 "%s: invalid handle - missing clone IO handle function.",
				 function );

				goto on_error;
			}
			if( internal_source_handle->clone_io_handle(
			     &destination_io_handle,
			     internal_source_handle->io_handle,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
				 "%s: unable to clone IO handle.",
				 function );

				goto on_error;
			}
			destination_flags = LIBBFIO_FLAG_IO_HANDLE_MANAGED;
		}
	}
	if( libbfio_handle_initialize(
	     destination_handle,
	     destination_io_handle,
	     internal_source_handle->free_io_handle,
	     internal_source_handle->clone_io_handle,
	     internal_source_handle->open,
	     internal_source_handle->close,
	     internal_source_handle->read,
	     internal_source_handle->write,
	     internal_source_handle->seek_offset,
	     internal_source_handle->exists,
	     internal_source_handle->is_open,
	     internal_source_handle->get_size,
	     destination_flags,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create destination handle.",
		 function );

		goto on_error;
	}
	destination_io_handle = NULL;

	if( libbfio_handle_open(
	     *destination_handle,
	     internal_source_handle->access_flags,
	     error ) == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_OPEN_FAILED,
		 "%s: unable to open destination handle.",
		 function );

		goto on_error;
	}
	if( libbfio_handle_seek_offset(
	     *destination_handle,
	     internal_source_handle->offset,
	     SEEK_SET,
	     error ) == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_SEEK_FAILED,
		 "%s: unable to seek offset in destination handle.",
		 function );

		goto on_error;
	}
	return( 1 );

on_error:
	if( destination_io_handle != NULL )
	{
		if( internal_source_handle->free_io_handle == NULL )
		{
			memory_free(
			 destination_io_handle );
		}
		else
		{
			internal_source_handle->free_io_handle(
			 &destination_io_handle,
			 NULL );
		}
	}
	if( destination_handle != NULL )
	{
		libbfio_handle_free(
		 destination_handle,
		 NULL );
	}
	return( -1 );
}

/* Opens the handle
 * Returns 1 if successful or -1 on error
 */
int libbfio_handle_open(
     libbfio_handle_t *handle,
     int access_flags,
     libcerror_error_t **error )
{
	libbfio_internal_handle_t *internal_handle = NULL;
	static char *function                      = "libbfio_handle_open";

	if( handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid handle.",
		 function );

		return( -1 );
	}
	internal_handle = (libbfio_internal_handle_t *) handle;

	if( internal_handle->io_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid handle - missing IO handle.",
		 function );

		return( -1 );
	}
	if( internal_handle->open == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid handle - missing open function.",
		 function );

		return( -1 );
	}
	if( ( ( access_flags & LIBBFIO_ACCESS_FLAG_READ ) == 0 )
	 && ( ( access_flags & LIBBFIO_ACCESS_FLAG_WRITE ) == 0 ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_UNSUPPORTED_VALUE,
		 "%s: unsupported access flags: 0x%02x.",
		 function,
		 access_flags );

		return( -1 );
	}
	if( internal_handle->open_on_demand == 0 )
	{
		if( internal_handle->open(
		     internal_handle->io_handle,
		     access_flags,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_IO,
			 LIBCERROR_IO_ERROR_OPEN_FAILED,
			 "%s: unable to open handle.",
			 function );

			return( -1 );
		}
	}
	else if( ( access_flags & LIBBFIO_ACCESS_FLAG_WRITE ) != 0 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_UNSUPPORTED_VALUE,
		 "%s: open on demand cannot be used in combination with write access.",
		 function );

		return( -1 );
	}
	internal_handle->access_flags = access_flags;

	return( 1 );
}

/* Reopens the handle
 * Returns 1 if successful or -1 on error
 */
int libbfio_handle_reopen(
     libbfio_handle_t *handle,
     int access_flags,
     libcerror_error_t **error )
{
	libbfio_internal_handle_t *internal_handle = NULL;
	static char *function                      = "libbfio_handle_reopen";

	if( handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid handle.",
		 function );

		return( -1 );
	}
	internal_handle = (libbfio_internal_handle_t *) handle;

	if( internal_handle->io_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid handle - missing IO handle.",
		 function );

		return( -1 );
	}
	if( internal_handle->close == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid handle - missing close function.",
		 function );

		return( -1 );
	}
	if( internal_handle->open == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid handle - missing open function.",
		 function );

		return( -1 );
	}
	if( internal_handle->seek_offset == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid handle - missing seek offset function.",
		 function );

		return( -1 );
	}
	if( ( ( access_flags & LIBBFIO_ACCESS_FLAG_READ ) == 0 )
	 && ( ( access_flags & LIBBFIO_ACCESS_FLAG_WRITE ) == 0 ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_UNSUPPORTED_VALUE,
		 "%s: unsupported access flags.",
		 function );

		return( -1 );
	}
	/* Only reopen if the access flags have changed
	 */
	if( internal_handle->access_flags != access_flags )
	{
		if( internal_handle->close(
		     internal_handle->io_handle,
		     error ) != 0 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_IO,
			 LIBCERROR_IO_ERROR_CLOSE_FAILED,
			 "%s: unable to close handle.",
			 function );

			return( -1 );
		}
		if( internal_handle->open_on_demand == 0 )
		{
			if( internal_handle->open(
			     internal_handle->io_handle,
			     access_flags,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_IO,
				 LIBCERROR_IO_ERROR_OPEN_FAILED,
				 "%s: unable to open handle.",
				 function );

				return( -1 );
			}
		}
		internal_handle->access_flags = access_flags;

		if( internal_handle->open_on_demand == 0 )
		{
			/* Seek the previous file offset only when at least reading the file
			 */
			if( ( internal_handle->access_flags & LIBBFIO_ACCESS_FLAG_READ ) != 0 )
			{
				if( internal_handle->seek_offset(
				     internal_handle->io_handle,
				     internal_handle->offset,
				     SEEK_CUR,
				     error ) == -1 )
				{
					libcerror_error_set(
					 error,
					 LIBCERROR_ERROR_DOMAIN_IO,
					 LIBCERROR_IO_ERROR_SEEK_FAILED,
					 "%s: unable to seek offset in handle.",
					 function );

					return( -1 );
				}
			}
		}
	}
	return( 1 );
}

/* Closes the handle
 * Returns 0 if successful or -1 on error
 */
int libbfio_handle_close(
     libbfio_handle_t *handle,
     libcerror_error_t **error )
{
	libbfio_internal_handle_t *internal_handle = NULL;
	static char *function                      = "libbfio_handle_close";
	int is_open                                = 0;

	if( handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid handle.",
		 function );

		return( -1 );
	}
	internal_handle = (libbfio_internal_handle_t *) handle;

	if( internal_handle->io_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid handle - missing IO handle.",
		 function );

		return( -1 );
	}
	if( internal_handle->close == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid handle - missing close function.",
		 function );

		return( -1 );
	}
	if( internal_handle->open_on_demand != 0 )
	{
		if( internal_handle->is_open == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
			 "%s: invalid handle - missing is open function.",
			 function );

			return( -1 );
		}
		is_open = internal_handle->is_open(
			   internal_handle->io_handle,
		           error );

		if( is_open == -1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_IO,
			 LIBCERROR_IO_ERROR_OPEN_FAILED,
			 "%s: unable to determine if handle is open.",
			 function );

			return( -1 );
		}
		else if( is_open == 0 )
		{
			return( 0 );
		}
	}
	if( internal_handle->close(
	     internal_handle->io_handle,
	     error ) != 0 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_CLOSE_FAILED,
		 "%s: unable to close handle.",
		 function );

		return( -1 );
	}
	return( 0 );
}

/* Reads a buffer from the handle
 * Returns the number of bytes read if successful, or -1 on error
 */
ssize_t libbfio_handle_read_buffer(
         libbfio_handle_t *handle,
         uint8_t *buffer,
         size_t size,
         libcerror_error_t **error )
{
	libbfio_internal_handle_t *internal_handle = NULL;
	static char *function                      = "libbfio_handle_read_buffer";
	ssize_t read_count                         = 0;
	int is_open                                = 0;

	if( handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid handle.",
		 function );

		return( -1 );
	}
	internal_handle = (libbfio_internal_handle_t *) handle;

	if( internal_handle->io_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid handle - missing IO handle.",
		 function );

		return( -1 );
	}
	if( internal_handle->read == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid handle - missing read function.",
		 function );

		return( -1 );
	}
	if( size > (size_t) SSIZE_MAX )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_EXCEEDS_MAXIMUM,
		 "%s: invalid size value exceeds maximum.",
		 function );

		return( -1 );
	}
	if( internal_handle->open_on_demand != 0 )
	{
		if( internal_handle->is_open == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
			 "%s: invalid handle - missing is open function.",
			 function );

			return( -1 );
		}
		if( internal_handle->open == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
			 "%s: invalid handle - missing open function.",
			 function );

			return( -1 );
		}
		if( internal_handle->seek_offset == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
			 "%s: invalid handle - missing seek offset function.",
			 function );

			return( -1 );
		}
		is_open = internal_handle->is_open(
			   internal_handle->io_handle,
		           error );

		if( is_open == -1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_IO,
			 LIBCERROR_IO_ERROR_OPEN_FAILED,
			 "%s: unable to determine if handle is open.",
			 function );

			return( -1 );
		}
		else if( is_open == 0 )
		{
			if( internal_handle->open(
			     internal_handle->io_handle,
			     internal_handle->access_flags,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_IO,
				 LIBCERROR_IO_ERROR_OPEN_FAILED,
				 "%s: unable to open handle on demand.",
				 function );

				return( -1 );
			}
			if( internal_handle->seek_offset(
			     internal_handle->io_handle,
			     internal_handle->offset,
			     SEEK_SET,
			     error ) == -1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_IO,
				 LIBCERROR_IO_ERROR_SEEK_FAILED,
				 "%s: unable to find current offset: %" PRIi64 " in handle.",
				 function,
				 internal_handle->offset );

				return( -1 );
			}
		}
	}
	read_count = internal_handle->read(
	              internal_handle->io_handle,
	              buffer,
	              size,
	              error );

	if( read_count < 0 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_READ_FAILED,
		 "%s: unable to read from handle.",
		 function );

		return( -1 );
	}
	if( internal_handle->track_offsets_read != 0 )
	{
		if( libbfio_offset_list_append_offset(
		     internal_handle->offsets_read,
		     internal_handle->offset,
		     read_count,
		     1,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_APPEND_FAILED,
			 "%s: unable to append offset range to offsets read table.",
			 function );

			return( -1 );
		}
	}
	internal_handle->offset += (off64_t) read_count;

	if( internal_handle->open_on_demand != 0 )
	{
		if( internal_handle->close == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
			 "%s: invalid handle - missing close function.",
			 function );

			return( -1 );
		}
		if( internal_handle->close(
		     internal_handle->io_handle,
		     error ) != 0 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_IO,
			 LIBCERROR_IO_ERROR_OPEN_FAILED,
			 "%s: unable to close handle on demand.",
			 function );

			return( -1 );
		}
	}
	return( read_count );
}

/* Writes a buffer to the handle
 * Returns the number of bytes written if successful, or -1 on error
 */
ssize_t libbfio_handle_write_buffer(
         libbfio_handle_t *handle,
         const uint8_t *buffer,
         size_t size,
         libcerror_error_t **error )
{
	libbfio_internal_handle_t *internal_handle = NULL;
	static char *function                      = "libbfio_handle_write_buffer";
	ssize_t write_count                        = 0;

	if( handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid handle.",
		 function );

		return( -1 );
	}
	internal_handle = (libbfio_internal_handle_t *) handle;

	if( internal_handle->io_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid handle - missing IO handle.",
		 function );

		return( -1 );
	}
	if( internal_handle->write == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid handle - missing write function.",
		 function );

		return( -1 );
	}
	if( size > (size_t) SSIZE_MAX )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_EXCEEDS_MAXIMUM,
		 "%s: invalid size value exceeds maximum.",
		 function );

		return( -1 );
	}
	write_count = internal_handle->write(
	               internal_handle->io_handle,
	               buffer,
	               size,
	               error );

	if( write_count < 0 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_WRITE_FAILED,
		 "%s: unable to write to handle.",
		 function );

		return( -1 );
	}
	internal_handle->offset += (off64_t) write_count;

	if( (size64_t) internal_handle->offset > internal_handle->size )
	{
		internal_handle->size = (size64_t) internal_handle->offset;
	}
	return( write_count );
}

/* Seeks a certain offset within the handle
 * Returns the offset if the seek is successful or -1 on error
 */
off64_t libbfio_handle_seek_offset(
         libbfio_handle_t *handle,
         off64_t offset,
         int whence,
         libcerror_error_t **error )
{
	libbfio_internal_handle_t *internal_handle = NULL;
	static char *function                      = "libbfio_handle_seek_offset";
	int is_open                                = 0;

	if( handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid handle.",
		 function );

		return( -1 );
	}
	internal_handle = (libbfio_internal_handle_t *) handle;

	if( internal_handle->io_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid handle - missing IO handle.",
		 function );

		return( -1 );
	}
	if( internal_handle->seek_offset == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid handle - missing seek offset function.",
		 function );

		return( -1 );
	}
	if( ( whence != SEEK_CUR )
	 && ( whence != SEEK_END )
	 && ( whence != SEEK_SET ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_UNSUPPORTED_VALUE,
		 "%s: unsupported whence.",
		 function );

		return( -1 );
	}
	if( internal_handle->open_on_demand != 0 )
	{
		if( internal_handle->is_open == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
			 "%s: invalid handle - missing is open function.",
			 function );

			return( -1 );
		}
		if( internal_handle->open == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
			 "%s: invalid handle - missing open function.",
			 function );

			return( -1 );
		}
		is_open = internal_handle->is_open(
			   internal_handle->io_handle,
			   error );

		if( is_open == -1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_IO,
			 LIBCERROR_IO_ERROR_OPEN_FAILED,
			 "%s: unable to determine if handle is open.",
			 function );

			return( -1 );
		}
		else if( is_open == 0 )
		{
			if( internal_handle->open(
			     internal_handle->io_handle,
			     internal_handle->access_flags,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_IO,
				 LIBCERROR_IO_ERROR_OPEN_FAILED,
				 "%s: unable to open handle on demand.",
				 function );

				return( -1 );
			}
			if( internal_handle->seek_offset(
			     internal_handle->io_handle,
			     internal_handle->offset,
			     SEEK_SET,
			     error ) == -1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_IO,
				 LIBCERROR_IO_ERROR_SEEK_FAILED,
				 "%s: unable to find current offset: %" PRIi64 " in handle.",
				 function,
				 internal_handle->offset );

				return( -1 );
			}
		}
	}
	offset = internal_handle->seek_offset(
		  internal_handle->io_handle,
		  offset,
		  whence,
		  error );

	if( offset == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_SEEK_FAILED,
		 "%s: unable to find offset: %" PRIi64 " in handle.",
		 function,
		 offset );

		return( -1 );
	}
	internal_handle->offset = offset;

	return( offset );
}

/* Function to determine if a file object exists
 * Returns 1 if file object exists, 0 if not or -1 on error
 */
int libbfio_handle_exists(
     libbfio_handle_t *handle,
     libcerror_error_t **error )
{
	libbfio_internal_handle_t *internal_handle = NULL;
	static char *function                      = "libbfio_handle_exists";
	int result                                 = 0;

	if( handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid handle.",
		 function );

		return( -1 );
	}
	internal_handle = (libbfio_internal_handle_t *) handle;

	if( internal_handle->io_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid handle - missing IO handle.",
		 function );

		return( -1 );
	}
	if( internal_handle->exists == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid handle - missing exists function.",
		 function );

		return( -1 );
	}
	result = internal_handle->exists(
	          internal_handle->io_handle,
	          error );

	if( result == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to determine if handle exists.",
		 function );

		return( -1 );
	}
	return( result );
}

/* Check if the handle is open
 * Returns 1 if open, 0 if not or -1 on error
 */
int libbfio_handle_is_open(
     libbfio_handle_t *handle,
     libcerror_error_t **error )
{
	libbfio_internal_handle_t *internal_handle = NULL;
	static char *function                      = "libbfio_handle_is_open";
	int result                                 = 0;

	if( handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid handle.",
		 function );

		return( -1 );
	}
	internal_handle = (libbfio_internal_handle_t *) handle;

	if( internal_handle->io_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid handle - missing IO handle.",
		 function );

		return( -1 );
	}
	if( internal_handle->is_open == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid handle - missing is open function.",
		 function );

		return( -1 );
	}
	result = internal_handle->is_open(
	          internal_handle->io_handle,
	          error );

	if( result == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to determine if handle is open.",
		 function );

		return( -1 );
	}
	return( result );
}

/* Retrieves the IO handle
 * Returns 1 if successful or -1 on error
 */
int libbfio_handle_get_io_handle(
     libbfio_handle_t *handle,
     intptr_t **io_handle,
     libcerror_error_t **error )
{
	libbfio_internal_handle_t *internal_handle = NULL;
	static char *function                      = "libbfio_handle_get_io_handle";

	if( handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid handle.",
		 function );

		return( -1 );
	}
	internal_handle = (libbfio_internal_handle_t *) handle;

	if( io_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid IO handle.",
		 function );

		return( -1 );
	}
	*io_handle = internal_handle->io_handle;

	return( 1 );
}

/* Retrieves the access flags
 * Returns 1 if successful or -1 on error
 */
int libbfio_handle_get_access_flags(
     libbfio_handle_t *handle,
     int *access_flags,
     libcerror_error_t **error )
{
	libbfio_internal_handle_t *internal_handle = NULL;
	static char *function                      = "libbfio_handle_get_access_flags";

	if( handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid handle.",
		 function );

		return( -1 );
	}
	internal_handle = (libbfio_internal_handle_t *) handle;

	if( access_flags == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid access flags.",
		 function );

		return( -1 );
	}
	*access_flags = internal_handle->access_flags;

	return( 1 );
}

/* Sets the access flags
 * Returns 1 if successful or -1 on error
 */
int libbfio_handle_set_access_flags(
     libbfio_handle_t *handle,
     int access_flags,
     libcerror_error_t **error )
{
	libbfio_internal_handle_t *internal_handle = NULL;
	static char *function                      = "libbfio_handle_set_access_flags";

	if( handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid handle.",
		 function );

		return( -1 );
	}
	internal_handle = (libbfio_internal_handle_t *) handle;

	if( ( ( access_flags & LIBBFIO_ACCESS_FLAG_READ ) == 0 )
	 && ( ( access_flags & LIBBFIO_ACCESS_FLAG_WRITE ) == 0 ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_UNSUPPORTED_VALUE,
		 "%s: unsupported access flags.",
		 function );

		return( -1 );
	}
	internal_handle->access_flags = access_flags;

	return( 1 );
}

/* Returns the size of the data of the handle
 * Returns 1 if successful or -1 on error
 */
int libbfio_handle_get_size(
     libbfio_handle_t *handle,
     size64_t *size,
     libcerror_error_t **error )
{
	libbfio_internal_handle_t *internal_handle = NULL;
	static char *function                      = "libbfio_handle_get_size";

	if( handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid handle.",
		 function );

		return( -1 );
	}
	internal_handle = (libbfio_internal_handle_t *) handle;

	if( internal_handle->io_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid handle - missing IO handle.",
		 function );

		return( -1 );
	}
	if( size == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid size",
		 function );

		return( -1 );
	}
	if( internal_handle->size_set == 0 )
	{
		if( internal_handle->get_size == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
			 "%s: invalid handle - missing get size function.",
			 function );

			return( -1 );
		}
		if( internal_handle->get_size(
		     internal_handle->io_handle,
		     &( internal_handle->size ),
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve size.",
			 function );

			return( -1 );
		}
		internal_handle->size_set = 1;
	}
	*size = internal_handle->size;

	return( 1 );
}

/* Retrieves the current offset
 * Returns 1 if successful or -1 on error
 */
int libbfio_handle_get_offset(
     libbfio_handle_t *handle,
     off64_t *offset,
     libcerror_error_t **error )
{
	libbfio_internal_handle_t *internal_handle = NULL;
	static char *function                      = "libbfio_handle_get_offset";

	if( handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid handle.",
		 function );

		return( -1 );
	}
	internal_handle = (libbfio_internal_handle_t *) handle;

	if( offset == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid offset.",
		 function );

		return( -1 );
	}
	*offset = internal_handle->offset;

	return( 1 );
}

/* Sets the value to have the library open and close 
 * the systems file descriptor or handle on demand
 * 0 disables open on demand any other value enables it
 * Returns 1 if successful or -1 on error
 */
int libbfio_handle_set_open_on_demand(
     libbfio_handle_t *handle,
     uint8_t open_on_demand,
     libcerror_error_t **error )
{
	libbfio_internal_handle_t *internal_handle = NULL;
	static char *function                      = "libbfio_handle_set_open_on_demand";

	if( handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid handle.",
		 function );

		return( -1 );
	}
	internal_handle = (libbfio_internal_handle_t *) handle;

	if( ( ( internal_handle->access_flags & LIBBFIO_ACCESS_FLAG_WRITE ) != 0 )
	 && ( open_on_demand != 0 ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_UNSUPPORTED_VALUE,
		 "%s: open on demand cannot be used in combination with write access.",
		 function );

		return( -1 );
	}
	internal_handle->open_on_demand = open_on_demand;

	return( 1 );
}

/* Sets the value to have the library track the offsets read
 * 0 disables tracking any other value enables it
 * Returns 1 if successful or -1 on error
 */
int libbfio_handle_set_track_offsets_read(
     libbfio_handle_t *handle,
     uint8_t track_offsets_read,
     libcerror_error_t **error )
{
	libbfio_internal_handle_t *internal_handle = NULL;
	static char *function                      = "libbfio_handle_set_track_offsets_read";

	if( handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid handle.",
		 function );

		return( -1 );
	}
	internal_handle = (libbfio_internal_handle_t *) handle;

	internal_handle->track_offsets_read = track_offsets_read;

	return( 1 );
}

/* Retrieves the number of offsets read
 * Returns 1 if successful or -1 on error
 */
int libbfio_handle_get_number_of_offsets_read(
     libbfio_handle_t *handle,
     int *number_of_read_offsets,
     libcerror_error_t **error )
{
	libbfio_internal_handle_t *internal_handle = NULL;
	static char *function                      = "libbfio_handle_get_number_of_offsets_read";

	if( handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid handle.",
		 function );

		return( -1 );
	}
	internal_handle = (libbfio_internal_handle_t *) handle;

	if( libbfio_offset_list_get_number_of_elements(
	     internal_handle->offsets_read,
	     number_of_read_offsets,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve number of read offsets.",
		 function );

		return( -1 );
	}
	return( 1 );
}

/* Retrieves the information of an offset read
 * Returns 1 if successful or -1 on error
 */
int libbfio_handle_get_offset_read(
     libbfio_handle_t *handle,
     int index,
     off64_t *offset,
     size64_t *size,
     libcerror_error_t **error )
{
	libbfio_internal_handle_t *internal_handle = NULL;
	static char *function                      = "libbfio_handle_get_offset_read";

	if( handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid handle.",
		 function );

		return( -1 );
	}
	internal_handle = (libbfio_internal_handle_t *) handle;

	if( libbfio_offset_list_get_offset(
	     internal_handle->offsets_read,
	     index,
	     offset,
	     size,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve read offset.",
		 function );

		return( -1 );
	}
	return( 1 );
}

