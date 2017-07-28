#!/bin/bash


#for graph in "$graphDirdir"/*.gph ; do 
#		  picname=$(basename "$graph")
#		  echo "Bearbeite Bild:    $picname"
#		  convert "$pic" -colorspace Gray "$outputdir/$picname"
#	done


# Variablendefinition
gitDir=/home/michael/Dokumente/Studium/MA/Module/APfS/git/appfs
graphDir=/home/michael/Dokumente/Studium/MA/Module/APfS/git/appfs/SP
outputDir=/home/michael/Dokumente/Studium/MA/Module/APfS/extra/skript/out

no_threads=(1 2 4 8)

#f.e. folder in appfs go into ~name/ex10
echo -e "START \n"

#echo "Graph_directory: $graphDir"
#echo $gitDir
#echo ${no_threads[*]}

for member in $gitDir/*; do
	#echo -e "LOOKING AT MEMBER: $member : \n"
	if [ -d $member/ex10 ] ; then
    	# Ordner ex10 existiert, also rein da..
        #echo -e "go into $member/ex10 \n"
        cd $member/ex10
    else
    	#echo -e "ERROR (no directory) in $member\n"
        continue                        
    fi

    if [ -e makefile ] ; then  
    	make >/dev/null
    elif [ -e Makefile ] ; then
      	make >/dev/null
    else
    	continue
    fi

    if [ -e ex10 ]; then
    	for threads_i in ${no_threads[*]}; do

			for file in $graphDir/*.gph; do

				make THREADS=$threads_i FILE=${file##*/} test >output.txt
				# GEHE DURCH OUTPUT UND SPEICHER DATEN TLEN, TIME, WALL
                tlen=$(grep TLEN output.txt | cut -d':' -f2 | tr -d '[:space:]')
                cputime=$(grep TIME output.txt | cut -d':' -f2 | tr -d '[:space:]')
                walltime=$(grep WALL output.txt | cut -d':' -f2 | tr -d '[:space:]')

                # SAVE DATA INTO OUTPUTFILE
                echo -e "writing global output for ${member##*/}"

                echo -e "${member##*/} $threads_i ${file##*/} $tlen $cputime $walltime" >> $outputDir/results.txt

            done

        done # threads_i
    else
        #echo -e "ERROR NO EXECUTABLE WAS CREATED IN $gitDir/$member/ex10 \n"
        echo -e "ERROR(no executable generated) in $member\n"
        continue
    fi
done # member    

echo -e "DONE \n"