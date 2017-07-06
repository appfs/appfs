# APPFS Tam Tran Exercise 1 Redo

import sys
import pandas as pd
from scipy.stats.mstats import gmean

f = open(sys.argv[1])

data = f.readlines()

df_data = pd.DataFrame(data, columns = ['original'])[:100]
print('File: ', sys.argv[1], " with " , df_data.shape[0], " lines")

df_data['afterHashtag'] = df_data.apply(lambda row: row['original'].split('#')[0], axis=1)
df_data['split'] = df_data.apply(lambda row: row['afterHashtag'].strip().split(';'), axis=1)
df_data['len 3'] = df_data.apply(lambda row: len(row['split']) == 3, axis=1) 

# can't simply assume that there will be 3 fields, might be less or more (hence invalid)
df_data['SeqNo'] = df_data[df_data['len 3'] == True].apply(lambda row: int(row['split'][0].strip()), axis=1)
df_data['Location'] = df_data[df_data['len 3'] == True].apply(lambda row: int(row['split'][1].strip()), axis=1)
df_data['Value'] = df_data[df_data['len 3'] == True].apply(lambda row: float(row['split'][2].strip()), axis=1)

df_dataa = df_data[(df_data['Location'].notnull()) & (df_data['SeqNo'].notnull()) & (df_data['Value'].notnull()) & (df_data['Value'] >= 0.0) & (df_data['SeqNo'] >= 0.0)]
#print('cleaned data size: ', df_dataa.shape)
gmean1 = gmean(df_dataa[(df_dataa['Location'] == 1.0)]['Value'], axis=0)
gmean2 = gmean(df_dataa[(df_dataa['Location'] == 2.0)]['Value'], axis=0)

# if mean not as expected, do further data cleaning. ex enforce that seqno = index + 1
print('Valid values Loc1: ' + str(df_dataa['Location'].value_counts()[1]) + ' with GeoMean: ' + str(gmean1))
print('Valid values Loc2: ' + str(df_dataa['Location'].value_counts()[2]) + ' with GeoMean: ' + str(gmean2))