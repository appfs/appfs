#ex1 APPFS Martha Karpeter 367847

import sys
import math

file = open(sys.argv[-1], "r").readlines()

valid = [0,0,0]
GMean = [0,0]

for line in file:
    Data = line.split("\n")[0].split("#")[0].split("; ")
    valid[0]+=1
    if len(Data)==3 and Data[2]!='NaN':
        try:
            loc = int(Data[1])
            val = float(Data[2])
            if (loc==1 or loc==2 and val>0):
                valid[loc]+=1
                GMean[loc-1]+=math.log(val,2)
                if math.isnan(GMean[loc-1]):
                    print(line)
                    break
        except:
            print(line)
            continue

print(GMean)
for i in range(2):
    GMean[i] = pow(2,(GMean[i]/valid[i+1]))


print("File", sys.argv[-1], "with", valid[0], "lines")
print('Valid values Loc1:', valid[1], 'with GeoMean:', "%.4f"%GMean[0])
print('Valid values Loc2:', valid[2], 'with GeoMean:', "%.4f"%GMean[1])

'''
File ex1.data with 100001235 lines
Valid values Loc1: 50004616 with GeoMean: 36.7818
Valid values Loc2: 49994716 with GeoMean: 36.7826

time    7m17.406s

'''