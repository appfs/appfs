# APPFS Exercise 2
# by Tam Tran
# in Python3

# 2 goals:
# 1) read XML to extract desired information and write it into an CSV
# 2) validate the XML against its XSD schema


# DEFINE FUNCTIONS HERE
import sys, csv
from bs4 import BeautifulSoup as Soup
from lxml import etree
#import pdb

def read_XML(file_XML):
    '''reads XML to extract desired information and outputs as list of lists'''
    '''filters out values that don't match CSV format, like nan or non-numerics'''
    
    # initialize
    list_data = []
    collect_date = ''
    collect_hour = ''
    collect_powervalue = ''
    
    
    with open(file_XML, 'r') as f:
        
        xmlsoup = Soup(f, 'lxml')

        # now swirling through this soup to collect the pieces we want
        # make sure to grab data in specified format
        for gasday in xmlsoup.find_all('gasday'): #soup lowers cases so your search term needs to be of lower case as well
            collect_date = gasday.attrs['date']
            
            # TODO
            # some formatting for date
        
            # going one level down at a time from gasday to gradually collect the tree values that we want
            for boundarynode in gasday.find_all('boundarynode'):
                for time in boundarynode.find_all('time'):
                    collect_hour = time.attrs['hour'].zfill(2) #so numerics can fit HH format
                        
                    if not collect_hour.isdigit():
                        continue

                    for power in time.find_all('amountofpower'):
                        collect_powervalue = power.attrs['value']
                        
                        # making sure only numeric power values are collected, whether positive or negative
                        if (not collect_powervalue.isdigit()) and (not (collect_powervalue[0] == '-' and collect_powervalue[1:].isdigit())):
                            continue
                        
                        # now that all desired data has been collected, concatenate it into one list per individual branch of the tree
                        list_data.append([collect_date, str(' ' + collect_hour), str(' ' + collect_powervalue)])

                        # could also just format the data as a string with ; in between, so that it will require less formatting in write_CSV()
                        # but then it would be too specific of a function, better to keep it general

    #print(len(list_data))
    return list_data # list of lists


def write_CSV(data, file_CSV):
    '''input of list of lists to write to CSV'''

    f = open(file_CSV, 'wt', encoding = 'utf-8')
    writer = csv.writer(f, delimiter = ';')
    #writer.writerow(['YYYY-MM-DD', ' HH', ' amountOfPower-Value'])

    for sublist in data:
        writer.writerow(sublist)




def validate_XML(file_XML, file_XSD):
    '''validate that XML file follows XSD'''
    
    xmlschema_doc = etree.parse(file_XSD)
    xmlschema = etree.XMLSchema(xmlschema_doc)
    
    xml_doc = etree.parse(file_XML)
    result = xmlschema.validate(xml_doc)
    
    if result:
        print('valid XML schema')
    else:
        raise ValueError('invalid XML schema')






# RUN FUNCTION CALLS HERE
input_XML = sys.argv[1]
output_CSV = sys.argv[2]
input_XSD = sys.argv[3]

data_extracted = read_XML(input_XML)
write_CSV(data_extracted, output_CSV)
validate_XML(input_XML, input_XSD)

# to validate that your written CSV will be read legibly
#ff = open(output_CSV, 'rt')
#reader = csv.reader(ff, delimiter = ';')
#for row in reader:
#    print(row)










