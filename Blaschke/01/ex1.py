# -*- coding: utf-8 -*-
"""
Created on Wed Apr 26 16:34:16 2017

@author: Lana
"""
import sys
import csv

def checkRow(row):
    # check if a row has 3 entries before any comments

    # empty lines are ok
    if not row:
        return [0, 0, 0]
    
    # if not empty, line must have 3 entries with
    # 1.) sequence number 2.) location {1,2} 3.) float > 0
    if len(row) == 3:
        loc = checkLoc(row[1])
        val = checkVal(row[2])
        if checkRow(row[0]) and loc and val:
            return [1, loc, val]
    
    return 0
    
    
def checkSeq(entry):
    # try if first entry is a (positive) sequence number
    try:
        if int(entry) > 0:
            return int(entry)
    except:
        return 0
    
def checkLoc(entry):
    # check if second entry in the line is a location {1,2}
    try:
        if int(entry) == 1 or int(entry) == 2:
            return int(entry)
    except:
        return 0
    
def checkVal(entry):
    # check if third entry in the line is a float > 0:
    try:
        if float(entry) > 0:
            return float(entry)
    except:
        return 0


def gMeanUpdate(currMean, value, n):
    # geometric mean defined as
    # (x1*x2*...*xn)^(1/n)
    #            = ((x1*x2*...*x(n-1))^(1/(n-1))^(n-1) * xn)^(1/n)
    #            = ((currMean)^(n-1) * value)^(1/n)
    return currMean**((n-1.)/n) * value**(1./n)



filename = sys.argv[1]#'ex1.dat'

nrRow = 0
nrLoc = [0, 0]
gMean = [1., 1.]


with open(filename, 'r') as dat:
    dat = csv.reader(dat, delimiter = ';')
    for row in dat:
        row = checkRow(row)
        if row:
            nrRow = nrRow + 1
            if row[0]:
                loc = row[1] - 1
                nrLoc[loc] = nrLoc[loc] + 1
                gMean[loc] = gMeanUpdate(gMean[loc], row[2], nrLoc[loc])
#            loc = checkLoc(row[1])
#            if loc and checkSeq(row[0]):
#                val = checkVal(row[2])
#                if val:
#                    nrLoc[loc-1] = nrLoc[loc-1] + 1
#                    gMean[loc-1] = gMeanUpdate(gMean[loc-1], val, nrLoc[loc-1])

print 'File: %s with %i valid lines'%(filename, nrRow)
print 'Valid values	Loc1: %i with GeoMean: %.3f' %(nrLoc[0], gMean[0])
print 'Valid values	Loc2: %i with GeoMean: %.3f' %(nrLoc[1], gMean[1])
    