#! /usr/bin/env python
#
# Python sccript to open and close a Personal Folder File using pypff
#
# Author:            Joachim Metz
# Creation date:     July 24, 2010
# Modification date: October 14, 2010
#

__author__    = "Joachim Metz"
__version__   = "20101014"
__date__      = "Oct 14, 2010"
__copyright__ = "Copyright (c) 2010, Joachim Metz <joachim.metz@gmail.com>"
__license__   = "GNU LGPL version 3"

import sys
import pypff

# ----------------------------------------------------------------------------
# Main
# ----------------------------------------------------------------------------

print "open_close.py " + __version__ + " (libpff " + pypff.get_version() + ")\n"

if len( sys.argv ) != 2:
	print "Usage: open_close.py filename\n"

	sys.exit( 1 )

file = pypff.new_file();

if file == None:
	print "Missing file object\n"

	sys.exit( 1 )

try:
	file.open(
	 sys.argv[ 1 ],
	 pypff.get_access_flags_read() )

except:
	print "Unable to open file\n"
	print sys.exc_info()[ 1 ]

	sys.exit( 1 )

try:
	file.close()
except:
	print "Unable to close file\n"
	print sys.exc_info()[ 1 ]
 
	sys.exit( 1 )

sys.exit( 0 )

