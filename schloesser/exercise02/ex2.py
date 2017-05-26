#!/usr/bin/env python

## @package exercise 2 + 4
#  This package provides means to solving exercises 2 and 4 from the lecture appfs

import sys
from lxml import etree

## Class XMLValidator
#  This class provides methods for completing the second exercise of the lecture appfs, summer 2017, TUB
class XMLValidator:
	
	@staticmethod
	## Validation 
	#  Validate an etree against an xml schema and print out the result.
	def validate(filename_schema, etree_xmlfile):
		schema_root = etree.parse(filename_schema)
		schema = etree.XMLSchema(schema_root)
		if schema.validate(etree_xmlfile):
			print("Validation against provided schema successful!")
		else:
			print("Validation against provided schema failed!")
	
	@staticmethod
	## Is the element a comment?
	#  Decide whether or not given element is a comment or not.
	# 
	#  @param element an element of an etree
	def is_comment(element):
		return type(element) == etree._Comment
	
	@staticmethod
	## Extract information
	#  Read from xml etree and print CSV data in format:
	#  YYYY-MM-DD; HH; amountOfPower-Value
	#  
	#  @param etree_xmlfile the etree of the xmlfile
	def extract_information(etree_xmlfile):
		root = etree_xmlfile.getroot() 
		for gas_day in root:
			if XMLValidator.is_comment(gas_day): continue
			shift_hour = int(gas_day.attrib['gasDayStartHourInUTC'])
			for boundary_node in gas_day:
				if XMLValidator.is_comment(boundary_node): continue
				for time in boundary_node:
					if XMLValidator.is_comment(time): continue
					for elem in time:
						if XMLValidator.is_comment(elem): continue
						if elem.tag != '{http://gaslab.zib.de/kwpt/measured}amountOfPower': continue
						print("{0}; {1:02d}; {2}".format(gas_day.attrib['date'], int(time.attrib['hour'])+shift_hour, elem.attrib['value']))


## Mainmethod
#  run when class is executed, but not after import
if __name__=="__main__":
	# read filename from commandlineargs
	try:
		opt = sys.argv[1] 
	except IndexError:
		opt = None
	# provide help to user
	for option in [None, "-h", "--h", "-help", "--help"]:
		if opt == option:
			print("USAGE:\n./ex2 filename.xml schema.xsd")
			print("EXAMPLE:\n./ex2 measured-1.0.0.2017-02-03.b0050c5c8deb1db59c7b2644414b079d.xml measured-1-1-0.xsd")
			exit()

	# read variables
	filename_xml = sys.argv[1]
	filename_schema = sys.argv[2]
	
	# construct xmltree
	etree_xmlfile = etree.parse(filename_xml)
	
	# extract information and validate against schema
	XMLValidator.extract_information(etree_xmlfile)
	XMLValidator.validate(filename_schema, etree_xmlfile)

