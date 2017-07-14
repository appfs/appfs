/**
 * @file
 * @author  Sven Fleischer
 * @version 2.1

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
double* heapVal;
//size_t temp2;
size_t vertexInd;
size_t lastFirst;
char* temp;
size_t* index66;
size_t* reversedIndex;



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
* Makes shure everything is still a heap.
*@param The heap-array
*@return The heap-array
*
*/

size_t* minHeapify(size_t i){
    size_t left;
    size_t right;
    size_t smallest;
    
    if(i != 0){
    	left = 2*i;
    	right = 2*i + 1;
    	smallest = i;
    }
    else{
    	left = 1;
    	right = 2;
    	smallest = 0;    	
    }
    if (left < graphSize && heapVal[index66[left]] < heapVal[index66[smallest]]){
    	//printf("HHH\n");
        smallest = left;
    }  
    if (right < graphSize && heapVal[index66[right]] < heapVal[index66[smallest]]){
        smallest = right;
    }    
    
    if (smallest != i){
    	size_t temp6 = index66[i];
    	index66[i] = index66[smallest];
    	index66[smallest] = temp6;
    	reversedIndex[index66[i]] = i;
    	reversedIndex[index66[smallest]] = smallest;
        index66 = minHeapify(smallest);
    }  
    return index66;   
}        

/**
* Check if still heap for updated element.
*@param The heap-array
*@return The heap array
*
*/
size_t* update(size_t i){
	size_t child;
    	size_t father;
    	if(i ==0){
    		return index66;
    	}
    	else{
    		child = i;
    		father = i/2;
    	}
    	
    	if(heapVal[index66[child]] < heapVal[index66[father]]){
    		size_t temp6 = index66[father];
    		if (father == 0){
    			lastFirst == child;
    		}
    		index66[father] = index66[child];
    		index66[child] = temp6;
    		reversedIndex[index66[child]] = child;
    		reversedIndex[index66[father]] = father;
        	index66 = update(father);
    	}
    	return index66;
}

/**
* Builds the heap.
*@return The heap in array form.
*
*/

size_t* buildMinHeap(){
    
    for(size_t i = graphSize/2; i>0; --i){
        index66 = minHeapify(i);
    } 
    index66 = minHeapify(0);
    return index66;	        
}        

/**
* Deletes the first element.
*@return The heap array
*
*/

size_t* popHeap(){
	heapVal[index66[lastFirst]] = INFINITY;
    	index66 = minHeapify(lastFirst);
	return index66;
}


/**
* Computes the shortest paths from vertex with inex 0 to every other else
*@return An array with all the values of the solution
*
*/

double* dijkstra(){
	
	dist = (double*) malloc(sizeof(double)*graphSize);
	heapVal = (double*) malloc(sizeof(double)*graphSize);
	index66 = (size_t*) malloc(sizeof(size_t)*graphSize);
	reversedIndex = (size_t*) malloc(sizeof(size_t)*graphSize);
	temp =(char*) malloc(sizeof(char)*graphSize);
	size_t minVal;
	
	for (size_t i = 0; i < graphSize; ++i){
		dist[i] = INFINITY;
		temp[i] = 0;
		heapVal[i] = INFINITY;
		index66[i] = i;
		reversedIndex[i] = i;
	}
	
	dist[0] = 0;
	heapVal[0] =0;
	index66 = buildMinHeap();
	
	for(size_t j = 0; j < graphSize; ++j){
		minVal = index66[0];
		if (minVal == INFINITY){
			return dist;
		}
		lastFirst = 0;
		size_t deapTree = 0;
		temp[minVal] = 1;
		for (size_t n= 0; n < vertexNo[minVal]; n++){
			if(!temp[graph[minVal][n][0]-1] && dist[minVal] != INFINITY){
				if(dist[minVal] + (double)graph[minVal][n][1] < dist[graph[minVal][n][0]-1]){
					dist[graph[minVal][n][0]-1] = dist[minVal] + (double)graph[minVal][n][1];
					heapVal[graph[minVal][n][0]-1] = dist[graph[minVal][n][0]-1];
					index66 = update(reversedIndex[graph[minVal][n][0]-1]);
					assert(!fetestexcept(FE_INVALID | FE_OVERFLOW | FE_UNDERFLOW));
				}
			}
		}
		index66 = popHeap();
		//deapTree);		
		
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
        

        size_t indEx1 = 0;
        double max = 0;
        
	buildGraph(fp);
	dijkstra();
	for (size_t i = 0; i < graphSize; ++i){
		if (max < dist[i]){
			max = dist[i];
			indEx1 = i;
		}
	}
	for (size_t i = 0; i<graphSize; ++i){
		for(size_t j = 0; j < vertexNo[i]; ++j){
			free(graph[i][j]);
		}
		free(graph[i]);
	}
	printf("RESULT VERTEX %ld \n", indEx1+1);
        printf("RESULT DIST %ld \n", (size_t)max);
	
	        cpu = clock() - cpu;
        assert(!gettimeofday(&time,NULL));
	wall=(double)time.tv_sec + (double)time.tv_usec * .000001 -wall;
        printf("The programm used the CPU time of: %fs\n",((float)cpu)/CLOCKS_PER_SEC);
        printf("The programm used the wall time of: %fs\n", wall);
}					 	    	
    	
