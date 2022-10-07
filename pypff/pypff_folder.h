/*
 * Python object definition of the folder type libpff item
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

#if !defined( _PYPFF_FOLDER_H )
#define _PYPFF_FOLDER_H

#include <common.h>
#include <types.h>

#include "pypff_libcerror.h"
#include "pypff_libpff.h"
#include "pypff_python.h"

#if defined( __cplusplus )
extern "C" {
#endif

typedef struct pypff_folder pypff_folder_t;

struct pypff_folder
{
	/* Python object initialization
	 */
	PyObject_HEAD

	/* The folder type libpff item
	 */
	libpff_item_t *item;
};

extern PyMethodDef pypff_folder_object_methods[];
extern PyTypeObject pypff_folder_type_object;

PyObject *pypff_folder_new(
           PyObject *self );

int pypff_folder_init(
     pypff_folder_t *pypff_folder );

void pypff_folder_free(
      pypff_folder_t *pypff_folder );

PyObject *pypff_folder_get_display_name(
           pypff_folder_t *pypff_folder );

PyObject *pypff_folder_get_number_of_sub_folders(
           pypff_folder_t *pypff_folder );

PyObject *pypff_folder_get_sub_folder(
           pypff_folder_t *pypff_folder,
           PyObject *arguments,
           PyObject *keywords );

PyObject *pypff_folder_get_number_of_sub_messages(
           pypff_folder_t *pypff_folder );

PyObject *pypff_folder_get_sub_message(
           pypff_folder_t *pypff_folder,
           PyObject *arguments,
           PyObject *keywords );

#if defined( __cplusplus )
}
#endif

#endif

