/*
 * Date and time functions
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
#include <types.h>

#include "pypff_datetime.h"
#include "pypff_python.h"

#include <datetime.h>

/* Creates a new datetime object from a filetime
 * Returns a Python object if successful or NULL on error
 */
PyObject *pypff_datetime_new_from_filetime(
           uint64_t filetime )
{
	static char *function      = "pypff_datetime_new_from_filetime";
	PyObject *date_time_object = NULL;
	uint32_t days_in_century   = 0;
	uint16_t days_in_year      = 0;
	uint16_t year              = 0;
	uint8_t day                = 0;
	uint8_t days_in_month      = 0;
	uint8_t hours              = 0;
	uint8_t micro_seconds      = 0;
	uint8_t minutes            = 0;
	uint8_t month              = 0;
	uint8_t seconds            = 0;

	/* The timestamp is in units of 100 nano seconds correct the value to seconds
	 */
	micro_seconds = ( filetime % 10000000 ) / 10;
	filetime      /= 10000000;

	/* There are 60 seconds in a minute correct the value to minutes
	 */
	seconds   = filetime % 60;
	filetime /= 60;

	/* There are 60 minutes in an hour correct the value to hours
	 */
	minutes   = filetime % 60;
	filetime /= 60;

	/* There are 24 hours in a day correct the value to days
	 */
	hours     = filetime % 24;
	filetime /= 24;

	/* Add 1 day to compensate that Jan 1 1601 is represented as 0
	 */
	filetime += 1;

	/* Determine the number of years starting at '1 Jan 1601 00:00:00'
	 * correct the value to days within the year
	 */
	year = 1601;

	if( filetime >= 36159 )
	{
		year = 1700;

		filetime -= 36159;
	}
	while( filetime > 0 )
	{
		if( ( year % 400 ) == 0 )
		{
			days_in_century = 36525;
		}
		else
		{
			days_in_century = 36524;
		}
		if( filetime <= days_in_century )
		{
			break;
		}
		filetime -= days_in_century;

		year += 100;
	}
	while( filetime > 0 )
	{
		/* Check for a leap year
		 * The year is ( ( dividable by 4 ) and ( not dividable by 100 ) ) or ( dividable by 400 )
		 */
		if( ( ( ( year % 4 ) == 0 )
		  &&  ( ( year % 100 ) != 0 ) )
		 || ( ( year % 400 ) == 0 ) )
		{
			days_in_year = 366;
		}
		else
		{
			days_in_year = 365;
		}
		if( filetime <= days_in_year )
		{
			break;
		}
		filetime -= days_in_year;

		year += 1;
	}
	/* Determine the month correct the value to days within the month
	 */
	month = 1;

	while( filetime > 0 )
	{
		/* February (2)
		 */
		if( month == 2 )
		{
			if( ( ( ( year % 4 ) == 0 )
			  &&  ( ( year % 100 ) != 0 ) )
			 || ( ( year % 400 ) == 0 ) )
			{
				days_in_month = 29;
			}
			else
			{
				days_in_month = 28;
			}
		}
		/* April (4), June (6), September (9), November (11)
		 */
		else if( ( month == 4 )
		      || ( month == 6 )
		      || ( month == 9 )
		      || ( month == 11 ) )
		{
			days_in_month = 30;
		}
		/* Januari (1), March (3), May (5), July (7), August (8), October (10), December (12)
		 */
		else if( ( month == 1 )
		      || ( month == 3 )
		      || ( month == 5 )
		      || ( month == 7 )
		      || ( month == 8 )
		      || ( month == 10 )
		      || ( month == 12 ) )
		{
			days_in_month = 31;
		}
		/* This should never happen, but just in case
		 */
		else
		{
			PyErr_Format(
			 PyExc_IOError,
			 "%s: unsupported month: %d.",
			 function,
			 month );

			return( NULL );
		}
		if( filetime <= days_in_month )
		{
			break;
		}
		filetime -= days_in_month;

		month += 1;
	}
	/* Determine the day
	 */
	day = (uint8_t) filetime;

	PyDateTime_IMPORT;

	date_time_object = (PyObject *) PyDateTime_FromDateAndTime(
	                                 year,
	                                 month,
	                                 day,
	                                 hours,
	                                 minutes,
	                                 seconds,
	                                 micro_seconds );

	return( date_time_object );
}

