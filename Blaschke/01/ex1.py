# -*- coding: utf-8 -*-
"""
Created on Wed Apr 26 16:34:16 2017

@author: Lana
"""
import sys
import csv

def checkRow(row, seqNr):
    # check if a row has 3 entries before any comments

    # empty lines are ok
    if not row:
        return [0, 0, 0]
    
    # if not empty, line must have 3 entries with
    # 1.) sequence number 2.) location {1,2} 3.) float > 0
    if len(row) == 3:
        seq = checkSeq(row[0], seqNr)
        loc = checkLoc(row[1])
        val = checkVal(row[2])
        if seq and loc and val:
            return [seq, loc, val]
    
    return 0
    
    
def checkSeq(entry, seqNr):
    # try if first entry is the next sequence number (or the first at all)
    try:
        if seqNr == 0 or int(entry) == seqNr + 1:
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



filename = 'ex1.dat'

nrRow = 0
nrLoc = [0, 0]
gMean = [1., 1.]
seqNr = 0


with open(filename, 'r') as dat:
    dat = csv.reader(dat, delimiter = ';')
    for nrow in dat:
        row = checkRow(nrow, seqNr)
        if row:
            nrRow = nrRow + 1
            if row[0]:
                seqNr = row[0]
                loc = row[1] - 1
                nrLoc[loc] = nrLoc[loc] + 1
                gMean[loc] = gMeanUpdate(gMean[loc], row[2], nrLoc[loc])

print 'File: %s with %i valid lines'%(filename, nrRow)
print 'Valid values	Loc1: %i with GeoMean: %.3f' %(nrLoc[0], gMean[0])
print 'Valid values	Loc2: %i with GeoMean: %.3f' %(nrLoc[1], gMean[1])
    