#!/usr/bin/env python

"""
Exercise 2 + 4

This package provides means to solving exercises 2 and 4 from the lecture appfs
"""

import sys
from lxml import etree

class XMLValidator:
    """
    Class XMLValidator

    This class provides methods for completing the second exercise of the lecture appfs, summer 2017, TUB
    """

    @staticmethod
    def validate(schema, xmlfile):
        """
        Validation

        Validate an etree against an xml schema and print out the result.

        Args:
            schema (str): filename of the schema to be checked against
            xmlfile (lxml.etree): etree of the xmlfile to be checked
        """
        schema_root = etree.parse(schema)
        schema = etree.XMLSchema(schema_root)
        if schema.validate(xmlfile):
            print("Validation against provided schema successful!")
        else:
            print("Validation against provided schema failed!")

    @staticmethod
    def is_comment(element):
        """ Is the element a comment?

        Decide whether or not given element is a comment or not.

        Args:
            element (lxml.etree): an element of an etree

        Returns:
            bool: Element is a comment.
        """
        return element is etree.Comment

    @staticmethod
    def extract_information(xmlfile):
        """
        Extract information
        Read from xml etree and print CSV data in format:
        YYYY-MM-DD; HH; amountOfPower-Value

        Args:
            xmlfile (lxml.etree): the etree of the xmlfile
        """
        root = xmlfile.getroot()
        for gas_day in root:
            if XMLValidator.is_comment(gas_day):
                continue
            shift_hour = int(gas_day.attrib['gasDayStartHourInUTC'])
            for boundary_node in gas_day:
                if XMLValidator.is_comment(boundary_node):
                    continue
                for time in boundary_node:
                    if XMLValidator.is_comment(time):
                        continue
                    for elem in time:
                        if XMLValidator.is_comment(elem):
                            continue
                        if elem.tag != '{http://gaslab.zib.de/kwpt/measured}amountOfPower':
                            continue
                        print("{0}; {1:02d}; {2}".format(gas_day.attrib['date'], \
                                int(time.attrib['hour'])+shift_hour, elem.attrib['value']))


if __name__ == "__main__":
    # read filename from commandlineargs
    try:
        OPTION = sys.argv[1]
    except IndexError:
        OPTION = None
    # provide help to user
    for helpopt in [None, "-h", "--h", "-help", "--help"]:
        if helpopt == OPTION:
            print("USAGE:\n./ex2 filename.xml schema.xsd")
            print("EXAMPLE:\n./ex2 measured-1.0.0.2017-02-03.b0050c5c8deb1db59c7b2644414b079d.xml measured-1-1-0.xsd")
            exit()

    # read variables
    FILENAME_XML = sys.argv[1]
    FILENAME_SCHEMA = sys.argv[2]

    # construct xmltree
    ETREE_XMLFILE = etree.parse(FILENAME_XML)

    # extract information and validate against schema
    XMLValidator.extract_information(ETREE_XMLFILE)
    XMLValidator.validate(FILENAME_SCHEMA, ETREE_XMLFILE)
