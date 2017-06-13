import sys
import traceback
from lxml import etree


## @package ex2
#  This script is a solution for exercise 2 from the advanced programming course in the sommer 2017 at TUB.
# For more information see @link http://www.zib.de/koch/lectures/ss2017_appfs.php


## This methods validates the specified xml-file against the given xml-schema.
# @param filename_schema: the filename of the xml-schema file
# @param etree_xmlfile: the parsed xml-file
def validate_agains_schema(filename_schema, etree_xmlfile):
    try:
        schema_root = etree.parse(filename_schema)
        schema = etree.XMLSchema(schema_root)

        if schema.validate(etree_xmlfile):
            sys.stdout.write("Schema validation successful!")
        else:
            sys.stderr.write("ERROR: Schema validation failed!")
    except etree.XMLSchemaParseError:
        sys.stderr.write("ERROR: Exception while validating schema: \n", )
        traceback.print_exc(file=sys.stderr)
    except etree.XMLSyntaxError:
        sys.stderr.write("ERROR: Exception while parsing schema: \n", )
        traceback.print_exc(file=sys.stderr)


## Checks if the current xml-element is a comment or not.
# @param element: xml-element to check if it is a comment.
def is_comment(element):
    return type(element) == etree._Comment


## This method exports the given xml-file into the specified output file.
# The exact guidelines can be read at
# @link{http://www.zib.de/koch/lectures/ss2017_appfs/2017-05-05-Koch-Berlin-TUB-Advanced_Programming-3.pdf}
# @param etree_xmlfile: the parsed xml-file
# @param output_filename: the filename to write the specified data out.
def export_data_to_csv(etree_xmlfile, output_filename):
    root = etree_xmlfile.getroot()
    output_file = open(output_filename, 'w')
    # output_writer = csv.writer(fp, delimiter=',')
    for gas_day in root:
        if is_comment(gas_day):
            continue
        shift_hour = int(gas_day.attrib['gasDayStartHourInUTC'])
        for boundary_node in gas_day:
            if is_comment(boundary_node):
                continue
            for time in boundary_node:
                if is_comment(time):
                    continue
                for elem in time:
                    if is_comment(elem):
                        continue
                    if elem.tag != '{http://gaslab.zib.de/kwpt/measured}amountOfPower':
                        continue
                    output_file.write(
                        "{0}, {1:02d}, {2}".format(gas_day.attrib['date'], int(time.attrib['hour']) + shift_hour,
                                                   elem.attrib['value']))
                    output_file.write('\n')


for option in [None, "-h", "--h", "-help", "--help"]:
    try:
        opt = sys.argv[1]
    except IndexError:
        opt = None
    if opt == option:
        sys.stderr.write('USAGE:\n./ex2 <input.xml> <output.csv> <schema.xsd>(optional)')
        exit()
try:
    xml_input_file = sys.argv[1]
    csv_output_filename = sys.argv[2]
    if len(sys.argv) == 4:
        xsd_input_file = sys.argv[3]
except IndexError:
    sys.stderr.write(
        'Not enough or too many arguments. \n USAGE:\n./ex2 <input.xml> <output.csv> <schema.xsd>(optional)')
    exit()
try:
    etree_xmlfile = etree.parse(xml_input_file)
except:
    sys.stderr.write('ERROR: Parsing .xml file with lxml-lib failed.')
    exit()

export_data_to_csv(etree_xmlfile, csv_output_filename)

if len(sys.argv) == 4:
    validate_agains_schema(xsd_input_file, etree_xmlfile)
else:
    print('Nothing to validate because no .xsd schema given.')
