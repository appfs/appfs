import sys
import csv
import numpy as np

def skipLine(row):
    #checks wheather the line should be skipped
    #input: row as parsed by linereader (python array of  3 strings)
    #output: true(skip)/false(don't skip)
    #
    #check row for the right format
    #row has to be a list of 3 strings, otherwise skip
    if not isinstance(row,list):
        return True
    if len(row)!=3:
        return True
    if (not isinstance(row[0],str) or not isinstance(row[1],str) \
        or not isinstance(row[2],str)):
        return True
    #first check if line is empty
    if row==[]:
        return True
    #then check if line starts with a #
    if row[0][0]=='#':
        return True
    #now check for basic data quality
    #Location has to be 1 or 2
    try:
        if int(row[1])!=1 and int(row[1])!=2:
            return True
    except:
        #We go here when the location can't be converted to int.
        #In this case we skip the line for now.
        #One could write a log file with these lines here and manually 
        #check them.
        return True
    #Value has to be nonnegative
    #Also skip if it is Nan
    try:
        if np.isnan(float(row[2])):
            return True
        if float(row[2])<=0:
            return True
    except:
        #We go here when the value can't be converted to a float.
        #It's not always clear if there are just decimal points missing 
        #or if digits are missing in the numbers.
        #For now all of those are skipped to not introduce any systematic
        #errors. Again one could write a log file with these lines here 
        #and manually check them.
        return True
    #if we got to this point, we don't skip the line
    return False

def updateGeometricAverage(avg,n,sample):
    #updates geometric average
    #n:     number of samples
    #avg:   geometric average with n samples
    #sample:n+1'st sample
    #reutn: geometric average with n+1 samples
    return avg**(n/(n+1.0))*sample**(1/(n+1.0))
    
#The averages are computed by a stepwise update to save memory.
#The root is not taken in the end but in every step via a update
#to prevent overflow of the variables
#
#set initial values for geometric averages 
#for each Locations respectively
geoAvg1=1   
geoAvg2=1

#initialize linenumber
lineNum=0

#Initalize temporary values for line entries.
location=0
value=0

#initialize lists of values of location 1 and 2
valueList1=[]
valueList2=[]

#initialize filename
filename=sys.argv[1]

#open file in read only mode
with open(filename,'r') as csvfile:

    #use csv reader to parse the file
    lineReader = csv.reader(csvfile,delimiter=";",skipinitialspace=True)

    #iterates one line at a time
    for row in lineReader:
        lineNum=lineNum+1
        #check if we should skip the line
        if skipLine(row)==True:
            #skip line
            continue
        else:
            #otherwise we save the values in the corresponding list
            location=int(row[1])
            value=float(row[2])
            if(location==1):
                #Location = 1 here
                valueList1.append(value)
            elif(location==2):
                #and here Location = 2
                valueList2.append(value)
    
#now the geometric averages are computed
#location 1
for n,value in enumerate(valueList1):
    geoAvg1=updateGeometricAverage(geoAvg1,n,value)
        
#location 2
for n,value in enumerate(valueList2):
    geoAvg2=updateGeometricAverage(geoAvg2,n,value)

#produce output
print('File: {:s} with {:d} lines'.format(filename,lineNum))
print('Valid values Loc1: {out1:d} with GeoMean: {out2:.4f}'.\
      format(out1=len(valueList1),out2=geoAvg1))
print('Valid values Loc2: {out1:d} with GeoMean: {out2:.4f}'.\
      format(out1=len(valueList2),out2=geoAvg2))
