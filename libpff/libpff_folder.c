/*
 * Folder functions
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
#include <memory.h>
#include <types.h>

#include "libpff_debug.h"
#include "libpff_definitions.h"
#include "libpff_folder.h"
#include "libpff_index_value.h"
#include "libpff_item.h"
#include "libpff_item_descriptor.h"
#include "libpff_item_tree.h"
#include "libpff_item_values.h"
#include "libpff_libcerror.h"
#include "libpff_libcnotify.h"
#include "libpff_libcstring.h"
#include "libpff_libfcache.h"
#include "libpff_libfdata.h"
#include "libpff_libfmapi.h"
#include "libpff_list_type.h"
#include "libpff_local_descriptor_value.h"
#include "libpff_local_descriptors_tree.h"
#include "libpff_mapi.h"
#include "libpff_tree_type.h"
#include "libpff_value_type.h"

#define LIBPFF_FOLDER_SUB_ITEM_SUB_FOLDERS		0
#define LIBPFF_FOLDER_SUB_ITEM_SUB_MESSAGES		1
#define LIBPFF_FOLDER_SUB_ITEM_SUB_ASSOCIATED_CONTENTS	2
#define LIBPFF_FOLDER_SUB_ITEM_UNKNOWNS			3

/* Retrieves the folder (container) type
 * Returns 1 if successful or -1 on error
 */
int libpff_folder_get_type(
     libpff_item_t *folder,
     uint8_t *type,
     libcerror_error_t **error )
{
	libpff_internal_item_t *internal_item = NULL;
	char *container_class_string          = NULL;
	static char *function                 = "libpff_folder_get_type";
	size_t container_class_string_size    = 0;
	uint8_t folder_type                   = LIBPFF_ITEM_TYPE_UNDEFINED;
	int result                            = 0;

	if( folder == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid folder.",
		 function );

		return( -1 );
	}
	internal_item = (libpff_internal_item_t *) folder;

	if( internal_item->internal_file == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid folder - missing internal file.",
		 function );

		return( -1 );
	}
	if( type == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid type.",
		 function );

		return( -1 );
	}
	result = libpff_item_get_entry_value_utf8_string_size(
		  folder,
		  0,
		  LIBPFF_ENTRY_TYPE_CONTAINER_CLASS,
		  &container_class_string_size,
		  0,
		  error );

	if( result == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve container class string size.",
		 function );

		return( -1 );
	}
	else if( result == 1 )
	{
		if( container_class_string_size > 0 )
		{
			container_class_string = libcstring_narrow_string_allocate(
			                          container_class_string_size );

			if( container_class_string == NULL )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_MEMORY,
				 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
				 "%s: unable to create container class string.",
				 function );

				return( -1 );
			}
			result = libpff_item_get_entry_value_utf8_string(
				  folder,
				  0,
				  LIBPFF_ENTRY_TYPE_CONTAINER_CLASS,
				  (uint8_t *) container_class_string,
				  container_class_string_size,
				  0,
				  error );

			if( result != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
				 "%s: unable to retrieve container class string.",
				 function );

				return( -1 );
			}
			if( container_class_string_size == 9 )
			{
				if( libcstring_narrow_string_compare(
				     container_class_string,
				     "IPF.Note",
				     8 ) == 0 )
				{
					folder_type = LIBPFF_ITEM_TYPE_EMAIL;
				}
				else if( libcstring_narrow_string_compare(
					  container_class_string,
					  "IPF.Task",
					  8 ) == 0 )
				{
					folder_type = LIBPFF_ITEM_TYPE_TASK;
				}
			}
			else if( container_class_string_size == 12 )
			{
				if( libcstring_narrow_string_compare(
				     container_class_string,
				     "IPF.Contact",
				     11 ) == 0 )
				{
					folder_type = LIBPFF_ITEM_TYPE_CONTACT;
				}
				else if( libcstring_narrow_string_compare(
					  container_class_string,
					  "IPF.Journal",
					  11 ) == 0 )
				{
					folder_type = LIBPFF_ITEM_TYPE_ACTIVITY;
				}
			}
			else if( container_class_string_size == 16 )
			{
				if( libcstring_narrow_string_compare(
				     container_class_string,
				     "IPF.Appointment",
				     15 ) == 0 )
				{
					folder_type = LIBPFF_ITEM_TYPE_APPOINTMENT;
				}
				else if( libcstring_narrow_string_compare(
					  container_class_string,
					  "IPF.StickyNote",
					  15 ) == 0 )
				{
					folder_type = LIBPFF_ITEM_TYPE_NOTE;
				}
			}
#if defined( HAVE_DEBUG_OUTPUT )
			if( ( libcnotify_verbose != 0 )
			 && ( folder_type == LIBPFF_ITEM_TYPE_UNDEFINED ) )
			{
				libcnotify_printf(
				 "%s: unsupported folder (container) type: %s\n",
				 function,
				 container_class_string );
			}
#endif
			memory_free(
			 container_class_string );
		}
	}
	*type = (int) folder_type;

	return( 1 );
}

/* Determine if the item has sub folders
 * Returns 1 if successful or -1 on error
 */
int libpff_folder_determine_sub_folders(
     libpff_internal_item_t *internal_item,
     libcerror_error_t **error )
{
	libpff_index_value_t *descriptor_index_value = NULL;
	static char *function                        = "libpff_folder_determine_sub_folders";
	uint32_t sub_folders_descriptor_identifier   = 0;
	int result                                   = 0;

	if( internal_item == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid internal item.",
		 function );

		return( -1 );
	}
	if( internal_item->internal_file == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid internal item - missing internal file.",
		 function );

		return( -1 );
	}
	if( internal_item->item_descriptor == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid internal item - missing item descriptor.",
		 function );

		return( -1 );
	}
	/* Determine if the item has sub folders
	 */
	if( internal_item->sub_item_values[ LIBPFF_FOLDER_SUB_ITEM_SUB_FOLDERS ] == NULL )
	{
		sub_folders_descriptor_identifier = internal_item->item_descriptor->descriptor_identifier + 11;

		result = libpff_io_handle_get_descriptor_index_value_by_identifier(
			  internal_item->internal_file->io_handle,
			  internal_item->file_io_handle,
			  sub_folders_descriptor_identifier,
			  internal_item->item_descriptor->recovered,
			  &descriptor_index_value,
			  error );

		if( result == -1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve descriptor index value: %" PRIu32 ".",
			 function,
			 sub_folders_descriptor_identifier );

			return( -1 );
		}
		else if( result != 0 )
		{
			if( descriptor_index_value == NULL )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
				 "%s: invalid descriptor index value.",
				 function );

				return( -1 );
			}
#if defined( HAVE_DEBUG_OUTPUT )
			if( libcnotify_verbose != 0 )
			{
				libcnotify_printf(
				 "%s: descriptor identifier: %" PRIu64 " ",
				 function,
				 descriptor_index_value->identifier );
				libpff_debug_print_node_identifier_type(
				 (uint8_t) ( descriptor_index_value->identifier & 0x0000001fUL ) );
				libcnotify_printf(
				 ", data: %" PRIu64 ", local descriptors: %" PRIu64 ", parent: %" PRIu32 "\n",
				 descriptor_index_value->data_identifier,
				 descriptor_index_value->local_descriptors_identifier,
				 descriptor_index_value->parent_identifier );
			}
#endif
			/* Cache the sub folders items values for successive usage
			 */
			if( libpff_item_values_initialize(
			     &( internal_item->sub_item_values[ LIBPFF_FOLDER_SUB_ITEM_SUB_FOLDERS ] ),
			     sub_folders_descriptor_identifier,
			     descriptor_index_value->data_identifier,
			     descriptor_index_value->local_descriptors_identifier,
			     internal_item->item_descriptor->recovered,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
				 "%s: unable to create sub folders item values.",
				 function );

				return( -1 );
			}
			if( libpff_item_values_read(
			     internal_item->sub_item_values[ LIBPFF_FOLDER_SUB_ITEM_SUB_FOLDERS ],
			     internal_item->internal_file->name_to_id_map_list,
			     internal_item->internal_file->io_handle,
			     internal_item->file_io_handle,
			     LIBPFF_DEBUG_ITEM_TYPE_DEFAULT,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_IO,
				 LIBCERROR_IO_ERROR_READ_FAILED,
				 "%s: unable to read descriptor identifier: %" PRIu32 ".",
				 function,
				 sub_folders_descriptor_identifier );

				return( -1 );
			}
		}
	}
	return( 1 );
}

/* Determine if the item has sub messages
 * Returns 1 if successful or -1 on error
 */
int libpff_folder_determine_sub_messages(
     libpff_internal_item_t *internal_item,
     libcerror_error_t **error )
{
	libpff_index_value_t *descriptor_index_value = NULL;
	static char *function                        = "libpff_folder_determine_sub_messages";
	uint32_t sub_messages_descriptor_identifier  = 0;
	int result                                   = 0;

	if( internal_item == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid internal item.",
		 function );

		return( -1 );
	}
	if( internal_item->internal_file == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid internal item - missing internal file.",
		 function );

		return( -1 );
	}
	if( internal_item->item_descriptor == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid internal item - missing item descriptor.",
		 function );

		return( -1 );
	}
	/* Determine if the item has sub messages
	 */
	if( internal_item->sub_item_values[ LIBPFF_FOLDER_SUB_ITEM_SUB_MESSAGES ] == NULL )
	{
		sub_messages_descriptor_identifier = internal_item->item_descriptor->descriptor_identifier + 12;

		result = libpff_io_handle_get_descriptor_index_value_by_identifier(
			  internal_item->internal_file->io_handle,
			  internal_item->file_io_handle,
			  sub_messages_descriptor_identifier,
			  internal_item->item_descriptor->recovered,
			  &descriptor_index_value,
			  error );

		if( result == -1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve descriptor index value: %" PRIu32 ".",
			 function,
			 sub_messages_descriptor_identifier );

			return( -1 );
		}
		else if( result != 0 )
		{
			if( descriptor_index_value == NULL )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
				 "%s: invalid descriptor index value.",
				 function );

				return( -1 );
			}
#if defined( HAVE_DEBUG_OUTPUT )
			if( libcnotify_verbose != 0 )
			{
				libcnotify_printf(
				 "%s: descriptor identifier: %" PRIu64 " ",
				 function,
				 descriptor_index_value->identifier );
				libpff_debug_print_node_identifier_type(
				 (uint8_t) ( descriptor_index_value->identifier & 0x0000001fUL ) );
				libcnotify_printf(
				 ", data: %" PRIu64 ", local descriptors: %" PRIu64 ", parent: %" PRIu32 "\n",
				 descriptor_index_value->data_identifier,
				 descriptor_index_value->local_descriptors_identifier,
				 descriptor_index_value->parent_identifier );
			}
