/*
 * Tree type functions
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

#if !defined( _LIBPFF_TREE_TYPE_H )
#define _LIBPFF_TREE_TYPE_H

#include <common.h>
#include <types.h>

#include "libpff_libcerror.h"
#include "libpff_list_type.h"

#if defined( __cplusplus )
extern "C" {
#endif

/* The tree node comparison definitions
 */
enum LIBPFF_TREE_NODE_COMPARE_DEFINITIONS
{
	/* The first value is less than the second value
	 */
        LIBPFF_TREE_NODE_COMPARE_LESS,

	/* The first and second values are equal
	 */
        LIBPFF_TREE_NODE_COMPARE_EQUAL,

	/* The first value is greater than the second value
	 */
        LIBPFF_TREE_NODE_COMPARE_GREATER
};

/* The tree node insert flag definitions
 */
enum LIBPFF_TREE_NODE_INSERT_FLAGS
{
	/* Allow duplicate entries
	 */
	LIBPFF_TREE_NODE_INSERT_FLAG_NON_UNIQUE_ENTRIES	= 0x00,

	/* Only allow unique entries, no duplicates
	 */
	LIBPFF_TREE_NODE_INSERT_FLAG_UNIQUE_ENTRIES	= 0x01,
};

typedef struct libpff_tree_node libpff_tree_node_t;

struct libpff_tree_node
{
	/* The parent node
	 */
	libpff_tree_node_t *parent_node;

	/* The previous node list
	 */
	libpff_tree_node_t *previous_node;

	/* The next node list
	 */
	libpff_tree_node_t *next_node;

	/* The first sub node
	 */
	libpff_tree_node_t *first_sub_node;

	/* The last sub node
	 */
	libpff_tree_node_t *last_sub_node;

	/* The number of sub nodes
	 */
	int number_of_sub_nodes;

	/* The node value
	 */
	intptr_t *value;
};

int libpff_tree_node_initialize(
     libpff_tree_node_t **node,
     libcerror_error_t **error );

int libpff_tree_node_free(
     libpff_tree_node_t **node,
     int (*value_free_function)(
            intptr_t **value,
            libcerror_error_t **error ),
     libcerror_error_t **error );

int libpff_tree_node_clone(
     libpff_tree_node_t **destination_node,
     libpff_tree_node_t *source_node,
     int (*value_free_function)(
            intptr_t **value,
            libcerror_error_t **error ),
     int (*value_clone_function)(
            intptr_t **destination,
            intptr_t *source,
            libcerror_error_t **error ),
     libcerror_error_t **error );

int libpff_tree_node_get_value(
     libpff_tree_node_t *node,
     intptr_t **value,
     libcerror_error_t **error );

int libpff_tree_node_set_value(
     libpff_tree_node_t *node,
     intptr_t *value,
     libcerror_error_t **error );

int libpff_tree_node_append_node(
     libpff_tree_node_t *parent_node,
     libpff_tree_node_t *node,
     libcerror_error_t **error );

int libpff_tree_node_append_value(
     libpff_tree_node_t *parent_node,
     intptr_t *value,
     libcerror_error_t **error );

int libpff_tree_node_insert_node(
     libpff_tree_node_t *parent_node,
     libpff_tree_node_t *node,
     int (*value_compare_function)(
            intptr_t *first_value,
            intptr_t *second_value,
            libcerror_error_t **error ),
     uint8_t insert_flags,
     libcerror_error_t **error );

int libpff_tree_node_insert_value(
     libpff_tree_node_t *parent_node,
     intptr_t *value,
     int (*value_compare_function)(
            intptr_t *first_value,
            intptr_t *second_value,
            libcerror_error_t **error ),
     uint8_t insert_flags,
     libcerror_error_t **error );

int libpff_tree_node_remove_node(
     libpff_tree_node_t *parent_node,
     libpff_tree_node_t *node,
     libcerror_error_t **error );

int libpff_tree_node_get_number_of_sub_nodes(
     libpff_tree_node_t *node,
     int *number_of_sub_nodes,
     libcerror_error_t **error );

int libpff_tree_node_get_sub_node_by_index(
     libpff_tree_node_t *node,
     int sub_node_index,
     libpff_tree_node_t **sub_node,
     libcerror_error_t **error );

int libpff_tree_node_get_leaf_node_list(
     libpff_tree_node_t *node,
     libpff_list_t **leaf_node_list,
     libcerror_error_t **error );

#if defined( __cplusplus )
}
#endif

#endif

