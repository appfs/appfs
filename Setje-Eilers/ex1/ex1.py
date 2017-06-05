# -*- coding: utf-8 -*-
"""

Created on Tue Apr 25 19:04:53 2017

@author: mona
"""

#from scipy.stats.mstats import gmean
from math import isnan, exp, log
from collections import deque
import sys
from sys import float_info
#computer the geometric mean

def geomean(x):
    """computes geometric mean
    """
    return exp(sum( log(i) for i in x) / len(x))


location=[[] for _ in range(2)]
filepath=sys.argv[1]
empty_lines=0
with open(filepath) as f:
    for n,line in enumerate(f):
        line_split=line.split(';')
        if (line_split[0].strip()==""):
            #empty_lines+=1            
            continue
        if line.startswith('#'):
            #print('Comment line: '+line)
            #empty_lines+=1
            continue
        if len(line_split)!=3:
            #print('Wrong length: '+line)
            continue
        try:
            value=float(line_split[2])
            loc_num=int(line_split[1])
            id_num=int(line_split[0])
        except ValueError:
            #print('Value Error: '+line)
            continue      
        except OverflowError:
            #print('Overflow Error: '+line)
            continue        
        if isnan(value) or value<=0.0 or float_info.max<value or (float_info.min>value):
            #print('Value out of range: '+line)
            continue
        if loc_num not in [1,2]:
            #print('Wrong location number: ' +line)
            continue
        location[loc_num-1].append(value)
	#optional: check if sequence number is correct. 
        #if(n-empty_lines+1!=id_num):
            #print('wrong sequence number: '+str(id_num)+' expected:'+str(n-empty_lines+1))
            #empty_lines=n-id_num+1
        
        


#print(location)
print('File: '+filepath+' with '+str(n)+' lines')
len0=len(location[0])
len1=len(location[1])
if len0>0:
    print('Valid values Loc1: ' +str(len0)+' with '+'GeoMean: '+str(geomean(location[0])))
else:
    print('Valid values Loc1: 0')
if len1>0:
    print('Valid values Loc2: ' +str(len1)+' with '+'GeoMean: '+str(geomean(location[1])))
else:
    print('Valid values Loc2: 0')            
