#/usr/bin/env python3
import numpy as np

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
