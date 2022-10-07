/*
 * Data block functions
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

#include <common.h>
#include <byte_stream.h>
#include <memory.h>
#include <types.h>

#include "libpff_definitions.h"
#include "libpff_data_block.h"
#include "libpff_encryption.h"
#include "libpff_io_handle.h"
#include "libpff_libbfio.h"
#include "libpff_libcerror.h"
#include "libpff_libcnotify.h"
#include "libpff_libfmapi.h"

#include "pff_block.h"

/* Initializes the data block and its values
 * Returns 1 if successful or -1 on error
 */
int libpff_data_block_initialize(
     libpff_data_block_t **data_block,
     uint32_t descriptor_identifier,
     uint64_t data_identifier,
     libpff_io_handle_t *io_handle,
     libcerror_error_t **error )
{
	static char *function = "libpff_data_block_initialize";

	if( data_block == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid data block.",
		 function );

		return( -1 );
	}
	if( *data_block != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid data block value already set.",
		 function );

		return( -1 );
	}
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
	*data_block = memory_allocate_structure(
	               libpff_data_block_t );

	if( *data_block == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create data block.",
		 function );

		goto on_error;
	}
	if( memory_set(
	     *data_block,
	     0,
	     sizeof( libpff_data_block_t ) ) == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_SET_FAILED,
		 "%s: unable to clear data block.",
		 function );

		goto on_error;
	}
	( *data_block )->descriptor_identifier = descriptor_identifier;
	( *data_block )->data_identifier       = data_identifier;
	( *data_block )->io_handle             = io_handle;

	return( 1 );

on_error:
	if( *data_block != NULL )
	{
		memory_free(
		 *data_block );

		*data_block = NULL;
	}
	return( -1 );
}

/* Frees the block and its values
 * Returns 1 if successful or -1 on error
 */
int libpff_data_block_free(
     libpff_data_block_t **data_block,
     libcerror_error_t **error )
{
	static char *function = "libpff_data_block_free";

	if( data_block == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid data block.",
		 function );

		return( -1 );
	}
	if( *data_block != NULL )
	{
		memory_free(
		 *data_block );

		*data_block = NULL;
	}
	return( 1 );
}

/* Clones the data block
 * Returns 1 if successful or -1 on error
 */
int libpff_data_block_clone(
     libpff_data_block_t **destination_data_block,
     libpff_data_block_t *source_data_block,
     libcerror_error_t **error )
{
	static char *function = "libpff_data_block_clone";

	if( destination_data_block == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid destination data block.",
		 function );

		return( -1 );
	}
	if( *destination_data_block != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid destination data block already set.",
		 function );

		return( -1 );
	}
	if( source_data_block == NULL )
	{
		*destination_data_block = NULL;

		return( 1 );
	}
	if( libpff_data_block_initialize(
	     destination_data_block,
	     source_data_block->descriptor_identifier,
	     source_data_block->data_identifier,
	     source_data_block->io_handle,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create destination data block.",
		 function );

		return( -1 );
	}
	if( *destination_data_block == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: missing destination data block.",
		 function );

		return( -1 );
	}
	( *destination_data_block )->flags = source_data_block->flags;

	return( 1 );
}

/* Reads a data block data
 * Returns the number of bytes read if successful or -1 on error
 */
ssize_t libpff_data_block_read_data(
         intptr_t *io_handle,
         libbfio_handle_t *file_io_handle,
         int segment_index,
         uint8_t *data,
         size_t data_size,
         uint8_t flags,
         libcerror_error_t **error )
{
	libpff_data_block_t *data_block = NULL;
	static char *function           = "libpff_data_block_read_entry_data";
	ssize_t read_count              = 0;

	if( io_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid data block IO handle.",
		 function );

		return( -1 );
	}
	data_block = (libpff_data_block_t *) io_handle;

	if( data_block->io_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid data block - missing IO handle.",
		 function );

		return( -1 );
	}
	if( segment_index < 0 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: invalid segment index value out of bounds.",
		 function );

		return( -1 );
	}
	read_count = libpff_data_block_read(
	              file_io_handle,
	              data_block->io_handle->file_type,
	              data_block->data_identifier,
	              data,
	              data_size,
	              &( data_block->flags ),
	              error );

	if( read_count != (ssize_t) data_size )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_READ_FAILED,
		 "%s: unable to read data block data.",
		 function );

		return( -1 );
	}
	if( libpff_data_block_decrypt_data(
	     data_block,
	     data_block->io_handle->encryption_type,
	     data,
	     data_size,
	     flags,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ENCRYPTION,
		 LIBCERROR_ENCRYPTION_ERROR_DECRYPT_FAILED,
		 "%s: unable to decrypt data block data.",
		 function );

		return( -1 );
	}
	return( (ssize_t) data_size );
}

