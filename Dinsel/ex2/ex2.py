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
        self.fileXML = xml
        self.fileXSD = xsd
        self.dict = defaultdict(list)

        try:  # to open the xml file and proceed if validation is true
            self.readXSD()
            self.readXML()
            self.readAttributes()
            self.writeCSV()
        except (XMLSchemaParseError, XMLSyntaxError) as e:
            # stream needs file object
            sys.stderr = open('err.log', 'w')
            sys.stderr.write(str(e))
            if verbose:
                print("ERROR: {}".format(e))

        if verbose:
            for k, v in self.dict.items():
                # stdout catches its file object from command line pipe
                print(k, v, file=sys.stdout)

    def readXSD(self):
        """Open a scheme file to validate the to-be-read file."""
        schema = etree.parse(self.fileXSD)
        self.schema = etree.XMLSchema(schema)
        # only returns True or False
        # print(self.schema.validate(etree.parse(self.fileXML)))

    def readXML(self):
        """Open XML file and return the root."""
        parser = etree.XMLParser(remove_comments=True, schema=self.schema)
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
        preHourValue = int(nodes.get('gasDayStartHourInUTC'))
        # boundaryNode - depth 1
        for node in nodes:
            # time - depth 2
            for elem in node:
                # amountOfPower - depth 3
                aop = elem.find("{" + nsmap[None] + "}amountOfPower")
                # set id as dictionary key and tuple the hour + value
                self.dict[node.get('id')].append((int(elem.get('hour')) + preHourValue, float(aop.get('value'))))

    def writeCSV(self):
        """Print data to csv."""
        with open('.'.join(self.fileXML.split('/')[1].split('.')[:-1]) + '.csv', 'w', newline='') as out:
            writer = csv.writer(out, delimiter=';')
            for key, vals in sorted(self.dict.items()):
                writer.writerow(['# ' + key])
                for val in vals:
                    writer.writerow([self.date, val[0], val[1]])


def main(argv):
    """."""
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