/* Creates a new datetime object from a POSIX time
 * Returns a Python object if successful or NULL on error
 */
PyObject *pypff_datetime_new_from_posix_time(
           uint32_t posix_time )
{
	static char *function      = "pypff_datetime_new_from_posix_time";
	PyObject *date_time_object = NULL;
	uint16_t days_in_year      = 0;
	uint16_t year              = 0;
	uint8_t day                = 0;
	uint8_t days_in_month      = 0;
	uint8_t hours              = 0;
	uint8_t minutes            = 0;
	uint8_t month              = 0;
	uint8_t seconds            = 0;

	/* There are 60 seconds in a minute correct the value to minutes
	 */
	seconds     = posix_time % 60;
	posix_time /= 60;

	/* There are 60 minutes in an hour correct the value to hours
	 */
	minutes     = posix_time % 60;
	posix_time /= 60;

	/* There are 24 hours in a day correct the value to days
	 */
	hours       = posix_time % 24;
	posix_time /= 24;

	/* Add 1 day to compensate that Jan 1 1601 is represented as 0
	 */
	posix_time += 1;

	/* Determine the number of years starting at '1 Jan 1970 00:00:00'
	 * correct the value to days within the year
	 */
	year = 1970;

	if( posix_time >= 10957 )
	{
		year = 2000;

		posix_time -= 10957;
	}
	while( posix_time > 0 )
	{
		/* Check for a leap year
		 * The year is ( ( dividable by 4 ) and ( not dividable by 100 ) ) or ( dividable by 400 )
		 */
		if( ( ( ( year % 4 ) == 0 )
		  &&  ( ( year % 100 ) != 0 ) )
		 || ( ( year % 400 ) == 0 ) )
		{
			days_in_year = 366;
		}
		else
		{
			days_in_year = 365;
		}
		if( posix_time <= days_in_year )
		{
			break;
		}
		posix_time -= days_in_year;

		year += 1;
	}
	/* Determine the month correct the value to days within the month
	 */
	month = 1;

	while( posix_time > 0 )
	{
		/* February (2)
		 */
		if( month == 2 )
		{
			if( ( ( ( year % 4 ) == 0 )
			  &&  ( ( year % 100 ) != 0 ) )
			 || ( ( year % 400 ) == 0 ) )
			{
				days_in_month = 29;
			}
			else
			{
				days_in_month = 28;
			}
		}
		/* April (4), June (6), September (9), November (11)
		 */
		else if( ( month == 4 )
		      || ( month == 6 )
		      || ( month == 9 )
		      || ( month == 11 ) )
		{
			days_in_month = 30;
		}
		/* Januari (1), March (3), May (5), July (7), August (8), October (10), December (12)
		 */
		else if( ( month == 1 )
		      || ( month == 3 )
		      || ( month == 5 )
		      || ( month == 7 )
		      || ( month == 8 )
		      || ( month == 10 )
		      || ( month == 12 ) )
		{
			days_in_month = 31;
		}
		/* This should never happen, but just in case
		 */
		else
		{
			PyErr_Format(
			 PyExc_IOError,
			 "%s: unsupported month: %d.",
			 function,
			 month );

			return( NULL );
		}
		if( posix_time <= days_in_month )
		{
			break;
		}
		posix_time -= days_in_month;

		month += 1;
	}
	/* Determine the day
	 */
	day = (uint8_t) posix_time;

	PyDateTime_IMPORT;

	date_time_object = (PyObject *) PyDateTime_FromDateAndTime(
	                                 year,
	                                 month,
	                                 day,
	                                 hours,
	                                 minutes,
	                                 seconds,
	                                 0 );

	return( date_time_object );
}

