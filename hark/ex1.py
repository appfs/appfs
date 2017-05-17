import sys
import math


def is_valid_line(line_as_array, line_number, stripped):
    try:
        int(line_as_array[0])
        location = int(line_as_array[1])
        location_value = float(line_as_array[2])
        if location not in [1, 2]:
            print("Line {} : Illegal location: {}".format(line_number + 1, location))
            return False
        if math.isnan(location_value) or location_value <= 0.0:
            print("Line {} : Invalid value".format(line_number + 1))
            return False
    except ValueError:
        print("Line {} : Syntax error [{}]".format(line_number + 1, stripped))
        return False
    return location, location_value


def compute_geometric_mean_from_csv(filename):
    valid_counts = [0, 0]
    means = [0.0, 0.0]
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
                    valid_counts[(location-1)] += 1
                    means[(location-1)] += math.log(value)
        means[0] = math.exp(means[0]/valid_counts[0])
        means[1] = math.exp(means[1]/valid_counts[1])
    return line_count, valid_counts[0], valid_counts[1], means[0], means[1]

lineCount, count1, count2, geo_mean1, geo_mean2 = compute_geometric_mean_from_csv(sys.argv[1])

print('File:', sys.argv[1], 'with', lineCount, 'lines')
print('Valid values Loc1:', count1, 'with GeoMean:', geo_mean1)
print('Valid values Loc2:', count2, 'with GeoMean:', geo_mean2)
