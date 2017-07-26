#!/bin/bash

FILES=../../data/*.gph
OUTFILE=testrun_output.txt

echo "##### New testrun: #####" >> $OUTFILE

for f in $FILES
do
    echo "Processing file $f..."
    echo $f >> $OUTFILE
    (time ./ex10 $f 1) &>> $OUTFILE
done