#endif
			/* Cache the sub messages items values for successive usage
			 */
			if( libpff_item_values_initialize(
			     &( internal_item->sub_item_values[ LIBPFF_FOLDER_SUB_ITEM_SUB_MESSAGES ] ),
			     sub_messages_descriptor_identifier,
			     descriptor_index_value->data_identifier,
			     descriptor_index_value->local_descriptors_identifier,
			     internal_item->item_descriptor->recovered,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
				 "%s: unable to create sub messages item values.",
				 function );

				return( -1 );
			}
			if( libpff_item_values_read(
			     internal_item->sub_item_values[ LIBPFF_FOLDER_SUB_ITEM_SUB_MESSAGES ],
			     internal_item->internal_file->name_to_id_map_list,
			     internal_item->internal_file->io_handle,
			     internal_item->file_io_handle,
			     LIBPFF_DEBUG_ITEM_TYPE_DEFAULT,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_IO,
				 LIBCERROR_IO_ERROR_READ_FAILED,
				 "%s: unable to read descriptor identifier: %" PRIu32 ".",
				 function,
				 sub_messages_descriptor_identifier );

				return( -1 );
			}
		}
	}
	return( 1 );
}

/* Determine if the item has sub associated contents
 * Returns 1 if successful or -1 on error
 */
int libpff_folder_determine_sub_associated_contents(
     libpff_internal_item_t *internal_item,
     libcerror_error_t **error )
{
	libpff_index_value_t *descriptor_index_value           = NULL;
	static char *function                                  = "libpff_folder_determine_sub_associated_contents";
	uint32_t sub_associated_contents_descriptor_identifier = 0;
	int result                                             = 0;

	if( internal_item == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid internal item.",
		 function );

		return( -1 );
	}
	if( internal_item->internal_file == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid internal item - missing internal file.",
		 function );

		return( -1 );
	}
	if( internal_item->item_descriptor == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid internal item - missing item descriptor.",
		 function );

		return( -1 );
	}
	/* Determine if the item has sub associated contents
	 */
	if( internal_item->sub_item_values[ LIBPFF_FOLDER_SUB_ITEM_SUB_ASSOCIATED_CONTENTS ] == NULL )
	{
		sub_associated_contents_descriptor_identifier = internal_item->item_descriptor->descriptor_identifier + 13;

		result = libpff_io_handle_get_descriptor_index_value_by_identifier(
			  internal_item->internal_file->io_handle,
			  internal_item->file_io_handle,
			  sub_associated_contents_descriptor_identifier,
			  internal_item->item_descriptor->recovered,
			  &descriptor_index_value,
			  error );

		if( result == -1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve descriptor index value: %" PRIu32 ".",
			 function,
			 sub_associated_contents_descriptor_identifier );

			return( -1 );
		}
		else if( result != 0 )
		{
			if( descriptor_index_value == NULL )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
				 "%s: invalid descriptor index value.",
				 function );

				return( -1 );
			}
#if defined( HAVE_DEBUG_OUTPUT )
			if( libcnotify_verbose != 0 )
			{
				libcnotify_printf(
				 "%s: descriptor identifier: %" PRIu64 " ",
				 function,
				 descriptor_index_value->identifier );
				libpff_debug_print_node_identifier_type(
				 (uint8_t) ( descriptor_index_value->identifier & 0x0000001fUL ) );
				libcnotify_printf(
				 ", data: %" PRIu64 ", local descriptors: %" PRIu64 ", parent: %" PRIu32 "\n",
				 descriptor_index_value->data_identifier,
				 descriptor_index_value->local_descriptors_identifier,
				 descriptor_index_value->parent_identifier );
			}
#endif
			/* Cache the sub associated contents items values for successive usage
			 */
			if( libpff_item_values_initialize(
			     &( internal_item->sub_item_values[ LIBPFF_FOLDER_SUB_ITEM_SUB_ASSOCIATED_CONTENTS ] ),
			     sub_associated_contents_descriptor_identifier,
			     descriptor_index_value->data_identifier,
			     descriptor_index_value->local_descriptors_identifier,
			     internal_item->item_descriptor->recovered,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
				 "%s: unable to create sub associated contents item values.",
				 function );

				return( -1 );
			}
			if( libpff_item_values_read(
			     internal_item->sub_item_values[ LIBPFF_FOLDER_SUB_ITEM_SUB_ASSOCIATED_CONTENTS ],
			     internal_item->internal_file->name_to_id_map_list,
			     internal_item->internal_file->io_handle,
			     internal_item->file_io_handle,
			     LIBPFF_DEBUG_ITEM_TYPE_DEFAULT,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_IO,
				 LIBCERROR_IO_ERROR_READ_FAILED,
				 "%s: unable to read descriptor identifier: %" PRIu32 ".",
				 function,
				 sub_associated_contents_descriptor_identifier );

				return( -1 );
			}
		}
	}
	return( 1 );
}

/* Determine if the item has unknowns
 * Returns 1 if successful or -1 on error
 */
int libpff_folder_determine_unknowns(
     libpff_internal_item_t *internal_item,
     libcerror_error_t **error )
{
	libpff_item_descriptor_t *unknowns_item_descriptor      = NULL;
	libpff_local_descriptor_value_t *local_descriptor_value = NULL;
	static char *function                                   = "libpff_folder_determine_unknowns";
	int result                                              = 0;

	if( internal_item == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid internal item.",
		 function );

		return( -1 );
	}
	if( internal_item->internal_file == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid internal item - missing internal file.",
		 function );

		return( -1 );
	}
	if( internal_item->item_descriptor == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid internal item - missing item descriptor.",
		 function );

		return( -1 );
	}
	if( internal_item->item_values == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid internal item - missing item values.",
		 function );

		return( -1 );
	}
	if( internal_item->sub_item_tree_node[ LIBPFF_FOLDER_SUB_ITEM_UNKNOWNS ] != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: unknowns sub item tree node already set.",
		 function );

		return( -1 );
	}
	/* Make sure the item values have been read
	 */
/* TODO is this still necessary ? */
	if( internal_item->item_values->table == NULL )
	{
#if defined( HAVE_DEBUG_OUTPUT )
		if( libcnotify_verbose != 0 )
		{
			libcnotify_printf(
			 "%s: reading item values of descriptor: %" PRIu32 "\n",
			 function,
			 internal_item->item_descriptor->descriptor_identifier );
		}
#endif

		if( libpff_item_values_read(
		     internal_item->item_values,
		     internal_item->internal_file->name_to_id_map_list,
		     internal_item->internal_file->io_handle,
		     internal_item->file_io_handle,
		     LIBPFF_DEBUG_ITEM_TYPE_DEFAULT,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_IO,
			 LIBCERROR_IO_ERROR_READ_FAILED,
			 "%s: unable to read item values.",
			 function );

			return( -1 );
		}
		if( internal_item->item_values->table == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
			 "%s: invalid item values - missing table.",
			 function );

			return( -1 );
		}
	}
	/* Determine if the item has unknowns
	 */
	result = libpff_item_values_get_local_descriptors_value_by_identifier(
		  internal_item->item_values,
		  internal_item->file_io_handle,
		  (uint32_t) LIBPFF_LOCAL_DESCRIPTOR_IDENTIFIER_UNKNOWN_1718,
		  &local_descriptor_value,
		  error );

	if( result == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve local descriptor identifier: %" PRIu32 ".",
		 function,
		 (uint32_t) LIBPFF_LOCAL_DESCRIPTOR_IDENTIFIER_UNKNOWN_1718 );

		return( -1 );
	}
	else if( result != 0 )
	{
		if( local_descriptor_value == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
			 "%s: invalid local descriptor value.",
			 function );

			return( -1 );
		}
#if defined( HAVE_DEBUG_OUTPUT )
		if( libcnotify_verbose != 0 )
		{
			libcnotify_printf(
			 "%s: local descriptor identifier: %" PRIu64 " ",
			 function,
			 local_descriptor_value->identifier );
			libpff_debug_print_node_identifier_type(
			 (uint8_t) ( local_descriptor_value->identifier & 0x0000001fUL ) );
			libcnotify_printf(
			 ", data: %" PRIu64 ", local descriptors: %" PRIu64 "\n",
			 local_descriptor_value->data_identifier,
			 local_descriptor_value->local_descriptors_identifier );
		}
#endif
		if( libpff_item_values_initialize(
		     &( internal_item->sub_item_values[ LIBPFF_FOLDER_SUB_ITEM_UNKNOWNS ] ),
		     LIBPFF_LOCAL_DESCRIPTOR_IDENTIFIER_UNKNOWN_1718,
		     local_descriptor_value->data_identifier,
		     local_descriptor_value->local_descriptors_identifier,
		     internal_item->item_descriptor->recovered,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
			 "%s: unable to create unknowns item values.",
			 function );

			return( -1 );
		}
		/* Cache the unknowns items values for successive usage
		 */
		if( libpff_item_values_read(
		     internal_item->sub_item_values[ LIBPFF_FOLDER_SUB_ITEM_UNKNOWNS ],
		     internal_item->internal_file->name_to_id_map_list,
		     internal_item->internal_file->io_handle,
		     internal_item->file_io_handle,
		     LIBPFF_DEBUG_ITEM_TYPE_DEFAULT,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_IO,
			 LIBCERROR_IO_ERROR_READ_FAILED,
			 "%s: unable to read local descriptor identifier: %" PRIu32 ".",
			 function,
			 LIBPFF_LOCAL_DESCRIPTOR_IDENTIFIER_UNKNOWN_1718 );

			return( -1 );
		}
		if( internal_item->sub_item_values[ LIBPFF_FOLDER_SUB_ITEM_UNKNOWNS ]->table == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
			 "%s: invalid unknowns item values - missing table.",
			 function );

			return( -1 );
		}
		/* Create an unknowns item descriptor
		 */
		if( libpff_item_descriptor_initialize(
		     &unknowns_item_descriptor,
		     LIBPFF_LOCAL_DESCRIPTOR_IDENTIFIER_UNKNOWN_1718,
		     local_descriptor_value->data_identifier,
		     local_descriptor_value->local_descriptors_identifier,
		     internal_item->item_descriptor->recovered,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
			 "%s: unable to create unknowns item descriptor.",
			 function );

			return( -1 );
		}
		if( unknowns_item_descriptor == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
			 "%s: missing unknowns item descriptor.",
			 function );

			return( -1 );
		}
		if( libpff_tree_node_initialize(
		     &( internal_item->sub_item_tree_node[ LIBPFF_FOLDER_SUB_ITEM_UNKNOWNS ] ),
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
			 "%s: unable to create unknowns sub item tree node.",
			 function );

			memory_free(
			 unknowns_item_descriptor );

			return( -1 );
		}
		if( libpff_tree_node_set_value(
		     internal_item->sub_item_tree_node[ LIBPFF_FOLDER_SUB_ITEM_UNKNOWNS ],
		     (intptr_t *) unknowns_item_descriptor,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
			 "%s: unable to set unknowns item descriptor in unknowns sub item tree node.",
			 function );

			libpff_tree_node_free(
			 &( internal_item->sub_item_tree_node[ LIBPFF_FOLDER_SUB_ITEM_UNKNOWNS ] ),
			 NULL,
			 NULL );
			memory_free(
			 unknowns_item_descriptor );

			return( -1 );
		}
		/* All the unknown data is in the unknowns item
		 * there are no sub items like for the attachments item
		 */
	}
	return( 1 );
}

