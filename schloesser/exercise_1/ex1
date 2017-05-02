#!/usr/bin/env python

import numpy as np
import scipy, scipy.stats
import sys

# read filename from commandlineargs
filename = sys.argv[1]

# open file
with open(filename, "r") as f:
    # store [number of lines, number of positions in location1, number of positions in location2]
    valid = [0,0,0]
    # store values in loc1, loc2 respectively
    loc = [[],[]]
    # store the geometric mean of both locations
    mean = []

	# loop over lines in file
    for i, line in enumerate(f):
    	# split the lines into the 3 values we are interested in
        tmp = line.split("\n")[0].split("#")[0].split("; ")
        # increase number of lines
        valid[0] = valid[0]+1
        # line broken if we get back less than three values
        if len(tmp) < 3:
        	# next line
            continue
        try:
			# try converting strings into numbers
            pos = int(tmp[1])
            val = float(tmp[2])
            # if these numbers are matching our format, then store them
            if val>0 and (pos == 1 or pos == 2): 
                loc[pos-1].append(val)
        # if something fails, then the line was broken
        except ValueError:
            continue

# output information
print("File: " + str(filename) + " with " + str(valid[0]) + " lines")
# for both locations calculate mean and output info
for i in range(2):
    valid[i+1] = len(loc[i])
    mean.append(scipy.stats.mstats.gmean(np.asarray(loc[i])))
    print("Valid values Loc" + str(i) + ": " + str(valid[i+1]) + " with GeoMean: " + str(mean[i]))

