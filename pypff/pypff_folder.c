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

#include <common.h>
#include <memory.h>
#include <types.h>

#if defined( HAVE_STDLIB_H ) || defined( HAVE_WINAPI )
#include <stdlib.h>
#endif

#include "pypff.h"
#include "pypff_folder.h"
#include "pypff_libcerror.h"
#include "pypff_libcstring.h"
#include "pypff_libpff.h"
#include "pypff_message.h"
#include "pypff_python.h"

PyMethodDef pypff_folder_object_methods[] = {

	/* Functions to access the folder values */

	{ "get_name",
	  (PyCFunction) pypff_folder_get_display_name,
	  METH_NOARGS,
	  "Retrieves the name" },

	{ "get_display_name",
	  (PyCFunction) pypff_folder_get_display_name,
	  METH_NOARGS,
	  "Retrieves the display name" },

	/* Functions to access the sub folders */

	{ "get_number_of_sub_folders",
	  (PyCFunction) pypff_folder_get_number_of_sub_folders,
	  METH_NOARGS,
	  "Retrieves the number of sub folders" },

	{ "get_sub_folder",
	  (PyCFunction) pypff_folder_get_sub_folder,
	  METH_VARARGS | METH_KEYWORDS,
	  "Retrieves a specific sub folder" },

	/* Functions to access the sub messages */

	{ "get_number_of_sub_messages",
	  (PyCFunction) pypff_folder_get_number_of_sub_messages,
	  METH_NOARGS,
	  "Retrieves the number of sub messages" },

	{ "get_sub_message",
	  (PyCFunction) pypff_folder_get_sub_message,
	  METH_VARARGS | METH_KEYWORDS,
	  "Retrieves a specific sub message" },

	/* Sentinel */
	{ NULL, NULL, 0, NULL }
};

