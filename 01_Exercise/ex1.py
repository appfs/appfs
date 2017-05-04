# TK 17Apr2017

import sys
import math

def geom_mean(value_list):
    return math.exp(sum( math.log(x) for x in value_list) / len(value_list))

loc_value = [[], []]

with open(sys.argv[1], "r") as f:

    # enumerate liefert dir die Zeilenummer automatisch. Die started dann aber bei 0.
    for lineno, line in enumerate(f):

        # Check for comment 
        line = line.split("#")[0]

        stripped = line.strip()
 
        # Ignore empty lines
        if stripped == "" :
            continue;

        fields = stripped.split(";")

        if len(fields) != 3 :
            print("Line {} : Syntax error [{}]".format(lineno + 1, stripped));
            continue;

        try:
            seq = int(fields[0])
            loc = int(fields[1])
            val = float(fields[2])

        except ValueError:
            print("Line {} : Syntax error [{}]".format(lineno + 1, stripped));
            continue;

        if loc not in [1, 2]:
            print("Line {} : Illegal location: {}".format(lineno + 1, loc));
            continue

        if math.isnan(val) or val <= 0.0 :
            print("Line {} : Invalid value".format(lineno + 1));
            continue

        loc -= 1

        loc_value[loc].append(val)

# Print statistics

print ("File: ", sys.argv[1], "with", lineno, "lines");

for loc in [0, 1]:
    print ("Valid values Loc", loc + 1, ": ", len(loc_value[loc]), "with GeoMean:", geom_mean(loc_value[loc]));

# EOF
