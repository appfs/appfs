#!/usr/bin/env python

## @package exercise 1 + 4
#  This package provides means to solving exercises 1 and 4 from the lecture appfs

import numpy as np
import scipy, scipy.stats
import sys

## Exercise 1 class
#  This class provides methods for completing the first exercise of the lecture appfs, summer 2017, TUB
class Ex1:
    
    ## Constructor
    #  
    #  @param string the name of the file containing the data. Each line has format:
    #      seq; loc; val
    #  where seq is a sequence number, loc is either 1 or 2 and val is a value > 0.
    #  Empty lines or a line starting with '#' is treated as comment and ignored.
    def __init__(self, string):
        ## store the filename
        self.filename = string;
        ## store [number of lines, number of positions in location1, number of positions in location2]
        self.total = 0
        ## store values in loc1, loc2 respectively
        self.loc = [[],[]]
        ## store number of values in locations
        self.n = []
        ## store the geometric mean of both locations
        self.mean = []
        
    ## Parse a file
    #  Write to total and loc the number of lines resp. the values in both locations
    def parse_file(self):
        # open file
        with open(self.filename, "r") as f:
            last = 0
            # loop over lines in file
            for i, line in enumerate(f):
                # split the lines into the 3 values we are interested in
                tmp = line.split("\n")[0].split("#")[0].split("; ")
                # increase number of lines
                self.total = self.total+1
                # line broken if we get back less than three values
                if len(tmp) < 3:
                    # next line
                    if not line[0] == '#' and not line == "\n":
                        print("ERROR: Line {} does not have required format or is comment:\n{}".format(self.total, line))
                    continue
                try:
                        # try converting strings into numbers
                    seq = int(tmp[0])
                    if seq != last+1: 
                        print("WARNING: Something wrong with sequence numbers in line {}. Expected {} but found {}:\n{}".format(self.total, last+1, seq, line))
                    last = seq

                    pos = int(tmp[1])
                    val = float(tmp[2])
                    # if these numbers are matching our format, then store them
                    if val>0 and (pos == 1 or pos == 2): 
                        self.loc[pos-1].append(val)
                        continue
                    else: 
                        print("ERROR: Values of one or more numbers in line {} do not satisfy requirements:\n{}".format(self.total, line))
                # if something fails, then the line was broken
                except ValueError:
                    print("ERROR: Not all numbers in line {} satisfy requirements:\n{}".format(self.total, line))
                    continue
        
    ## Print information
    #  print collected information to stdout such as filename, number of lines and values and the geometric means.
    def print_info(self):
        # output information
        print("File: " + str(self.filename) + " with " + str(self.total) + " lines")
        # for both locations calculate mean and output info
        for i in range(2):
            print("Valid values Loc" + str(i+1) + ": " + str(self.n[i]) + " with GeoMean: " + str(self.mean[i]))
    
    ## Calculate geometric mean
    #  calculate the geometric means of the values in the lists in loc
    def calculate_means(self):
        for i in range(2):
            self.n.append(len(self.loc[i]))
            self.mean.append(scipy.stats.mstats.gmean(np.asarray(self.loc[i])))
    
## Mainmethod
#  run when class is executed, but not after import
if __name__=="__main__":
    # read filename from commandlineargs
    filename = sys.argv[1]
    e = Ex1(filename)

    # parse file, calculate means and output result to user
    e.parse_file()
    e.calculate_means()
    e.print_info()

