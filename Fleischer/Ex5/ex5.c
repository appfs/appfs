/**
 * @file
 * @author  Sven Fleischer
 * @version 1.1

 *
 * @section DESCRIPTION
 *
 * This program parses a graph and parses the longest shortest path from any edge to edge with the index 1.
 */
//compile with gcc -std=c11 -O3 -g -Wall -Wextra -Wpedantic -LDFLAGS ex5.1.c -o ex5.1 -lm

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
* This program parses a graph and parses the longest shortest path from any node to node with the index 1. The matrix, which is generated is of half size.
*
* @param live at the call of the programm
* @return The longest shortest path from any node to node with the index 1.
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
    		graph[i] = (long int*) malloc((i+1)*sizeof(long int));
    		for (j = 0; j <=i; ++j){
    			if (i==j){
    				graph[i][j] = 0;
    			}
    			else{ 
    				graph[i][j] = -1;  				
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
			if (temp1 == temp2){
				continue;
			}
			else if(temp1 > temp2){			
				if (*(*(graph+(temp1-1))+ temp2-1) > temp3 || *(*(graph+(temp1-1))+ temp2-1) == -1){
					*(*(graph+(temp1-1))+ temp2-1) = temp3;
				}	
			}
			else{
				if (*(*(graph+(temp2-1))+ temp1-1) > temp3 || *(*(graph+(temp2-1))+ temp1-1) == -1){
					*(*(graph+(temp2-1))+ temp1-1) = temp3;
				}
			}
		}
	}
        
	
	for (int k = 0; k < graphSize; ++k){
		for (i = 0; i < graphSize; ++i){
			for (j = 0; j <= i; ++j){
				if (i == j || i==k || j==k) continue;
				else if (i>j){
					if (k < j && k > i){
						if (graph[i][j] == -1||graph[i][j] > (graph[k][i] + graph[j][k])){
							if (graph[k][i] != -1 && graph[j][k] != -1){ 
								graph[i][j] = graph[k][i]+graph[j][k];
								assert(!fetestexcept(FE_INVALID | FE_OVERFLOW | FE_UNDERFLOW));
							}
						}
					}
					else if (k > j && k > i){
						if (graph[i][j] == -1||graph[i][j] > (graph[k][i] + graph[j][k])){
							if (graph[k][i] != -1 && graph[j][k] != -1){ 
								graph[i][j] = graph[k][i]+graph[j][k];
								assert(!fetestexcept(FE_INVALID | FE_OVERFLOW | FE_UNDERFLOW));
							}
						}
					}
					else if (k < j && k < i){
						if (graph[i][j] == -1||graph[i][j] > (graph[i][k] + graph[j][k])){
							if (graph[i][k] != -1 && graph[j][k] != -1){ 
								graph[i][j] = graph[i][k]+graph[j][k];
								assert(!fetestexcept(FE_INVALID | FE_OVERFLOW | FE_UNDERFLOW));
							}
						}
					}
					else if (k > j && k < i){
						if (graph[i][j] == -1||graph[i][j] > (graph[i][k] + graph[k][j])){
							if (graph[i][k] != -1 && graph[k][j] != -1){ 
								graph[i][j] = graph[i][k]+graph[k][j];
								assert(!fetestexcept(FE_INVALID | FE_OVERFLOW | FE_UNDERFLOW));
							}
						}
					}					
					else {printf("Fehler+k: %d + i: %d > j:%d\n", k,i,j);
					return EXIT_FAILURE;
					}												
				}		
			}
		}
	}
    

	    
	for (i = 0; i < graphSize; ++i){        
		if (graph[i][0] > cost && graph[i][0] != -1){
        		temp = i;
        		cost = graph[i][0];
        		free(graph[i]);
        	}	
        }

        
        printf("RESULT VERTEX %d \n", temp+1);
        printf("RESULT DIST %ld \n", cost);
        free(graph);
        return 0;
}
