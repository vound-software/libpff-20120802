/*
 * Library to support the Personal Folder File (PAB, PST and OST) format
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

#if !defined( _LIBPFF_H )
#define _LIBPFF_H

#include <libpff/codepage.h>
#include <libpff/definitions.h>
#include <libpff/error.h>
#include <libpff/extern.h>
#include <libpff/features.h>
#include <libpff/mapi.h>
#include <libpff/types.h>

#include <stdio.h>

#if defined( LIBPFF_HAVE_BFIO )
#include <libbfio.h>
#endif

#if defined( __cplusplus )
extern "C" {
#endif

/* -------------------------------------------------------------------------
 * Support functions
 * ------------------------------------------------------------------------- */

/* Returns the library version
 */
LIBPFF_EXTERN \
const char *libpff_get_version(
             void );

/* Returns the access flags for reading
 */
LIBPFF_EXTERN \
int libpff_get_access_flags_read(
     void );

/* Retrieves the narrow system string codepage
 * A value of 0 represents no codepage, UTF-8 encoding is used instead
 * Returns 1 if successful or -1 on error
 */
LIBPFF_EXTERN \
int libpff_get_codepage(
     int *codepage,
     libpff_error_t **error );

/* Sets the narrow system string codepage
 * A value of 0 represents no codepage, UTF-8 encoding is used instead
 * Returns 1 if successful or -1 on error
 */
LIBPFF_EXTERN \
int libpff_set_codepage(
     int codepage,
     libpff_error_t **error );

/* Determines if a file is a Personal Folder File file (check for the file signature)
 * Returns 1 if true, 0 if not or -1 on error
 */
LIBPFF_EXTERN \
int libpff_check_file_signature(
     const char *filename,
     libpff_error_t **error );

#if defined( LIBPFF_HAVE_WIDE_CHARACTER_TYPE )
/* Determines if a file is a Personal Folder File file (check for the file signature)
 * Returns 1 if true, 0 if not or -1 on error
 */
LIBPFF_EXTERN \
int libpff_check_file_signature_wide(
     const wchar_t *filename,
     libpff_error_t **error );
#endif

#if defined( LIBPFF_HAVE_BFIO )
/* Determines if a file is a Personal Folder File file (check for the file signature) using a Basic File IO (bfio) handle
 * Returns 1 if true, 0 if not or -1 on error
 */
LIBPFF_EXTERN \
int libpff_check_file_signature_file_io_handle(
     libbfio_handle_t *file_io_handle,
     libpff_error_t **error );
#endif

/* -------------------------------------------------------------------------
 * Notify functions
 * ------------------------------------------------------------------------- */

/* Sets the verbose notification
 */
LIBPFF_EXTERN \
void libpff_notify_set_verbose(
      int verbose );

/* Sets the notification stream
 * Returns 1 if successful or -1 on error
 */
LIBPFF_EXTERN \
int libpff_notify_set_stream(
     FILE *stream,
     libpff_error_t **error );

/* Opens the notification stream using a filename
 * The stream is opened in append mode
 * Returns 1 if successful or -1 on error
 */
LIBPFF_EXTERN \
int libpff_notify_stream_open(
     const char *filename,
     libpff_error_t **error );

/* Closes the notification stream if opened using a filename
 * Returns 0 if successful or -1 on error
 */
LIBPFF_EXTERN \
int libpff_notify_stream_close(
     libpff_error_t **error );

/* -------------------------------------------------------------------------
 * Error functions
 * ------------------------------------------------------------------------- */

/* Frees the error including elements
 */
LIBPFF_EXTERN \
void libpff_error_free(
      libpff_error_t **error );

/* Prints a descriptive string of the error to the stream
 * Returns the number of printed characters if successful or -1 on error
 */
LIBPFF_EXTERN \
int libpff_error_fprint(
     libpff_error_t *error,
     FILE *stream );

/* Prints a descriptive string of the error to the string
 * The end-of-string character is not included in the return value
 * Returns the number of printed characters if successful or -1 on error
 */
LIBPFF_EXTERN \
int libpff_error_sprint(
     libpff_error_t *error,
     char *string, 
     size_t size );

/* Prints a backtrace of the error to the stream
 * Returns the number of printed characters if successful or -1 on error
 */
LIBPFF_EXTERN \
int libpff_error_backtrace_fprint(
     libpff_error_t *error,
     FILE *stream );

/* Prints a backtrace of the error to the string
 * The end-of-string character is not included in the return value
 * Returns the number of printed characters if successful or -1 on error
 */
LIBPFF_EXTERN \
int libpff_error_backtrace_sprint(
     libpff_error_t *error,
     char *string,
     size_t size );

/* -------------------------------------------------------------------------
 * File functions
 * ------------------------------------------------------------------------- */

/* Initializes a file
 * Make sure the value file is pointing to is set to NULL
 * Returns 1 if successful or -1 on error
 */
LIBPFF_EXTERN \
int libpff_file_initialize(
     libpff_file_t **file,
     libpff_error_t **error );

/* Frees a file
 * Returns 1 if successful or -1 on error
 */
LIBPFF_EXTERN \
int libpff_file_free(
     libpff_file_t **file,
     libpff_error_t **error );

/* Signals the file to abort its current activity
 * Returns 1 if successful or -1 on error
 */
LIBPFF_EXTERN \
int libpff_file_signal_abort(
     libpff_file_t *file,
     libpff_error_t **error );

/* Opens a file
 * Returns 1 if successful or -1 on error
 */
LIBPFF_EXTERN \
int libpff_file_open(
     libpff_file_t *file,
     const char *filename,
     int access_flags,
     libpff_error_t **error );

#if defined( LIBPFF_HAVE_WIDE_CHARACTER_TYPE )
/* Opens a file
 * Returns 1 if successful or -1 on error
 */
LIBPFF_EXTERN \
int libpff_file_open_wide(
     libpff_file_t *file,
     const wchar_t *filename,
     int access_flags,
     libpff_error_t **error );
#endif

#if defined( LIBPFF_HAVE_BFIO )
/* Opens a file using a Basic File IO (bfio) handle
 * Returns 1 if successful or -1 on error
 */
LIBPFF_EXTERN \
int libpff_file_open_file_io_handle(
     libpff_file_t *file,
     libbfio_handle_t *file_io_handle,
     int access_flags,
     libpff_error_t **error );
#endif

/* Closes a file
 * Returns 0 if successful or -1 on error
 */
LIBPFF_EXTERN \
int libpff_file_close(
     libpff_file_t *file,
     libpff_error_t **error );

/* Recovers deleted items within a file
 * By default only the unallocated space is checked for recoverable items
 * Returns 1 if successful or -1 on error
 */
LIBPFF_EXTERN \
int libpff_file_recover_items(
     libpff_file_t *file,
     uint8_t recovery_flags,
     libpff_error_t **error );

/* Retrieves the file size
 * Returns 1 if successful or -1 on error
 */
LIBPFF_EXTERN \
int libpff_file_get_size(
     libpff_file_t *file,
     size64_t *size,
     libpff_error_t **error );

/* Retrieves the content type
 * Returns 1 if successful or -1 on error
 */
LIBPFF_EXTERN \
int libpff_file_get_content_type(
     libpff_file_t *file,
     uint8_t *content_type,
     libpff_error_t **error );

/* Retrieves the file type
 * Returns 1 if successful or -1 on error
 */
LIBPFF_EXTERN \
int libpff_file_get_type(
     libpff_file_t *file,
     uint8_t *type,
     libpff_error_t **error );

/* Retrieves the encryption type
 * Returns 1 if successful or -1 on error
 */
LIBPFF_EXTERN \
int libpff_file_get_encryption_type(
     libpff_file_t *file,
     uint8_t *encryption_type,
     libpff_error_t **error );

/* Retrieves the ASCII codepage
 * Returns 1 if successful or -1 on error
 */
LIBPFF_EXTERN \
int libpff_file_get_ascii_codepage(
     libpff_file_t *file,
     int *ascii_codepage,
     libpff_error_t **error );

/* Sets the ASCII codepage
 * Returns 1 if successful or -1 on error
 */
LIBPFF_EXTERN \
int libpff_file_set_ascii_codepage(
     libpff_file_t *file,
     int ascii_codepage,
     libpff_error_t **error );

/* Retrieves the number of unallocated blocks
 * Returns 1 if successful or -1 on error
 */
LIBPFF_EXTERN \
int libpff_file_get_number_of_unallocated_blocks(
     libpff_file_t *file,
     int unallocated_block_type,
     int *number_of_unallocated_blocks,
     libpff_error_t **error );

/* Retrieves a specific unallocated block
 * Returns 1 if successful or -1 on error
 */
LIBPFF_EXTERN \
int libpff_file_get_unallocated_block(
     libpff_file_t *file,
     int unallocated_block_type,
     int unallocated_block_index,
     off64_t *offset,
     size64_t *size,
     libpff_error_t **error );

/* Retrieves the root item
 * Returns 1 if successful or -1 on error
 */
LIBPFF_EXTERN \
int libpff_file_get_root_item(
     libpff_file_t *file,
     libpff_item_t **root_item,
     libpff_error_t **error );

/* Retrieves the message store
 * Returns 1 if successful, 0 if not present or -1 on error
 */
LIBPFF_EXTERN \
int libpff_file_get_message_store(
     libpff_file_t *file,
     libpff_item_t **message_store,
     libpff_error_t **error );

/* Retrieves the root folder
 * Returns 1 if successful or -1 on error
 */
LIBPFF_EXTERN \
int libpff_file_get_root_folder(
     libpff_file_t *file,
     libpff_item_t **root_folder,
     libpff_error_t **error );

/* Retrieves an item for a specific identifier
 * Returns 1 if successful, 0 if no such item, or -1 on error
 */
LIBPFF_EXTERN \
int libpff_file_get_item_by_identifier(
     libpff_file_t *file,
     uint32_t item_identifier,
     libpff_item_t **item,
     libpff_error_t **error );

/* Retrieves the number of orphan items
 * Returns 1 if successful or -1 on error
 */
LIBPFF_EXTERN \
int libpff_file_get_number_of_orphan_items(
     libpff_file_t *file,
     int *number_of_orphan_items,
     libpff_error_t **error );

/* Retrieves a specific orphan item
 * Returns 1 if successful or -1 on error
 */
LIBPFF_EXTERN \
int libpff_file_get_orphan_item(
     libpff_file_t *file,
     int orphan_item_index,
     libpff_item_t **orphan_item,
     libpff_error_t **error );

/* Retrieves the number of recovered items
 * Returns 1 if successful or -1 on error
 */
LIBPFF_EXTERN \
int libpff_file_get_number_of_recovered_items(
     libpff_file_t *file,
     int *number_of_recovered_items,
     libpff_error_t **error );

/* Retrieves a specific recovered item
 * Returns 1 if successful or -1 on error
 */
LIBPFF_EXTERN \
int libpff_file_get_recovered_item(
     libpff_file_t *file,
     int recovered_item_index,
     libpff_item_t **recovered_item,
     libpff_error_t **error );

/* -------------------------------------------------------------------------
 * Name to id map entry functions
 * ------------------------------------------------------------------------- */

/* Retrieves the type
 * Returns 1 if successful or -1 on error
 */
LIBPFF_EXTERN \
int libpff_name_to_id_map_entry_get_type(
     libpff_name_to_id_map_entry_t *name_to_id_map_entry,
     uint8_t *entry_type,
     libpff_error_t **error );

/* Retrieves the number
 * Returns 1 if successful or -1 on error
 */
LIBPFF_EXTERN \
int libpff_name_to_id_map_entry_get_number(
     libpff_name_to_id_map_entry_t *name_to_id_map_entry,
     uint32_t *number,
     libpff_error_t **error );

/* Retrieves the UTF-8 string size
 * The returned size includes the end of string character
 * Returns 1 if successful or -1 on error
 */
LIBPFF_EXTERN \
int libpff_name_to_id_map_entry_get_utf8_string_size(
     libpff_name_to_id_map_entry_t *name_to_id_map_entry,
     size_t *utf8_string_size,
     libpff_error_t **error );

/* Retrieves the UTF-8 string
 * The size should include the end of string character
 * Returns 1 if successful or -1 on error
 */
LIBPFF_EXTERN \
int libpff_name_to_id_map_entry_get_utf8_string(
     libpff_name_to_id_map_entry_t *name_to_id_map_entry,
     uint8_t *utf8_string,
     size_t utf8_string_size,
     libpff_error_t **error );

/* Retrieves the UTF-16 string size
 * The returned size includes the end of string character
 * Returns 1 if successful or -1 on error
 */
LIBPFF_EXTERN \
int libpff_name_to_id_map_entry_get_utf16_string_size(
     libpff_name_to_id_map_entry_t *name_to_id_map_entry,
     size_t *utf16_string_size,
     libpff_error_t **error );

/* Retrieves the UTF-16 string
 * The size should include the end of string character
 * Returns 1 if successful or -1 on error
 */
LIBPFF_EXTERN \
int libpff_name_to_id_map_entry_get_utf16_string(
     libpff_name_to_id_map_entry_t *name_to_id_map_entry,
     uint16_t *utf16_string,
     size_t utf16_string_size,
     libpff_error_t **error );

/* Retrieves the GUID
 * Returns 1 if successful or -1 on error
 */
LIBPFF_EXTERN \
int libpff_name_to_id_map_entry_get_guid(
     libpff_name_to_id_map_entry_t *name_to_id_map_entry,
     uint8_t *guid,
     size_t size,
     libpff_error_t **error );

/* -------------------------------------------------------------------------
 * Item functions
 * ------------------------------------------------------------------------- */

/* Clones an item
 * Returns 1 if successful or -1 on error
 */
LIBPFF_EXTERN \
int libpff_item_clone(
     libpff_item_t **destination_item,
     libpff_item_t *source_item,
     libpff_error_t **error );

/* Frees an item
 * Returns 1 if successful or -1 on error
 */
LIBPFF_EXTERN \
int libpff_item_free(
     libpff_item_t **item,
     libpff_error_t **error );

/* Retrieves the identifier
 * Returns 1 if successful or -1 on error
 */
LIBPFF_EXTERN \
int libpff_item_get_identifier(
     libpff_item_t *item,
     uint32_t *identifier,
     libpff_error_t **error );

/* Retrieves the number of sets values
 * Returns 1 if successful or -1 on error
 */
LIBPFF_EXTERN \
int libpff_item_get_number_of_sets(
     libpff_item_t *item,
     uint32_t *number_of_sets,
     libpff_error_t **error );

/* Retrieves the number of entries (of a set)
 * All sets in an item contain the same number of entries
 * Returns 1 if successful or -1 on error
 */
LIBPFF_EXTERN \
int libpff_item_get_number_of_entries(
     libpff_item_t *item,
     uint32_t *number_of_entries,
     libpff_error_t **error );

/* Retrieves the entry and value type of a specific entry
 * The name to id map entry is set when available
 * Returns 1 if successful or -1 on error
 */
LIBPFF_EXTERN \
int libpff_item_get_entry_type(
     libpff_item_t *item,
     int set_index,
     int entry_index,
     uint32_t *entry_type,
     uint32_t *value_type,
     libpff_name_to_id_map_entry_t **name_to_id_map_entry,
     libpff_error_t **error );

/* Retrieves the value type of a specific entry type
 *
 * When the LIBPFF_ENTRY_VALUE_FLAG_IGNORE_NAME_TO_ID_MAP is set
 * the name to identifier mapping is ignored. The default behavior is
 * the use the mapped entry value. In this case named properties are not
 * retrieved.
 *
 * Returns 1 if successful, 0 if the item does not contain such value or -1 on error
 */
LIBPFF_EXTERN \
int libpff_item_get_value_type(
     libpff_item_t *item,
     int set_index,
     uint32_t entry_type,
     uint32_t *value_type,
     uint8_t flags,
     libpff_error_t **error );

/* Retrieves the value of a specific entry
 *
 * When the LIBPFF_ENTRY_VALUE_FLAG_MATCH_ANY_VALUE_TYPE is set
 * the value type is ignored and set. The default behavior is a strict
 * matching of the value type. In this case the value type must be filled
 * with the corresponding value type
 *
 * When the LIBPFF_ENTRY_VALUE_FLAG_IGNORE_NAME_TO_ID_MAP is set
 * the name to identifier mapping is ignored. The default behavior is
 * the use the mapped entry value. In this case named properties are not
 * retrieved.
 *
 * Returns 1 if successful, 0 if the item does not contain such value or -1 on error
 */
LIBPFF_EXTERN \
int libpff_item_get_entry_value(
     libpff_item_t *item,
     int set_index,
     uint32_t entry_type,
     uint32_t *value_type,
     uint8_t **value_data,
     size_t *value_data_size,
     uint8_t flags,
     libpff_error_t **error );

/* Retrieves the value of a specific entry for an UTF-8 encoded name
 * Returns 1 if successful, 0 if the item does not contain such value or -1 on error
 */
LIBPFF_EXTERN \
int libpff_item_get_entry_value_by_utf8_name(
     libpff_item_t *item,
     int set_index,
     uint8_t *utf8_entry_name,
     size_t utf8_entry_name_length,
     uint32_t *value_type,
     uint8_t **value_data,
     size_t *value_data_size,
     libpff_error_t **error );

/* Retrieves the value of a specific entry for an UTF-16 encoded name
 * Returns 1 if successful, 0 if the item does not contain such value or -1 on error
 */
LIBPFF_EXTERN \
int libpff_item_get_entry_value_by_utf16_name(
     libpff_item_t *item,
     int set_index,
     uint16_t *utf16_entry_name,
     size_t utf16_entry_name_length,
     uint32_t *value_type,
     uint8_t **value_data,
     size_t *value_data_size,
     libpff_error_t **error );