/* Retrieves the number of sub folders from a folder
 * Returns 1 if successful or -1 on error
 */
int libpff_folder_get_number_of_sub_folders(
     libpff_item_t *folder,
     int *number_of_sub_folders,
     libcerror_error_t **error )
{
	libpff_internal_item_t *internal_item = NULL;
	static char *function                 = "libpff_folder_get_number_of_sub_folders";
	uint32_t number_of_sets               = 0;

	if( folder == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid folder.",
		 function );

		return( -1 );
	}
	internal_item = (libpff_internal_item_t *) folder;

	if( internal_item->internal_file == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid folder - missing internal file.",
		 function );

		return( -1 );
	}
	if( internal_item->type == LIBPFF_ITEM_TYPE_UNDEFINED )
	{
		if( libpff_item_determine_type(
		     internal_item,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to determine item type.",
			 function );

			return( -1 );
		}
	}
	if( internal_item->type != LIBPFF_ITEM_TYPE_FOLDER )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_UNSUPPORTED_VALUE,
		 "%s: unsupported item type: 0x%08" PRIx32 "",
		 function,
		 internal_item->type );

		return( -1 );
	}
	if( number_of_sub_folders == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid number of sub folders.",
		 function );

		return( -1 );
	}
	if( internal_item->sub_item_values[ LIBPFF_FOLDER_SUB_ITEM_SUB_FOLDERS ] == NULL )
	{
		if( libpff_folder_determine_sub_folders(
		     internal_item,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to determine sub folders.",
			 function );

			return( -1 );
		}
	}
	*number_of_sub_folders = 0;

	if( internal_item->sub_item_values[ LIBPFF_FOLDER_SUB_ITEM_SUB_FOLDERS ] != NULL )
	{
		if( libpff_item_values_get_number_of_sets(
		     internal_item->sub_item_values[ LIBPFF_FOLDER_SUB_ITEM_SUB_FOLDERS ],
		     internal_item->internal_file->name_to_id_map_list,
		     internal_item->internal_file->io_handle,
		     internal_item->file_io_handle,
		     &number_of_sets,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to determine the number of sub folders.",
			 function );

			return( -1 );
		}
		*number_of_sub_folders = (int) number_of_sets;
	}
	return( 1 );
}

/* Retrieves the sub folder for the specific index from a folder
 * Returns 1 if successful or -1 on error
 */
int libpff_folder_get_sub_folder(
     libpff_item_t *folder,
     int sub_folder_index,
     libpff_item_t **sub_folder,
     libcerror_error_t **error )
{
	libfcache_cache_t *value_data_cache        = NULL;
	libfdata_reference_t *value_data_reference = NULL;
	libpff_internal_item_t *internal_item      = NULL;
	libpff_tree_node_t *sub_folder_tree_node   = NULL;
	uint8_t *value_data                        = NULL;
	static char *function                      = "libpff_folder_get_sub_folder";
	size_t value_data_size                     = 0;
	uint32_t sub_folder_descriptor_identifier  = 0;
	uint32_t value_type                        = LIBPFF_VALUE_TYPE_INTEGER_32BIT_SIGNED;

	if( folder == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid folder.",
		 function );

		return( -1 );
	}
	internal_item = (libpff_internal_item_t *) folder;

	if( internal_item->internal_file == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid folder - missing internal file.",
		 function );

		return( -1 );
	}
	if( internal_item->type == LIBPFF_ITEM_TYPE_UNDEFINED )
	{
		if( libpff_item_determine_type(
		     internal_item,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to determine item type.",
			 function );

			return( -1 );
		}
	}
	if( internal_item->type != LIBPFF_ITEM_TYPE_FOLDER )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_UNSUPPORTED_VALUE,
		 "%s: unsupported item type: 0x%08" PRIx32 "",
		 function,
		 internal_item->type );

		return( -1 );
	}
	if( sub_folder == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid sub folder.",
		 function );

		return( -1 );
	}
	if( *sub_folder != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: sub folder already set.",
		 function );

		return( -1 );
	}
	if( internal_item->sub_item_values[ LIBPFF_FOLDER_SUB_ITEM_SUB_FOLDERS ] == NULL )
	{
		if( libpff_folder_determine_sub_folders(
		     internal_item,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to determine sub folders.",
			 function );

			return( -1 );
		}
	}
	if( internal_item->sub_item_values[ LIBPFF_FOLDER_SUB_ITEM_SUB_FOLDERS ] != NULL )
	{
		if( libpff_item_values_get_entry_value(
		     internal_item->sub_item_values[ LIBPFF_FOLDER_SUB_ITEM_SUB_FOLDERS ],
		     internal_item->internal_file->name_to_id_map_list,
		     internal_item->internal_file->io_handle,
		     internal_item->file_io_handle,
		     (int) sub_folder_index,
		     LIBPFF_ENTRY_TYPE_SUB_ITEM_IDENTIFIER,
		     &value_type,
		     &value_data_reference,
		     &value_data_cache,
		     0,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve entry value.",
			 function );

			return( -1 );
		}
		if( libfdata_reference_get_data(
		     value_data_reference,
		     internal_item->file_io_handle,
		     value_data_cache,
		     &value_data,
		     &value_data_size,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve value data.",
			 function );

			return( -1 );
		}
		if( libpff_value_type_copy_to_32bit(
		     value_data,
		     value_data_size,
		     &sub_folder_descriptor_identifier,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
			 "%s: unable to set 32-bit entry value.",
			 function );

			return( -1 );
		}
		if( libpff_item_tree_get_sub_node_by_identifier(
		     internal_item->item_tree_node,
		     sub_folder_descriptor_identifier,
		     &sub_folder_tree_node,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve sub folder tree node.",
			 function );

			return( -1 );
		}
		if( sub_folder_tree_node == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
			 "%s: invalid sub folder tree node.",
			 function );

			return( -1 );
		}
		if( libpff_item_initialize(
		     sub_folder,
		     internal_item->file_io_handle,
		     internal_item->internal_file,
		     sub_folder_tree_node,
		     (libpff_item_descriptor_t *) sub_folder_tree_node->value,
		     LIBPFF_ITEM_FLAGS_DEFAULT,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
			 "%s: unable to create sub folder.",
			 function );

			return( -1 );
		}
		if( *sub_folder == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
			 "%s: invalid sub folder.",
			 function );

			return( -1 );
		}
		( (libpff_internal_item_t *) *sub_folder )->type = LIBPFF_ITEM_TYPE_FOLDER;
	}
	return( 1 );
}

/* Retrieves the sub folder from a folder for the specific UTF-8 encoded name
 * This function uses the PidTagDisplayName MAPI property as the name
 * Returns 1 if successful, 0 if no such sub folder or -1 on error
 */
