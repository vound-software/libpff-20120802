/*
 * Python object definition of the libpff item
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

#if defined( HAVE_STDLIB_H ) || defined( HAVE_WINAPI )
#include <stdlib.h>
#endif

#include "pypff.h"
#include "pypff_item.h"
#include "pypff_libcerror.h"
#include "pypff_libcstring.h"
#include "pypff_libpff.h"
#include "pypff_python.h"

PyMethodDef pypff_item_object_methods[] = {

	/* Functions to access the item values */

	{ "get_display_name",
	  (PyCFunction) pypff_item_get_display_name,
	  METH_NOARGS,
	  "Retrieves the display name" },

	/* Functions to access the sub items */

	{ "get_number_of_sub_items",
	  (PyCFunction) pypff_item_get_number_of_sub_items,
	  METH_NOARGS,
	  "Retrieves the number of sub items" },

	{ "get_sub_item",
	  (PyCFunction) pypff_item_get_sub_item,
	  METH_VARARGS | METH_KEYWORDS,
	  "Retrieves a specific sub item" },

	/* Sentinel */
	{ NULL, NULL, 0, NULL }
};

PyTypeObject pypff_item_type_object = {
	PyObject_HEAD_INIT( NULL )

	/* ob_size */
	0,
	/* tp_name */
	"pypff.item",
	/* tp_basicsize */
	sizeof( pypff_item_t ),
	/* tp_itemsize */
	0,
	/* tp_dealloc */
	(destructor) pypff_item_free,
	/* tp_print */
	0,
	/* tp_getattr */
	0,
	/* tp_setattr */
	0,
	/* tp_compare */
	0,
	/* tp_repr */
	0,
	/* tp_as_number */
	0,
	/* tp_as_sequence */
	0,
	/* tp_as_mapping */
	0,
	/* tp_hash */
	0,
	/* tp_call */
	0,
	/* tp_str */
	0,
	/* tp_getattro */
	0,
	/* tp_setattro */
	0,
	/* tp_as_buffer */
	0,
        /* tp_flags */
	Py_TPFLAGS_DEFAULT,
	/* tp_doc */
	"pypff item object (wraps libpff_item_t)",
	/* tp_traverse */
	0,
	/* tp_clear */
	0,
	/* tp_richcompare */
	0,
	/* tp_weaklistoffset */
	0,
	/* tp_iter */
	0,
	/* tp_iternext */
	0,
	/* tp_methods */
	pypff_item_object_methods,
	/* tp_members */
	0,
	/* tp_getset */
	0,
	/* tp_base */
	0,
	/* tp_dict */
	0,
	/* tp_descr_get */
	0,
	/* tp_descr_set */
	0,
	/* tp_dictoffset */
	0,
	/* tp_init */
	(initproc) pypff_item_init,
	/* tp_alloc */
	0,
	/* tp_new */
	0,
	/* tp_free */
	0,
	/* tp_is_gc */
	0,
	/* tp_bases */
	NULL,
	/* tp_mro */
	NULL,
	/* tp_cache */
	NULL,
	/* tp_subclasses */
	NULL,
	/* tp_weaklist */
	NULL,
	/* tp_del */
	0
};

/* Creates a new item object
 * Returns a Python object if successful or NULL on error
 */
PyObject *pypff_item_new(
           PyObject *self )
{
	pypff_item_t *pypff_item = NULL;
	static char *function    = "pypff_item_new";

	pypff_item = PyObject_New(
	              struct pypff_item,
	              &pypff_item_type_object );

	if( pypff_item == NULL )
	{
		PyErr_Format(
		 PyExc_MemoryError,
		 "%s: unable to initialize item.",
		 function );

		return( NULL );
	}
	if( pypff_item_init(
	     pypff_item ) != 0 )
	{
		PyErr_Format(
		 PyExc_MemoryError,
		 "%s: unable to initialize item.",
		 function );

		goto on_error;
	}
	return( (PyObject *) pypff_item );

on_error:
	if( pypff_item != NULL )
	{
		Py_DecRef(
		 (PyObject *) pypff_item );
	}
	return( NULL );
}

/* Intializes an item object
 * Returns 0 if successful or -1 on error
 */