/* Retrieves the boolean value of a specific entry
 *
 * When the LIBPFF_ENTRY_VALUE_FLAG_IGNORE_NAME_TO_ID_MAP is set
 * the name to identifier mapping is ignored. The default behavior is
 * the use the mapped entry value. In this case named properties are not
 * retrieved.
 *
 * Returns 1 if successful, 0 if the item does not contain such value or -1 on error
 */
LIBPFF_EXTERN \
int libpff_item_get_entry_value_boolean(
     libpff_item_t *item,
     int set_index,
     uint32_t entry_type,
     uint8_t *entry_value,
     uint8_t flags,
     libpff_error_t **error );

/* Retrieves the 16-bit value of a specific entry
 *
 * When the LIBPFF_ENTRY_VALUE_FLAG_IGNORE_NAME_TO_ID_MAP is set
 * the name to identifier mapping is ignored. The default behavior is
 * the use the mapped entry value. In this case named properties are not
 * retrieved.
 *
 * Returns 1 if successful, 0 if the item does not contain such value or -1 on error
 */
LIBPFF_EXTERN \
int libpff_item_get_entry_value_16bit(
     libpff_item_t *item,
     int set_index,
     uint32_t entry_type,
     uint16_t *entry_value,
     uint8_t flags,
     libpff_error_t **error );

/* Retrieves the 32-bit value of a specific entry
 *
 * When the LIBPFF_ENTRY_VALUE_FLAG_IGNORE_NAME_TO_ID_MAP is set
 * the name to identifier mapping is ignored. The default behavior is
 * the use the mapped entry value. In this case named properties are not
 * retrieved.
 *
 * Returns 1 if successful, 0 if the item does not contain such value or -1 on error
 */
LIBPFF_EXTERN \
int libpff_item_get_entry_value_32bit(
     libpff_item_t *item,
     int set_index,
     uint32_t entry_type,
     uint32_t *entry_value,
     uint8_t flags,
     libpff_error_t **error );

/* Retrieves the 64-bit value of a specific entry
 *
 * When the LIBPFF_ENTRY_VALUE_FLAG_IGNORE_NAME_TO_ID_MAP is set
 * the name to identifier mapping is ignored. The default behavior is
 * the use the mapped entry value. In this case named properties are not
 * retrieved.
 *
 * Returns 1 if successful, 0 if the item does not contain such value or -1 on error
 */
LIBPFF_EXTERN \
int libpff_item_get_entry_value_64bit(
     libpff_item_t *item,
     int set_index,
     uint32_t entry_type,
     uint64_t *entry_value,
     uint8_t flags,
     libpff_error_t **error );

/* Retrieves the 64-bit filetime value of a specific entry
 *
 * When the LIBPFF_ENTRY_VALUE_FLAG_IGNORE_NAME_TO_ID_MAP is set
 * the name to identifier mapping is ignored. The default behavior is
 * the use the mapped entry value. In this case named properties are not
 * retrieved.
 *
 * Returns 1 if successful, 0 if the item does not contain such value or -1 on error
 */
LIBPFF_EXTERN \
int libpff_item_get_entry_value_filetime(
     libpff_item_t *item,
     int set_index,
     uint32_t entry_type,
     uint64_t *entry_value,
     uint8_t flags,
     libpff_error_t **error );

/* Retrieves the size value of a specific entry
 *
 * When the LIBPFF_ENTRY_VALUE_FLAG_IGNORE_NAME_TO_ID_MAP is set
 * the name to identifier mapping is ignored. The default behavior is
 * the use the mapped entry value. In this case named properties are not
 * retrieved.
 *
 * Returns 1 if successful, 0 if the item does not contain such value or -1 on error
 */
LIBPFF_EXTERN \
int libpff_item_get_entry_value_size(
     libpff_item_t *item,
     int set_index,
     uint32_t entry_type,
     size_t *entry_value,
     uint8_t flags,
     libpff_error_t **error );

/* Retrieves the floating point value of a specific entry
 *
 * When the LIBPFF_ENTRY_VALUE_FLAG_IGNORE_NAME_TO_ID_MAP is set
 * the name to identifier mapping is ignored. The default behavior is
 * the use the mapped entry value. In this case named properties are not
 * retrieved.
 *
 * Returns 1 if successful, 0 if the item does not contain such value or -1 on error
 */
LIBPFF_EXTERN \
int libpff_item_get_entry_value_floating_point(
     libpff_item_t *item,
     int set_index,
     uint32_t entry_type,
     double *entry_value,
     uint8_t flags,
     libpff_error_t **error );

/* Retrieves the UTF-8 string size of a specific entry
 * The returned size includes the end of string character
 *
 * When the LIBPFF_ENTRY_VALUE_FLAG_IGNORE_NAME_TO_ID_MAP is set
 * the name to identifier mapping is ignored. The default behavior is
 * the use the mapped entry value. In this case named properties are not
 * retrieved.
 *
 * Returns 1 if successful, 0 if the item does not contain such value or -1 on error
 */
LIBPFF_EXTERN \
int libpff_item_get_entry_value_utf8_string_size(
     libpff_item_t *item,
     int set_index,
     uint32_t entry_type,
     size_t *utf8_string_size,
     uint8_t flags,
     libpff_error_t **error );

/* Retrieves the UTF-8 string value of a specific entry
 * The function uses a codepage if necessary, it uses the codepage set for the library
 * The size should include the end of string character
 *
 * When the LIBPFF_ENTRY_VALUE_FLAG_IGNORE_NAME_TO_ID_MAP is set
 * the name to identifier mapping is ignored. The default behavior is
 * the use the mapped entry value. In this case named properties are not
 * retrieved.
 *
 * Returns 1 if successful, 0 if the item does not contain such value or -1 on error
 */
LIBPFF_EXTERN \
int libpff_item_get_entry_value_utf8_string(
     libpff_item_t *item,
     int set_index,
     uint32_t entry_type,
     uint8_t *utf8_string,
     size_t utf8_string_size,
     uint8_t flags,
     libpff_error_t **error );

/* Retrieves the UTF-16 string size of a specific entry
 * The returned size includes the end of string character
 *
 * When the LIBPFF_ENTRY_VALUE_FLAG_IGNORE_NAME_TO_ID_MAP is set
 * the name to identifier mapping is ignored. The default behavior is
 * the use the mapped entry value. In this case named properties are not
 * retrieved.
 *
 * Returns 1 if successful, 0 if the item does not contain such value or -1 on error
 */
LIBPFF_EXTERN \
int libpff_item_get_entry_value_utf16_string_size(
     libpff_item_t *item,
     int set_index,
     uint32_t entry_type,
     size_t *utf16_string_size,
     uint8_t flags,
     libpff_error_t **error );

/* Retrieves the UTF-16 string value of a specific entry
 * The function uses a codepage if necessary, it uses the codepage set for the library
 * The size should include the end of string character
 * Returns 1 if successful, 0 if the item does not contain such value or -1 on error
 */
LIBPFF_EXTERN \
int libpff_item_get_entry_value_utf16_string(
     libpff_item_t *item,
     int set_index,
     uint32_t entry_type,
     uint16_t *utf16_string,
     size_t utf16_string_size,
     uint8_t flags,
     libpff_error_t **error );

/* Retrieves the size of a binary data value of a specific entry
 *
 * When the LIBPFF_ENTRY_VALUE_FLAG_IGNORE_NAME_TO_ID_MAP is set
 * the name to identifier mapping is ignored. The default behavior is
 * the use the mapped entry value. In this case named properties are not
 * retrieved.
 *
 * Returns 1 if successful, 0 if the item does not contain such value or -1 on error
 */
LIBPFF_EXTERN \
int libpff_item_get_entry_value_binary_data_size(
     libpff_item_t *item,
     int set_index,
     uint32_t entry_type,
     size_t *size,
     uint8_t flags,
     libpff_error_t **error );

/* Retrieves the binary data value of a specific entry
 *
 * When the LIBPFF_ENTRY_VALUE_FLAG_IGNORE_NAME_TO_ID_MAP is set
 * the name to identifier mapping is ignored. The default behavior is
 * the use the mapped entry value. In this case named properties are not
 * retrieved.
 *
 * Returns 1 if successful, 0 if the item does not contain such value or -1 on error
 */
LIBPFF_EXTERN \
int libpff_item_get_entry_value_binary_data(
     libpff_item_t *item,
     int set_index,
     uint32_t entry_type,
     uint8_t *binary_data,
     size_t size,
     uint8_t flags,
     libpff_error_t **error );

/* Retrieves the GUID value of a specific entry
 *
 * When the LIBPFF_ENTRY_VALUE_FLAG_IGNORE_NAME_TO_ID_MAP is set
 * the name to identifier mapping is ignored. The default behavior is
 * the use the mapped entry value. In this case named properties are not
 * retrieved.
 *
 * Returns 1 if successful, 0 if the item does not contain such value or -1 on error
 */
LIBPFF_EXTERN \
int libpff_item_get_entry_value_guid(
     libpff_item_t *item,
     int set_index,
     uint32_t entry_type,
     uint8_t *guid,
     size_t size,
     uint8_t flags,
     libpff_error_t **error );

/* Retrieves the multi value of a specific entry
 * Creates a new multi value
 *
 * When the LIBPFF_ENTRY_VALUE_FLAG_IGNORE_NAME_TO_ID_MAP is set
 * the name to identifier mapping is ignored. The default behavior is
 * the use the mapped entry value. In this case named properties are not
 * retrieved.
 *
 * Returns 1 if successful, 0 if the item does not contain such value or -1 on error
 */
LIBPFF_EXTERN \
int libpff_item_get_entry_multi_value(
     libpff_item_t *item,
     int set_index,
     uint32_t entry_type,
     libpff_multi_value_t **multi_value,
     uint8_t flags,
     libpff_error_t **error );

/* Retrieves the type value
 * Determines the item type if neccessary
 * Returns 1 if successful or -1 on error
 */
LIBPFF_EXTERN \
int libpff_item_get_type(
     libpff_item_t *item,
     uint8_t *item_type,
     libpff_error_t **error );

/* Retrieves the number of sub items from a item
 * Returns 1 if successful or -1 on error
 */
LIBPFF_EXTERN \
int libpff_item_get_number_of_sub_items(
     libpff_item_t *item,
     int *number_of_sub_items,
     libpff_error_t **error );

/* Retrieves the sub item for the specific index from a item
 * Returns 1 if successful or -1 on error
 */
LIBPFF_EXTERN \
int libpff_item_get_sub_item(
     libpff_item_t *item,
     int sub_item_index,
     libpff_item_t **sub_item,
     libpff_error_t **error );

/* Retrieves the sub item for the specific identifier from a item
 * Returns 1 if successful, 0 if no such item, or -1 on error
 */
LIBPFF_EXTERN \
int libpff_item_get_sub_item_by_identifier(
     libpff_item_t *item,
     uint32_t sub_item_identifier,
     libpff_item_t **sub_item,
     libpff_error_t **error );

/* Retrieves the size of the UTF-8 encoded item display name
 * Size includes the end of string character
 * Returns 1 if successful, 0 if the item does not contain such value or -1 on error
 */
#define libpff_item_get_utf8_display_name_size( item, utf8_string_size, error ) \
        libpff_item_get_entry_value_utf8_string_size( item, 0, LIBPFF_ENTRY_TYPE_DISPLAY_NAME, utf8_string_size, 0, error )

/* Retrieves the UTF-8 encoded item display name
 * Returns 1 if successful, 0 if the item does not contain such value or -1 on error
 */
#define libpff_item_get_utf8_display_name( item, utf8_string, utf8_string_size, error ) \
        libpff_item_get_entry_value_utf8_string( item, 0, LIBPFF_ENTRY_TYPE_DISPLAY_NAME, utf8_string, utf8_string_size, 0, error )

/* Retrieves the size of the UTF-16 encoded item display name
 * Size includes the end of string character
 * Returns 1 if successful, 0 if the item does not contain such value or -1 on error
 */
#define libpff_item_get_utf16_display_name_size( item, utf16_string_size, error ) \
        libpff_item_get_entry_value_utf16_string_size( item, 0, LIBPFF_ENTRY_TYPE_DISPLAY_NAME, utf16_string_size, 0, error )

/* Retrieves the UTF-16 encoded item display name
 * Returns 1 if successful, 0 if the item does not contain such value or -1 on error
 */
#define libpff_item_get_utf16_display_name( item, utf16_string, utf16_string_size, error ) \
        libpff_item_get_entry_value_utf16_string( item, 0, LIBPFF_ENTRY_TYPE_DISPLAY_NAME, utf16_string, utf16_string_size, 0, error )

/* Retrieves the size of the UTF-8 encoded item address type
 * Size includes the end of string character
 * Returns 1 if successful, 0 if the item does not contain such value or -1 on error
 */
#define libpff_item_get_utf8_address_type_size( item, utf8_string_size, error ) \
        libpff_item_get_entry_value_utf8_string_size( item, 0, LIBPFF_ENTRY_TYPE_ADDRESS_TYPE, utf8_string_size, 0, error )

/* Retrieves the UTF-8 encoded item address type
 * Returns 1 if successful, 0 if the item does not contain such value or -1 on error
 */
#define libpff_item_get_utf8_address_type( item, utf8_string, utf8_string_size, error ) \
        libpff_item_get_entry_value_utf8_string( item, 0, LIBPFF_ENTRY_TYPE_ADDRESS_TYPE, utf8_string, utf8_string_size, 0, error )

/* Retrieves the size of the UTF-16 encoded item address type
 * Size includes the end of string character
 * Returns 1 if successful, 0 if the item does not contain such value or -1 on error
 */
#define libpff_item_get_utf16_address_type_size( item, utf16_string_size, error ) \
        libpff_item_get_entry_value_utf16_string_size( item, 0, LIBPFF_ENTRY_TYPE_ADDRESS_TYPE, utf16_string_size, 0, error )

/* Retrieves the UTF-16 encoded item address type
 * Returns 1 if successful, 0 if the item does not contain such value or -1 on error
 */
#define libpff_item_get_utf16_address_type( item, utf16_string, utf16_string_size, error ) \
        libpff_item_get_entry_value_utf16_string( item, 0, LIBPFF_ENTRY_TYPE_ADDRESS_TYPE, utf16_string, utf16_string_size, 0, error )

/* Retrieves the size of the UTF-8 encoded item e-mail address
 * Size includes the end of string character
 * Returns 1 if successful, 0 if the item does not contain such value or -1 on error
 */
#define libpff_item_get_utf8_email_address_size( item, utf8_string_size, error ) \
        libpff_item_get_entry_value_utf8_string_size( item, 0, LIBPFF_ENTRY_TYPE_EMAIL_ADDRESS, utf8_string_size, 0, error )

/* Retrieves the UTF-8 encoded item e-mail address
 * Returns 1 if successful, 0 if the item does not contain such value or -1 on error
 */
#define libpff_item_get_utf8_email_address( item, utf8_string, size, error ) \
        libpff_item_get_entry_value_utf8_string( item, 0, LIBPFF_ENTRY_TYPE_EMAIL_ADDRESS, utf8_string, utf8_string_size, 0, error )

/* Retrieves the size of the UTF-16 encoded item e-mail address
 * Size includes the end of string character
 * Returns 1 if successful, 0 if the item does not contain such value or -1 on error
 */
#define libpff_item_get_utf16_email_address_size( item, utf16_string_size, error ) \
        libpff_item_get_entry_value_utf16_string_size( item, 0, LIBPFF_ENTRY_TYPE_EMAIL_ADDRESS, utf16_string_size, 0, error )

/* Retrieves the UTF-16 encoded item e-mail address
 * Returns 1 if successful, 0 if the item does not contain such value or -1 on error
 */
#define libpff_item_get_utf16_email_address( item, utf16_string, size, error ) \
        libpff_item_get_entry_value_utf16_string( item, 0, LIBPFF_ENTRY_TYPE_EMAIL_ADDRESS, utf16_string, utf16_string_size, 0, error )

/* -------------------------------------------------------------------------
 * Multi value functions
 * ------------------------------------------------------------------------- */

/* Frees a multi value
 * Returns 1 if successful or -1 on error
 */
LIBPFF_EXTERN \
int libpff_multi_value_free(
     libpff_multi_value_t **multi_value,
     libpff_error_t **error );

/* Retrieves the number of values in a multi value
 * Returns 1 if successful or -1 on error
 */
LIBPFF_EXTERN \
int libpff_multi_value_get_number_of_values(
     libpff_multi_value_t *multi_value,
     int *number_of_values,
     libpff_error_t **error );

/* Retrieves a specific value of the multi value
 * Returns 1 if successful or -1 on error
 */
LIBPFF_EXTERN \
int libpff_multi_value_get_value(
     libpff_multi_value_t *multi_value,
     int value_index,
     uint32_t *value_type,
     uint8_t **value_data,
     size_t *value_data_size,
     libpff_error_t **error );

/* Retrieves the 32-bit value of a specific value of the multi value
 * Returns 1 if successful or -1 on error
 */
LIBPFF_EXTERN \
int libpff_multi_value_get_value_32bit(
     libpff_multi_value_t *multi_value,
     int value_index,
     uint32_t *value,
     libpff_error_t **error );

/* Retrieves the 64-bit value of a specific value of the multi value
 * Returns 1 if successful or -1 on error
 */
LIBPFF_EXTERN \
int libpff_multi_value_get_value_64bit(
     libpff_multi_value_t *multi_value,
     int value_index,
     uint64_t *value,
     libpff_error_t **error );

/* Retrieves the 64-bit filetime value of a specific value of the multi value
 * Returns 1 if successful or -1 on error
 */
