/*
 * Python object definition of the libpff file
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
#include "pypff_codepage.h"
#include "pypff_file.h"
#include "pypff_file_object_io_handle.h"
#include "pypff_folder.h"
#include "pypff_item.h"
#include "pypff_libcerror.h"
#include "pypff_libclocale.h"
#include "pypff_libcstring.h"
#include "pypff_libpff.h"
#include "pypff_python.h"

#if !defined( LIBPFF_HAVE_BFIO )
LIBPFF_EXTERN \
int libpff_file_open_file_io_handle(
     libpff_file_t *file,
     libbfio_handle_t *file_io_handle,
     int access_flags,
     libpff_error_t **error );
#endif

PyMethodDef pypff_file_object_methods[] = {

	{ "signal_abort",
	  (PyCFunction) pypff_file_signal_abort,
	  METH_NOARGS,
	  "signal_abort() -> None\n"
	  "\n"
	  "Signals the file to abort the current activity" },

	/* Functions to access the file */

	{ "open",
	  (PyCFunction) pypff_file_open,
	  METH_VARARGS | METH_KEYWORDS,
	  "open(filename, access_flags) -> None\n"
	  "\n"
	  "Opens a file" },

	{ "open_file_object",
	  (PyCFunction) pypff_file_open_file_object,
	  METH_VARARGS | METH_KEYWORDS,
	  "open(file_object, access_flags) -> None\n"
	  "\n"
	  "Opens a file using a file-like object" },

	{ "close",
	  (PyCFunction) pypff_file_close,
	  METH_NOARGS,
	  "close() -> None\n"
	  "\n"
	  "Closes a file" },

	{ "get_ascii_codepage",
	  (PyCFunction) pypff_file_get_ascii_codepage,
	  METH_NOARGS,
	  "get_ascii_codepage() -> String\n"
	  "\n"
	  "Returns the codepage used for ASCII strings in the file" },

	{ "set_ascii_codepage",
	  (PyCFunction) pypff_file_set_ascii_codepage,
	  METH_VARARGS | METH_KEYWORDS,
	  "set_ascii_codepage(codepage) -> None\n"
	  "\n"
	  "Set the codepage used for ASCII strings in the file\n"
	  "Expects the codepage to be a String containing a Python codec definition" },

	{ "recover_items",
	  (PyCFunction) pypff_file_recover_items,
	  METH_NOARGS,
	  "recover_items() -> None\n"
	  "\n"
	  "Tries to recover items" },

	/* Functions to access the items */

	{ "get_root_folder",
	  (PyCFunction) pypff_file_get_root_folder,
	  METH_NOARGS,
	  "get_root_folder() -> Object or None\n"
	  "\n"
	  "Retrieves the root folder" },

	/* Functions to access the recovered items */

	{ "get_number_of_recovered_items",
	  (PyCFunction) pypff_file_get_number_of_recovered_items,
	  METH_NOARGS,
	  "get_number_of_recovered_items() -> Integer\n"
	  "\n"
	  "Retrieves the number of recovered items" },

	/* Sentinel */
	{ NULL, NULL, 0, NULL }
};

PyGetSetDef pypff_file_object_get_set_definitions[] = {

	{ "ascii_codepage",
	  (getter) pypff_file_get_ascii_codepage,
	  (setter) pypff_file_set_ascii_codepage,
	  "The codepage used for ASCII strings in the file",
	  NULL },

	/* Sentinel */
	{ NULL, NULL, NULL, NULL, NULL }
};

