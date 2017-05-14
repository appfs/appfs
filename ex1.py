# -*- coding: utf-8 -*-
import codecs
import sys
import numpy
# Simon June 2017
# appfs_ex1

validValuesLoc1 =0
validValuesLoc2 =0
sumOfValidValuesLoc1 =0
sumOfValidValuesLoc2 =0
noOfLines =0
 

# Here your functions
def isfloat(value):
  try:
    float(value)
    return True
  except ValueError:
    return False

# Here your main program

# Open files
inputFileName = str(sys.argv[1])
inputfile = open(inputFileName)

for l in inputfile:
    dataRecord = l.strip().split(';')
    noOfLines +=1
    if(len(dataRecord)==3):		
		if(isfloat(dataRecord[2]) and float(dataRecord[2])>0):
			#print dataRecord
			if(dataRecord[1]==' 1'):
				validValuesLoc1 +=1
				sumOfValidValuesLoc1 += float(dataRecord[2])
		
			if(dataRecord[1]==' 2'):
				validValuesLoc2 +=1
				sumOfValidValuesLoc2 += float(dataRecord[2])
	

inputfile.close()
# Go through file line by line
geoMeanLoc1 = 0
geoMeanLoc2 = 0
if(validValuesLoc1 >0):
	geoMeanLoc1 = sumOfValidValuesLoc1/validValuesLoc1
if(validValuesLoc2 >0):
	geoMeanLoc2 = sumOfValidValuesLoc2/validValuesLoc2

print ('File:\t'+ inputFileName + ' with ' + str(noOfLines) +' lines \n' + 
'Valid values Loc1: ' + str(validValuesLoc1) + ' with GeoMean: ' + str(geoMeanLoc1) +'\n'+
'Valid values Loc2: ' + str(validValuesLoc2) + ' with GeoMean: ' + str(geoMeanLoc2))

