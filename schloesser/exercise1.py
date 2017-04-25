#!/usr/bin/env python

import numpy as np
import scipy, scipy.stats

filename = "ex1-8.dat"

f = open(filename, "r")
valid = [0,0,0]
loc = [[],[]]
mean = []
for i, line in enumerate(f):
    tmp = line.split("\n")[0].split("#")[0].split("; ")
    valid[0] = valid[0]+1
    if len(tmp) < 3:
        continue
    if tmp[1] != "":
        try:
            tmp[1] = int(tmp[1])
            if tmp[1] == 1 or tmp[1] == 2:
                loc[1].append(tmp[1])
        except ValueError:
            pass
    if tmp[2] != "":
        try:
            tmp[2] = float(tmp[2])
            if tmp[2] > 0:
                loc[0].append(tmp[2])
        except ValueError:
            pass

for i in range(2):
    valid[i+1] = len(loc[i])
    mean.append(scipy.stats.mstats.gmean(np.asarray(loc[i])))

print("File: " + str(filename) + " with " + str(valid[0]) + " lines")
for i in range(2):
    print("Valid values Loc" + str(i) + ": " + str(valid[i+1]) + " with GeoMean: " + str(mean[i]))

