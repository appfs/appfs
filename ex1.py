# -*- coding: utf-8 -*-
import codecs
import sys
import numpy
import math
# Simon June 2017
# appfs_ex1

noOfValidValuesLoc1 =0
noOfValidValuesLoc2 =0
ValidValuesLoc1 = []
ValidValuesLoc2 = []
noOfLines =0
 

# Here your functions
def isfloat(value):
  try:
    float(value)
    return True
  except ValueError:
    return False

def writeErrorsToFile(lineWithErrors):
	errors = open("Errors.txt", "w")
	errors.write(str(lineWithErrors))

# Here your main program

# Open files
errors = open("Errors.txt", "w")
result = open("Result.txt", "w")
inputFileName = str(sys.argv[1])
inputfile = open(inputFileName)

for l in inputfile:
    dataRecord = l.strip().split('#')
    dataRecord = dataRecord[0].strip().split(';')
    noOfLines +=1
    if(len(dataRecord)==3):		
		if(isfloat(dataRecord[2]) and float(dataRecord[2])>0):
			#print dataRecord
			if(dataRecord[1]==' 1'):
				val = float(dataRecord[2])
				if(not numpy.isnan(val)):
					noOfValidValuesLoc1 +=1
					ValidValuesLoc1.append(float(dataRecord[2]))
				else:
					errors.write(str(l))
		
			elif(dataRecord[1]==' 2'):
				val = float(dataRecord[2])
				if(not numpy.isnan(val)):
					noOfValidValuesLoc2 +=1
					ValidValuesLoc2.append(float(dataRecord[2]))
				else:
					errors.write(str(l))
			else:
				errors.write(str(l))
		else:
			errors.write(str(l))
    else:
	errors.write(str(l))

inputfile.close()
# Go through file line by line
geoMeanLoc1 = 0
geoMeanLoc2 = 0
#print(ValidValuesLoc1)
#print(ValidValuesLoc1)
pow1=1./noOfValidValuesLoc1
pow2=1./noOfValidValuesLoc2
if(noOfValidValuesLoc1 >0):
	sumOfLogs1 = 0
	for x in ValidValuesLoc1:
		sumOfLogs1 += math.log(x,2)
	geoMeanLoc1 = 2 ** (pow1*sumOfLogs1)
if(noOfValidValuesLoc2 >0):
	sumOfLogs2 = 0
	for x in ValidValuesLoc2:
		sumOfLogs2 += math.log(x,2)
	geoMeanLoc2 = 2 ** (pow2*sumOfLogs2)
print ('File:\t'+ inputFileName + ' with ' + str(noOfLines) +' lines \n' + 
'Valid values Loc1: ' + str(noOfValidValuesLoc1) + ' with GeoMean: ' + str(geoMeanLoc1) +'\n'+
'Valid values Loc2: ' + str(noOfValidValuesLoc2) + ' with GeoMean: ' + str(geoMeanLoc2))

result.write('File:\t'+ inputFileName + ' with ' + str(noOfLines) +' lines \n' + 
'Valid values Loc1: ' + str(noOfValidValuesLoc1) + ' with GeoMean: ' + str(geoMeanLoc1) +'\n'+
'Valid values Loc2: ' + str(noOfValidValuesLoc2) + ' with GeoMean: ' + str(geoMeanLoc2))

