#Advanced Pratical Programming For Scientists
#Assignemnt 2
#author:Viktor Jeney
#written in Python 3

#This program reads a XML and writes it into a CSV
#For each of those tasks there is a seperate function written.
#The filename to read has to be the first argument from the command line.
#The filename to write into has to be the second arguments from command line.


import sys
from bs4 import BeautifulSoup as Soup
import csv

###############################################################################

def readXML(filename):
    #reads an xml file using BeautifulSoup with lxml parser
    #input:     name of file
    #returns:   list with 3 entries: date as YYYY-MM-DD, time as HH
    #           and value of measured power without unit 
    #
    #initializing list to return    
    measurements=[]
    
    #initializing temporary objects
    currentDay=''
    currentTime=''
    currentPower=''

    #opening file with handler
    with open(filename,'r') as fileHandler:
        #parse the file into an soup object using lxml parser
        #note that all the tags have only lower case letters now
        #because xml should't distinguish between lower and upper case
        soup=Soup(fileHandler,'lxml')
        
        #find all tags 'gasDay '
        for day in soup.findAll('gasday'):
            #set the date for list entry
            currentDay=day.attrs['date']
            #find all tags 'boundarynode'
            #this is just to make sure that the rest is enclosed in a
            #boundarynode tag
            for node in day.findAll('boundarynode'):
                #find all tags 'time'
                for time in node.findAll('time'):
                    #set the time for list entry
                    currentTime=time.attrs['hour'].zfill(2)
                    #find all tags 'amountofpower'
                    for power in time.findAll('amountofpower'):
                        currentPower=power.attrs['value']
                        measurements.append([currentDay,currentTime,\
                        currentPower])
    return measurements

###############################################################################

def writeCSV(fileName,data):
    #write a CSV file with the 
    #input:     name of file
    #returns:   None

    #open file with handler
    with open(fileName,'w') as fileHandler:
        #create CSV writer
        writer = csv.writer(fileHandler, delimiter=',')
        #iterate through data
        for line in data:
            #write row
            writer.writerow(line)

###############################################################################
   
#read filenames out of system input
inputName=sys.argv[1]
outputName=sys.argv[2]

#inputName='example.measured-1-1-0.xml'
#outputName='output.csv'

#read in the measurements
measurements=readXML(inputName)

#write the measurements into csv file
writeCSV(outputName,measurements)

            
