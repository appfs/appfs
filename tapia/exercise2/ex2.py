"""@package docstring
Author: Roxana Tapia Campos
Module: Advanced practical programming skills for scientists
SS17 - TU Berlin 
Exercise 2: XML to CSV measured
"""

#!/usr/bin/env python
# -*- coding: utf-8 -*-

import sys

from bs4 import BeautifulSoup


def xml_to_csv(data):
    """
    Searches all suitable values (tags) to build a response with format:
    YYYY-MM-DD; HH; amountOfPower-Value
    
    :param data: Parsed data from XML file as a tree format.
    """
    output = list()
    gas_days = data.find_all('gasDay')

    for gas_day in gas_days:
        date = gas_day.get('date')
        boundary_nodes = gas_day.find_all('boundaryNode')
        for boundary_node in boundary_nodes:
            times = boundary_node.find_all('time')
            for time in times:
                hour = time.get('hour')
                amounts_of_power = time.find_all('amountOfPower')
                for amounts_of_power in amounts_of_power:
                    value = amounts_of_power.get('value')

                    record = "{date}; {hour}; {value}".format(date=date,hour=hour,value=value)
                    output.append(record)

    write_csv(output)


def write_csv(content_list):
    """
    Writes a CSV 'output.csv' file with the formatted result.
    
    :param content_list: A list with triplets.
    """
    with open('output.csv','w') as file:
        for line in content_list:
            file.write(line)
            file.write('\n')

if __name__ == '__main__':
    filename = sys.argv[-1]
    xml_text = open(filename).read()
    parsed_data = BeautifulSoup(xml_text, 'xml')

    xml_to_csv(parsed_data)