int libpff_folder_get_sub_folder_by_utf8_name(
     libpff_item_t *folder,
     uint8_t *utf8_sub_folder_name,
     size_t utf8_sub_folder_name_size,
     libpff_item_t **sub_folder,
     libcerror_error_t **error )
{
	libfcache_cache_t *value_data_cache        = NULL;
	libfdata_reference_t *value_data_reference = NULL;
	libpff_internal_item_t *internal_item      = NULL;
	libpff_tree_node_t *sub_folder_tree_node   = NULL;
	uint8_t *value_data                        = NULL;
	static char *function                      = "libpff_folder_get_sub_folder_by_utf8_name";
	size_t value_data_size                     = 0;
	uint32_t number_of_sub_folders             = 0;
	uint32_t sub_folder_descriptor_identifier  = 0;
	uint32_t sub_folder_index                  = 0;
	uint32_t value_type                        = 0;
	uint8_t is_ascii_string                    = 0;
	int result                                 = 0;

	if( folder == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid folder.",
		 function );

		return( -1 );
	}
	internal_item = (libpff_internal_item_t *) folder;

	if( internal_item->internal_file == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid folder - missing internal file.",
		 function );

		return( -1 );
	}
	if( internal_item->internal_file->io_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid internal item - invalid internal file - missing IO handle.",
		 function );

		return( -1 );
	}
	if( internal_item->type == LIBPFF_ITEM_TYPE_UNDEFINED )
	{
		if( libpff_item_determine_type(
		     internal_item,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to determine item type.",
			 function );

			return( -1 );
		}
	}
	if( internal_item->type != LIBPFF_ITEM_TYPE_FOLDER )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_UNSUPPORTED_VALUE,
		 "%s: unsupported item type: 0x%08" PRIx32 "",
		 function,
		 internal_item->type );

		return( -1 );
	}
	if( utf8_sub_folder_name == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid UTF-8 sub folder name.",
		 function );

		return( -1 );
	}
	if( utf8_sub_folder_name_size > (size_t) SSIZE_MAX )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_EXCEEDS_MAXIMUM,
		 "%s: invalid UTF-8 sub folder name size value exceeds maximum.",
		 function );

		return( -1 );
	}
	if( sub_folder == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid sub folder.",
		 function );

		return( -1 );
	}
	if( *sub_folder != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: sub folder already set.",
		 function );

		return( -1 );
	}
	if( internal_item->sub_item_values[ LIBPFF_FOLDER_SUB_ITEM_SUB_FOLDERS ] == NULL )
	{
		if( libpff_folder_determine_sub_folders(
		     internal_item,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to determine sub folders.",
			 function );

			return( -1 );
		}
	}
	if( internal_item->sub_item_values[ LIBPFF_FOLDER_SUB_ITEM_SUB_FOLDERS ] != NULL )
	{
		if( libpff_item_values_get_number_of_sets(
		     internal_item->sub_item_values[ LIBPFF_FOLDER_SUB_ITEM_SUB_FOLDERS ],
		     internal_item->internal_file->name_to_id_map_list,
		     internal_item->internal_file->io_handle,
		     internal_item->file_io_handle,
		     &number_of_sub_folders,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve number of sub folders.",
			 function );

			return( -1 );
		}
		for( sub_folder_index = 0;
		     sub_folder_index < number_of_sub_folders;
		     sub_folder_index++ )
		{
			if( libpff_item_values_get_entry_value(
			     internal_item->sub_item_values[ LIBPFF_FOLDER_SUB_ITEM_SUB_FOLDERS ],
			     internal_item->internal_file->name_to_id_map_list,
			     internal_item->internal_file->io_handle,
			     internal_item->file_io_handle,
			     (int) sub_folder_index,
			     LIBPFF_ENTRY_TYPE_DISPLAY_NAME,
			     &value_type,
			     &value_data_reference,
			     &value_data_cache,
			     LIBPFF_ENTRY_VALUE_FLAG_MATCH_ANY_VALUE_TYPE,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
				 "%s: unable to retrieve entry value.",
				 function );

				return( -1 );
			}
			if( libfdata_reference_get_data(
			     value_data_reference,
			     internal_item->file_io_handle,
			     value_data_cache,
			     &value_data,
			     &value_data_size,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
				 "%s: unable to retrieve value data.",
				 function );

				return( -1 );
			}
			if( value_type == LIBPFF_VALUE_TYPE_STRING_ASCII )
			{
				is_ascii_string = 1;
			}
			result = libpff_value_type_compare_with_utf8_string(
			          value_data,
			          value_data_size,
			          is_ascii_string,
			          internal_item->internal_file->io_handle->ascii_codepage,
			          utf8_sub_folder_name,
			          utf8_sub_folder_name_size,
			          error );

			if( result == -1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_GENERIC,
				 "%s: unable to compare value data with UTF-8 string.",
				 function );

				return( -1 );
			}
			else if( result != 0 )
			{
				break;
			}
		}
		if( sub_folder_index >= number_of_sub_folders )
		{
			return( 0 );
		}
		value_type = LIBPFF_VALUE_TYPE_INTEGER_32BIT_SIGNED;

		if( libpff_item_values_get_entry_value(
		     internal_item->sub_item_values[ LIBPFF_FOLDER_SUB_ITEM_SUB_FOLDERS ],
		     internal_item->internal_file->name_to_id_map_list,
		     internal_item->internal_file->io_handle,
		     internal_item->file_io_handle,
		     (int) sub_folder_index,
		     LIBPFF_ENTRY_TYPE_SUB_ITEM_IDENTIFIER,
		     &value_type,
		     &value_data_reference,
		     &value_data_cache,
		     0,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve entry value.",
			 function );

			return( -1 );
		}
		if( libfdata_reference_get_data(
		     value_data_reference,
		     internal_item->file_io_handle,
		     value_data_cache,
		     &value_data,
		     &value_data_size,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve value data.",
			 function );

			return( -1 );
		}
		if( libpff_value_type_copy_to_32bit(
		     value_data,
		     value_data_size,
		     &sub_folder_descriptor_identifier,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
			 "%s: unable to set 32-bit entry value.",
			 function );

			return( -1 );
		}
		if( libpff_item_tree_get_sub_node_by_identifier(
		     internal_item->item_tree_node,
		     sub_folder_descriptor_identifier,
		     &sub_folder_tree_node,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve sub folder tree node.",
			 function );

			return( -1 );
		}
		if( sub_folder_tree_node == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
			 "%s: invalid sub folder tree node.",
			 function );

			return( -1 );
		}
		if( libpff_item_initialize(
		     sub_folder,
		     internal_item->file_io_handle,
		     internal_item->internal_file,
		     sub_folder_tree_node,
		     (libpff_item_descriptor_t *) sub_folder_tree_node->value,
		     LIBPFF_ITEM_FLAGS_DEFAULT,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
			 "%s: unable to create sub folder.",
			 function );

			return( -1 );
		}
		if( *sub_folder == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
			 "%s: invalid sub folder.",
			 function );

			return( -1 );
		}
		( (libpff_internal_item_t *) *sub_folder )->type = LIBPFF_ITEM_TYPE_FOLDER;
	}
	return( 1 );
}

/* Retrieves the sub folder from a folder for the specific UTF-16 encoded name
 * This function uses the PidTagDisplayName MAPI property as the name
 * Returns 1 if successful, 0 if no such sub folder or -1 on error
 */
int libpff_folder_get_sub_folder_by_utf16_name(
     libpff_item_t *folder,
     uint16_t *utf16_sub_folder_name,
     size_t utf16_sub_folder_name_size,
     libpff_item_t **sub_folder,
     libcerror_error_t **error )
{
	libfcache_cache_t *value_data_cache        = NULL;
	libfdata_reference_t *value_data_reference = NULL;
	libpff_internal_item_t *internal_item      = NULL;
	libpff_tree_node_t *sub_folder_tree_node   = NULL;
	uint8_t *value_data                        = NULL;
	static char *function                      = "libpff_folder_get_sub_folder_by_utf16_name";
	size_t value_data_size                     = 0;
	uint32_t number_of_sub_folders             = 0;
	uint32_t sub_folder_descriptor_identifier  = 0;
	uint32_t sub_folder_index                  = 0;
	uint32_t value_type                        = 0;
	uint8_t is_ascii_string                    = 0;
	int result                                 = 0;

	if( folder == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid folder.",
		 function );

		return( -1 );
	}
	internal_item = (libpff_internal_item_t *) folder;

	if( internal_item->internal_file == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid folder - missing internal file.",
		 function );

		return( -1 );
	}
	if( internal_item->internal_file->io_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid internal item - invalid internal file - missing IO handle.",
		 function );

		return( -1 );
	}
	if( internal_item->type == LIBPFF_ITEM_TYPE_UNDEFINED )
	{
		if( libpff_item_determine_type(
		     internal_item,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to determine item type.",
			 function );

			return( -1 );
		}
	}
	if( internal_item->type != LIBPFF_ITEM_TYPE_FOLDER )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_UNSUPPORTED_VALUE,
		 "%s: unsupported item type: 0x%08" PRIx32 "",
		 function,
		 internal_item->type );

		return( -1 );
	}
	if( utf16_sub_folder_name == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid UTF-16 sub folder name.",
		 function );

		return( -1 );
	}
	if( utf16_sub_folder_name_size > (size_t) SSIZE_MAX )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_EXCEEDS_MAXIMUM,
		 "%s: invalid UTF-16 sub folder name size value exceeds maximum.",
		 function );

		return( -1 );
	}
	if( sub_folder == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid sub folder.",
		 function );

		return( -1 );
	}
	if( *sub_folder != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: sub folder already set.",
		 function );

		return( -1 );
	}
	if( internal_item->sub_item_values[ LIBPFF_FOLDER_SUB_ITEM_SUB_FOLDERS ] == NULL )
	{
		if( libpff_folder_determine_sub_folders(
		     internal_item,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to determine sub folders.",
			 function );

			return( -1 );
		}
	}
	if( internal_item->sub_item_values[ LIBPFF_FOLDER_SUB_ITEM_SUB_FOLDERS ] != NULL )
	{
		if( libpff_item_values_get_number_of_sets(
		     internal_item->sub_item_values[ LIBPFF_FOLDER_SUB_ITEM_SUB_FOLDERS ],
		     internal_item->internal_file->name_to_id_map_list,
		     internal_item->internal_file->io_handle,
		     internal_item->file_io_handle,
		     &number_of_sub_folders,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve number of sub folders.",
			 function );

			return( -1 );
		}
		for( sub_folder_index = 0;
		     sub_folder_index < number_of_sub_folders;
		     sub_folder_index++ )
		{
			if( libpff_item_values_get_entry_value(
			     internal_item->sub_item_values[ LIBPFF_FOLDER_SUB_ITEM_SUB_FOLDERS ],
			     internal_item->internal_file->name_to_id_map_list,
			     internal_item->internal_file->io_handle,
			     internal_item->file_io_handle,
			     (int) sub_folder_index,
			     LIBPFF_ENTRY_TYPE_DISPLAY_NAME,
			     &value_type,
			     &value_data_reference,
			     &value_data_cache,
			     LIBPFF_ENTRY_VALUE_FLAG_MATCH_ANY_VALUE_TYPE,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
				 "%s: unable to retrieve entry value.",
				 function );

				return( -1 );
			}
			if( libfdata_reference_get_data(
			     value_data_reference,
			     internal_item->file_io_handle,
			     value_data_cache,
			     &value_data,
			     &value_data_size,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
				 "%s: unable to retrieve value data.",
				 function );

				return( -1 );
			}
			if( value_type == LIBPFF_VALUE_TYPE_STRING_ASCII )
			{
				is_ascii_string = 1;
			}
			result = libpff_value_type_compare_with_utf16_string(
			          value_data,
			          value_data_size,
			          is_ascii_string,
			          internal_item->internal_file->io_handle->ascii_codepage,
			          utf16_sub_folder_name,
			          utf16_sub_folder_name_size,
			          error );

			if( result == -1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
				 "%s: unable to compare value data with UTF-16 string.",
				 function );

				return( -1 );
			}
			else if( result != 0 )
			{
				break;
			}
		}
		if( sub_folder_index >= number_of_sub_folders )
		{
			return( 0 );
		}
		value_type = LIBPFF_VALUE_TYPE_INTEGER_32BIT_SIGNED;

		if( libpff_item_values_get_entry_value(
		     internal_item->sub_item_values[ LIBPFF_FOLDER_SUB_ITEM_SUB_FOLDERS ],
		     internal_item->internal_file->name_to_id_map_list,
		     internal_item->internal_file->io_handle,
		     internal_item->file_io_handle,
		     (int) sub_folder_index,
		     LIBPFF_ENTRY_TYPE_SUB_ITEM_IDENTIFIER,
		     &value_type,
		     &value_data_reference,
		     &value_data_cache,
		     0,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve entry value.",
			 function );

			return( -1 );
		}
		if( libfdata_reference_get_data(
		     value_data_reference,
		     internal_item->file_io_handle,
		     value_data_cache,
		     &value_data,
		     &value_data_size,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve value data.",
			 function );

			return( -1 );
		}
		if( libpff_value_type_copy_to_32bit(
		     value_data,
		     value_data_size,
		     &sub_folder_descriptor_identifier,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
			 "%s: unable to set 32-bit entry value.",
			 function );

			return( -1 );
		}
		if( libpff_item_tree_get_sub_node_by_identifier(
		     internal_item->item_tree_node,
		     sub_folder_descriptor_identifier,
		     &sub_folder_tree_node,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve sub folder tree node.",
			 function );

			return( -1 );
		}
		if( sub_folder_tree_node == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
			 "%s: invalid sub folder tree node.",
			 function );

			return( -1 );
		}
		if( libpff_item_initialize(
		     sub_folder,
		     internal_item->file_io_handle,
		     internal_item->internal_file,
		     sub_folder_tree_node,
		     (libpff_item_descriptor_t *) sub_folder_tree_node->value,
		     LIBPFF_ITEM_FLAGS_DEFAULT,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
			 "%s: unable to create sub folder.",
			 function );

			return( -1 );
		}
		if( *sub_folder == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
			 "%s: invalid sub folder.",
			 function );

			return( -1 );
		}
		( (libpff_internal_item_t *) *sub_folder )->type = LIBPFF_ITEM_TYPE_FOLDER;
	}
	return( 1 );
}

