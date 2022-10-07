/*
 * Record functions
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

#if !defined( _LIBPFF_RECORD_H )
#define _LIBPFF_RECORD_H

#include <common.h>
#include <types.h>

#include "libpff_libfcache.h"
#include "libpff_libfdata.h"
#include "libpff_name_to_id_map.h"
#include "libpff_types.h"

#if defined( __cplusplus )
extern "C" {
#endif

enum LIBPFF_RECORD_ENTRY_IDENTIFIER_FORMATS
{
	/* The entry identifier consists of a MAPI property type
	 * (entry type, value type)
	 */
	LIBPFF_RECORD_ENTRY_IDENTIFIER_FORMAT_MAPI_PROPERTY	= 1,

	/* The entry identifier consists of a GUID
	 */
	LIBPFF_RECORD_ENTRY_IDENTIFIER_FORMAT_GUID		= 2,

	/* The entry identifier consists of a PRQ_ID_SECURE4 value
	 * the PRQ_ID_SECURE4 value is a yet unknown 10-byte value
	 */
	LIBPFF_RECORD_ENTRY_IDENTIFIER_FORMAT_SECURE4		= 3
};

enum LIBPFF_RECORD_ENTRY_FLAGS
{
	/* The data descriptor could not be read
	 */
	LIBPFF_RECORD_ENTRY_FLAG_MISSING_DATA_DESCRIPTOR	= 0x01,
};

typedef struct libpff_record_entry_identifier libpff_record_entry_identifier_t;

struct libpff_record_entry_identifier
{
	/* The entry identifier format
	 */
	uint8_t format;

	/* The entry identifier
	 */
	union
	{
		/* The MAPI property based entry identifier
		 */
		struct
		{
			/* The entry type
			 */
			uint32_t entry_type;

			/* The value type
			 */
			uint32_t value_type;
		};
		/* The GUID based entry identifier
		 */
		uint8_t guid[ 16 ];

		/* The PRQ_ID_SECURE4 based entry identifier
		 */
		uint64_t secure4;
	};
};

typedef struct libpff_record_entry libpff_record_entry_t;

struct libpff_record_entry
{
	/* The record entry identifier
	 */
	libpff_record_entry_identifier_t identifier;

	/* The value data reference
	 */
	libfdata_reference_t *value_data_reference;

	/* The value data cache
	 */
	libfcache_cache_t *value_data_cache;

	/* The name to id map entry
	 */
	libpff_internal_name_to_id_map_entry_t *name_to_id_map_entry;

	/* The flags
	 */
	uint8_t flags;
};

#if defined( __cplusplus )
}
#endif

#endif