int pypff_item_init(
     pypff_item_t *pypff_item )
{
	static char *function = "pypff_item_init";

	if( pypff_item == NULL )
	{
		PyErr_Format(
		 PyExc_TypeError,
		 "%s: invalid item.",
		 function );

		return( -1 );
	}
	/* Make sure libpff item is set to NULL
	 */
	pypff_item->item = NULL;

	return( 0 );
}

/* Frees an item object
 */
void pypff_item_free(
      pypff_item_t *pypff_item )
{
	char error_string[ PYPFF_ERROR_STRING_SIZE ];

	libcerror_error_t *error = NULL;
	static char *function   = "pypff_item_free";

	if( pypff_item == NULL )
	{
		PyErr_Format(
		 PyExc_TypeError,
		 "%s: invalid item.",
		 function );

		return;
	}
	if( pypff_item->ob_type == NULL )
	{
		PyErr_Format(
		 PyExc_TypeError,
		 "%s: invalid item - missing ob_type.",
		 function );

		return;
	}
	if( pypff_item->ob_type->tp_free == NULL )
	{
		PyErr_Format(
		 PyExc_TypeError,
		 "%s: invalid item - invalid ob_type - missing tp_free.",
		 function );

		return;
	}
	if( pypff_item->item == NULL )
	{
		PyErr_Format(
		 PyExc_TypeError,
		 "%s: invalid item - missing libpff item.",
		 function );

		return;
	}
	if( libpff_item_free(
	     &( pypff_item->item ),
	     &error ) != 1 )
	{
		if( libcerror_error_backtrace_sprint(
		     error,
		     error_string,
		     PYPFF_ERROR_STRING_SIZE ) == -1 )
                {
			PyErr_Format(
			 PyExc_IOError,
			 "%s: unable to free libpff item.",
			 function );
		}
		else
                {
			PyErr_Format(
			 PyExc_IOError,
			 "%s: unable to free libpff item.\n%s",
			 function,
			 error_string );
		}
		libcerror_error_free(
		 &error );
	}
	pypff_item->ob_type->tp_free(
	 (PyObject*) pypff_item );
}

/* Retrieves the display name
 * Returns a Python object if successful or NULL on error
 */
PyObject *pypff_item_get_display_name(
           pypff_item_t *pypff_item )
{
	char error_string[ PYPFF_ERROR_STRING_SIZE ];

	libcerror_error_t *error  = NULL;
	PyObject *string_object  = NULL;
	const char *errors       = NULL;
	uint8_t *value_string    = NULL;
	static char *function    = "pypff_item_get_display_name";
	size_t value_string_size = 0;
	int result               = 0;

	if( pypff_item == NULL )
	{
		PyErr_Format(
		 PyExc_TypeError,
		 "%s: invalid item.",
		 function );

		return( NULL );
	}
	result = libpff_item_get_entry_value_utf8_string_size(
	          pypff_item->item,
	          0,
	          LIBPFF_ENTRY_TYPE_DISPLAY_NAME,
	          &value_string_size,
	          0,
	          &error );

	if( result == -1 )
	{
		if( libcerror_error_backtrace_sprint(
		     error,
		     error_string,
		     PYPFF_ERROR_STRING_SIZE ) == -1 )
                {
			PyErr_Format(
			 PyExc_IOError,
			 "%s: unable to retrieve display name size.",
			 function );
		}
		else
                {
			PyErr_Format(
			 PyExc_IOError,
			 "%s: unable to retrieve display name size.\n%s",
			 function,
			 error_string );
		}
		libcerror_error_free(
		 &error );

		goto on_error;
	}
	else if( ( result == 0 )
	      || ( value_string_size == 0 ) )
	{
		Py_IncRef(
		 Py_None );

		return( Py_None );
	}
	value_string = (uint8_t *) memory_allocate(
				    sizeof( uint8_t ) * value_string_size );

	if( value_string == NULL )
	{
		PyErr_Format(
		 PyExc_IOError,
		 "%s: unable to create display name.",
		 function );

		goto on_error;
	}
	result = libpff_item_get_entry_value_utf8_string(
		  pypff_item->item,
		  0,
		  LIBPFF_ENTRY_TYPE_DISPLAY_NAME,
		  value_string,
		  value_string_size,
		  0,
		  &error );

	if( result != 1 )
	{
		if( libcerror_error_backtrace_sprint(
		     error,
		     error_string,
		     PYPFF_ERROR_STRING_SIZE ) == -1 )
                {
			PyErr_Format(
			 PyExc_IOError,
			 "%s: unable to retrieve display name.",
			 function );
		}
		else
                {
			PyErr_Format(
			 PyExc_IOError,
			 "%s: unable to retrieve display name.\n%s",
			 function,
			 error_string );
		}
		libcerror_error_free(
		 &error );

		goto on_error;
	}
	/* Pass the string length to PyUnicode_DecodeUTF8
	 * otherwise it makes the end of string character is part
	 * of the string
	 */
	string_object = PyUnicode_DecodeUTF8(
			 (char *) value_string,
			 (Py_ssize_t) value_string_size - 1,
			 errors );

	memory_free(
	 value_string );

	return( string_object );

on_error:
	if( value_string != NULL )
	{
		memory_free(
		 value_string );
	}
	return( NULL );
}

