/*
 * Table functions
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

#include "libpff_array_type.h"
#include "libpff_column_definition.h"
#include "libpff_data_array.h"
#include "libpff_debug.h"
#include "libpff_definitions.h"
#include "libpff_index.h"
#include "libpff_io_handle.h"
#include "libpff_libbfio.h"
#include "libpff_libcerror.h"
#include "libpff_libcnotify.h"
#include "libpff_libfcache.h"
#include "libpff_libfdata.h"
#include "libpff_libfguid.h"
#include "libpff_libfmapi.h"
#include "libpff_libuna.h"
#include "libpff_list_type.h"
#include "libpff_local_descriptor_value.h"
#include "libpff_local_descriptors_tree.h"
#include "libpff_mapi.h"
#include "libpff_name_to_id_map.h"
#include "libpff_record_entry.h"
#include "libpff_table.h"
#include "libpff_tree_type.h"
#include "libpff_types.h"

#include "pff_table.h"

/* Initializes the table index values
 * Returns 1 if successful or -1 on error
 */
int libpff_table_index_values_initialize(
     libpff_table_index_values_t **table_index_values,
     uint16_t number_of_values,
     libcerror_error_t **error )
{
	static char *function          = "libpff_table_index_values_initialize";
	size_t table_index_values_size = 0;

	if( table_index_values == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid table index values.",
		 function );

		return( -1 );
	}
	if( *table_index_values != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid table index values value already set.",
		 function );

		return( -1 );
	}
	*table_index_values = memory_allocate_structure(
	                       libpff_table_index_values_t );

	if( *table_index_values == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create table index values.",
		 function );

		goto on_error;
	}
	if( memory_set(
	     *table_index_values,
	     0,
	     sizeof( libpff_table_index_values_t ) ) == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_SET_FAILED,
		 "%s: unable to clear table index values.",
		 function );

		memory_free(
		 *table_index_values );

		*table_index_values = NULL;

		return( -1 );
	}
/* TODO replace by array type ? */
	table_index_values_size = sizeof( libpff_table_index_value_t ) * number_of_values;

	( *table_index_values )->value = (libpff_table_index_value_t *) memory_allocate(
	                                                                 table_index_values_size );

	if( ( *table_index_values )->value == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create table index values.",
		 function );

		goto on_error;
	}
	if( memory_set(
	     ( *table_index_values )->value,
	     0,
	     table_index_values_size ) == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_SET_FAILED,
		 "%s: unable to clear table index values.",
		 function );

		goto on_error;
	}
	( *table_index_values )->number_of_values = number_of_values;

	return( 1 );

on_error:
	if( *table_index_values != NULL )
	{
		if( ( *table_index_values )->value != NULL )
		{
			memory_free(
			 ( *table_index_values )->value );
		}
		memory_free(
		 *table_index_values );

		*table_index_values = NULL;
	}
	return( -1 );
}

/* Frees table index values
 * Returns 1 if successful or -1 on error
 */
int libpff_table_index_values_free(
     libpff_table_index_values_t **table_index_values,
     libcerror_error_t **error )
{
	static char *function = "libpff_table_index_values_free";

	if( table_index_values == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid table index values.",
		 function );

		return( -1 );
	}
	if( *table_index_values != NULL )
	{
		if( ( *table_index_values )->value != NULL )
		{
			memory_free(
			 ( *table_index_values )->value );
		}
		memory_free(
		 *table_index_values );

		*table_index_values = NULL;
	}
	return( 1 );
}

/* Resizes the record entries
 * Returns 1 if successful or -1 on error
 */
int libpff_record_entries_resize(
     libpff_table_t *table,
     int number_of_sets,
     int number_of_entries,
     libcerror_error_t **error )
{
	static char *function = "libpff_record_entries_resize";
	void *reallocation    = NULL;
	int set_iterator      = 0;

	if( table == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid table.",
		 function );

		return( -1 );
	}
	if( number_of_sets <= 0 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_ZERO_OR_LESS,
		 "%s: invalid number of sets value zero or less.",
		 function );

		return( -1 );
	}
	if( number_of_entries <= 0 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_ZERO_OR_LESS,
		 "%s: invalid number of entries value zero or less.",
		 function );

		return( -1 );
	}
	if( number_of_sets > table->number_of_sets )
	{
		reallocation = memory_reallocate(
		                table->entry,
		                sizeof( libpff_record_entry_t * ) * number_of_sets );

		if( reallocation == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_MEMORY,
			 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
			 "%s: unable to resize record entries.",
			 function );

			return( -1 );
		}
		table->entry = (libpff_record_entry_t **) reallocation;

		if( memory_set(
		     &( table->entry[ table->number_of_sets ] ),
		     0,
		     sizeof( libpff_record_entry_t * ) * ( number_of_sets - table->number_of_sets ) ) == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_MEMORY,
			 LIBCERROR_MEMORY_ERROR_SET_FAILED,
			 "%s: unable to clear record entries.",
			 function );

			return( -1 );
		}
		for( set_iterator = table->number_of_sets;
		     set_iterator < number_of_sets;
		     set_iterator++ )
		{
			table->entry[ set_iterator ] = (libpff_record_entry_t *) memory_allocate(
			                                                          sizeof( libpff_record_entry_t ) * table->number_of_entries );

			if( table->entry[ set_iterator ] == NULL )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_MEMORY,
				 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
				 "%s: unable to create record entries for set: %d.",
				 function,
				 set_iterator );

				return( -1 );
			}
			if( memory_set(
			     table->entry[ set_iterator ],
			     0,
			     sizeof( libpff_record_entry_t ) * table->number_of_entries ) == NULL )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_MEMORY,
				 LIBCERROR_MEMORY_ERROR_SET_FAILED,
				 "%s: unable to clear record entries for set: %d.",
				 function,
				 set_iterator );

				return( -1 );
			}
		}
		table->number_of_sets = number_of_sets;
	}
	if( ( table->number_of_sets > 0 )
	 && ( number_of_entries > table->number_of_entries ) )
	{
		for( set_iterator = 0;
		     set_iterator < table->number_of_sets;
		     set_iterator++ )
		{
			reallocation = memory_reallocate(
			                table->entry[ set_iterator ],
			                sizeof( libpff_record_entry_t ) * number_of_entries );

			if( reallocation == NULL )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_MEMORY,
				 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
				 "%s: unable to resize record entries for set: %d.",
				 function,
				 set_iterator );

				return( -1 );
			}
			table->entry[ set_iterator ] = (libpff_record_entry_t *) reallocation;

			if( memory_set(
			     &( ( table->entry[ set_iterator ] )[ table->number_of_entries ] ),
			     0,
			     sizeof( libpff_record_entry_t ) * ( number_of_entries - table->number_of_entries ) ) == NULL )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_MEMORY,
				 LIBCERROR_MEMORY_ERROR_SET_FAILED,
				 "%s: unable to clear record entries for set: %d.",
				 function,
				 set_iterator );

				return( -1 );
			}
		}
		table->number_of_entries = number_of_entries;
	}
	return( 1 );
}

/* Copies the values in the record entries
 * Returns 1 if successful or -1 on error
 */
int libpff_record_entries_clone(
     libpff_table_t *destination,
     libpff_table_t *source,
     libcerror_error_t **error )
{
	static char *function = "libpff_record_entries_copy";
	int entry_iterator    = 0;
	int set_iterator      = 0;
	int result            = 1;

	if( destination == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid destination table.",
		 function );

		return( -1 );
	}
	if( destination->entry != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid destination table - entries already set.",
		 function );

		return( -1 );
	}
	if( source == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid source table.",
		 function );

		return( -1 );
	}
	if( source->entry == NULL )
	{
		destination->entry             = NULL;
		destination->number_of_sets    = 0;
		destination->number_of_entries = 0;

		return( 1 );
	}
	destination->entry = (libpff_record_entry_t **) memory_allocate(
	                                                 sizeof( libpff_record_entry_t * ) * source->number_of_sets );

	if( destination->entry == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create record entries.",
		 function );

		return( -1 );
	}
	if( memory_set(
	     destination->entry,
	     0,
	     sizeof( libpff_record_entry_t * ) * source->number_of_sets ) == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_SET_FAILED,
		 "%s: unable to clear record entries.",
		 function );

		return( -1 );
	}
	for( set_iterator = 0;
	     set_iterator < source->number_of_sets;
	     set_iterator++ )
	{
		if( source->entry[ set_iterator ] == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
			 "%s: missing source table set: %d entries.",
			 function,
			 set_iterator );

			result = -1;

			break;
		}
		destination->entry[ set_iterator ] = (libpff_record_entry_t *) memory_allocate(
		                                                                sizeof( libpff_record_entry_t ) * source->number_of_entries );

		if( destination->entry[ set_iterator ] == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_MEMORY,
			 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
			 "%s: unable to create destination table set: %d entries.",
			 function,
			 set_iterator );

			result = -1;

			break;
		}
		if( memory_set(
		     destination->entry[ set_iterator ],
		     0,
		     sizeof( libpff_record_entry_t ) * source->number_of_entries ) == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_MEMORY,
			 LIBCERROR_MEMORY_ERROR_SET_FAILED,
			 "%s: unable to clear destination table set: %d entries.",
			 function,
			 set_iterator );

			result = -1;

			break;
		}
		for( entry_iterator = 0;
		     entry_iterator < source->number_of_entries;
		     entry_iterator++ )
		{
			if( memory_copy(
			     &( ( destination->entry[ set_iterator ] )[ entry_iterator ].identifier ),
			     &( ( source->entry[ set_iterator ] )[ entry_iterator ].identifier ),
			     sizeof( libpff_record_entry_identifier_t ) ) == NULL )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_MEMORY,
				 LIBCERROR_MEMORY_ERROR_COPY_FAILED,
				 "%s: unable to copy record entry identifier of table set: %d entry: %d.",
				 function,
				 set_iterator,
				 entry_iterator );

				result = -1;

				break;
			}
			if( libfdata_reference_clone(
			     &( ( destination->entry[ set_iterator ] )[ entry_iterator ].value_data_reference ),
			     ( source->entry[ set_iterator ] )[ entry_iterator ].value_data_reference,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
				 "%s: unable to create destination value data reference of table set: %d entry: %d.",
				 function,
				 set_iterator,
				 entry_iterator );

				result = -1;

				break;
			}
			if( libfcache_cache_clone(
			     &( ( destination->entry[ set_iterator ] )[ entry_iterator ].value_data_cache ),
			     ( source->entry[ set_iterator ] )[ entry_iterator ].value_data_cache,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
				 "%s: unable to create destination value data cache of table set: %d entry: %d.",
				 function,
				 set_iterator,
				 entry_iterator );

				result = -1;

				break;
			}
		}
		if( result != 1 )
		{
			break;
		}
	}
	if( result != 1 )
	{
		while( set_iterator >= 0 ) 
		{
			if( destination->entry[ set_iterator ] != NULL )
			{
				while( entry_iterator >= 0 ) 
				{
					if( ( destination->entry[ set_iterator ] )[ entry_iterator ].value_data_reference != NULL )
					{
						if( libfdata_reference_free(
						     &( ( destination->entry[ set_iterator ] )[ entry_iterator ].value_data_reference ),
						     error ) != 1 )
						{
							libcerror_error_set(
							 error,
							 LIBCERROR_ERROR_DOMAIN_RUNTIME,
							 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
							 "%s: unable to free table set: %d entry: %d value data reference.",
							 function,
							 set_iterator,
							 entry_iterator );
						}
						if( libfcache_cache_free(
						     &( ( destination->entry[ set_iterator ] )[ entry_iterator ].value_data_cache ),
						     error ) != 1 )
						{
							libcerror_error_set(
							 error,
							 LIBCERROR_ERROR_DOMAIN_RUNTIME,
							 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
							 "%s: unable to free table set: %d entry: %d value data cache.",
							 function,
							 set_iterator,
							 entry_iterator );
						}
					}
					entry_iterator--;
				}
				memory_free(
				 destination->entry[ set_iterator ] );
			}
			entry_iterator = source->number_of_entries - 1;

			set_iterator--;
		}
		memory_free(
		 destination->entry );

		destination->entry = NULL;

		return( -1 );
	}
	destination->number_of_sets    = source->number_of_sets;
	destination->number_of_entries = source->number_of_entries;

	return( 1 );
}

/* Creates a table
 * Returns 1 if successful or -1 on error
 */
int libpff_table_initialize(
     libpff_table_t **table,
     uint32_t descriptor_identifier,
     uint64_t data_identifier,
     uint64_t local_descriptors_identifier,
     uint8_t recovered,
     libcerror_error_t **error )
{
	static char *function = "libpff_table_initialize";

	if( table == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid table.",
		 function );

		return( -1 );
	}
	if( *table != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid table value already set.",
		 function );

		return( -1 );
	}
	*table = memory_allocate_structure(
	          libpff_table_t );

	if( *table == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create table.",
		 function );

		goto on_error;
	}
	if( memory_set(
	     *table,
	     0,
	     sizeof( libpff_table_t ) ) == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_SET_FAILED,
		 "%s: unable to clear table.",
		 function );

		goto on_error;
	}
	if( libpff_array_initialize(
	     &( ( *table )->index ),
	     0,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create index array.",
		 function );

		goto on_error;
	}
	( *table )->descriptor_identifier        = descriptor_identifier;
	( *table )->data_identifier              = data_identifier;
	( *table )->local_descriptors_identifier = local_descriptors_identifier;
	( *table )->recovered                    = recovered;

	return( 1 );

on_error:
	if( *table != NULL )
	{
		memory_free(
		 *table );

		*table = NULL;
	}
	return( -1 );
}

/* Frees a table
 * Returns 1 if successful or -1 on error
 */
int libpff_table_free(
     libpff_table_t **table,
     libcerror_error_t **error )
{
	static char *function = "libpff_table_free";
	int entry_iterator    = 0;
	int set_iterator      = 0;
	int result            = 1;

	if( table == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid table.",
		 function );

		return( -1 );
	}
	if( *table != NULL )
	{
		if( ( *table )->descriptor_data_block != NULL )
		{
			if( libfdata_block_free(
			     &( ( *table )->descriptor_data_block ),
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
				 "%s: unable to free descriptor data block.",
				 function );

				result = -1;
			}
		}
		if( ( *table )->descriptor_data_cache != NULL )
		{
			if( libfcache_cache_free(
			     &( ( *table )->descriptor_data_cache ),
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
				 "%s: unable to free descriptor data cache.",
				 function );

				result = -1;
			}
		}
		if( ( *table )->local_descriptors_tree != NULL )
		{
			if( libfdata_tree_free(
			     &( ( *table )->local_descriptors_tree ),
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
				 "%s: unable to free local descriptors tree.",
				 function );

				result = -1;
			}
		}
		if( ( *table )->local_descriptors_cache != NULL )
		{
			if( libfcache_cache_free(
			     &( ( *table )->local_descriptors_cache ),
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
				 "%s: unable to free local descriptors cache.",
				 function );

				result = -1;
			}
		}
		if( ( *table )->index != NULL )
		{
			if( libpff_array_free(
			     &( ( *table )->index ),
			     (int (*)(intptr_t **, libcerror_error_t **)) &libpff_table_index_values_free,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
				 "%s: unable to free index array.",
				 function );

				result = -1;
			}
		}
		if( ( *table )->record_entries_data_references != NULL )
		{
			if( libpff_array_free(
			     &( ( *table )->record_entries_data_references ),
			     (int (*)(intptr_t **, libcerror_error_t **)) &libfdata_reference_free,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
				 "%s: unable to free record entries data references array.",
				 function );

				result = -1;
			}
		}
		if( ( *table )->entry != NULL )
		{
			for( set_iterator = 0;
			     set_iterator < ( *table )->number_of_sets;
			     set_iterator++ )
			{
				for( entry_iterator = 0;
				     entry_iterator < ( *table )->number_of_entries;
				     entry_iterator++ )
				{
					if( ( ( *table )->entry[ set_iterator ] )[ entry_iterator ].value_data_reference != NULL )
					{
						if( libfdata_reference_free(
						     &( ( ( *table )->entry[ set_iterator ] )[ entry_iterator ].value_data_reference ),
						     error ) != 1 )
						{
							libcerror_error_set(
							 error,
							 LIBCERROR_ERROR_DOMAIN_RUNTIME,
							 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
							 "%s: unable to free table set: %d entry: %d value data reference.",
							 function,
							 set_iterator,
							 entry_iterator );

							result = -1;
						}
					}
					if( ( ( *table )->entry[ set_iterator ] )[ entry_iterator ].value_data_cache != NULL )
					{
						if( libfcache_cache_free(
						     &( ( ( *table )->entry[ set_iterator ] )[ entry_iterator ].value_data_cache ),
						     error ) != 1 )
						{
							libcerror_error_set(
							 error,
							 LIBCERROR_ERROR_DOMAIN_RUNTIME,
							 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
							 "%s: unable to free table set: %d entry: %d value data cache.",
							 function,
							 set_iterator,
							 entry_iterator );

							result = -1;
						}
					}
				}
				memory_free(
				 ( *table )->entry[ set_iterator ] );
			}
			memory_free(
			 ( *table )->entry );

			( *table )->entry = NULL;
		}
		memory_free(
		 *table );

		*table = NULL;
	}
	return( result );
}

/* Clones the existing table
 * Returns 1 if successful or -1 on error
 */
int libpff_table_clone(
     libpff_table_t **destination_table,
     libpff_table_t *source_table,
     libcerror_error_t **error )
{
	static char *function = "libpff_table_clone";

	if( destination_table == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid destination table.",
		 function );

		return( -1 );
	}
	if( *destination_table != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid destination table value already set.",
		 function );

		return( -1 );
	}
	if( source_table == NULL )
	{
		*destination_table = NULL;

		return( 1 );
	}
	if( libpff_table_initialize(
	     destination_table,
	     source_table->descriptor_identifier,
	     source_table->data_identifier,
	     source_table->local_descriptors_identifier,
	     source_table->recovered,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create destination table.",
		 function );

		goto on_error;
	}
/* TODO is this necessary or should it be re-read on demand ? */
	if( source_table->local_descriptors_tree != NULL )
	{
		if( libfdata_tree_clone(
		     &( ( *destination_table )->local_descriptors_tree ),
		     source_table->local_descriptors_tree,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
			 "%s: unable to create destination local descriptors tree.",
			 function );

			goto on_error;
		}
		if( libfcache_cache_clone(
		     &( ( *destination_table )->local_descriptors_cache ),
		     source_table->local_descriptors_cache,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
			 "%s: unable to create destination local descriptors cache.",
			 function );

			goto on_error;
		}
	}
	if( libpff_record_entries_clone(
	     *destination_table,
	     source_table,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to clone record entries.",
		 function );

		goto on_error;
	}
	return( 1 );

on_error:
	if( *destination_table != NULL )
	{
		libpff_table_free(
		 destination_table,
		 NULL );
	}
	return( -1 );
}

/* Retrieves the local descriptor value for the specific identifier
 * Returns 1 if successful, 0 if no value was found or -1 on error
 */
int libpff_table_get_local_descriptors_value_by_identifier(
     libpff_table_t *table,
     libbfio_handle_t *file_io_handle,
     uint32_t descriptor_identifier,
     libpff_local_descriptor_value_t **local_descriptor_value,
     libcerror_error_t **error )
{
	static char *function = "libpff_table_get_local_descriptors_value_by_identifier";
	int result            = 0;

	if( table == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid table.",
		 function );

		return( -1 );
	}
	if( table->local_descriptors_tree != NULL )
	{
		result = libpff_local_descriptors_tree_get_value_by_identifier(
			  table->local_descriptors_tree,
			  file_io_handle,
			  table->local_descriptors_cache,
			  (uint64_t) descriptor_identifier,
			  local_descriptor_value,
			  error );

		if( result == -1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve local descriptor identifier: %" PRIu32 ".",
			 function,
			 descriptor_identifier );

			return( -1 );
		}
	}
	return( result );
}

/* Retrieves the table index value for a specific reference
 * Returns 1 if successful or -1 on error
 */
int libpff_table_get_index_value_by_reference(
     libpff_table_t *table,
     uint32_t table_index_reference,
     libpff_table_index_value_t **table_index_value,
     libcerror_error_t **error )
{
	libpff_table_index_values_t *table_index_values = NULL;
	static char *function                           = "libpff_table_get_index_value_by_reference";
	uint16_t table_index_array_reference            = 0;
	uint16_t table_index_value_reference            = 0;

	if( table == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid table.",
		 function );

		return( -1 );
	}
	if( table_index_value == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid table index value.",
		 function );

		return( -1 );
	}
	if( ( table_index_reference & 0x0000001fUL ) != 0 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_UNSUPPORTED_VALUE,
		 "%s: unsupported table index reference: 0x%08" PRIx32 " (0x%08" PRIx32 ").",
		 function,
		 table_index_reference & 0x0000001fUL,
		 table_index_reference );

		return( -1 );
	}
	/* Determine the index array reference
	 */
	table_index_array_reference = (uint16_t) ( table_index_reference >> 16 );

	if( libpff_array_get_entry_by_index(
	     table->index,
	     (int) table_index_array_reference,
	     (intptr_t **) &table_index_values,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve table index array entry: %" PRIu16 ".",
		 function,
		 table_index_array_reference );

		return( -1 );
	}
	if( table_index_values == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: missing table index values.",
		 function );

		return( -1 );
	}
	table_index_value_reference = (uint16_t) ( ( table_index_reference & 0x0000ffe0 ) >> 5 ) - 1;

	if( table_index_value_reference >= table_index_values->number_of_values )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: invalid table index value reference value out of bounds.",
		 function );

		return( -1 );
	}
#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		libcnotify_printf(
		 "%s: retrieving table index array entry: %" PRIu16 " value: %" PRIu16 ".\n",
		 function,
		 table_index_array_reference,
		 table_index_value_reference );
	}
#endif
	*table_index_value = &( table_index_values->value[ table_index_value_reference ] );

	return( 1 );
}

/* Retrieves the number of sets
 * Returns 1 if successful or -1 on error
 */
int libpff_table_get_number_of_sets(
     libpff_table_t *table,
     int *number_of_sets,
     libcerror_error_t **error )
{
	static char *function = "libpff_table_get_number_of_sets";

	if( table == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid table.",
		 function );

		return( -1 );
	}
	if( number_of_sets == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid number of sets.",
		 function );

		return( -1 );
	}
	*number_of_sets = table->number_of_sets;

	return( 1 );
}

/* Retrieves the number of entries
 * Returns 1 if successful or -1 on error
 */
int libpff_table_get_number_of_entries(
     libpff_table_t *table,
     int *number_of_entries,
     libcerror_error_t **error )
{
	static char *function = "libpff_table_get_number_of_entries";

	if( table == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid table.",
		 function );

		return( -1 );
	}
	if( number_of_entries == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid number of entries.",
		 function );

		return( -1 );
	}
	*number_of_entries = table->number_of_entries;

	return( 1 );
}

/* Retrieves the entry and value type of a the entry matching the index from a table
 * Returns 1 if successful or -1 on error
 */
int libpff_table_get_entry_type_by_index(
     libpff_table_t *table,
     int set_index,
     int entry_index,
     uint32_t *entry_type,
     uint32_t *value_type,
     libpff_name_to_id_map_entry_t **name_to_id_map_entry,
     libcerror_error_t **error )
{
	libpff_record_entry_t *record_entry = NULL;
	static char *function               = "libpff_table_get_entry_type_by_index";

	if( table == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid table.",
		 function );

		return( -1 );
	}
	if( table->number_of_sets == 0 )
	{
		return( 0 );
	}
	if( table->entry == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid table - missing entries.",
		 function );

		return( -1 );
	}
	if( entry_type == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid entry type.",
		 function );

		return( -1 );
	}
	if( value_type == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid value type.",
		 function );

		return( -1 );
	}
	if( name_to_id_map_entry == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid name to id map entry.",
		 function );

		return( -1 );
	}
	if( ( set_index < 0 )
	 || ( set_index >= table->number_of_sets ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: invalid set index value out of bounds.",
		 function );

		return( -1 );
	}
#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		libcnotify_printf(
		 "%s: retrieving table set: %d entry index: %d\n",
		 function,
		 set_index,
		 entry_index );
	}
#endif
	if( table->entry[ set_index ] == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid table - missing entry set.",
		 function );

		return( -1 );
	}
	record_entry = table->entry[ set_index ];

	if( ( entry_index < 0 )
	 || ( entry_index >= table->number_of_entries ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: invalid entry index value out of bounds.",
		 function );

		return( -1 );
	}
	if( ( record_entry[ entry_index ] ).identifier.format != LIBPFF_RECORD_ENTRY_IDENTIFIER_FORMAT_MAPI_PROPERTY )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
		 "%s: unsupported record entry identifier format: %" PRIu8 ".",
		 function,
		 ( record_entry[ entry_index ] ).identifier.format );

		return( -1 );
	}
	*entry_type           = ( record_entry[ entry_index ] ).identifier.entry_type;
	*value_type           = ( record_entry[ entry_index ] ).identifier.value_type;
	*name_to_id_map_entry = (libpff_name_to_id_map_entry_t *) ( record_entry[ entry_index ] ).name_to_id_map_entry;

	return( 1 );
}

/* Retrieves the value of a the entry matching the index from a table
 * Returns 1 if successful, 0 if the item does not contain such value or -1 on error
 */
int libpff_table_get_entry_value_by_index(
     libpff_table_t *table,
     int set_index,
     int entry_index,
     libfdata_reference_t **value_data_reference,
     libfcache_cache_t **value_data_cache,
     libcerror_error_t **error )
{
	libpff_record_entry_t *record_entry = NULL;
	static char *function               = "libpff_table_get_entry_value_by_index";

	if( table == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid table.",
		 function );

		return( -1 );
	}
	if( table->number_of_sets == 0 )
	{
		return( 0 );
	}
	if( table->entry == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid table - missing entries.",
		 function );

		return( -1 );
	}
	if( ( set_index < 0 )
	 || ( set_index >= table->number_of_sets ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: invalid set index value out of bounds.",
		 function );

		return( -1 );
	}
	if( value_data_reference == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid value data reference.",
		 function );

		return( -1 );
	}
	if( value_data_cache == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid value data cache.",
		 function );

		return( -1 );
	}
#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		libcnotify_printf(
		 "%s: retrieving entry index: %d from set: %d\n",
		 function,
		 entry_index,
		 set_index );
	}
#endif
	if( table->entry[ set_index ] == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid table - missing entry set.",
		 function );

		return( -1 );
	}
	record_entry = table->entry[ set_index ];

	if( ( entry_index < 0 )
	 || ( entry_index >= table->number_of_entries ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: invalid entry index value out of bounds.",
		 function );

		return( -1 );
	}
	*value_data_reference = ( record_entry[ entry_index ] ).value_data_reference;
	*value_data_cache     = ( record_entry[ entry_index ] ).value_data_cache;

	return( 1 );
}

