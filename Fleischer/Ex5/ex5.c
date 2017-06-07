/**
 * @file
 * @author  Sven Fleischer
 * @version 1.0

 *
 * @section DESCRIPTION
 *
 * This program parses a graph and parses the longest shortest path from any edge to edge with the index 1.
 */

#define _GNU_SOURCE
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<math.h> 
#include<assert.h>
#include<ctype.h>
#include<fenv.h>

#define MAX_LINE_LEN 512

/**
* This program parses a graph and parses the longest shortest path from any edge to edge with the index 1.
*
* @param live at the call of the programm
* @return The longest shortest path from any edge to edge with the index 1.
*/

int main(int argc, char *argv[]){
	int temp = 0;
	long int cost = -1;
	FILE *fp;
	if (argc<=1){ 
		exit(EXIT_FAILURE);
	}
	fp = fopen(argv[1], "r");
    	if (fp == NULL)	exit(EXIT_FAILURE);
        
	int i = 0;
	int j = 0;
        
        char delimiter[] = " ";
	char *ptr;
    	char *bla;
    	size_t len = 0;
    	char *line = (char*) malloc(MAX_LINE_LEN * sizeof(char));
    	getline(&line, &len, fp);
    	const int graphSize = strtol(line, &bla, 10);
    	int temp1 = 0;
    	int temp2 = 0;
    	long int temp3 = 0;
    	long int** graph = (long int**) malloc(graphSize*sizeof(long int*));
    	for (i = 0; i < graphSize; ++i){
    		graph[i] = (long int*) malloc(graphSize*sizeof(long int));
    		for (j = 0; j < graphSize; ++j){
    			if (i==j){
    				*(*(graph+i)+j) = 0;
       			}
    			else{ 
    				*(*(graph+i)+j) = -1;   				
    			}	
    		}	
    	}
	while ((getline(&line, &len, fp)) != -1) {
		if (*(line) != '\n'){
			ptr = strtok(line, delimiter);
			temp1 = strtol(ptr, &bla, 10);
			ptr = strtok(NULL, delimiter);
			temp2 = strtol(ptr, &bla, 10);
			ptr = strtok(NULL, delimiter);
			temp3 = strtol(ptr, &bla, 10);
			if (*(*(graph+(temp1-1))+ temp2-1) > temp3 || *(*(graph+(temp1-1))+ temp2-1) == -1){
				*(*(graph+(temp1-1))+ temp2-1) = temp3;
			}
		}
	}
        
	long int** shortways = (long int**) malloc(graphSize * sizeof(long int*));
        for (i = 0; i < graphSize; ++i){
  		shortways[i] = (long int*) malloc(graphSize * sizeof(long int));
  		for (j = 0; j < graphSize; j++){
  			shortways[i][j] = *(*(graph+i)+j);
  	}
  }
	for (int k = 0; k < graphSize; ++k){
		for (i = 0; i < graphSize; ++i){
			for (j = 0; j < graphSize; ++j){
				if (shortways[i][j] == -1||shortways[i][j] > (shortways[i][k] + shortways[k][j])){
					if (shortways[i][k] != -1 && shortways[k][j] != -1){ 
						shortways[i][j] = shortways[i][k]+shortways[k][j];
					}
				}	
			}
		}
	}
    
	for (i = 0; i < graphSize; ++i){        
		if (shortways[i][0] > cost && shortways[i][0] != -1){
        		temp = i;
        		cost = shortways[i][0];
        		free(shortways[i]);
        	}	
        }

        
        printf("RESULT VERTEX %d \n", temp+1);
        printf("RESULT DIST %ld \n", cost);
        free(graph);
        free(shortways);
        return 0;
}

