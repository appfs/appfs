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
num_lines=0
filepath=sys.argv[1]
with open(filepath) as f:
    for line in f:
        line_split=line.split(';')
        if not line.startswith('#'):
            try:
                value=float(line_split[2])
                if not isnan(value) and value>0.0:
                    if int(line_split[1])==1:
                        location1.append(value)
                    elif int(line_split[1])==2:
                        location2.append(value)

            except ValueError:
                num_lines+=1
                continue
            except IndexError:
                num_lines+=1
                continue
        #num_lines+=1
len1=len(location1)
len2=len(location2)

print('File: '+filepath+' with '+str(num_lines+len1+len2)+' lines')
print('Valid values Loc0: ' +str(len1)+' with '+'GeoMean: '+str(gmean(list(location1))))
print('Valid values Loc1: ' +str(len2)+' with '+'GeoMean: '+str(gmean(list(location2))))

            