/* Retrieves the value of a the entry matching the entry type from a table
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
int libpff_table_get_entry_value_by_entry_type(
     libpff_table_t *table,
     int set_index,
     uint32_t entry_type,
     uint32_t *value_type,
     libfdata_reference_t **value_data_reference,
     libfcache_cache_t **value_data_cache,
     uint8_t flags,
     libcerror_error_t **error )
{
	libpff_record_entry_t *record_entry = NULL;
	static char *function               = "libpff_table_get_entry_value_by_entry_type";
	int entry_type_match                = 0;
	int record_entry_index              = 0;

	if( table == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid table.",
		 function );

		return( -1 );
	}
	if( table->number_of_sets == 0 )
	{
		return( 0 );
	}
	if( table->entry == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid table - missing entries.",
		 function );

		return( -1 );
	}
	if( ( set_index < 0 )
	 || ( set_index >= table->number_of_sets ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: invalid set index value out of bounds.",
		 function );

		return( -1 );
	}
	if( value_type == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid value type.",
		 function );

		return( -1 );
	}
	if( value_data_reference == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid value data reference.",
		 function );

		return( -1 );
	}
	if( value_data_cache == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid value data cache.",
		 function );

		return( -1 );
	}
#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		libcnotify_printf(
		 "%s: retrieving table set: %d entry type: 0x%04" PRIx32 "\n",
		 function,
		 set_index,
		 entry_type );
	}
#endif
	if( table->entry[ set_index ] == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid table - missing entry set.",
		 function );

		return( -1 );
	}
	record_entry = table->entry[ set_index ];

	for( record_entry_index = 0;
	     record_entry_index < table->number_of_entries;
	     record_entry_index++ )
	{
		/* Skip tables entries that do not contain a MAPI identifier
		 */
		if( ( record_entry[ record_entry_index ] ).identifier.format != LIBPFF_RECORD_ENTRY_IDENTIFIER_FORMAT_MAPI_PROPERTY )
		{
			continue;
		}
		/* Ignore the name to identifier map
		 */
		if( ( flags & LIBPFF_ENTRY_VALUE_FLAG_IGNORE_NAME_TO_ID_MAP ) == LIBPFF_ENTRY_VALUE_FLAG_IGNORE_NAME_TO_ID_MAP )
		{
			if( ( record_entry[ record_entry_index ] ).identifier.entry_type == entry_type )
			{
				entry_type_match = 1;
			}
		}
		else
		{
			/* Mapped properties must be accessed through their mapped entry type value
			 */
			if( ( record_entry[ record_entry_index ] ).name_to_id_map_entry != NULL )
			{
				if( ( record_entry[ record_entry_index ] ).name_to_id_map_entry->type == LIBPFF_NAME_TO_ID_MAP_ENTRY_TYPE_NUMERIC )
				{
					if( ( record_entry[ record_entry_index ] ).name_to_id_map_entry->numeric_value == entry_type )
					{
						entry_type_match = 1;
					}
				}
			}
			/* Other properties are accessed through their entry type value
			 */
			else if( ( record_entry[ record_entry_index ] ).identifier.entry_type == entry_type )
			{
				entry_type_match = 1;
			}
		}
		if( entry_type_match != 0 )
		{
			if( ( ( flags & LIBPFF_ENTRY_VALUE_FLAG_MATCH_ANY_VALUE_TYPE ) == LIBPFF_ENTRY_VALUE_FLAG_MATCH_ANY_VALUE_TYPE )
			 || ( ( record_entry[ record_entry_index ] ).identifier.value_type == *value_type ) )
			{
				if( ( flags & LIBPFF_ENTRY_VALUE_FLAG_MATCH_ANY_VALUE_TYPE ) == LIBPFF_ENTRY_VALUE_FLAG_MATCH_ANY_VALUE_TYPE )
				{
					*value_type = ( record_entry[ record_entry_index ] ).identifier.value_type;
				}
				*value_data_reference = ( record_entry[ record_entry_index ] ).value_data_reference;
				*value_data_cache     = ( record_entry[ record_entry_index ] ).value_data_cache;

				return( 1 );
			}
		}
	}
	return( 0 );
}

/* Retrieves the value of an UTF-8 encoded entry name matching the entry name from a table
 * Returns 1 if successful, 0 if the item does not contain such value or -1 on error
 */
int libpff_table_get_entry_value_by_utf8_entry_name(
     libpff_table_t *table,
     int set_index,
     uint8_t *utf8_entry_name,
     size_t utf8_entry_name_length,
     uint32_t *value_type,
     libfdata_reference_t **value_data_reference,
     libfcache_cache_t **value_data_cache,
     libcerror_error_t **error )
{
	libpff_record_entry_t *record_entry = NULL;
	static char *function               = "libpff_table_get_entry_value_by_utf8_entry_name";
	int result                          = 0;
	int record_entry_index              = 0;

	if( table == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid table.",
		 function );

		return( -1 );
	}
	if( table->number_of_sets == 0 )
	{
		return( 0 );
	}
	if( table->entry == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid table - missing entries.",
		 function );

		return( -1 );
	}
	if( ( set_index < 0 )
	 || ( set_index >= table->number_of_sets ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: invalid set index value out of bounds.",
		 function );

		return( -1 );
	}
	if( utf8_entry_name == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid UTF-8 entry name.",
		 function );

		return( -1 );
	}
	if( utf8_entry_name_length == 0 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_ZERO_OR_LESS,
		 "%s: invalid UTF-8 entry name length value zero or less.",
		 function );

		return( -1 );
	}
	if( utf8_entry_name_length > (size_t) SSIZE_MAX )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_EXCEEDS_MAXIMUM,
		 "%s: invalid UTF-8 entry name length value exceeds maximum.",
		 function );

		return( -1 );
	}
	if( value_type == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid value type.",
		 function );

		return( -1 );
	}
	if( value_data_reference == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid value data reference.",
		 function );

		return( -1 );
	}
	if( value_data_cache == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid value data cache.",
		 function );

		return( -1 );
	}
#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		libcnotify_printf(
		 "%s: retrieving table set: %d entry name: %s\n",
		 function,
		 set_index,
		 utf8_entry_name );
	}
#endif
	if( table->entry[ set_index ] == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid table - missing entry set.",
		 function );

		return( -1 );
	}
	record_entry = table->entry[ set_index ];

	for( record_entry_index = 0;
	     record_entry_index < table->number_of_entries;
	     record_entry_index++ )
	{
		/* There is a match
		 * if there is a name-to-id map entry
		 *   and the name-to-id map entry is a string
		 *   and the entry name string matches the one in the name-to-id map
		 */
		if( ( ( record_entry[ record_entry_index ] ).name_to_id_map_entry != NULL )
		 && ( ( record_entry[ record_entry_index ] ).name_to_id_map_entry->type == LIBPFF_NAME_TO_ID_MAP_ENTRY_TYPE_STRING ) )
		{
			if( ( record_entry[ record_entry_index ] ).name_to_id_map_entry->is_ascii_string == 0 )
			{
				result = libuna_utf8_string_compare_with_utf16_stream(
				          utf8_entry_name,
				          utf8_entry_name_length + 1,
				          ( record_entry[ record_entry_index ] ).name_to_id_map_entry->string_value,
				          ( record_entry[ record_entry_index ] ).name_to_id_map_entry->value_size,
				          LIBPFF_ENDIAN_LITTLE,
				          error );
			}
			else
			{
				result = libuna_utf8_string_compare_with_byte_stream(
				          utf8_entry_name,
				          utf8_entry_name_length + 1,
				          ( record_entry[ record_entry_index ] ).name_to_id_map_entry->string_value,
				          ( record_entry[ record_entry_index ] ).name_to_id_map_entry->value_size,
				          LIBUNA_CODEPAGE_ASCII,
				          error );
			}
			if( result == -1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_GENERIC,
				 "%s: unable to compare entry name.",
				 function );

				return( -1 );
			}
			else if( result != 0 )
			{
				*value_type           = ( record_entry[ record_entry_index ] ).identifier.value_type;
				*value_data_reference = ( record_entry[ record_entry_index ] ).value_data_reference;
				*value_data_cache     = ( record_entry[ record_entry_index ] ).value_data_cache;

				return( 1 );
			}
		}
	}
	return( 0 );
}

/* Retrieves the value of an UTF-16 encoded entry name matching the entry name from a table
 * Returns 1 if successful, 0 if the item does not contain such value or -1 on error
 */
int libpff_table_get_entry_value_by_utf16_entry_name(
     libpff_table_t *table,
     int set_index,
     uint16_t *utf16_entry_name,
     size_t utf16_entry_name_length,
     uint32_t *value_type,
     libfdata_reference_t **value_data_reference,
     libfcache_cache_t **value_data_cache,
     libcerror_error_t **error )
{
	libpff_record_entry_t *record_entry = NULL;
	static char *function               = "libpff_table_get_entry_value_by_utf16_entry_name";
	int result                          = 0;
	int record_entry_index              = 0;

	if( table == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid table.",
		 function );

		return( -1 );
	}
	if( table->number_of_sets == 0 )
	{
		return( 0 );
	}
	if( table->entry == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid table - missing entries.",
		 function );

		return( -1 );
	}
	if( ( set_index < 0 )
	 || ( set_index >= table->number_of_sets ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: invalid set index value out of bounds.",
		 function );

		return( -1 );
	}
	if( utf16_entry_name == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid UTF-16 entry name.",
		 function );

		return( -1 );
	}
	if( utf16_entry_name_length == 0 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_ZERO_OR_LESS,
		 "%s: invalid UTF-16 entry name length value zero or less.",
		 function );

		return( -1 );
	}
	if( utf16_entry_name_length > (size_t) SSIZE_MAX )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_EXCEEDS_MAXIMUM,
		 "%s: invalid UTF-16 entry name length value exceeds maximum.",
		 function );

		return( -1 );
	}
	if( value_type == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid value type.",
		 function );

		return( -1 );
	}
	if( value_data_reference == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid value data reference.",
		 function );

		return( -1 );
	}
	if( value_data_cache == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid value data cache.",
		 function );

		return( -1 );
	}
	if( table->entry[ set_index ] == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid table - missing entry set.",
		 function );

		return( -1 );
	}
#if defined( HAVE_DEBUG_OUTPUT )
/* TODO add system string support 
	if( libcnotify_verbose != 0 )
	{
		libcnotify_printf(
		 "%s: retrieving table set: %d entry name: %" PRIs_LIBCSTRING_SYSTEM "\n",
		 function,
		 set_index,
		 utf16_entry_name );
	}
*/
#endif
	record_entry = table->entry[ set_index ];

	for( record_entry_index = 0;
	     record_entry_index < table->number_of_entries;
	     record_entry_index++ )
	{
		/* There is a match
		 * if there is a name-to-id map entry
		 *   and the name-to-id map entry is a string
		 *   and the entry name string matches the one in the name-to-id map
		 */
		if( ( ( record_entry[ record_entry_index ] ).name_to_id_map_entry != NULL )
		 && ( ( record_entry[ record_entry_index ] ).name_to_id_map_entry->type == LIBPFF_NAME_TO_ID_MAP_ENTRY_TYPE_STRING ) )
		{
			if( ( record_entry[ record_entry_index ] ).name_to_id_map_entry->is_ascii_string == 0 )
			{
				result = libuna_utf16_string_compare_with_utf16_stream(
				          utf16_entry_name,
				          utf16_entry_name_length + 1,
				          ( record_entry[ record_entry_index ] ).name_to_id_map_entry->string_value,
				          ( record_entry[ record_entry_index ] ).name_to_id_map_entry->value_size,
				          LIBPFF_ENDIAN_LITTLE,
				          error );
			}
			else
			{
				result = libuna_utf16_string_compare_with_byte_stream(
				          utf16_entry_name,
				          utf16_entry_name_length + 1,
				          ( record_entry[ record_entry_index ] ).name_to_id_map_entry->string_value,
				          ( record_entry[ record_entry_index ] ).name_to_id_map_entry->value_size,
				          LIBUNA_CODEPAGE_ASCII,
				          error );
			}
			if( result == -1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_GENERIC,
				 "%s: unable to compare entry name.",
				 function );

				return( -1 );
			}
			else if( result != 0 )
			{
				*value_type           = ( record_entry[ record_entry_index ] ).identifier.value_type;
				*value_data_reference = ( record_entry[ record_entry_index ] ).value_data_reference;
				*value_data_cache     = ( record_entry[ record_entry_index ] ).value_data_cache;

				return( 1 );
			}
		}
	}
	return( 0 );
}

/* Reads a table and its values
 * Returns 1 if successful or -1 on error
 */
int libpff_table_read(
     libpff_table_t *table,
     libpff_io_handle_t *io_handle,
     libbfio_handle_t *file_io_handle,
     libpff_list_t *name_to_id_map_list,
     int debug_item_type,
     libcerror_error_t **error )
{
	uint8_t *table_data                             = NULL;
	static char *function                           = "libpff_table_read";
	size_t table_data_size                          = 0;
	uint32_t table_value_reference                  = 0;
	uint8_t table_signature                         = 0;
	uint8_t table_type                              = 0;

#if defined( HAVE_DEBUG_OUTPUT )
	libpff_table_index_values_t *table_index_values = NULL;
	uint16_t table_index_values_iterator            = 0;
	int number_of_table_index_array_entries         = 0;
	int table_index_array_iterator                  = 0;
#endif

	if( table == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid table.",
		 function );

		return( -1 );
	}
	if( table->data_identifier == 0 )
	{
#ifdef TODO
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid table - missing data identifier.",
		 function );
#else
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid table: %" PRIu32 " - missing data identifier.",
		 function,
		 table->descriptor_identifier );
#endif
		return( -1 );
	}
	if( table->local_descriptors_tree != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid table - local descriptors tree already set.",
		 function );

		return( -1 );
	}
	if( table->local_descriptors_cache != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid table - local descriptors cache already set.",
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
	if( table->local_descriptors_identifier > 0 )
	{
		if( libpff_io_handle_read_descriptor_local_descriptors_tree(
		     io_handle,
		     file_io_handle,
		     table->local_descriptors_identifier,
		     table->recovered,
		     table->recovered_local_descriptors_identifier_value_index,
		     &( table->local_descriptors_tree ),
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_IO,
			 LIBCERROR_IO_ERROR_READ_FAILED,
			 "%s: unable to read local descriptors tree with identifier: %" PRIu64 ".",
			 function,
			 table->local_descriptors_identifier );

			return( -1 );
		}
		if( libfcache_cache_initialize(
		     &( table->local_descriptors_cache ),
		     LIBPFF_MAXIMUM_CACHE_ENTRIES_LOCAL_DESCRIPTORS_VALUES,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
			 "%s: unable to create local descriptors cache.",
			 function );

			if( table->local_descriptors_tree != NULL )
			{
				libfdata_tree_free(
				 &( table->local_descriptors_tree ),
				 NULL );
			}
			return( -1 );
		}
	}
	if( libpff_io_handle_read_descriptor_data_block(
	     io_handle,
	     file_io_handle,
	     table->descriptor_identifier,
	     table->data_identifier,
	     table->recovered,
	     table->recovered_data_identifier_value_index,
	     &( table->descriptor_data_block ),
	     &( table->descriptor_data_cache ),
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_READ_FAILED,
		 "%s: unable to read descriptor: %" PRIu32 " data: %" PRIu64 " block.",
		 function,
		 table->descriptor_identifier,
		 table->data_identifier );

		return( -1 );
	}
	/* Retrieve the table array block first segment data
	 */
	if( libfdata_block_get_segment_data(
	     table->descriptor_data_block,
	     file_io_handle,
	     table->descriptor_data_cache,
	     0,
	     &table_data,
	     &table_data_size,
	     0,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve data reference segment: 0 data.",
		 function );

		return( -1 );
	}
	if( table_data == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: missing data reference segment: 0 data.",
		 function );

		return( -1 );
	}
	if( table_data_size < sizeof( pff_table_t ) )
	{
#if defined( HAVE_DEBUG_OUTPUT )
		if( libcnotify_verbose != 0 )
		{
			libcnotify_printf(
			 "%s: descriptor data:\n",
			 function );
			libcnotify_print_data(
			 table_data,
			 table_data_size,
			 0 );
		}
#endif
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_TOO_SMALL,
		 "%s: data reference entry: 0 too small to be a table.",
		 function );

		return( -1 );
	}
#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		libcnotify_printf(
		 "%s: table start:\n",
		 function );
		libcnotify_print_data(
		 table_data,
		 sizeof( pff_table_t ),
		 0 );
	}
#endif
	table_signature = ( (pff_table_t *) table_data )->signature;
	table_type      = ( (pff_table_t *) table_data )->type;

	byte_stream_copy_to_uint32_little_endian(
	 ( (pff_table_t *) table_data )->value_reference,
	 table_value_reference );

#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		libcnotify_printf(
		 "%s: table signature\t\t\t\t: 0x%02" PRIx8 "\n",
		 function,
		 table_signature );

		libcnotify_printf(
		 "%s: table type\t\t\t\t\t: 0x%02" PRIx8 "\n",
		 function,
		 table_type );

		libcnotify_printf(
		 "%s: table value reference\t\t\t: 0x%08" PRIx32 " ",
		 function,
		 table_value_reference );
		libpff_debug_print_node_identifier_type(
		 (uint8_t) ( table_value_reference & 0x0000001fUL ) );
		libcnotify_printf(
		 "\n" );
	}
#endif
	if( table_signature != 0xec )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
		 "%s: unsupported table signature: 0x%02" PRIx8 ".",
		 function,
		 table_signature );

		return( -1 );
	}
	if( ( table_type != 0x6c )
	 && ( table_type != 0x7c )
	 && ( table_type != 0x8c )
	 && ( table_type != 0x9c )
	 && ( table_type != 0xa5 )
	 && ( table_type != 0xac )
	 && ( table_type != 0xbc ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
		 "%s: unsupported table type: 0x%02" PRIx8 ".",
		 function,
		 table_type );

		return( -1 );
	}
	if( libpff_table_read_index(
	     table,
	     file_io_handle,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_READ_FAILED,
		 "%s: unable to read table index.",
		 function );

		return( -1 );
	}
#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		if( libpff_array_get_number_of_entries(
		     table->index,
		     &number_of_table_index_array_entries,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve number of table index array entries.",
			 function );

			return( -1 );
		}
		for( table_index_array_iterator = 0;
		     table_index_array_iterator < number_of_table_index_array_entries;
		     table_index_array_iterator++ )
		{
			libcnotify_printf(
			 "%s: table index array entry: %d\n",
			 function,
			 table_index_array_iterator );

			if( libpff_array_get_entry_by_index(
			     table->index,
			     table_index_array_iterator,
			     (intptr_t **) &table_index_values,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
				 "%s: unable to retrieve table index array entry: %d.",
				 function,
				 table_index_array_iterator );

				return( -1 );
			}
			if( table_index_values == NULL )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
				 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
				 "%s: invalid table index values.",
				 function );

				return( -1 );
			}
			if( table_index_values->value == NULL )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
				 "%s: invalid table index values - missing values.",
				 function );

				return( -1 );
			}
			for( table_index_values_iterator = 0;
			     table_index_values_iterator < table_index_values->number_of_values;
			     table_index_values_iterator++ )
			{
				if( libfdata_block_get_segment_data(
				     table->descriptor_data_block,
				     file_io_handle,
				     table->descriptor_data_cache,
				     (int) ( table_index_values->value[ table_index_values_iterator ] ).array_entry,
				     &table_data,
				     &table_data_size,
				     0,
				     error ) != 1 )
				{
					libcerror_error_set(
					 error,
					 LIBCERROR_ERROR_DOMAIN_RUNTIME,
					 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
					 "%s: unable to retrieve table array entry: %" PRIu16 ".",
					 function,
					 ( table_index_values->value[ table_index_values_iterator ] ).array_entry );

					return( -1 );
				}
				if( table_data == NULL )
				{
					libcerror_error_set(
					 error,
					 LIBCERROR_ERROR_DOMAIN_RUNTIME,
					 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
					 "%s: missing table array entry: %" PRIu16 ".",
					 function,
					 ( table_index_values->value[ table_index_values_iterator ] ).array_entry );

					return( -1 );
				}
				libcnotify_printf(
				 "%s: table value: %d at offset: %" PRIu16 " of size: %" PRIu16 "\n",
				 function,
				 table_index_values_iterator,
				 ( table_index_values->value[ table_index_values_iterator ] ).offset,
				 ( table_index_values->value[ table_index_values_iterator ] ).size );
				libcnotify_print_data(
				 &( table_data[ ( table_index_values->value[ table_index_values_iterator ] ).offset ] ),
				 (size_t) ( table_index_values->value[ table_index_values_iterator ] ).size,
				 0 );
			}
		}
		libcnotify_printf(
		 "\n" );
	}
#endif
	if( libpff_table_read_values(
	     table,
	     table_type,
	     table_value_reference,
	     io_handle,
	     name_to_id_map_list,
	     file_io_handle,
	     debug_item_type,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_READ_FAILED,
		 "%s: unable to read table values.",
		 function );

		return( -1 );
	}
#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		libcnotify_printf(
		 "\n" );
	}
#endif
	return( 1 );
}

/* Reads the table index
 * Returns 1 if successful or -1 on error
 */
int libpff_table_read_index(
     libpff_table_t *table,
     libbfio_handle_t *file_io_handle,
     libcerror_error_t **error )
{
	libpff_table_index_values_t *table_index_values = NULL;
	uint8_t *table_data                             = NULL;
	uint8_t *table_index_data                       = NULL;
	static char *function                           = "libpff_table_read_index";
	size_t table_data_size                          = 0;
	ssize_t table_data_offset                       = 0;
	uint16_t table_index_offset                     = 0;
	uint16_t table_value_start_offset               = 0;
	uint16_t table_value_end_offset                 = 0;
	uint16_t table_number_of_index_offsets          = 0;
	uint16_t table_number_of_unused_index_offsets   = 0;
	uint16_t table_index_iterator                   = 0;
	int number_of_table_array_entries               = 0;
	int table_array_entry_iterator                  = 0;

	if( table == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid table.",
		 function );

		return( -1 );
	}
	if( libfdata_block_get_number_of_segments(
	     table->descriptor_data_block,
	     &number_of_table_array_entries,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve number of table array entries.",
		 function );

		goto on_error;
	}
	if( libpff_array_resize(
	     table->index,
	     number_of_table_array_entries,
	     (int (*)(intptr_t **, libcerror_error_t **)) &libpff_table_index_values_free,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_RESIZE_FAILED,
		 "%s: unable to resize table index array.",
		 function );

		goto on_error;
	}
	/* Iterate the table array to fill the table index array
	 */
	for( table_array_entry_iterator = 0;
	     table_array_entry_iterator < number_of_table_array_entries;
	     table_array_entry_iterator++ )
	{
		if( libfdata_block_get_segment_data(
		     table->descriptor_data_block,
		     file_io_handle,
		     table->descriptor_data_cache,
		     table_array_entry_iterator,
		     &table_data,
		     &table_data_size,
		     0,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve table array entry: %d.",
			 function,
			 table_array_entry_iterator );

			goto on_error;
		}
		if( table_data == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
			 "%s: missing table array entry: %d.",
			 function,
			 table_array_entry_iterator );

			goto on_error;
		}
		byte_stream_copy_to_uint16_little_endian(
		 table_data,
		 table_index_offset );

#if defined( HAVE_DEBUG_OUTPUT )
		if( libcnotify_verbose != 0 )
		{
			libcnotify_printf(
			 "%s: table data offset\t\t\t: %" PRIzd "\n",
			 function,
			 table_data_offset );

			libcnotify_printf(
			 "%s: table size\t\t\t\t: %" PRIzd "\n",
			 function,
			 table_data_size );

			libcnotify_printf(
			 "%s: table index offset\t\t\t: %" PRIu16 "\n",
			 function,
			 table_index_offset );
		}
#endif
		if( table_index_offset == 0 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
			 "%s: invalid table index offset.",
			 function );

			goto on_error;
		}
		/* The table index offset should point to an offset within the table
		 */
		if( (size_t) table_index_offset >= table_data_size )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
			 "%s: table index offset exceeds table data size.",
			 function );

			goto on_error;
		}
		/* Determine which values are in the table using the index
		 */
		table_index_data = &( table_data[ table_index_offset ] );

#if defined( HAVE_DEBUG_OUTPUT )
		if( libcnotify_verbose != 0 )
		{
			libcnotify_printf(
			 "%s: table index:\n",
			 function );
			libcnotify_print_data(
			 table_index_data,
			 4,
			 0 );
		}
#endif
		byte_stream_copy_to_uint16_little_endian(
		 ( (pff_table_index_t *) table_index_data )->number_of_offsets,
		 table_number_of_index_offsets );
		byte_stream_copy_to_uint16_little_endian(
		 ( (pff_table_index_t *) table_index_data )->number_of_unused_offsets,
		 table_number_of_unused_index_offsets );

		table_index_data += 4;

#if defined( HAVE_DEBUG_OUTPUT )
		if( libcnotify_verbose != 0 )
		{
			libcnotify_printf(
			 "%s: table number of index offsets\t\t: %" PRIu16 "\n",
			 function,
			 table_number_of_index_offsets );

			libcnotify_printf(
			 "%s: table number of unused index offsets\t: %" PRIu16 "\n",
			 function,
			 table_number_of_unused_index_offsets );
		}
#endif
		if( (size_t) ( table_index_offset + 4 + ( ( table_number_of_index_offsets + 1 ) * 2 ) ) > table_data_size )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
			 "%s: number of index offsets do not fit in table.",
			 function );

			goto on_error;
		}
		/* Create the table index values structure
		 * The table number of index items should be considered more of a last item number
		 * The table actually contains 1 additional table index value
		 */
		if( libpff_table_index_values_initialize(
		     &table_index_values,
		     table_number_of_index_offsets,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
			 "%s: unable to create table index values.",
			 function );

			goto on_error;
		}
		if( table_number_of_index_offsets > 0 )
		{
#if defined( HAVE_DEBUG_OUTPUT )
			if( libcnotify_verbose != 0 )
			{
				libcnotify_printf(
				 "\n" );

				libcnotify_printf(
				 "%s: table index offsets:\n",
				 function );
				libcnotify_print_data(
				 table_index_data,
				 ( table_number_of_index_offsets + 1 ) * 2,
				 0 );
			}
#endif
			/* Fill the table index values
			 */
			byte_stream_copy_to_uint16_little_endian(
			 table_index_data,
			 table_value_start_offset );

			table_index_data += 2;

			for( table_index_iterator = 0;
			     table_index_iterator < table_number_of_index_offsets;
			     table_index_iterator++ )
			{
				byte_stream_copy_to_uint16_little_endian(
				 table_index_data,
				 table_value_end_offset );

				table_index_data += 2;

#if defined( HAVE_DEBUG_OUTPUT )
				if( libcnotify_verbose != 0 )
				{
					libcnotify_printf(
					 "%s: table index value: %03" PRIu16 " offset\t\t: %" PRIu16 " - %" PRIu16 "\n",
					 function,
					 table_index_iterator,
					 table_value_start_offset,
					 table_value_end_offset );
				}
#endif
				if( table_value_start_offset > table_value_end_offset )
				{
					libcerror_error_set(
					 error,
					 LIBCERROR_ERROR_DOMAIN_RUNTIME,
					 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
					 "%s: table index start offset: %" PRIu16 " exceeds end offset: %" PRIu16 ".",
					 function,
					 table_value_start_offset,
					 table_value_end_offset );

					goto on_error;
				}
				/* TODO add function to set index values */
				( table_index_values->value[ table_index_iterator ] ).array_entry = (uint32_t) table_array_entry_iterator;
				( table_index_values->value[ table_index_iterator ] ).offset      = table_value_start_offset;
				( table_index_values->value[ table_index_iterator ] ).size        = table_value_end_offset - table_value_start_offset;

				table_value_start_offset = table_value_end_offset;
			}
			if( table_value_end_offset > table_index_offset )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
				 "%s: last table index value end offset: %" PRIu16 " exceeds table index offset: %" PRIu16 ".",
				 function,
				 table_value_end_offset,
				 table_index_offset );

				goto on_error;
			}
		}