/* Retrieves the number of sub items
 * Returns a Python object if successful or NULL on error
 */
PyObject *pypff_item_get_number_of_sub_items(
           pypff_item_t *pypff_item )
{
	char error_string[ PYPFF_ERROR_STRING_SIZE ];

	libcerror_error_t *error = NULL;
	static char *function   = "pypff_item_get_number_of_sub_items";
	int number_of_sub_items = 0;

	if( pypff_item == NULL )
	{
		PyErr_Format(
		 PyExc_TypeError,
		 "%s: invalid item.",
		 function );

		return( NULL );
	}
	if( libpff_item_get_number_of_sub_items(
	     pypff_item->item,
	     &number_of_sub_items,
	     &error ) != 1 )
	{
		if( libcerror_error_backtrace_sprint(
		     error,
		     error_string,
		     PYPFF_ERROR_STRING_SIZE ) == -1 )
                {
			PyErr_Format(
			 PyExc_IOError,
			 "%s: unable to retrieve number of sub items.",
			 function );
		}
		else
		{
			PyErr_Format(
			 PyExc_IOError,
			 "%s: unable to retrieve number of sub items.\n%s",
			 function,
			 error_string );
		}
		libcerror_error_free(
		 &error );

		return( NULL );
	}
	return( PyInt_FromLong(
	         (long) number_of_sub_items ) );
}

/* Retrieves a specific sub item
 * Returns a Python object if successful or NULL on error
 */
PyObject *pypff_item_get_sub_item(
           pypff_item_t *pypff_item,
           PyObject *arguments,
           PyObject *keywords )
{
	char error_string[ PYPFF_ERROR_STRING_SIZE ];

	libcerror_error_t *error     = NULL;
	libpff_item_t *sub_item     = NULL;
	PyObject *item_object       = NULL;
	static char *keyword_list[] = { "sub_item_index", NULL };
	static char *function       = "pypff_item_get_sub_item";
	int sub_item_index          = 0;

	if( pypff_item == NULL )
	{
		PyErr_Format(
		 PyExc_TypeError,
		 "%s: invalid item.",
		 function );

		return( NULL );
	}
	if( PyArg_ParseTupleAndKeywords(
	     arguments,
	     keywords,
	     "i",
	     keyword_list,
	     &sub_item_index ) == 0 )
        {
		goto on_error;
        }
	if( libpff_item_get_sub_item(
	     pypff_item->item,
	     sub_item_index,
	     &sub_item,
	     &error ) != 1 )
	{
		if( libcerror_error_backtrace_sprint(
		     error,
		     error_string,
		     PYPFF_ERROR_STRING_SIZE ) == -1 )
                {
			PyErr_Format(
			 PyExc_IOError,
			 "%s: unable to retrieve sub item: %d.",
			 function,
			 sub_item_index );
		}
		else
		{
			PyErr_Format(
			 PyExc_IOError,
			 "%s: unable to retrieve sub item: %d.\n%s",
			 function,
			 sub_item_index,
			 error_string );
		}
		libcerror_error_free(
		 &error );

		goto on_error;
	}
	item_object = pypff_item_new(
	               NULL );

	if( item_object == NULL )
	{
		PyErr_Format(
		 PyExc_MemoryError,
		 "%s: unable to create item object.",
		 function );

		goto on_error;
	}
	( (pypff_item_t *) item_object )->item = sub_item;

	return( item_object );

on_error:
	if( sub_item != NULL )
	{
		libpff_item_free(
		 &sub_item,
		 NULL );
	}
	return( NULL );
}

