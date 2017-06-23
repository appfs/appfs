#!/bin/bash

FILES=../data/*.gph
OUTFILE=testrun_output.txt

echo "##### New testrun: #####" >> $OUTFILE

for f in $FILES
do
    echo "Processing file $f..."
    echo $f >> $OUTFILE 
    echo "py standalone" >> $OUTFILE
    (time python3 py_standalone/ex5_standalone.py $f) &>> $OUTFILE
done