#if defined( HAVE_DEBUG_OUTPUT )
		if( libcnotify_verbose != 0 )
		{
			if( table_value_end_offset < table_index_offset )
			{
				libcnotify_printf(
				 "%s: last table index values end offset: %" PRIu16 " does not match table index offset: %" PRIu16 "\n",
				 function,
				 table_value_start_offset,
				 table_index_offset );

				libcnotify_print_data(
				 &( table_data[ table_value_end_offset ] ),
				 ( table_index_offset - table_value_end_offset ),
				 0 );
			}
			if( (size_t) ( table_index_data - table_data ) != table_data_size )
			{
				libcnotify_printf(
				 "\n" );
				libcnotify_printf(
				 "%s: data after table index of size: %" PRIzd "\n",
				 function,
				 table_data_size - (size_t) ( table_index_data - table_data ) );
				libcnotify_print_data(
				 table_index_data,
				 table_data_size - (size_t) ( table_index_data - table_data ),
				 0 );
			}
		}
#endif
		table_data_offset += table_data_size;

		if( libpff_array_set_entry_by_index(
		     table->index,
		     table_array_entry_iterator,
		     (intptr_t *) table_index_values,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
			 "%s: unable to set table index array entry: %d.",
			 function,
			 table_array_entry_iterator );

			goto on_error;
		}
		table_index_values = NULL;
	}
#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		libcnotify_printf(
		 "\n" );
	}
#endif
	return( 1 );

on_error:
	if( table_index_values != NULL )
	{
		libpff_table_index_values_free(
		 &table_index_values,
		 NULL );
	}
	return( -1 );
}

/* Reads the record entries data references
 * Returns 1 if successful or -1 on error
 */
int libpff_table_read_record_entries_data_references(
     libpff_table_t *table,
     uint8_t record_entries_level,
     uint8_t record_entry_identifier_size,
     uint32_t record_entries_reference,
     libbfio_handle_t *file_io_handle,
     libcerror_error_t **error )
{
	libfdata_reference_t *record_entries_data_reference = NULL;
	libpff_table_index_value_t *table_index_value       = NULL;
	uint8_t *record_entries_data                        = NULL;
	uint8_t *table_data                                 = NULL;
	static char *function                               = "libpff_table_read_record_entries_data_references";
	size_t number_of_record_entries                     = 0;
	size_t record_entries_data_size                     = 0;
	size_t record_entry_size                            = 0;
	size_t table_data_size                              = 0;
	int record_entry_index                              = 0;

#if defined( HAVE_DEBUG_OUTPUT )
	libcstring_system_character_t guid_string[ 48 ];

	libfguid_identifier_t *guid                         = NULL;
	uint64_t record_entry_identifier                    = 0;
	int result                                          = 0;
#endif

	if( table == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid table.",
		 function );

		return( -1 );
	}
	if( table->record_entries_data_references == NULL )
	{
		if( libpff_array_initialize(
		     &( table->record_entries_data_references ),
		     0,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
			 "%s: unable to create record entries data references array.",
			 function );

			goto on_error;
		}
	}
	if( ( record_entry_identifier_size != 2 )
	 && ( record_entry_identifier_size != 4 )
	 && ( record_entry_identifier_size != 8 )
	 && ( record_entry_identifier_size != 16 ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
		 "%s: unsupported record entry identifier size: %" PRIu8 ".",
		 function,
		 record_entry_identifier_size );

		goto on_error;
	}
#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		libcnotify_printf(
		 "%s: reading record entries at level: %" PRIu8 " with reference: 0x%08" PRIx32 " ",
		 function,
		 record_entries_level,
		 record_entries_reference );
		libpff_debug_print_node_identifier_type(
		 (uint8_t) ( record_entries_reference & 0x0000001fUL ) );
		libcnotify_printf(
		 "\n" );
		libcnotify_printf(
		 "\n" );
	}
#endif
	if( record_entries_reference == 0 )
	{
#if defined( HAVE_DEBUG_OUTPUT )
		if( libcnotify_verbose != 0 )
		{
			libcnotify_printf(
			 "%s: table contains no record entries.\n",
			 function );
		}
#endif
		return( 1 );
	}
	if( ( record_entries_reference & 0x0000001fUL ) != 0 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_UNSUPPORTED_VALUE,
		 "%s: unsupported record entries reference: 0x%08" PRIx32 " (0x%08" PRIx32 ").",
		 function,
		 record_entries_reference & 0x0000001fUL,
		 record_entries_reference );

		goto on_error;
	}
	/* Retrieve the index value of the record entries reference
	 */
	if( libpff_table_get_index_value_by_reference(
	     table,
	     record_entries_reference,
	     &table_index_value,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve record entries reference.",
		 function );

		goto on_error;
	}
	if( table_index_value == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: missing table index value.",
		 function );

		goto on_error;
	}
	/* Retrieve the corresponding table array entry
	 */
	if( libfdata_block_get_segment_data(
	     table->descriptor_data_block,
	     file_io_handle,
	     table->descriptor_data_cache,
	     (int) table_index_value->array_entry,
	     &table_data,
	     &table_data_size,
	     0,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve table array entry: %" PRIu32 ".",
		 function,
		 table_index_value->array_entry );

		goto on_error;
	}
	if( table_data == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: missing table array entry: %" PRIu32 ".",
		 function,
		 table_index_value->array_entry );

		goto on_error;
	}
	if( (size_t) table_index_value->offset >= table_data_size )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: table value offset exceeds table data size.",
		 function );

		goto on_error;
	}
	/* Create a buffer data reference of the record entries data
	 */
	if( libfdata_buffer_reference_initialize(
	     &record_entries_data_reference,
	     &( table_data[ table_index_value->offset ] ),
	     table_index_value->size,
	     LIBFDATA_BUFFER_DATA_FLAG_NON_MANAGED,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create record entries data reference.",
		 function );

		goto on_error;
	}
	if( record_entries_level == 0 )
	{
		if( libpff_array_append_entry(
		     table->record_entries_data_references,
		     &record_entry_index,
		     (intptr_t *) record_entries_data_reference,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
			 "%s: unable to append record entries data reference.",
			 function );

			goto on_error;
		}
	}
	else
	{
		record_entry_size = record_entry_identifier_size + 4;

		if( ( table_index_value->size % record_entry_size ) != 0 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
			 "%s: unsupported record entries size.",
			 function );

			goto on_error;
		}
		number_of_record_entries = table_index_value->size / record_entry_size;

		for( record_entry_index = 0;
		     (size_t) record_entry_index < number_of_record_entries;
		     record_entry_index++ )
		{
			/* The buffer data reference does not require a cache
			 */
			if( libfdata_reference_get_segment_data_at_offset(
			     record_entries_data_reference,
			     file_io_handle,
			     NULL,
			     (off64_t) ( record_entry_index * record_entry_size ),
			     &record_entries_data,
			     &record_entries_data_size,
			     0,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
				 "%s: unable to retrieve record entries data at offset: %" PRIzd ".",
				 function,
				 record_entry_index * record_entry_size );

				goto on_error;
			}
			if( record_entries_data == NULL )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
				 "%s: missing record entries data.",
				 function );

				goto on_error;
			}
			if( record_entries_data_size < record_entry_size )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
				 "%s: invalid record entries data size value out of bounds.",
				 function );

				goto on_error;
			}
#if defined( HAVE_DEBUG_OUTPUT )
			if( libcnotify_verbose != 0 )
			{
				if( ( record_entry_identifier_size == 2 )
				 || ( record_entry_identifier_size == 4 )
				 || ( record_entry_identifier_size == 8 ) )
				{
					if( record_entry_identifier_size == 2 )
					{
						byte_stream_copy_to_uint16_little_endian(
						 record_entries_data,
						 record_entry_identifier );
					}
					else if( record_entry_identifier_size == 4 )
					{
						byte_stream_copy_to_uint32_little_endian(
						 record_entries_data,
						 record_entry_identifier );
					}
					else if( record_entry_identifier_size == 8 )
					{
						byte_stream_copy_to_uint64_little_endian(
						 record_entries_data,
						 record_entry_identifier );
					}
					libcnotify_printf(
					 "%s: record entry: %03d at level: %" PRIu8 " identifier\t\t\t: 0x%08" PRIx64 "\n",
					 function,
					 record_entry_index,
					 record_entries_level,
					 record_entry_identifier );
				}
				else if( record_entry_identifier_size == 16 )
				{
					if( libfguid_identifier_initialize(
					     &guid,
					     error ) != 1 )
					{
						libcerror_error_set(
						 error,
						 LIBCERROR_ERROR_DOMAIN_RUNTIME,
						 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
						 "%s: unable to create GUID.",
						 function );

						goto on_error;
					}
					if( libfguid_identifier_copy_from_byte_stream(
					     guid,
					     record_entries_data,
					     16,
					     LIBFGUID_ENDIAN_LITTLE,
					     error ) != 1 )
					{
						libcerror_error_set(
						 error,
						 LIBCERROR_ERROR_DOMAIN_RUNTIME,
						 LIBCERROR_RUNTIME_ERROR_COPY_FAILED,
						 "%s: unable to copy byte stream to GUID.",
						 function );

						goto on_error;
					}
#if defined( LIBCSTRING_HAVE_WIDE_SYSTEM_CHARACTER )
					result = libfguid_identifier_copy_to_utf16_string(
						  guid,
						  (uint16_t *) guid_string,
						  48,
						  LIBFGUID_STRING_FORMAT_USE_LOWER_CASE,
						  error );
#else
					result = libfguid_identifier_copy_to_utf8_string(
						  guid,
						  (uint8_t *) guid_string,
						  48,
						  LIBFGUID_STRING_FORMAT_USE_LOWER_CASE,
						  error );
#endif
					if( result != 1 )
					{
						libcerror_error_set(
						 error,
						 LIBCERROR_ERROR_DOMAIN_RUNTIME,
						 LIBCERROR_RUNTIME_ERROR_COPY_FAILED,
						 "%s: unable to copy GUID to string.",
						 function );

						goto on_error;
					}
					if( libfguid_identifier_free(
					     &guid,
					     error ) != 1 )
					{
						libcerror_error_set(
						 error,
						 LIBCERROR_ERROR_DOMAIN_RUNTIME,
						 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
						 "%s: unable to free GUID.",
						 function );

						goto on_error;
					}
					libcnotify_printf(
					 "%s: record entry: %03d at level: %" PRIu8 " identifier\t\t\t: %" PRIs_LIBCSTRING_SYSTEM "s\n",
					 function,
					 record_entry_index,
					 record_entries_level,
					 guid_string );
				}
			}
#endif
			/* TODO use the record entry identifier to validate sub level record entries
			 */
			record_entries_data += record_entry_identifier_size;

			byte_stream_copy_to_uint32_little_endian(
			 record_entries_data,
			 record_entries_reference );

			record_entries_data += 4;

#if defined( HAVE_DEBUG_OUTPUT )
			if( libcnotify_verbose != 0 )
			{
				libcnotify_printf(
				 "%s: record entry: %03d at level: %" PRIu8 " reference\t\t\t: 0x%08" PRIx32 " ",
				 function,
				 record_entry_index,
				 record_entries_level,
				 record_entries_reference );
				libpff_debug_print_node_identifier_type(
				 (uint8_t) ( record_entries_reference & 0x0000001fUL ) );
				libcnotify_printf(
				 "\n" );
				libcnotify_printf(
				 "\n" );
			}
#endif
			if( libpff_table_read_record_entries_data_references(
			     table,
			     record_entries_level - 1,
			     record_entry_identifier_size,
			     record_entries_reference,
			     file_io_handle,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_IO,
				 LIBCERROR_IO_ERROR_READ_FAILED,
				 "%s: unable to read record entries data references.",
				 function );

				goto on_error;
			}
		}
#if defined( HAVE_DEBUG_OUTPUT )
		if( libcnotify_verbose != 0 )
		{
			libcnotify_printf(
			 "\n" );
		}
#endif
		if( libfdata_reference_free(
		     &record_entries_data_reference,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
			 "%s: unable to free record entries data reference.",
			 function );

			goto on_error;
		}
	}
	return( 1 );

on_error:
#if defined( HAVE_DEBUG_OUTPUT )
	if( guid != NULL )
	{
		libfguid_identifier_free(
		 &guid,
		 NULL );
	}
#endif
	if( record_entries_data_reference != NULL )
	{
		libfdata_reference_free(
		 &record_entries_data_reference,
		 NULL );
	}
	if( table->record_entries_data_references != NULL )
	{
		libpff_array_free(
		 &( table->record_entries_data_references ),
		 (int (*)(intptr_t **, libcerror_error_t **)) &libfdata_reference_free,
		 NULL );
	}
	return( -1 );
}

/* Reads the table values
 * Returns 1 if successful or -1 on error
 */
int libpff_table_read_values(
     libpff_table_t *table,
     uint8_t table_type,
     uint32_t table_value_reference,
     libpff_io_handle_t *io_handle,
     libpff_list_t *name_to_id_map_list,
     libbfio_handle_t *file_io_handle,
     int debug_item_type,
     libcerror_error_t **error )
{
	static char *function = "libpff_table_read_values";
	int result            = 0;

	switch( table_type )
	{
		case 0x6c:
			result = libpff_table_read_6c_values(
				  table,
				  table_value_reference,
				  file_io_handle,
				  error );
			break;

		case 0x7c:
			result = libpff_table_read_7c_values(
				  table,
				  table_value_reference,
				  io_handle,
				  name_to_id_map_list,
				  file_io_handle,
				  error );
			break;

		case 0x8c:
			result = libpff_table_read_8c_values(
				  table,
				  table_value_reference,
				  file_io_handle,
				  error );
			break;

		case 0x9c:
			result = libpff_table_read_9c_values(
				  table,
				  table_value_reference,
				  file_io_handle,
				  error );
			break;

		case 0xa5:
			result = libpff_table_read_a5_values(
				  table,
				  table_value_reference,
				  file_io_handle,
				  error );
			break;

		case 0xac:
			result = libpff_table_read_ac_values(
				  table,
				  table_value_reference,
				  io_handle,
				  name_to_id_map_list,
				  file_io_handle,
				  error );
			break;

		case 0xbc:
			result = libpff_table_read_bc_values(
				  table,
				  table_value_reference,
				  io_handle,
				  name_to_id_map_list,
				  file_io_handle,
				  debug_item_type,
				  error );
			break;
	}
	if( result != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_READ_FAILED,
		 "%s: unable to read table values.",
		 function );

		return( -1 );
	}
	return( 1 );
}

/* Reads the 6c table values
 * Returns 1 if successful or -1 on error
 */
int libpff_table_read_6c_values(
     libpff_table_t *table,
     uint32_t table_header_reference,
     libbfio_handle_t *file_io_handle,
     libcerror_error_t **error )
{
	libpff_table_index_value_t *table_index_value = NULL;
	uint8_t *table_data                           = NULL;
	uint8_t *table_value_data                     = NULL;
	static char *function                         = "libpff_table_read_6c_values";
	size_t table_data_size                        = 0;
	uint32_t b5_table_header_reference            = 0;
	uint32_t record_entries_reference             = 0;
	uint32_t values_array_reference               = 0;
	uint8_t record_entry_identifier_size          = 0;
	uint8_t record_entry_value_size               = 0;
	uint8_t record_entries_level                  = 0;

	if( ( table_header_reference & 0x0000001fUL ) != 0 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_UNSUPPORTED_VALUE,
		 "%s: unsupported table header reference: 0x%08" PRIx32 " (0x%08" PRIx32 ").",
		 function,
		 table_header_reference & 0x0000001fUL,
		 table_header_reference );

		return( -1 );
	}
	/* Fetch the 6c table header reference
	 */
	if( libpff_table_get_index_value_by_reference(
	     table,
	     table_header_reference,
	     &table_index_value,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve 6c table header reference.",
		 function );

		return( -1 );
	}
	if( table_index_value == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: missing table index value.",
		 function );

		return( -1 );
	}
	/* Retrieve the corresponding table array entry
	 */
	if( libfdata_block_get_segment_data(
	     table->descriptor_data_block,
	     file_io_handle,
	     table->descriptor_data_cache,
	     (int) table_index_value->array_entry,
	     &table_data,
	     &table_data_size,
	     0,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve table array entry: %" PRIu32 ".",
		 function,
		 table_index_value->array_entry );

		return( -1 );
	}
	if( table_data == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: missing table array entry: %" PRIu32 ".",
		 function,
		 table_index_value->array_entry );

		return( -1 );
	}
	if( (size_t) table_index_value->offset >= table_data_size )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: table value offset exceeds table data size.",
		 function );

		return( -1 );
	}
	table_value_data = &( table_data[ table_index_value->offset ] );

	/* Read the 6c table header
	 * This table header contains no type indicator
	 * to make sure the it is supported the size is checked
	 */
	if( table_index_value->size != 8 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: unsupported 6c table header of size: %" PRIu16 ".",
		 function,
		 table_index_value->size );

		return( -1 );
	}
	byte_stream_copy_to_uint32_little_endian(
	 table_value_data,
	 b5_table_header_reference );

	table_value_data += 4;

	byte_stream_copy_to_uint32_little_endian(
	 table_value_data,
	 values_array_reference );

	table_value_data += 4;

#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		libcnotify_printf(
		 "%s: b5 table header reference\t\t: 0x%08" PRIx32 " ",
		 function,
		 b5_table_header_reference );
		libpff_debug_print_node_identifier_type(
		 (uint8_t) ( b5_table_header_reference & 0x0000001fUL ) );
		libcnotify_printf(
		 "\n" );
		libcnotify_printf(
		 "%s: values array reference\t\t: 0x%08" PRIx32 " ",
		 function,
		 values_array_reference );
		libpff_debug_print_node_identifier_type(
		 (uint8_t) ( values_array_reference & 0x0000001fUL ) );
		libcnotify_printf(
		 "\n" );
	}
#endif
	/* Read the b5 table header
	 */
	if( libpff_table_read_b5_header(
	     table,
	     b5_table_header_reference,
	     &record_entry_identifier_size,
	     &record_entry_value_size,
	     &record_entries_level,
	     &record_entries_reference,
	     file_io_handle,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_READ_FAILED,
		 "%s: unable to read b5 table header.",
		 function );

		return( -1 );
	}
	if( ( record_entry_identifier_size != 16 )
	 || ( record_entry_value_size != 2 ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
		 "%s: unsupported record entry identifier size: %" PRIu8 " and record entry value size: %" PRIu8 ".",
		 function,
		 record_entry_identifier_size,
		 record_entry_value_size );

		return( -1 );
	}
	/* Check if the table contains any entries
	 */
	if( ( record_entries_reference == 0 )
	 && ( values_array_reference == 0 ) )
	{
#if defined( HAVE_DEBUG_OUTPUT )
		if( libcnotify_verbose != 0 )
		{
			libcnotify_printf(
			 "%s: table contains no entries.\n",
			 function );
		}
#endif

		return( 1 );
	}
	if( record_entries_reference == 0 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: table contains value array but no record entries.",
		 function );

		return( -1 );
	}
	if( values_array_reference == 0 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: table contains record entries but no value array.",
		 function );

		return( -1 );
	}
	if( libpff_table_read_record_entries_data_references(
	     table,
	     record_entries_level,
	     record_entry_identifier_size,
	     record_entries_reference,
	     file_io_handle,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_READ_FAILED,
		 "%s: unable to read record entries data references.",
		 function );

		return( -1 );
	}
	if( libpff_table_read_6c_record_entries(
	     table,
	     values_array_reference,
	     file_io_handle,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_READ_FAILED,
		 "%s: unable to read 6c table record entries.",
		 function );

		return( -1 );
	}
	return( 1 );
}

/* Reads the 7c table values
 * Returns 1 if successful or -1 on error
 */
int libpff_table_read_7c_values(
     libpff_table_t *table,
     uint32_t table_header_reference,
     libpff_io_handle_t *io_handle,
     libpff_list_t *name_to_id_map_list,
     libbfio_handle_t *file_io_handle,
     libcerror_error_t **error )
{
	libpff_array_t *column_definitions_array                = NULL;
	libfdata_reference_t *column_definitions_data_reference = NULL;
	libpff_table_index_value_t *table_index_value           = NULL;
	uint8_t *table_data                                     = NULL;
	uint8_t *table_header_data                              = NULL;
	static char *function                                   = "libpff_table_read_7c_values";
	size_t table_data_size                                  = 0;
	size_t table_header_data_size                           = 0;
	uint32_t b5_table_header_reference                      = 0;
	uint32_t values_array_reference                         = 0;
	uint32_t record_entries_reference                       = 0;
	uint16_t values_array_size                              = 0;
	uint8_t record_entry_identifier_size                    = 0;
	uint8_t record_entry_value_size                         = 0;
	uint8_t record_entries_level                            = 0;
	int number_of_column_definitions                        = 0;

#if defined( HAVE_DEBUG_OUTPUT )
	uint16_t value_16bit                                    = 0;
#endif

	if( ( table_header_reference & 0x0000001fUL ) != 0 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_UNSUPPORTED_VALUE,
		 "%s: unsupported table header reference: 0x%08" PRIx32 " (0x%08" PRIx32 ").",
		 function,
		 table_header_reference & 0x0000001fUL,
		 table_header_reference );

		return( -1 );
	}
	/* Fetch the 7c table header reference
	 */
	if( libpff_table_get_index_value_by_reference(
	     table,
	     table_header_reference,
	     &table_index_value,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve 7c table header reference.",
		 function );

		goto on_error;
	}
	if( table_index_value == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: missing table index value.",
		 function );

		goto on_error;
	}
	/* Retrieve the corresponding table array entry
	 */
	if( libfdata_block_get_segment_data(
	     table->descriptor_data_block,
	     file_io_handle,
	     table->descriptor_data_cache,
	     (int) table_index_value->array_entry,
	     &table_data,
	     &table_data_size,
	     0,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve table array entry: %" PRIu32 ".",
		 function,
		 table_index_value->array_entry );

		goto on_error;
	}
	if( table_data == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: missing table array entry: %" PRIu32 ".",
		 function,
		 table_index_value->array_entry );

		goto on_error;
	}
	if( (size_t) table_index_value->offset >= table_data_size )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: table value offset exceeds table data size.",
		 function );

		goto on_error;
	}
	table_header_data      = &( table_data[ table_index_value->offset ] );
	table_header_data_size = (size_t) table_index_value->size;

	/* Read the 7c table header
	 */
	if( ( (pff_table_header_7c_t *) table_header_data )->type != 0x7c )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
		 "%s: unsupported table header type: 0x%02x.",
		 function,
		 ( (pff_table_header_7c_t *) table_header_data )->type );

		goto on_error;
	}
	byte_stream_copy_to_uint16_little_endian(
	 ( (pff_table_header_7c_t *) table_header_data )->values_array_end_offset_cell_existence_block,
	 values_array_size );
	byte_stream_copy_to_uint32_little_endian(
	 ( (pff_table_header_7c_t *) table_header_data )->b5_table_header_reference,
	 b5_table_header_reference );
	byte_stream_copy_to_uint32_little_endian(
	 ( (pff_table_header_7c_t *) table_header_data )->values_array_reference,
	 values_array_reference );

	number_of_column_definitions = (int) ( (pff_table_header_7c_t *) table_header_data )->number_of_column_definitions;

#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		libcnotify_printf(
		 "%s: table header type\t\t\t\t\t: 0x%02" PRIx8 "\n",
		 function,
		 ( (pff_table_header_7c_t *) table_header_data )->type );

		libcnotify_printf(
		 "%s: number of column definitions\t\t\t: %d\n",
		 function,
		 number_of_column_definitions );

		byte_stream_copy_to_uint16_little_endian(
		 ( (pff_table_header_7c_t *) table_header_data )->values_array_end_offset_32bit_values,
		 value_16bit );
		libcnotify_printf(
		 "%s: values array end offset 32-bit values\t\t: %" PRIu16 "\n",
		 function,
		 value_16bit );

		byte_stream_copy_to_uint16_little_endian(
		 ( (pff_table_header_7c_t *) table_header_data )->values_array_end_offset_16bit_values,
		 value_16bit );
		libcnotify_printf(
		 "%s: values array end offset 16-bit values\t\t: %" PRIu16 "\n",
		 function,
		 value_16bit );

		byte_stream_copy_to_uint16_little_endian(
		 ( (pff_table_header_7c_t *) table_header_data )->values_array_end_offset_8bit_values,
		 value_16bit );
		libcnotify_printf(
		 "%s: values array end offset 8-bit values\t\t: %" PRIu16 "\n",
		 function,
		 value_16bit );

		byte_stream_copy_to_uint16_little_endian(
		 ( (pff_table_header_7c_t *) table_header_data )->values_array_end_offset_cell_existence_block,
		 value_16bit );
		libcnotify_printf(
		 "%s: values array end offset cell existence block\t: %" PRIu16 "\n",
		 function,
		 value_16bit );

		libcnotify_printf(
		 "%s: b5 table header reference\t\t\t\t: 0x%08" PRIx32 " ",
		 function,
		 b5_table_header_reference );
		libpff_debug_print_node_identifier_type(
		 (uint8_t) ( b5_table_header_reference & 0x0000001fUL ) );
		libcnotify_printf(
		 "\n" );

		libcnotify_printf(
		 "%s: values array reference\t\t\t\t: 0x%08" PRIx32 " ",
		 function,
		 values_array_reference );
		libpff_debug_print_node_identifier_type(
		 (uint8_t) ( values_array_reference & 0x0000001fUL ) );
		libcnotify_printf(
		 "\n" );

		libcnotify_printf(
		 "%s: unknown1:\n",
		 function );
		libcnotify_print_data(
		 ( (pff_table_header_7c_t *) table_header_data )->unknown1,
		 4,
		 0 );
	}
#endif
	table_header_data      += sizeof( pff_table_header_7c_t );
	table_header_data_size -= sizeof( pff_table_header_7c_t );

	/* Read the column definitions in the 7c table header
	 */
	if( ( (size_t) number_of_column_definitions * sizeof( pff_table_column_definition_7c_t ) ) != table_header_data_size )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
		 "%s: mismatch in number of the column definitions and data size.",
		 function );

		goto on_error;
	}
