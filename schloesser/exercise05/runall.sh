#!/bin/bash

FILES=data/*.gph
OUTFILE=testrun_output.txt

echo "##### New testrun: #####" >> $OUTFILE

for f in $FILES
do
    echo "Processing file $f..."
    echo $f >> $OUTFILE 
    (time python3 ex5_standalone.py $f) &>> $OUTFILE
done
