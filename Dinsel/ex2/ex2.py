#!/usr/bin/env python3
# encoding: UTF-8

"""
Strip down any given XML-File and store the 'amountOfPower' as CSV

@FriedrichDinsel
"""


import sys
import csv
from lxml import etree, objectify
from collections import defaultdict


class ParseXML():
    """XML file handling."""

    def __init__(self, filename, verbose):
        self.fileXML = filename
        self.dict = defaultdict(list)

        self.openXML()
        self.readAttributes()
        self.writeCSV()

        if verbose:
            for k, v in self.dict.items():
                print(k, v)

    def openXML(self):
        """Open XML file and return the root."""
        parser = etree.XMLParser(remove_comments=True)
        tree = objectify.parse(self.fileXML, parser=parser)
        self.rootXML = tree.getroot()

    def readAttributes(self):
        """Catch XML nodes and attributes."""
        nsmap = self.rootXML.nsmap
        # gasDay - depth 0
        nodes = self.rootXML.find("{" + nsmap[None] + "}gasDay")
        self.date = nodes.get('date')
        pre = int(nodes.get('gasDayStartHourInUTC'))
        # boundaryNode - depth 1
        for node in nodes:
            # time - depth 2
            for elem in node:
                # amountOfPower - depth 3
                aop = elem.find("{" + nsmap[None] + "}amountOfPower")
                self.dict[node.get('id')].append((int(elem.get('hour')) + pre, aop.get('value')))

    def writeCSV(self):
        """Print data to csv."""
        with open('.'.join(self.fileXML.split('/')[1].split('.')[:-1]) + '.csv', 'w', newline='') as out:
            writer = csv.writer(out, delimiter=';')
            for key, vals in self.dict.items():
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
            with XSD scheme.
            '''
        )

    parser.add_argument('file', help='file to read', action='store')
    parser.add_argument('-v', '--verbose', help='print some output', action='store_true', dest='verbose')

    args = parser.parse_args()
    ParseXML(args.file, args.verbose)


if __name__ == '__main__':
    main(sys.argv[1:])
