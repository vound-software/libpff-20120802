/*
 * Java application to open and close a Personal Folder File using jpff
 *
 * Copyright (c) 2010, Joachim Metz <joachim.metz@gmail.com>
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

import jpff.File;

public class OpenClose
{
	public static void main( String args[] )
	{
		if( args.length != 1 )
		{
			System.err.println(
			 "Usage: OpenClose filename\n" );

			System.exit( 1 );
		}
		jpff.File file = new jpff.File();

		try
		{
			// TODO replace 1 by descriptive definition
			file.open(
			 args[ 0 ],
			 1 );
		}
		catch( Exception exception )
		{
			System.err.println(
			 "Unable to open file with exception: " + exception + ".\n" );

			System.exit( 1 );
		}
		try
		{
			file.close();
		}
		catch( Exception exception )
		{
			System.err.println(
			 "Unable to close file with exception: " + exception + ".\n" );

			System.exit( 1 );
		}
		System.exit( 0 );
	}
}