LIBPFF_EXTERN \
int libpff_multi_value_get_value_filetime(
     libpff_multi_value_t *multi_value,
     int value_index,
     uint64_t *filetime,
     libpff_error_t **error );

/* Retrieves the UTF-8 string size of a specific value of the multi value
 * The returned size includes the end of string character
 * Returns 1 if successful or -1 on error
 */
LIBPFF_EXTERN \
int libpff_multi_value_get_value_utf8_string_size(
     libpff_multi_value_t *multi_value,
     int value_index,
     size_t *utf8_string_size,
     libpff_error_t **error );

/* Retrieves the UTF-8 string value of a specific value of the multi value
 * The size should include the end of string character
 * Returns 1 if successful or -1 on error
 */
LIBPFF_EXTERN \
int libpff_multi_value_get_value_utf8_string(
     libpff_multi_value_t *multi_value,
     int value_index,
     uint8_t *utf8_string,
     size_t utf8_string_size,
     libpff_error_t **error );

/* Retrieves the UTF-16 string size of a specific value of the multi value
 * The returned size includes the end of string character
 * Returns 1 if successful or -1 on error
 */
LIBPFF_EXTERN \
int libpff_multi_value_get_value_utf16_string_size(
     libpff_multi_value_t *multi_value,
     int value_index,
     size_t *utf16_string_size,
     libpff_error_t **error );

/* Retrieves the UTF-16 string value of a specific value of the multi value
 * The size should include the end of string character
 * Returns 1 if successful or -1 on error
 */
LIBPFF_EXTERN \
int libpff_multi_value_get_value_utf16_string(
     libpff_multi_value_t *multi_value,
     int value_index,
     uint16_t *utf16_string,
     size_t utf16_string_size,
     libpff_error_t **error );

/* Retrieves the size of a binary data value of a specific value of the multi value
 * Returns 1 if successful or -1 on error
 */
LIBPFF_EXTERN \
int libpff_multi_value_get_value_binary_data_size(
     libpff_multi_value_t *multi_value,
     int value_index,
     size_t *size,
     libpff_error_t **error );

/* Retrieves the binary data value of a specific value of the multi value
 * Returns 1 if successful or -1 on error
 */
LIBPFF_EXTERN \
int libpff_multi_value_get_value_binary_data(
     libpff_multi_value_t *multi_value,
     int value_index,
     uint8_t *binary_data,
     size_t size,
     libpff_error_t **error );

/* Retrieves the GUID value of a specific value of the multi value
 * Returns 1 if successful or -1 on error
 */
LIBPFF_EXTERN \
int libpff_multi_value_get_value_guid(
     libpff_multi_value_t *multi_value,
     int value_index,
     uint8_t *guid,
     size_t size,
     libpff_error_t **error );

/* -------------------------------------------------------------------------
 * Folder functions
 * ------------------------------------------------------------------------- */

/* Retrieves the size of the UTF-8 encoded folder name
 * Size includes the end of string character
 * Returns 1 if successful, 0 if the folder does not contain such value or -1 on error
 */
#define libpff_folder_get_utf8_name_size( folder, utf8_string_size, error ) \
	libpff_item_get_entry_value_utf8_string_size( folder, 0, LIBPFF_ENTRY_TYPE_DISPLAY_NAME, utf8_string_size, 0, error )

/* Retrieves the UTF-8 encoded folder name
 * Returns 1 if successful, 0 if the folder does not contain such value or -1 on error
 */
#define libpff_folder_get_utf8_name( folder, utf8_string, utf8_string_size, error ) \
	libpff_item_get_entry_value_utf8_string( folder, 0, LIBPFF_ENTRY_TYPE_DISPLAY_NAME, utf8_string, utf8_string_size, 0, error )

/* Retrieves the size of the UTF-16 encoded folder name
 * Size includes the end of string character
 * Returns 1 if successful, 0 if the folder does not contain such value or -1 on error
 */
#define libpff_folder_get_utf16_name_size( folder, utf16_string_size, error ) \
	libpff_item_get_entry_value_utf16_string_size( folder, 0, LIBPFF_ENTRY_TYPE_DISPLAY_NAME, utf16_string_size, 0, error )

/* Retrieves the UTF-16 encoded folder name
 * Returns 1 if successful, 0 if the folder does not contain such value or -1 on error
 */
#define libpff_folder_get_utf16_name( folder, utf16_string, utf16_string_size, error ) \
	libpff_item_get_entry_value_utf16_string( folder, 0, LIBPFF_ENTRY_TYPE_DISPLAY_NAME, utf16_string, utf16_string_size, 0, error )

/* Retrieves the folder type
 * Returns 1 if successful or -1 on error
 */
LIBPFF_EXTERN \
int libpff_folder_get_type(
     libpff_item_t *folder,
     uint8_t *type,
     libpff_error_t **error );

/* Retrieves the number of sub folders from a folder
 * Returns 1 if successful or -1 on error
 */
LIBPFF_EXTERN \
int libpff_folder_get_number_of_sub_folders(
     libpff_item_t *folder,
     int *number_of_sub_folders,
     libpff_error_t **error );

/* Retrieves the sub folder for the specific index from a folder
 * Returns 1 if successful or -1 on error
 */
LIBPFF_EXTERN \
int libpff_folder_get_sub_folder(
     libpff_item_t *folder,
     int sub_folder_index,
     libpff_item_t **sub_folder,
     libpff_error_t **error );

/* Retrieves the sub folder from a folder for the specific UTF-8 encoded name
 * This function uses the PidTagDisplayName MAPI property as the name
 * Returns 1 if successful, 0 if no such sub folder or -1 on error
 */
LIBPFF_EXTERN \
int libpff_folder_get_sub_folder_by_utf8_name(
     libpff_item_t *folder,
     uint8_t *utf8_sub_folder_name,
     size_t utf8_sub_folder_name_size,
     libpff_item_t **sub_folder,
     libpff_error_t **error );

/* Retrieves the sub folder from a folder for the specific UTF-16 encoded name
 * This function uses the PidTagDisplayName MAPI property as the name
 * Returns 1 if successful, 0 if no such sub folder or -1 on error
 */
LIBPFF_EXTERN \
int libpff_folder_get_sub_folder_by_utf16_name(
     libpff_item_t *folder,
     uint16_t *utf16_sub_folder_name,
     size_t utf16_sub_folder_name_size,
     libpff_item_t **sub_folder,
     libpff_error_t **error );

/* Retrieves the sub folders from a folder
 * Returns 1 if successful, 0 if the folder does not contain such value or -1 on error
 */
LIBPFF_EXTERN \
int libpff_folder_get_sub_folders(
     libpff_item_t *folder,
     libpff_item_t **sub_folders,
     libpff_error_t **error );

/* Retrieves the number of sub messages from a folder
 * Returns 1 if successful or -1 on error
 */
LIBPFF_EXTERN \
int libpff_folder_get_number_of_sub_messages(
     libpff_item_t *folder,
     int *number_of_sub_messages,
     libpff_error_t **error );

/* Retrieves the sub message for the specific index from a folder
 * Returns 1 if successful or -1 on error
 */
LIBPFF_EXTERN \
int libpff_folder_get_sub_message(
     libpff_item_t *folder,
     int sub_message_index,
     libpff_item_t **sub_message,
     libpff_error_t **error );

/* Retrieves the sub message from a folder for the specific UTF-8 encoded name
 * This function uses the PidTagDisplayName MAPI property as the name
 * Returns 1 if successful, 0 if no such sub message or -1 on error
 */
LIBPFF_EXTERN \
int libpff_folder_get_sub_message_by_utf8_name(
     libpff_item_t *folder,
     uint8_t *utf8_sub_message_name,
     size_t utf8_sub_message_name_size,
     libpff_item_t **sub_message,
     libpff_error_t **error );

/* Retrieves the sub message from a folder for the specific UTF-16 encoded name
 * This function uses the PidTagDisplayName MAPI property as the name
 * Returns 1 if successful, 0 if no such sub message or -1 on error
 */
LIBPFF_EXTERN \
int libpff_folder_get_sub_message_by_utf16_name(
     libpff_item_t *folder,
     uint16_t *utf16_sub_message_name,
     size_t utf16_sub_message_name_size,
     libpff_item_t **sub_message,
     libpff_error_t **error );

/* Retrieves the sub messages from a folder
 * Returns 1 if successful, 0 if the folder does not contain such value or -1 on error
 */
LIBPFF_EXTERN \
int libpff_folder_get_sub_messages(
     libpff_item_t *folder,
     libpff_item_t **sub_messages,
     libpff_error_t **error );

/* Retrieves the number of sub associated contents from a folder
 * Returns 1 if successful or -1 on error
 */
LIBPFF_EXTERN \
int libpff_folder_get_number_of_sub_associated_contents(
     libpff_item_t *folder,
     int *number_of_sub_associated_contents,
     libpff_error_t **error );

/* Retrieves the sub associated content for the specific index from a folder
 * Returns 1 if successful or -1 on error
 */
LIBPFF_EXTERN \
int libpff_folder_get_sub_associated_content(
     libpff_item_t *folder,
     int sub_associated_content_index,
     libpff_item_t **sub_associated_content,
     libpff_error_t **error );

/* Retrieves the sub associated contents from a folder
 * Returns 1 if successful, 0 if the folder does not contain such value or -1 on error
 */
LIBPFF_EXTERN \
int libpff_folder_get_sub_associated_contents(
     libpff_item_t *folder,
     libpff_item_t **sub_associated_contents,
     libpff_error_t **error );

/* NOTE that the following function is currently for debugging only */

/* Retrieves the unknowns from a folder
 * Returns 1 if successful, 0 if the folder does not contain such value or -1 on error
 */
LIBPFF_EXTERN \
int libpff_folder_get_unknowns(
     libpff_item_t *folder,
     libpff_item_t **unknowns,
     libpff_error_t **error );

/* -------------------------------------------------------------------------
 * Message store functions
 * ------------------------------------------------------------------------- */

/* Retrieves the message store valid folder mask
 * Returns 1 if successful, 0 if the item does not contain such value or -1 on error
 */
#define libpff_message_store_get_valid_folder_mask( message_store, valid_folder_mask, error ) \
	libpff_item_get_entry_value_32bit( message_store, 0, LIBPFF_ENTRY_TYPE_MESSAGE_STORE_VALID_FOLDER_MASK, valid_folder_mask, 0, error )

/* Retrieves the message store password checksum
 * Returns 1 if successful, 0 if the item does not contain such value or -1 on error
 */
#define libpff_message_store_get_password_checksum( message_store, password_checksum, error ) \
	libpff_item_get_entry_value_32bit( message_store, 0, LIBPFF_ENTRY_TYPE_MESSAGE_STORE_PASSWORD_CHECKSUM, password_checksum, 0, error )

/* -------------------------------------------------------------------------
 * Name to id map functions
 * ------------------------------------------------------------------------- */

/* -------------------------------------------------------------------------
 * Message functions
 * ------------------------------------------------------------------------- */

/* Retrieves the UTF-8 string size of a specific entry from the referenced message item
 * The returned size includes the end of string character
 * Returns 1 if successful, 0 if the message does not contain such value or -1 on error
 */
LIBPFF_EXTERN \
int libpff_message_get_entry_value_utf8_string_size(
     libpff_item_t *message,
     uint32_t entry_type,
     size_t *utf8_string_size,
     libpff_error_t **error );

/* Retrieves the UTF-8 string value of a specific entry from the referenced message item
 * The function uses a codepage if necessary, it tries to read PidTagMessageCodepage
 * if not available it uses the codepage set for the library
 * The size should include the end of string character
 * Returns 1 if successful, 0 if the message does not contain such value or -1 on error
 */
LIBPFF_EXTERN \
int libpff_message_get_entry_value_utf8_string(
     libpff_item_t *message,
     uint32_t entry_type,
     uint8_t *utf8_string,
     size_t utf8_string_size,
     libpff_error_t **error );

/* Retrieves the UTF-16 string size of a specific entry from the referenced message item
 * The returned size includes the end of string character
 * Returns 1 if successful, 0 if the message does not contain such value or -1 on error
 */
LIBPFF_EXTERN \
int libpff_message_get_entry_value_utf16_string_size(
     libpff_item_t *message,
     uint32_t entry_type,
     size_t *utf16_string_size,
     libpff_error_t **error );

/* Retrieves the UTF-16 string value of a specific entry from the referenced message item
 * The function uses a codepage if necessary, it tries to read PidTagMessageCodepage
 * if not available it uses the codepage set for the library
 * The size should include the end of string character
 * Returns 1 if successful, 0 if the message does not contain such value or -1 on error
 */
LIBPFF_EXTERN \
int libpff_message_get_entry_value_utf16_string(
     libpff_item_t *message,
     uint32_t entry_type,
     uint16_t *utf16_string,
     size_t utf16_string_size,
     libpff_error_t **error );

/* Retrieves the number of attachment from a message item
 * Returns 1 if successful or -1 on error
 */
LIBPFF_EXTERN \
int libpff_message_get_number_of_attachments(
     libpff_item_t *message,
     int *number_of_attachments,
     libpff_error_t **error );

/* Retrieves the attachment for the specific index from a message item
 * Returns 1 if successful or -1 on error
 */
LIBPFF_EXTERN \
int libpff_message_get_attachment(
     libpff_item_t *message,
     int attachment_index,
     libpff_item_t **attachment,
     libpff_error_t **error );

/* Retrieves the attachments from a message item
 * Returns 1 if successful, 0 if the message item does not contain such value or -1 on error
 */
LIBPFF_EXTERN \
int libpff_message_get_attachments(
     libpff_item_t *message,
     libpff_item_t **attachments,
     libpff_error_t **error );

/* Retrieves the recipients from a message item
 * Returns 1 if successful, 0 if the message item does not contain such value or -1 on error
 */
LIBPFF_EXTERN \
int libpff_message_get_recipients(
     libpff_item_t *message,
     libpff_item_t **recipients,
     libpff_error_t **error );

/* Retrieves the size of the UTF-8 encoded message class
 * Size includes the end of string character
 * Returns 1 if successful, 0 if the item does not contain such value or -1 on error
 */
#define libpff_message_get_utf8_class_size( message, utf8_string_size, error ) \
        libpff_message_get_entry_value_utf8_string_size( message, LIBPFF_ENTRY_TYPE_MESSAGE_CLASS, utf8_string_size, error )

/* Retrieves the UTF-8 encoded message class
 * Returns 1 if successful, 0 if the item does not contain such value or -1 on error
 */
#define libpff_message_get_utf8_class( message, utf8_string, utf8_string_size, error ) \
        libpff_message_get_entry_value_utf8_string( message, LIBPFF_ENTRY_TYPE_MESSAGE_CLASS, utf8_string, utf8_string_size, error )

/* Retrieves the size of the UTF-16 encoded message class
 * Size includes the end of string character
 * Returns 1 if successful, 0 if the item does not contain such value or -1 on error
 */
#define libpff_message_get_utf16_class_size( message, utf16_string_size, error ) \
        libpff_message_get_entry_value_utf16_string_size( message, LIBPFF_ENTRY_TYPE_MESSAGE_CLASS, utf16_string_size, error )

/* Retrieves the UTF-16 encoded message class
 * Returns 1 if successful, 0 if the item does not contain such value or -1 on error
 */
#define libpff_message_get_utf16_class( message, utf16_string, utf16_string_size, error ) \
        libpff_message_get_entry_value_utf16_string( message, LIBPFF_ENTRY_TYPE_MESSAGE_CLASS, utf16_string, utf16_string_size, error )

/* Retrieves the size of the UTF-8 encoded message subject
 * Size includes the end of string character
 * Returns 1 if successful, 0 if the item does not contain such value or -1 on error
 */
#define libpff_message_get_utf8_subject_size( message, utf8_string_size, error ) \
        libpff_message_get_entry_value_utf8_string_size( message, LIBPFF_ENTRY_TYPE_MESSAGE_SUBJECT, utf8_string_size, error )

/* Retrieves the UTF-8 encoded message subject
 * Returns 1 if successful, 0 if the item does not contain such value or -1 on error
 */
#define libpff_message_get_utf8_subject( message, utf8_string, utf8_string_size, error ) \
        libpff_message_get_entry_value_utf8_string( message, LIBPFF_ENTRY_TYPE_MESSAGE_SUBJECT, utf8_string, utf8_string_size, error )

/* Retrieves the size of the UTF-16 encoded message subject
 * Size includes the end of string character
 * Returns 1 if successful, 0 if the item does not contain such value or -1 on error
 */
#define libpff_message_get_utf16_subject_size( message, utf16_string_size, error ) \
        libpff_message_get_entry_value_utf16_string_size( message, LIBPFF_ENTRY_TYPE_MESSAGE_SUBJECT, utf16_string_size, error )

/* Retrieves the UTF-16 encoded message subject
 * Returns 1 if successful, 0 if the item does not contain such value or -1 on error
 */
#define libpff_message_get_utf16_subject( message, utf16_string, utf16_string_size, error ) \
        libpff_message_get_entry_value_utf16_string( message, LIBPFF_ENTRY_TYPE_MESSAGE_SUBJECT, utf16_string, utf16_string_size, error )

/* Retrieves the size of the UTF-8 encoded message conversation topic
 * Size includes the end of string character
 * Returns 1 if successful, 0 if the item does not contain such value or -1 on error
 */
#define libpff_message_get_utf8_conversation_topic_size( message, utf8_string_size, error ) \
	libpff_message_get_entry_value_utf8_string_size( message, LIBPFF_ENTRY_TYPE_MESSAGE_CONVERSATION_TOPIC, utf8_string_size, error )

/* Retrieves the UTF-8 encoded message conversation topic
 * Returns 1 if successful, 0 if the item does not contain such value or -1 on error
 */
