#!/bin/bash


#for graph in "$graphDirdir"/*.gph ; do 
#		  picname=$(basename "$graph")
#		  echo "Bearbeite Bild:    $picname"
#		  convert "$pic" -colorspace Gray "$outputDir/$picname"
#	done


# User - Variablendefinition
#1) Directory of git
gitDir=/home/michael/Dokumente/Studium/MA/Module/APfS/git/appfs
#2) Output directory
outputDir=/home/michael/Dokumente/Studium/MA/Module/APfS/extra/skript/out
#3) Run with following no_threads
no_threads=(1 2 4 8)

# Fixed variables
#Directory of SP-directory
graphDir=$gitDir/SP

#f.e. folder in appfs go into ~name/ex10
echo -e "\n START...  \n"



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
        echo -e "run 'make' for ${member##*/}\n"
    	make >/dev/null
    elif [ -e Makefile ] ; then
        echo -e "run 'make' for ${member##*/}\n"
      	make >/dev/null
    else
        echo -e "error for ${member##*/}\n"
    	continue
    fi

    if [ -e ex10 ]; then

        for file in $graphDir/*.gph; do
    	
            for threads_i in ${no_threads[*]}; do
			
                echo -e "run 'make THREADS=$threads_i FILE=${file##*/} test' for ${member##*/}\n"

				make THREADS=$threads_i FILE=${file##*/} test >| output.txt


				# GEHE DURCH OUTPUT UND SPEICHER DATEN TLEN, TIME, WALL
                # OUTPUT FORMAT:
                #------------------------------
                # TLEN: XXXX
                # TIME: YYYY
                # WALL: ZZZZ
                #------------------------------
                tlen=$(grep TLEN output.txt | cut -d':' -f2 | tr -d '[:space:]')
                cputime=$(grep TIME output.txt | cut -d':' -f2 | tr -d '[:space:]')
                walltime=$(grep WALL output.txt | cut -d':' -f2 | tr -d '[:space:]')

                # SAVE DATA INTO OUTPUTFILE
                #echo -e "writing global output for ${member##*/}"

                echo -e "${member##*/} $threads_i $tlen $cputime $walltime" >> $outputDir/results_${file##*/}.txt

            done

        done # threads_i
    else
        #echo -e "ERROR NO EXECUTABLE WAS CREATED IN $gitDir/$member/ex10 \n"
        echo -e "ERROR(no executable generated) in $member\n"
        continue
    fi
done # member  

echo -e "DONE \n"