/* Retrieves the sub folders from a folder
 * Returns 1 if successful, 0 if the item does not contain such value or -1 on error
 */
int libpff_folder_get_sub_folders(
     libpff_item_t *folder,
     libpff_item_t **sub_folders,
     libcerror_error_t **error )
{
	libpff_internal_item_t *internal_item          = NULL;
	libpff_tree_node_t *sub_folders_item_tree_node = NULL;
	static char *function                          = "libpff_folder_get_sub_folders";
	uint32_t sub_folders_descriptor_identifier     = 0;
	int result                                     = 0;

	if( folder == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid folder.",
		 function );

		return( -1 );
	}
	internal_item = (libpff_internal_item_t *) folder;

	if( internal_item->internal_file == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid folder - missing internal file.",
		 function );

		return( -1 );
	}
	if( internal_item->type == LIBPFF_ITEM_TYPE_UNDEFINED )
	{
		if( libpff_item_determine_type(
		     internal_item,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to determine item type.",
			 function );

			return( -1 );
		}
	}
	if( internal_item->type != LIBPFF_ITEM_TYPE_FOLDER )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_UNSUPPORTED_VALUE,
		 "%s: unsupported item type: 0x%08" PRIx32 "",
		 function,
		 internal_item->type );

		return( -1 );
	}
	if( sub_folders == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid sub folders.",
		 function );

		return( -1 );
	}
	if( *sub_folders != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: sub folders already set.",
		 function );

		return( -1 );
	}
	if( internal_item->sub_item_values[ LIBPFF_FOLDER_SUB_ITEM_SUB_FOLDERS ] == NULL )
	{
		if( libpff_folder_determine_sub_folders(
		     internal_item,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to determine sub folders.",
			 function );

			return( -1 );
		}
	}
	if( internal_item->sub_item_values[ LIBPFF_FOLDER_SUB_ITEM_SUB_FOLDERS ] == NULL )
	{
		return( 0 );
	}
	/* Determine the sub folders item descriptor identifier
	 */
	if( libpff_item_tree_get_identifier(
	     internal_item->item_tree_node,
	     &sub_folders_descriptor_identifier,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve folder identifier.",
		 function );

		return( -1 );
	}
	sub_folders_descriptor_identifier += 11;

	/* Find sub folders tree node
	 */
	result = libpff_item_tree_get_tree_node_by_identifier(
	          internal_item->internal_file->item_tree_root_node,
	          sub_folders_descriptor_identifier,
	          &sub_folders_item_tree_node,
	          error );

	if( result == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve sub folders tree node.",
		 function );

		return( -1 );
	}
	if( ( result == 0 )
	 || ( sub_folders_item_tree_node == NULL ) )
	{
		return( 0 );
	}
	if( libpff_item_initialize(
	     sub_folders,
	     internal_item->file_io_handle,
	     internal_item->internal_file,
	     sub_folders_item_tree_node,
	     (libpff_item_descriptor_t *) sub_folders_item_tree_node->value,
	     LIBPFF_ITEM_FLAGS_DEFAULT,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create sub folders.",
		 function );

		return( -1 );
	}
	if( *sub_folders == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid sub folders.",
		 function );

		return( -1 );
	}
	( (libpff_internal_item_t *) *sub_folders )->type = LIBPFF_ITEM_TYPE_SUB_FOLDERS;

	/* Clones the item values sub elements from the cached sub item values
	 */
	if( libpff_item_values_clone_copy(
	     ( (libpff_internal_item_t *) *sub_folders )->item_values,
	     internal_item->sub_item_values[ LIBPFF_FOLDER_SUB_ITEM_SUB_FOLDERS ],
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_COPY_FAILED,
		 "%s: unable to copy sub folders item values.",
		 function );

		libpff_item_free(
		 sub_folders,
		 NULL );

		return( -1 );
	}
	return( 1 );
}

/* Retrieves the number of sub messages from a folder
 * Returns 1 if successful or -1 on error
 */
int libpff_folder_get_number_of_sub_messages(
     libpff_item_t *folder,
     int *number_of_sub_messages,
     libcerror_error_t **error )
{
	libpff_internal_item_t *internal_item = NULL;
	static char *function                 = "libpff_folder_get_number_of_sub_messages";
	uint32_t number_of_sets               = 0;

	if( folder == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid folder.",
		 function );

		return( -1 );
	}
	internal_item = (libpff_internal_item_t *) folder;

	if( internal_item->internal_file == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid folder - missing internal file.",
		 function );

		return( -1 );
	}
	if( internal_item->type == LIBPFF_ITEM_TYPE_UNDEFINED )
	{
		if( libpff_item_determine_type(
		     internal_item,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to determine item type.",
			 function );

			return( -1 );
		}
	}
	if( internal_item->type != LIBPFF_ITEM_TYPE_FOLDER )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_UNSUPPORTED_VALUE,
		 "%s: unsupported item type: 0x%08" PRIx32 "",
		 function,
		 internal_item->type );

		return( -1 );
	}
	if( number_of_sub_messages == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid number of sub messages.",
		 function );

		return( -1 );
	}
	if( internal_item->sub_item_values[ LIBPFF_FOLDER_SUB_ITEM_SUB_MESSAGES ] == NULL )
	{
		if( libpff_folder_determine_sub_messages(
		     internal_item,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to determine sub messages.",
			 function );

			return( -1 );
		}
	}
	*number_of_sub_messages = 0;

	if( internal_item->sub_item_values[ LIBPFF_FOLDER_SUB_ITEM_SUB_MESSAGES ] != NULL )
	{
		if( libpff_item_values_get_number_of_sets(
		     internal_item->sub_item_values[ LIBPFF_FOLDER_SUB_ITEM_SUB_MESSAGES ],
		     internal_item->internal_file->name_to_id_map_list,
		     internal_item->internal_file->io_handle,
		     internal_item->file_io_handle,
		     &number_of_sets,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to determine the number of sub messages.",
			 function );

			return( -1 );
		}
		*number_of_sub_messages = (int) number_of_sets;
	}
	return( 1 );
}

/* Retrieves the sub messages for the specific index from a folder
 * Returns 1 if successful or -1 on error
 */
int libpff_folder_get_sub_message(
     libpff_item_t *folder,
     int sub_message_index,
     libpff_item_t **sub_message,
     libcerror_error_t **error )
{
	libfcache_cache_t *value_data_cache        = NULL;
	libfdata_reference_t *value_data_reference = NULL;
	libpff_internal_item_t *internal_item      = NULL;
	libpff_tree_node_t *sub_message_tree_node  = NULL;
	uint8_t *value_data                        = NULL;
	static char *function                      = "libpff_folder_get_sub_message";
	size_t value_data_size                     = 0;
	uint32_t sub_message_descriptor_identifier = 0;
	uint32_t value_type                        = LIBPFF_VALUE_TYPE_INTEGER_32BIT_SIGNED;

	if( folder == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid folder.",
		 function );

		return( -1 );
	}
	internal_item = (libpff_internal_item_t *) folder;

	if( internal_item->internal_file == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid folder - missing internal file.",
		 function );

		return( -1 );
	}
	if( internal_item->type == LIBPFF_ITEM_TYPE_UNDEFINED )
	{
		if( libpff_item_determine_type(
		     internal_item,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to determine item type.",
			 function );

			return( -1 );
		}
	}
	if( internal_item->type != LIBPFF_ITEM_TYPE_FOLDER )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_UNSUPPORTED_VALUE,
		 "%s: unsupported item type: 0x%08" PRIx32 "",
		 function,
		 internal_item->type );

		return( -1 );
	}
	if( sub_message == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid sub message.",
		 function );

		return( -1 );
	}
	if( *sub_message != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: sub message already set.",
		 function );

		return( -1 );
	}
	if( internal_item->sub_item_values[ LIBPFF_FOLDER_SUB_ITEM_SUB_MESSAGES ] == NULL )
	{
		if( libpff_folder_determine_sub_messages(
		     internal_item,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to determine sub messages.",
			 function );

			return( -1 );
		}
	}
	if( internal_item->sub_item_values[ LIBPFF_FOLDER_SUB_ITEM_SUB_MESSAGES ] != NULL )
	{
		if( libpff_item_values_get_entry_value(
		     internal_item->sub_item_values[ LIBPFF_FOLDER_SUB_ITEM_SUB_MESSAGES ],
		     internal_item->internal_file->name_to_id_map_list,
		     internal_item->internal_file->io_handle,
		     internal_item->file_io_handle,
		     (int) sub_message_index,
		     LIBPFF_ENTRY_TYPE_SUB_ITEM_IDENTIFIER,
		     &value_type,
		     &value_data_reference,
		     &value_data_cache,
		     0,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve entry value.",
			 function );

			return( -1 );
		}
		if( libfdata_reference_get_data(
		     value_data_reference,
		     internal_item->file_io_handle,
		     value_data_cache,
		     &value_data,
		     &value_data_size,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve value data.",
			 function );

			return( -1 );
		}
		if( libpff_value_type_copy_to_32bit(
		     value_data,
		     value_data_size,
		     &sub_message_descriptor_identifier,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
			 "%s: unable to set 32-bit entry value.",
			 function );

			return( -1 );
		}
		if( libpff_item_tree_get_sub_node_by_identifier(
		     internal_item->item_tree_node,
		     sub_message_descriptor_identifier,
		     &sub_message_tree_node,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve sub message tree node.",
			 function );

			return( -1 );
		}
		if( sub_message_tree_node == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
			 "%s: invalid sub message tree node.",
			 function );

			return( -1 );
		}
		if( libpff_item_initialize(
		     sub_message,
		     internal_item->file_io_handle,
		     internal_item->internal_file,
		     sub_message_tree_node,
		     (libpff_item_descriptor_t *) sub_message_tree_node->value,
		     LIBPFF_ITEM_FLAGS_DEFAULT,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
			 "%s: unable to create sub message.",
			 function );

			return( -1 );
		}
		if( *sub_message == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
			 "%s: invalid sub message.",
			 function );

			return( -1 );
		}
		( (libpff_internal_item_t *) *sub_message )->type = LIBPFF_ITEM_TYPE_UNDEFINED;
	}
	return( 1 );
}

