#/usr/bin/env python3
import numpy as np

def getData(fname):
    """."""
    loc1 = 0
    loc2 = 0
    c1 = 0
    c2 = 0
    # e = 0
    with open(fname) as infile:
        for line in infile:
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
                    # AttributeError - ['']
                    # e += 1
                    continue

    # print("{} exceptions were made due to errors.".format(e))
    return (loc1, c1), (loc2, c2)
