#!/usr/bin/env python

"""
Exercise 1 + 4

This package provides means to solving exercises 1 and 4 from the lecture appfs, summer 2017, TUB

The size of the inputfile is around 2GB.
Each line of the inputfile should consist of

   - a sequence-number,
   - a location (1 or 2),and
   - a floating point value > 0.

Empty lines are allowed. Comment lines start with a ”#”.
Everything after a ”’#” on a line should be ignored.

This module reads in and internally stores the data from the inputfile.

Computes the geometric mean for each location and gives output in a form like this:

    "File: ex1.dat with 100001235 lines
    Valid values Loc1: 50004598 with GeoMean: 36.7817
    Valid values Loc2: 49994703 with GeoMean: 36.7825"
"""

import sys
import numpy as np
from scipy import stats

class Ex1:
    """
    Exercise 1 class

    This class provides methods for completing the first exercise of the lecture appfs,

    Attributes:
        filename: store the filename
        total: store [number of lines, number of positions in location1, number of positions in location2]
        loc: store values in loc1, loc2 respectively
        n: store number of values in locations
        mean: store the geometric mean of both locations
    """

    def __init__(self, string):
        """
        Constructor

        Args:
            self: the object pointer
            string (str): the name of the file containing the data
        """
        self.filename = string
        self.total = 0
        self.loc = [[], []]
        self.n = []
        self.mean = []

    def parse_file(self):
        """
        Parse a file

        Write to total and loc the number of lines resp. the values in both locations

        Args:
            self: the object pointer
        """
        # open file
        with open(self.filename, "r") as f:
            last = 0
            # loop over lines in file
            for line in f:
                # split the lines into the 3 values we are interested in
                tmp = line.split("\n")[0].split("#")[0].split("; ")
                # increase number of lines
                self.total = self.total+1
                # line broken if we get back less than three values
                if len(tmp) < 3:
                    # next line
                    if not line[0] != '#' and not line != "\n":
                        msg = "ERROR: Line {} does not have required format or is comment:\n{}"
                        print(msg.format(self.total, line))
                    continue
                try:
                        # try converting strings into numbers
                    seq = int(tmp[0])
                    if seq != last+1:
                        msg = "WARNING: Error in sequence numbers in line {}. Expected {} but found {}:\n{}"
                        print(msg.format(self.total, last+1, seq, line))
                    last = seq

                    pos = int(tmp[1])
                    val = float(tmp[2])
                    # if these numbers are matching our format, then store them
                    if val > 0 and (pos == 1 or pos == 2):
                        self.loc[pos-1].append(val)
                        continue
                    else:
                        msg = "ERROR: One or more values from line {} do not satisfy requirements:\n{}"
                        print(msg.format(self.total, line))
                # if something fails, then the line was broken
                except ValueError:
                    msg = "ERROR: One or more numbers in line {} satisfy requirements:\n{}"
                    print(msg.format(self.total, line))
                    continue

    def print_info(self):
        """
        Print information

        print collected information to stdout such as filename, number of lines and values
        and the geometric means.

        Args:
            self: the object pointer
        """
        # output information
        print("File: " + str(self.filename) + " with " + str(self.total) + " lines")
        # for both locations calculate mean and output info
        for i in range(2):
            print("Valid values Loc {}: {} with GeoMean: {}".format(i+1, self.n[i], self.mean[i]))

    def calculate_means(self):
        """
        Calculate geometric mean

        calculate the geometric means of the values in the lists in loc

        Args:
            self: the object pointer
        """
        for i in range(2):
            self.n.append(len(self.loc[i]))
            self.mean.append(stats.mstats.gmean(np.asarray(self.loc[i])))

if __name__ == "__main__":
    # read filename from commandlineargs
    filename = sys.argv[1]
    ex = Ex1(filename)

    # parse file, calculate means and output result to user
    ex.parse_file()
    ex.calculate_means()
    ex.print_info()
    print("")