/* Retrieves the sub message from a folder for the specific UTF-8 encoded name
 * This function uses the PidTagDisplayName MAPI property as the name
 * Returns 1 if successful, 0 if no such sub message or -1 on error
 */
int libpff_folder_get_sub_message_by_utf8_name(
     libpff_item_t *folder,
     uint8_t *utf8_sub_message_name,
     size_t utf8_sub_message_name_size,
     libpff_item_t **sub_message,
     libcerror_error_t **error )
{
	libfcache_cache_t *value_data_cache        = NULL;
	libfdata_reference_t *value_data_reference = NULL;
	libpff_internal_item_t *internal_item      = NULL;
	libpff_tree_node_t *sub_message_tree_node  = NULL;
	uint8_t *value_data                        = NULL;
	static char *function                      = "libpff_folder_get_sub_message_by_utf8_name";
	size_t value_data_size                     = 0;
	uint32_t number_of_sub_messages            = 0;
	uint32_t sub_message_descriptor_identifier = 0;
	uint32_t sub_message_index                 = 0;
	uint32_t value_type                        = 0;
	uint8_t is_ascii_string                    = 0;
	int result                                 = 0;

	if( folder == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid folder.",
		 function );

		return( -1 );
	}
	internal_item = (libpff_internal_item_t *) folder;

	if( internal_item->internal_file == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid folder - missing internal file.",
		 function );

		return( -1 );
	}
	if( internal_item->internal_file->io_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid internal item - invalid internal file - missing IO handle.",
		 function );

		return( -1 );
	}
	if( internal_item->type == LIBPFF_ITEM_TYPE_UNDEFINED )
	{
		if( libpff_item_determine_type(
		     internal_item,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to determine item type.",
			 function );

			return( -1 );
		}
	}
	if( internal_item->type != LIBPFF_ITEM_TYPE_FOLDER )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_UNSUPPORTED_VALUE,
		 "%s: unsupported item type: 0x%08" PRIx32 "",
		 function,
		 internal_item->type );

		return( -1 );
	}
	if( utf8_sub_message_name == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid UTF-8 sub message name.",
		 function );

		return( -1 );
	}
	if( utf8_sub_message_name_size > (size_t) SSIZE_MAX )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_EXCEEDS_MAXIMUM,
		 "%s: invalid UTF-8 sub message name size value exceeds maximum.",
		 function );

		return( -1 );
	}
	if( sub_message == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid sub message.",
		 function );

		return( -1 );
	}
	if( *sub_message != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: sub message already set.",
		 function );

		return( -1 );
	}
	if( internal_item->sub_item_values[ LIBPFF_FOLDER_SUB_ITEM_SUB_MESSAGES ] == NULL )
	{
		if( libpff_folder_determine_sub_messages(
		     internal_item,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to determine sub messages.",
			 function );

			return( -1 );
		}
	}
	if( internal_item->sub_item_values[ LIBPFF_FOLDER_SUB_ITEM_SUB_MESSAGES ] != NULL )
	{
		if( libpff_item_values_get_number_of_sets(
		     internal_item->sub_item_values[ LIBPFF_FOLDER_SUB_ITEM_SUB_MESSAGES ],
		     internal_item->internal_file->name_to_id_map_list,
		     internal_item->internal_file->io_handle,
		     internal_item->file_io_handle,
		     &number_of_sub_messages,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve number of sub messages.",
			 function );

			return( -1 );
		}
		for( sub_message_index = 0;
		     sub_message_index < number_of_sub_messages;
		     sub_message_index++ )
		{
			if( libpff_item_values_get_entry_value(
			     internal_item->sub_item_values[ LIBPFF_FOLDER_SUB_ITEM_SUB_MESSAGES ],
			     internal_item->internal_file->name_to_id_map_list,
			     internal_item->internal_file->io_handle,
			     internal_item->file_io_handle,
			     (int) sub_message_index,
			     LIBPFF_ENTRY_TYPE_DISPLAY_NAME,
			     &value_type,
			     &value_data_reference,
			     &value_data_cache,
			     LIBPFF_ENTRY_VALUE_FLAG_MATCH_ANY_VALUE_TYPE,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
				 "%s: unable to retrieve entry value.",
				 function );

				return( -1 );
			}
			if( libfdata_reference_get_data(
			     value_data_reference,
			     internal_item->file_io_handle,
			     value_data_cache,
			     &value_data,
			     &value_data_size,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
				 "%s: unable to retrieve value data.",
				 function );

				return( -1 );
			}
			if( value_type == LIBPFF_VALUE_TYPE_STRING_ASCII )
			{
				is_ascii_string = 1;
			}
			result = libpff_value_type_compare_with_utf8_string(
			          value_data,
			          value_data_size,
			          is_ascii_string,
			          internal_item->internal_file->io_handle->ascii_codepage,
			          utf8_sub_message_name,
			          utf8_sub_message_name_size,
			          error );

			if( result == -1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_GENERIC,
				 "%s: unable to compare value data with UTF-8 string.",
				 function );

				return( -1 );
			}
			else if( result != 0 )
			{
				break;
			}
		}
		if( sub_message_index >= number_of_sub_messages )
		{
			return( 0 );
		}
		value_type = LIBPFF_VALUE_TYPE_INTEGER_32BIT_SIGNED;

		if( libpff_item_values_get_entry_value(
		     internal_item->sub_item_values[ LIBPFF_FOLDER_SUB_ITEM_SUB_MESSAGES ],
		     internal_item->internal_file->name_to_id_map_list,
		     internal_item->internal_file->io_handle,
		     internal_item->file_io_handle,
		     (int) sub_message_index,
		     LIBPFF_ENTRY_TYPE_SUB_ITEM_IDENTIFIER,
		     &value_type,
		     &value_data_reference,
		     &value_data_cache,
		     0,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve entry value.",
			 function );

			return( -1 );
		}
		if( libfdata_reference_get_data(
		     value_data_reference,
		     internal_item->file_io_handle,
		     value_data_cache,
		     &value_data,
		     &value_data_size,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve value data.",
			 function );

			return( -1 );
		}
		if( libpff_value_type_copy_to_32bit(
		     value_data,
		     value_data_size,
		     &sub_message_descriptor_identifier,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
			 "%s: unable to set 32-bit entry value.",
			 function );

			return( -1 );
		}
		if( libpff_item_tree_get_sub_node_by_identifier(
		     internal_item->item_tree_node,
		     sub_message_descriptor_identifier,
		     &sub_message_tree_node,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve sub message tree node.",
			 function );

			return( -1 );
		}
		if( sub_message_tree_node == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
			 "%s: invalid sub message tree node.",
			 function );

			return( -1 );
		}
		if( libpff_item_initialize(
		     sub_message,
		     internal_item->file_io_handle,
		     internal_item->internal_file,
		     sub_message_tree_node,
		     (libpff_item_descriptor_t *) sub_message_tree_node->value,
		     LIBPFF_ITEM_FLAGS_DEFAULT,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
			 "%s: unable to create sub message.",
			 function );

			return( -1 );
		}
		if( *sub_message == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
			 "%s: invalid sub message.",
			 function );

			return( -1 );
		}
		( (libpff_internal_item_t *) *sub_message )->type = LIBPFF_ITEM_TYPE_UNDEFINED;
	}
	return( 1 );
}

/* Retrieves the sub message from a folder for the specific UTF-16 encoded name
 * This function uses the PidTagDisplayName MAPI property as the name
 * Returns 1 if successful, 0 if no such sub message or -1 on error
 */