#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		libcnotify_printf(
		 "%s: 7c column definitions data:\n",
		 function );
		libcnotify_print_data(
		 table_header_data,
		 table_header_data_size,
		 0 );
	}
#endif
	/* Create a buffer data reference of the column definitions data
	 */
	if( libfdata_buffer_reference_initialize(
	     &column_definitions_data_reference,
	     table_header_data,
	     table_header_data_size,
	     LIBFDATA_BUFFER_DATA_FLAG_NON_MANAGED,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create column definitions data reference.",
		 function );

		goto on_error;
	}
	/* Read the b5 table header
	 */
	if( libpff_table_read_b5_header(
	     table,
	     b5_table_header_reference,
	     &record_entry_identifier_size,
	     &record_entry_value_size,
	     &record_entries_level,
	     &record_entries_reference,
	     file_io_handle,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_READ_FAILED,
		 "%s: unable to read b5 table header.",
		 function );

		goto on_error;
	}
	if( ( record_entry_identifier_size != 4 )
	 || ( ( record_entry_value_size != 2 )
	  &&  ( record_entry_value_size != 4 ) ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
		 "%s: unsupported record entry identifier size: 0x%02" PRIx8 " and record entry value size: 0x%02" PRIx8 ".",
		 function,
		 record_entry_identifier_size,
		 record_entry_value_size );

		goto on_error;
	}
	/* Create the column definitions array
	 */
	if( libpff_array_initialize(
	     &column_definitions_array,
	     0,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create column definitions array.",
		 function );

		goto on_error;
	}
	if( libpff_table_read_7c_column_definitions(
	     column_definitions_array,
	     column_definitions_data_reference,
	     number_of_column_definitions,
	     file_io_handle,
	     name_to_id_map_list,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_READ_FAILED,
		 "%s: unable to read 7c table column definitions.",
		 function );

		goto on_error;
	}
	if( libpff_table_read_record_entries_data_references(
	     table,
	     record_entries_level,
	     record_entry_identifier_size,
	     record_entries_reference,
	     file_io_handle,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_READ_FAILED,
		 "%s: unable to read record entries data references.",
		 function );

		goto on_error;
	}
	if( number_of_column_definitions > 0 )
	{
		if( libpff_table_read_values_array(
		     table,
		     record_entries_reference,
		     values_array_reference,
		     record_entry_identifier_size,
		     record_entry_value_size,
		     values_array_size,
		     column_definitions_array,
		     io_handle,
		     file_io_handle,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_IO,
			 LIBCERROR_IO_ERROR_READ_FAILED,
			 "%s: unable to read values array.",
			 function );

			goto on_error;
		}
	}
	if( libpff_array_free(
	     &column_definitions_array,
	     (int (*)(intptr_t **, libcerror_error_t **)) &libpff_column_definition_free,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
		 "%s: unable to free the column definitions array.",
		 function );

		goto on_error;
	}
	if( libfdata_reference_free(
	     &column_definitions_data_reference,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
		 "%s: unable to free column definitions data reference.",
		 function );

		goto on_error;
	}
	return( 1 );

on_error:
	if( column_definitions_array != NULL )
	{
		libpff_array_free(
		 &column_definitions_array,
		 (int (*)(intptr_t **, libcerror_error_t **)) &libpff_column_definition_free,
		 NULL );
	}
	if( column_definitions_data_reference != NULL )
	{
		libfdata_reference_free(
		 &column_definitions_data_reference,
		 NULL );
	}
	return( -1 );
}

/* Reads the 8c table values
 * Returns 1 if successful or -1 on error
 */
int libpff_table_read_8c_values(
     libpff_table_t *table,
     uint32_t b5_table_header_reference,
     libbfio_handle_t *file_io_handle,
     libcerror_error_t **error )
{
	static char *function                = "libpff_table_read_8c_values";
	uint32_t record_entries_reference    = 0;
	uint8_t record_entry_identifier_size = 0;
	uint8_t record_entry_value_size      = 0;
	uint8_t record_entries_level         = 0;

	/* Read the b5 table header
	 */
	if( libpff_table_read_b5_header(
	     table,
	     b5_table_header_reference,
	     &record_entry_identifier_size,
	     &record_entry_value_size,
	     &record_entries_level,
	     &record_entries_reference,
	     file_io_handle,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_READ_FAILED,
		 "%s: unable to read b5 table header.",
		 function );

		return( -1 );
	}
	if( ( record_entry_identifier_size != 8 )
	 || ( record_entry_value_size != 4 ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
		 "%s: unsupported record entry identifier size: 0x%02" PRIx8 " and record entry value size: 0x%02" PRIx8 ".",
		 function,
		 record_entry_identifier_size,
		 record_entry_value_size );

		return( -1 );
	}
	if( libpff_table_read_record_entries_data_references(
	     table,
	     record_entries_level,
	     record_entry_identifier_size,
	     record_entries_reference,
	     file_io_handle,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_READ_FAILED,
		 "%s: unable to read record entries data references.",
		 function );

		return( -1 );
	}
	if( libpff_table_read_8c_record_entries(
	     table,
	     file_io_handle,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_READ_FAILED,
		 "%s: unable to read table record entries.",
		 function );

		return( -1 );
	}
	return( 1 );
}

/* Reads the 9c table values
 * Returns 1 if successful or -1 on error
 */
int libpff_table_read_9c_values(
     libpff_table_t *table,
     uint32_t table_header_reference,
     libbfio_handle_t *file_io_handle,
     libcerror_error_t **error )
{
	libpff_table_index_value_t *table_index_value = NULL;
	uint8_t *table_data                           = NULL;
	uint8_t *table_value_data                     = NULL;
	static char *function                         = "libpff_table_read_9c_values";
	size_t table_data_size                        = 0;
	uint32_t b5_table_header_reference            = 0;
	uint32_t record_entries_reference             = 0;
	uint8_t record_entry_identifier_size          = 0;
	uint8_t record_entry_value_size               = 0;
	uint8_t record_entries_level                  = 0;

	if( ( table_header_reference & 0x0000001fUL ) != 0 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_UNSUPPORTED_VALUE,
		 "%s: unsupported table header reference: 0x%08" PRIx32 " (0x%08" PRIx32 ").",
		 function,
		 table_header_reference & 0x0000001fUL,
		 table_header_reference );

		return( -1 );
	}
	/* Fetch the 9c table header reference
	 */
	if( libpff_table_get_index_value_by_reference(
	     table,
	     table_header_reference,
	     &table_index_value,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve 9c table header reference.",
		 function );

		return( -1 );
	}
	if( table_index_value == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: missing table index value.",
		 function );

		return( -1 );
	}
	/* Retrieve the corresponding table array entry
	 */
	if( libfdata_block_get_segment_data(
	     table->descriptor_data_block,
	     file_io_handle,
	     table->descriptor_data_cache,
	     (int) table_index_value->array_entry,
	     &table_data,
	     &table_data_size,
	     0,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve table array entry: %" PRIu32 ".",
		 function,
		 table_index_value->array_entry );

		return( -1 );
	}
	if( table_data == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: missing table array entry: %" PRIu32 ".",
		 function,
		 table_index_value->array_entry );

		return( -1 );
	}
	if( (size_t) table_index_value->offset >= table_data_size )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: table value offset exceeds table data size.",
		 function );

		return( -1 );
	}
	table_value_data = &( table_data[ table_index_value->offset ] );

	/* Read the 9c table header
	 * This table header contains no type indicator
	 * to make sure the it is supported the size is checked
	 */
	if( table_index_value->size != 4 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: unsupported 9c table header of size: %" PRIu16 ".",
		 function,
		 table_index_value->size );

		return( -1 );
	}
	byte_stream_copy_to_uint32_little_endian(
	 table_value_data,
	 b5_table_header_reference );

#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		libcnotify_printf(
		 "%s: b5 table header reference\t\t: 0x%08" PRIx32 " ",
		 function,
		 b5_table_header_reference );
		libpff_debug_print_node_identifier_type(
		 (uint8_t) ( b5_table_header_reference & 0x0000001fUL ) );
		libcnotify_printf(
		 "\n" );
	}
#endif
	/* Read the b5 table header
	 */
	if( libpff_table_read_b5_header(
	     table,
	     b5_table_header_reference,
	     &record_entry_identifier_size,
	     &record_entry_value_size,
	     &record_entries_level,
	     &record_entries_reference,
	     file_io_handle,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_READ_FAILED,
		 "%s: unable to read b5 table header.",
		 function );

		return( -1 );
	}
	if( ( record_entry_identifier_size != 16 )
	 || ( record_entry_value_size != 4 ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
		 "%s: unsupported record entry identifier size: 0x%02" PRIx8 " and record entry value size: 0x%02" PRIx8 ".",
		 function,
		 record_entry_identifier_size,
		 record_entry_value_size );

		return( -1 );
	}
	if( libpff_table_read_record_entries_data_references(
	     table,
	     record_entries_level,
	     record_entry_identifier_size,
	     record_entries_reference,
	     file_io_handle,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_READ_FAILED,
		 "%s: unable to read record entries data references.",
		 function );

		return( -1 );
	}
	if( libpff_table_read_9c_record_entries(
	     table,
	     file_io_handle,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_READ_FAILED,
		 "%s: unable to read 9c table record entries.",
		 function );

		return( -1 );
	}
	return( 1 );
}

/* Reads the a5 table values
 * Returns 1 if successful or -1 on error
 */
int libpff_table_read_a5_values(
     libpff_table_t *table,
     uint32_t table_header_reference,
     libbfio_handle_t *file_io_handle,
     libcerror_error_t **error )
{
	libpff_table_index_values_t *table_index_values = NULL;
	static char *function                           = "libpff_table_read_a5_values";

	/* The a5 table contains no b5 table header
	 */
	if( table_header_reference != 0 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
		 "%s: unsupported table header reference: 0x%08" PRIx32 ".",
		 function,
		 table_header_reference );

		return( -1 );
	}
	if( libpff_array_get_entry_by_index(
	     table->index,
	     0,
	     (intptr_t **) &table_index_values,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve table index array entry: 0.",
		 function );

		return( -1 );
	}
	if( table_index_values == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid table index values.",
		 function );

		return( -1 );
	}
	if( table_index_values->value == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid table index values - missing values.",
		 function );

		return( -1 );
	}
	if( table_index_values->number_of_values > 1 )
	{
		if( libpff_table_read_a5_record_entries(
		     table,
		     0x00000020,
		     file_io_handle,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_IO,
			 LIBCERROR_IO_ERROR_READ_FAILED,
			 "%s: unable to read a5 table record entries.",
			 function );

			return( -1 );
		}
	}
#if defined( HAVE_DEBUG_OUTPUT )
	else if( libcnotify_verbose != 0 )
	{
		libcnotify_printf(
		 "%s: table contains no entries.\n",
		 function );
	}
#endif
	return( 1 );
}

/* Reads the ac table values
 * Returns 1 if successful or -1 on error
 */
int libpff_table_read_ac_values(
     libpff_table_t *table,
     uint32_t table_header_reference,
     libpff_io_handle_t *io_handle,
     libpff_list_t *name_to_id_map_list,
     libbfio_handle_t *file_io_handle,
     libcerror_error_t **error )
{
	libpff_array_t *column_definitions_array      = NULL;
	libpff_table_index_value_t *table_index_value = NULL;
	uint8_t *table_data                           = NULL;
	uint8_t *table_header_data                    = NULL;
	static char *function                         = "libpff_table_read_ac_values";
	size_t table_data_size                        = 0;
	size_t table_header_data_size                 = 0;
	uint32_t b5_table_header_reference            = 0;
	uint32_t record_entries_reference             = 0;
	uint32_t column_definitions_reference         = 0;
	uint32_t values_array_reference               = 0;
	uint16_t values_array_size                    = 0;
	uint8_t record_entry_identifier_size          = 0;
	uint8_t record_entry_value_size               = 0;
	uint8_t record_entries_level                  = 0;
	int number_of_column_definitions              = 0;

#if defined( HAVE_DEBUG_OUTPUT )
	uint16_t value_16bit                          = 0;
#endif

	if( ( table_header_reference & 0x0000001fUL ) != 0 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_UNSUPPORTED_VALUE,
		 "%s: unsupported table header reference: 0x%08" PRIx32 " (0x%08" PRIx32 ").",
		 function,
		 table_header_reference & 0x0000001fUL,
		 table_header_reference );

		return( -1 );
	}
	/* Fetch the ac table header reference
	 */
	if( libpff_table_get_index_value_by_reference(
	     table,
	     table_header_reference,
	     &table_index_value,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve ac table header reference.",
		 function );

		goto on_error;
	}
	if( table_index_value == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: missing table index value.",
		 function );

		goto on_error;
	}
	/* Retrieve the corresponding table array entry
	 */
	if( libfdata_block_get_segment_data(
	     table->descriptor_data_block,
	     file_io_handle,
	     table->descriptor_data_cache,
	     (int) table_index_value->array_entry,
	     &table_data,
	     &table_data_size,
	     0,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve table array entry: %" PRIu32 ".",
		 function,
		 table_index_value->array_entry );

		goto on_error;
	}
	if( table_data == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: missing table array entry: %" PRIu32 ".",
		 function,
		 table_index_value->array_entry );

		goto on_error;
	}
	if( (size_t) table_index_value->offset >= table_data_size )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: table value offset exceeds table data size.",
		 function );

		goto on_error;
	}
	table_header_data      = &( table_data[ table_index_value->offset ] );
	table_header_data_size = (size_t) table_index_value->size;

	/* Read the ac table header
	 */
	if( ( (pff_table_header_ac_t *) table_header_data )->type != 0xac )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
		 "%s: unsupported table header type: 0x%02x.",
		 function,
		 ( (pff_table_header_ac_t *) table_header_data )->type );

		goto on_error;
	}
	/* TODO check ac header size */

	byte_stream_copy_to_uint16_little_endian(
	 ( (pff_table_header_ac_t *) table_header_data )->values_array_end_offset_cell_existence_block,
	 values_array_size );
	byte_stream_copy_to_uint32_little_endian(
	 ( (pff_table_header_ac_t *) table_header_data )->b5_table_header_reference,
	 b5_table_header_reference );
	byte_stream_copy_to_uint32_little_endian(
	 ( (pff_table_header_ac_t *) table_header_data )->values_array_reference,
	 values_array_reference );
	byte_stream_copy_to_uint16_little_endian(
	 ( (pff_table_header_ac_t *) table_header_data )->number_of_column_definitions,
	 number_of_column_definitions );
	byte_stream_copy_to_uint32_little_endian(
	 ( (pff_table_header_ac_t *) table_header_data )->column_definitions_reference,
	 column_definitions_reference );

#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		libcnotify_printf(
		 "%s: table header type\t\t\t\t\t: 0x%02" PRIx8 "\n",
		 function,
		 ( (pff_table_header_ac_t *) table_header_data )->type );

		libcnotify_printf(
		 "%s: padding1\t\t\t\t\t\t: 0x%02" PRIx8 "\n",
		 function,
		 ( (pff_table_header_ac_t *) table_header_data )->padding1 );

		byte_stream_copy_to_uint16_little_endian(
		 ( (pff_table_header_ac_t *) table_header_data )->values_array_end_offset_32bit_values,
		 value_16bit );
		libcnotify_printf(
		 "%s: values array end offset 32-bit values\t: %" PRIu16 "\n",
		 function,
		 value_16bit );

		byte_stream_copy_to_uint16_little_endian(
		 ( (pff_table_header_ac_t *) table_header_data )->values_array_end_offset_16bit_values,
		 value_16bit );
		libcnotify_printf(
		 "%s: values array end offset 16-bit values\t: %" PRIu16 "\n",
		 function,
		 value_16bit );

		byte_stream_copy_to_uint16_little_endian(
		 ( (pff_table_header_ac_t *) table_header_data )->values_array_end_offset_8bit_values,
		 value_16bit );
		libcnotify_printf(
		 "%s: values array end offset 8-bit values\t\t: %" PRIu16 "\n",
		 function,
		 value_16bit );

		byte_stream_copy_to_uint16_little_endian(
		 ( (pff_table_header_ac_t *) table_header_data )->values_array_end_offset_cell_existence_block,
		 value_16bit );
		libcnotify_printf(
		 "%s: values array end offset cell existence block\t: %" PRIu16 "\n",
		 function,
		 value_16bit );

		libcnotify_printf(
		 "%s: b5 table header reference\t\t\t\t: 0x%08" PRIx32 " ",
		 function,
		 b5_table_header_reference );
		libpff_debug_print_node_identifier_type(
		 (uint8_t) ( b5_table_header_reference & 0x0000001fUL ) );
		libcnotify_printf(
		 "\n" );

		libcnotify_printf(
		 "%s: values array reference\t\t\t\t: 0x%08" PRIx32 " ",
		 function,
		 values_array_reference );
		libpff_debug_print_node_identifier_type(
		 (uint8_t) ( values_array_reference & 0x0000001fUL ) );
		libcnotify_printf(
		 "\n" );

		libcnotify_printf(
		 "%s: padding2:\n",
		 function );
		libcnotify_print_data(
		 ( (pff_table_header_ac_t *) table_header_data )->padding2,
		 4,
		 0 );

		libcnotify_printf(
		 "%s: number of column definitions\t\t\t: %d\n",
		 function,
		 number_of_column_definitions );

		libcnotify_printf(
		 "%s: column definitions reference\t\t\t: 0x%08" PRIx32 " ",
		 function,
		 column_definitions_reference );
		libpff_debug_print_node_identifier_type(
		 (uint8_t) ( column_definitions_reference & 0x0000001fUL ) );
		libcnotify_printf(
		 "\n" );

		libcnotify_printf(
		 "%s: unknown2:\n",
		 function );
		libcnotify_print_data(
		 ( (pff_table_header_ac_t *) table_header_data )->unknown2,
		 12,
		 0 );
	}
#endif
	/* Read the b5 table header
	 */
	if( libpff_table_read_b5_header(
	     table,
	     b5_table_header_reference,
	     &record_entry_identifier_size,
	     &record_entry_value_size,
	     &record_entries_level,
	     &record_entries_reference,
	     file_io_handle,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_READ_FAILED,
		 "%s: unable to read b5 table header.",
		 function );

		goto on_error;
	}
	if( ( record_entry_identifier_size != 4 )
	 || ( record_entry_value_size != 4 ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
		 "%s: unsupported record entry identifier size: 0x%02" PRIx8 " and record entry value size: 0x%02" PRIx8 ".",
		 function,
		 record_entry_identifier_size,
		 record_entry_value_size );

		goto on_error;
	}
	/* Create the column definitions array
	 */
	if( libpff_array_initialize(
	     &column_definitions_array,
	     0,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create column definitions array.",
		 function );

		goto on_error;
	}
	if( libpff_table_read_ac_column_definitions(
	     table,
	     column_definitions_array,
	     column_definitions_reference,
	     number_of_column_definitions,
	     io_handle,
	     file_io_handle,
	     name_to_id_map_list,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_READ_FAILED,
		 "%s: unable to read ac table column definitions.",
		 function );

		goto on_error;
	}
	if( libpff_table_read_record_entries_data_references(
	     table,
	     record_entries_level,
	     record_entry_identifier_size,
	     record_entries_reference,
	     file_io_handle,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_READ_FAILED,
		 "%s: unable to read record entries data references.",
		 function );

		goto on_error;
	}
	if( libpff_array_get_number_of_entries(
	     table->index,
	     &number_of_column_definitions,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve number of column definitions.",
		 function );

		goto on_error;
	}
	if( number_of_column_definitions > 0 )
	{
		if( libpff_table_read_values_array(
		     table,
		     record_entries_reference,
		     values_array_reference,
		     record_entry_identifier_size,
		     record_entry_value_size,
		     values_array_size,
		     column_definitions_array,
		     io_handle,
		     file_io_handle,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_IO,
			 LIBCERROR_IO_ERROR_READ_FAILED,
			 "%s: unable to read values array.",
			 function );

			goto on_error;
		}
	}
	if( libpff_array_free(
	     &column_definitions_array,
	     (int (*)(intptr_t **, libcerror_error_t **)) &libpff_column_definition_free,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
		 "%s: unable to free the column definitions array.",
		 function );

		goto on_error;
	}
	return( 1 );

on_error:
	if( column_definitions_array != NULL )
	{
		libpff_array_free(
		 &column_definitions_array,
		 (int (*)(intptr_t **, libcerror_error_t **)) &libpff_column_definition_free,
		 NULL );
	}
	return( -1 );
}

/* Reads the bc table values
 * Returns 1 if successful or -1 on error
 */
int libpff_table_read_bc_values(
     libpff_table_t *table,
     uint32_t b5_table_header_reference,
     libpff_io_handle_t *io_handle,
     libpff_list_t *name_to_id_map_list,
     libbfio_handle_t *file_io_handle,
     int debug_item_type,
     libcerror_error_t **error )
{
	static char *function                = "libpff_table_read_bc_values";
	uint32_t record_entries_reference    = 0;
	uint8_t record_entry_identifier_size = 0;
	uint8_t record_entry_value_size      = 0;
	uint8_t record_entries_level         = 0;

	/* Read the b5 table header
	 */
	if( libpff_table_read_b5_header(
	     table,
	     b5_table_header_reference,
	     &record_entry_identifier_size,
	     &record_entry_value_size,
	     &record_entries_level,
	     &record_entries_reference,
	     file_io_handle,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_READ_FAILED,
		 "%s: unable to read b5 table header.",
		 function );

		return( -1 );
	}
	if( ( record_entry_identifier_size != 2 )
	 || ( record_entry_value_size != 6 ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
		 "%s: unsupported record entry identifier size: 0x%02" PRIx8 " and record entry value size: 0x%02" PRIx8 ".",
		 function,
		 record_entry_identifier_size,
		 record_entry_value_size );

		return( -1 );
	}
	if( libpff_table_read_record_entries_data_references(
	     table,
	     record_entries_level,
	     record_entry_identifier_size,
	     record_entries_reference,
	     file_io_handle,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_READ_FAILED,
		 "%s: unable to read record entries data references.",
		 function );

		return( -1 );
	}
	if( libpff_table_read_bc_record_entries(
	     table,
	     io_handle,
	     file_io_handle,
	     name_to_id_map_list,
	     debug_item_type,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_READ_FAILED,
		 "%s: unable to read table record entries.",
		 function );

		return( -1 );
	}
	return( 1 );
}

/* Reads the b5 table header
 * Returns 1 if successful or -1 on error
 */
int libpff_table_read_b5_header(
     libpff_table_t *table,
     uint32_t table_header_reference,
     uint8_t *record_entry_identifier_size,
     uint8_t *record_entry_value_size,
     uint8_t *record_entries_level,
     uint32_t *record_entries_reference,
     libbfio_handle_t *file_io_handle,
     libcerror_error_t **error )
{
	libpff_table_index_value_t *table_index_value = NULL;
	uint8_t *table_data                           = NULL;
	uint8_t *table_value_data                     = NULL;
	static char *function                         = "libpff_table_read_b5_header";
	size_t table_data_size                        = 0;

	if( record_entry_identifier_size == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid record entry identifier size.",
		 function );

		return( -1 );
	}
	if( record_entry_value_size == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid record entry value size.",
		 function );

		return( -1 );
	}
	if( record_entries_level == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid record entries level.",
		 function );

		return( -1 );
	}
	if( record_entries_reference == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid record entries reference.",
		 function );

		return( -1 );
	}
	if( ( table_header_reference & 0x0000001fUL ) != 0 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_UNSUPPORTED_VALUE,
		 "%s: unsupported table header reference: 0x%08" PRIx32 " (0x%08" PRIx32 ").",
		 function,
		 table_header_reference & 0x0000001fUL,
		 table_header_reference );

		return( -1 );
	}
	/* Fetch the b5 table header reference
	 */
	if( libpff_table_get_index_value_by_reference(
	     table,
	     table_header_reference,
	     &table_index_value,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve b5 table header reference.",
		 function );

		return( -1 );
	}
	if( table_index_value == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: missing table index value.",
		 function );

		return( -1 );
	}
	/* Retrieve the corresponding table array entry
	 */
	if( libfdata_block_get_segment_data(
	     table->descriptor_data_block,
	     file_io_handle,
	     table->descriptor_data_cache,
	     (int) table_index_value->array_entry,
	     &table_data,
	     &table_data_size,
	     0,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve table array entry: %" PRIu32 ".",
		 function,
		 table_index_value->array_entry );

		return( -1 );
	}
	if( table_data == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: missing table array entry: %" PRIu32 ".",
		 function,
		 table_index_value->array_entry );

		return( -1 );
	}
	if( (size_t) table_index_value->offset >= table_data_size )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: table value offset exceeds table data size.",
		 function );

		return( -1 );
	}
	table_value_data = &( table_data[ table_index_value->offset ] );

	if( ( (pff_table_header_b5_t *) table_value_data )->type != 0xb5 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
		 "%s: unsupported table header type: 0x%02x.",
		 function,
		 ( (pff_table_header_b5_t *) table_value_data )->type );

		return( -1 );
	}
	*record_entry_identifier_size = ( (pff_table_header_b5_t *) table_value_data )->record_entry_identifier_size;
	*record_entry_value_size      = ( (pff_table_header_b5_t *) table_value_data )->record_entry_value_size;
	*record_entries_level         = ( (pff_table_header_b5_t *) table_value_data )->record_entries_level;

	byte_stream_copy_to_uint32_little_endian(
	 ( (pff_table_header_b5_t *) table_value_data )->record_entries_reference,
	 *record_entries_reference );

#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		libcnotify_printf(
		 "%s: table header type\t\t\t: 0x%02" PRIx8 "\n",
		 function,
		 ( (pff_table_header_b5_t *) table_value_data )->type );
		libcnotify_printf(
		 "%s: record entry identifier size\t: %" PRIu8 "\n",
		 function,
		 *record_entry_identifier_size );
		libcnotify_printf(
		 "%s: record entry value size\t\t: %" PRIu8 "\n",
		 function,
		 *record_entry_value_size );
		libcnotify_printf(
		 "%s: record entries level\t\t: %" PRIu8 "\n",
		 function,
		 ( (pff_table_header_b5_t *) table_value_data )->record_entries_level );
		libcnotify_printf(
		 "%s: record entries reference\t\t: 0x%08" PRIx32 " ",
		 function,
		 *record_entries_reference );
		libpff_debug_print_node_identifier_type(
		 (uint8_t) ( *record_entries_reference & 0x0000001fUL ) );
		libcnotify_printf(
		 "\n" );
		libcnotify_printf(
		 "\n" );
	}
#endif
	return( 1 );
}

/* Reads the 6c table record entries and their values
 * Returns 1 if successful or -1 on error
 */