/* Reads the data block
 * Returns the number of bytes read if successful or -1 on error
 */
ssize_t libpff_data_block_read(
         libbfio_handle_t *file_io_handle,
         uint8_t file_type,
         uint64_t data_identifier,
         uint8_t *data,
         size_t data_size,
         uint8_t *flags,
         libcerror_error_t **error )
{
	uint8_t *data_block_data         = NULL;
	uint8_t *data_block_footer       = NULL;
	static char *function            = "libpff_data_block_read";
	size_t data_block_size           = 0;
	size_t data_block_footer_size    = 0;
	size_t data_block_padding_size   = 0;
	ssize_t read_count               = 0;
	uint64_t data_block_back_pointer = 0;
	uint32_t calculated_crc          = 0;
	uint32_t stored_crc              = 0;
	uint16_t data_block_data_size    = 0;
	uint16_t data_block_signature    = 0;

	if( ( file_type != LIBPFF_FILE_TYPE_32BIT )
	 && ( file_type != LIBPFF_FILE_TYPE_64BIT ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_UNSUPPORTED_VALUE,
		 "%s: unsupported file type.",
		 function );

		return( -1 );
	}
	if( data == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid data.",
		 function );

		return( -1 );
	}
	if( data_size > (size_t) SSIZE_MAX )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_EXCEEDS_MAXIMUM,
		 "%s: data size value exceeds maximum.",
		 function );

		return( -1 );
	}
	if( flags == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid data.",
		 function );

		return( -1 );
	}
	if( file_type == LIBPFF_FILE_TYPE_32BIT )
	{
		data_block_footer_size = sizeof( pff_block_footer_32bit_t );
	}
	else if( file_type == LIBPFF_FILE_TYPE_64BIT )
	{
		data_block_footer_size = sizeof( pff_block_footer_64bit_t );
	}
	if( data_size > 0 )
	{
		read_count = libbfio_handle_read_buffer(
		              file_io_handle,
		              data,
		              data_size,
		              error );

		if( read_count != (ssize_t) data_size )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_IO,
			 LIBCERROR_IO_ERROR_READ_FAILED,
			 "%s: unable to read data block data.",
			 function );

			return( -1 );
		}
	}
	if( ( *flags & LIBPFF_DATA_BLOCK_FLAG_VALIDATED ) == 0 )
	{
		if( data_size > 0 )
		{
			data_block_size = data_size / 64;

			if( ( data_size % 64 ) != 0 )
			{
				data_block_size += 1;
			}
			data_block_size *= 64;

			if( ( data_block_size - data_size ) < data_block_footer_size )
			{
				data_block_size += 64;
			}
			data_block_padding_size = data_block_size - data_size;

			data_block_data = (uint8_t *) memory_allocate(
						       sizeof( uint8_t ) * data_block_padding_size );

			if( data_block_data == NULL )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_MEMORY,
				 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
				 "%s: unable to create remaining data block data.",
				 function );

				return( -1 );
			}
			read_count = libbfio_handle_read_buffer(
				      file_io_handle,
				      data_block_data,
				      data_block_padding_size,
				      error );

			if( read_count != (ssize_t) data_block_padding_size )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_IO,
				 LIBCERROR_IO_ERROR_READ_FAILED,
				 "%s: unable to read remaining data block data.",
				 function );

				memory_free(
				 data_block_data );

				return( -1 );
			}
			data_block_padding_size -= data_block_footer_size;

			data_block_footer = &( data_block_data[ data_block_padding_size ] );

#if defined( HAVE_DEBUG_OUTPUT )
			if( libcnotify_verbose != 0 )
			{
				libcnotify_printf(
				 "%s: data block padding size\t: %" PRIzd "\n",
				 function,
				 data_block_padding_size );
				libcnotify_printf(
				 "%s: data block padding:\n",
				 function );
				libcnotify_print_data(
				 data_block_data,
				 data_block_padding_size,
				 0 );

				libcnotify_printf(
				 "%s: data block footer:\n",
				 function );
				libcnotify_print_data(
				 data_block_footer,
				 data_block_footer_size,
				 0 );
			}
