#!/usr/bin/env python

import numpy as np
import scipy, scipy.stats
import sys

filename = sys.argv[1]

f = open(filename, "r")
valid = [0,0,0]
loc = [[],[]]
mean = []
for i, line in enumerate(f):
    tmp = line.split("\n")[0].split("#")[0].split("; ")
    if len(tmp) < 3:
        continue
    if tmp[1] != "" and tmp[2] != "":
        try:
            pos = int(tmp[1])
            val = float(tmp[2])
            if (pos == 1 or pos == 2) and val > 0:
                loc[pos-1].append(val)
        except ValueError:
            continue
    valid[0] = valid[0]+1

for i in range(2):
    valid[i+1] = len(loc[i])
    mean.append(scipy.stats.mstats.gmean(np.asarray(loc[i])))

print("File: " + str(filename) + " with " + str(valid[0]) + " lines")
for i in range(2):
    print("Valid values Loc" + str(i) + ": " + str(valid[i+1]) + " with GeoMean: " + str(mean[i]))