#define libpff_message_get_utf8_conversation_topic( message, utf8_string, utf8_string_size, error ) \
	libpff_message_get_entry_value_utf8_string( message, LIBPFF_ENTRY_TYPE_MESSAGE_CONVERSATION_TOPIC, utf8_string, utf8_string_size, error )

/* Retrieves the size of the UTF-16 encoded message conversation topic
 * Size includes the end of string character
 * Returns 1 if successful, 0 if the item does not contain such value or -1 on error
 */
#define libpff_message_get_utf16_conversation_topic_size( message, utf16_string_size, error ) \
	libpff_message_get_entry_value_utf16_string_size( message, LIBPFF_ENTRY_TYPE_MESSAGE_CONVERSATION_TOPIC, utf16_string_size, error )

/* Retrieves the UTF-16 encoded message conversation topic
 * Returns 1 if successful, 0 if the item does not contain such value or -1 on error
 */
#define libpff_message_get_utf16_conversation_topic( message, utf16_string, utf16_string_size, error ) \
	libpff_message_get_entry_value_utf16_string( message, LIBPFF_ENTRY_TYPE_MESSAGE_CONVERSATION_TOPIC, utf16_string, utf16_string_size, error )

/* Retrieves the message conversation index size
 * Returns 1 if successful, 0 if the item does not contain such value or -1 on error
 */
#define libpff_message_get_conversation_index_size( message, size, error ) \
	libpff_item_get_entry_value_binary_data_size( message, 0, LIBPFF_ENTRY_TYPE_MESSAGE_CONVERSATION_INDEX, size, 0, error )

/* Retrieves the message conversation index
 * Returns 1 if successful, 0 if the item does not contain such value or -1 on error
 */
#define libpff_message_get_conversation_index( message, conversation_index, size, error ) \
	libpff_item_get_entry_value_binary_data( message, 0, LIBPFF_ENTRY_TYPE_MESSAGE_CONVERSATION_INDEX, conversation_index, size, 0, error )

/* Retrieves the size of the UTF-8 encoded message sender name
 * Size includes the end of string character
 * Returns 1 if successful, 0 if the item does not contain such value or -1 on error
 */
#define libpff_message_get_utf8_sender_name_size( message, utf8_string_size, error ) \
	libpff_message_get_entry_value_utf8_string_size( message, LIBPFF_ENTRY_TYPE_MESSAGE_SENDER_NAME, utf8_string_size, error )

/* Retrieves the UTF-8 encoded message sender name
 * Returns 1 if successful, 0 if the item does not contain such value or -1 on error
 */
#define libpff_message_get_utf8_sender_name( message, utf8_string, utf8_string_size, error ) \
	libpff_message_get_entry_value_utf8_string( message, LIBPFF_ENTRY_TYPE_MESSAGE_SENDER_NAME, utf8_string, utf8_string_size, error )

/* Retrieves the size of the UTF-16 encoded message sender name
 * Size includes the end of string character
 * Returns 1 if successful, 0 if the item does not contain such value or -1 on error
 */
#define libpff_message_get_utf16_sender_name_size( message, utf16_string_size, error ) \
	libpff_message_get_entry_value_utf16_string_size( message, LIBPFF_ENTRY_TYPE_MESSAGE_SENDER_NAME, utf16_string_size, error )

/* Retrieves the UTF-16 encoded message sender name
 * Returns 1 if successful, 0 if the item does not contain such value or -1 on error
 */
#define libpff_message_get_utf16_sender_name( message, utf16_string, utf16_string_size, error ) \
	libpff_message_get_entry_value_utf16_string( message, LIBPFF_ENTRY_TYPE_MESSAGE_SENDER_NAME, utf16_string, utf16_string_size, error )

/* Retrieves the size of the UTF-8 encoded message sender e-mail address
 * Size includes the end of string character
 * Returns 1 if successful, 0 if the item does not contain such value or -1 on error
 */
#define libpff_message_get_utf8_sender_email_address_size( message, utf8_string_size, error ) \
	libpff_message_get_entry_value_utf8_string_size( message, LIBPFF_ENTRY_TYPE_MESSAGE_SENDER_EMAIL_ADDRESS, utf8_string_size, error )

/* Retrieves the UTF-8 encoded message sender e-mail address
 * Returns 1 if successful, 0 if the item does not contain such value or -1 on error
 */
#define libpff_message_get_utf8_sender_email_address( message, utf8_string, utf8_string_size, error ) \
	libpff_message_get_entry_value_utf8_string( message, LIBPFF_ENTRY_TYPE_MESSAGE_SENDER_EMAIL_ADDRESS, utf8_string, utf8_string_size, error )

/* Retrieves the size of the UTF-16 encoded message sender e-mail address
 * Size includes the end of string character
 * Returns 1 if successful, 0 if the item does not contain such value or -1 on error
 */
#define libpff_message_get_utf16_sender_email_address_size( message, utf16_string_size, error ) \
	libpff_message_get_entry_value_utf16_string_size( message, LIBPFF_ENTRY_TYPE_MESSAGE_SENDER_EMAIL_ADDRESS, utf16_string_size, error )

/* Retrieves the UTF-16 encoded message sender e-mail address
 * Returns 1 if successful, 0 if the item does not contain such value or -1 on error
 */
#define libpff_message_get_utf16_sender_email_address( message, utf16_string, utf16_string_size, error ) \
	libpff_message_get_entry_value_utf16_string( message, LIBPFF_ENTRY_TYPE_MESSAGE_SENDER_EMAIL_ADDRESS, utf16_string, utf16_string_size, error )

/* Retrieves the size of the UTF-8 encoded message sent reprenting name
 * Size includes the end of string character
 * Returns 1 if successful, 0 if the item does not contain such value or -1 on error
 */
#define libpff_message_get_utf8_sent_representing_name_size( message, utf8_string_size, error ) \
	libpff_message_get_entry_value_utf8_string_size( message, LIBPFF_ENTRY_TYPE_MESSAGE_SENT_REPRESENTING_NAME, utf8_string_size, error )

/* Retrieves the UTF-8 encoded message sent reprenting name
 * Returns 1 if successful, 0 if the item does not contain such value or -1 on error
 */
#define libpff_message_get_utf8_sent_representing_name( message, utf8_string, utf8_string_size, error ) \
	libpff_message_get_entry_value_utf8_string( message, LIBPFF_ENTRY_TYPE_MESSAGE_SENT_REPRESENTING_NAME, utf8_string, utf8_string_size, error )

/* Retrieves the size of the UTF-16 encoded message sent reprenting name
 * Size includes the end of string character
 * Returns 1 if successful, 0 if the item does not contain such value or -1 on error
 */
#define libpff_message_get_utf16_sent_representing_name_size( message, utf16_string_size, error ) \
	libpff_message_get_entry_value_utf16_string_size( message, LIBPFF_ENTRY_TYPE_MESSAGE_SENT_REPRESENTING_NAME, utf16_string_size, error )

/* Retrieves the UTF-16 encoded message sent reprenting name
 * Returns 1 if successful, 0 if the item does not contain such value or -1 on error
 */
#define libpff_message_get_utf16_sent_representing_name( message, utf16_string, utf16_string_size, error ) \
	libpff_message_get_entry_value_utf16_string( message, LIBPFF_ENTRY_TYPE_MESSAGE_SENT_REPRESENTING_NAME, utf16_string, utf16_string_size, error )

/* Retrieves the size of the UTF-8 encoded message sent representing e-mail address
 * Size includes the end of string character
 * Returns 1 if successful, 0 if the item does not contain such value or -1 on error
 */
#define libpff_message_get_utf8_sent_representing_email_address_size( message, utf8_string_size, error ) \
	libpff_message_get_entry_value_utf8_string_size( message, LIBPFF_ENTRY_TYPE_MESSAGE_SENT_REPRESENTING_EMAIL_ADDRESS, utf8_string_size, error )

/* Retrieves the UTF-8 encoded message sent representing e-mail address
 * Returns 1 if successful, 0 if the item does not contain such value or -1 on error
 */
#define libpff_message_get_utf8_sent_representing_email_address( message, utf8_string, utf8_string_size, error ) \
	libpff_message_get_entry_value_utf8_string( message, LIBPFF_ENTRY_TYPE_MESSAGE_SENT_REPRESENTING_EMAIL_ADDRESS, utf8_string, utf8_string_size, error )

/* Retrieves the size of the UTF-16 encoded message sent representing e-mail address
 * Size includes the end of string character
 * Returns 1 if successful, 0 if the item does not contain such value or -1 on error
 */
#define libpff_message_get_utf16_sent_representing_email_address_size( message, utf16_string_size, error ) \
	libpff_message_get_entry_value_utf16_string_size( message, LIBPFF_ENTRY_TYPE_MESSAGE_SENT_REPRESENTING_EMAIL_ADDRESS, utf16_string_size, error )

/* Retrieves the UTF-16 encoded message sent representing e-mail address
 * Returns 1 if successful, 0 if the item does not contain such value or -1 on error
 */
#define libpff_message_get_utf16_sent_representing_email_address( message, utf16_string, utf16_string_size, error ) \
	libpff_message_get_entry_value_utf16_string( message, LIBPFF_ENTRY_TYPE_MESSAGE_SENT_REPRESENTING_EMAIL_ADDRESS, utf16_string, utf16_string_size, error )

/* Retrieves the size of the UTF-8 encoded message received by name
 * Size includes the end of string character
 * Returns 1 if successful, 0 if the item does not contain such value or -1 on error
 */
#define libpff_message_get_utf8_received_by_name_size( message, utf8_string_size, error ) \
	libpff_message_get_entry_value_utf8_string_size( message, LIBPFF_ENTRY_TYPE_MESSAGE_RECEIVED_BY_NAME, utf8_string_size, error )

/* Retrieves the UTF-8 encoded message received by name
 * Returns 1 if successful, 0 if the item does not contain such value or -1 on error
 */
#define libpff_message_get_utf8_received_by_name( message, utf8_string, utf8_string_size, error ) \
	libpff_message_get_entry_value_utf8_string( message, LIBPFF_ENTRY_TYPE_MESSAGE_RECEIVED_BY_NAME, utf8_string, utf8_string_size, error )

/* Retrieves the size of the UTF-16 encoded message received by name
 * Size includes the end of string character
 * Returns 1 if successful, 0 if the item does not contain such value or -1 on error
 */
#define libpff_message_get_utf16_received_by_name_size( message, utf16_string_size, error ) \
	libpff_message_get_entry_value_utf16_string_size( message, LIBPFF_ENTRY_TYPE_MESSAGE_RECEIVED_BY_NAME, utf16_string_size, error )

/* Retrieves the UTF-16 encoded message received by name
 * Returns 1 if successful, 0 if the item does not contain such value or -1 on error
 */
#define libpff_message_get_utf16_received_by_name( message, utf16_string, utf16_string_size, error ) \
	libpff_message_get_entry_value_utf16_string( message, LIBPFF_ENTRY_TYPE_MESSAGE_RECEIVED_BY_NAME, utf16_string, utf16_string_size, error )

/* Retrieves the size of the UTF-8 encoded message received by e-mail address
 * Size includes the end of string character
 * Returns 1 if successful, 0 if the item does not contain such value or -1 on error
 */
#define libpff_message_get_utf8_received_by_email_address_size( message, utf8_string_size, error ) \
	libpff_message_get_entry_value_utf8_string_size( message, LIBPFF_ENTRY_TYPE_MESSAGE_RECEIVED_BY_EMAIL_ADDRESS, utf8_string_size, error )

/* Retrieves the UTF-8 encoded message received by e-mail address
 * Returns 1 if successful, 0 if the item does not contain such value or -1 on error
 */
#define libpff_message_get_utf8_received_by_email_address( message, utf8_string, utf8_string_size, error ) \
	libpff_message_get_entry_value_utf8_string( message, LIBPFF_ENTRY_TYPE_MESSAGE_RECEIVED_BY_EMAIL_ADDRESS, utf8_string, utf8_string_size, error )

/* Retrieves the size of the UTF-16 encoded message received by e-mail address
 * Size includes the end of string character
 * Returns 1 if successful, 0 if the item does not contain such value or -1 on error
 */
#define libpff_message_get_utf16_received_by_email_address_size( message, utf16_string_size, error ) \
	libpff_message_get_entry_value_utf16_string_size( message, LIBPFF_ENTRY_TYPE_MESSAGE_RECEIVED_BY_EMAIL_ADDRESS, utf16_string_size, error )

/* Retrieves the UTF-16 encoded message received by e-mail address
 * Returns 1 if successful, 0 if the item does not contain such value or -1 on error
 */
#define libpff_message_get_utf16_received_by_email_address( message, utf16_string, utf16_string_size, error ) \
	libpff_message_get_entry_value_utf16_string( message, LIBPFF_ENTRY_TYPE_MESSAGE_RECEIVED_BY_EMAIL_ADDRESS, utf16_string, utf16_string_size, error )

/* Retrieves the message importance
 * Returns 1 if successful, 0 if the item does not contain such value or -1 on error
 */
#define libpff_message_get_importance( message, importance, error ) \
	libpff_item_get_entry_value_32bit( message, 0, LIBPFF_ENTRY_TYPE_MESSAGE_IMPORTANCE, importance, 0, error )

/* Retrieves the message priority
 * Returns 1 if successful, 0 if the item does not contain such value or -1 on error
 */
#define libpff_message_get_priority( message, priority, error ) \
	libpff_item_get_entry_value_32bit( message, 0, LIBPFF_ENTRY_TYPE_MESSAGE_PRIORITY, priority, 0, error )

/* Retrieves the message sensitivity
 * Returns 1 if successful, 0 if the item does not contain such value or -1 on error
 */
#define libpff_message_get_sensitivity( message, sensitivity, error ) \
	libpff_item_get_entry_value_32bit( message, 0, LIBPFF_ENTRY_TYPE_MESSAGE_SENSITIVITY, sensitivity, 0, error )

/* Retrieves the message flags
 * Returns 1 if successful, 0 if the item does not contain such value or -1 on error
 */
#define libpff_message_get_flags( message, flags, error ) \
	libpff_item_get_entry_value_32bit( message, 0, LIBPFF_ENTRY_TYPE_MESSAGE_FLAGS, flags, 0, error )

/* Retrieves the message size
 * Returns 1 if successful, 0 if the item does not contain such value or -1 on error
 */
#define libpff_message_get_size( message, size, error ) \
	libpff_item_get_entry_value_32bit( message, 0, LIBPFF_ENTRY_TYPE_MESSAGE_SIZE, size, 0, error )

/* Retrieves the message client submit time
 * Returns 1 if successful, 0 if the item does not contain such value or -1 on error
 */
#define libpff_message_get_client_submit_time( message, client_submit_time, error ) \
	libpff_item_get_entry_value_filetime( message, 0, LIBPFF_ENTRY_TYPE_MESSAGE_CLIENT_SUBMIT_TIME, client_submit_time, 0, error )

/* Retrieves the message delivery time
 * Returns 1 if successful, 0 if the item does not contain such value or -1 on error
 */
#define libpff_message_get_delivery_time( message, delivery_time, error ) \
	libpff_item_get_entry_value_filetime( message, 0, LIBPFF_ENTRY_TYPE_MESSAGE_DELIVERY_TIME, delivery_time, 0, error )

/* Retrieves the message creation time
 * Returns 1 if successful, 0 if the item does not contain such value or -1 on error
 */
#define libpff_message_get_creation_time( message, creation_time, error ) \
	libpff_item_get_entry_value_filetime( message, 0, LIBPFF_ENTRY_TYPE_MESSAGE_CREATION_TIME, creation_time, 0, error )

/* Retrieves the message modification time
 * Returns 1 if successful, 0 if the item does not contain such value or -1 on error
 */
#define libpff_message_get_modification_time( message, modification_time, error ) \
	libpff_item_get_entry_value_filetime( message, 0, LIBPFF_ENTRY_TYPE_MESSAGE_MODIFICATION_TIME, modification_time, 0, error )

/* Retrieves the size of the UTF-8 encoded message transport headers
 * Size includes the end of string character
 * Returns 1 if successful, 0 if the item does not contain such value or -1 on error
 */
#define libpff_message_get_utf8_transport_headers_size( message, utf8_string_size, error ) \
	libpff_message_get_entry_value_utf8_string_size( message, LIBPFF_ENTRY_TYPE_MESSAGE_TRANSPORT_HEADERS, utf8_string_size, error )

/* Retrieves the UTF-8 encoded message transport headers
 * Size should include the end of string character
 * Returns 1 if successful, 0 if the item does not contain such value or -1 on error
 */
#define libpff_message_get_utf8_transport_headers( message, utf8_string, utf8_string_size, error ) \
	libpff_message_get_entry_value_utf8_string( message, LIBPFF_ENTRY_TYPE_MESSAGE_TRANSPORT_HEADERS, utf8_string, utf8_string_size, error )

/* Retrieves the size of the UTF-16 encoded message transport headers
 * Size includes the end of string character
 * Returns 1 if successful, 0 if the item does not contain such value or -1 on error
 */
#define libpff_message_get_utf16_transport_headers_size( message, utf16_string_size, error ) \
	libpff_message_get_entry_value_utf16_string_size( message, LIBPFF_ENTRY_TYPE_MESSAGE_TRANSPORT_HEADERS, utf16_string_size, error )

/* Retrieves the UTF-16 encoded message transport headers
 * Size should include the end of string character
 * Returns 1 if successful, 0 if the item does not contain such value or -1 on error
 */
#define libpff_message_get_utf16_transport_headers( message, utf16_string, utf16_string_size, error ) \
	libpff_message_get_entry_value_utf16_string( message, LIBPFF_ENTRY_TYPE_MESSAGE_TRANSPORT_HEADERS, utf16_string, utf16_string_size, error )

