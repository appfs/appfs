#!/bin/bash

FILES=../data/*.gph
OUTFILE=testrun_output.txt

echo "##### New testrun: #####" >> $OUTFILE

for f in $FILES
do
    echo "Processing file $f..."
    echo $f >> $OUTFILE 
    echo "bonus c standalone" >> $OUTFILE
    (time ./bonus_c_standalone/main $f) &>> $OUTFILE
done
