#!/usr/bin/env python
# encoding: UTF-8

"""
Get the geometric mean of a list of nums.

@FriedrichDinsel
"""

import numpy as np
import sys
import time, datetime



def getData(fname):
    """."""
    loc1 = 0
    loc2 = 0
    c1 = 0
    c2 = 0
    # e = 0
    with open(fname, 'r') as infile:
        for line in infile:
            # e += 1
            if not line.startswith('#'):
                try:
                    # line --> array
                    data = line.strip().split(';')
                    # check if idx in valid
                    idx = int(data[0])
                    # check if value is valid
                    num = np.log(np.float(data[2]))
                    # check for location and if num is NaN
                    if data[1].strip() == '1' and not np.isnan(num):
                        loc1 += num
                        c1 += 1
                    elif data[1].strip() == '2' and not np.isnan(num):
                        loc2 += num
                        c2 += 1
                except (IndexError, ValueError, AttributeError):
                    # IndexError - missing ';'
                    # ValueError - NaN
                    # AttributeError
                    # FIXME: this:
                    # e += 1
                    continue
    # print("{} exceptions were made due to errors.".format(e))
    return (loc1, c1), (loc2, c2)


def hTime(num):
    """Return a string of human readable time."""
    return str(datetime.timedelta(seconds=num))


def getGMean(args):
    """Calculate the geometric mean of file."""
    # start timing
    if args.cython:
        try:
            import cGetData
            cython = True
        except ImportError:
            print("could not upload the compiled .so... but i could compile")
            cython = False
            # import os
            # os.system('python setup.py build_ext --inplace')
            # import cGetData
            # cython = True

    t0 = time.time()
    if cython:
        vals1, vals2 = cGetData.getData(args.file)
    else:
        vals1, vals2 = getData(args.file)
    print("got arrays in {}".format(hTime(time.time() - t0)))

    print("{} valid values of loc1 with GeoMean: {}".format(vals1[1], np.exp(vals1[0]/vals1[1])))
    print("{} valid values of loc2 with GeoMean: {}".format(vals2[1], np.exp(vals2[0]/vals2[1])))

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