/* Retrieves the message plain text body size
 * Size includes the end of string character
 * Returns 1 if successful, 0 if the item does not contain such value or -1 on error
 */
LIBPFF_EXTERN \
int libpff_message_get_plain_text_body_size(
     libpff_item_t *message,
     size_t *size,
     libpff_error_t **error );

/* Retrieves the plain text message body
 * The body is encoded in UTF-8
 * The function uses a codepage if necessary, it tries to read PidTagInternetCodepage first,
 * after that PidTagMessageCodepage and if not available it uses the codepage set for the library
 * Size should include the end of string character
 * Returns 1 if successful, 0 if the item does not contain such value or -1 on error
 */
LIBPFF_EXTERN \
int libpff_message_get_plain_text_body(
     libpff_item_t *message,
     uint8_t *message_body,
     size_t size,
     libpff_error_t **error );

/* Retrieves the message RTF body size
 * Size includes the end of string character
 * Returns 1 if successful, 0 if the item does not contain such value or -1 on error
 */
LIBPFF_EXTERN \
int libpff_message_get_rtf_body_size(
     libpff_item_t *message,
     size_t *size,
     libpff_error_t **error );

/* Retrieves the RTF message body
 * The body is encoded in UTF-8
 * Size should include the end of string character
 * Returns 1 if successful, 0 if the item does not contain such value or -1 on error
 */
LIBPFF_EXTERN \
int libpff_message_get_rtf_body(
     libpff_item_t *message,
     uint8_t *message_body,
     size_t size,
     libpff_error_t **error );

/* Retrieves the message HTML body size
 * Size includes the end of string character
 * Returns 1 if successful, 0 if the item does not contain such value or -1 on error
 */
LIBPFF_EXTERN \
int libpff_message_get_html_body_size(
     libpff_item_t *message,
     size_t *size,
     libpff_error_t **error );

/* Retrieves the HTML message body
 * Size should include the end of string character
 * Returns 1 if successful, 0 if the item does not contain such value or -1 on error
 */
LIBPFF_EXTERN \
int libpff_message_get_html_body(
     libpff_item_t *message,
     uint8_t *message_body,
     size_t size,
     libpff_error_t **error );

/* Retrieves the message reminder time
 * Returns 1 if successful, 0 if the item does not contain such value or -1 on error
 */
#define libpff_message_get_reminder_time( message, reminder_time, error ) \
	libpff_item_get_entry_value_filetime( message, 0, LIBPFF_ENTRY_TYPE_MESSAGE_REMINDER_TIME, reminder_time, 0, error )

/* Retrieves the value indicating if the message is a reminder
 * Returns 1 if successful, 0 if the item does not contain such value or -1 on error
 */
#define libpff_message_get_is_reminder( message, is_reminder, error ) \
	libpff_item_get_entry_value_boolean( message, 0, LIBPFF_ENTRY_TYPE_MESSAGE_IS_REMINDER, is_reminder, 0, error )

/* Retrieves the value indicating if the message is private
 * Returns 1 if successful, 0 if the item does not contain such value or -1 on error
 */
#define libpff_message_get_is_private( message, is_private, error ) \
	libpff_item_get_entry_value_boolean( message, 0, LIBPFF_ENTRY_TYPE_MESSAGE_IS_PRIVATE, is_private, 0, error )

/* Retrieves the message reminder signal time
 * Returns 1 if successful, 0 if the item does not contain such value or -1 on error
 */
#define libpff_message_get_reminder_signal_time( message, reminder_signal_time, error ) \
	libpff_item_get_entry_value_filetime( message, 0, LIBPFF_ENTRY_TYPE_MESSAGE_REMINDER_SIGNAL_TIME, reminder_signal_time, 0, error )

/* -------------------------------------------------------------------------
 * Task functions
 * ------------------------------------------------------------------------- */

/* Retrieves the task status
 * Returns 1 if successful, 0 if the item does not contain such value or -1 on error
 */
#define libpff_task_get_status( task, status, error ) \
	libpff_item_get_entry_value_32bit( task, 0, LIBPFF_ENTRY_TYPE_TASK_STATUS, status, 0, error )

/* Retrieves the task percentage complete
 * Returns 1 if successful, 0 if the item does not contain such value or -1 on error
 */
#define libpff_task_get_percentage_complete( task, percentage_complete, error ) \
	libpff_item_get_entry_value_floating_point( task, 0, LIBPFF_ENTRY_TYPE_TASK_PERCENTAGE_COMPLETE, percentage_complete, 0, error )

/* Retrieves the task start date
 * Returns 1 if successful, 0 if the item does not contain such value or -1 on error
 */
#define libpff_task_start_date( task, start_date, error ) \
	libpff_item_get_entry_value_filetime( task, 0, LIBPFF_ENTRY_TYPE_TASK_START_DATE, start_date, 0, error )

/* Retrieves the task due date
 * Returns 1 if successful, 0 if the item does not contain such value or -1 on error
 */
#define libpff_task_due_date( task, due_date, error ) \
	libpff_item_get_entry_value_filetime( task, 0, LIBPFF_ENTRY_TYPE_TASK_DUE_DATE, due_date, 0, error )

/* Retrieves the task actual effort in minutes
 * Returns 1 if successful, 0 if the item does not contain such value or -1 on error
 */
#define libpff_task_get_actual_effort( task, actual_effort, error ) \
	libpff_item_get_entry_value_32bit( task, 0, LIBPFF_ENTRY_TYPE_TASK_ACTUAL_EFFORT, actual_effort, 0, error )

/* Retrieves the task total effort in minutes
 * Returns 1 if successful, 0 if the item does not contain such value or -1 on error
 */
#define libpff_task_get_total_effort( task, total_effort, error ) \
	libpff_item_get_entry_value_32bit( task, 0, LIBPFF_ENTRY_TYPE_TASK_TOTAL_EFFORT, total_effort, 0, error )

/* Retrieves the task version
 * Returns 1 if successful, 0 if the item does not contain such value or -1 on error
 */
#define libpff_task_get_version( task, version, error ) \
	libpff_item_get_entry_value_32bit( task, 0, LIBPFF_ENTRY_TYPE_TASK_VERSION, version, 0, error )

/* Retrieves the value indicating if the task is complete
 * Returns 1 if successful, 0 if the item does not contain such value or -1 on error
 */
#define libpff_task_get_is_complete( task, is_complete, error ) \
	libpff_item_get_entry_value_boolean( task, 0, LIBPFF_ENTRY_TYPE_TASK_IS_COMPLETE, is_complete, 0, error )

/* Retrieves the value indicating if the task is recurring
 * Returns 1 if successful, 0 if the item does not contain such value or -1 on error
 */
#define libpff_task_get_is_recurring( task, is_recurring, error ) \
	libpff_item_get_entry_value_boolean( task, 0, LIBPFF_ENTRY_TYPE_TASK_IS_RECURRING, is_recurring, 0, error )

/* -------------------------------------------------------------------------
 * Appointment functions
 * ------------------------------------------------------------------------- */

/* Retrieves the appointment busy status
 * Returns 1 if successful, 0 if the item does not contain such value or -1 on error
 */
#define libpff_appointment_get_busy_status( appointment, busy_status, error ) \
	libpff_item_get_entry_value_32bit( appointment, 0, LIBPFF_ENTRY_TYPE_APPOINTMENT_BUSY_STATUS, busy_status, 0, error )

/* Retrieves the size of the UTF-8 encoded appointment location
 * Size includes the end of string character
 * Returns 1 if successful, 0 if the item does not contain such value or -1 on error
 */
#define libpff_appointment_get_utf8_location_size( appointment, utf8_string_size, error ) \
	libpff_message_get_entry_value_utf8_string_size( appointment, LIBPFF_ENTRY_TYPE_APPOINTMENT_LOCATION, utf8_string_size, error )

/* Retrieves the UTF-8 encoded appointment location
 * Returns 1 if successful, 0 if the item does not contain such value or -1 on error
 */
#define libpff_appointment_get_utf8_location( appointment, utf8_string, utf8_string_size, error ) \
	libpff_message_get_entry_value_utf8_string( appointment, LIBPFF_ENTRY_TYPE_APPOINTMENT_LOCATION, utf8_string, utf8_string_size, error )

/* Retrieves the size of the UTF-16 encoded appointment location
 * Size includes the end of string character
 * Returns 1 if successful, 0 if the item does not contain such value or -1 on error
 */
#define libpff_appointment_get_utf16_location_size( appointment, utf16_string_size, error ) \
	libpff_message_get_entry_value_utf16_string_size( appointment, LIBPFF_ENTRY_TYPE_APPOINTMENT_LOCATION, utf16_string_size, error )

/* Retrieves the UTF-16 encoded appointment location
 * Returns 1 if successful, 0 if the item does not contain such value or -1 on error
 */
#define libpff_appointment_get_utf16_location( appointment, utf16_string, utf16_string_size, error ) \
	libpff_message_get_entry_value_utf16_string( appointment, LIBPFF_ENTRY_TYPE_APPOINTMENT_LOCATION, utf16_string, utf16_string_size, error )

/* Retrieves the appointment start time
 * Returns 1 if successful, 0 if the item does not contain such value or -1 on error
 */
#define libpff_appointment_get_start_time( appointment, start_time, error ) \
	libpff_item_get_entry_value_filetime( appointment, 0, LIBPFF_ENTRY_TYPE_APPOINTMENT_START_TIME, start_time, 0, error )

/* Retrieves the appointment end time
 * Returns 1 if successful, 0 if the item does not contain such value or -1 on error
 */
#define libpff_appointment_get_end_time( appointment, end_time, error ) \
	libpff_item_get_entry_value_filetime( appointment, 0, LIBPFF_ENTRY_TYPE_APPOINTMENT_END_TIME, end_time, 0, error )

/* Retrieves the appointment duration in minutes
 * Returns 1 if successful, 0 if the item does not contain such value or -1 on error
 */
#define libpff_appointment_get_duration( appointment, duration, error ) \
	libpff_item_get_entry_value_32bit( appointment, 0, LIBPFF_ENTRY_TYPE_APPOINTMENT_DURATION, duration, 0, error )

/* Retrieves the size of the UTF-8 encoded appointment recurrence pattern
 * Size includes the end of string character
 * Returns 1 if successful, 0 if the item does not contain such value or -1 on error
 */
#define libpff_appointment_get_utf8_recurrence_pattern_size( appointment, utf8_string_size, error ) \
	libpff_message_get_entry_value_utf8_string_size( appointment, LIBPFF_ENTRY_TYPE_APPOINTMENT_RECURRENCE_PATTERN, utf8_string_size, error )

/* Retrieves the UTF-8 encoded appointment recurrence pattern
 * Returns 1 if successful, 0 if the item does not contain such value or -1 on error
 */
#define libpff_appointment_get_utf8_recurrence_pattern( appointment, utf8_string, utf8_string_size, error ) \
	libpff_message_get_entry_value_utf8_string( appointment, LIBPFF_ENTRY_TYPE_APPOINTMENT_RECURRENCE_PATTERN, utf8_string, utf8_string_size, error )

/* Retrieves the size of the UTF-16 encoded appointment recurrence pattern
 * Size includes the end of string character
 * Returns 1 if successful, 0 if the item does not contain such value or -1 on error
 */
#define libpff_appointment_get_utf16_recurrence_pattern_size( appointment, utf16_string_size, error ) \
	libpff_message_get_entry_value_utf16_string_size( appointment, LIBPFF_ENTRY_TYPE_APPOINTMENT_RECURRENCE_PATTERN, utf16_string_size, error )

/* Retrieves the UTF-16 encoded appointment recurrence pattern
 * Returns 1 if successful, 0 if the item does not contain such value or -1 on error
 */
#define libpff_appointment_get_utf16_recurrence_pattern( appointment, utf16_string, utf16_string_size, error ) \
	libpff_message_get_entry_value_utf16_string( appointment, LIBPFF_ENTRY_TYPE_APPOINTMENT_RECURRENCE_PATTERN, utf16_string, utf16_string_size, error )

/* Retrieves the size of the UTF-8 encoded appointment timezone description
 * Size includes the end of string character
 * Returns 1 if successful, 0 if the item does not contain such value or -1 on error
 */
#define libpff_appointment_get_utf8_timezone_description_size( appointment, utf8_string_size, error ) \
	libpff_message_get_entry_value_utf8_string_size( appointment, LIBPFF_ENTRY_TYPE_APPOINTMENT_TIMEZONE_DESCRIPTION, utf8_string_size, error )

/* Retrieves the UTF-8 encoded appointment timezone description
 * Returns 1 if successful, 0 if the item does not contain such value or -1 on error
 */
#define libpff_appointment_get_utf8_timezone_description( appointment, utf8_string, utf8_string_size, error ) \
	libpff_message_get_entry_value_utf8_string( appointment, LIBPFF_ENTRY_TYPE_APPOINTMENT_TIMEZONE_DESCRIPTION, utf8_string, utf8_string_size, error )

/* Retrieves the size of the UTF-16 encoded appointment timezone description
 * Size includes the end of string character
 * Returns 1 if successful, 0 if the item does not contain such value or -1 on error
 */
#define libpff_appointment_get_utf16_timezone_description_size( appointment, utf16_string_size, error ) \
	libpff_message_get_entry_value_utf16_string_size( appointment, LIBPFF_ENTRY_TYPE_APPOINTMENT_TIMEZONE_DESCRIPTION, utf16_string_size, error )

/* Retrieves the UTF-16 encoded appointment timezone description
 * Returns 1 if successful, 0 if the item does not contain such value or -1 on error
 */
#define libpff_appointment_get_utf16_timezone_description( appointment, utf16_string, utf16_string_size, error ) \
	libpff_message_get_entry_value_utf16_string( appointment, LIBPFF_ENTRY_TYPE_APPOINTMENT_TIMEZONE_DESCRIPTION, utf16_string, utf16_string_size, error )

/* Retrieves the appointment first effective time
 * Returns 1 if successful, 0 if the item does not contain such value or -1 on error
 */
#define libpff_appointment_first_effective_time( appointment, first_effective_time, error ) \
	libpff_item_get_entry_value_filetime( appointment, 0, LIBPFF_ENTRY_TYPE_APPOINTMENT_FIRST_EFFECTIVE_TIME, first_effective_time, 0, error )

/* Retrieves the appointment last effective time
 * Returns 1 if successful, 0 if the item does not contain such value or -1 on error
 */
#define libpff_appointment_last_effective_time( appointment, last_effective_time, error ) \
	libpff_item_get_entry_value_filetime( appointment, 0, LIBPFF_ENTRY_TYPE_APPOINTMENT_LAST_EFFECTIVE_TIME, last_effective_time, 0, error )

/* -------------------------------------------------------------------------
 * Address functions
 * ------------------------------------------------------------------------- */

/* Retrieves the size of the UTF-8 encoded address file under
 * Size includes the end of string character
 * Returns 1 if successful, 0 if the item does not contain such value or -1 on error
 */
#define libpff_address_get_utf8_file_under_size( address, utf8_string_size, error ) \
	libpff_message_get_entry_value_utf8_string_size( address, LIBPFF_ENTRY_TYPE_ADDRESS_FILE_UNDER, utf8_string_size, error )

/* Retrieves the UTF-8 encoded address file under
 * Returns 1 if successful, 0 if the item does not contain such value or -1 on error
 */
#define libpff_address_get_utf8_file_under( address, utf8_string, utf8_string_size, error ) \
	libpff_message_get_entry_value_utf8_string( address, LIBPFF_ENTRY_TYPE_ADDRESS_FILE_UNDER, utf8_string, utf8_string_size, error )

/* Retrieves the size of the UTF-16 encoded address file under
 * Size includes the end of string character
 * Returns 1 if successful, 0 if the item does not contain such value or -1 on error
 */
#define libpff_address_get_utf16_file_under_size( address, utf16_string_size, error ) \
	libpff_message_get_entry_value_utf16_string_size( address, LIBPFF_ENTRY_TYPE_ADDRESS_FILE_UNDER, utf16_string_size, error )

/* Retrieves the UTF-16 encoded address file under
 * Returns 1 if successful, 0 if the item does not contain such value or -1 on error
 */
#define libpff_address_get_utf16_file_under( address, utf16_string, utf16_string_size, error ) \
	libpff_message_get_entry_value_utf16_string( address, LIBPFF_ENTRY_TYPE_ADDRESS_FILE_UNDER, utf16_string, utf16_string_size, error )

/* -------------------------------------------------------------------------
 * Contact functions
 * ------------------------------------------------------------------------- */

/* Retrieves the size of the UTF-8 encoded contact generational abbreviation
 * Size includes the end of string character
 * Returns 1 if successful, 0 if the item does not contain such value or -1 on error
 */
#define libpff_contact_get_utf8_generational_abbreviation_size( contact, utf8_string_size, error ) \
	libpff_message_get_entry_value_utf8_string_size( contact, LIBPFF_ENTRY_TYPE_CONTACT_GENERATIONAL_ABBREVIATION, utf8_string_size, error )

/* Retrieves the UTF-8 encoded contact generational abbreviation name
 * Returns 1 if successful, 0 if the item does not contain such value or -1 on error
 */
#define libpff_contact_get_utf8_generational_abbreviation( contact, utf8_string, utf8_string_size, error ) \
	libpff_message_get_entry_value_utf8_string( contact, LIBPFF_ENTRY_TYPE_CONTACT_GENERATIONAL_ABBREVIATION, utf8_string, utf8_string_size, error )

/* Retrieves the size of the UTF-16 encoded contact generational abbreviation
 * Size includes the end of string character
 * Returns 1 if successful, 0 if the item does not contain such value or -1 on error
 */