int libpff_table_read_6c_record_entries(
     libpff_table_t *table,
     uint32_t values_array_reference,
     libbfio_handle_t *file_io_handle,
     libcerror_error_t **error )
{
	libfdata_reference_t *record_entries_data_reference = NULL;
	libpff_record_entry_t *record_entry                 = NULL;
	libpff_table_index_value_t *table_index_value       = NULL;
	uint8_t *table_data                                 = NULL;
	uint8_t *record_entries_data                        = NULL;
	uint8_t *table_values_array_data                    = NULL;
	static char *function                               = "libpff_table_read_6c_record_entries";
	size64_t record_entries_data_reference_size         = 0;
	size_t number_of_record_entries                     = 0;
	size_t table_data_size                              = 0;
	size_t record_entries_data_size                     = 0;
	size_t table_values_array_data_size                 = 0;
	uint32_t record_entry_iterator                      = 0;
	uint16_t values_array_number                        = 0;
	int number_of_record_entries_data_references        = 0;
	int record_entries_data_references_index            = 0;
	int record_entry_index                              = 0;

#if defined( HAVE_DEBUG_OUTPUT )
	libcstring_system_character_t guid_string[ 48 ];

	libfguid_identifier_t *guid                        = NULL;
	int result                                         = 0;
#endif

	if( table == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid table.",
		 function );

		return( -1 );
	}
	if( table->entry != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid table - entries already set.",
		 function );

		return( -1 );
	}
	if( values_array_reference == 0 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: table contains no value array.",
		 function );

		return( -1 );
	}
	if( ( values_array_reference & 0x0000001fUL ) != 0 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_UNSUPPORTED_VALUE,
		 "%s: unsupported values array entries reference: 0x%08" PRIx32 " (0x%08" PRIx32 ").",
		 function,
		 values_array_reference & 0x0000001fUL,
		 values_array_reference );

		return( -1 );
	}
	/* Fetch the values array
	 */
	if( libpff_table_get_index_value_by_reference(
	     table,
	     values_array_reference,
	     &table_index_value,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve values array entries reference.",
		 function );

		goto on_error;
	}
	if( table_index_value == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: missing table index value.",
		 function );

		goto on_error;
	}
	/* Retrieve the corresponding table array entry
	 */
	if( libfdata_block_get_segment_data(
	     table->descriptor_data_block,
	     file_io_handle,
	     table->descriptor_data_cache,
	     (int) table_index_value->array_entry,
	     &table_data,
	     &table_data_size,
	     0,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve table array entry: %" PRIu32 ".",
		 function,
		 table_index_value->array_entry );

		goto on_error;
	}
	if( table_data == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: missing table array entry: %" PRIu32 ".",
		 function,
		 table_index_value->array_entry );

		goto on_error;
	}
	if( (size_t) table_index_value->offset >= table_data_size )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: table value offset exceeds table data size.",
		 function );

		goto on_error;
	}
	table_values_array_data      = &( table_data[ table_index_value->offset ] );
	table_values_array_data_size = (size_t) table_index_value->size;

	if( libpff_array_get_number_of_entries(
	     table->record_entries_data_references,
	     &number_of_record_entries_data_references,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve number of record entries data references.",
		 function );

		goto on_error;
	}
	for( record_entries_data_references_index = 0;
	     record_entries_data_references_index < number_of_record_entries_data_references;
	     record_entries_data_references_index++ )
	{
#if defined( HAVE_DEBUG_OUTPUT )
		if( libcnotify_verbose != 0 )
		{
			libcnotify_printf(
			 "%s: record entries data reference: %d\n",
			 function,
			 record_entries_data_references_index );
		}
#endif
		if( libpff_array_get_entry_by_index(
		     table->record_entries_data_references,
		     record_entries_data_references_index,
		     (intptr_t **) &record_entries_data_reference,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve record entries data reference: %d.",
			 function,
			 record_entries_data_references_index );

			goto on_error;
		}
		if( libfdata_reference_get_size(
		     record_entries_data_reference,
		     &record_entries_data_reference_size,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve record entries data reference size: %d.",
			 function,
			 record_entries_data_references_index );

			goto on_error;
		}
		if( record_entries_data_reference_size > (size64_t) SSIZE_MAX )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
			 LIBCERROR_ARGUMENT_ERROR_VALUE_EXCEEDS_MAXIMUM,
			 "%s: invalid record entries data reference size value exceeds maximum.",
			 function );

			goto on_error;
		}
		if( ( record_entries_data_reference_size % sizeof( pff_table_record_entry_6c_t ) ) != 0 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
			 "%s: unsupported record entries data reference size.",
			 function );

			goto on_error;
		}
		number_of_record_entries = (size_t) record_entries_data_reference_size
		                         / sizeof( pff_table_record_entry_6c_t );

		if( number_of_record_entries > (size_t) INT_MAX )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
			 LIBCERROR_ARGUMENT_ERROR_VALUE_EXCEEDS_MAXIMUM,
			 "%s: number of record entries value exceeds maximum.",
			 function );

			goto on_error;
		}
		if( libpff_record_entries_resize(
		     table,
		     1,
		     table->number_of_entries + (int) number_of_record_entries,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_RESIZE_FAILED,
			 "%s: unable to resize record entries.",
			 function );

			goto on_error;
		}
		if( table->entry == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
			 "%s: invalid table - missing record entries.",
			 function );

			goto on_error;
		}
		record_entry = table->entry[ 0 ];

		if( record_entry == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
			 "%s: missing record entry.",
			 function );

			goto on_error;
		}
		for( record_entry_iterator = 0;
		     record_entry_iterator < number_of_record_entries;
		     record_entry_iterator++ )
		{
			/* The buffer data reference does not require a cache
			 */
			if( libfdata_reference_get_segment_data_at_offset(
			     record_entries_data_reference,
			     file_io_handle,
			     NULL,
			     (off64_t) ( record_entry_iterator * sizeof( pff_table_record_entry_6c_t ) ),
			     &record_entries_data,
			     &record_entries_data_size,
			     0,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
				 "%s: unable to retrieve record entries data at offset: %" PRIzd ".",
				 function,
				 (size_t) record_entry_iterator * sizeof( pff_table_record_entry_6c_t ) );

				goto on_error;
			}
			if( record_entries_data == NULL )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
				 "%s: missing record entries data.",
				 function );

				goto on_error;
			}
			if( record_entries_data_size < sizeof( pff_table_record_entry_6c_t ) )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
				 "%s: invalid record entries data size value out of bounds.",
				 function );

				goto on_error;
			}
			( record_entry[ record_entry_index ] ).identifier.format = LIBPFF_RECORD_ENTRY_IDENTIFIER_FORMAT_GUID;

			if( memory_copy(
			     ( record_entry[ record_entry_index ] ).identifier.guid,
			     ( (pff_table_record_entry_6c_t *) record_entries_data )->record_entry_guid,
			     16 ) == NULL )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_MEMORY,
				 LIBCERROR_MEMORY_ERROR_COPY_FAILED,
				 "%s: unable to copy record entry identifier GUID.",
				 function );

				goto on_error;
			}
			byte_stream_copy_to_uint16_little_endian(
			 ( (pff_table_record_entry_6c_t *) record_entries_data )->values_array_number,
			 values_array_number );

			if( (size_t) ( 16 * values_array_number ) > table_values_array_data_size )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
				 "%s: values array number exceeds table values array data size.",
				 function );

				goto on_error;
			}
			/* Create a buffer data reference of the record entry value data
			 */
			if( libfdata_buffer_reference_initialize(
			     &( ( record_entry[ record_entry_index ] ).value_data_reference ),
			     &( table_values_array_data[ 16 * values_array_number ] ),
			     16,
			     LIBFDATA_BUFFER_DATA_FLAG_NON_MANAGED,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
				 "%s: unable to create record entry value data reference.",
				 function );

				goto on_error;
			}
			/* TODO do something with values_array_number ? */

#if defined( HAVE_DEBUG_OUTPUT )
			if( libcnotify_verbose != 0 )
			{
				if( libfguid_identifier_initialize(
				     &guid,
				     error ) != 1 )
				{
					libcerror_error_set(
					 error,
					 LIBCERROR_ERROR_DOMAIN_RUNTIME,
					 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
					 "%s: unable to create GUID.",
					 function );

					goto on_error;
				}
				if( libfguid_identifier_copy_from_byte_stream(
				     guid,
				     ( record_entry[ record_entry_index ] ).identifier.guid,
				     16,
				     LIBFGUID_ENDIAN_LITTLE,
				     error ) != 1 )
				{
					libcerror_error_set(
					 error,
					 LIBCERROR_ERROR_DOMAIN_RUNTIME,
					 LIBCERROR_RUNTIME_ERROR_COPY_FAILED,
					 "%s: unable to copy byte stream to GUID.",
					 function );

					goto on_error;
				}
#if defined( LIBCSTRING_HAVE_WIDE_SYSTEM_CHARACTER )
				result = libfguid_identifier_copy_to_utf16_string(
					  guid,
					  (uint16_t *) guid_string,
					  48,
					  LIBFGUID_STRING_FORMAT_USE_LOWER_CASE,
					  error );
#else
				result = libfguid_identifier_copy_to_utf8_string(
					  guid,
					  (uint8_t *) guid_string,
					  48,
					  LIBFGUID_STRING_FORMAT_USE_LOWER_CASE,
					  error );
#endif
				if( result != 1 )
				{
					libcerror_error_set(
					 error,
					 LIBCERROR_ERROR_DOMAIN_RUNTIME,
					 LIBCERROR_RUNTIME_ERROR_COPY_FAILED,
					 "%s: unable to copy GUID to string.",
					 function );

					goto on_error;
				}
				libcnotify_printf(
				 "%s: table set: %03d entry: %03d record entry guid\t\t\t: %" PRIs_LIBCSTRING_SYSTEM "\n",
				 function,
				 0,
				 record_entry_index,
				 guid_string );

				if( libfguid_identifier_copy_from_byte_stream(
				     guid,
				     &( table_values_array_data[ 16 * values_array_number ] ),
				     16,
				     LIBFGUID_ENDIAN_LITTLE,
				     error ) != 1 )
				{
					libcerror_error_set(
					 error,
					 LIBCERROR_ERROR_DOMAIN_RUNTIME,
					 LIBCERROR_RUNTIME_ERROR_COPY_FAILED,
					 "%s: unable to copy byte stream to GUID.",
					 function );

					goto on_error;
				}
#if defined( LIBCSTRING_HAVE_WIDE_SYSTEM_CHARACTER )
				result = libfguid_identifier_copy_to_utf16_string(
					  guid,
					  (uint16_t *) guid_string,
					  48,
					  LIBFGUID_STRING_FORMAT_USE_LOWER_CASE,
					  error );
#else
				result = libfguid_identifier_copy_to_utf8_string(
					  guid,
					  (uint8_t *) guid_string,
					  48,
					  LIBFGUID_STRING_FORMAT_USE_LOWER_CASE,
					  error );
#endif
				if( result != 1 )
				{
					libcerror_error_set(
					 error,
					 LIBCERROR_ERROR_DOMAIN_RUNTIME,
					 LIBCERROR_RUNTIME_ERROR_COPY_FAILED,
					 "%s: unable to copy GUID to string.",
					 function );

					goto on_error;
				}
				if( libfguid_identifier_free(
				     &guid,
				     error ) != 1 )
				{
					libcerror_error_set(
					 error,
					 LIBCERROR_ERROR_DOMAIN_RUNTIME,
					 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
					 "%s: unable to free GUID.",
					 function );

					goto on_error;
				}
				libcnotify_printf(
				 "%s: table set: %03d entry: %03d record entry value guid\t\t: %" PRIs_LIBCSTRING_SYSTEM "\n",
				 function,
				 0,
				 record_entry_index,
				 guid_string );
				libcnotify_printf(
				 "%s: table set: %03d entry: %03d record entry value identifier\t: 0x%04" PRIx16 "\n",
				 function,
				 0,
				 record_entry_index,
				 values_array_number );

				libcnotify_printf(
				 "\n" );
			}
#endif
			record_entries_data += sizeof( pff_table_record_entry_6c_t );

			record_entry_index++;
		}
	}
	return( 1 );

on_error:
#if defined( HAVE_DEBUG_OUTPUT )
	if( guid != NULL )
	{
		libfguid_identifier_free(
		 &guid,
		 NULL );
	}
#endif
	return( -1 );
}

/* Reads the 7c table column definitions
 * Returns 1 if successful or -1 on error
 */
int libpff_table_read_7c_column_definitions(
     libpff_array_t *column_definitions_array,
     libfdata_reference_t *column_definitions_data_reference,
     int number_of_column_definitions,
     libbfio_handle_t *file_io_handle,
     libpff_list_t *name_to_id_map_list,
     libcerror_error_t **error )
{
	libpff_list_element_t *list_element           = NULL;
	libpff_column_definition_t *column_definition = NULL;
	uint8_t *column_definitions_data              = NULL;
	static char *function                         = "libpff_table_read_7c_column_definitions";
	size_t column_definitions_data_size           = 0;
	uint8_t column_definition_number              = 0;
	int list_iterator                             = 0;
	int column_definition_index                   = 0;

	if( column_definitions_array == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid column definitions array.",
		 function );

		return( -1 );
	}
	if( number_of_column_definitions == 0 )
	{
#if defined( HAVE_DEBUG_OUTPUT )
		if( libcnotify_verbose != 0 )
		{
			libcnotify_printf(
			 "%s: table contains no column definitions.\n",
			 function );
		}
#endif
		return( 1 );
	}
	if( libpff_array_resize(
	     column_definitions_array,
	     number_of_column_definitions,
	     (int (*)(intptr_t **, libcerror_error_t **)) &libpff_column_definition_free,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_RESIZE_FAILED,
		 "%s: unable to resize column definition array.",
		 function );

		goto on_error;
	}
	for( column_definition_index = 0;
	     column_definition_index < number_of_column_definitions;
	     column_definition_index++ )
	{
		/* The buffer data reference does not require a cache
		 */
		if( libfdata_reference_get_segment_data_at_offset(
		     column_definitions_data_reference,
		     file_io_handle,
		     NULL,
		     (off64_t) ( column_definition_index * sizeof( pff_table_column_definition_7c_t ) ),
		     &column_definitions_data,
		     &column_definitions_data_size,
		     0,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve column definitions data at offset: %" PRIzd ".",
			 function,
			 (size_t) column_definition_index * sizeof( pff_table_column_definition_7c_t ) );

			goto on_error;
		}
		if( column_definitions_data == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
			 "%s: missing column definitions data.",
			 function );

			goto on_error;
		}
		if( column_definitions_data_size < sizeof( pff_table_column_definition_7c_t ) )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
			 "%s: invalid column definitions data size value out of bounds.",
			 function );

			goto on_error;
		}
		if( libpff_column_definition_initialize(
		     &column_definition,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
			 "%s: unable to create column definition.",
			 function );

			goto on_error;
		}
		byte_stream_copy_to_uint16_little_endian(
		 ( (pff_table_column_definition_7c_t *) column_definitions_data )->record_entry_type,
		 column_definition->entry_type );
		byte_stream_copy_to_uint16_little_endian(
		 ( (pff_table_column_definition_7c_t *) column_definitions_data )->record_entry_value_type,
		 column_definition->value_type );
		byte_stream_copy_to_uint16_little_endian(
		 ( (pff_table_column_definition_7c_t *) column_definitions_data )->values_array_offset,
		 column_definition->values_array_offset );

		column_definition->values_array_size = ( (pff_table_column_definition_7c_t *) column_definitions_data )->values_array_size;
		column_definition_number             = ( (pff_table_column_definition_7c_t *) column_definitions_data )->values_array_number;

#if defined( HAVE_DEBUG_OUTPUT )
		if( libcnotify_verbose != 0 )
		{
			libcnotify_printf(
			 "%s: column definition: %03d record entry type\t: 0x%04" PRIx16 "",
			 function,
			 column_definition_index,
			 column_definition->entry_type );
		}
#endif
		if( ( column_definition->entry_type >= 0x8000 )
		 || ( column_definition->entry_type <= 0xfffe ) )
		{
			if( name_to_id_map_list != NULL )
			{
				list_element = name_to_id_map_list->first_element;

				for( list_iterator = 0;
				     list_iterator < name_to_id_map_list->number_of_elements;
				     list_iterator++ )
				{
					if( list_element == NULL )
					{
						break;
					}
					if( list_element->value == NULL )
					{
						continue;
					}
					if( ( (libpff_internal_name_to_id_map_entry_t *) list_element->value )->identifier == (uint32_t) column_definition->entry_type )
					{
						column_definition->name_to_id_map_entry = (libpff_internal_name_to_id_map_entry_t *) list_element->value;
					}
					list_element = list_element->next_element;
				}
			}
		}
#if defined( HAVE_DEBUG_OUTPUT )
		if( libcnotify_verbose != 0 )
		{
			if( column_definition->name_to_id_map_entry != NULL )
			{
				if( column_definition->name_to_id_map_entry->type == LIBPFF_NAME_TO_ID_MAP_ENTRY_TYPE_STRING )
				{
					libcnotify_printf(
					 " maps to: %s (%s : %s)\n",
					 (char *) column_definition->name_to_id_map_entry->debug_string,
					 libfmapi_named_property_type_get_identifier(
					  column_definition->name_to_id_map_entry->guid,
					  (char *) column_definition->name_to_id_map_entry->debug_string,
					  column_definition->name_to_id_map_entry->value_size,
					  column_definition->value_type ),
					 libfmapi_named_property_type_get_description(
					  column_definition->name_to_id_map_entry->guid,
					  (char *) column_definition->name_to_id_map_entry->debug_string,
					  column_definition->name_to_id_map_entry->value_size,
					  column_definition->value_type ) );
				}
				else
				{
					libcnotify_printf(
					 " maps to: 0x%04" PRIx32 " (%s : %s)\n",
					 column_definition->name_to_id_map_entry->numeric_value,
					 libfmapi_property_type_get_identifier(
					  column_definition->name_to_id_map_entry->guid,
					  column_definition->name_to_id_map_entry->numeric_value,
					  column_definition->value_type ),
					 libfmapi_property_type_get_description(
					  column_definition->name_to_id_map_entry->guid,
					  column_definition->name_to_id_map_entry->numeric_value,
					  column_definition->value_type ) );
				}
			}
			else
			{
				libcnotify_printf(
				 " (%s : %s)\n",
				 libfmapi_property_type_get_identifier(
				  NULL,
				  column_definition->entry_type,
				  column_definition->value_type ),
				 libfmapi_property_type_get_description(
				  NULL,
				  column_definition->entry_type,
				  column_definition->value_type ) );
			}
			libcnotify_printf(
			 "%s: column definition: %03d record entry value type\t: 0x%04" PRIx16 " (%s : %s)\n",
			 function,
			 column_definition_index,
			 column_definition->value_type,
			 libfmapi_value_type_get_identifier(
			  column_definition->value_type ),
			 libfmapi_value_type_get_description(
			  column_definition->value_type ) );
			libcnotify_printf(
			 "%s: column definition: %03d values array offset\t: %" PRIu16 "\n",
			 function,
			 column_definition_index,
			 column_definition->values_array_offset );
			libcnotify_printf(
			 "%s: column definition: %03d values array size\t: %" PRIu16 "\n",
			 function,
			 column_definition_index,
			 column_definition->values_array_size );
			libcnotify_printf(
			 "%s: column definition: %03d values array number\t: %" PRIu8 "\n",
			 function,
			 column_definition_index,
			 column_definition_number );
			libcnotify_printf(
			 "\n" );
		}
#endif
		if( libpff_array_set_entry_by_index(
		     column_definitions_array,
		     (int) column_definition_number,
		     (intptr_t *) column_definition,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
			 "%s: unable to set column definitions: %" PRIu8 " in array.",
			 function,
			 column_definition_number );

			goto on_error;
		}
		column_definition = NULL;
	}
	return( 1 );

on_error:
	if( column_definition != NULL )
	{
		libpff_column_definition_free(
		 &column_definition,
		 NULL );
	}
	return( -1 );
}

/* Reads the 8c table record entries and their values
 * Returns 1 if successful or -1 on error
 */
int libpff_table_read_8c_record_entries(
     libpff_table_t *table,
     libbfio_handle_t *file_io_handle,
     libcerror_error_t **error )
{
	libfdata_reference_t *record_entries_data_reference = NULL;
	libpff_record_entry_t *record_entry                 = NULL;
	uint8_t *record_entries_data                        = NULL;
	static char *function                               = "libpff_table_read_8c_record_entries";
	size64_t record_entries_data_reference_size         = 0;
	size_t number_of_record_entries                     = 0;
	size_t record_entries_data_size                     = 0;
	uint32_t record_entry_iterator                      = 0;
	int number_of_record_entries_data_references        = 0;
	int record_entries_data_references_index            = 0;
	int record_entry_index                              = 0;

#if defined( HAVE_DEBUG_OUTPUT )
	uint32_t value_32bit                                = 0;
#endif

	if( table == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid table.",
		 function );

		return( -1 );
	}
	if( table->entry != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid table - entries already set.",
		 function );

		return( -1 );
	}
	if( libpff_array_get_number_of_entries(
	     table->record_entries_data_references,
	     &number_of_record_entries_data_references,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve number of record entries data references.",
		 function );

		return( -1 );
	}
	for( record_entries_data_references_index = 0;
	     record_entries_data_references_index < number_of_record_entries_data_references;
	     record_entries_data_references_index++ )
	{
#if defined( HAVE_DEBUG_OUTPUT )
		if( libcnotify_verbose != 0 )
		{
			libcnotify_printf(
			 "%s: record entries data reference: %d\n",
			 function,
			 record_entries_data_references_index );
		}
#endif
		if( libpff_array_get_entry_by_index(
		     table->record_entries_data_references,
		     record_entries_data_references_index,
		     (intptr_t **) &record_entries_data_reference,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve record entries data reference: %d.",
			 function,
			 record_entries_data_reference );

			return( -1 );
		}
		if( libfdata_reference_get_size(
		     record_entries_data_reference,
		     &record_entries_data_reference_size,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve record entries data reference size.",
			 function );

			return( -1 );
		}
		if( record_entries_data_reference_size > (size64_t) SSIZE_MAX )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
			 LIBCERROR_ARGUMENT_ERROR_VALUE_EXCEEDS_MAXIMUM,
			 "%s: invalid record entries data reference size value exceeds maximum.",
			 function );

			return( -1 );
		}
		if( ( record_entries_data_reference_size % sizeof( pff_table_record_entry_8c_t ) ) != 0 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
			 "%s: unsupported record entries data reference size.",
			 function );

			return( -1 );
		}
		number_of_record_entries = (size_t) ( record_entries_data_reference_size / sizeof( pff_table_record_entry_8c_t ) );

		if( number_of_record_entries > (size_t) INT_MAX )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
			 LIBCERROR_ARGUMENT_ERROR_VALUE_EXCEEDS_MAXIMUM,
			 "%s: number of record entries value exceeds maximum.",
			 function );

			return( -1 );
		}
		if( libpff_record_entries_resize(
		     table,
		     1,
		     table->number_of_entries + (int) number_of_record_entries,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_RESIZE_FAILED,
			 "%s: unable to resize record entries.",
			 function );

			return( -1 );
		}
		if( table->entry == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
			 "%s: invalid table - missing record entries.",
			 function );

			return( -1 );
		}
		record_entry = table->entry[ 0 ];

		if( record_entry == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
			 "%s: missing record entry.",
			 function );

			return( -1 );
		}
		for( record_entry_iterator = 0;
		     record_entry_iterator < number_of_record_entries;
		     record_entry_iterator++ )
		{
			/* The buffer data reference does not require a cache
			 */
			if( libfdata_reference_get_segment_data_at_offset(
			     record_entries_data_reference,
			     file_io_handle,
			     NULL,
			     (off64_t) ( record_entry_iterator * sizeof( pff_table_record_entry_8c_t ) ),
			     &record_entries_data,
			     &record_entries_data_size,
			     0,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
				 "%s: unable to retrieve record entries data at offset: %" PRIzd ".",
				 function,
				 (size_t) record_entry_iterator * sizeof( pff_table_record_entry_8c_t ) );

				return( -1 );
			}
			if( record_entries_data == NULL )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
				 "%s: missing record entries data.",
				 function );

				return( -1 );
			}
			if( record_entries_data_size < sizeof( pff_table_record_entry_8c_t ) )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
				 "%s: invalid record entries data size value out of bounds.",
				 function );

				return( -1 );
			}
			( record_entry[ record_entry_index ] ).identifier.format = LIBPFF_RECORD_ENTRY_IDENTIFIER_FORMAT_SECURE4;

			byte_stream_copy_to_uint64_little_endian(
			 ( (pff_table_record_entry_8c_t *) record_entries_data )->identifier,
			 ( record_entry[ record_entry_index ] ).identifier.secure4 );

			/* Create a buffer data reference of the record entry value data
			 */
			if( libfdata_buffer_reference_initialize(
			     &( ( record_entry[ record_entry_index ] ).value_data_reference ),
			     ( (pff_table_record_entry_8c_t *) record_entries_data )->descriptor_identifier,
			     sizeof( uint32_t ),
			     LIBFDATA_BUFFER_DATA_FLAG_NON_MANAGED,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
				 "%s: unable to create record entry value data reference.",
				 function );

				return( -1 );
			}
#if defined( HAVE_DEBUG_OUTPUT )
			if( libcnotify_verbose != 0 )
			{
				libcnotify_printf(
				 "%s: table set: %03d entry: %03d identifier\t\t\t: 0x%" PRIx64 "\n",
				 function,
				 0,
				 record_entry_index,
				 ( record_entry[ record_entry_index ] ).identifier.secure4 );

				byte_stream_copy_to_uint32_little_endian(
				 ( (pff_table_record_entry_8c_t *) record_entries_data )->descriptor_identifier,
				 value_32bit );

				libcnotify_printf(
				 "%s: table set: %03d entry: %03d descriptor identifier\t: 0x%08" PRIx32 "\n",
				 function,
				 0,
				 record_entry_index,
				 value_32bit );

				libcnotify_printf(
				 "\n" );
			}
#endif
			record_entries_data += sizeof( pff_table_record_entry_8c_t );

			record_entry_index++;
		}
	}
	return( 1 );
}

/* Reads the 9c table record entries and their values
 * Returns 1 if successful or -1 on error
 */
