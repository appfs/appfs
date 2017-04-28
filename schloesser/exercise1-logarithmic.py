#!/usr/bin/env python

import sys
import math

filename = sys.argv[1]

with open(filename, "r") as f:
	valid = [0,0,0]
	mean = [0,0]
	for i, line in enumerate(f):
	    tmp = line.split("\n")[0].split("#")[0].split("; ")
	    valid[0] = valid[0]+1
	    if len(tmp) < 3:
	        continue
	    try:
	        pos = int(tmp[1])
	        val = float(tmp[2])
	        if (pos == 1 or pos == 2) and val > 0:
	            mean[pos-1] = mean[pos-1] + math.log(val)
	            valid[pos] = valid[pos]+1
	    except ValueError:
	        continue

print("File: " + str(filename) + " with " + str(valid[0]) + " lines")
for i in range(2):
    mean[i] = math.exp(mean[i]/valid[i+1])
    print("Valid values Loc" + str(i) + ": " + str(valid[i+1]) + " with GeoMean: " + str(mean[i]))

