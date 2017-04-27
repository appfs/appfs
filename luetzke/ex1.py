import math

file = open("ex1.dat")
firstLogSum =0
secondLogSum = 0
file.readline()
file.readline()
file.readline()
line = file.readline()#                                 skip to the first relevant line
i=4#                                                    linecount
count1=0#                                               valid loc1 values
count2=0#                                               valid loc2 values
while (line):
  stringList = line.split("; ")
  if len(stringList) !=3 or  stringList[2].strip()=="NaN":#skip faulty datapoint
    i+=1
    line=file.readline()
    continue
  try:#calculate arithmetic mean of log of the datapoints
    if stringList[1]=="1":
      firstLogSum += math.log2(float(stringList[2])) 
      count1+=1
    elif stringList[1] == "2":
      secondLogSum += math.log2(float(stringList[2])) 
      count2+=1
  except ValueError:#skip lines with faulty value
    pass
  line = file.readline()
  i+=1
print("File: ex1.dat with {} lines".format(i))
print("Valid Values Loc1: {} with GeoMean: {}".format(count1, math.pow(2,firstLogSum/count1)))
print("Valid Values Loc2: {} with GeoMean: {}".format(count2, math.pow(2,secondLogSum/count2)))
file.close()