int libpff_table_read_9c_record_entries(
     libpff_table_t *table,
     libbfio_handle_t *file_io_handle,
     libcerror_error_t **error )
{
	libfdata_reference_t *record_entries_data_reference = NULL;
	libpff_record_entry_t *record_entry                 = NULL;
	uint8_t *record_entries_data                        = NULL;
	static char *function                               = "libpff_table_read_9c_record_entries";
	size64_t record_entries_data_reference_size         = 0;
	size_t number_of_record_entries                     = 0;
	size_t record_entries_data_size                     = 0;
	uint32_t record_entry_iterator                      = 0;
	int number_of_record_entries_data_references        = 0;
	int record_entries_data_references_index            = 0;
	int record_entry_index                              = 0;

#if defined( HAVE_DEBUG_OUTPUT )
	libcstring_system_character_t guid_string[ 48 ];

	libfguid_identifier_t *guid                         = NULL;
	uint32_t value_32bit                                = 0;
	int result                                          = 0;
#endif

	if( table == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid table.",
		 function );

		return( -1 );
	}
	if( table->entry != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid table - entries already set.",
		 function );

		return( -1 );
	}
	if( libpff_array_get_number_of_entries(
	     table->record_entries_data_references,
	     &number_of_record_entries_data_references,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve number of record entries data references.",
		 function );

		goto on_error;
	}
	for( record_entries_data_references_index = 0;
	     record_entries_data_references_index < number_of_record_entries_data_references;
	     record_entries_data_references_index++ )
	{
#if defined( HAVE_DEBUG_OUTPUT )
		if( libcnotify_verbose != 0 )
		{
			libcnotify_printf(
			 "%s: record entries data reference: %d\n",
			 function,
			 record_entries_data_references_index );
		}
#endif
		if( libpff_array_get_entry_by_index(
		     table->record_entries_data_references,
		     record_entries_data_references_index,
		     (intptr_t **) &record_entries_data_reference,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve record entries data reference: %d.",
			 function,
			 record_entries_data_references_index );

			goto on_error;
		}
		if( libfdata_reference_get_size(
		     record_entries_data_reference,
		     &record_entries_data_reference_size,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve record entries data size: %d.",
			 function,
			 record_entries_data_references_index );

			goto on_error;
		}
		if( record_entries_data_reference_size > (size64_t) SSIZE_MAX )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
			 LIBCERROR_ARGUMENT_ERROR_VALUE_EXCEEDS_MAXIMUM,
			 "%s: invalid record entries data reference size value exceeds maximum.",
			 function );

			goto on_error;
		}
		if( ( record_entries_data_reference_size % sizeof( pff_table_record_entry_9c_t ) ) != 0 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
			 "%s: unsupported record entries data reference size.",
			 function );

			goto on_error;
		}
		number_of_record_entries = (size_t) record_entries_data_reference_size
		                         / sizeof( pff_table_record_entry_9c_t );

		if( number_of_record_entries > (size_t) INT_MAX )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
			 LIBCERROR_ARGUMENT_ERROR_VALUE_EXCEEDS_MAXIMUM,
			 "%s: number of record entries value exceeds maximum.",
			 function );

			goto on_error;
		}
		if( libpff_record_entries_resize(
		     table,
		     1,
		     table->number_of_entries + (int) number_of_record_entries,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_RESIZE_FAILED,
			 "%s: unable to resize record entries.",
			 function );

			goto on_error;
		}
		if( table->entry == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
			 "%s: invalid table - missing record entries.",
			 function );

			goto on_error;
		}
		record_entry = table->entry[ 0 ];

		if( record_entry == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
			 "%s: missing record entry.",
			 function );

			goto on_error;
		}
		for( record_entry_iterator = 0;
		     record_entry_iterator < number_of_record_entries;
		     record_entry_iterator++ )
		{
			/* The buffer data reference does not require a cache
			 */
			if( libfdata_reference_get_segment_data_at_offset(
			     record_entries_data_reference,
			     file_io_handle,
			     NULL,
			     (off64_t) ( record_entry_iterator * sizeof( pff_table_record_entry_9c_t ) ),
			     &record_entries_data,
			     &record_entries_data_size,
			     0,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
				 "%s: unable to retrieve record entries data at offset: %" PRIzd ".",
				 function,
				 (size_t) record_entry_iterator * sizeof( pff_table_record_entry_9c_t ) );

				goto on_error;
			}
			if( record_entries_data == NULL )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
				 "%s: missing record entries data.",
				 function );

				goto on_error;
			}
			if( record_entries_data_size < sizeof( pff_table_record_entry_9c_t ) )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
				 "%s: invalid record entries data size value out of bounds.",
				 function );

				goto on_error;
			}
			( record_entry[ record_entry_index ] ).identifier.format = LIBPFF_RECORD_ENTRY_IDENTIFIER_FORMAT_GUID;

			if( memory_copy(
			     ( record_entry[ record_entry_index ] ).identifier.guid,
			     ( (pff_table_record_entry_9c_t *) record_entries_data )->record_entry_guid,
			     16 ) == NULL )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_MEMORY,
				 LIBCERROR_MEMORY_ERROR_COPY_FAILED,
				 "%s: unable to copy record entry identifier GUID.",
				 function );

				goto on_error;
			}
			/* Create a buffer data reference of the record entry value data
			 */
			if( libfdata_buffer_reference_initialize(
			     &( ( record_entry[ record_entry_index ] ).value_data_reference ),
			     ( (pff_table_record_entry_9c_t *) record_entries_data )->descriptor_identifier,
			     sizeof( uint32_t ),
			     LIBFDATA_BUFFER_DATA_FLAG_NON_MANAGED,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
				 "%s: unable to create record entry value data reference.",
				 function );

				goto on_error;
			}
#if defined( HAVE_DEBUG_OUTPUT )
			if( libcnotify_verbose != 0 )
			{
				if( libfguid_identifier_initialize(
				     &guid,
				     error ) != 1 )
				{
					libcerror_error_set(
					 error,
					 LIBCERROR_ERROR_DOMAIN_RUNTIME,
					 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
					 "%s: unable to create GUID.",
					 function );

					goto on_error;
				}
				if( libfguid_identifier_copy_from_byte_stream(
				     guid,
				     ( record_entry[ record_entry_index ] ).identifier.guid,
				     16,
				     LIBFGUID_ENDIAN_LITTLE,
				     error ) != 1 )
				{
					libcerror_error_set(
					 error,
					 LIBCERROR_ERROR_DOMAIN_RUNTIME,
					 LIBCERROR_RUNTIME_ERROR_COPY_FAILED,
					 "%s: unable to copy byte stream to GUID.",
					 function );

					goto on_error;
				}
#if defined( LIBCSTRING_HAVE_WIDE_SYSTEM_CHARACTER )
				result = libfguid_identifier_copy_to_utf16_string(
					  guid,
					  (uint16_t *) guid_string,
					  48,
					  LIBFGUID_STRING_FORMAT_USE_LOWER_CASE,
					  error );
#else
				result = libfguid_identifier_copy_to_utf8_string(
					  guid,
					  (uint8_t *) guid_string,
					  48,
					  LIBFGUID_STRING_FORMAT_USE_LOWER_CASE,
					  error );
#endif
				if( result != 1 )
				{
					libcerror_error_set(
					 error,
					 LIBCERROR_ERROR_DOMAIN_RUNTIME,
					 LIBCERROR_RUNTIME_ERROR_COPY_FAILED,
					 "%s: unable to copy GUID to string.",
					 function );

					goto on_error;
				}
				if( libfguid_identifier_free(
				     &guid,
				     error ) != 1 )
				{
					libcerror_error_set(
					 error,
					 LIBCERROR_ERROR_DOMAIN_RUNTIME,
					 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
					 "%s: unable to free GUID.",
					 function );

					goto on_error;
				}
				libcnotify_printf(
				 "%s: table set: %03d entry: %03d record entry guid\t\t\t: %" PRIs_LIBCSTRING_SYSTEM "\n",
				 function,
				 0,
				 record_entry_index,
				 guid_string );

				byte_stream_copy_to_uint32_little_endian(
				 ( (pff_table_record_entry_9c_t *) record_entries_data )->descriptor_identifier,
				 value_32bit );

				libcnotify_printf(
				 "%s: table set: %03d entry: %03d record entry descriptor identifier\t: 0x%08" PRIx32 "\n",
				 function,
				 0,
				 record_entry_index,
				 value_32bit );

				libcnotify_printf(
				 "\n" );
			}
#endif
			record_entries_data += sizeof( pff_table_record_entry_9c_t );

			record_entry_index++;
		}
	}
	return( 1 );

on_error:
#if defined( HAVE_DEBUG_OUTPUT )
	if( guid != NULL )
	{
		libfguid_identifier_free(
		 &guid,
		 NULL );
	}
#endif
	return( -1 );
}

/* Reads the a5 table record entry values
 * Returns 1 if successful or -1 on error
 */
int libpff_table_read_a5_record_entries(
     libpff_table_t *table,
     uint32_t record_entries_reference,
     libbfio_handle_t *file_io_handle,
     libcerror_error_t **error )
{
	libpff_record_entry_t *record_entry             = NULL;
	libpff_table_index_value_t *table_index_value   = NULL;
	libpff_table_index_values_t *table_index_values = NULL;
	uint8_t *table_data                             = NULL;
	static char *function                           = "libpff_table_read_a5_record_entries";
	size_t table_data_size                          = 0;
	uint16_t table_index_values_iterator            = 0;
	int number_of_table_index_array_entries         = 0;
	int table_index_array_entries_iterator          = 0;

	if( table == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid table.",
		 function );

		return( -1 );
	}
	if( table->entry != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid table - entries already set.",
		 function );

		return( -1 );
	}
	/* Check if the table contains any entries
	 */
	if( record_entries_reference == 0 )
	{
#if defined( HAVE_DEBUG_OUTPUT )
		if( libcnotify_verbose != 0 )
		{
			libcnotify_printf(
			 "%s: table contains no entries.\n",
			 function );
		}
#endif
		return( 1 );
	}
	if( ( record_entries_reference & 0x0000001fUL ) != 0 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_UNSUPPORTED_VALUE,
		 "%s: unsupported record entries reference: 0x%08" PRIx32 " (0x%08" PRIx32 ").",
		 function,
		 record_entries_reference & 0x0000001fUL,
		 record_entries_reference );

		return( -1 );
	}
	if( libpff_array_get_number_of_entries(
	     table->index,
	     &number_of_table_index_array_entries,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve number of table index array entries.",
		 function );

		return( -1 );
	}
	for( table_index_array_entries_iterator = 0;
	     table_index_array_entries_iterator < number_of_table_index_array_entries;
	     table_index_array_entries_iterator++ )
	{
		if( libpff_array_get_entry_by_index(
		     table->index,
		     table_index_array_entries_iterator,
		     (intptr_t **) &table_index_values,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve table index array entry: %" PRIu16 ".",
			 function,
			 table_index_array_entries_iterator );

			return( -1 );
		}
		if( table_index_values == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
			 "%s: missing table index values.",
			 function );

			return( -1 );
		}
		if( table_index_values->value == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
			 "%s: invalid table index values - missing value.",
			 function );

			return( -1 );
		}
		if( ( number_of_table_index_array_entries > table->number_of_sets )
		 || ( table_index_values->number_of_values > table->number_of_entries ) )
		{
			if( libpff_record_entries_resize(
			     table,
			     number_of_table_index_array_entries,
			     (int) table_index_values->number_of_values,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_RESIZE_FAILED,
				 "%s: unable to resize record entries.",
				 function );

				return( -1 );
			}
		}
		if( table->entry == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
			 "%s: invalid table - missing record entries.",
			 function );

			return( -1 );
		}
		record_entry = table->entry[ table_index_array_entries_iterator ];

		if( record_entry == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
			 "%s: missing record entry.",
			 function );

			return( -1 );
		}
		for( table_index_values_iterator = 0;
		     table_index_values_iterator < table_index_values->number_of_values;
		     table_index_values_iterator++ )
		{
			table_index_value = &( table_index_values->value[ table_index_values_iterator ] );

			if( table_index_value == NULL )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
				 "%s: missing table index value.",
				 function );

				return( -1 );
			}
#if defined( HAVE_DEBUG_OUTPUT )
			if( libcnotify_verbose != 0 )
			{
				libcnotify_printf(
				 "%s: table set: %03" PRIu32 " entry: %03d value at offset: %" PRIu32 " with size: %" PRIu16 ".\n",
				 function,
				 table_index_array_entries_iterator,
				 table_index_values_iterator,
				 table_index_value->offset,
				 table_index_value->size );
			}
#endif
			/* Retrieve the corresponding table array entry
			 */
			if( libfdata_block_get_segment_data(
			     table->descriptor_data_block,
			     file_io_handle,
			     table->descriptor_data_cache,
			     (int) table_index_value->array_entry,
			     &table_data,
			     &table_data_size,
			     0,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
				 "%s: unable to retrieve table array entry: %" PRIu32 ".",
				 function,
				 table_index_value->array_entry );

				return( -1 );
			}
			if( table_data == NULL )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
				 "%s: missing table array entry: %" PRIu32 ".",
				 function,
				 table_index_value->array_entry );

				return( -1 );
			}
			if( (size_t) table_index_value->offset >= table_data_size )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
				 "%s: table value offset exceeds table data size.",
				 function );

				return( -1 );
			}
			/* Create a buffer data reference of the record entry value data
			 */
			if( libfdata_buffer_reference_initialize(
			     &( ( record_entry[ table_index_values_iterator ] ).value_data_reference ),
			     &( table_data[ table_index_value->offset ] ),
			     table_index_value->size,
			     LIBFDATA_BUFFER_DATA_FLAG_NON_MANAGED,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
				 "%s: unable to create record entry value data reference.",
				 function );

				return( -1 );
			}
		}
	}
#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		libcnotify_printf(
		 "\n" );
	}
#endif
	return( 1 );
}

/* Reads the ac table column definitions
 * Returns 1 if successful or -1 on error
 */
int libpff_table_read_ac_column_definitions(
     libpff_table_t *table,
     libpff_array_t *column_definitions_array,
     uint32_t column_definitions_reference,
     int number_of_column_definitions,
     libpff_io_handle_t *io_handle,
     libbfio_handle_t *file_io_handle,
     libpff_list_t *name_to_id_map_list,
     libcerror_error_t **error )
{
	libfcache_cache_t *column_definitions_data_cache        = NULL;
	libfdata_reference_t *column_definitions_data_reference = NULL;
	libpff_column_definition_t *column_definition           = NULL;
	libpff_list_element_t *list_element                     = NULL;
	libpff_local_descriptor_value_t *local_descriptor_value = NULL;
	libpff_table_index_value_t *table_index_value           = NULL;
	uint8_t *column_definition_data                         = NULL;
	uint8_t *column_definitions_data                        = NULL;
	static char *function                                   = "libpff_table_read_ac_column_definitions";
	size_t column_definition_data_size                      = 0;
	size_t column_definitions_data_size                     = 0;
	uint32_t record_entry_values_table_descriptor           = 0;
	uint16_t column_definition_number                       = 0;
	int list_iterator                                       = 0;
	int result                                              = 0;
	int column_definition_index                             = 0;

	if( table == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid table.",
		 function );

		return( -1 );
	}
	if( column_definitions_array == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid column definitions array.",
		 function );

		return( -1 );
	}
	if( number_of_column_definitions == 0 )
	{
#if defined( HAVE_DEBUG_OUTPUT )
		if( libcnotify_verbose != 0 )
		{
			libcnotify_printf(
			 "%s: table contains no column definitions.\n",
			 function );
		}
#endif
		return( 1 );
	}
	/* Read the column definitions
	 */
	result = libpff_local_descriptors_tree_get_value_by_identifier(
		  table->local_descriptors_tree,
		  file_io_handle,
		  table->local_descriptors_cache,
		  column_definitions_reference,
		  &local_descriptor_value,
	          error );

	if( result == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve descriptor identifier: %" PRIu32 " from local descriptors.",
		 function,
		 column_definitions_reference );

		goto on_error;
	}
	else if( result == 0 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: missing column definitions descriptor: 0x%08" PRIx32 " (%" PRIu32 ").",
		 function,
		 column_definitions_reference,
		 column_definitions_reference );

		goto on_error;
	}
	if( local_descriptor_value == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid local descriptor value.",
		 function );

		goto on_error;
	}
#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		libcnotify_printf(
		 "%s: identifier: %" PRIu64 " ",
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
	/* TODO handle multiple recovered offset index values */
	if( libpff_io_handle_read_descriptor_data_reference(
	     io_handle,
	     file_io_handle,
	     column_definitions_reference,
	     local_descriptor_value->data_identifier,
	     table->recovered,
	     0,
	     &column_definitions_data_reference,
	     &column_definitions_data_cache,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_READ_FAILED,
		 "%s: unable to read descriptor: %" PRIu32 " data: %" PRIu64 ".",
		 function,
		 column_definitions_reference,
		 local_descriptor_value->data_identifier );

		goto on_error;
	}
	/* Retrieve the corresponding column definitions data reference segment
	 */
	if( libfdata_reference_get_segment_data(
	     column_definitions_data_reference,
	     file_io_handle,
	     column_definitions_data_cache,
	     0,
	     &column_definitions_data,
	     &column_definitions_data_size,
	     0,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve column definitions data reference segment: 0.",
		 function );

		goto on_error;
	}
	if( column_definitions_data == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: missing table array entry: %" PRIu32 ".",
		 function,
		 table_index_value->array_entry );

		goto on_error;
	}
#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		libcnotify_printf(
		 "%s: ac column definitions:\n",
		 function );
		libcnotify_print_data(
		 column_definitions_data,
		 column_definitions_data_size,
		 0 );
	}
#endif
	if( ( (size_t) number_of_column_definitions * sizeof( pff_table_column_definition_ac_t ) ) != column_definitions_data_size )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: mismatch in number of column definitions and the data size.",
		 function );

		goto on_error;
	}
	if( libpff_array_resize(
	     column_definitions_array,
	     number_of_column_definitions,
	     (int (*)(intptr_t **, libcerror_error_t **)) &libpff_column_definition_free,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_RESIZE_FAILED,
		 "%s: unable to resize column definition array.",
		 function );

		goto on_error;
	}
	for( column_definition_index = 0;
	     column_definition_index < number_of_column_definitions;
	     column_definition_index++ )
	{
		if( libpff_column_definition_initialize(
		     &column_definition,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
			 "%s: unable to create column definition.",
			 function );

			goto on_error;
		}
		if( libfdata_reference_get_segment_data_at_offset(
		     column_definitions_data_reference,
		     file_io_handle,
		     column_definitions_data_cache,
		     (off64_t) ( column_definition_index * sizeof( pff_table_column_definition_ac_t ) ),
		     &column_definition_data,
		     &column_definition_data_size,
		     0,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve column definitions data at offset: %" PRIzd ".",
			 function,
			 (size_t) column_definition_index * sizeof( pff_table_column_definition_ac_t ) );

			goto on_error;
		}
		if( column_definition_data == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
			 "%s: missing column definitions data.",
			 function );

			goto on_error;
		}
		if( column_definition_data_size < sizeof( pff_table_column_definition_ac_t ) )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
			 "%s: invalid column definitions data size value out of bounds.",
			 function );

			goto on_error;
		}
		byte_stream_copy_to_uint16_little_endian(
		 ( (pff_table_column_definition_ac_t *) column_definition_data )->record_entry_type,
		 column_definition->entry_type );
		byte_stream_copy_to_uint16_little_endian(
		 ( (pff_table_column_definition_ac_t *) column_definition_data )->record_entry_value_type,
		 column_definition->value_type );
		byte_stream_copy_to_uint16_little_endian(
		 ( (pff_table_column_definition_ac_t *) column_definition_data )->values_array_offset,
		 column_definition->values_array_offset );
		byte_stream_copy_to_uint16_little_endian(
		 ( (pff_table_column_definition_ac_t *) column_definition_data )->values_array_size,
		 column_definition->values_array_size );
		byte_stream_copy_to_uint16_little_endian(
		 ( (pff_table_column_definition_ac_t *) column_definition_data )->values_array_number,
		 column_definition_number );
		byte_stream_copy_to_uint32_little_endian(
		 ( (pff_table_column_definition_ac_t *) column_definition_data )->record_entry_values_table_descriptor,
		 record_entry_values_table_descriptor );

#if defined( HAVE_DEBUG_OUTPUT )
		if( libcnotify_verbose != 0 )
		{
			libcnotify_printf(
			 "%s: column definition: %03d record entry type\t\t\t: 0x%04" PRIx16 "",
			 function,
			 column_definition_index,
			 column_definition->entry_type );
		}
#endif
		if( ( column_definition->entry_type >= 0x8000 )
		 || ( column_definition->entry_type <= 0xfffe ) )
		{
			if( name_to_id_map_list != NULL )
			{
				list_element = name_to_id_map_list->first_element;

				for( list_iterator = 0;
				     list_iterator < name_to_id_map_list->number_of_elements;
				     list_iterator++ )
				{
					if( list_element == NULL )
					{
						break;
					}
					if( list_element->value == NULL )
					{
						continue;
					}
					if( ( (libpff_internal_name_to_id_map_entry_t *) list_element->value )->identifier == (uint32_t) column_definition->entry_type )
					{
						column_definition->name_to_id_map_entry = (libpff_internal_name_to_id_map_entry_t *) list_element->value;
					}
					list_element = list_element->next_element;
				}
			}
		}
#if defined( HAVE_DEBUG_OUTPUT )
		if( libcnotify_verbose != 0 )
		{
			if( column_definition->name_to_id_map_entry != NULL )
			{
				if( column_definition->name_to_id_map_entry->type == LIBPFF_NAME_TO_ID_MAP_ENTRY_TYPE_STRING )
				{
					libcnotify_printf(
					 " maps to: %s (%s : %s)\n",
					 (char *) column_definition->name_to_id_map_entry->debug_string,
					 libfmapi_named_property_type_get_identifier(
					  column_definition->name_to_id_map_entry->guid,
					  (char *) column_definition->name_to_id_map_entry->debug_string,
					  column_definition->name_to_id_map_entry->value_size,
					  column_definition->value_type ),
					 libfmapi_named_property_type_get_description(
					  column_definition->name_to_id_map_entry->guid,
					  (char *) column_definition->name_to_id_map_entry->debug_string,
					  column_definition->name_to_id_map_entry->value_size,
					  column_definition->value_type ) );
				}
				else
				{
					libcnotify_printf(
					 " maps to: 0x%04" PRIx32 " (%s : %s)\n",
					 column_definition->name_to_id_map_entry->numeric_value,
					 libfmapi_property_type_get_identifier(
					  column_definition->name_to_id_map_entry->guid,
					  column_definition->name_to_id_map_entry->numeric_value,
					  column_definition->value_type ),
					 libfmapi_property_type_get_description(
					  column_definition->name_to_id_map_entry->guid,
					  column_definition->name_to_id_map_entry->numeric_value,
					  column_definition->value_type ) );
				}
			}
			else
			{
				libcnotify_printf(
				 " (%s : %s)\n",
				 libfmapi_property_type_get_identifier(
				  NULL,
				  column_definition->entry_type,
				  column_definition->value_type ),
				 libfmapi_property_type_get_description(
				  NULL,
				  column_definition->entry_type,
				  column_definition->value_type ) );
			}
			libcnotify_printf(
			 "%s: column definition: %03d record entry value type\t\t: 0x%04" PRIx16 " (%s : %s)\n",
			 function,
			 column_definition_index,
			 column_definition->value_type,
			 libfmapi_value_type_get_identifier(
			  column_definition->value_type ),
			 libfmapi_value_type_get_description(
			  column_definition->value_type ) );

			libcnotify_printf(
			 "%s: column definition: %03d values array offset\t\t: %" PRIu16 "\n",
			 function,
			 column_definition_index,
			 column_definition->values_array_offset );

			libcnotify_printf(
			 "%s: column definition: %03d values array size\t\t: %" PRIu16 "\n",
			 function,
			 column_definition_index,
			 column_definition->values_array_size );

			libcnotify_printf(
			 "%s: column definition: %03d values array number\t\t: %" PRIu16 "\n",
			 function,
			 column_definition_index,
			 column_definition_number );

			libcnotify_printf(
			 "%s: padding1:\n",
			 function );
			libcnotify_print_data(
			 ( (pff_table_column_definition_ac_t *) column_definition_data )->padding1,
			 2,
			 0 );

			libcnotify_printf(
			 "%s: column definition: %03d record entry values table descriptor\t: %" PRIu32 "\n",
			 function,
			 column_definition_index,
			 record_entry_values_table_descriptor );

			libcnotify_printf(
			 "\n" );
		}
#endif
		/* Read the record entry values table if necessary
		 */
		if( record_entry_values_table_descriptor > 0 )
		{
			result = libpff_local_descriptors_tree_get_value_by_identifier(
				  table->local_descriptors_tree,
				  file_io_handle,
				  table->local_descriptors_cache,
				  record_entry_values_table_descriptor,
				  &local_descriptor_value,
				  error );

			if( result == -1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
				 "%s: unable to retrieve descriptor identifier: %" PRIu32 " from local descriptors.",
				 function,
				 record_entry_values_table_descriptor );

				goto on_error;
			}
			else if( result == 0 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
				 "%s: missing local descriptor identifier: %" PRIu32 ".",
				 function,
				 record_entry_values_table_descriptor );

				goto on_error;
			}
			if( local_descriptor_value == NULL )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
				 "%s: invalid local descriptor value.",
				 function );

				goto on_error;
			}
#if defined( HAVE_DEBUG_OUTPUT )
			if( libcnotify_verbose != 0 )
			{
				libcnotify_printf(
				 "%s: identifier: %" PRIu64 " ",
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
			if( local_descriptor_value->data_identifier == 0 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
				 "%s: invalid local descriptor identifier: %" PRIu32 " - missing data identifier.",
				 function,
				 record_entry_values_table_descriptor );

				goto on_error;
			}
			if( libpff_table_initialize(
			     &( column_definition->record_entry_values_table ),
			     record_entry_values_table_descriptor,
			     local_descriptor_value->data_identifier,
			     local_descriptor_value->local_descriptors_identifier,
			     table->recovered,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
				 "%s: unable to create record entry values table.",
				 function );

				goto on_error;
			}
			if( libpff_table_read(
			     column_definition->record_entry_values_table,
			     io_handle,
			     file_io_handle,
			     name_to_id_map_list,
			     LIBPFF_DEBUG_ITEM_TYPE_DEFAULT,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_IO,
				 LIBCERROR_IO_ERROR_READ_FAILED,
				 "%s: unable to read record entry values table.",
				 function );

				goto on_error;
			}
		}
		if( libpff_array_set_entry_by_index(
		     column_definitions_array,
		     (int) column_definition_number,
		     (intptr_t *) column_definition,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
			 "%s: unable to set column definition: %" PRIu16 " in array.",
			 function,
			 column_definition_number );

			goto on_error;
		}
		column_definition = NULL;
	}
	if( libfcache_cache_free(
	     &column_definitions_data_cache,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
		 "%s: unable to free column definitions data cache.",
		 function );

		goto on_error;
	}
	if( libfdata_reference_free(
	     &column_definitions_data_reference,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
		 "%s: unable to free column definitions data reference.",
		 function );

		goto on_error;
	}
	return( 1 );

on_error:
	if( column_definition != NULL )
	{
		libpff_column_definition_free(
		 &column_definition,
		 NULL );
	}
	if( column_definitions_data_cache != NULL )
	{
		libfcache_cache_free(
		 &column_definitions_data_cache,
		 NULL );
	}
	if( column_definitions_data_reference != NULL )
	{
		libfdata_reference_free(
		 &column_definitions_data_reference,
		 NULL );
	}
	return( -1 );
}

/* Reads the bc table record entries and their values
 * Returns 1 if successful or -1 on error
 */