int libpff_folder_get_sub_message_by_utf16_name(
     libpff_item_t *folder,
     uint16_t *utf16_sub_message_name,
     size_t utf16_sub_message_name_size,
     libpff_item_t **sub_message,
     libcerror_error_t **error )
{
	libfcache_cache_t *value_data_cache        = NULL;
	libfdata_reference_t *value_data_reference = NULL;
	libpff_internal_item_t *internal_item      = NULL;
	libpff_tree_node_t *sub_message_tree_node  = NULL;
	uint8_t *value_data                        = NULL;
	static char *function                      = "libpff_folder_get_sub_message_by_utf16_name";
	size_t value_data_size                     = 0;
	uint32_t number_of_sub_messages            = 0;
	uint32_t sub_message_descriptor_identifier = 0;
	uint32_t sub_message_index                 = 0;
	uint32_t value_type                        = 0;
	uint8_t is_ascii_string                    = 0;
	int result                                 = 0;

	if( folder == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid folder.",
		 function );

		return( -1 );
	}
	internal_item = (libpff_internal_item_t *) folder;

	if( internal_item->internal_file == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid folder - missing internal file.",
		 function );

		return( -1 );
	}
	if( internal_item->internal_file->io_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid internal item - invalid internal file - missing IO handle.",
		 function );

		return( -1 );
	}
	if( internal_item->type == LIBPFF_ITEM_TYPE_UNDEFINED )
	{
		if( libpff_item_determine_type(
		     internal_item,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to determine item type.",
			 function );

			return( -1 );
		}
	}
	if( internal_item->type != LIBPFF_ITEM_TYPE_FOLDER )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_UNSUPPORTED_VALUE,
		 "%s: unsupported item type: 0x%08" PRIx32 "",
		 function,
		 internal_item->type );

		return( -1 );
	}
	if( utf16_sub_message_name == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid UTF-16 sub message name.",
		 function );

		return( -1 );
	}
	if( utf16_sub_message_name_size > (size_t) SSIZE_MAX )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_EXCEEDS_MAXIMUM,
		 "%s: invalid UTF-16 sub message name size value exceeds maximum.",
		 function );

		return( -1 );
	}
	if( sub_message == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid sub message.",
		 function );

		return( -1 );
	}
	if( *sub_message != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: sub message already set.",
		 function );

		return( -1 );
	}
	if( internal_item->sub_item_values[ LIBPFF_FOLDER_SUB_ITEM_SUB_MESSAGES ] == NULL )
	{
		if( libpff_folder_determine_sub_messages(
		     internal_item,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to determine sub messages.",
			 function );

			return( -1 );
		}
	}
	if( internal_item->sub_item_values[ LIBPFF_FOLDER_SUB_ITEM_SUB_MESSAGES ] != NULL )
	{
		if( libpff_item_values_get_number_of_sets(
		     internal_item->sub_item_values[ LIBPFF_FOLDER_SUB_ITEM_SUB_MESSAGES ],
		     internal_item->internal_file->name_to_id_map_list,
		     internal_item->internal_file->io_handle,
		     internal_item->file_io_handle,
		     &number_of_sub_messages,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve number of sub messages.",
			 function );

			return( -1 );
		}
		for( sub_message_index = 0;
		     sub_message_index < number_of_sub_messages;
		     sub_message_index++ )
		{
			if( libpff_item_values_get_entry_value(
			     internal_item->sub_item_values[ LIBPFF_FOLDER_SUB_ITEM_SUB_MESSAGES ],
			     internal_item->internal_file->name_to_id_map_list,
			     internal_item->internal_file->io_handle,
			     internal_item->file_io_handle,
			     (int) sub_message_index,
			     LIBPFF_ENTRY_TYPE_DISPLAY_NAME,
			     &value_type,
			     &value_data_reference,
			     &value_data_cache,
			     LIBPFF_ENTRY_VALUE_FLAG_MATCH_ANY_VALUE_TYPE,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
				 "%s: unable to retrieve entry value.",
				 function );

				return( -1 );
			}
			if( libfdata_reference_get_data(
			     value_data_reference,
			     internal_item->file_io_handle,
			     value_data_cache,
			     &value_data,
			     &value_data_size,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
				 "%s: unable to retrieve value data.",
				 function );

				return( -1 );
			}
			if( value_type == LIBPFF_VALUE_TYPE_STRING_ASCII )
			{
				is_ascii_string = 1;
			}
			result = libpff_value_type_compare_with_utf16_string(
			          value_data,
			          value_data_size,
			          is_ascii_string,
			          internal_item->internal_file->io_handle->ascii_codepage,
			          utf16_sub_message_name,
			          utf16_sub_message_name_size,
			          error );

			if( result == -1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
				 "%s: unable to compare value data with UTF-16 string.",
				 function );

				return( -1 );
			}
			else if( result != 0 )
			{
				break;
			}
		}
		if( sub_message_index >= number_of_sub_messages )
		{
			return( 0 );
		}
		value_type = LIBPFF_VALUE_TYPE_INTEGER_32BIT_SIGNED;

		if( libpff_item_values_get_entry_value(
		     internal_item->sub_item_values[ LIBPFF_FOLDER_SUB_ITEM_SUB_MESSAGES ],
		     internal_item->internal_file->name_to_id_map_list,
		     internal_item->internal_file->io_handle,
		     internal_item->file_io_handle,
		     (int) sub_message_index,
		     LIBPFF_ENTRY_TYPE_SUB_ITEM_IDENTIFIER,
		     &value_type,
		     &value_data_reference,
		     &value_data_cache,
		     0,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve entry value.",
			 function );

			return( -1 );
		}
		if( libfdata_reference_get_data(
		     value_data_reference,
		     internal_item->file_io_handle,
		     value_data_cache,
		     &value_data,
		     &value_data_size,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve value data.",
			 function );

			return( -1 );
		}
		if( libpff_value_type_copy_to_32bit(
		     value_data,
		     value_data_size,
		     &sub_message_descriptor_identifier,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
			 "%s: unable to set 32-bit entry value.",
			 function );

			return( -1 );
		}
		if( libpff_item_tree_get_sub_node_by_identifier(
		     internal_item->item_tree_node,
		     sub_message_descriptor_identifier,
		     &sub_message_tree_node,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve sub message tree node.",
			 function );

			return( -1 );
		}
		if( sub_message_tree_node == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
			 "%s: invalid sub message tree node.",
			 function );

			return( -1 );
		}
		if( libpff_item_initialize(
		     sub_message,
		     internal_item->file_io_handle,
		     internal_item->internal_file,
		     sub_message_tree_node,
		     (libpff_item_descriptor_t *) sub_message_tree_node->value,
		     LIBPFF_ITEM_FLAGS_DEFAULT,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
			 "%s: unable to create sub message.",
			 function );

			return( -1 );
		}
		if( *sub_message == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
			 "%s: invalid sub message.",
			 function );

			return( -1 );
		}
		( (libpff_internal_item_t *) *sub_message )->type = LIBPFF_ITEM_TYPE_UNDEFINED;
	}
	return( 1 );
}

/* Retrieves the sub messages from a folder
 * Returns 1 if successful, 0 if the item does not contain such value or -1 on error
 */
int libpff_folder_get_sub_messages(
     libpff_item_t *folder,
     libpff_item_t **sub_messages,
     libcerror_error_t **error )
{
	libpff_internal_item_t *internal_item           = NULL;
	libpff_tree_node_t *sub_messages_item_tree_node = NULL;
	static char *function                           = "libpff_folder_get_sub_messages";
	uint32_t sub_messages_descriptor_identifier     = 0;
	int result                                      = 0;

	if( folder == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid folder.",
		 function );

		return( -1 );
	}
	internal_item = (libpff_internal_item_t *) folder;

	if( internal_item->internal_file == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid folder - missing internal file.",
		 function );

		return( -1 );
	}
	if( internal_item->type == LIBPFF_ITEM_TYPE_UNDEFINED )
	{
		if( libpff_item_determine_type(
		     internal_item,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to determine item type.",
			 function );

			return( -1 );
		}
	}
	if( internal_item->type != LIBPFF_ITEM_TYPE_FOLDER )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_UNSUPPORTED_VALUE,
		 "%s: unsupported item type: 0x%08" PRIx32 "",
		 function,
		 internal_item->type );

		return( -1 );
	}
	if( sub_messages == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid sub messages.",
		 function );

		return( -1 );
	}
	if( *sub_messages != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: sub messages already set.",
		 function );

		return( -1 );
	}
	if( internal_item->sub_item_values[ LIBPFF_FOLDER_SUB_ITEM_SUB_MESSAGES ] == NULL )
	{
		if( libpff_folder_determine_sub_messages(
		     internal_item,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to determine sub messages.",
			 function );

			return( -1 );
		}
	}
	if( internal_item->sub_item_values[ LIBPFF_FOLDER_SUB_ITEM_SUB_MESSAGES ] == NULL )
	{
		return( 0 );
	}
	/* Determine the sub messages item descriptor identifier
	 */
	if( libpff_item_tree_get_identifier(
	     internal_item->item_tree_node,
	     &sub_messages_descriptor_identifier,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve folder identifier.",
		 function );

		return( -1 );
	}
	sub_messages_descriptor_identifier += 12;

	/* Find sub messages tree node
	 */
	result = libpff_item_tree_get_tree_node_by_identifier(
	          internal_item->internal_file->item_tree_root_node,
	          sub_messages_descriptor_identifier,
	          &sub_messages_item_tree_node,
	          error );

	if( result == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve sub messages tree node.",
		 function );

		return( -1 );
	}
	if( ( result == 0 )
	 || ( sub_messages_item_tree_node == NULL ) )
	{
		return( 0 );
	}
	if( libpff_item_initialize(
	     sub_messages,
	     internal_item->file_io_handle,
	     internal_item->internal_file,
	     sub_messages_item_tree_node,
	     (libpff_item_descriptor_t *) sub_messages_item_tree_node->value,
	     LIBPFF_ITEM_FLAGS_DEFAULT,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create sub messages.",
		 function );

		return( -1 );
	}
	if( *sub_messages == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid sub messages.",
		 function );

		return( -1 );
	}
	( (libpff_internal_item_t *) *sub_messages )->type = LIBPFF_ITEM_TYPE_SUB_MESSAGES;

	/* Clones the item values sub elements from the cached sub item values
	 */
	if( libpff_item_values_clone_copy(
	     ( (libpff_internal_item_t *) *sub_messages )->item_values,
	     internal_item->sub_item_values[ LIBPFF_FOLDER_SUB_ITEM_SUB_MESSAGES ],
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_COPY_FAILED,
		 "%s: unable to copy sub messages item values.",
		 function );

		libpff_item_free(
		 sub_messages,
		 NULL );

		return( -1 );
	}
	return( 1 );
}

/* Retrieves the number of sub associated contents from a folder
 * Returns 1 if successful or -1 on error
 */
int libpff_folder_get_number_of_sub_associated_contents(
     libpff_item_t *folder,
     int *number_of_sub_associated_contents,
     libcerror_error_t **error )
{
	libpff_internal_item_t *internal_item = NULL;
	static char *function                 = "libpff_folder_get_number_of_sub_associated_contents";
	uint32_t number_of_sets               = 0;

	if( folder == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid folder.",
		 function );

		return( -1 );
	}
	internal_item = (libpff_internal_item_t *) folder;

	if( internal_item->internal_file == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid folder - missing internal file.",
		 function );

		return( -1 );
	}
	if( internal_item->type == LIBPFF_ITEM_TYPE_UNDEFINED )
	{
		if( libpff_item_determine_type(
		     internal_item,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to determine item type.",
			 function );

			return( -1 );
		}
	}
	if( internal_item->type != LIBPFF_ITEM_TYPE_FOLDER )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_UNSUPPORTED_VALUE,
		 "%s: unsupported item type: 0x%08" PRIx32 "",
		 function,
		 internal_item->type );

		return( -1 );
	}
	if( number_of_sub_associated_contents == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid number of sub associated contents.",
		 function );

		return( -1 );
	}
	if( internal_item->sub_item_values[ LIBPFF_FOLDER_SUB_ITEM_SUB_ASSOCIATED_CONTENTS ] == NULL )
	{
		if( libpff_folder_determine_sub_associated_contents(
		     internal_item,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to determine sub associated contents.",
			 function );

			return( -1 );
		}
	}
	*number_of_sub_associated_contents = 0;

	if( internal_item->sub_item_values[ LIBPFF_FOLDER_SUB_ITEM_SUB_ASSOCIATED_CONTENTS ] != NULL )
	{
		if( libpff_item_values_get_number_of_sets(
		     internal_item->sub_item_values[ LIBPFF_FOLDER_SUB_ITEM_SUB_ASSOCIATED_CONTENTS ],
		     internal_item->internal_file->name_to_id_map_list,
		     internal_item->internal_file->io_handle,
		     internal_item->file_io_handle,
		     &number_of_sets,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to determine the number of sub associated contents.",
			 function );

			return( -1 );
		}
		*number_of_sub_associated_contents = (int) number_of_sets;
	}
	return( 1 );
}

/* Retrieves the sub associated contents for the specific index from a folder
 * Returns 1 if successful or -1 on error
 */
