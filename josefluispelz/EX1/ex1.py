#!/usr/bin/env python
import sys
import math

def geom_mean(value_list):
    return math.pow(2, sum(math.log(value, 2) for value in value_list)/len(value_list))

loc_value = [[], []]

with open(sys.argv[-1], "r") as file:

    for line_number, line in enumerate(file):

        # check for comments
        line = line.split("#")[0]
        line = line.strip()

        # skip empty lines
        if line == "":
            continue

        fields = line.split(";")

        if 3 != len(fields):
            print("Syntax error      in line {}: {}".format(line_number+1, line))

        try:
            seq = int(fields[0])
            loc = int(fields[1])
            val = float(fields[2])

        except ValueError:
            print("Syntax error      in line {}: {}".format(line_number+1, line))
            continue

        # check for invalid value
        if math.isnan(val) or val <= 0.0:
            print("Invalid value     in line {}".format(line_number + 1))
            continue

        if loc not in [1, 2]:
            print("Invalid location  in line {}".format(line_number + 1))
            continue

        loc_value[loc-1].append(val)

# print results

print("File: {} with {} lines".format(sys.argv[-1], line_number+1))
for loc in [0, 1]:
    print("Valid values Loc1: {} ".format(len(loc_value[loc])) + "with GeoMean: {0:.4f}".format(geom_mean(loc_value[loc])))


# EOF