int libpff_table_read_bc_record_entries(
     libpff_table_t *table,
     libpff_io_handle_t *io_handle,
     libbfio_handle_t *file_io_handle,
     libpff_list_t *name_to_id_map_list,
     int debug_item_type,
     libcerror_error_t **error )
{
	libfdata_reference_t *record_entries_data_reference = NULL;
	uint8_t *record_entries_data                        = NULL;
	static char *function                               = "libpff_table_read_bc_record_entries";
	size64_t record_entries_data_reference_size         = 0;
	size_t number_of_record_entries                     = 0;
	size_t record_entries_data_size                     = 0;
	uint32_t record_entry_iterator                      = 0;
	uint16_t record_entry_type                          = 0;
	uint16_t record_entry_value_type                    = 0;
	int number_of_record_entries_data_references        = 0;
	int record_entries_data_references_index            = 0;
	int record_entry_index                              = 0;

	if( table == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid table.",
		 function );

		return( -1 );
	}
	if( table->entry != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid table - entries already set.",
		 function );

		return( -1 );
	}
	if( libpff_array_get_number_of_entries(
	     table->record_entries_data_references,
	     &number_of_record_entries_data_references,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve number of record entries data references.",
		 function );

		return( -1 );
	}
	for( record_entries_data_references_index = 0;
	     record_entries_data_references_index < number_of_record_entries_data_references;
	     record_entries_data_references_index++ )
	{
#if defined( HAVE_DEBUG_OUTPUT )
		if( libcnotify_verbose != 0 )
		{
			libcnotify_printf(
			 "%s: record entries data reference: %d\n",
			 function,
			 record_entries_data_references_index );
		}
#endif
		if( libpff_array_get_entry_by_index(
		     table->record_entries_data_references,
		     record_entries_data_references_index,
		     (intptr_t **) &record_entries_data_reference,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve record entries data reference: %d.",
			 function,
			 record_entries_data_reference );

			return( -1 );
		}
		if( libfdata_reference_get_size(
		     record_entries_data_reference,
		     &record_entries_data_reference_size,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve record entries data reference size.",
			 function );

			return( -1 );
		}
		if( record_entries_data_reference_size > (size64_t) SSIZE_MAX )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
			 LIBCERROR_ARGUMENT_ERROR_VALUE_EXCEEDS_MAXIMUM,
			 "%s: invalid record entries data reference size value exceeds maximum.",
			 function );

			return( -1 );
		}
		if( ( record_entries_data_reference_size % sizeof( pff_table_record_entry_bc_t ) ) != 0 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
			 "%s: unsupported record entries data reference size.",
			 function );

			return( -1 );
		}
		number_of_record_entries = (size_t) record_entries_data_reference_size
		                         / sizeof( pff_table_record_entry_bc_t );

		if( number_of_record_entries > (size_t) INT_MAX )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
			 LIBCERROR_ARGUMENT_ERROR_VALUE_EXCEEDS_MAXIMUM,
			 "%s: number of record entries value exceeds maximum.",
			 function );

			return( -1 );
		}
		if( libpff_record_entries_resize(
		     table,
		     1,
		     table->number_of_entries + (int) number_of_record_entries,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_RESIZE_FAILED,
			 "%s: unable to resize record entries.",
			 function );

			return( -1 );
		}
		for( record_entry_iterator = 0;
		     record_entry_iterator < number_of_record_entries;
		     record_entry_iterator++ )
		{
			/* The buffer data reference does not require a cache
			 */
			if( libfdata_reference_get_segment_data_at_offset(
			     record_entries_data_reference,
			     file_io_handle,
			     NULL,
			     (off64_t) ( record_entry_iterator * sizeof( pff_table_record_entry_bc_t ) ),
			     &record_entries_data,
			     &record_entries_data_size,
			     0,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
				 "%s: unable to retrieve record entries data at offset: %" PRIzd ".",
				 function,
				 (size_t) record_entry_iterator * sizeof( pff_table_record_entry_bc_t ) );

				return( -1 );
			}
			if( record_entries_data == NULL )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
				 "%s: missing record entries data.",
				 function );

				return( -1 );
			}
			if( record_entries_data_size < sizeof( pff_table_record_entry_bc_t ) )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
				 "%s: invalid record entries data size value out of bounds.",
				 function );

				return( -1 );
			}
			byte_stream_copy_to_uint16_little_endian(
			 ( (pff_table_record_entry_bc_t *) record_entries_data )->record_entry_type,
			 record_entry_type );
			byte_stream_copy_to_uint16_little_endian(
			 ( (pff_table_record_entry_bc_t *) record_entries_data )->record_entry_value_type,
			 record_entry_value_type );

			if( libpff_table_read_entry_value(
			     table,
			     0,
			     record_entry_index,
			     (uint32_t) record_entry_type,
			     (uint32_t) record_entry_value_type,
			     (uint8_t *) ( (pff_table_record_entry_bc_t *) record_entries_data )->record_entry_value,
			     4,
			     io_handle,
			     file_io_handle,
			     name_to_id_map_list,
			     NULL,
			     NULL,
			     debug_item_type,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_IO,
				 LIBCERROR_IO_ERROR_READ_FAILED,
				 "%s: unable to read entry value: %" PRIu32 ".",
				 function,
				 record_entry_index );

				return( -1 );
			}
			record_entry_index++;
		}
	}
	return( 1 );
}

/* Reads the table values array
 * Returns 1 if successful or -1 on error
 */
int libpff_table_read_values_array(
     libpff_table_t *table,
     uint32_t record_entries_reference,
     uint32_t values_array_reference,
     uint8_t record_entry_identifier_size,
     uint8_t record_entry_value_size,
     uint16_t values_array_size,
     libpff_array_t *column_definitions_array,
     libpff_io_handle_t *io_handle,
     libbfio_handle_t *file_io_handle,
     libcerror_error_t **error )
{
	libfcache_cache_t *table_values_data_cache              = NULL;
	libfdata_reference_t *record_entries_data_reference     = NULL;
	libfdata_reference_t *table_values_data_reference       = NULL;
	libpff_local_descriptor_value_t *local_descriptor_value = NULL;
	libpff_column_definition_t *column_definition           = NULL;
	libpff_table_index_value_t *table_index_value           = NULL;
	uint8_t *table_data                                     = NULL;
	uint8_t *record_entries_data                            = NULL;
	uint8_t *record_entry_values_data                       = NULL;
	uint8_t *table_values_data                              = NULL;
	static char *function                                   = "libpff_table_read_values_array";
	size64_t record_entries_data_reference_size             = 0;
	size_t number_of_record_entries                         = 0;
	size_t number_of_values_array_entries                   = 0;
	size_t table_data_size                                  = 0;
	size_t record_entries_data_size                         = 0;
	size_t record_entry_size                                = 0;
	size_t record_entry_values_data_size                    = 0;
	size_t table_values_data_size                           = 0;
	uint32_t record_entry_iterator                          = 0;
	uint32_t record_entry_values_array_identifier           = 0;
	uint32_t record_entry_values_array_number               = 0;
	uint32_t table_values_array_identifier                  = 0;
	int column_definition_index                             = 0;
	int number_of_record_entries_data_references            = 0;
	int result                                              = 0;
	int record_entries_data_references_index                = 0;
	int record_entry_index                                  = 0;

	if( table == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid table.",
		 function );

		return( -1 );
	}
	if( ( record_entry_identifier_size != 4 )
	 || ( ( record_entry_value_size != 2 )
	  &&  ( record_entry_value_size != 4 ) ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
		 "%s: unsupported record entry identifier size: 0x%02" PRIx8 " and record entry value size: 0x%02" PRIx8 ".",
		 function,
		 record_entry_identifier_size,
		 record_entry_value_size );

		return( -1 );
	}
	if( values_array_size == 0 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_ZERO_OR_LESS,
		 "%s: invalid values array size value zero or less.",
		 function );

		return( -1 );
	}
	if( column_definitions_array == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid column definitions array.",
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
	/* Check if the table contains any entries
	 */
	if( ( record_entries_reference == 0 )
	 && ( values_array_reference == 0 ) )
	{
#if defined( HAVE_DEBUG_OUTPUT )
		if( libcnotify_verbose != 0 )
		{
			libcnotify_printf(
			 "%s: table contains no entries.\n",
			 function );
		}
#endif
		return( 1 );
	}
	if( record_entries_reference == 0 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: table contains value array but no record entries.",
		 function );

		goto on_error;
	}
	if( values_array_reference == 0 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: table contains record entries but no value array.",
		 function );

		goto on_error;
	}
	record_entry_size = record_entry_identifier_size + record_entry_value_size;

	/* Determine the values array entries
	 */

	/* TODO check node type ! */
	if( ( values_array_reference & 0x0000001fUL ) != 0 )
	{
		result = libpff_local_descriptors_tree_get_value_by_identifier(
			  table->local_descriptors_tree,
			  file_io_handle,
			  table->local_descriptors_cache,
			  values_array_reference,
			  &local_descriptor_value,
			  error );

		if( result == -1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve descriptor identifier: %" PRIu32 " from local descriptors.",
			 function,
			 values_array_reference );

			goto on_error;
		}
		else if( result == 0 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
			 "%s: missing values array descriptor: 0x%08" PRIx32 " (%" PRIu32 ").",
			 function,
			 values_array_reference,
			 values_array_reference );

			goto on_error;
		}
		if( local_descriptor_value == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
			 "%s: invalid local descriptor value.",
			 function );

			goto on_error;
		}
#if defined( HAVE_DEBUG_OUTPUT )
		if( libcnotify_verbose != 0 )
		{
			libcnotify_printf(
			 "%s: identifier: %" PRIu64 " ",
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
		/* TODO handle multiple recovered offset index values */
		if( libpff_io_handle_read_descriptor_data_reference(
		     io_handle,
		     file_io_handle,
		     values_array_reference,
		     local_descriptor_value->data_identifier,
		     table->recovered,
		     0,
		     &table_values_data_reference,
		     &table_values_data_cache,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_IO,
			 LIBCERROR_IO_ERROR_READ_FAILED,
			 "%s: unable to read descriptor: %" PRIu32 " data: %" PRIu64 ".",
			 function,
			 values_array_reference,
			 local_descriptor_value->data_identifier );

			goto on_error;
		}
	}
	else
	{
		if( libpff_table_get_index_value_by_reference(
		     table,
		     values_array_reference,
		     &table_index_value,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve values array entries reference.",
			 function );

			goto on_error;
		}
		if( table_index_value == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
			 "%s: missing table index value.",
			 function );

			goto on_error;
		}
		/* Retrieve the corresponding table array entry
		 */
		if( libfdata_block_get_segment_data(
		     table->descriptor_data_block,
		     file_io_handle,
		     table->descriptor_data_cache,
		     (int) table_index_value->array_entry,
		     &table_data,
		     &table_data_size,
		     0,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve table values array data reference segment: %" PRIu32 ".",
			 function,
			 table_index_value->array_entry );

			goto on_error;
		}
		if( table_data == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
			 "%s: missing table array entry: %" PRIu32 ".",
			 function,
			 table_index_value->array_entry );

			goto on_error;
		}
		if( (size_t) table_index_value->offset >= table_data_size )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
			 "%s: table value offset exceeds table data size.",
			 function );

			goto on_error;
		}
		/* Embed the data in a buffer data reference
		 */
		if( libfdata_buffer_reference_initialize(
		     &table_values_data_reference,
		     &( table_data[ table_index_value->offset ] ),
		     table_index_value->size,
		     LIBFDATA_BUFFER_DATA_FLAG_NON_MANAGED,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
			 "%s: unable to create data reference.",
			 function );

			goto on_error;
		}
	}
	if( libfdata_reference_get_segment_data(
	     table_values_data_reference,
	     file_io_handle,
	     table_values_data_cache,
	     0,
	     &table_values_data,
	     &table_values_data_size,
	     0,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve table values data reference segment: 0.",
		 function );

		goto on_error;
	}
	if( table_values_data == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: missing table values data.",
		 function );

		goto on_error;
	}
#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		if( ( values_array_reference & 0x0000001fUL ) != 0 )
		{
			libcnotify_printf(
			 "%s: table values data:\n",
			 function );
			libcnotify_print_data(
			 table_values_data,
			 table_values_data_size,
			 0 );
		}
	}
#endif
	number_of_values_array_entries = table_values_data_size / values_array_size;

	if( libpff_array_get_number_of_entries(
	     table->record_entries_data_references,
	     &number_of_record_entries_data_references,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve number of record entries data references.",
		 function );

		goto on_error;
	}
	for( record_entries_data_references_index = 0;
	     record_entries_data_references_index < number_of_record_entries_data_references;
	     record_entries_data_references_index++ )
	{
#if defined( HAVE_DEBUG_OUTPUT )
		if( libcnotify_verbose != 0 )
		{
			libcnotify_printf(
			 "%s: record entries data reference\t\t\t: %d\n",
			 function,
			 record_entries_data_references_index );
		}
#endif
		if( libpff_array_get_entry_by_index(
		     table->record_entries_data_references,
		     record_entries_data_references_index,
		     (intptr_t **) &record_entries_data_reference,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve record entries data reference: %d.",
			 function,
			 record_entries_data_references_index );

			goto on_error;
		}
		if( libfdata_reference_get_size(
		     record_entries_data_reference,
		     &record_entries_data_reference_size,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve record entries data reference size.",
			 function );

			goto on_error;
		}
		if( record_entries_data_reference_size > (size64_t) SSIZE_MAX )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
			 LIBCERROR_ARGUMENT_ERROR_VALUE_EXCEEDS_MAXIMUM,
			 "%s: invalid record entries data reference size value exceeds maximum.",
			 function );

			goto on_error;
		}
		if( ( record_entries_data_reference_size % record_entry_size ) != 0 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
			 "%s: unsupported record entries data reference size.",
			 function );

			goto on_error;
		}
		number_of_record_entries = (size_t) ( record_entries_data_reference_size / record_entry_size );

		if( number_of_record_entries > (size_t) INT_MAX )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
			 LIBCERROR_ARGUMENT_ERROR_VALUE_EXCEEDS_MAXIMUM,
			 "%s: number of record entries value exceeds maximum.",
			 function );

			goto on_error;
		}
#if defined( HAVE_DEBUG_OUTPUT )
		if( libcnotify_verbose != 0 )
		{
			libcnotify_printf(
			 "%s: number of record entries\t\t\t: %" PRIzd "\n",
			 function,
			 number_of_record_entries );
		}
#endif
		if( table->number_of_sets < ( record_entry_index + (int) number_of_record_entries ) )
		{
/* TODO column_definitions_array->number_of_entries replace by function */
			if( libpff_record_entries_resize(
			     table,
			     record_entry_index + (int) number_of_record_entries,
			     column_definitions_array->number_of_entries,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_RESIZE_FAILED,
				 "%s: unable to resize record entries.",
				 function );

				goto on_error;
			}
		}
		for( record_entry_iterator = 0;
		     record_entry_iterator < number_of_record_entries;
		     record_entry_iterator++ )
		{
			/* The buffer data reference does not require a cache
			 */
			if( libfdata_reference_get_segment_data_at_offset(
			     record_entries_data_reference,
			     file_io_handle,
			     NULL,
			     (off64_t) ( record_entry_iterator * record_entry_size ),
			     &record_entries_data,
			     &record_entries_data_size,
			     0,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
				 "%s: unable to retrieve record entries data at offset: %" PRIzd ".",
				 function,
				 (size_t) record_entry_iterator * record_entry_size );

				goto on_error;
			}
			if( record_entries_data == NULL )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
				 "%s: missing record entries data.",
				 function );

				goto on_error;
			}
			if( record_entries_data_size < record_entry_size )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
				 "%s: invalid record entries data size value out of bounds.",
				 function );

				goto on_error;
			}
			byte_stream_copy_to_uint32_little_endian(
			 record_entries_data,
			 record_entry_values_array_identifier );

			record_entries_data += 4;

			if( record_entry_value_size == 2 )
			{
				byte_stream_copy_to_uint16_little_endian(
				 record_entries_data,
				 record_entry_values_array_number );

				record_entries_data += 2;
			}
			else if( record_entry_value_size == 4 )
			{
				byte_stream_copy_to_uint32_little_endian(
				 record_entries_data,
				 record_entry_values_array_number );

				record_entries_data += 4;
			}
#if defined( HAVE_DEBUG_OUTPUT )
			if( libcnotify_verbose != 0 )
			{
				libcnotify_printf(
				 "%s: record entry: %03" PRIu32 " values array identifier\t: 0x%08" PRIx32 "\n",
				 function,
				 record_entry_iterator,
				 record_entry_values_array_identifier );
				libcnotify_printf(
				 "%s: record entry: %03" PRIu32 " values array number\t\t: %" PRIu32 "\n",
				 function,
				 record_entry_iterator,
				 record_entry_values_array_number );
				libcnotify_printf(
				 "\n" );
			}
#endif
			check_and_limit_number_of_records(record_entry_values_array_number);
			
			/* If the value array numbers are not stored sequential
			 * resize the record entries to the required size
			 */
			if( (uint32_t) table->number_of_sets < record_entry_values_array_number )
			{
/* TODO column_definitions_array->number_of_entries replace by function */
				if( libpff_record_entries_resize(
				     table,
				     (int) record_entry_values_array_number,
				     column_definitions_array->number_of_entries,
				     error ) != 1 )
				{
					libcerror_error_set(
					 error,
					 LIBCERROR_ERROR_DOMAIN_RUNTIME,
					 LIBCERROR_RUNTIME_ERROR_RESIZE_FAILED,
					 "%s: unable to resize record entries.",
					 function );

					goto on_error;
				}
			}
			if( libfdata_reference_get_segment_data_at_value_index(
			     table_values_data_reference,
			     file_io_handle,
			     table_values_data_cache,
			     (int) record_entry_values_array_number,
			     (size_t) values_array_size,
			     &record_entry_values_data,
			     &record_entry_values_data_size,
			     0,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
				 "%s: unable to retrieve record entry values data for index: %" PRIu32 ".",
				 function,
				 record_entry_values_array_number );

				goto on_error;
			}
			if( record_entry_values_data == NULL )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
				 "%s: missing record entry values data.",
				 function );

				goto on_error;
			}
			if( record_entry_values_data_size < (size_t) values_array_size )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
				 "%s: invalid table values data size value out of bounds.",
				 function );

				goto on_error;
			}
/* TODO column_definitions_array->number_of_entries replace by function */
			for( column_definition_index = 0;
			     column_definition_index < column_definitions_array->number_of_entries;
			     column_definition_index++ )
			{
				if( libpff_array_get_entry_by_index(
				     column_definitions_array,
				     column_definition_index,
				     (intptr_t **) &column_definition,
				     error ) != 1 )
				{
					libcerror_error_set(
					 error,
					 LIBCERROR_ERROR_DOMAIN_RUNTIME,
					 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
					 "%s: unable to retrieve column definitions array entry: %d.",
					 function,
					 column_definition_index );

					goto on_error;
				}
				if( column_definition == NULL )
				{
					libcerror_error_set(
					 error,
					 LIBCERROR_ERROR_DOMAIN_RUNTIME,
					 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
					 "%s: missing column definition: %d.",
					 function,
					 column_definition_index );

					goto on_error;
				}
				/* For some unknown reason when the values array is read
				 * the data array is padded with zero or remnant values
				 * therefore the values array entries do not align
				 * this check is makes sure the aligment is correct
				 */
				if( column_definition_index == 0 )
				{
					if( column_definition->values_array_offset != 0 )
					{
						libcerror_error_set(
						 error,
						 LIBCERROR_ERROR_DOMAIN_RUNTIME,
						 LIBCERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
						 "%s: unsupported first column definition values array offset.",
						 function );

						goto on_error;
					}
					if( column_definition->values_array_size != 4 )
					{
						libcerror_error_set(
						 error,
						 LIBCERROR_ERROR_DOMAIN_RUNTIME,
						 LIBCERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
						 "%s: unsupported first column definition values array size.",
						 function );

						goto on_error;
					}
					byte_stream_copy_to_uint32_little_endian(
					 record_entry_values_data,
					 table_values_array_identifier );

					/* If decryption was forced reread the entry without decryption
					 */
					if( ( io_handle->force_decryption != 0 )
					 && ( record_entry_values_array_identifier != table_values_array_identifier ) )
					{
						if( libfdata_reference_get_segment_data_at_value_index(
						     table_values_data_reference,
						     file_io_handle,
						     table_values_data_cache,
						     (int) record_entry_values_array_number,
						     (size_t) values_array_size,
						     &record_entry_values_data,
						     &record_entry_values_data_size,
						     LIBFDATA_READ_FLAG_IGNORE_CACHE | LIBPFF_READ_FLAG_IGNORE_FORCE_DECRYPTION,
						     error ) != 1 )
						{
							libcerror_error_set(
							 error,
							 LIBCERROR_ERROR_DOMAIN_RUNTIME,
							 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
							 "%s: unable to retrieve record entry values data for index: %" PRIu32 ".",
							 function,
							 record_entry_values_array_number );

							goto on_error;
						}
						if( record_entry_values_data == NULL )
						{
							libcerror_error_set(
							 error,
							 LIBCERROR_ERROR_DOMAIN_RUNTIME,
							 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
							 "%s: missing record entry values data.",
							 function );

							goto on_error;
						}
						if( record_entry_values_data_size < (size_t) values_array_size )
						{
							libcerror_error_set(
							 error,
							 LIBCERROR_ERROR_DOMAIN_RUNTIME,
							 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
							 "%s: invalid table values data size value out of bounds.",
							 function );

							goto on_error;
						}
						byte_stream_copy_to_uint32_little_endian(
						 record_entry_values_data,
						 table_values_array_identifier );
					}
					if( record_entry_values_array_identifier != table_values_array_identifier )
					{
						libcerror_error_set(
						 error,
						 LIBCERROR_ERROR_DOMAIN_RUNTIME,
						 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
						 "%s: mismatch in values array identifier (%" PRIu32 " != %" PRIu32 ").",
						 function,
						 record_entry_values_array_identifier,
						 table_values_array_identifier );

						goto on_error;
					}
#if defined( HAVE_DEBUG_OUTPUT )
					if( libcnotify_verbose != 0 )
					{
						libcnotify_printf(
						 "%s: values array data:\n",
						 function );
						libcnotify_print_data(
						 record_entry_values_data,
						 values_array_size,
						 0 );
					}
#endif
				}
				if( column_definition->values_array_offset > values_array_size )
				{
					libcerror_error_set(
					 error,
					 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
					 LIBCERROR_ARGUMENT_ERROR_VALUE_OUT_OF_BOUNDS,
					 "%s: invalid column definition values array offset value exceeds values array size.",
					 function );

					goto on_error;
				}
#if defined( HAVE_DEBUG_OUTPUT )
				if( libcnotify_verbose != 0 )
				{
					libcnotify_printf(
					 "%s: values array data at offset: %" PRIu16 " of size: %" PRIu16 "\n",
					 function,
					 column_definition->values_array_offset,
					 column_definition->values_array_size );
					libcnotify_print_data(
					 &( record_entry_values_data[ column_definition->values_array_offset ] ),
					 (size_t) column_definition->values_array_size,
					 0 );
				}
#endif
				/* To prevent multiple lookups the name to id map is not passed
				 */
				if( libpff_table_read_entry_value(
				     table,
				     record_entry_index,
				     column_definition_index,
				     column_definition->entry_type,
				     column_definition->value_type,
				     &( record_entry_values_data[ column_definition->values_array_offset ] ),
				     (size_t) column_definition->values_array_size,
				     io_handle,
				     file_io_handle,
				     NULL,
				     column_definition->name_to_id_map_entry,
				     column_definition->record_entry_values_table,
				     LIBPFF_DEBUG_ITEM_TYPE_DEFAULT,
				     error ) != 1 )
				{
					libcerror_error_set(
					 error,
					 LIBCERROR_ERROR_DOMAIN_IO,
					 LIBCERROR_IO_ERROR_READ_FAILED,
					 "%s: unable to read entry value: %" PRIu32 ".",
					 function,
					 record_entry_values_array_number );

					goto on_error;
				}
			}
			record_entry_index++;
		}
	}
	if( table_values_data_cache != NULL )
	{
		if( libfcache_cache_free(
		     &table_values_data_cache,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
			 "%s: unable to free table values data cache.",
			 function );

			goto on_error;
		}
	}
	if( libfdata_reference_free(
	     &table_values_data_reference,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
		 "%s: unable to free table values data reference.",
		 function );

		goto on_error;
	}
	return( 1 );

on_error:
	if( table_values_data_cache != NULL )
	{
		libfcache_cache_free(
		 &table_values_data_cache,
		 NULL );
	}
	if( table_values_data_reference != NULL )
	{
		libfdata_reference_free(
		 &table_values_data_reference,
		 NULL );
	}
	return( -1 );
}

/* Reads a table record entry value
 * Returns 1 if successful or -1 on error
 */
int libpff_table_read_entry_value(
     libpff_table_t *table,
     int set_index,
     int entry_index,
     uint32_t record_entry_type,
     uint32_t record_entry_value_type,
     uint8_t *record_entry_value,
     size_t record_entry_value_size,
     libpff_io_handle_t *io_handle,
     libbfio_handle_t *file_io_handle,
     libpff_list_t *name_to_id_map_list,
     libpff_internal_name_to_id_map_entry_t *name_to_id_map_entry,
     libpff_table_t *record_entry_values_table,
     int debug_item_type,
     libcerror_error_t **error )
{
	libfcache_cache_t *record_entry_value_data_cache        = NULL;
	libfdata_reference_t *record_entry_value_data_reference = NULL;
	libpff_list_element_t *list_element                     = NULL;
	libpff_local_descriptor_value_t *local_descriptor_value = NULL;
	libpff_record_entry_t *record_entry                     = NULL;
	libpff_table_index_value_t *table_index_value           = NULL;
	uint8_t *table_data                                     = NULL;
	uint8_t *record_entry_value_data                        = NULL;
	static char *function                                   = "libpff_table_read_entry_value";
	size_t table_data_size                                  = 0;
	size_t record_entry_value_data_size                     = 0;
	uint64_t entry_value                                    = 0;
	int list_iterator                                       = 0;
	int result                                              = 0;

	if( table == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid table.",
		 function );

		return( -1 );
	}
	if( table->entry == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid table - missing entries.",
		 function );

		return( -1 );
	}
	if( ( set_index < 0 )
	 || ( set_index >= table->number_of_sets ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: invalid set index value out of bounds.",
		 function );

		return( -1 );
	}
	if( ( entry_index < 0 )
	 || ( entry_index >= table->number_of_entries ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: invalid entry index value out of bounds.",
		 function );

		return( -1 );
	}
	if( record_entry_value == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid record entry value.",
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
	if( table->entry[ set_index ] == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid table - missing entry set.",
		 function );

		return( -1 );
	}
	record_entry = table->entry[ set_index ];

#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		libcnotify_printf(
		 "%s: table set: %03" PRIu32 " entry: %03" PRIu32 " record entry type\t\t\t: 0x%04" PRIx32 "",
		 function,
		 set_index,
		 entry_index,
		 record_entry_type );
	}