#define libpff_contact_get_utf16_generational_abbreviation_size( contact, utf16_string_size, error ) \
	libpff_message_get_entry_value_utf16_string_size( contact, LIBPFF_ENTRY_TYPE_CONTACT_GENERATIONAL_ABBREVIATION, utf16_string_size, error )

/* Retrieves the UTF-16 encoded contact generational abbreviation name
 * Returns 1 if successful, 0 if the item does not contain such value or -1 on error
 */
#define libpff_contact_get_utf16_generational_abbreviation( contact, utf16_string, utf16_string_size, error ) \
	libpff_message_get_entry_value_utf16_string( contact, LIBPFF_ENTRY_TYPE_CONTACT_GENERATIONAL_ABBREVIATION, utf16_string, utf16_string_size, error )

/* Retrieves the size of the UTF-8 encoded contact given name
 * Size includes the end of string character
 * Returns 1 if successful, 0 if the item does not contain such value or -1 on error
 */
#define libpff_contact_get_utf8_given_name_size( contact, utf8_string_size, error ) \
	libpff_message_get_entry_value_utf8_string_size( contact, LIBPFF_ENTRY_TYPE_CONTACT_GIVEN_NAME, utf8_string_size, error )

/* Retrieves the UTF-8 encoded contact given name
 * Returns 1 if successful, 0 if the item does not contain such value or -1 on error
 */
#define libpff_contact_get_utf8_given_name( contact, utf8_string, utf8_string_size, error ) \
	libpff_message_get_entry_value_utf8_string( contact, LIBPFF_ENTRY_TYPE_CONTACT_GIVEN_NAME, utf8_string, utf8_string_size, error )

/* Retrieves the size of the UTF-16 encoded contact given name
 * Size includes the end of string character
 * Returns 1 if successful, 0 if the item does not contain such value or -1 on error
 */
#define libpff_contact_get_utf16_given_name_size( contact, utf16_string_size, error ) \
	libpff_message_get_entry_value_utf16_string_size( contact, LIBPFF_ENTRY_TYPE_CONTACT_GIVEN_NAME, utf16_string_size, error )

/* Retrieves the UTF-16 encoded contact given name
 * Returns 1 if successful, 0 if the item does not contain such value or -1 on error
 */
#define libpff_contact_get_utf16_given_name( contact, utf16_string, utf16_string_size, error ) \
	libpff_message_get_entry_value_utf16_string( contact, LIBPFF_ENTRY_TYPE_CONTACT_GIVEN_NAME, utf16_string, utf16_string_size, error )

/* Retrieves the size of the UTF-8 encoded contact initials
 * Size includes the end of string character
 * Returns 1 if successful, 0 if the item does not contain such value or -1 on error
 */
#define libpff_contact_get_utf8_initials_size( contact, utf8_string_size, error ) \
	libpff_message_get_entry_value_utf8_string_size( contact, LIBPFF_ENTRY_TYPE_CONTACT_INITIALS, utf8_string_size, error )

/* Retrieves the UTF-8 encoded contact initials
 * Returns 1 if successful, 0 if the item does not contain such value or -1 on error
 */
#define libpff_contact_get_utf8_initials( contact, utf8_string, utf8_string_size, error ) \
	libpff_message_get_entry_value_utf8_string( contact, LIBPFF_ENTRY_TYPE_CONTACT_INITIALS, utf8_string, utf8_string_size, error )

/* Retrieves the size of the UTF-16 encoded contact initials
 * Size includes the end of string character
 * Returns 1 if successful, 0 if the item does not contain such value or -1 on error
 */
#define libpff_contact_get_utf16_initials_size( contact, utf16_string_size, error ) \
	libpff_message_get_entry_value_utf16_string_size( contact, LIBPFF_ENTRY_TYPE_CONTACT_INITIALS, utf16_string_size, error )

/* Retrieves the UTF-16 encoded contact initials
 * Returns 1 if successful, 0 if the item does not contain such value or -1 on error
 */
#define libpff_contact_get_utf16_initials( contact, utf16_string, utf16_string_size, error ) \
	libpff_message_get_entry_value_utf16_string( contact, LIBPFF_ENTRY_TYPE_CONTACT_INITIALS, utf16_string, utf16_string_size, error )

/* Retrieves the size of the UTF-8 encoded contact surname
 * Size includes the end of string character
 * Returns 1 if successful, 0 if the item does not contain such value or -1 on error
 */
#define libpff_contact_get_utf8_surname_size( contact, utf8_string_size, error ) \
	libpff_message_get_entry_value_utf8_string_size( contact, LIBPFF_ENTRY_TYPE_CONTACT_SURNAME, utf8_string_size, error )

/* Retrieves the UTF-8 encoded contact surname
 * Returns 1 if successful, 0 if the item does not contain such value or -1 on error
 */
#define libpff_contact_get_utf8_surname( contact, utf8_string, utf8_string_size, error ) \
	libpff_message_get_entry_value_utf8_string( contact, LIBPFF_ENTRY_TYPE_CONTACT_SURNAME, utf8_string, utf8_string_size, error )

/* Retrieves the size of the UTF-16 encoded contact surname
 * Size includes the end of string character
 * Returns 1 if successful, 0 if the item does not contain such value or -1 on error
 */
#define libpff_contact_get_utf16_surname_size( contact, utf16_string_size, error ) \
	libpff_message_get_entry_value_utf16_string_size( contact, LIBPFF_ENTRY_TYPE_CONTACT_SURNAME, utf16_string_size, error )

/* Retrieves the UTF-16 encoded contact surname
 * Returns 1 if successful, 0 if the item does not contain such value or -1 on error
 */
#define libpff_contact_get_utf16_surname( contact, utf16_string, utf16_string_size, error ) \
	libpff_message_get_entry_value_utf16_string( contact, LIBPFF_ENTRY_TYPE_CONTACT_SURNAME, utf16_string, utf16_string_size, error )

/* Retrieves the size of the UTF-8 encoded contact postal address
 * Size includes the end of string character
 * Returns 1 if successful, 0 if the item does not contain such value or -1 on error
 */
#define libpff_contact_get_utf8_postal_address_size( contact, utf8_string_size, error ) \
	libpff_message_get_entry_value_utf8_string_size( contact, LIBPFF_ENTRY_TYPE_CONTACT_POSTAL_ADDRESS, utf8_string_size, error )

/* Retrieves the UTF-8 encoded contact postal address
 * Returns 1 if successful, 0 if the item does not contain such value or -1 on error
 */
#define libpff_contact_get_utf8_postal_address( contact, utf8_string, utf8_string_size, error ) \
	libpff_message_get_entry_value_utf8_string( contact, LIBPFF_ENTRY_TYPE_CONTACT_POSTAL_ADDRESS, utf8_string, utf8_string_size, error )

/* Retrieves the size of the UTF-16 encoded contact postal address
 * Size includes the end of string character
 * Returns 1 if successful, 0 if the item does not contain such value or -1 on error
 */
#define libpff_contact_get_utf16_postal_address_size( contact, utf16_string_size, error ) \
	libpff_message_get_entry_value_utf16_string_size( contact, LIBPFF_ENTRY_TYPE_CONTACT_POSTAL_ADDRESS, utf16_string_size, error )

/* Retrieves the UTF-16 encoded contact postal address
 * Returns 1 if successful, 0 if the item does not contain such value or -1 on error
 */
#define libpff_contact_get_utf16_postal_address( contact, utf16_string, utf16_string_size, error ) \
	libpff_message_get_entry_value_utf16_string( contact, LIBPFF_ENTRY_TYPE_CONTACT_POSTAL_ADDRESS, utf16_string, utf16_string_size, error )

/* Retrieves the size of the UTF-8 encoded contact company name
 * Size includes the end of string character
 * Returns 1 if successful, 0 if the item does not contain such value or -1 on error
 */
#define libpff_contact_get_utf8_company_name_size( contact, utf8_string_size, error ) \
	libpff_message_get_entry_value_utf8_string_size( contact, LIBPFF_ENTRY_TYPE_CONTACT_COMPANY_NAME, utf8_string_size, error )

/* Retrieves the UTF-8 encoded contact company name
 * Returns 1 if successful, 0 if the item does not contain such value or -1 on error
 */
#define libpff_contact_get_utf8_company_name( contact, utf8_string, utf8_string_size, error ) \
	libpff_message_get_entry_value_utf8_string( contact, LIBPFF_ENTRY_TYPE_CONTACT_COMPANY_NAME, utf8_string, utf8_string_size, error )

/* Retrieves the size of the UTF-16 encoded contact company name
 * Size includes the end of string character
 * Returns 1 if successful, 0 if the item does not contain such value or -1 on error
 */
#define libpff_contact_get_utf16_company_name_size( contact, utf16_string_size, error ) \
	libpff_message_get_entry_value_utf16_string_size( contact, LIBPFF_ENTRY_TYPE_CONTACT_COMPANY_NAME, utf16_string_size, error )

/* Retrieves the UTF-16 encoded contact company name
 * Returns 1 if successful, 0 if the item does not contain such value or -1 on error
 */
#define libpff_contact_get_utf16_company_name( contact, utf16_string, utf16_string_size, error ) \
	libpff_message_get_entry_value_utf16_string( contact, LIBPFF_ENTRY_TYPE_CONTACT_COMPANY_NAME, utf16_string, utf16_string_size, error )

/* Retrieves the size of the UTF-8 encoded contact job title
 * Size includes the end of string character
 * Returns 1 if successful, 0 if the item does not contain such value or -1 on error
 */
#define libpff_contact_get_utf8_job_title_size( contact, utf8_string_size, error ) \
	libpff_message_get_entry_value_utf8_string_size( contact, LIBPFF_ENTRY_TYPE_CONTACT_JOB_TITLE, utf8_string_size, error )

/* Retrieves the UTF-8 encoded contact job title
 * Returns 1 if successful, 0 if the item does not contain such value or -1 on error
 */
#define libpff_contact_get_utf8_job_title( contact, utf8_string, utf8_string_size, error ) \
	libpff_message_get_entry_value_utf8_string( contact, LIBPFF_ENTRY_TYPE_CONTACT_JOB_TITLE, utf8_string, utf8_string_size, error )

/* Retrieves the size of the UTF-16 encoded contact job title
 * Size includes the end of string character
 * Returns 1 if successful, 0 if the item does not contain such value or -1 on error
 */
#define libpff_contact_get_utf16_job_title_size( contact, utf16_string_size, error ) \
	libpff_message_get_entry_value_utf16_string_size( contact, LIBPFF_ENTRY_TYPE_CONTACT_JOB_TITLE, utf16_string_size, error )

/* Retrieves the UTF-16 encoded contact job title
 * Returns 1 if successful, 0 if the item does not contain such value or -1 on error
 */
#define libpff_contact_get_utf16_job_title( contact, utf16_string, utf16_string_size, error ) \
	libpff_message_get_entry_value_utf16_string( contact, LIBPFF_ENTRY_TYPE_CONTACT_JOB_TITLE, utf16_string, utf16_string_size, error )

/* Retrieves the size of the UTF-8 encoded contact department name
 * Size includes the end of string character
 * Returns 1 if successful, 0 if the item does not contain such value or -1 on error
 */
#define libpff_contact_get_utf8_department_name_size( contact, utf8_string_size, error ) \
	libpff_message_get_entry_value_utf8_string_size( contact, LIBPFF_ENTRY_TYPE_CONTACT_DEPARTMENT_NAME, utf8_string_size, error )

/* Retrieves the UTF-8 encoded contact department name
 * Returns 1 if successful, 0 if the item does not contain such value or -1 on error
 */
#define libpff_contact_get_utf8_department_name( contact, utf8_string, utf8_string_size, error ) \
	libpff_message_get_entry_value_utf8_string( contact, LIBPFF_ENTRY_TYPE_CONTACT_DEPARTMENT_NAME, utf8_string, utf8_string_size, error )

/* Retrieves the size of the UTF-16 encoded contact department name
 * Size includes the end of string character
 * Returns 1 if successful, 0 if the item does not contain such value or -1 on error
 */
#define libpff_contact_get_utf16_department_name_size( contact, utf16_string_size, error ) \
	libpff_message_get_entry_value_utf16_string_size( contact, LIBPFF_ENTRY_TYPE_CONTACT_DEPARTMENT_NAME, utf16_string_size, error )

/* Retrieves the UTF-16 encoded contact department name
 * Returns 1 if successful, 0 if the item does not contain such value or -1 on error
 */
#define libpff_contact_get_utf16_department_name( contact, utf16_string, utf16_string_size, error ) \
	libpff_message_get_entry_value_utf16_string( contact, LIBPFF_ENTRY_TYPE_CONTACT_DEPARTMENT_NAME, utf16_string, utf16_string_size, error )

/* Retrieves the size of the UTF-8 encoded contact office location
 * Size includes the end of string character
 * Returns 1 if successful, 0 if the item does not contain such value or -1 on error
 */
#define libpff_contact_get_utf8_office_location_size( contact, utf8_string_size, error ) \
	libpff_message_get_entry_value_utf8_string_size( contact, LIBPFF_ENTRY_TYPE_CONTACT_OFFICE_LOCATION, utf8_string_size, error )

/* Retrieves the UTF-8 encoded contact office location
 * Returns 1 if successful, 0 if the item does not contain such value or -1 on error
 */
#define libpff_contact_get_utf8_office_location( contact, utf8_string, utf8_string_size, error ) \
	libpff_message_get_entry_value_utf8_string( contact, LIBPFF_ENTRY_TYPE_CONTACT_OFFICE_LOCATION, utf8_string, utf8_string_size, error )

/* Retrieves the size of the UTF-16 encoded contact office location
 * Size includes the end of string character
 * Returns 1 if successful, 0 if the item does not contain such value or -1 on error
 */
#define libpff_contact_get_utf16_office_location_size( contact, utf16_string_size, error ) \
	libpff_message_get_entry_value_utf16_string_size( contact, LIBPFF_ENTRY_TYPE_CONTACT_OFFICE_LOCATION, utf16_string_size, error )

/* Retrieves the UTF-16 encoded contact office location
 * Returns 1 if successful, 0 if the item does not contain such value or -1 on error
 */
#define libpff_contact_get_utf16_office_location( contact, utf16_string, utf16_string_size, error ) \
	libpff_message_get_entry_value_utf16_string( contact, LIBPFF_ENTRY_TYPE_CONTACT_OFFICE_LOCATION, utf16_string, utf16_string_size, error )

/* Retrieves the size of the UTF-8 encoded contact callback phone number
 * Size includes the end of string character
 * Returns 1 if successful, 0 if the item does not contain such value or -1 on error
 */
#define libpff_contact_get_utf8_callback_phone_number_size( contact, utf8_string_size, error ) \
	libpff_message_get_entry_value_utf8_string_size( contact, LIBPFF_ENTRY_TYPE_CONTACT_CALLBACK_PHONE_NUMBER, utf8_string_size, error )

/* Retrieves the UTF-8 encoded contact callback phone number
 * Returns 1 if successful, 0 if the item does not contain such value or -1 on error
 */
#define libpff_contact_get_utf8_callback_phone_number( contact, utf8_string, utf8_string_size, error ) \
	libpff_message_get_entry_value_utf8_string( contact, LIBPFF_ENTRY_TYPE_CONTACT_CALLBACK_PHONE_NUMBER, utf8_string, utf8_string_size, error )

/* Retrieves the size of the UTF-16 encoded contact callback phone number
 * Size includes the end of string character
 * Returns 1 if successful, 0 if the item does not contain such value or -1 on error
 */
#define libpff_contact_get_utf16_callback_phone_number_size( contact, utf16_string_size, error ) \
	libpff_message_get_entry_value_utf16_string_size( contact, LIBPFF_ENTRY_TYPE_CONTACT_CALLBACK_PHONE_NUMBER, utf16_string_size, error )

/* Retrieves the UTF-16 encoded contact callback phone number
 * Returns 1 if successful, 0 if the item does not contain such value or -1 on error
 */
#define libpff_contact_get_utf16_callback_phone_number( contact, utf16_string, utf16_string_size, error ) \
	libpff_message_get_entry_value_utf16_string( contact, LIBPFF_ENTRY_TYPE_CONTACT_CALLBACK_PHONE_NUMBER, utf16_string, utf16_string_size, error )

/* Retrieves the size of the UTF-8 encoded contact primary phone number
 * Size includes the end of string character
 * Returns 1 if successful, 0 if the item does not contain such value or -1 on error
 */
#define libpff_contact_get_utf8_primary_phone_number_size( contact, utf8_string_size, error ) \
	libpff_message_get_entry_value_utf8_string_size( contact, LIBPFF_ENTRY_TYPE_CONTACT_PRIMARY_PHONE_NUMBER, utf8_string_size, error )

/* Retrieves the UTF-8 encoded contact primary phone number
 * Returns 1 if successful, 0 if the item does not contain such value or -1 on error
 */
#define libpff_contact_get_utf8_primary_phone_number( contact, utf8_string, utf8_string_size, error ) \
	libpff_message_get_entry_value_utf8_string( contact, LIBPFF_ENTRY_TYPE_CONTACT_PRIMARY_PHONE_NUMBER, utf8_string, utf8_string_size, error )

/* Retrieves the size of the UTF-16 encoded contact primary phone number
 * Size includes the end of string character
 * Returns 1 if successful, 0 if the item does not contain such value or -1 on error
 */
#define libpff_contact_get_utf16_primary_phone_number_size( contact, utf16_string_size, error ) \
	libpff_message_get_entry_value_utf16_string_size( contact, LIBPFF_ENTRY_TYPE_CONTACT_PRIMARY_PHONE_NUMBER, utf16_string_size, error )

/* Retrieves the UTF-16 encoded contact primary phone number
 * Returns 1 if successful, 0 if the item does not contain such value or -1 on error
 */