#endif
			if( file_type == LIBPFF_FILE_TYPE_32BIT )
			{
				byte_stream_copy_to_uint16_little_endian(
				 ( (pff_block_footer_32bit_t *) data_block_footer )->size,
				 data_block_data_size );

				byte_stream_copy_to_uint16_little_endian(
				 ( (pff_block_footer_32bit_t *) data_block_footer )->signature,
				 data_block_signature );

				byte_stream_copy_to_uint32_little_endian(
				 ( (pff_block_footer_32bit_t *) data_block_footer )->back_pointer,
				 data_block_back_pointer );

				byte_stream_copy_to_uint32_little_endian(
				 ( (pff_block_footer_32bit_t *) data_block_footer )->crc,
				 stored_crc );
			}
			else if( file_type == LIBPFF_FILE_TYPE_64BIT )
			{
				byte_stream_copy_to_uint16_little_endian(
				 ( (pff_block_footer_64bit_t *) data_block_footer )->size,
				 data_block_data_size );

				byte_stream_copy_to_uint16_little_endian(
				 ( (pff_block_footer_64bit_t *) data_block_footer )->signature,
				 data_block_signature );

				byte_stream_copy_to_uint32_little_endian(
				 ( (pff_block_footer_64bit_t *) data_block_footer )->crc,
				 stored_crc );

				byte_stream_copy_to_uint64_little_endian(
				 ( (pff_block_footer_64bit_t *) data_block_footer )->back_pointer,
				 data_block_back_pointer );
			}
#if defined( HAVE_DEBUG_OUTPUT )
			if( libcnotify_verbose != 0 )
			{
				libcnotify_printf(
				 "%s: data block data size\t: %" PRIu16 "\n",
				 function,
				 data_block_data_size );
				libcnotify_printf(
				 "%s: data block signature\t: 0x%04" PRIx16 "\n",
				 function,
				 data_block_signature );

				if( file_type == LIBPFF_FILE_TYPE_32BIT )
				{
					libcnotify_printf(
					 "%s: data block back pointer\t: 0x%08" PRIx64 "\n",
					 function,
					 data_block_back_pointer );
					libcnotify_printf(
					 "%s: data block data crc\t: 0x%08" PRIx32 "\n",
					 function,
					 stored_crc );
				}
				else if( file_type == LIBPFF_FILE_TYPE_64BIT )
				{
					libcnotify_printf(
					 "%s: data block data crc\t: 0x%08" PRIx32 "\n",
					 function,
					 stored_crc );
					libcnotify_printf(
					 "%s: data block back pointer\t: 0x%08" PRIx64 "\n",
					 function,
					 data_block_back_pointer );
				}
				libcnotify_printf(
				 "\n" );
			}
#endif
			memory_free(
			 data_block_data );

			if( data_block_data_size != 0 )
			{
				if( data_size != (size_t) data_block_data_size )
				{
					/* TODO flag size mismatch and error tollerance */
					*flags |= LIBPFF_DATA_BLOCK_FLAG_SIZE_MISMATCH;

					libcerror_error_set(
					 error,
					 LIBCERROR_ERROR_DOMAIN_INPUT,
					 LIBCERROR_INPUT_ERROR_VALUE_MISMATCH,
					 "%s: mismatch in data size ( %" PRIzd " != %" PRIu16 " ).",
					 function,
					 data_size,
					 data_block_data_size );

					return( -1 );
				}
			}
			if( stored_crc != 0 )
			{
				if( libfmapi_crc32_weak_calculate(
				     &calculated_crc,
				     data,
				     data_size,
				     0,
				     error ) != 1 )
				{
					libcerror_error_set(
					 error,
					 LIBCERROR_ERROR_DOMAIN_RUNTIME,
					 LIBCERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
					 "%s: unable to calculate CRC-32.",
					 function );

					return( -1 );
				}
				if( stored_crc != calculated_crc )
				{
#if defined( HAVE_DEBUG_OUTPUT )
					if( libcnotify_verbose != 0 )
					{
						libcnotify_printf(
						 "%s: mismatch in data block CRC-32 ( %" PRIu32 " != %" PRIu32 " ).\n",
						 function,
						 stored_crc,
						 calculated_crc );
					}
#endif
					*flags |= LIBPFF_DATA_BLOCK_FLAG_CRC_MISMATCH;

					/* TODO smart error handling */
				}
			}
			if( data_block_back_pointer != 0 )
			{
				if( data_identifier != data_block_back_pointer )
				{
#if defined( HAVE_DEBUG_OUTPUT )
					if( libcnotify_verbose != 0 )
					{
						libcnotify_printf(
						 "%s: mismatch in data identifier: %" PRIu64 " and back pointer: 0x%08" PRIx64 ".\n",
						 function,
						 data_identifier,
						 data_block_back_pointer );
					}
#endif
					*flags |= LIBPFF_DATA_BLOCK_FLAG_IDENTIFIER_MISMATCH;

					/* TODO smart error handling */
				}
			}
		}
		*flags |= LIBPFF_DATA_BLOCK_FLAG_VALIDATED;
	}
	return( (ssize_t) data_size );
}

