#ex1 APPFS Martha Karpeter 367847

from math import sqrt
import decimal
Decim = decimal.Decimal

file = open("ex2.dat","r") 
Data = file.readlines() #reads lines from the data set
decimal.getcontext().prec = 10

#devide Data into Location1 and Location2
Location1 = [Decim(line[line.index(";")+5:]) for line in Data if ";" in line and line[line.index(";")+5:].count(".")==1 and line.count(" ") == 2 and int(line[line.index(";")+2]) == 1]
Location2 = [Decim(line[line.index(";")+5:]) for line in Data if ";" in line and line[line.index(";")+5:].count(".")==1 and line.count(" ") == 2 and int(line[line.index(";")+2]) == 2]


#computes Geometric Mean of a given dataset, returns type decimal.Decimal
def GeometricMean(dataset):
    length = Decim(len(dataset))
    result = Decim(1)
    for line in dataset:
        result *= line**(Decim(1)/length)
    return(Decim(result))

#get the geometric mean for each location
GeometricMean1 = GeometricMean(Location1)
GeometricMean2 = GeometricMean(Location2)

#output 
print("File ex2.dat with", len(Data), "lines")
print('Valid values Loc1:', len(Location1), 'with GeoMean:', "%.4f"%GeometricMean1)
print('Valid values Loc2:', len(Location2), 'with GeoMean:', "%.4f"%GeometricMean2)

file.close()


# File ex1.dat with 10000128 lines
# Valid values Loc1: 4998044 with GeoMean: 36.7802
# Valid values Loc2: 5000938 with GeoMean: 36.7666

# real    9m42.231s
