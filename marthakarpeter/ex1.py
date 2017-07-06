#!/usr/bin/env python3

#ex1 APPFS Martha Karpeter 367847


# File ex1-100.dat with 100001235 lines
# Valid values Loc1: 50004466 with GeoMean: 36.781736117203764
# Valid values Loc2: 49994581 with GeoMean: 36.78258320024247

# time 8m18.949s


import sys
import math

file = open(sys.argv[-1], "r").readlines()

valid = [0,0,0]
GMean = [0,0]

for line in file:
    valid[0] += 1
    # ignore everything after #
    Data1 = line.split("\n")[0].split("#")[0].strip()

    # ignore empty lines
    if Data1 == "":
        continue

    Data = Data1.split(";")

    # through error if line does not have 3 semicolons
    if len(Data) != 3:
        print(line)
        continue

    # through error if line number, location and value are not integer or float (i.e. contain a space)
    try:

        lin = int(Data[0])
        loc = int(Data[1])
        val = float(Data[2])

    except ValueError:
        print(line)
        continue

    # through error if value is not a number
    if math.isnan(val):
        print(line)
        continue

    # through error if location is other than 1 or 2
    if (loc != 1 and loc != 2) or val <= 0.0:
        print(line)
        continue
    else:
    # seperate and add values according to location
        valid[loc] += 1
        GMean[loc-1] += math.log(val,2)


# compute geometric mean for locations 1 and 2 seperately
for i in [1,2]:
    GMean[i-1] = pow(2,(GMean[i-1]/valid[i]))


# output
print("File", sys.argv[-1], "with", valid[0], "lines")
print('Valid values Loc1:', valid[1], 'with GeoMean:', GMean[0]) # "%.4f"%GMean[0])
print('Valid values Loc2:', valid[2], 'with GeoMean:', GMean[1]) # "%.4f"%GMean[1])