#define libpff_contact_get_utf16_primary_phone_number( contact, utf16_string, utf16_string_size, error ) \
	libpff_message_get_entry_value_utf16_string( contact, LIBPFF_ENTRY_TYPE_CONTACT_PRIMARY_PHONE_NUMBER, utf16_string, utf16_string_size, error )

/* Retrieves the size of the UTF-8 encoded contact home phone number
 * Size includes the end of string character
 * Returns 1 if successful, 0 if the item does not contain such value or -1 on error
 */
#define libpff_contact_get_utf8_home_phone_number_size( contact, utf8_string_size, error ) \
	libpff_message_get_entry_value_utf8_string_size( contact, LIBPFF_ENTRY_TYPE_CONTACT_HOME_PHONE_NUMBER, utf8_string_size, error )

/* Retrieves the UTF-8 encoded contact home phone number
 * Returns 1 if successful, 0 if the item does not contain such value or -1 on error
 */
#define libpff_contact_get_utf8_home_phone_number( contact, utf8_string, utf8_string_size, error ) \
	libpff_message_get_entry_value_utf8_string( contact, LIBPFF_ENTRY_TYPE_CONTACT_HOME_PHONE_NUMBER, utf8_string, utf8_string_size, error )

/* Retrieves the size of the UTF-16 encoded contact home phone number
 * Size includes the end of string character
 * Returns 1 if successful, 0 if the item does not contain such value or -1 on error
 */
#define libpff_contact_get_utf16_home_phone_number_size( contact, utf16_string_size, error ) \
	libpff_message_get_entry_value_utf16_string_size( contact, LIBPFF_ENTRY_TYPE_CONTACT_HOME_PHONE_NUMBER, utf16_string_size, error )

/* Retrieves the UTF-16 encoded contact home phone number
 * Returns 1 if successful, 0 if the item does not contain such value or -1 on error
 */
#define libpff_contact_get_utf16_home_phone_number( contact, utf16_string, utf16_string_size, error ) \
	libpff_message_get_entry_value_utf16_string( contact, LIBPFF_ENTRY_TYPE_CONTACT_HOME_PHONE_NUMBER, utf16_string, utf16_string_size, error )

/* Retrieves the size of the UTF-8 encoded contact primary business phone number
 * Size includes the end of string character
 * Returns 1 if successful, 0 if the item does not contain such value or -1 on error
 */
#define libpff_contact_get_utf8_business_phone_number_1_size( contact, utf8_string_size, error ) \
	libpff_message_get_entry_value_utf8_string_size( contact, LIBPFF_ENTRY_TYPE_CONTACT_BUSINESS_PHONE_NUMBER_1, utf8_string_size, error )

/* Retrieves the UTF-8 encoded contact primary business phone number
 * Returns 1 if successful, 0 if the item does not contain such value or -1 on error
 */
#define libpff_contact_get_utf8_business_phone_number_1( contact, utf8_string, utf8_string_size, error ) \
	libpff_message_get_entry_value_utf8_string( contact, LIBPFF_ENTRY_TYPE_CONTACT_BUSINESS_PHONE_NUMBER_1, utf8_string, utf8_string_size, error )

/* Retrieves the size of the UTF-16 encoded contact primary business phone number
 * Size includes the end of string character
 * Returns 1 if successful, 0 if the item does not contain such value or -1 on error
 */
#define libpff_contact_get_utf16_business_phone_number_1_size( contact, utf16_string_size, error ) \
	libpff_message_get_entry_value_utf16_string_size( contact, LIBPFF_ENTRY_TYPE_CONTACT_BUSINESS_PHONE_NUMBER_1, utf16_string_size, error )

/* Retrieves the UTF-16 encoded contact primary business phone number
 * Returns 1 if successful, 0 if the item does not contain such value or -1 on error
 */
#define libpff_contact_get_utf16_business_phone_number_1( contact, utf16_string, utf16_string_size, error ) \
	libpff_message_get_entry_value_utf16_string( contact, LIBPFF_ENTRY_TYPE_CONTACT_BUSINESS_PHONE_NUMBER_1, utf16_string, utf16_string_size, error )

/* Retrieves the size of the UTF-8 encoded contact secondary business phone number
 * Size includes the end of string character
 * Returns 1 if successful, 0 if the item does not contain such value or -1 on error
 */
#define libpff_contact_get_utf8_business_phone_number_2_size( contact, utf8_string_size, error ) \
	libpff_message_get_entry_value_utf8_string_size( contact, LIBPFF_ENTRY_TYPE_CONTACT_BUSINESS_PHONE_NUMBER_2, utf8_string_size, error )

/* Retrieves the UTF-8 encoded contact secondary business phone number
 * Returns 1 if successful, 0 if the item does not contain such value or -1 on error
 */
#define libpff_contact_get_utf8_business_phone_number_2( contact, utf8_string, utf8_string_size, error ) \
	libpff_message_get_entry_value_utf8_string( contact, LIBPFF_ENTRY_TYPE_CONTACT_BUSINESS_PHONE_NUMBER_2, utf8_string, utf8_string_size, error )

/* Retrieves the size of the UTF-16 encoded contact secondary business phone number
 * Size includes the end of string character
 * Returns 1 if successful, 0 if the item does not contain such value or -1 on error
 */
#define libpff_contact_get_utf16_business_phone_number_2_size( contact, utf16_string_size, error ) \
	libpff_message_get_entry_value_utf16_string_size( contact, LIBPFF_ENTRY_TYPE_CONTACT_BUSINESS_PHONE_NUMBER_2, utf16_string_size, error )

/* Retrieves the UTF-16 encoded contact secondary business phone number
 * Returns 1 if successful, 0 if the item does not contain such value or -1 on error
 */
#define libpff_contact_get_utf16_business_phone_number_2( contact, utf16_string, utf16_string_size, error ) \
	libpff_message_get_entry_value_utf16_string( contact, LIBPFF_ENTRY_TYPE_CONTACT_BUSINESS_PHONE_NUMBER_2, utf16_string, utf16_string_size, error )

/* Retrieves the size of the UTF-8 encoded contact mobile phone number
 * Size includes the end of string character
 * Returns 1 if successful, 0 if the item does not contain such value or -1 on error
 */
#define libpff_contact_get_utf8_mobile_phone_number_size( contact, utf8_string_size, error ) \
	libpff_message_get_entry_value_utf8_string_size( contact, LIBPFF_ENTRY_TYPE_CONTACT_MOBILE_PHONE_NUMBER, utf8_string_size, error )

/* Retrieves the UTF-8 encoded contact mobile phone number
 * Returns 1 if successful, 0 if the item does not contain such value or -1 on error
 */
#define libpff_contact_get_utf8_mobile_phone_number( contact, utf8_string, utf8_string_size, error ) \
	libpff_message_get_entry_value_utf8_string( contact, LIBPFF_ENTRY_TYPE_CONTACT_MOBILE_PHONE_NUMBER, utf8_string, utf8_string_size, error )

/* Retrieves the size of the UTF-16 encoded contact mobile phone number
 * Size includes the end of string character
 * Returns 1 if successful, 0 if the item does not contain such value or -1 on error
 */
#define libpff_contact_get_utf16_mobile_phone_number_size( contact, utf16_string_size, error ) \
	libpff_message_get_entry_value_utf16_string_size( contact, LIBPFF_ENTRY_TYPE_CONTACT_MOBILE_PHONE_NUMBER, utf16_string_size, error )

/* Retrieves the UTF-16 encoded contact mobile phone number
 * Returns 1 if successful, 0 if the item does not contain such value or -1 on error
 */
#define libpff_contact_get_utf16_mobile_phone_number( contact, utf16_string, utf16_string_size, error ) \
	libpff_message_get_entry_value_utf16_string( contact, LIBPFF_ENTRY_TYPE_CONTACT_MOBILE_PHONE_NUMBER, utf16_string, utf16_string_size, error )

/* Retrieves the size of the UTF-8 encoded contact business fax number
 * Size includes the end of string character
 * Returns 1 if successful, 0 if the item does not contain such value or -1 on error
 */
#define libpff_contact_get_utf8_business_fax_number_size( contact, utf8_string_size, error ) \
	libpff_message_get_entry_value_utf8_string_size( contact, LIBPFF_ENTRY_TYPE_CONTACT_BUSINESS_FAX_NUMBER, utf8_string_size, error )

/* Retrieves the UTF-8 encoded contact business fax number
 * Returns 1 if successful, 0 if the item does not contain such value or -1 on error
 */
#define libpff_contact_get_utf8_business_fax_number( contact, utf8_string, utf8_string_size, error ) \
	libpff_message_get_entry_value_utf8_string( contact, LIBPFF_ENTRY_TYPE_CONTACT_BUSINESS_FAX_NUMBER, utf8_string, utf8_string_size, error )

/* Retrieves the size of the UTF-16 encoded contact business fax number
 * Size includes the end of string character
 * Returns 1 if successful, 0 if the item does not contain such value or -1 on error
 */
#define libpff_contact_get_utf16_business_fax_number_size( contact, utf16_string_size, error ) \
	libpff_message_get_entry_value_utf16_string_size( contact, LIBPFF_ENTRY_TYPE_CONTACT_BUSINESS_FAX_NUMBER, utf16_string_size, error )

/* Retrieves the UTF-16 encoded contact business fax number
 * Returns 1 if successful, 0 if the item does not contain such value or -1 on error
 */
#define libpff_contact_get_utf16_business_fax_number( contact, utf16_string, utf16_string_size, error ) \
	libpff_message_get_entry_value_utf16_string( contact, LIBPFF_ENTRY_TYPE_CONTACT_BUSINESS_FAX_NUMBER, utf16_string, utf16_string_size, error )

/* Retrieves the size of the UTF-8 encoded contact country
 * Size includes the end of string character
 * Returns 1 if successful, 0 if the item does not contain such value or -1 on error
 */
#define libpff_contact_get_utf8_country_size( contact, utf8_string_size, error ) \
	libpff_message_get_entry_value_utf8_string_size( contact, LIBPFF_ENTRY_TYPE_CONTACT_COUNTRY, utf8_string_size, error )

/* Retrieves the UTF-8 encoded contact country
 * Returns 1 if successful, 0 if the item does not contain such value or -1 on error
 */
#define libpff_contact_get_utf8_country( contact, utf8_string, utf8_string_size, error ) \
	libpff_message_get_entry_value_utf8_string( contact, LIBPFF_ENTRY_TYPE_CONTACT_COUNTRY, utf8_string, utf8_string_size, error )

/* Retrieves the size of the UTF-16 encoded contact country
 * Size includes the end of string character
 * Returns 1 if successful, 0 if the item does not contain such value or -1 on error
 */
#define libpff_contact_get_utf16_country_size( contact, utf16_string_size, error ) \
	libpff_message_get_entry_value_utf16_string_size( contact, LIBPFF_ENTRY_TYPE_CONTACT_COUNTRY, utf16_string_size, error )

/* Retrieves the UTF-16 encoded contact country
 * Returns 1 if successful, 0 if the item does not contain such value or -1 on error
 */
#define libpff_contact_get_utf16_country( contact, utf16_string, utf16_string_size, error ) \
	libpff_message_get_entry_value_utf16_string( contact, LIBPFF_ENTRY_TYPE_CONTACT_COUNTRY, utf16_string, utf16_string_size, error )

/* Retrieves the size of the UTF-8 encoded contact locality
 * Size includes the end of string character
 * Returns 1 if successful, 0 if the item does not contain such value or -1 on error
 */
#define libpff_contact_get_utf8_locality_size( contact, utf8_string_size, error ) \
	libpff_message_get_entry_value_utf8_string_size( contact, LIBPFF_ENTRY_TYPE_CONTACT_LOCALITY, utf8_string_size, error )

/* Retrieves the UTF-8 encoded contact locality
 * Returns 1 if successful, 0 if the item does not contain such value or -1 on error
 */
#define libpff_contact_get_utf8_locality( contact, utf8_string, utf8_string_size, error ) \
	libpff_message_get_entry_value_utf8_string( contact, LIBPFF_ENTRY_TYPE_CONTACT_LOCALITY, utf8_string, utf8_string_size, error )

/* Retrieves the size of the UTF-16 encoded contact locality
 * Size includes the end of string character
 * Returns 1 if successful, 0 if the item does not contain such value or -1 on error
 */
#define libpff_contact_get_utf16_locality_size( contact, utf16_string_size, error ) \
	libpff_message_get_entry_value_utf16_string_size( contact, LIBPFF_ENTRY_TYPE_CONTACT_LOCALITY, utf16_string_size, error )

/* Retrieves the UTF-16 encoded contact locality
 * Returns 1 if successful, 0 if the item does not contain such value or -1 on error
 */
#define libpff_contact_get_utf16_locality( contact, utf16_string, utf16_string_size, error ) \
	libpff_message_get_entry_value_utf16_string( contact, LIBPFF_ENTRY_TYPE_CONTACT_LOCALITY, utf16_string, utf16_string_size, error )

/* Retrieves the size of the UTF-8 encoded contact title
 * Size includes the end of string character
 * Returns 1 if successful, 0 if the item does not contain such value or -1 on error
 */
#define libpff_contact_get_utf8_title_size( contact, utf8_string_size, error ) \
	libpff_message_get_entry_value_utf8_string_size( contact, LIBPFF_ENTRY_TYPE_CONTACT_TITLE, utf8_string_size, error )

/* Retrieves the UTF-8 encoded contact title
 * Returns 1 if successful, 0 if the item does not contain such value or -1 on error
 */
#define libpff_contact_get_utf8_title( contact, utf8_string, utf8_string_size, error ) \
	libpff_message_get_entry_value_utf8_string( contact, LIBPFF_ENTRY_TYPE_CONTACT_TITLE, utf8_string, utf8_string_size, error )

/* Retrieves the size of the UTF-16 encoded contact title
 * Size includes the end of string character
 * Returns 1 if successful, 0 if the item does not contain such value or -1 on error
 */
#define libpff_contact_get_utf16_title_size( contact, utf16_string_size, error ) \
	libpff_message_get_entry_value_utf16_string_size( contact, LIBPFF_ENTRY_TYPE_CONTACT_TITLE, utf16_string_size, error )

/* Retrieves the UTF-16 encoded contact title
 * Returns 1 if successful, 0 if the item does not contain such value or -1 on error
 */
#define libpff_contact_get_utf16_title( contact, utf16_string, utf16_string_size, error ) \
	libpff_message_get_entry_value_utf16_string( contact, LIBPFF_ENTRY_TYPE_CONTACT_TITLE, utf16_string, utf16_string_size, error )

/* Retrieves the size of the UTF-8 encoded contact primary email address
 * Size includes the end of string character
 * Returns 1 if successful, 0 if the item does not contain such value or -1 on error
 */
#define libpff_contact_get_utf8_email_address_1_size( contact, utf8_string_size, error ) \
	libpff_message_get_entry_value_utf8_string_size( contact, LIBPFF_ENTRY_TYPE_CONTACT_EMAIL_ADDRESS_1, utf8_string_size, error )

/* Retrieves the UTF-8 encoded contact primary email address
 * Returns 1 if successful, 0 if the item does not contain such value or -1 on error
 */
#define libpff_contact_get_utf8_email_address_1( contact, utf8_string, utf8_string_size, error ) \
	libpff_message_get_entry_value_utf8_string( contact, LIBPFF_ENTRY_TYPE_CONTACT_EMAIL_ADDRESS_1, utf8_string, utf8_string_size, error )

/* Retrieves the size of the UTF-16 encoded contact primary email address
 * Size includes the end of string character
 * Returns 1 if successful, 0 if the item does not contain such value or -1 on error
 */
#define libpff_contact_get_utf16_email_address_1_size( contact, utf16_string_size, error ) \
	libpff_message_get_entry_value_utf16_string_size( contact, LIBPFF_ENTRY_TYPE_CONTACT_EMAIL_ADDRESS_1, utf16_string_size, error )

/* Retrieves the UTF-16 encoded contact primary email address
 * Returns 1 if successful, 0 if the item does not contain such value or -1 on error
 */
#define libpff_contact_get_utf16_email_address_1( contact, utf16_string, utf16_string_size, error ) \
	libpff_message_get_entry_value_utf16_string( contact, LIBPFF_ENTRY_TYPE_CONTACT_EMAIL_ADDRESS_1, utf16_string, utf16_string_size, error )

/* Retrieves the size of the UTF-8 encoded contact secondary email address
 * Size includes the end of string character
 * Returns 1 if successful, 0 if the item does not contain such value or -1 on error
 */
#define libpff_contact_get_utf8_email_address_2_size( contact, utf8_string_size, error ) \
	libpff_message_get_entry_value_utf8_string_size( contact, LIBPFF_ENTRY_TYPE_CONTACT_EMAIL_ADDRESS_2, utf8_string_size, error )

/* Retrieves the UTF-8 encoded contact secondary email address
 * Returns 1 if successful, 0 if the item does not contain such value or -1 on error
 */
#define libpff_contact_get_utf8_email_address_2( contact, utf8_string, utf8_string_size, error ) \
	libpff_message_get_entry_value_utf8_string( contact, LIBPFF_ENTRY_TYPE_CONTACT_EMAIL_ADDRESS_2, utf8_string, utf8_string_size, error )

/* Retrieves the size of the UTF-16 encoded contact secondary email address
 * Size includes the end of string character
 * Returns 1 if successful, 0 if the item does not contain such value or -1 on error
 */
#define libpff_contact_get_utf16_email_address_2_size( contact, utf16_string_size, error ) \
	libpff_message_get_entry_value_utf16_string_size( contact, LIBPFF_ENTRY_TYPE_CONTACT_EMAIL_ADDRESS_2, utf16_string_size, error )

