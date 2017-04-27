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
    # e = 0
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
            except (IndexError, ValueError, AttributeError):
                # FIXME: this:
                # e += 1
                continue
    # print("{} exceptions were made due to errors.".format(e))
    return vals1, vals2


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
        vals1, vals2 = cGetData.getData(args.file)
    else:
        vals1, vals2 = getData(args.file)
    print("got arrays in {}".format(hTime(time.time() - t0)))

    print("{} valid values of loc1 with GeoMean: {}".format(len(vals1), gmean((vals1))))
    print("{} valid values of loc2 with GeoMean: {}".format(len(vals2), gmean((vals2))))
    # print("{} valid values of loc1 with GeoMean: {}".format(vals1[1], np.exp(vals1[0]/vals1[1])))
    # print("{} valid values of loc2 with GeoMean: {}".format(vals2[1], np.exp(vals2[0]/vals2[1])))

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
