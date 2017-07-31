#! /bin/bash
# run ex10 for all graphs in ../graphs/

for f in ../graphs/*
do
	echo RUNNING: ex10 "$f" 2
	echo BEGIN: ex10 "$f" 2 >> output.txt
	./ex10 "$f" 2 >> output.txt
	echo END: ex10 "$f" 2 >> output.txt
	echo >> output.txt
done
