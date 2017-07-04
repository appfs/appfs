#!/bin/bash

FILES=../data/*.gph
OUTFILE=testrun_output.txt

echo "##### New testrun: #####" >> $OUTFILE

for f in $FILES
do
    echo "Processing file $f..."
    echo $f >> $OUTFILE 
    echo "bonus c" >> $OUTFILE
    (time ./main $f) &>> $OUTFILE
done
