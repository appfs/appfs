import numpy as np
import sys

def RepresentsInt(s):
    try:
        int(s)
        return True
    except ValueError:
        return False

def RepresentsFloat(s):
    try:
        float(s)
        return True
    except ValueError:
        return False

filename = sys.argv[1]
f = open(filename)
loc1 = []
loc2 = []
counter = 0
for line in f:
    counter += 1
    if line and line[0] != '#':
        numbers = (line.split('; '))
        if len(numbers) == 3:
            if numbers[2][-2:] == '/n':
                numbers[2] = numbers[2][:-2]
            if RepresentsInt(numbers[1]) and RepresentsFloat(numbers[2]):
                loc = int(numbers[1])
                value = float(numbers[2])
                if loc == 1 and value > 0:
                    loc1.append(value)
                elif loc == 2 and value > 0:
                    loc2.append(value)

geo_mean_1 = np.product(np.power(np.asarray(loc1),(1/len(loc1))))
geo_mean_2 = np.product(np.power(np.asarray(loc2),(1/len(loc2))))
print('File: ', filename, 'with ', counter, ' lines (containing data)')
print('Valid Values Loc1: ', len(loc1), ' with gemetric mean: ', geo_mean_1)
print('Valid Values Loc2: ', len(loc2), ' with gemetric mean: ', geo_mean_2)