#endif
	if( ( record_entry_type >= 0x8000 )
	 || ( record_entry_type <= 0xfffe ) )
	{
		/* The corresponding name to id map entry was already determined
		 */
		if( name_to_id_map_entry != NULL )
		{
			( record_entry[ entry_index ] ).name_to_id_map_entry = name_to_id_map_entry;
		}
		else if( name_to_id_map_list != NULL )
		{
			list_element = name_to_id_map_list->first_element;

			for( list_iterator = 0;
			     list_iterator < name_to_id_map_list->number_of_elements;
			     list_iterator++ )
			{
				if( list_element == NULL )
				{
					break;
				}
				if( list_element->value == NULL )
				{
					continue;
				}
				if( ( (libpff_internal_name_to_id_map_entry_t *) list_element->value )->identifier == record_entry_type )
				{
					( record_entry[ entry_index ] ).name_to_id_map_entry = (libpff_internal_name_to_id_map_entry_t *) list_element->value;
				}
				list_element = list_element->next_element;
			}
		}
	}
	( record_entry[ entry_index ] ).identifier.format     = LIBPFF_RECORD_ENTRY_IDENTIFIER_FORMAT_MAPI_PROPERTY;
	( record_entry[ entry_index ] ).identifier.entry_type = record_entry_type;
	( record_entry[ entry_index ] ).identifier.value_type = record_entry_value_type;

#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		if( debug_item_type == LIBPFF_DEBUG_ITEM_TYPE_NAME_TO_ID_MAP )
		{
			libcnotify_printf(
			 " (%s : %s)\n",
			 libpff_debug_get_name_to_id_map_property_type_identifier(
			  record_entry_type,
			  record_entry_value_type ),
			 libpff_debug_get_name_to_id_map_property_type_description(
			  record_entry_type,
			  record_entry_value_type ) );
		}
		else if( ( record_entry[ entry_index ] ).name_to_id_map_entry != NULL )
		{
			if( ( record_entry[ entry_index ] ).name_to_id_map_entry->type == LIBPFF_NAME_TO_ID_MAP_ENTRY_TYPE_STRING )
			{
				libcnotify_printf(
				 " maps to: %s (%s : %s)\n",
				 (char *) ( record_entry[ entry_index ] ).name_to_id_map_entry->debug_string,
				 libfmapi_named_property_type_get_identifier(
				  ( record_entry[ entry_index ] ).name_to_id_map_entry->guid,
				  (char *) ( record_entry[ entry_index ] ).name_to_id_map_entry->debug_string,
				  ( record_entry[ entry_index ] ).name_to_id_map_entry->value_size,
				  ( record_entry[ entry_index ] ).identifier.value_type ),
				 libfmapi_named_property_type_get_description(
				  ( record_entry[ entry_index ] ).name_to_id_map_entry->guid,
				  (char *) ( record_entry[ entry_index ] ).name_to_id_map_entry->debug_string,
				  ( record_entry[ entry_index ] ).name_to_id_map_entry->value_size,
				  ( record_entry[ entry_index ] ).identifier.value_type ) );
			}
			else
			{
				libcnotify_printf(
				 " maps to: 0x%04" PRIx32 " (%s : %s)\n",
				 ( record_entry[ entry_index ] ).name_to_id_map_entry->numeric_value,
				 libfmapi_property_type_get_identifier(
				  ( record_entry[ entry_index ] ).name_to_id_map_entry->guid,
				  ( record_entry[ entry_index ] ).name_to_id_map_entry->numeric_value,
				  ( record_entry[ entry_index ] ).identifier.value_type ),
				 libfmapi_property_type_get_description(
				  ( record_entry[ entry_index ] ).name_to_id_map_entry->guid,
				  ( record_entry[ entry_index ] ).name_to_id_map_entry->numeric_value,
				  ( record_entry[ entry_index ] ).identifier.value_type ) );
			}
		}
		else
		{
			libcnotify_printf(
			 " (%s : %s)\n",
			 libfmapi_property_type_get_identifier(
			  NULL,
			  ( record_entry[ entry_index ] ).identifier.entry_type,
			  ( record_entry[ entry_index ] ).identifier.value_type ),
			 libfmapi_property_type_get_description(
			  NULL,
			  ( record_entry[ entry_index ] ).identifier.entry_type,
			  ( record_entry[ entry_index ] ).identifier.value_type ) );
		}
		libcnotify_printf(
		 "%s: table set: %03" PRIu32 " entry: %03" PRIu32 " record entry value type\t\t: 0x%04" PRIx16 " (%s : %s)\n",
		 function,
		 set_index,
		 entry_index,
		 record_entry_value_type,
		 libfmapi_value_type_get_identifier(
		  ( record_entry[ entry_index ] ).identifier.value_type ),
		 libfmapi_value_type_get_description(
		  ( record_entry[ entry_index ] ).identifier.value_type ) );
	}
#endif
	if( record_entry_value_size == 1 )
	{
		entry_value = record_entry_value[ 0 ];
	}
	else if( record_entry_value_size == 2 )
	{
		byte_stream_copy_to_uint16_little_endian(
		 record_entry_value,
		 entry_value );
	}
	else if( record_entry_value_size == 4 )
	{
		byte_stream_copy_to_uint32_little_endian(
		 record_entry_value,
		 entry_value );
	}
	else if( record_entry_value_size == 8 )
	{
		byte_stream_copy_to_uint64_little_endian(
		 record_entry_value,
		 entry_value );
	}
	else
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
		 "%s: unsupported record entry value size: %" PRIu8 ".",
		 function,
		 record_entry_value_size );

		return( -1 );
	}
	/* Check if there is a record entry values (a5) table
	 */
	if( record_entry_values_table != NULL )
	{
#if defined( HAVE_DEBUG_OUTPUT )
		if( libcnotify_verbose != 0 )
		{
			libcnotify_printf(
			 "%s: table set: %03" PRIu32 " entry: %03" PRIu32 " record entry values table reference\t: 0x%08" PRIx64 "\n",
			 function,
			 set_index,
			 entry_index,
			 entry_value );
		}
#endif
		/* TODO check entry value type */
		if( ( entry_value & 0x0000001fUL ) != 0 )
		{
			result = libpff_local_descriptors_tree_get_value_by_identifier(
				  table->local_descriptors_tree,
				  file_io_handle,
				  table->local_descriptors_cache,
				  (uint32_t) entry_value,
				  &local_descriptor_value,
				  error );

			if( result == -1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
				 "%s: unable to retrieve descriptor identifier: %" PRIu32 " from local descriptors.",
				 function,
				 (uint32_t) entry_value );

				return( -1 );
			}
			else if( result == 0 )
			{
#if defined( HAVE_DEBUG_OUTPUT )
				if( libcnotify_verbose != 0 )
				{
					libcnotify_printf(
					 "%s: missing descriptor: %" PRIu32 " - marked as missing.\n",
					 function,
					 (uint32_t) entry_value );
				}
#endif
				( record_entry[ entry_index ] ).value_data_reference = NULL;
				( record_entry[ entry_index ] ).flags               |= LIBPFF_RECORD_ENTRY_FLAG_MISSING_DATA_DESCRIPTOR;
				table->flags                                        |= LIBPFF_TABLE_FLAG_MISSING_RECORD_ENTRY_DATA;
			}
			else
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
					 "%s: identifier: %" PRIu64 " ",
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
				/* TODO handle multiple recovered offset index values */
				if( libpff_io_handle_read_descriptor_data_reference(
				     io_handle,
				     file_io_handle,
				     (uint32_t) entry_value,
				     local_descriptor_value->data_identifier,
				     table->recovered,
				     0,
				     &( ( record_entry[ entry_index ] ).value_data_reference ),
				     &( ( record_entry[ entry_index ] ).value_data_cache ),
				     error ) != 1 )
				{
					libcerror_error_set(
					 error,
					 LIBCERROR_ERROR_DOMAIN_IO,
					 LIBCERROR_IO_ERROR_READ_FAILED,
					 "%s: unable to read record entry value data with descriptor: %" PRIu32 " - marked as missing.",
					 function,
					 (uint32_t) entry_value );

#if defined( HAVE_DEBUG_OUTPUT )
					if( ( libcnotify_verbose != 0 )
					 && ( error != NULL )
					 && ( *error != NULL ) )
					{
						libcnotify_print_error_backtrace(
						 *error );
					}
#endif
					libcerror_error_free(
					 error );

					/* If the data descriptor could not be read mark it as missing
					 * and give it an empty value data reference
					 */
					( record_entry[ entry_index ] ).value_data_reference = NULL;
					( record_entry[ entry_index ] ).flags               |= LIBPFF_RECORD_ENTRY_FLAG_MISSING_DATA_DESCRIPTOR;
					table->flags                                        |= LIBPFF_TABLE_FLAG_MISSING_RECORD_ENTRY_DATA;
				}
			}
		}
		/* Check if the entry value is a value within the record entry values (a5) table
		 */
		else if( entry_value > 0 )
		{
			/* The record entry value reference needs to be transformed into a table set and entry
			 * value. Table array entries have been stored as separate sets.
			 */
			result = libpff_table_get_entry_value_by_index(
				  record_entry_values_table,
				  (int) ( ( entry_value >> 16 ) & 0xffff ),
				  (int) ( ( ( entry_value & 0xffe0 ) >> 5 ) - 1 ),
				  &record_entry_value_data_reference,
				  &record_entry_value_data_cache,
				  error );

			if( result == -1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
				 "%s: unable to retrieve record entry value index: 0x%08" PRIx64 " (set: %d, entry: %d)",
				 function,
				 entry_value,
				 (int) ( ( entry_value >> 16 ) & 0xffff ),
				 (int) ( ( ( entry_value & 0xffe0 ) >> 5 ) - 1 ) );

				return( -1 );
			}
			/* A missing record entry value reference signifies an empty value (NULL)
			 */
			else if( result != 0 )
			{
				if( libfdata_reference_get_segment_data_at_offset(
				     record_entry_value_data_reference,
				     file_io_handle,
				     record_entry_value_data_cache,
				     0,
				     &record_entry_value_data,
				     &record_entry_value_data_size,
				     0,
				     error ) != 1 )
				{
					libcerror_error_set(
					 error,
					 LIBCERROR_ERROR_DOMAIN_RUNTIME,
					 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
					 "%s: unable to retrieve record entry value data at offset: 0.",
					 function );

					return( -1 );
				}
				if( record_entry_value_data == NULL )
				{
					libcerror_error_set(
					 error,
					 LIBCERROR_ERROR_DOMAIN_RUNTIME,
					 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
					 "%s: missing record entry value data.",
					 function );

					return( -1 );
				}
			}
		}
	}
	else
	{
		/* The Boolean (0x000b)
		 * is 1 byte of size in the 7c table
		 * is 4 bytes of size in the bc table
		 *
		 * the first byte contains the value
		 * the value is 0x00 if false or true otherwise
		 */
		if( record_entry_value_type == LIBPFF_VALUE_TYPE_BOOLEAN )
		{
#if defined( HAVE_DEBUG_OUTPUT )
			if( libcnotify_verbose != 0 )
			{
				libcnotify_printf(
				 "%s: table set: %03" PRIu32 " entry: %03" PRIu32 " record entry value\t\t\t: 0x%08" PRIx64 "\n",
				 function,
				 set_index,
				 entry_index,
				 entry_value );

				if( ( record_entry_value_size != 1 )
				 && ( record_entry_value_size != 4 ) )
				{
					libcerror_error_set(
					 error,
					 LIBCERROR_ERROR_DOMAIN_RUNTIME,
					 LIBCERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
					 "%s: unsupported value type: 0x%08" PRIx32 " with value size: %" PRIu8 ".",
					 function,
					 record_entry_value_type,
					 record_entry_value_size );

					return( -1 );
				}
			}
#endif
			/* The first byte in the record entry value buffer
			 * contains the actual value of the boolean
			 */
			record_entry_value_data      = record_entry_value;
			record_entry_value_data_size = sizeof( uint8_t );
		}
		/* The Integer 16-bit signed (0x0002)
		 * is 2 bytes of size in the 7c table
		 * is 4 bytes of size in the bc table
		 *
		 * the first two bytes contains the value
		 */
		else if( record_entry_value_type == LIBPFF_VALUE_TYPE_INTEGER_16BIT_SIGNED )
		{
#if defined( HAVE_DEBUG_OUTPUT )
			if( libcnotify_verbose != 0 )
			{
				libcnotify_printf(
				 "%s: table set: %03" PRIu32 " entry: %03" PRIu32 " record entry value\t\t\t: 0x%08" PRIx64 "\n",
				 function,
				 set_index,
				 entry_index,
				 entry_value );

				if( ( record_entry_value_size != 2 )
				 && ( record_entry_value_size != 4 ) )
				{
					libcerror_error_set(
					 error,
					 LIBCERROR_ERROR_DOMAIN_RUNTIME,
					 LIBCERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
					 "%s: unsupported value type: 0x%08" PRIx32 " with value size: %" PRIu8 ".",
					 function,
					 record_entry_value_type,
					 record_entry_value_size );

					return( -1 );
				}
			}
#endif
			record_entry_value_data      = record_entry_value;
			record_entry_value_data_size = sizeof( uint16_t );
		}
		/* The Integer 32-bit signed (0x0003)
		 *     Floating point single precision (0x0004)
		 *     Error scode (0x000a)
		 *
		 * is 4 bytes of size in the 7c and bc table
		 */
		else if( ( record_entry_value_type == LIBPFF_VALUE_TYPE_INTEGER_32BIT_SIGNED )
		      || ( record_entry_value_type == LIBPFF_VALUE_TYPE_FLOAT_32BIT )
		      || ( record_entry_value_type == LIBPFF_VALUE_TYPE_ERROR ) )
		{
#if defined( HAVE_DEBUG_OUTPUT )
			if( libcnotify_verbose != 0 )
			{
				libcnotify_printf(
				 "%s: table set: %03" PRIu32 " entry: %03" PRIu32 " record entry value\t\t\t: 0x%08" PRIx64 "\n",
				 function,
				 set_index,
				 entry_index,
				 entry_value );

				if( record_entry_value_size != 4 )
				{
					libcerror_error_set(
					 error,
					 LIBCERROR_ERROR_DOMAIN_RUNTIME,
					 LIBCERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
					 "%s: unsupported value type: 0x%08" PRIx32 " with value size: %" PRIu8 ".",
					 function,
					 record_entry_value_type,
					 record_entry_value_size );

					return( -1 );
				}
			}
#endif
			record_entry_value_data      = record_entry_value;
			record_entry_value_data_size = sizeof( uint32_t );
		}
		/* The Floating point double precision (0x0005)
		 *     Currency (64-bit) (0x0006)
		 *     Application time (64-bit) (0x0007)
		 *     Integer 64-bit signed (0x0014)
		 *     Windows Filetime (64-bit) (0x0040)
		 *
		 * is 8 bytes of size in the 7c table
		 */
		else if( ( record_entry_value_size == 8 )
		      && ( ( record_entry_value_type == LIBPFF_VALUE_TYPE_DOUBLE_64BIT )
		       || ( record_entry_value_type == LIBPFF_VALUE_TYPE_CURRENCY )
		       || ( record_entry_value_type == LIBPFF_VALUE_TYPE_APPLICATION_TIME )
		       || ( record_entry_value_type == LIBPFF_VALUE_TYPE_INTEGER_64BIT_SIGNED )
		       || ( record_entry_value_type == LIBPFF_VALUE_TYPE_FILETIME ) ) )
		{
#if defined( HAVE_DEBUG_OUTPUT )
			if( libcnotify_verbose != 0 )
			{
				libcnotify_printf(
				 "%s: table set: %03" PRIu32 " entry: %03" PRIu32 " record entry value\t\t\t: 0x%08" PRIx64 "\n",
				 function,
				 set_index,
				 entry_index,
				 entry_value );
			}
#endif
			record_entry_value_data      = record_entry_value;
			record_entry_value_data_size = sizeof( uint64_t );
		}
		/* These values are references in the bc table
		 */
		else if( ( record_entry_value_type == LIBPFF_VALUE_TYPE_DOUBLE_64BIT )
		      || ( record_entry_value_type == LIBPFF_VALUE_TYPE_CURRENCY )
		      || ( record_entry_value_type == LIBPFF_VALUE_TYPE_APPLICATION_TIME )
		      || ( record_entry_value_type == LIBPFF_VALUE_TYPE_OBJECT )
		      || ( record_entry_value_type == LIBPFF_VALUE_TYPE_INTEGER_64BIT_SIGNED )
		      || ( record_entry_value_type == LIBPFF_VALUE_TYPE_STRING_ASCII )
		      || ( record_entry_value_type == LIBPFF_VALUE_TYPE_STRING_UNICODE )
		      || ( record_entry_value_type == LIBPFF_VALUE_TYPE_FILETIME )
		      || ( record_entry_value_type == LIBPFF_VALUE_TYPE_GUID )
		      || ( record_entry_value_type == LIBPFF_VALUE_TYPE_SERVER_IDENTIFIER )
		      || ( record_entry_value_type == LIBPFF_VALUE_TYPE_RESTRICTION )
		      || ( record_entry_value_type == LIBPFF_VALUE_TYPE_RULE_ACTION )
		      || ( record_entry_value_type == LIBPFF_VALUE_TYPE_BINARY_DATA )
		      || ( record_entry_value_type == LIBPFF_VALUE_TYPE_MULTI_VALUE_INTEGER_16BIT_SIGNED )
		      || ( record_entry_value_type == LIBPFF_VALUE_TYPE_MULTI_VALUE_INTEGER_32BIT_SIGNED )
		      || ( record_entry_value_type == LIBPFF_VALUE_TYPE_MULTI_VALUE_FLOAT_32BIT )
		      || ( record_entry_value_type == LIBPFF_VALUE_TYPE_MULTI_VALUE_DOUBLE_64BIT )
		      || ( record_entry_value_type == LIBPFF_VALUE_TYPE_MULTI_VALUE_CURRENCY )
		      || ( record_entry_value_type == LIBPFF_VALUE_TYPE_MULTI_VALUE_APPLICATION_TIME )
		      || ( record_entry_value_type == LIBPFF_VALUE_TYPE_MULTI_VALUE_INTEGER_64BIT_SIGNED )
		      || ( record_entry_value_type == LIBPFF_VALUE_TYPE_MULTI_VALUE_STRING_ASCII )
		      || ( record_entry_value_type == LIBPFF_VALUE_TYPE_MULTI_VALUE_STRING_UNICODE )
		      || ( record_entry_value_type == LIBPFF_VALUE_TYPE_MULTI_VALUE_FILETIME )
		      || ( record_entry_value_type == LIBPFF_VALUE_TYPE_MULTI_VALUE_GUID )
		      || ( record_entry_value_type == LIBPFF_VALUE_TYPE_MULTI_VALUE_BINARY_DATA ) )
		{
			/* Check if the entry value is a referenced local descriptor
			 */
			/* TODO check entry value type */
			if( ( entry_value & 0x0000001fUL ) != 0 )
			{
#if defined( HAVE_DEBUG_OUTPUT )
				if( libcnotify_verbose != 0 )
				{
					libcnotify_printf(
					 "%s: table set: %03" PRIu32 " entry: %03" PRIu32 " record entry value reference\t\t: %" PRIu64 " ",
					 function,
					 set_index,
					 entry_index,
					 entry_value );
					libpff_debug_print_node_identifier_type(
					 (uint8_t) ( entry_value & 0x0000001fUL ) );
					libcnotify_printf(
					 "\n" );
				}
#endif
				if( entry_value > (uint64_t) UINT32_MAX )
				{
					libcerror_error_set(
					 error,
					 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
					 LIBCERROR_ARGUMENT_ERROR_VALUE_EXCEEDS_MAXIMUM,
					 "%s: entry value reference value exceeds maximum.",
					 function );

					return( -1 );
				}
				result = libpff_local_descriptors_tree_get_value_by_identifier(
					  table->local_descriptors_tree,
					  file_io_handle,
					  table->local_descriptors_cache,
					  (uint32_t) entry_value,
					  &local_descriptor_value,
					  error );

				if( result == -1 )
				{
					libcerror_error_set(
					 error,
					 LIBCERROR_ERROR_DOMAIN_RUNTIME,
					 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
					 "%s: unable to retrieve descriptor identifier: %" PRIu32 " from local descriptors.",
					 function,
					 (uint32_t) entry_value );

					return( -1 );
				}
				else if( result == 0 )
				{
#if defined( HAVE_DEBUG_OUTPUT )
					if( libcnotify_verbose != 0 )
					{
						libcnotify_printf(
						 "%s: missing descriptor: %" PRIu32 " - marked as missing.\n",
						 function,
						 (uint32_t) entry_value );
					}
#endif
					( record_entry[ entry_index ] ).value_data_reference = NULL;
					( record_entry[ entry_index ] ).flags               |= LIBPFF_RECORD_ENTRY_FLAG_MISSING_DATA_DESCRIPTOR;
					table->flags                                        |= LIBPFF_TABLE_FLAG_MISSING_RECORD_ENTRY_DATA;
				}
				else
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
						 "%s: identifier: %" PRIu64 " ",
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
					/* TODO handle multiple recovered offset index values */
					if( libpff_io_handle_read_descriptor_data_reference(
					     io_handle,
					     file_io_handle,
					     (uint32_t) entry_value,
					     local_descriptor_value->data_identifier,
					     table->recovered,
					     0,
					     &( ( record_entry[ entry_index ] ).value_data_reference ),
					     &( ( record_entry[ entry_index ] ).value_data_cache ),
					     error ) != 1 )
					{
						libcerror_error_set(
						 error,
						 LIBCERROR_ERROR_DOMAIN_IO,
						 LIBCERROR_IO_ERROR_READ_FAILED,
						 "%s: unable to read record entry value data with descriptor: %" PRIu32 " - marked as missing.",
						 function,
						 (uint32_t) entry_value );

#if defined( HAVE_DEBUG_OUTPUT )
						if( ( libcnotify_verbose != 0 )
						 && ( error != NULL )
						 && ( *error != NULL ) )
						{
							libcnotify_print_error_backtrace(
							 *error );
						}
#endif
						libcerror_error_free(
						 error );

						/* If the data descriptor could not be read mark it as missing
						 * and give it an empty value data reference
						 */
						( record_entry[ entry_index ] ).value_data_reference = NULL;
						( record_entry[ entry_index ] ).flags               |= LIBPFF_RECORD_ENTRY_FLAG_MISSING_DATA_DESCRIPTOR;
						table->flags                                        |= LIBPFF_TABLE_FLAG_MISSING_RECORD_ENTRY_DATA;
					}
				}
			}
			/* Check if the entry value is empty
			 */
			else if( entry_value == 0 )
			{
#if defined( HAVE_DEBUG_OUTPUT )
				if( libcnotify_verbose != 0 )
				{
					libcnotify_printf(
					 "%s: table set: %03" PRIu32 " entry: %03" PRIu32 " record entry value\t\t\t: <NULL>\n",
					 function,
					 set_index,
					 entry_index );
				}
#endif
			}
			/* Otherwise the entry value is a referenced table value
			 */
			else
			{
				/* Fetch the record entry value reference
				 */
				if( libpff_table_get_index_value_by_reference(
				     table,
				     (uint32_t) entry_value,
				     &table_index_value,
				     error ) != 1 )
				{
					libcerror_error_set(
					 error,
					 LIBCERROR_ERROR_DOMAIN_RUNTIME,
					 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
					 "%s: unable to retrieve record entry value reference: 0x%08" PRIx64 ".",
					 function,
					 entry_value );

#if defined( HAVE_DEBUG_OUTPUT )
					if( ( libcnotify_verbose != 0 )
					 && ( error != NULL )
					 && ( *error != NULL ) )
					{
						libcnotify_print_error_backtrace(
						 *error );
					}
#endif
					libcerror_error_free(
					 error );

					( record_entry[ entry_index ] ).value_data_reference = NULL;
					( record_entry[ entry_index ] ).flags               |= LIBPFF_RECORD_ENTRY_FLAG_MISSING_DATA_DESCRIPTOR;
					table->flags                                        |= LIBPFF_TABLE_FLAG_MISSING_RECORD_ENTRY_DATA;
				}
				else
				{
					if( table_index_value == NULL )
					{
						libcerror_error_set(
						 error,
						 LIBCERROR_ERROR_DOMAIN_RUNTIME,
						 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
						 "%s: missing table index value.",
						 function );

						return( -1 );
					}
					/* Retrieve the corresponding table array entry
					 */
					if( libfdata_block_get_segment_data(
					     table->descriptor_data_block,
					     file_io_handle,
					     table->descriptor_data_cache,
					     (int) table_index_value->array_entry,
					     &table_data,
					     &table_data_size,
					     0,
					     error ) != 1 )
					{
						libcerror_error_set(
						 error,
						 LIBCERROR_ERROR_DOMAIN_RUNTIME,
						 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
						 "%s: unable to retrieve table array entry: %" PRIu32 ".",
						 function,
						 table_index_value->array_entry );

						return( -1 );
					}
					if( table_data == NULL )
					{
						libcerror_error_set(
						 error,
						 LIBCERROR_ERROR_DOMAIN_RUNTIME,
						 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
						 "%s: missing table array entry: %" PRIu32 ".",
						 function,
						 table_index_value->array_entry );

						return( -1 );
					}
					if( (size_t) table_index_value->offset >= table_data_size )
					{
						libcerror_error_set(
						 error,
						 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
						 LIBCERROR_ARGUMENT_ERROR_VALUE_OUT_OF_BOUNDS,
						 "%s: table value offset exceeds table data size.",
						 function );

						return( -1 );
					}
					record_entry_value_data      = &( table_data[ table_index_value->offset ] );
					record_entry_value_data_size = table_index_value->size;
				}
			}
		}
		else
		{
#if defined( HAVE_DEBUG_OUTPUT )
			if( libcnotify_verbose != 0 )
			{
				libcnotify_printf(
				 "%s: table set: %03" PRIu32 " entry: %03" PRIu32 " record entry value (reference)\t: 0x%08" PRIx64 "\n",
				 function,
				 set_index,
				 entry_index,
				 entry_value );
				libcnotify_printf(
				 "\n" );
			}
#endif
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
			 "%s: unsupported value type: 0x%08" PRIx32 " with value size: %" PRIu8 ".",
			 function,
			 record_entry_value_type,
			 record_entry_value_size );

			return( -1 );
		}
	}
	if( ( record_entry[ entry_index ] ).value_data_reference == NULL )
	{
		/* Create a buffer data reference of the record entry value data
		 */
		if( libfdata_buffer_reference_initialize(
		     &( ( record_entry[ entry_index ] ).value_data_reference ),
		     record_entry_value_data,
		     record_entry_value_data_size,
		     LIBFDATA_BUFFER_DATA_FLAG_NON_MANAGED,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
			 "%s: unable to create record entry value data reference.",
			 function );

			return( -1 );
		}
	}
#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		if( libpff_debug_property_type_value_reference_print(
		     name_to_id_map_list,
		     ( record_entry[ entry_index ] ).identifier.entry_type,
		     ( record_entry[ entry_index ] ).identifier.value_type,
		     ( record_entry[ entry_index ] ).value_data_reference,
		     file_io_handle,
		     ( record_entry[ entry_index ] ).value_data_cache,
		     debug_item_type,
		     io_handle->ascii_codepage,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_PRINT_FAILED,
			 "%s: unable to print record entry value reference.",
			 function );

			return( -1 );
		}
	}
#endif
	return( 1 );
}

