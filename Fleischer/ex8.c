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
size_t temp2;
size_t vertexInd;



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
* @param ditances already discovered by dijkstra and a marker if alredy used
* @return index of minDist vertex not alredy worked with
*/

size_t minDist(double** dist, char* temp){
	double min= INFINITY;
	size_t temp2 = 0;
	for(size_t i = 0; i < graphSize; ++i){
		if(temp[i] == 0 && dist[i][0] <= min){
			min = dist[i][0];
			temp2 = i;
		}	
	}
	return temp2;
}


/**
* Computes the shortest paths from nodes already in the steiner tree to every other else
* @param nodes already in steinertree 
* @return shortest paths distance at [0] and [1] the previus node
*
*/

double** dijkstra(char* s){

	double** dist = malloc(sizeof(double*)*graphSize);
	char* temp =(char*) malloc(sizeof(char)*graphSize);
	
	for (size_t i = 0; i < graphSize; ++i){
		dist[i] = malloc(sizeof(double)*2);
		if(s[i] == 0){
			dist[i][0] = INFINITY;
		}
		else{
			//printf("kl %ld\n", i);
			dist[i][0] = 0;
			dist[i][1] = -1;
		}	
		
		temp[i] = 0;
	}
	//dist[s] = 0;

	for(size_t j = 0; j < graphSize; ++j){
		temp2 = minDist(dist, temp);
		temp[temp2] = 1;
		for (size_t n= 0; n < graphSize; n++){
			if(!temp[n] && extPath(temp2, n) && dist[temp2][0] != INFINITY && dist[n][1] != -1){
				if(dist[temp2][0] + (double)graph[temp2][vertexInd][1] < dist[n][0]){
					dist[n][0] = dist[temp2][0] + (double)graph[temp2][vertexInd][1];
					dist[n][1] = temp2;
					assert(!fetestexcept(FE_INVALID | FE_OVERFLOW | FE_UNDERFLOW));
				}
			}
		}
	}
	return dist;
}

/**
* Checks if there if there are unconected terminals.
*
* @param Used terminal marker and the number of terminals
* @return 1 of yes or 0 as not
*/

char terminalsLeft(char* marker, size_t numberOfTerminal){
	for(size_t j= 0; j < numberOfTerminal; ++j){
		printf("markel: %d\n", marker[j]);
	}	
	for(size_t j= 0; j < numberOfTerminal; ++j){
		//printf("marker: %d\n", marker[j]);
		if(!marker[j]){
			return 1;
		}
	}
	return 0;
}


/**
* Finds a steiner tree.
*
* @param All the terminal edges
* @return The objective value
*/
double steinerTree(size_t* terminal, size_t numberOfTerminal){
	char* marker = malloc(numberOfTerminal*sizeof(char));
	char* inTree = malloc(graphSize*sizeof(char));
	size_t nodesInTree = 1;
	double min = INFINITY;
	double value = 0;
	size_t temp = 0;	
	double** vector = malloc(sizeof(double*)*graphSize);
	for(size_t i = 0; i < graphSize; ++i){
		vector[i] = malloc(sizeof(double)*2);
		inTree[i] = 0;
	}
	for(size_t i = 0; i<numberOfTerminal; ++i){
		marker[i] = 0;
	}
	printf("val %f\n", value);
	inTree[terminal[0]] = 1;
	vector = dijkstra(inTree);
	marker[0] = 1;
	//search for cheapest next terminal
	for(size_t i = 1; i < numberOfTerminal; ++i){
		if(vector[terminal[i]][0] < min && vector[terminal[i]][1] != -1){
		printf("%f + %ld + %ld\n",vector[terminal[i]][0], i, temp);
		min = vector[terminal[i]][0];
		temp = i;
		}
	}
	marker[temp] = 1;
	value = min;
	printf("val+min %f+ %f\n", value, min);
	size_t iter = terminal[temp];
	//find path to next cheaperst
	while(vector[iter][1] != -1){
		inTree[iter] = 1;
		iter = (size_t) vector[iter][1]; 
		++nodesInTree;
	}
	for(size_t i = 0; i < nodesInTree; ++i){
		free(vector[i]);
	}
	//printf("First Terminal\n");
	//repeat the hole procedure
	while (terminalsLeft(marker, numberOfTerminal)){
		double** vector2 = malloc(sizeof(double*)*graphSize);
		for(size_t i = 0; i < graphSize; ++i){
			vector2[i] = malloc(sizeof(double)*2);
		}
		vector2 = dijkstra(inTree);
		
		
		
		min = INFINITY;
		
		temp = 0;
		for(size_t i = 1; i < numberOfTerminal; ++i){
			if(!marker[i]){
			//printf("hi + %f + %f\n",vector2[terminal[i]][0], vector[terminal[i]][1]);
				if(vector2[terminal[i]][0] < min && vector2[terminal[i]][1] != -1){
					//printf("ho\n");
					min = vector2[terminal[i]][0];
					temp = i;
				}	
			}
		}
		printf("min: %f\n", min);	
		marker[temp] = 1;
		//marker[temp] = 1;
		printf("temp %ld\n", temp);
		iter = terminal[temp];
		//printf("asd %ld\n", terminal[temp]);
		//printf("we %f\n", vector2[iter][1]);
		//printf("we2 %f\n", vector2[0][1]);
		while(vector2[iter][1] != -1){
			inTree[iter] = 1;
			//printf("gg %ld\n", iter);
			for(size_t h = 0; h < numberOfTerminal; ++h){
				if(iter == terminal[h]){
					marker[h] = 1;
					break;
				}
			}
			iter = (size_t) vector2[iter][1]; 
			//printf("we %f\n", vector[iter][1]);
			++nodesInTree;
		}		
		printf("min + val: %f+%f\n", min, value);			
		value = value + min;
		for(size_t i = 0; i < graphSize; ++i){
			free(vector2[i]);
		}
		free(vector2);	
		//printf("one more connected\n");	
	}
	return value;
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
	if (argc<=2){ 
		exit(EXIT_FAILURE);
	}
	fp = fopen(argv[1], "r");
    	if (fp == NULL)	exit(EXIT_FAILURE);
        
        size_t* terminal = malloc(sizeof(size_t)*(argc-1));
        size_t noOfTerminal = argc -2;
        //printf("tt %ld\n", noOfTerminal);
	for(int i = 2;i < argc; ++i){
		terminal[i-2] = strtoul(argv[i], NULL, 10)-1;
	}


	buildGraph(fp);
	double value = steinerTree(terminal, noOfTerminal);
	
	
	

	for (size_t i = 0; i<graphSize; ++i){
		for(size_t j = 0; j < vertexNo[i]; ++j){
			free(graph[i][j]);
		}
		free(graph[i]);
	}
	
	printf("Objective value:%f\n", value);
	        cpu = clock() - cpu;
        assert(!gettimeofday(&time,NULL));
	wall=(double)time.tv_sec + (double)time.tv_usec * .000001 -wall;
        printf("The programm used the CPU time of: %fs\n",((float)cpu)/CLOCKS_PER_SEC);
        printf("The programm used the wall time of: %fs\n", wall);
}
