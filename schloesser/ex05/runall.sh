#!/bin/bash

FILES=../../data/*.gph
OUTFILE=testrun_output.txt

echo "##### New testrun: #####" >> $OUTFILE

for f in $FILES
do
    echo "Processing file $f..."
    echo $f >> $OUTFILE
    (time ./ex5 $f) &>> $OUTFILE
done
