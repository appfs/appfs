#!/usr/bin/env python
# encoding: UTF-8

"""
Get the geometric mean of a list of nums.

@FriedrichDinsel
"""

import numpy as np
import sys
import time
import datetime
from scipy.stats.mstats import gmean


def getData(fname):
    """."""
    vals1 = []
    vals2 = []
    errors = []
    with open(fname, 'r') as infile:
        for line in infile:
            # line --> array
            data = line.strip().split(';')
            try:
                # check if idx is valid or throw exception
                idx = int(data[0])
                num = np.float(data[2])
                if data[1].strip() == '1' and num > 0:
                    vals1.append(num)
                elif data[1].strip() == '2' and num > 0:
                    vals2.append(num)
                else:
                    raise ValueError("Found other location than 1 or 2")
            except (ValueError, IndexError, AttributeError):
                errors.append(line)

    return vals1, vals2, errors


def hTime(num):
    """Return a string of human readable time."""
    return str(datetime.timedelta(seconds=num))


def getGMean(args):
    """Calculate the geometric mean of file."""
    # start timing
    # cython = False
    t0 = time.time()
    if args.cython:
        import cGetData
        vals1, vals2, errorLines = cGetData.getData(args.file)
    else:
        vals1, vals2, errorLines = getData(args.file)
    print("got arrays in {}".format(hTime(time.time() - t0)))
    print("File {} contained {} lines.".format(args.file, len(vals1) + len(vals2) + len(errorLines)))
    print("{} valid values of loc1 with GeoMean: {}".format(len(vals1), gmean((vals1))))
    print("{} valid values of loc2 with GeoMean: {}".format(len(vals2), gmean((vals2))))

    t1 = time.time()
    print("{} took {} to solve.".format(">"*10, hTime(t1-t0)))


def main(argv):
    """."""
    import argparse
    parser = argparse.ArgumentParser(
        formatter_class=argparse.RawDescriptionHelpFormatter,
        description=
            '''\
                     EXERCISE 1
            ----------------------------
            Script to read a large file,
            sort out falsified lines and
            calculate the geometric mean
            of the values after location
            '''
        )

    parser.add_argument('file', help='file to read', action='store')

    parser.add_argument('-c', '--cython', help='use the cython lib (not as fast as expected)', action='store_true', dest='cython')

    getGMean(parser.parse_args())


if __name__ == '__main__':
    main(sys.argv[1:])