/* Decrypts the data block data
 * Returns 1 if successful or -1 on error
 */
int libpff_data_block_decrypt_data(
     libpff_data_block_t *data_block,
     uint8_t encryption_type,
     uint8_t *data,
     size_t data_size,
     uint8_t flags,
     libcerror_error_t **error )
{
	static char *function        = "libpff_data_block_decrypt_data";
	ssize_t process_count        = 0;
	uint8_t decrypt_data         = 0;
	uint8_t force_decryption     = 0;
	uint8_t node_identifier_type = 0;
	uint8_t node_contains_table  = 0;

	if( data_block == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid data block.",
		 function );

		return( -1 );
	}
	if( data == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid data.",
		 function );

		return( -1 );
	}
	if( data_size > (size_t) SSIZE_MAX )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_EXCEEDS_MAXIMUM,
		 "%s: data size value exceeds maximum.",
		 function );

		return( -1 );
	}
	/* Check if internal (unencrypted) flag in the (data) offset index identifier is not set
	 */
	if( ( data_block->data_identifier & LIBPFF_OFFSET_INDEX_IDENTIFIER_FLAG_INTERNAL ) == 0 )
	{
		decrypt_data = 1;
	}
	/* Check if data is encrypted
	 * Some 'invalid' files have an encryption type of none but contain encrypted data
	 * Although they are considered invalid by Outlook it is still possilble to read them
	 */
	if( ( encryption_type == LIBPFF_ENCRYPTION_TYPE_NONE )
	 && ( ( flags & LIBPFF_READ_FLAG_IGNORE_FORCE_DECRYPTION ) == 0 )
	 && ( data_size > 4 ) )
	{
		node_identifier_type = (uint8_t) ( data_block->descriptor_identifier & 0x0000001fUL );

		/* Check if the node identifier type consists of a table
		 */
		if( ( ( node_identifier_type == LIBPFF_NODE_IDENTIFIER_TYPE_INTERNAL )
		  && ( ( data_block->descriptor_identifier == LIBPFF_DESCRIPTOR_IDENTIFIER_MESSAGE_STORE )
		    || ( data_block->descriptor_identifier == LIBPFF_DESCRIPTOR_IDENTIFIER_NAME_TO_ID_MAP )
		    || ( data_block->descriptor_identifier == LIBPFF_DESCRIPTOR_IDENTIFIER_UNKNOWN_2049 )
		    || ( data_block->descriptor_identifier == LIBPFF_DESCRIPTOR_IDENTIFIER_UNKNOWN_2081 )
		    || ( data_block->descriptor_identifier == LIBPFF_DESCRIPTOR_IDENTIFIER_UNKNOWN_2113 )
		    || ( data_block->descriptor_identifier == LIBPFF_DESCRIPTOR_IDENTIFIER_UNKNOWN_3073 ) ) )
		 || ( node_identifier_type == LIBPFF_NODE_IDENTIFIER_TYPE_FOLDER )
		 || ( node_identifier_type == LIBPFF_NODE_IDENTIFIER_TYPE_SEARCH_FOLDER )
		 || ( node_identifier_type == LIBPFF_NODE_IDENTIFIER_TYPE_MESSAGE )
		 || ( node_identifier_type == LIBPFF_NODE_IDENTIFIER_TYPE_ASSOCIATED_CONTENT )
		 || ( node_identifier_type == LIBPFF_NODE_IDENTIFIER_TYPE_SUB_FOLDERS )
		 || ( node_identifier_type == LIBPFF_NODE_IDENTIFIER_TYPE_SUB_MESSAGES )
		 || ( node_identifier_type == LIBPFF_NODE_IDENTIFIER_TYPE_SUB_ASSOCIATED_CONTENTS )
		 || ( node_identifier_type == LIBPFF_NODE_IDENTIFIER_TYPE_SEARCH_CONTENTS_TABLE )
		 || ( node_identifier_type == LIBPFF_NODE_IDENTIFIER_TYPE_ATTACHMENTS )
		 || ( node_identifier_type == LIBPFF_NODE_IDENTIFIER_TYPE_RECIPIENTS )
		 || ( node_identifier_type == LIBPFF_NODE_IDENTIFIER_TYPE_UNKNOWN_1718 )
		 || ( node_identifier_type == LIBPFF_NODE_IDENTIFIER_TYPE_UNKNOWN_1751 )
		 || ( node_identifier_type == LIBPFF_NODE_IDENTIFIER_TYPE_UNKNOWN_1784 ) )
		{
			node_contains_table = 1;
		}
		if( node_contains_table != 0 )
		{
			/* Test if the data contains an unencrypted table
			 * a table consists of 0xec in the third byte
			 * and 0x6c, 0x7c, 0x8c, 0x9c, 0xa5, 0xac, 0xbc, 0xcc in the fourth
			 */
			if( ( data[ 2 ] != 0xec )
			 || ( ( data[ 3 ] != 0x6c )
			  &&  ( data[ 3 ] != 0x7c )
			  &&  ( data[ 3 ] != 0x8c )
			  &&  ( data[ 3 ] != 0x9c )
			  &&  ( data[ 3 ] != 0xa5 )
			  &&  ( data[ 3 ] != 0xac )
			  &&  ( data[ 3 ] != 0xbc )
			  &&  ( data[ 3 ] != 0xcc ) ) )
			{
#if defined( HAVE_DEBUG_OUTPUT )
				if( libcnotify_verbose != 0 )
				{
					libcnotify_printf(
					 "%s: table signature missing trying to force decryption.\n",
					 function );
				}
#endif
				force_decryption   = 1;
				encryption_type    = LIBPFF_ENCRYPTION_TYPE_COMPRESSIBLE;
				decrypt_data       = 1;
				data_block->flags |= LIBPFF_DATA_BLOCK_FLAG_DECRYPTION_FORCED;
			}
		}
		else if( data_block->io_handle->force_decryption == 1 )
		{
			/* Check if the internal (unencrypted) flag in (data) offset index identifier is not set
			 */
			if( ( data_block->data_identifier & LIBPFF_OFFSET_INDEX_IDENTIFIER_FLAG_INTERNAL ) == 0 )
			{
#if defined( HAVE_DEBUG_OUTPUT )
				if( libcnotify_verbose != 0 )
				{
					libcnotify_printf(
					 "%s: decryption forced.\n",
					 function );
				}
#endif
				encryption_type    = LIBPFF_ENCRYPTION_TYPE_COMPRESSIBLE;
				decrypt_data       = 1;
				data_block->flags |= LIBPFF_DATA_BLOCK_FLAG_DECRYPTION_FORCED;
			}
		}
	}
	if( decrypt_data != 0 )
	{
		process_count = libpff_encryption_decrypt(
				 encryption_type,
				 (uint32_t) data_block->data_identifier,
				 data,
				 data_size,
				 error );

		if( process_count != (ssize_t) data_size )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_ENCRYPTION,
			 LIBCERROR_ENCRYPTION_ERROR_DECRYPT_FAILED,
			 "%s: unable to decrypt data block data.",
			 function );

			return( -1 );
		}
		if( force_decryption != 0 )
		{
			/* Test if the data contains an unencrypted table
			 * a table consists of 0xec in the third byte
			 * and 0x6c, 0x7c, 0x8c, 0x9c, 0xa5, 0xac, 0xbc, 0xcc in the fourth
			 */
			if( ( data[ 2 ] == 0xec )
			 && ( ( data[ 3 ] == 0x6c )
			  ||  ( data[ 3 ] == 0x7c )
			  ||  ( data[ 3 ] == 0x8c )
			  ||  ( data[ 3 ] == 0x9c )
			  ||  ( data[ 3 ] == 0xa5 )
			  ||  ( data[ 3 ] == 0xac )
			  ||  ( data[ 3 ] == 0xbc )
			  ||  ( data[ 3 ] == 0xcc ) ) )
			{
#if defined( HAVE_DEBUG_OUTPUT )
				if( libcnotify_verbose != 0 )
				{
					libcnotify_printf(
					 "%s: compressible encrypted data detected while encryption type is none - decryption forced.\n",
					 function );
				}
#endif
				data_block->io_handle->force_decryption = 1;
			}
		}
	}
	return( 1 );
}