int libpff_folder_get_sub_associated_content(
     libpff_item_t *folder,
     int sub_associated_content_index,
     libpff_item_t **sub_associated_content,
     libcerror_error_t **error )
{
	libfcache_cache_t *value_data_cache                   = NULL;
	libfdata_reference_t *value_data_reference            = NULL;
	libpff_internal_item_t *internal_item                 = NULL;
	libpff_tree_node_t *sub_associated_content_tree_node  = NULL;
	uint8_t *value_data                                   = NULL;
	static char *function                                 = "libpff_folder_get_sub_associated_content";
	size_t value_data_size                                = 0;
	uint32_t sub_associated_content_descriptor_identifier = 0;
	uint32_t value_type                                   = LIBPFF_VALUE_TYPE_INTEGER_32BIT_SIGNED;

	if( folder == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid folder.",
		 function );

		return( -1 );
	}
	internal_item = (libpff_internal_item_t *) folder;

	if( internal_item->internal_file == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid folder - missing internal file.",
		 function );

		return( -1 );
	}
	if( internal_item->type == LIBPFF_ITEM_TYPE_UNDEFINED )
	{
		if( libpff_item_determine_type(
		     internal_item,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to determine item type.",
			 function );

			return( -1 );
		}
	}
	if( internal_item->type != LIBPFF_ITEM_TYPE_FOLDER )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_UNSUPPORTED_VALUE,
		 "%s: unsupported item type: 0x%08" PRIx32 "",
		 function,
		 internal_item->type );

		return( -1 );
	}
	if( sub_associated_content == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid sub associated content.",
		 function );

		return( -1 );
	}
	if( *sub_associated_content != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: sub associated content already set.",
		 function );

		return( -1 );
	}
	if( internal_item->sub_item_values[ LIBPFF_FOLDER_SUB_ITEM_SUB_ASSOCIATED_CONTENTS ] == NULL )
	{
		if( libpff_folder_determine_sub_associated_contents(
		     internal_item,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to determine sub associated contents.",
			 function );

			return( -1 );
		}
	}
	if( internal_item->sub_item_values[ LIBPFF_FOLDER_SUB_ITEM_SUB_ASSOCIATED_CONTENTS ] != NULL )
	{
		if( libpff_item_values_get_entry_value(
		     internal_item->sub_item_values[ LIBPFF_FOLDER_SUB_ITEM_SUB_ASSOCIATED_CONTENTS ],
		     internal_item->internal_file->name_to_id_map_list,
		     internal_item->internal_file->io_handle,
		     internal_item->file_io_handle,
		     (int) sub_associated_content_index,
		     LIBPFF_ENTRY_TYPE_SUB_ITEM_IDENTIFIER,
		     &value_type,
		     &value_data_reference,
		     &value_data_cache,
		     0,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve entry value.",
			 function );

			return( -1 );
		}
		if( libfdata_reference_get_data(
		     value_data_reference,
		     internal_item->file_io_handle,
		     value_data_cache,
		     &value_data,
		     &value_data_size,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve value data.",
			 function );

			return( -1 );
		}
		if( libpff_value_type_copy_to_32bit(
		     value_data,
		     value_data_size,
		     &sub_associated_content_descriptor_identifier,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
			 "%s: unable to set 32-bit entry value.",
			 function );

			return( -1 );
		}
		if( libpff_item_tree_get_sub_node_by_identifier(
		     internal_item->item_tree_node,
		     sub_associated_content_descriptor_identifier,
		     &sub_associated_content_tree_node,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve sub associated content tree node.",
			 function );

			return( -1 );
		}
		if( sub_associated_content_tree_node == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
			 "%s: invalid sub associated content tree node.",
			 function );

			return( -1 );
		}
		if( libpff_item_initialize(
		     sub_associated_content,
		     internal_item->file_io_handle,
		     internal_item->internal_file,
		     sub_associated_content_tree_node,
		     (libpff_item_descriptor_t *) sub_associated_content_tree_node->value,
		     LIBPFF_ITEM_FLAGS_DEFAULT,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
			 "%s: unable to create sub associated content.",
			 function );

			return( -1 );
		}
		if( *sub_associated_content == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
			 "%s: invalid sub associated content.",
			 function );

			return( -1 );
		}
		( (libpff_internal_item_t *) *sub_associated_content )->type = LIBPFF_ITEM_TYPE_UNDEFINED;
	}
	return( 1 );
}

/* Retrieves the sub associated contents from a folder
 * Returns 1 if successful, 0 if the item does not contain such value or -1 on error
 */
int libpff_folder_get_sub_associated_contents(
     libpff_item_t *folder,
     libpff_item_t **sub_associated_contents,
     libcerror_error_t **error )
{
	libpff_internal_item_t *internal_item                      = NULL;
	libpff_tree_node_t *sub_associated_contents_item_tree_node = NULL;
	static char *function                                      = "libpff_folder_get_sub_associated_contents";
	uint32_t sub_associated_contents_descriptor_identifier     = 0;
	int result                                                 = 0;

	if( folder == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid folder.",
		 function );

		return( -1 );
	}
	internal_item = (libpff_internal_item_t *) folder;

	if( internal_item->internal_file == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid folder - missing internal file.",
		 function );

		return( -1 );
	}
	if( internal_item->type == LIBPFF_ITEM_TYPE_UNDEFINED )
	{
		if( libpff_item_determine_type(
		     internal_item,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to determine item type.",
			 function );

			return( -1 );
		}
	}
	if( internal_item->type != LIBPFF_ITEM_TYPE_FOLDER )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_UNSUPPORTED_VALUE,
		 "%s: unsupported item type: 0x%08" PRIx32 "",
		 function,
		 internal_item->type );

		return( -1 );
	}
	if( sub_associated_contents == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid sub associated contents.",
		 function );

		return( -1 );
	}
	if( *sub_associated_contents != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: sub associated contents already set.",
		 function );

		return( -1 );
	}
	if( internal_item->sub_item_values[ LIBPFF_FOLDER_SUB_ITEM_SUB_ASSOCIATED_CONTENTS ] == NULL )
	{
		if( libpff_folder_determine_sub_associated_contents(
		     internal_item,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to determine sub associated contents.",
			 function );

			return( -1 );
		}
	}
	if( internal_item->sub_item_values[ LIBPFF_FOLDER_SUB_ITEM_SUB_ASSOCIATED_CONTENTS ] == NULL )
	{
		return( 0 );
	}
	/* Determine the sub associated contents item descriptor identifier
	 */
	if( libpff_item_tree_get_identifier(
	     internal_item->item_tree_node,
	     &sub_associated_contents_descriptor_identifier,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve folder identifier.",
		 function );

		return( -1 );
	}
	sub_associated_contents_descriptor_identifier += 13;

	/* Find sub associated contents tree node
	 */
	result = libpff_item_tree_get_tree_node_by_identifier(
	          internal_item->internal_file->item_tree_root_node,
	          sub_associated_contents_descriptor_identifier,
	          &sub_associated_contents_item_tree_node,
	          error );

	if( result == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve sub associated contents tree node.",
		 function );

		return( -1 );
	}
	if( ( result == 0 )
	 || ( sub_associated_contents_item_tree_node == NULL ) )
	{
		return( 0 );
	}
	if( libpff_item_initialize(
	     sub_associated_contents,
	     internal_item->file_io_handle,
	     internal_item->internal_file,
	     sub_associated_contents_item_tree_node,
	     (libpff_item_descriptor_t *) sub_associated_contents_item_tree_node->value,
	     LIBPFF_ITEM_FLAGS_DEFAULT,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create sub associated contents.",
		 function );

		return( -1 );
	}
	if( *sub_associated_contents == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid sub associated contents.",
		 function );

		return( -1 );
	}
	( (libpff_internal_item_t *) *sub_associated_contents )->type = LIBPFF_ITEM_TYPE_SUB_ASSOCIATED_CONTENTS;

	/* Clones the item values sub elements from the cached sub item values
	 */
	if( libpff_item_values_clone_copy(
	     ( (libpff_internal_item_t *) *sub_associated_contents )->item_values,
	     internal_item->sub_item_values[ LIBPFF_FOLDER_SUB_ITEM_SUB_ASSOCIATED_CONTENTS ],
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_COPY_FAILED,
		 "%s: unable to copy sub associated contents item values.",
		 function );

		libpff_item_free(
		 sub_associated_contents,
		 NULL );

		return( -1 );
	}
	return( 1 );
}

/* Retrieves the unknowns from a folder
 * Returns 1 if successful, 0 if the folder does not contain such value or -1 on error
 */
int libpff_folder_get_unknowns(
     libpff_item_t *folder,
     libpff_item_t **unknowns,
     libcerror_error_t **error )
{
	libpff_internal_item_t *internal_item = NULL;
	static char *function                 = "libpff_folder_get_unknowns";

	if( folder == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid folder.",
		 function );

		return( -1 );
	}
	internal_item = (libpff_internal_item_t *) folder;

	if( internal_item->internal_file == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid folder - missing internal file.",
		 function );

		return( -1 );
	}
	if( internal_item->type == LIBPFF_ITEM_TYPE_UNDEFINED )
	{
		if( libpff_item_determine_type(
		     internal_item,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to determine item type.",
			 function );

			return( -1 );
		}
	}
	if( internal_item->type != LIBPFF_ITEM_TYPE_FOLDER )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_UNSUPPORTED_VALUE,
		 "%s: unsupported item type: 0x%08" PRIx32 "",
		 function,
		 internal_item->type );

		return( -1 );
	}
	if( unknowns == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid unknowns.",
		 function );

		return( -1 );
	}
	if( *unknowns != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: unknowns already set.",
		 function );

		return( -1 );
	}
	if( internal_item->sub_item_tree_node[ LIBPFF_FOLDER_SUB_ITEM_UNKNOWNS ] == NULL )
	{
		if( libpff_folder_determine_unknowns(
		     internal_item,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to determine unknowns.",
			 function );

			return( -1 );
		}
	}
	if( internal_item->sub_item_tree_node[ LIBPFF_FOLDER_SUB_ITEM_UNKNOWNS ] == NULL )
	{
		return( 0 );
	}
	if( libpff_item_initialize(
	     unknowns,
	     internal_item->file_io_handle,
	     internal_item->internal_file,
	     internal_item->sub_item_tree_node[ LIBPFF_FOLDER_SUB_ITEM_UNKNOWNS ],
	     (libpff_item_descriptor_t *) internal_item->sub_item_tree_node[ LIBPFF_FOLDER_SUB_ITEM_UNKNOWNS ]->value,
	     LIBPFF_ITEM_FLAGS_DEFAULT | LIBPFF_ITEM_FLAG_MANAGED_ITEM_TREE_NODE,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create unknowns.",
		 function );

		return( -1 );
	}
	if( *unknowns == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid unknowns.",
		 function );

		return( -1 );
	}
	( (libpff_internal_item_t *) *unknowns )->type = LIBPFF_ITEM_TYPE_UNKNOWN;

	/* Clones the item values sub elements from the cached sub item values
	 */
	if( libpff_item_values_clone_copy(
	     ( (libpff_internal_item_t *) *unknowns )->item_values,
	     internal_item->sub_item_values[ LIBPFF_FOLDER_SUB_ITEM_UNKNOWNS ],
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_COPY_FAILED,
		 "%s: unable to copy unknowns item values.",
		 function );

		libpff_item_free(
		 unknowns,
		 NULL );

		return( -1 );
	}
	return( 1 );
}

