/**
 * @file
 * @author  Sven Fleischer
 * @version 2.0

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
#include <fenv.h>
#include <time.h>
#include <sys/time.h>

#define MAX_LINE_LEN 512

size_t graphSize;
size_t* vertexNo;
size_t*** graph;
double* dist;
size_t temp2;
size_t vertexInd;
char* temp;



/**
* Builds and fits the graph
*
* @param The input file
*/
void buildGraph(FILE *fp){
	size_t i = 0;
        char delimiter[] = " ";
	char *ptr;
    	char *bla;
    	size_t len = 0;
    	char *line = (char*) malloc(MAX_LINE_LEN * sizeof(char));
    	getline(&line, &len, fp);
    	graphSize = strtol(line, &bla, 10);
    	int temp1 = 0;
    	int temp2 = 0;
    	long int temp3 = 0;
    	vertexNo =(size_t*) malloc(graphSize*sizeof(size_t));
    	graph = (size_t***) malloc(graphSize*sizeof(size_t**));
    	for (i = 0; i < graphSize; ++i){
    		graph[i] = (size_t**) malloc(2*sizeof(size_t**));
    		vertexNo[i] = 0;
    	}
    	
	printf("Graph built\n");
	while ((getline(&line, &len, fp)) != -1) {
		if (*(line) != '\n'){
			ptr = strtok(line, delimiter);
			temp1 = strtol(ptr, &bla, 10);
			ptr = strtok(NULL, delimiter);
			temp2 = strtol(ptr, &bla, 10);
			ptr = strtok(NULL, delimiter);
			temp3 = strtol(ptr, &bla, 10);
			assert(temp1 >=0);
			assert(temp2 >=0);
			assert(temp3 >=0);
			
			graph[temp1-1] = (size_t**) realloc(graph[temp1-1], (vertexNo[temp1-1]+1) * sizeof(size_t**));
			assert(graph[temp1-1]);
			graph[temp1-1][vertexNo[temp1-1]] = (size_t*) malloc(2*sizeof(size_t));
			graph[temp1-1][vertexNo[temp1-1]][0] = temp2;
			graph[temp1-1][vertexNo[temp1-1]][1] = temp3;   
			++vertexNo[temp1-1];
			
			
			
			graph[temp2-1] = (size_t**) realloc(graph[temp2-1], (vertexNo[temp2-1]+1) * sizeof(size_t**));
			assert(graph[temp2-1]);			
			graph[temp2-1][vertexNo[temp2-1]] = (size_t*) malloc(2*sizeof(size_t));
			graph[temp2-1][vertexNo[temp2-1]][0] = temp1;
			graph[temp2-1][vertexNo[temp2-1]][1] = temp3;   
			++vertexNo[temp2-1];
				
		}
	}
	printf("Graph fitted\n");
}

/**
* Checks of two edges are connected.
*
* @param The two edges, which the method has to check if they are connected
* @return If two certain edges are connected. 1 if yes, 0 if not
*/
char extPath(size_t a, size_t b){
	for (size_t i= 0; i < vertexNo[a]; ++i){
		if (graph[a][i][0] == b+1){
			vertexInd = i;
			return 1;
		}
	}
	return 0;
}

/**
* Findes the minimal enty of an array. Solution is going to be saved into one of the input parameters.
*
*
*/

void minDist(){
	double min= INFINITY;
	temp2 = 0;
	for(size_t i = 0; i < graphSize; ++i){
		if(temp[i] == 0 && dist[i] <= min){
			min = dist[i];
			temp2 = i;
		}	
	}
}


/**
* Computes the shortest paths from vertex with inex 0 to every other else
*
*
*/

double* dijkstra(){

	dist = (double*) malloc(sizeof(double)*graphSize);
	temp =(char*) malloc(sizeof(char)*graphSize);
	
	for (size_t i = 0; i < graphSize; ++i){
		dist[i] = INFINITY;
		temp[i] = 0;
	}
	
	dist[0] = 0;

	for(size_t j = 0; j < graphSize; ++j){
		minDist();
		temp[temp2] = 1;
		for (size_t n= 0; n < graphSize; n++){
			if(!temp[n] && extPath(temp2, n) && dist[temp2] != INFINITY){
				if(dist[temp2] + (double)graph[temp2][vertexInd][1] < dist[n]){
					dist[n] = dist[temp2] + (double)graph[temp2][vertexInd][1];
					assert(!fetestexcept(FE_INVALID | FE_OVERFLOW | FE_UNDERFLOW));
				}
			}
		}
	}
	return dist;
}


/**
* Main method. Findes the most expensive shortest path from vertex 1 to any other vertex in the graph.
*
* @param The input file
* @return The most expensive shortest path. It's given out on the shell
*/

int main(int argc, char *argv[]){
  	clock_t cpu;
  	double wall;
  	struct timeval time;
   	assert(!gettimeofday(&time,NULL));

    
     	wall=(double)time.tv_sec + (double)time.tv_usec * .000001;
  	
  	cpu = clock();
	FILE *fp;
	if (argc<=1){ 
		exit(EXIT_FAILURE);
	}
	fp = fopen(argv[1], "r");
    	if (fp == NULL)	exit(EXIT_FAILURE);
        

        size_t index = 0;
        double max = 0;
        
	buildGraph(fp);
	dijkstra();
	for (size_t i = 0; i < graphSize; ++i){
		if (max < dist[i]){
			max = dist[i];
			index = i;
		}
	}
	for (size_t i = 0; i<graphSize; ++i){
		for(size_t j = 0; j < vertexNo[i]; ++j){
			free(graph[i][j]);
		}
		free(graph[i]);
	}
	printf("RESULT VERTEX %ld \n", index+1);
        printf("RESULT DIST %ld \n", (size_t)max);
	
	cpu = clock() - cpu;
        assert(!gettimeofday(&time,NULL));
	wall=(double)time.tv_sec + (double)time.tv_usec * .000001 -wall;
        printf("The programm used the CPU time of: %fs\n",((float)cpu)/CLOCKS_PER_SEC);
        printf("The programm used the wall time of: %fs\n", wall);
}					 	    	
    	