PyTypeObject pypff_file_type_object = {
	PyObject_HEAD_INIT( NULL )

	/* ob_size */
	0,
	/* tp_name */
	"pypff.file",
	/* tp_basicsize */
	sizeof( pypff_file_t ),
	/* tp_itemsize */
	0,
	/* tp_dealloc */
	(destructor) pypff_file_free,
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
	"pypff file object (wraps libpff_file_t)",
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
	pypff_file_object_methods,
	/* tp_members */
	0,
	/* tp_getset */
	pypff_file_object_get_set_definitions,
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
	(initproc) pypff_file_init,
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

/* Creates a new file object
 * Returns a Python object if successful or NULL on error
 */
PyObject *pypff_file_new(
           PyObject *self )
{
	pypff_file_t *pypff_file = NULL;
	static char *function    = "pypff_file_new";

	pypff_file = PyObject_New(
	              struct pypff_file,
	              &pypff_file_type_object );

	if( pypff_file == NULL )
	{
		PyErr_Format(
		 PyExc_MemoryError,
		 "%s: unable to initialize file.",
		 function );

		return( NULL );
	}
	if( pypff_file_init(
	     pypff_file ) != 0 )
	{
		PyErr_Format(
		 PyExc_MemoryError,
		 "%s: unable to initialize file.",
		 function );

		goto on_error;
	}
	return( (PyObject *) pypff_file );

on_error:
	if( pypff_file != NULL )
	{
		Py_DecRef(
		 (PyObject *) pypff_file );
	}
	return( NULL );
}

/* Creates a new file object and opens it
 * Returns a Python object if successful or NULL on error
 */
PyObject *pypff_file_new_open(
           PyObject *self,
           PyObject *arguments,
           PyObject *keywords )
{
	PyObject *pypff_file = NULL;

	pypff_file = pypff_file_new(
	              self );

	pypff_file_open(
	 (pypff_file_t *) pypff_file,
	 arguments,
	 keywords );

	return( pypff_file );
}

/* Intializes a file object
 * Returns 0 if successful or -1 on error
 */
int pypff_file_init(
     pypff_file_t *pypff_file )
{
	char error_string[ PYPFF_ERROR_STRING_SIZE ];

	static char *function   = "pypff_file_init";
	libcerror_error_t *error = NULL;

	if( pypff_file == NULL )
	{
		PyErr_Format(
		 PyExc_TypeError,
		 "%s: invalid file.",
		 function );

		return( -1 );
	}
	/* Make sure libpff file is set to NULL
	 */
	pypff_file->file = NULL;

	if( libpff_file_initialize(
	     &( pypff_file->file ),
	     &error ) != 1 )
	{
		if( libcerror_error_backtrace_sprint(
		     error,
		     error_string,
		     PYPFF_ERROR_STRING_SIZE ) == -1 )
                {
			PyErr_Format(
			 PyExc_MemoryError,
			 "%s: unable to initialize file.",
			 function );
		}
		else
		{
			PyErr_Format(
			 PyExc_MemoryError,
			 "%s: unable to initialize file.\n%s",
			 function,
			 error_string );
		}
		libcerror_error_free(
		 &error );

		return( -1 );
	}
	return( 0 );
}

/* Frees a file object
 */
void pypff_file_free(
      pypff_file_t *pypff_file )
{
	char error_string[ PYPFF_ERROR_STRING_SIZE ];

	libcerror_error_t *error = NULL;
	static char *function   = "pypff_file_free";

	if( pypff_file == NULL )
	{
		PyErr_Format(
		 PyExc_TypeError,
		 "%s: invalid file.",
		 function );

		return;
	}
	if( pypff_file->ob_type == NULL )
	{
		PyErr_Format(
		 PyExc_TypeError,
		 "%s: invalid file - missing ob_type.",
		 function );

		return;
	}
	if( pypff_file->ob_type->tp_free == NULL )
	{
		PyErr_Format(
		 PyExc_TypeError,
		 "%s: invalid file - invalid ob_type - missing tp_free.",
		 function );

		return;
	}
	if( pypff_file->file == NULL )
	{
		PyErr_Format(
		 PyExc_TypeError,
		 "%s: invalid file - missing libpff file.",
		 function );

		return;
	}
	if( libpff_file_free(
	     &( pypff_file->file ),
	     &error ) != 1 )
	{
		if( libcerror_error_backtrace_sprint(
		     error,
		     error_string,
		     PYPFF_ERROR_STRING_SIZE ) == -1 )
                {
			PyErr_Format(
			 PyExc_IOError,
			 "%s: unable to free libpff file.",
			 function );
		}
		else
		{
			PyErr_Format(
			 PyExc_IOError,
			 "%s: unable to free libpff file.\n%s",
			 function,
			 error_string );
		}
		libcerror_error_free(
		 &error );
	}
	pypff_file->ob_type->tp_free(
	 (PyObject*) pypff_file );
}

/* Signals the file to abort the current activity
 * Returns a Python object if successful or NULL on error
 */
PyObject *pypff_file_signal_abort(
           pypff_file_t *pypff_file )
{
	char error_string[ PYPFF_ERROR_STRING_SIZE ];

	libcerror_error_t *error = NULL;
	static char *function   = "pypff_file_signal_abort";

	if( pypff_file == NULL )
	{
		PyErr_Format(
		 PyExc_TypeError,
		 "%s: invalid file.",
		 function );

		return( NULL );
	}
	if( libpff_file_signal_abort(
	     pypff_file->file,
	     &error ) != 1 )
	{
		if( libcerror_error_backtrace_sprint(
		     error,
		     error_string,
		     PYPFF_ERROR_STRING_SIZE ) == -1 )
                {
			PyErr_Format(
			 PyExc_IOError,
			 "%s: unable to signal abort.",
			 function );
		}
		else
		{
			PyErr_Format(
			 PyExc_IOError,
			 "%s: unable to signal abort.\n%s",
			 function,
			 error_string );
		}
		libcerror_error_free(
		 &error );

		return( NULL );
	}
	Py_IncRef(
	 Py_None );

	return( Py_None );
}

/* Opens a file
 * Returns a Python object if successful or NULL on error
 */
PyObject *pypff_file_open(
           pypff_file_t *pypff_file,
           PyObject *arguments,
           PyObject *keywords )
{
	char error_string[ PYPFF_ERROR_STRING_SIZE ];

	libcerror_error_t *error     = NULL;
	char *filename              = NULL;
	static char *keyword_list[] = { "filename", "access_flags", NULL };
	static char *function       = "pypff_file_open";
	int access_flags            = 0;

	if( pypff_file == NULL )
	{
		PyErr_Format(
		 PyExc_TypeError,
		 "%s: invalid file.",
		 function );

		return( NULL );
	}
	if( PyArg_ParseTupleAndKeywords(
	     arguments,
	     keywords,
	     "s|i",
	     keyword_list,
	     &filename,
	     &access_flags ) == 0 )
        {
                return( NULL );
        }
	if( libpff_file_open(
	     pypff_file->file,
             filename,
             (uint8_t) access_flags,
	     &error ) != 1 )
	{
		if( libcerror_error_backtrace_sprint(
		     error,
		     error_string,
		     PYPFF_ERROR_STRING_SIZE ) == -1 )
                {
			PyErr_Format(
			 PyExc_IOError,
			 "%s: unable to open file.",
			 function );
		}
		else
		{
			PyErr_Format(
			 PyExc_IOError,
			 "%s: unable to open file.\n%s",
			 function,
			 error_string );
		}
		libcerror_error_free(
		 &error );

		return( NULL );
	}
	Py_IncRef(
	 Py_None );

	return( Py_None );
}

/* Opens a file using a file-like object
 * Returns a Python object if successful or NULL on error
 */
PyObject *pypff_file_open_file_object(
           pypff_file_t *pypff_file,
           PyObject *arguments,
           PyObject *keywords )
{
	char error_string[ PYPFF_ERROR_STRING_SIZE ];

	PyObject *file_object            = NULL;
	libbfio_handle_t *file_io_handle = NULL;
	libcerror_error_t *error         = NULL;
	static char *keyword_list[]      = { "file_object", "access_flags", NULL };
	static char *function            = "pypff_file_open_file_object";
	int access_flags                 = 0;
	int result                       = 0;

	if( pypff_file == NULL )
	{
		PyErr_Format(
		 PyExc_ValueError,
		 "%s: invalid file.",
		 function );

		return( NULL );
	}
	if( PyArg_ParseTupleAndKeywords(
	     arguments,
	     keywords,
	     "O|i",
	     keyword_list,
	     &file_object,
	     &access_flags ) == 0 )
        {
                return( NULL );
        }
	/* Default to read-only if no access flags were provided
	 */
	if( access_flags == 0 )
	{
		access_flags = libpff_get_access_flags_read();
	}
	if( pypff_file_object_initialize(
	     &file_io_handle,
	     file_object,
	     &error ) != 1 )
	{
		if( libcerror_error_backtrace_sprint(
		     error,
		     error_string,
		     PYPFF_ERROR_STRING_SIZE ) == -1 )
                {
			PyErr_Format(
			 PyExc_MemoryError,
			 "%s: unable to initialize file IO handle.",
			 function );
		}
		else
		{
			PyErr_Format(
			 PyExc_MemoryError,
			 "%s: unable to initialize file IO handle.\n%s",
			 function,
			 error_string );
		}
		libcerror_error_free(
		 &error );

		goto on_error;
	}
	Py_BEGIN_ALLOW_THREADS

	result = libpff_file_open_file_io_handle(
	          pypff_file->file,
                  file_io_handle,
                  access_flags,
	          &error );

	Py_END_ALLOW_THREADS

	if( result != 1 )
	{
		if( libcerror_error_backtrace_sprint(
		     error,
		     error_string,
		     PYPFF_ERROR_STRING_SIZE ) == -1 )
                {
			PyErr_Format(
			 PyExc_IOError,
			 "%s: unable to open file.",
			 function );
		}
		else
		{
			PyErr_Format(
			 PyExc_IOError,
			 "%s: unable to open file.\n%s",
			 function,
			 error_string );
		}
		libcerror_error_free(
		 &error );

		goto on_error;
	}
	Py_IncRef(
	 Py_None );

	return( Py_None );

on_error:
	if( file_io_handle != NULL )
	{
		libbfio_handle_free(
		 &file_io_handle,
		 NULL );
	}
	return( NULL );
}

/* Closes a file
 * Returns a Python object if successful or NULL on error
 */
PyObject *pypff_file_close(
           pypff_file_t *pypff_file )
{
	char error_string[ PYPFF_ERROR_STRING_SIZE ];

	libcerror_error_t *error = NULL;
	static char *function   = "pypff_file_close";

	if( pypff_file == NULL )
	{
		PyErr_Format(
		 PyExc_TypeError,
		 "%s: invalid file.",
		 function );

		return( NULL );
	}
	if( libpff_file_close(
	     pypff_file->file,
	     &error ) != 0 )
	{
		if( libcerror_error_backtrace_sprint(
		     error,
		     error_string,
		     PYPFF_ERROR_STRING_SIZE ) == -1 )
                {
			PyErr_Format(
			 PyExc_IOError,
			 "%s: unable to close file.",
			 function );
		}
		else
		{
			PyErr_Format(
			 PyExc_IOError,
			 "%s: unable to close file.\n%s",
			 function,
			 error_string );
		}
		libcerror_error_free(
		 &error );

		return( NULL );
	}
	Py_IncRef(
	 Py_None );

	return( Py_None );
}

/* Retrieves the codepage used for ASCII strings in the file
 * Returns a Python object holding the offset if successful or NULL on error
 */
PyObject *pypff_file_get_ascii_codepage(
           pypff_file_t *pypff_file )
{
	char error_string[ PYPFF_ERROR_STRING_SIZE ];

	libcerror_error_t *error    = NULL;
	PyObject *string_object     = NULL;
	const char *codepage_string = NULL;
	static char *function       = "pypff_file_get_ascii_codepage";
	int ascii_codepage          = 0;

	if( pypff_file == NULL )
	{
		PyErr_Format(
		 PyExc_ValueError,
		 "%s: invalid file.",
		 function );

		return( NULL );
	}
	if( libpff_file_get_ascii_codepage(
	     pypff_file->file,
	     &ascii_codepage,
	     &error ) != 1 )
	{
		if( libcerror_error_backtrace_sprint(
		     error,
		     error_string,
		     PYPFF_ERROR_STRING_SIZE ) == -1 )
		{
			PyErr_Format(
			 PyExc_IOError,
			 "%s: unable to retrieve ASCII codepage.",
			 function );
		}
		else
		{
			PyErr_Format(
			 PyExc_IOError,
			 "%s: unable to retrieve ASCII codepage.\n%s",
			 function,
			 error_string );
		}
		libcerror_error_free(
		 &error );

		return( NULL );
	}
	codepage_string = pypff_codepage_to_string(
	                   ascii_codepage );

	if( codepage_string == NULL )
	{
		PyErr_Format(
		 PyExc_ValueError,
		 "%s: unsupported ASCII codepage: %d.",
		 function,
		 ascii_codepage );

		return( NULL );
	}
	string_object = PyString_FromString(
	                 codepage_string );

	if( string_object == NULL )
	{
		PyErr_Format(
		 PyExc_IOError,
		 "%s: unable to convert codepage string into string object.",
		 function );

		return( NULL );
	}
	return( string_object );
}

/* Sets the codepage used for ASCII strings in the file
 * Returns a Python object holding the offset if successful or NULL on error
 */
PyObject *pypff_file_set_ascii_codepage(
           pypff_file_t *pypff_file,
           PyObject *arguments,
           PyObject *keywords )
{
	char error_string[ PYPFF_ERROR_STRING_SIZE ];

	libcerror_error_t *error      = NULL;
	char *codepage_string         = NULL;
	static char *keyword_list[]   = { "codepage", NULL };
	static char *function         = "pypff_file_set_ascii_codepage";
	size_t codepage_string_length = 0;
	uint32_t feature_flags        = 0;
	int ascii_codepage            = 0;

	if( pypff_file == NULL )
	{
		PyErr_Format(
		 PyExc_ValueError,
		 "%s: invalid file.",
		 function );

		return( NULL );
	}
	if( PyArg_ParseTupleAndKeywords(
	     arguments,
	     keywords,
	     "s",
	     keyword_list,
	     &codepage_string ) == 0 )
        {
                return( NULL );
        }
	if( codepage_string == NULL )
	{
		PyErr_Format(
		 PyExc_ValueError,
		 "%s: invalid codepage string.",
		 function );

		return( NULL );
	}
	codepage_string_length = libcstring_narrow_string_length(
	                          codepage_string );

	feature_flags = LIBCLOCALE_CODEPAGE_FEATURE_FLAG_HAVE_KOI8
	              | LIBCLOCALE_CODEPAGE_FEATURE_FLAG_HAVE_WINDOWS;

	if( libclocale_codepage_copy_from_string(
	     &ascii_codepage,
	     codepage_string,
	     codepage_string_length,
	     feature_flags,
	     &error ) != 1 )
	{
		if( libcerror_error_backtrace_sprint(
		     error,
		     error_string,
		     PYPFF_ERROR_STRING_SIZE ) == -1 )
		{
			PyErr_Format(
			 PyExc_RuntimeError,
			 "%s: unable to determine ASCII codepage.",
			 function );
		}
		else
		{
			PyErr_Format(
			 PyExc_RuntimeError,
			 "%s: unable to determine ASCII codepage.\n%s",
			 function,
			 error_string );
		}
		libcerror_error_free(
		 &error );

		return( NULL );
	}
	if( libpff_file_set_ascii_codepage(
	     pypff_file->file,
	     ascii_codepage,
	     &error ) != 1 )
	{
		if( libcerror_error_backtrace_sprint(
		     error,
		     error_string,
		     PYPFF_ERROR_STRING_SIZE ) == -1 )
		{
			PyErr_Format(
			 PyExc_IOError,
			 "%s: unable to set ASCII codepage.",
			 function );
		}
		else
		{
			PyErr_Format(
			 PyExc_IOError,
			 "%s: unable to set ASCII codepage.\n%s",
			 function,
			 error_string );
		}
		libcerror_error_free(
		 &error );

		return( NULL );
	}
	Py_IncRef(
	 Py_None );

	return( Py_None );
}

/* Tries to recover items
 * Returns a Python object if successful or NULL on error
 */
PyObject *pypff_file_recover_items(
           pypff_file_t *pypff_file )
{
	char error_string[ PYPFF_ERROR_STRING_SIZE ];

	libcerror_error_t *error = NULL;
	static char *function   = "pypff_file_recover_items";

	if( pypff_file == NULL )
	{
		PyErr_Format(
		 PyExc_TypeError,
		 "%s: invalid file.",
		 function );

		return( NULL );
	}
	if( libpff_file_recover_items(
	     pypff_file->file,
	     0,
	     &error ) != 1 )
	{
		if( libcerror_error_backtrace_sprint(
		     error,
		     error_string,
		     PYPFF_ERROR_STRING_SIZE ) == -1 )
		{
			PyErr_Format(
			 PyExc_IOError,
			 "%s: unable to recover items.",
			 function );
		}
		else
		{
			PyErr_Format(
			 PyExc_IOError,
			 "%s: unable to recover items.\n%s",
			 function,
			 error_string );
		}
		libcerror_error_free(
		 &error );

		return( NULL );
	}
	Py_IncRef(
	 Py_None );

	return( Py_None );
}

/* Retrieves the root folder
 * Returns a Python object if successful or NULL on error
 */
PyObject *pypff_file_get_root_folder(
           pypff_file_t *pypff_file )
{
	char error_string[ PYPFF_ERROR_STRING_SIZE ];

	libcerror_error_t *error    = NULL;
	libpff_item_t *root_folder = NULL;
	PyObject *folder_object    = NULL;
	static char *function      = "pypff_file_get_root_folder";

	if( pypff_file == NULL )
	{
		PyErr_Format(
		 PyExc_TypeError,
		 "%s: invalid file.",
		 function );

		return( NULL );
	}
	if( libpff_file_get_root_folder(
	     pypff_file->file,
	     &root_folder,
	     &error ) != 1 )
	{
		if( libcerror_error_backtrace_sprint(
		     error,
		     error_string,
		     PYPFF_ERROR_STRING_SIZE ) == -1 )
                {
			PyErr_Format(
			 PyExc_IOError,
			 "%s: unable to retrieve root folder item.",
			 function );
		}
		else
		{
			PyErr_Format(
			 PyExc_IOError,
			 "%s: unable to retrieve root folder item.\n%s",
			 function,
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
	( (pypff_folder_t *) folder_object )->item = root_folder;

	return( folder_object );

on_error:
	if( root_folder != NULL )
	{
		libpff_item_free(
		 &root_folder,
		 NULL );
	}
	return( NULL );
}

/* Retrieves the number of recovered items
 * Returns a Python object if successful or NULL on error
 */
PyObject *pypff_file_get_number_of_recovered_items(
           pypff_file_t *pypff_file )
{
	char error_string[ PYPFF_ERROR_STRING_SIZE ];

	libcerror_error_t *error = NULL;
	static char *function   = "pypff_file_get_number_of_recovered_items";
	int number_of_items     = 0;

	if( pypff_file == NULL )
	{
		PyErr_Format(
		 PyExc_TypeError,
		 "%s: invalid file.",
		 function );

		return( NULL );
	}
	if( libpff_file_get_number_of_recovered_items(
	     pypff_file->file,
	     &number_of_items,
	     &error ) != 1 )
	{
		if( libcerror_error_backtrace_sprint(
		     error,
		     error_string,
		     PYPFF_ERROR_STRING_SIZE ) == -1 )
                {
			PyErr_Format(
			 PyExc_IOError,
			 "%s: unable to retrieve number of recover items.",
			 function );
		}
		else
		{
			PyErr_Format(
			 PyExc_IOError,
			 "%s: unable to retrieve number of recover items.\n%s",
			 function,
			 error_string );
		}
		libcerror_error_free(
		 &error );

		return( NULL );
	}
	return( PyInt_FromLong(
	         (long) number_of_items ) );
}

