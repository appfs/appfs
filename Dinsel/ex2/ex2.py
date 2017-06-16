#!/usr/bin/env python3
# encoding: UTF-8

"""
Strip down any given XML-File and store the 'amountOfPower' as CSV

@FriedrichDinsel
"""


import os
import sys
import csv
from lxml import etree, objectify
from lxml.etree import XMLSchemaParseError, XMLSyntaxError
from collections import defaultdict


class ParseXML():
    """XML file handling."""

    def __init__(self, xml, xsd, verbose):
        """
        Initiate the passed files and options.

        Parameters
        ----------
        xml : the xml file to extract the "amountOfPower" from
        xsd : the schema file to validate the xml against
        verbose : prompt some info on the screen
        """
        self.fileXML = xml
        self.fileXSD = xsd
        self.verbose = verbose
        self.dict = defaultdict(list)

        try:  # to open the xml file with schema
            schema = self.readXSD()
            self.readXML(schema)
            self.readAttributes()
            self.writeCSV()
        except (XMLSchemaParseError, XMLSyntaxError) as e:
            # stream needs file object
            # err_log = os.path.join(os.getcwd(), 'ex2/err.log')
            sys.stderr = open('err.log', 'w')
            sys.stderr.write(str(e))
            # possible to get the data although not valid
            print("Encountered error (see err.log). Trying to continue anyway...")
            self.readXML()
            self.readAttributes()
            self.writeCSV()
            # yesNo = input("Encountered error (see err.log). Continue anyway? [yn] ")
            # if yesNo == 'y':
            #     self.readXML()
            #     self.readAttributes()
            #     self.writeCSV()
            # else:
            #     sys.exit()

    def readXSD(self):
        """Open a scheme file to validate the to-be-read file."""
        schema = etree.parse(self.fileXSD)
        return etree.XMLSchema(schema)
        # only returns True or False
        # print(self.schema.validate(etree.parse(self.fileXML)))

    def readXML(self, schema=None):
        """
        Open XML file and return the root.

        Parameters
        ----------
        schema : validation with a schema is optional

        """
        parser = etree.XMLParser(remove_comments=True, schema=schema)
        # validation happens with the schema argument during parsing or throw
        # an error to see whats wrong (in init)
        tree = objectify.parse(self.fileXML, parser=parser)
        self.rootXML = tree.getroot()

    def readAttributes(self):
        """Catch XML nodes and attributes."""
        nsmap = self.rootXML.nsmap
        # gasDay - depth 0
        nodes = self.rootXML.find("{" + nsmap[None] + "}gasDay")
        self.date = nodes.get('date')
        startingHour = int(nodes.get('gasDayStartHourInUTC'))
        # boundaryNode - depth 1
        for node in nodes:
            # time - depth 2
            for elem in node:
                # amountOfPower - depth 3
                aop = elem.find("{" + nsmap[None] + "}amountOfPower")
                # set id as dictionary key and tuple the hour + value
                self.dict[node.get('id')].append((int(elem.get('hour')) + startingHour, float(aop.get('value'))))

    def writeCSV(self):
        """Print data to csv."""
        # fName = '.'.join(self.fileXML.split('/')[1].split('.')[:-1]) + '.csv'
        fName = os.path.join(os.getcwd(), 'ex2/ex2.csv')
        with open(fName, 'w', newline='') as out:
            writer = csv.writer(out, delimiter=';', lineterminator='\n')
            # sort dictionary by key
            for key, vals in sorted(self.dict.items()):
                writer.writerow(['# ' + key])
                for val in vals:
                    writer.writerow([self.date, val[0], val[1]])

        if self.verbose:
            print("Saved as {}".format(fName), file=sys.stdout)


def main(argv):
    """Argument handler."""
    import argparse
    parser = argparse.ArgumentParser(
        formatter_class=argparse.RawDescriptionHelpFormatter,
        description=
            '''\
                     EXERCISE 2
            ----------------------------
            read given XML file and store
            'amountOfPower' with date in
            CSV table. First check valid
            with XSD schema.
            '''
        ,
        epilog="If successfull a 'xml-file'.csv will be created.")

    parser.add_argument('xml', help='xml-file to read', action='store')
    parser.add_argument('xsd', help='xsd-file to validate against', action='store')
    parser.add_argument('-v', '--verbose', help='print some output', action='store_true', dest='verbose')

    args = parser.parse_args()
    ParseXML(args.xml, args.xsd, args.verbose)


if __name__ == '__main__':
    main(sys.argv[1:])
