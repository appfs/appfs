"""@package docstring
Author: Roxana Tapia Campos
Module: Advanced practical programming skills for scientists
SS17 - TU Berlin 
Exercise 1: GeoMean
"""

import sys

import math

LINES_COUNT = 0

VALID_VALUES_LOC1 = 0
LOG_SUM_LOC1 = 0

VALID_VALUES_LOC2 = 0
LOG_SUM_LOC2 = 0


def read_file(filename):
    """
    Reads a file, counts its lines and yields each line.

    :param filename: A filename from command line.
    """
    global LINES_COUNT

    with open(filename) as f:
        for line in f:
            LINES_COUNT += 1
            if is_valid(line):
                yield line


def is_valid(line):
    """
    Returns True if the line is valid, False otherwise.
    
    :param line: A raw input line from the file.
    :rtype: Boolean
    """
    values = line.split(';')

    wrong_args_number = len(values) != 3
    is_empty_line = not line.strip()
    is_comment = line.startswith("#")

    if wrong_args_number or is_empty_line or is_comment:
        print('Wrong format, skipping line.')
        return False

    try:
        int(values[1])
        float_value = float(values[2])
        if math.isnan(float_value) or float_value <= 0.0:
            return False
    except ValueError:
        return False

    return True


def aggregate_data(line):
    """
    Aggregates line counts and logarithm calculations for locations 1 and 2.
    Checks if location is valid.

    :param line: A validated input line from the file.
    """
    global VALID_VALUES_LOC1, VALID_VALUES_LOC2, LOG_SUM_LOC1, LOG_SUM_LOC2

    values = line.split(';')
    location = int(values[1])
    point = float(values[2])

    result = math.log(point)

    if location == 1:
        VALID_VALUES_LOC1 += 1
        LOG_SUM_LOC1 += result
    elif location == 2:
        VALID_VALUES_LOC2 += 1
        LOG_SUM_LOC2 += result
    else:
        print('Location not supported.')


def compute_result():
    """
    Computes the geometric mean for locations 1 and 2 and displays the formatted result. 
    
    """

    geo_mean_loc1 = math.exp(LOG_SUM_LOC1/VALID_VALUES_LOC1)
    geo_mean_loc2 = math.exp(LOG_SUM_LOC2/VALID_VALUES_LOC2)

    print(math.isnan(geo_mean_loc1))
    print(math.isnan(geo_mean_loc2))

    print("File: {filename} with {lines_count} lines".format(
        filename=sys.argv[-1],
        lines_count=LINES_COUNT,
    ))
    print("Valid values Loc1: {valid_values_loc1} with GeoMean: {geo_mean_loc1}".format(
        valid_values_loc1=VALID_VALUES_LOC1,
        geo_mean_loc1=geo_mean_loc1
    ))
    print("Valid values Loc2: {valid_values_loc2} with GeoMean: {geo_mean_loc2}".format(
        valid_values_loc2=VALID_VALUES_LOC2,
        geo_mean_loc2=geo_mean_loc2
    ))

if __name__ == '__main__':

    content = read_file(sys.argv[-1])
    for line in content:
        aggregate_data(line)

    compute_result()

