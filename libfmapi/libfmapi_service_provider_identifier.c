/*
 * Sevice provider functions
 *
 * Copyright (c) 2009-2012, Joachim Metz <joachim.metz@gmail.com>
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

#include "libfmapi_class_identifier.h"

uint8_t libfmapi_service_provider_identifier_one_off_entry_identifier[ 16 ] = \
	{ 0x81, 0x2b, 0x1f, 0xa4, 0xbe, 0xa3, 0x10, 0x19, 0x9d, 0x6e, 0x00, 0xdd, 0x01, 0x0f, 0x54, 0x02 };

uint8_t libfmapi_service_provider_identifier_x500_address[ 16 ] = \
	{ 0xdc, 0xa7, 0x40, 0xc8, 0xc0, 0x42, 0x10, 0x1a, 0xb4, 0xb9, 0x08, 0x00, 0x2b, 0x2f, 0xe1, 0x82 };

#if defined( HAVE_DEBUG_OUTPUT )

/* The service provider identifiers
 */
libfmapi_class_identifier_definition_t libfmapi_service_provider_identifier_definitions[ ] = {
	{ libfmapi_service_provider_identifier_one_off_entry_identifier,	"One-off entry identifier" },
	{ libfmapi_service_provider_identifier_x500_address,			"X500 address" },

	{ libfmapi_class_identifier_unknown,					"Unknown" } };

#endif

#if defined( HAVE_DEBUG_OUTPUT )

/* Retrieves a string containing the name of the service provider identifier
 */
const char *libfmapi_service_provider_identifier_get_name(
             uint8_t *service_provider_identifier )
{
	int iterator = 0;

	if( service_provider_identifier == NULL )
	{
		return( "Invalid service provider identifier" );
	}
	while( memory_compare(
	        ( libfmapi_service_provider_identifier_definitions[ iterator ] ).identifier,
	        libfmapi_class_identifier_unknown,
	        16 ) != 0 )
	{
		if( memory_compare(
		     ( libfmapi_service_provider_identifier_definitions[ iterator ] ).identifier,
		     service_provider_identifier,
		     16 ) == 0 )
		{
			break;
		}
		iterator++;
	}
	return(
	 ( libfmapi_service_provider_identifier_definitions[ iterator ] ).name );
}

#endif

