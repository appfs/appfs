import math

filename = input("insert file name: ")
file = []

try:
    file = open(filename)
except:
    print("unsupported file name")
    filename = input("insert file name: ")

GMean = [0,0]
line_counter = [0,0,0]
for line in file:
    if "#" not in line:
        if line.count("; ") == 2:
            (k, l, v) = line.split("; ")
            if "NaN" not in v:
                try:
                    l = int(l)
                    GMean[l-1] += math.log(float(v),2)
                    line_counter[l] += 1
                except:
                    line_counter[0] += 1
        else:
            line_counter[0] += 1

GMean = [math.pow(2,(GMean[0]/line_counter[1])),math.pow(2,(GMean[1])/line_counter[2])]
print("File: {} with {} lines".format(filename,sum(line_counter)))
print("Valid values Loc1: {} ".format(line_counter[1])+ "with GeoMean: {0:.4f}".format(GMean[0]))
print("Valid values Loc1: {} ".format(line_counter[2])+ "with GeoMean: {0:.4f}".format(GMean[1]))

file.close()