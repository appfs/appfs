# -*- coding: utf-8 -*-
"""
Created on Mon May 01 23:02:52 2017

@author: dom
"""

from sys import argv


def isvalid(line):
# Checks if a read line can be used
    if line[0] == '#':
        return False
    elem = line.split(';')
    if not len(elem) == 3:
        return False   
    if not float(elem[2]) > 0:
        return False
    
    return elem


# Input
#_, filename = argv
filename='ex1-100.dat'
txt = open(filename)

# Params
i = 0                   # total line count
n = [0,0]               # valid lines per location
gmean = [1.,1.]         # geometric mean per location

# Loop over lines
for line in txt:
    i+=1
    try:
        elem = isvalid(line)
        if not elem == False:
            loc = int(elem[1])-1
            value = float(elem[2])
            gmean[loc] = gmean[loc]**(n[loc]/(n[loc]+1.)) * value**(1./(n[loc]+1.))
            n[loc] += 1
    except:
        pass


# Output
print 'File: %s with %i valid lines' %(filename, i)
print 'Valid values Loc1: %i with GeoMean: %.3f' %(n[0], gmean[0])
print 'Valid values Loc2: %i with GeoMean: %.3f' %(n[1], gmean[1])
     
txt.close()