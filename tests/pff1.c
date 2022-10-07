#include <common.h>

#if defined( HAVE_STDLIB_H ) || defined( WINAPI )
#include <stdlib.h>
#endif

#include <libpff.h>

#define TOUCH_ITEMS

int touch_item(
     libpff_item_t *item )
{
	libpff_error_t *error    = NULL;
	libpff_item_t *sub_item  = NULL;
	size_t value_string_size = 0;
	int number_of_sub_items  = 0;
	int sub_item_index       = 0;

	if( item == NULL )
	{
		fprintf(
		 stderr,
		 "Invalid item.\n" );

		return( -1 );
	}
	libpff_item_get_display_name_size(
	 item,
	 &value_string_size,
	 NULL );

	if( libpff_item_get_number_of_sub_items(
	     item,
	     &number_of_sub_items,
	     &error ) != 1 )
	{
		fprintf(
		 stderr,
		 "Unable to retrieve number of sub items.\n" );

		libpff_error_backtrace_fprint(
		 error,
		 stderr );

		libpff_error_free(
		 &error );

		return( -1 );
	}
	for( sub_item_index = 0;
	     sub_item_index < number_of_sub_items;
	     sub_item_index++ )
	{
		if( libpff_item_get_sub_item(
	             item,
		     sub_item_index,
		     &sub_item,
		     &error ) != 1 )
		{
			fprintf(
			 stderr,
			 "Unable to retrieve sub item: %d.\n",
			 sub_item_index );

			libpff_error_backtrace_fprint(
			 error,
			 stderr );

			libpff_error_free(
			 &error );

			return( -1 );
		}
		if( touch_item(
		     sub_item ) != 1 )
		{
			fprintf(
			 stderr,
			 "Unable to touch sub item: %d.\n",
			 sub_item_index );

			return( -1 );
		}
		if( libpff_item_free(
	             &sub_item,
		     &error ) != 1 )
		{
			fprintf(
			 stderr,
			 "Unable to free sub item: %d.\n",
			 sub_item_index );

			libpff_error_backtrace_fprint(
			 error,
			 stderr );

			libpff_error_free(
			 &error );

			return( -1 );
		}
	}
	return( 1 );
}

int main( int argc, char * const argv[] )
{
	libpff_error_t *error         = NULL;
	libpff_file_t *file           = NULL;
	libpff_item_t *item           = NULL;
	int number_of_recovered_items = 0;

	if( argc < 2 )
	{
		fprintf(
		 stderr,
		 "Missing filename.\n" );

		return( EXIT_FAILURE );
	}
	if( libpff_file_initialize(
	     &file,
	     &error ) != 1 )
	{
		fprintf(
		 stderr,
		 "Unable to create file.\n" );

		libpff_error_backtrace_fprint(
		 error,
		 stderr );

		libpff_error_free(
		 &error );

		return( EXIT_FAILURE );
	}
	if( libpff_file_open(
	     file,
	     argv[ 1 ],
	     LIBPFF_OPEN_READ,
	     &error ) != 1 )
	{
		fprintf(
		 stderr,
		 "Unable to open: %s.\n",
		 argv[ 1 ] );

		libpff_error_backtrace_fprint(
		 error,
		 stderr );

		libpff_error_free(
		 &error );

		return( EXIT_FAILURE );
	}
	if( libpff_file_get_root_folder(
	     file,
	     &item,
	     &error ) != 1 )
	{
		fprintf(
		 stderr,
		 "Unable to retrieve root folder item.\n" );

		libpff_error_backtrace_fprint(
		 error,
		 stderr );

		libpff_error_free(
		 &error );

		return( EXIT_FAILURE );
	}
#if defined( TOUCH_ITEMS )
	if( touch_item(
	     item ) != 1 )
	{
		fprintf(
		 stderr,
		 "Unable to touch root folder item.\n" );

		return( EXIT_FAILURE );
	}
#endif
	if( libpff_file_recover_items(
	     file,
	     0,
	     &error ) != 1 )
	{
		fprintf(
		 stderr,
		 "Unable to recover items.\n" );

		libpff_error_backtrace_fprint(
		 error,
		 stderr );

		libpff_error_free(
		 &error );

		return( EXIT_FAILURE );
	}
	if( libpff_file_get_number_of_recovered_items(
	     file,
	     &number_of_recovered_items,
	     &error ) != 1 )
	{
		fprintf(
		 stderr,
		 "Unable to retrieve number of recover items.\n" );

		libpff_error_backtrace_fprint(
		 error,
		 stderr );

		libpff_error_free(
		 &error );

		return( EXIT_FAILURE );
	}
	fprintf(
	 stdout,
	 "Number of recovered items: %d\n",
	 number_of_recovered_items );

	if( libpff_file_close(
	     file,
	     &error ) != 0 )
	{
		fprintf(
		 stderr,
		 "Unable to close file.\n" );

		libpff_error_backtrace_fprint(
		 error,
		 stderr );

		libpff_error_free(
		 &error );

		return( EXIT_FAILURE );
	}
	if( libpff_file_free(
	     &file,
	     &error ) != 1 )
	{
		fprintf(
		 stderr,
		 "Unable to free file.\n" );

		libpff_error_backtrace_fprint(
		 error,
		 stderr );

		libpff_error_free(
		 &error );

		return( EXIT_FAILURE );
	}
	return( EXIT_SUCCESS );
}

