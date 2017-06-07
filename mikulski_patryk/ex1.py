import csv
from scipy.stats.mstats import gmean

in_filename = 'ex1-100.dat'

seq1 = []
seq2 = []
invalids = []

with open(in_filename, 'r') as in_file:
    reader = csv.reader(in_file, delimiter=';')
    i = 0
    for row in reader:
        i = i + 1
        if len(row)==3:

            seqN = row[0]
            locN = row[1]
            value = row[2]
            try:
                int(seqN)
            except ValueError:
                print('Sequence number '+seqN+' is not an int.')
                invalids.append(row)
                continue
            try:
                if int(locN) == 1:
                    try:
                        if float(value) > 0:
                            seq1.append(float(value))  
                    except ValueError:
                        print(value+' at '+ seqN+' cannot be converted to a float')
                        invalids.append(row)
                elif int(row[1]) == 2:
                    try:
                        if float(value) > 0:
                            seq2.append(float(value))
                    except ValueError:
                        print(value+' at '+ seqN+' cannot be converted to a float')
                        invalids.append(row)
            except ValueError:
                print(locN+' at '+seqN+' cannot be converted to an int')
                invalids.append(row)

  
print(i)
print('Valid values Loc1: '+str(len(seq1))+' Loc2: '+str(len(seq2)))
print(min(seq1), min(seq2))
print(max(seq1), max(seq2))
print(gmean(seq1),gmean(seq2))
print(len(invalids))

out_filename = 'invalids.csv'
with open(out_filename, 'w') as out_file:
    writer = csv.writer(out_file, delimiter=';')
    for invalid in invalids:
        writer.writerow(invalid)
