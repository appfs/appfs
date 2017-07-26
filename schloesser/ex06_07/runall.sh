#!/bin/bash

FILES=../../data/*.gph
OUTFILE=testrun_output.txt

echo "##### New testrun: #####" >> $OUTFILE

for f in $FILES
do
    echo "Processing file $f..."
    echo $f >> $OUTFILE 
    echo "Processing with param m1 ..."
    echo "Processing with param m1 ..." &>> $OUTFILE
    (time python3 ex6 -m1 $f) &>> $OUTFILE
    echo "Processing with param m2 ..."
    echo "Processing with param m2 ..." &>> $OUTFILE
    (time python3 ex6 -m2 $f) &>> $OUTFILE
done
