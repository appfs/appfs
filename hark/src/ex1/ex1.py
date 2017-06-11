import math
import sys
## @package ex1
#  This script is a solution for exercise 1 from the advanced programming course in the sommer 2017 at TUB.
# For more information see @link http://www.zib.de/koch/lectures/ss2017_appfs.php

floating_point_upper_bound = sys.float_info.max

## This method takes care of the error handling during the parsing of a line of the csv-file.
# @param line_as_array: line split into array at each ";".
# @param line_number: current line number for the case of error printing including the line number
# @param stripped: line split into array at each ";" and strip (removed white spaces) afterwards.
# @return: False if the location is not in {0,1}, the location value is not a number, there is a parsing error or there is an floating point underflow/overflow
def is_valid_line(line_as_array, line_number, stripped):
    try:
        location = int(line_as_array[1])
        location_value = float(line_as_array[2])
        if location not in [1, 2]:
            print("Line {} : Illegal location: {}".format(line_number + 1, location))
            return False
        if math.isnan(location_value):
            print("Line {} : Invalid value (NaN)".format(line_number + 1))
            return False
        if int(line_as_array[2].split(".")[0]) > floating_point_upper_bound:
            print("Line {} : Floating point overflow".format(line_number + 1, location_value))
            return False
        if int(location_value <= 2.2250738585072014e-308):
            print("Line {} : Floating point underflow".format(line_number + 1, location_value))
            return False
    except ValueError:
        print("Line {} : Syntax error [{}]".format(line_number + 1, stripped))
        return False
    return location, location_value


## This method opens up the specified file and tries to compute
# the geometric mean value of the two locations in {0,1}.
# @param filename: the expected csv-like file separated with colons.
# @return number of valid lines of the specified file,
# valid locations of location 1,
# valid locations of location 2,
# geometric mean of location values 1 or
# geometric mean of location values 2
def compute_geometric_mean_from_csv(filename):
    valid_counts = [0, 0]
    means = [0.0, 0.0]
    previous_sequence_number = 0
    sequence_number = 0
    with open(filename, "r") as file:
        for line_count, line in enumerate(file):
            line = line.split("#")[0]

            stripped_line = line.strip()
            if stripped_line == "":
                continue

            split_line = stripped_line.split(";")

            if len(split_line) != 3:
                print("Line {} : Syntax error [{}]".format(line_count + 1, split_line))
                continue

            if is_valid_line(split_line, line_count, stripped_line) is not False:
                location, value = is_valid_line(split_line, line_count, stripped_line)
                try:
                    sequence_number = int(split_line[0])
                except ValueError:
                    print("Line {} : Sequence consecutive error [{}]".format(line_count + 1, split_line))
                    sequence_number = -1
                if sequence_number == previous_sequence_number+1 or (line_count == 0 and sequence_number == 1):
                    valid_counts[(location - 1)] += 1
                    means[(location - 1)] += math.log(value)
            previous_sequence_number = sequence_number
        if valid_counts[0] > 0:
            means[0] = math.exp(means[0] / valid_counts[0])
        else:
            means[0] = 1
        if valid_counts[1]:
            means[1] = math.exp(means[1] / valid_counts[1])
        else:
            means[1] = 1
    return line_count, valid_counts[0], valid_counts[1], means[0], means[1]


lineCount, count1, count2, geo_mean1, geo_mean2 = compute_geometric_mean_from_csv(sys.argv[1])

print('File:', sys.argv[1], 'with', lineCount, 'lines')
print('Valid values Loc1:', count1, 'with GeoMean:', format(geo_mean1, '.32f'))
print('Valid values Loc2:', count2, 'with GeoMean:', format(geo_mean2, '.32f'))
