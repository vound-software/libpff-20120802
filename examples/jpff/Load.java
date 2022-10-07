/*
 * Java application to load jpff
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

public class Load
{
	public static void main( String args[] )
	{
		try
		{
			System.loadLibrary(
			 "jpff" );
		}
		catch( UnsatisfiedLinkError exception )
		{
			System.err.println(
			 "Failed to load: jpff with exception: " + exception + "\n" );

			System.err.println(
			 "java.library.path: " + System.getProperty( "java.library.path" ) + "\n" );

			System.exit( 1 );
		}
		System.out.println(
		 "jpff loaded successfully.\n" );

		System.exit( 0 );
	}
}

