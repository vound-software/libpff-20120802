/*
 * The block definition of a Personal Folder File
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

#if !defined( _PFF_BLOCK_H )
#define _PFF_BLOCK_H

#include <common.h>
#include <types.h>

#if defined( __cplusplus )
extern "C" {
#endif

typedef struct pff_block_footer_32bit pff_block_footer_32bit_t;

struct pff_block_footer_32bit
{
	/* The block size
	 * Consists of 2 bytes
	 */
	uint8_t size[ 2 ];

	/* The block signature
	 * Consists of 2 bytes
	 */
	uint8_t signature[ 2 ];

	/* The block back pointer
	 * Consists of 4 bytes
	 */
	uint8_t back_pointer[ 4 ];

	/* The block data crc
	 * Consists of 4 bytes
	 */
	uint8_t crc[ 4 ];
};

typedef struct pff_block_footer_64bit pff_block_footer_64bit_t;

struct pff_block_footer_64bit
{
	/* The block size
	 * Consists of 2 bytes
	 */
	uint8_t size[ 2 ];

	/* The block signature
	 * Consists of 2 bytes
	 */
	uint8_t signature[ 2 ];

	/* The block data crc
	 * Consists of 4 bytes
	 */
	uint8_t crc[ 4 ];

	/* The block back pointer
	 * Consists of 8 bytes
	 */
	uint8_t back_pointer[ 8 ];
};

#if defined( __cplusplus )
}
#endif

#endif