/* Retrieves the UTF-16 encoded contact secondary email address
 * Returns 1 if successful, 0 if the item does not contain such value or -1 on error
 */
#define libpff_contact_get_utf16_email_address_2( contact, utf16_string, utf16_string_size, error ) \
	libpff_message_get_entry_value_utf16_string( contact, LIBPFF_ENTRY_TYPE_CONTACT_EMAIL_ADDRESS_2, utf16_string, utf16_string_size, error )

/* Retrieves the size of the UTF-8 encoded contact tertiary email address
 * Size includes the end of string character
 * Returns 1 if successful, 0 if the item does not contain such value or -1 on error
 */
#define libpff_contact_get_utf8_email_address_3_size( contact, utf8_string_size, error ) \
	libpff_message_get_entry_value_utf8_string_size( contact, LIBPFF_ENTRY_TYPE_CONTACT_EMAIL_ADDRESS_3, utf8_string_size, error )

/* Retrieves the UTF-8 encoded contact tertiary email address
 * Returns 1 if successful, 0 if the item does not contain such value or -1 on error
 */
#define libpff_contact_get_utf8_email_address_3( contact, utf8_string, utf8_string_size, error ) \
	libpff_message_get_entry_value_utf8_string( contact, LIBPFF_ENTRY_TYPE_CONTACT_EMAIL_ADDRESS_3, utf8_string, utf8_string_size, error )

/* Retrieves the size of the UTF-16 encoded contact tertiary email address
 * Size includes the end of string character
 * Returns 1 if successful, 0 if the item does not contain such value or -1 on error
 */
#define libpff_contact_get_utf16_email_address_3_size( contact, utf16_string_size, error ) \
	libpff_message_get_entry_value_utf16_string_size( contact, LIBPFF_ENTRY_TYPE_CONTACT_EMAIL_ADDRESS_3, utf16_string_size, error )

/* Retrieves the UTF-16 encoded contact tertiary email address
 * Returns 1 if successful, 0 if the item does not contain such value or -1 on error
 */
#define libpff_contact_get_utf16_email_address_3( contact, utf16_string, utf16_string_size, error ) \
	libpff_message_get_entry_value_utf16_string( contact, LIBPFF_ENTRY_TYPE_CONTACT_EMAIL_ADDRESS_3, utf16_string, utf16_string_size, error )

/* -------------------------------------------------------------------------
 * E-mail functions
 * ------------------------------------------------------------------------- */

/* Retrieves the size of the UTF-8 encoded e-mail filename
 * Size includes the end of string character
 * Returns 1 if successful, 0 if the item does not contain such value or -1 on error
 */
#define libpff_email_get_utf8_filename_size( email, utf8_string_size, error ) \
	libpff_message_get_entry_value_utf8_string_size( email, LIBPFF_ENTRY_TYPE_EMAIL_EML_FILENAME, utf8_string_size, error )

/* Retrieves the utf-8 encoded e-mail filename
 * Returns 1 if successful, 0 if the item does not contain such value or -1 on error
 */
#define libpff_email_get_utf8_filename( email, utf8_string, utf8_string_size, error ) \
	libpff_message_get_entry_value_utf8_string( email, LIBPFF_ENTRY_TYPE_EMAIL_EML_FILENAME, utf8_string, utf8_string_size, error )

/* Retrieves the size of the UTF-16 encoded e-mail filename
 * Size includes the end of string character
 * Returns 1 if successful, 0 if the item does not contain such value or -1 on error
 */
#define libpff_email_get_utf16_filename_size( email, utf16_string_size, error ) \
	libpff_message_get_entry_value_utf16_string_size( email, LIBPFF_ENTRY_TYPE_EMAIL_EML_FILENAME, utf16_string_size, error )

/* Retrieves the utf-16 encoded e-mail filename
 * Returns 1 if successful, 0 if the item does not contain such value or -1 on error
 */
#define libpff_email_get_utf16_filename( email, utf16_string, utf16_string_size, error ) \
	libpff_message_get_entry_value_utf16_string( email, LIBPFF_ENTRY_TYPE_EMAIL_EML_FILENAME, utf16_string, utf16_string_size, error )

/* -------------------------------------------------------------------------
 * Attachment functions
 * ------------------------------------------------------------------------- */

/* Retrieves the attachment type
 * Returns 1 if successful or -1 on error
 */
LIBPFF_EXTERN \
int libpff_attachment_get_type(
     libpff_item_t *attachment,
     int *attachment_type,
     libpff_error_t **error );

/* Retrieves the size of the UTF-8 encoded attachment short filename
 * Size includes the end of string character
 * Returns 1 if successful, 0 if the item does not contain such value or -1 on error
 */
#define libpff_attachment_get_utf8_short_filename_size( attachment, utf8_string_size, error ) \
	libpff_item_get_entry_value_utf8_string_size( attachment, 0, LIBPFF_ENTRY_TYPE_ATTACHMENT_FILENAME_SHORT, utf8_string_size, 0, error )

/* Retrieves the UTF-8 encoded attachment short filename
 * Returns 1 if successful, 0 if the item does not contain such value or -1 on error
 */
#define libpff_attachment_get_utf8_short_filename( attachment, utf8_string, utf8_string_size, error ) \
	libpff_item_get_entry_value_utf8_string( attachment, 0, LIBPFF_ENTRY_TYPE_ATTACHMENT_FILENAME_SHORT, utf8_string, utf8_string_size, 0, error )

/* Retrieves the size of the UTF-16 encoded attachment short filename
 * Size includes the end of string character
 * Returns 1 if successful, 0 if the item does not contain such value or -1 on error
 */
#define libpff_attachment_get_utf16_short_filename_size( attachment, utf16_string_size, error ) \
	libpff_item_get_entry_value_utf16_string_size( attachment, 0, LIBPFF_ENTRY_TYPE_ATTACHMENT_FILENAME_SHORT, utf16_string_size, 0, error )

/* Retrieves the UTF-16 encoded attachment short filename
 * Returns 1 if successful, 0 if the item does not contain such value or -1 on error
 */
#define libpff_attachment_get_utf16_short_filename( attachment, utf16_string, utf16_string_size, error ) \
	libpff_item_get_entry_value_utf16_string( attachment, 0, LIBPFF_ENTRY_TYPE_ATTACHMENT_FILENAME_SHORT, utf16_string, utf16_string_size, 0, error )

/* Retrieves the size of the UTF-8 encoded attachment long filename
 * Size includes the end of string character
 * Returns 1 if successful, 0 if the item does not contain such value or -1 on error
 */
#define libpff_attachment_get_utf8_long_filename_size( attachment, utf8_string_size, error ) \
	libpff_item_get_entry_value_utf8_string_size( attachment, 0, LIBPFF_ENTRY_TYPE_ATTACHMENT_FILENAME_LONG, utf8_string_size, 0, error )

/* Retrieves the UTF-8 encoded attachment long filename
 * Returns 1 if successful, 0 if the item does not contain such value or -1 on error
 */
#define libpff_attachment_get_utf8_long_filename( attachment, utf8_string, utf8_string_size, error ) \
	libpff_item_get_entry_value_utf8_string( attachment, 0, LIBPFF_ENTRY_TYPE_ATTACHMENT_FILENAME_LONG, utf8_string, utf8_string_size, 0, error )

/* Retrieves the size of the UTF-16 encoded attachment long filename
 * Size includes the end of string character
 * Returns 1 if successful, 0 if the item does not contain such value or -1 on error
 */
#define libpff_attachment_get_utf16_long_filename_size( attachment, utf16_string_size, error ) \
	libpff_item_get_entry_value_utf16_string_size( attachment, 0, LIBPFF_ENTRY_TYPE_ATTACHMENT_FILENAME_LONG, utf16_string_size, 0, error )

/* Retrieves the UTF-16 encoded attachment long filename
 * Returns 1 if successful, 0 if the item does not contain such value or -1 on error
 */
#define libpff_attachment_get_utf16_long_filename( attachment, utf16_string, utf16_string_size, error ) \
	libpff_item_get_entry_value_utf16_string( attachment, 0, LIBPFF_ENTRY_TYPE_ATTACHMENT_FILENAME_LONG, utf16_string, utf16_string_size, 0, error )

/* Retrieves the attachment data size
 * Returns 1 if successful, 0 if the item does not contain such value or -1 on error
 */
LIBPFF_EXTERN \
int libpff_attachment_get_data_size(
     libpff_item_t *attachment,
     size64_t *size,
     libpff_error_t **error );

/* Reads attachment data from the curent offset into a buffer
 * Returns the amount of bytes read or -1 on error
 */
LIBPFF_EXTERN \
ssize_t libpff_attachment_data_read_buffer(
         libpff_item_t *attachment,
         uint8_t *buffer,
         size_t buffer_size,
         libpff_error_t **error );

/* Seeks a certain offset of the attachment data
 * Returns the offset if seek is successful or -1 on error
 */
LIBPFF_EXTERN \
off64_t libpff_attachment_data_seek_offset(
         libpff_item_t *attachment,
         off64_t offset,
         int whence,
         libpff_error_t **error );

#if defined( LIBPFF_HAVE_BFIO )
/* Retrieves the attachment data file io handle
 * Returns 1 if successful, 0 if the item does not contain such value or -1 on error
 */
LIBPFF_EXTERN \
int libpff_attachment_get_data_file_io_handle(
     libpff_item_t *attachment,
     libbfio_handle_t **file_io_handle,
     libpff_error_t **error );
#endif

/* Retrieves the attached item
 * Returns 1 if successful, 0 if the item does not contain such value or -1 on error
 */
LIBPFF_EXTERN \
int libpff_attachment_get_item(
     libpff_item_t *attachment,
     libpff_item_t **attached_item,
     libpff_error_t **error );

/* -------------------------------------------------------------------------
 * Recipients functions
 * ------------------------------------------------------------------------- */

/* Retrieves the size of the UTF-8 encoded display name
 * Size includes the end of string character
 * Returns 1 if successful, 0 if the recipient does not contain such value or -1 on error
 */
#define libpff_recipients_get_utf8_display_name_size( recipients, recipient_index, utf8_string_size, error ) \
        libpff_item_get_entry_value_utf8_string_size( recipients, recipient_index, LIBPFF_ENTRY_TYPE_DISPLAY_NAME, utf8_string_size, 0, error )

/* Retrieves the UTF-8 encoded display name
 * Returns 1 if successful, 0 if the recipient does not contain such value or -1 on error
 */
#define libpff_recipients_get_utf8_display_name( recipients, recipient_index, utf8_string, utf8_string_size, error ) \
        libpff_item_get_entry_value_utf8_string( recipients, recipient_index, LIBPFF_ENTRY_TYPE_DISPLAY_NAME, utf8_string, utf8_string_size, 0, error )

/* Retrieves the size of the UTF-16 encoded display name
 * Size includes the end of string character
 * Returns 1 if successful, 0 if the recipient does not contain such value or -1 on error
 */
#define libpff_recipients_get_utf16_display_name_size( recipients, recipient_index, utf16_string_size, error ) \
        libpff_item_get_entry_value_utf16_string_size( recipients, recipient_index, LIBPFF_ENTRY_TYPE_DISPLAY_NAME, utf16_string_size, 0, error )

/* Retrieves the UTF-16 encoded display name
 * Returns 1 if successful, 0 if the recipient does not contain such value or -1 on error
 */
#define libpff_recipients_get_utf16_display_name( recipients, recipient_index, utf16_string, utf16_string_size, error ) \
        libpff_item_get_entry_value_utf16_string( recipients, recipient_index, LIBPFF_ENTRY_TYPE_DISPLAY_NAME, utf16_string, utf16_string_size, 0, error )

/* Retrieves the size of the UTF-8 encoded address type
 * Size includes the end of string character
 * Returns 1 if successful, 0 if the recipient does not contain such value or -1 on error
 */
#define libpff_recipients_get_utf8_address_type_size( recipients, recipient_index, utf8_string_size, error ) \
        libpff_item_get_entry_value_utf8_string_size( recipients, recipient_index, LIBPFF_ENTRY_TYPE_ADDRESS_TYPE, utf8_string_size, 0, error )

/* Retrieves the UTF-8 encoded address type
 * Returns 1 if successful, 0 if the recipient does not contain such value or -1 on error
 */
#define libpff_recipients_get_utf8_address_type( recipients, recipient_index, utf8_string, utf8_string_size, error ) \
        libpff_item_get_entry_value_utf8_string( recipients, recipient_index, LIBPFF_ENTRY_TYPE_ADDRESS_TYPE, utf8_string, utf8_string_size, 0, error )

/* Retrieves the size of the UTF-16 encoded address type
 * Size includes the end of string character
 * Returns 1 if successful, 0 if the recipient does not contain such value or -1 on error
 */
#define libpff_recipients_get_utf16_address_type_size( recipients, recipient_index, utf16_string_size, error ) \
        libpff_item_get_entry_value_utf16_string_size( recipients, recipient_index, LIBPFF_ENTRY_TYPE_ADDRESS_TYPE, utf16_string_size, 0, error )

/* Retrieves the UTF-16 encoded address type
 * Returns 1 if successful, 0 if the recipient does not contain such value or -1 on error
 */
#define libpff_recipients_get_utf16_address_type( recipients, recipient_index, utf16_string, utf16_string_size, error ) \
        libpff_item_get_entry_value_utf16_string( recipients, recipient_index, LIBPFF_ENTRY_TYPE_ADDRESS_TYPE, utf16_string, utf16_string_size, 0, error )

/* Retrieves the size of the UTF-8 encoded email address
 * Size includes the end of string character
 * Returns 1 if successful, 0 if the recipient does not contain such value or -1 on error
 */
#define libpff_recipients_get_utf8_email_address_size( recipients, recipient_index, utf8_string_size, error ) \
        libpff_item_get_entry_value_utf8_string_size( recipients, recipient_index, LIBPFF_ENTRY_TYPE_EMAIL_ADDRESS, utf8_string_size, 0, error )

/* Retrieves the UTF-8 encoded email address
 * Returns 1 if successful, 0 if the recipient does not contain such value or -1 on error
 */
#define libpff_recipients_get_utf8_email_address( recipients, recipient_index, utf8_string, utf8_string_size, error ) \
        libpff_item_get_entry_value_utf8_string( recipients, recipient_index, LIBPFF_ENTRY_TYPE_EMAIL_ADDRESS, utf8_string, utf8_string_size, 0, error )

/* Retrieves the size of the UTF-16 encoded email address
 * Size includes the end of string character
 * Returns 1 if successful, 0 if the recipient does not contain such value or -1 on error
 */
#define libpff_recipients_get_utf16_email_address_size( recipients, recipient_index, utf16_string_size, error ) \
        libpff_item_get_entry_value_utf16_string_size( recipients, recipient_index, LIBPFF_ENTRY_TYPE_EMAIL_ADDRESS, utf16_string_size, 0, error )

/* Retrieves the UTF-16 encoded email address
 * Returns 1 if successful, 0 if the recipient does not contain such value or -1 on error
 */
#define libpff_recipients_get_utf16_email_address( recipients, recipient_index, utf16_string, utf16_string_size, error ) \
        libpff_item_get_entry_value_utf16_string( recipients, recipient_index, LIBPFF_ENTRY_TYPE_EMAIL_ADDRESS, utf16_string, utf16_string_size, 0, error )

/* Retrieves the size of the UTF-8 encoded recipient display name
 * Size includes the end of string character
 * Returns 1 if successful, 0 if the recipient does not contain such value or -1 on error
 */
#define libpff_recipients_get_utf8_recipient_display_name_size( recipients, recipient_index, utf8_string_size, error ) \
        libpff_item_get_entry_value_utf8_string_size( recipients, recipient_index, LIBPFF_ENTRY_TYPE_RECIPIENT_DISPLAY_NAME, utf8_string_size, 0, error )

/* Retrieves the UTF-8 encoded recipient display name
 * Returns 1 if successful, 0 if the recipient does not contain such value or -1 on error
 */
#define libpff_recipients_get_utf8_recipient_display_name( recipients, recipient_index, utf8_string, utf8_string_size, error ) \
        libpff_item_get_entry_value_utf8_string( recipients, recipient_index, LIBPFF_ENTRY_TYPE_RECIPIENT_DISPLAY_NAME, utf8_string, utf8_string_size, 0, error )

/* Retrieves the size of the UTF-16 encoded recipient display name
 * Size includes the end of string character
 * Returns 1 if successful, 0 if the recipient does not contain such value or -1 on error
 */
#define libpff_recipients_get_utf16_recipient_display_name_size( recipients, recipient_index, utf16_string_size, error ) \
        libpff_item_get_entry_value_utf16_string_size( recipients, recipient_index, LIBPFF_ENTRY_TYPE_RECIPIENT_DISPLAY_NAME, utf16_string_size, 0, error )

/* Retrieves the UTF-16 encoded recipient display name
 * Returns 1 if successful, 0 if the recipient does not contain such value or -1 on error
 */
#define libpff_recipients_get_utf16_recipient_display_name( recipients, recipient_index, utf16_string, utf16_string_size, error ) \
        libpff_item_get_entry_value_utf16_string( recipients, recipient_index, LIBPFF_ENTRY_TYPE_RECIPIENT_DISPLAY_NAME, utf16_string, utf16_string_size, 0, error )

/* Retrieves the recipient type
 * Returns 1 if successful, 0 if the item does not contain such value or -1 on error
 */
#define libpff_recipients_get_type( recipients, recipient_index, type, error ) \
	libpff_item_get_entry_value_32bit( recipients, recipient_index, LIBPFF_ENTRY_TYPE_RECIPIENT_TYPE, type, 0, error )

#if defined( __cplusplus )
}
#endif

#endif

