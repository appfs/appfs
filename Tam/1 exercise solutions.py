# TK 17Apr2017

import sys
import math

def geom_mean(value_list):
    return math.exp(sum( math.log(x) for x in value_list) / len(value_list))
    # a more computationally effective way of calculating geo-mean

loc_value = [[], []]

with open(sys.argv[1], "r") as f: #r for read

    # enumerate liefert dir die Zeilenummer automatisch. Die started dann aber bei 0.
    for lineno, line in enumerate(f):
            
        # Check for comment 
        line = line.split("#")[0] #everything after the comment is discarded; everything before it kept, even if it's nothing

        stripped = line.strip()
        print(stripped)      
 
        # Ignore empty lines
        if stripped == "" :
            continue; # continue onto next loop iteration; don't execute the rest of the loop program for this iteration

        fields = stripped.split(";")

        # filtering of data happens with these if-stmts and their continue command
        # omitting all lines with syntax errors and value errors
        if len(fields) != 3 :
            print("Line {} : Syntax error [{}]".format(lineno + 1, stripped));
            continue;

        try:
            seq = int(fields[0])
            loc = int(fields[1])
            val = float(fields[2])

        except ValueError:
            print("Line {} : Syntax error [{}]".format(lineno + 1, stripped));
            continue;

        if loc not in [1, 2]:
            print("Line {} : Illegal location: {}".format(lineno + 1, loc));
            continue

        if math.isnan(val) or val <= 0.0 :
            print("Line {} : Invalid value".format(lineno + 1));
            continue

        loc -= 1 #temp adjustment with loc 1,2 values so that it fits with loc_value 0,1 indices

        loc_value[loc].append(val)

# Print statistics

# very effective way of gathering statistics about data
# instead of generating a filtered/cleaned version of the data to do len(cleandata[location == 1]), just get it via the gradual accumulation of ONLY values by location.. small data-object
# TK: need to +1
print ("File: ", sys.argv[1], "with", lineno + 1, "lines");

for loc in [0, 1]:
    print ("Valid values Loc", loc + 1, ": ", len(loc_value[loc]), "with GeoMean:", geom_mean(loc_value[loc]));

# EOF