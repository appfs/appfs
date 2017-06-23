#!/bin/bash

FILES=../data/*.gph
OUTFILE=testrun_output.txt

echo "##### New testrun: #####" >> $OUTFILE

for f in $FILES
do
    echo "Processing file $f..."
    echo $f >> $OUTFILE 
    echo "py library" >> $OUTFILE
    (time python3 py_library/ex5_library.py $f) &>> $OUTFILE
done
