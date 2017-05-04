import sys
import csv
import numpy as np

def is_valid_value(str_value):
    try:
        if np.isnan(float(str_value)) or float(str_value) <= 0:
            return False
    except:
        return False
    return True

def compute_geometric_mean_from_csv(filename):
    with open(filename, 'r') as csvfile:
        line_reader = csv.reader(csvfile, delimiter=";", skipinitialspace=True)
        line_count = 0
        valid_count1 = 0
        valid_count2 = 0
        mean1 = 0
        mean2 = 0
        for line in line_reader:
            line_count += 1
            if line:
                if len(line) == 3 and line[0][0] != '#':
                    if line[1] == '1' and is_valid_value(line[2]):
                            valid_count1 += 1
                            mean1 += np.log(float(line[2]))
                    elif line[1] == '2' and is_valid_value(line[2]):
                            valid_count2 += 1
                            mean2 += np.log(float(line[2]))
        mean1 = np.exp(mean1/valid_count1)
        mean2 = np.exp(mean2/valid_count2)
    return line_count, valid_count1, valid_count2, mean1, mean2

lineCount, count1, count2, geo_mean1, geo_mean2 = compute_geometric_mean_from_csv(sys.argv[1])

print('File:', sys.argv[1], 'with', lineCount, 'lines')
print('Valid values Loc1:', count1, 'with GeoMean:', geo_mean1)
print('Valid values Loc2:', count2, 'with GeoMean:', geo_mean2)

# output
# File: ex1.dat with 100001235 lines
# Valid values Loc1: 50004616 with GeoMean: 36.7818
# Valid values Loc2: 49994716 with GeoMean: 36.7826