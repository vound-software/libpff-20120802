#! /usr/bin/env python
#
# Python script to iterate the folder hierarchy in a Personal Folder File using pypff
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
# Functions
# ----------------------------------------------------------------------------
def print_folder( folder, level ):
	if folder == None:
		print "Missing folder object."

		sys.exit( 1 )

	for sub_folder_index in range( 0, folder.get_number_of_sub_folders() ):
		sub_folder = folder.get_sub_folder(
		              sub_folder_index )

		for level_iterator in range( 0, level ):
			print "  ",

		name = sub_folder.get_name()

		if name != None:
			print name.encode( "utf8" )

		print_folder(
		 sub_folder,
		 level + 1 )

# ----------------------------------------------------------------------------
# Main
# ----------------------------------------------------------------------------

print "folders.py " + __version__ + " (libpff " + pypff.get_version() + ")\n"

if len( sys.argv ) != 2:
	print "Usage: folders.py filename."

	sys.exit( 1 )

file = pypff.new_file();

if file == None:
	print "Missing file object."
	print sys.exc_info()[ 1 ]

	sys.exit( 1 )

try:
	file.open(
	 sys.argv[ 1 ],
	 pypff.get_access_flags_read() )

except:
	print "Unable to open file."
	print sys.exc_info()[ 1 ]

	sys.exit( 1 )

print_folder(
 file.get_root_folder(),
 0 )

print ""

try:
	file.close()
except:
	print "Unable to close file."
	print sys.exc_info()[ 1 ]

	sys.exit( 1 )

sys.exit( 0 )