PyTypeObject pypff_folder_type_object = {
	PyObject_HEAD_INIT( NULL )

	/* ob_size */
	0,
	/* tp_name */
	"pypff.folder",
	/* tp_basicsize */
	sizeof( pypff_folder_t ),
	/* tp_foldersize */
	0,
	/* tp_dealloc */
	(destructor) pypff_folder_free,
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
	"pypff folder object (wraps folder type libpff_item_t)",
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
	pypff_folder_object_methods,
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
	(initproc) pypff_folder_init,
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

/* Creates a new folder object
 * Returns a Python object if successful or NULL on error
 */
PyObject *pypff_folder_new(
           PyObject *self )
{
	pypff_folder_t *pypff_folder = NULL;
	static char *function        = "pypff_folder_new";

	pypff_folder = PyObject_New(
	                struct pypff_folder,
	                &pypff_folder_type_object );

	if( pypff_folder == NULL )
	{
		PyErr_Format(
		 PyExc_MemoryError,
		 "%s: unable to initialize folder.",
		 function );

		return( NULL );
	}
	if( pypff_folder_init(
	     pypff_folder ) != 0 )
	{
		PyErr_Format(
		 PyExc_MemoryError,
		 "%s: unable to initialize folder.",
		 function );

		goto on_error;
	}
	return( (PyObject *) pypff_folder );

on_error:
	if( pypff_folder != NULL )
	{
		Py_DecRef(
		 (PyObject *) pypff_folder );
	}
	return( NULL );
}

/* Intializes a folder object
 * Returns 0 if successful or -1 on error
 */
int pypff_folder_init(
     pypff_folder_t *pypff_folder )
{
	static char *function = "pypff_folder_init";

	if( pypff_folder == NULL )
	{
		PyErr_Format(
		 PyExc_TypeError,
		 "%s: invalid folder.",
		 function );

		return( -1 );
	}
	/* Make sure libpff item is set to NULL
	 */
	pypff_folder->item = NULL;

	return( 0 );
}

/* Frees a folder object
 */
void pypff_folder_free(
      pypff_folder_t *pypff_folder )
{
	char error_string[ PYPFF_ERROR_STRING_SIZE ];

	libcerror_error_t *error = NULL;
	static char *function   = "pypff_folder_free";

	if( pypff_folder == NULL )
	{
		PyErr_Format(
		 PyExc_TypeError,
		 "%s: invalid folder.",
		 function );

		return;
	}
	if( pypff_folder->ob_type == NULL )
	{
		PyErr_Format(
		 PyExc_TypeError,
		 "%s: invalid folder - missing ob_type.",
		 function );

		return;
	}
	if( pypff_folder->ob_type->tp_free == NULL )
	{
		PyErr_Format(
		 PyExc_TypeError,
		 "%s: invalid folder - invalid ob_type - missing tp_free.",
		 function );

		return;
	}
	if( pypff_folder->item == NULL )
	{
		PyErr_Format(
		 PyExc_TypeError,
		 "%s: invalid folder - missing libpff folder.",
		 function );

		return;
	}
	if( libpff_item_free(
	     &( pypff_folder->item ),
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
	pypff_folder->ob_type->tp_free(
	 (PyObject*) pypff_folder );
}

/* Retrieves the display name
 * Returns a Python object if successful or NULL on error
 */
PyObject *pypff_folder_get_display_name(
           pypff_folder_t *pypff_folder )
{
	char error_string[ PYPFF_ERROR_STRING_SIZE ];

	libcerror_error_t *error  = NULL;
	PyObject *string_object  = NULL;
	const char *errors       = NULL;
	uint8_t *value_string    = NULL;
	static char *function    = "pypff_folder_get_display_name";
	size_t value_string_size = 0;
	int result               = 0;

	if( pypff_folder == NULL )
	{
		PyErr_Format(
		 PyExc_TypeError,
		 "%s: invalid folder.",
		 function );

		return( NULL );
	}
	result = libpff_item_get_entry_value_utf8_string_size(
	          pypff_folder->item,
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
		  pypff_folder->item,
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

/* Retrieves the number of sub folders
 * Returns a Python object if successful or NULL on error
 */
PyObject *pypff_folder_get_number_of_sub_folders(
           pypff_folder_t *pypff_folder )
{
	char error_string[ PYPFF_ERROR_STRING_SIZE ];

	libcerror_error_t *error   = NULL;
	static char *function     = "pypff_folder_get_number_of_sub_folders";
	int number_of_sub_folders = 0;

	if( pypff_folder == NULL )
	{
		PyErr_Format(
		 PyExc_TypeError,
		 "%s: invalid folder.",
		 function );

		return( NULL );
	}
	if( libpff_folder_get_number_of_sub_folders(
	     pypff_folder->item,
	     &number_of_sub_folders,
	     &error ) != 1 )
	{
		if( libcerror_error_backtrace_sprint(
		     error,
		     error_string,
		     PYPFF_ERROR_STRING_SIZE ) == -1 )
                {
			PyErr_Format(
			 PyExc_IOError,
			 "%s: unable to retrieve number of sub folders.",
			 function );
		}
		else
		{
			PyErr_Format(
			 PyExc_IOError,
			 "%s: unable to retrieve number of sub folders.\n%s",
			 function,
			 error_string );
		}
		libcerror_error_free(
		 &error );

		return( NULL );
	}
	return( PyInt_FromLong(
	         (long) number_of_sub_folders ) );
}

/* Retrieves a specific sub folder
 * Returns a Python object if successful or NULL on error
 */
PyObject *pypff_folder_get_sub_folder(
           pypff_folder_t *pypff_folder,
           PyObject *arguments,
           PyObject *keywords )
{
	char error_string[ PYPFF_ERROR_STRING_SIZE ];

	libcerror_error_t *error     = NULL;
	libpff_item_t *sub_folder   = NULL;
	PyObject *folder_object     = NULL;
	static char *keyword_list[] = { "sub_folder_index", NULL };
	static char *function       = "pypff_folder_get_sub_folder";
	int sub_folder_index        = 0;

	if( pypff_folder == NULL )
	{
		PyErr_Format(
		 PyExc_TypeError,
		 "%s: invalid folder.",
		 function );

		return( NULL );
	}
	if( PyArg_ParseTupleAndKeywords(
	     arguments,
	     keywords,
	     "i",
	     keyword_list,
	     &sub_folder_index ) == 0 )
        {
		goto on_error;
        }
	if( libpff_folder_get_sub_folder(
	     pypff_folder->item,
	     sub_folder_index,
	     &sub_folder,
	     &error ) != 1 )
	{
		if( libcerror_error_backtrace_sprint(
		     error,
		     error_string,
		     PYPFF_ERROR_STRING_SIZE ) == -1 )
                {
			PyErr_Format(
			 PyExc_IOError,
			 "%s: unable to retrieve sub folder: %d.",
			 function,
			 sub_folder_index );
		}
		else
                {
			PyErr_Format(
			 PyExc_IOError,
			 "%s: unable to retrieve sub folder: %d.\n%s",
			 function,
			 sub_folder_index,
			 error_string );
		}
		libcerror_error_free(
		 &error );

		goto on_error;
	}
	folder_object = pypff_folder_new(
	                 NULL );

	if( folder_object == NULL )
	{
		PyErr_Format(
		 PyExc_MemoryError,
		 "%s: unable to create folder object.",
		 function );

		goto on_error;
	}
	( (pypff_folder_t *) folder_object )->item = sub_folder;

	return( folder_object );

on_error:
	if( sub_folder != NULL )
	{
		libpff_item_free(
		 &sub_folder,
		 NULL );
	}
	return( NULL );
}

/* Retrieves the number of sub messages
 * Returns a Python object if successful or NULL on error
 */
PyObject *pypff_folder_get_number_of_sub_messages(
           pypff_folder_t *pypff_folder )
{
	char error_string[ PYPFF_ERROR_STRING_SIZE ];

	libcerror_error_t *error    = NULL;
	static char *function      = "pypff_folder_get_number_of_sub_messages";
	int number_of_sub_messages = 0;

	if( pypff_folder == NULL )
	{
		PyErr_Format(
		 PyExc_TypeError,
		 "%s: invalid folder.",
		 function );

		return( NULL );
	}
	if( libpff_folder_get_number_of_sub_messages(
	     pypff_folder->item,
	     &number_of_sub_messages,
	     &error ) != 1 )
	{
		if( libcerror_error_backtrace_sprint(
		     error,
		     error_string,
		     PYPFF_ERROR_STRING_SIZE ) == -1 )
                {
			PyErr_Format(
			 PyExc_IOError,
			 "%s: unable to retrieve number of sub messages.",
			 function );
		}
		else
                {
			PyErr_Format(
			 PyExc_IOError,
			 "%s: unable to retrieve number of sub messages.\n%s",
			 function,
			 error_string );
		}
		libcerror_error_free(
		 &error );

		return( NULL );
	}
	return( PyInt_FromLong(
	         (long) number_of_sub_messages ) );
}

/* Retrieves a specific sub message
 * Returns a Python object if successful or NULL on error
 */
PyObject *pypff_folder_get_sub_message(
           pypff_folder_t *pypff_folder,
           PyObject *arguments,
           PyObject *keywords )
{
	char error_string[ PYPFF_ERROR_STRING_SIZE ];

	libcerror_error_t *error     = NULL;
	libpff_item_t *sub_message  = NULL;
	PyObject *message_object    = NULL;
	static char *keyword_list[] = { "sub_message_index", NULL };
	static char *function       = "pypff_folder_get_sub_message";
	int sub_message_index       = 0;

	if( pypff_folder == NULL )
	{
		PyErr_Format(
		 PyExc_TypeError,
		 "%s: invalid folder.",
		 function );

		return( NULL );
	}
	if( PyArg_ParseTupleAndKeywords(
	     arguments,
	     keywords,
	     "i",
	     keyword_list,
	     &sub_message_index ) == 0 )
        {
		goto on_error;
        }
	if( libpff_folder_get_sub_message(
	     pypff_folder->item,
	     sub_message_index,
	     &sub_message,
	     &error ) != 1 )
	{
		if( libcerror_error_backtrace_sprint(
		     error,
		     error_string,
		     PYPFF_ERROR_STRING_SIZE ) == -1 )
                {
			PyErr_Format(
			 PyExc_IOError,
			 "%s: unable to retrieve sub message: %d.",
			 function,
			 sub_message_index );
		}
		else
                {
			PyErr_Format(
			 PyExc_IOError,
			 "%s: unable to retrieve sub message: %d.\n%s",
			 function,
			 sub_message_index,
			 error_string );
		}
		libcerror_error_free(
		 &error );

		goto on_error;
	}
	message_object = pypff_message_new(
	                  NULL );

	if( message_object == NULL )
	{
		PyErr_Format(
		 PyExc_MemoryError,
		 "%s: unable to create message object.",
		 function );

		goto on_error;
	}
	( (pypff_message_t *) message_object )->item = sub_message;

	return( message_object );

on_error:
	if( sub_message != NULL )
	{
		libpff_item_free(
		 &sub_message,
		 NULL );
	}
	return( NULL );
}


