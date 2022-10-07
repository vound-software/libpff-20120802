#! /usr/bin/env python
#
# Ptyhon script to iterate the messages in a Personal Folder File using pypff
#
# Author:            Joachim Metz
# Creation date:     July 24, 2010
# Modification date: October 14, 2010
#

__author__    = "Joachim Metz"
__version__   = "20101014"
__date__      = "OCt 14, 2010"
__copyright__ = "Copyright (c) 2010, Joachim Metz <joachim.metz@gmail.com>"
__license__   = "GNU LGPL version 3"

import sys
import pypff

# ----------------------------------------------------------------------------
# Functions
# ----------------------------------------------------------------------------
def print_folder( folder ):
	if folder == None:
		print "Missing folder object."

		sys.exit( 1 )

	for sub_message_index in range( 0, folder.get_number_of_sub_messages() ):
		print_message(
		 folder.get_sub_message(
		  sub_message_index ) )

	for sub_folder_index in range( 0, folder.get_number_of_sub_folders() ):
		print_folder(
		 folder.get_sub_folder(
		  sub_folder_index ) )

def print_message( message ):
	if message == None:
		print "Missing message object."

		sys.exit( 1 )

	subject = message.get_subject()

	if subject != None:
		print "Subject: " + subject.encode( "utf8" )

	print ""

	plain_text_body = message.get_plain_text_body()

	if plain_text_body != None:
		print plain_text_body.encode( "utf8" )

	print ""

# ----------------------------------------------------------------------------
# Main
# ----------------------------------------------------------------------------

print "messages.py " + __version__ + " (libpff " + pypff.get_version() + ")\n"

if len( sys.argv ) != 2:
	print "Usage: messages.py filename."

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
 file.get_root_folder() )

print ""

try:
	file.close()
except:
	print "Unable to close file."
	print sys.exc_info()[ 1 ]

	sys.exit( 1 )

sys.exit( 0 )

