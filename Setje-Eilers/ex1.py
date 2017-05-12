# -*- coding: utf-8 -*-
"""
Created on Tue Apr 25 19:04:53 2017

@author: mona
"""

from scipy.stats.mstats import gmean
from math import isnan, exp, log
from collections import deque
import sys

def geomean(x):
    return exp(sum( log(i) for i in x) / len(x))

location1=deque()
location2=deque()
#num_lines=0
filepath=sys.argv[1]
with open(filepath) as f:
    for num_lines,line in enumerate(f):
        line_split=line.split(';')
        if not line.startswith('#') and len(line_split)==3:
            try:
                value=float(line_split[2])
                if not isnan(value) and value>0.0:
                    if int(line_split[1])==1:
                        location1.append(value)
                    elif int(line_split[1])==2:
                        location2.append(value)

            except ValueError:
                continue
#        num_lines+=1
len1=len(location1)
len2=len(location2)

print('File: '+filepath+' with '+str(num_lines)+' lines \n'+
'Valid values Loc0: ' +str(len1)+' with '+'GeoMean: '+str(geomean(list(location1)))+'\n'+
'Valid values Loc1: ' +str(len2)+' with '+'GeoMean: '+str(geomean(list(location2))))

            