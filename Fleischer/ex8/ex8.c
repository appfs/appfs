/**
 * @file
 * @author  Sven Fleischer
 * @version 2.0

 *
 * @section DESCRIPTION
 *
 * This program parses a graph and builds a steiner tree with every prime indexed node as terminal. In this programm are components, which gives the oppurtunity to give out every edge of the tree. And every, through the heuristic computed tree is compared, which means, that we let the heuristic work starting with a different start terminal in the beginning.
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
//size_t temp2;
size_t vertexInd;
size_t lastFirst;
double* value;



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
*Findes every prime indexed node
*@return An array which workes as an boolen, and says that node x is a prime indexed node.
*/
char* findPrimes(){
	char* numbers = malloc(sizeof(char)*(graphSize+1));
	numbers[0] = 0;
	numbers[1] = 0;
	for (size_t i = 2; i <= graphSize; ++i){
		numbers[i] = 1;
	}
	double pows = pow((double) graphSize, 0.5);
	for (size_t i = 2; (double) i < pows; ++i){
   		if(numbers[i]){
   			size_t num = 0;
   			size_t j = 0;
   			while((j = i*i+num*i) <= graphSize){
   				numbers[j] = 0;
   				++num;
   			}
   		}
	} 
	return numbers;
}

/**
* Makes shure everything is still a heap.
*
*@param The heap array and the array with the values, on which the heap based should be build.
*@return The heap array, where the first line is are index in heap order and in the second one we can read out where to find which node in the heap.
*
*/

size_t** minHeapify(size_t i, size_t** index66, double* heapVal){
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
    if (left < graphSize && heapVal[index66[left][0]] < heapVal[index66[smallest][0]]){
        smallest = left;
    }  
    if (right < graphSize && heapVal[index66[right][0]] < heapVal[index66[smallest][0]]){
        smallest = right;
    }    
    
    if (smallest != i){
    	size_t temp6 = index66[i][0];
    	index66[i][0] = index66[smallest][0];
    	index66[smallest][0] = temp6;
    	index66[index66[i][0]][1] = i;
    	index66[index66[smallest][0]][1] = smallest;
        index66 = minHeapify(smallest, index66, heapVal);
    }  
    return index66;   
}        

/**
* Check if still heap for updated element.

*@param The heap array and the array with the values, on which the heap based should be build.
*@return The heap array, where the first line is are index in heap order and in the second one we can read out where to find which node in the heap.
*/
size_t** update(size_t i, size_t** index66, double* heapVal){
	size_t child;
    	size_t father;
    	if(i ==0){
    		return index66;
    	}
    	else{
    		child = i;
    		father = i/2;
    	}
    	
    	if(heapVal[index66[child][0]] < heapVal[index66[father][0]]){
    		size_t temp6 = index66[father][0];
    		if (father == lastFirst){
    			lastFirst == child;
    		}
    		index66[father][0] = index66[child][0];
    		index66[child][0] = temp6;
    		index66[index66[child][0]][1] = child;
    		index66[index66[father][0]][1] = father;
        	index66 = update(father, index66, heapVal);
    	}
    	return index66;
}

/**
* Builds the heap.
*@param The heap array and the array with the values, on which the heap based should be build.
*@return The heap array, where the first line is are index in heap order and in the second one we can read out where to find which node in the heap.
*/

size_t** buildMinHeap(size_t** index66, double* heapVal){
    
    for(size_t i = graphSize/2; i>0; --i){
        index66 = minHeapify(i, index66, heapVal);
    } 
    index66 = minHeapify(0, index66, heapVal);
    return index66;	        
}        



/**
* Computes the shortest paths from nodes already in the steiner tree to every other else
* @param nodes already in steinertree 
* @return shortest paths distance at [0] and [1] the previus node
*
*/

double** dijkstra(char* inTree, double** dist, double* heapVal, size_t** index66){
	char* visited =(char*) malloc(sizeof(char)*graphSize);
	size_t minVal;

	for (size_t i = 0; i < graphSize; ++i){
		index66[i] =  malloc(sizeof(size_t)*2);
		visited[i] = 0;
		index66[i][0] = i;
		index66[i][1] = i;

		if(inTree[i] == 0){
			dist[i][0] = INFINITY;
			heapVal[i] = INFINITY;
			dist[i][1] = -2;
		}
		else{
			heapVal[i] = 0;
			dist[i][0] = 0;
			dist[i][1] = -1;
		}
	}
	

	index66 = buildMinHeap(index66, heapVal);
	
	for(size_t j = 0; j < graphSize; ++j)
	{
		minVal = index66[0][0];
		if (minVal == INFINITY){
			free(visited);
			return dist;
		}
		lastFirst = 0;
		visited[minVal] = 1;
		for (size_t n= 0; n < vertexNo[minVal]; n++){
			if(!visited[graph[minVal][n][0]-1] && dist[minVal][0] != INFINITY){
				if(dist[minVal][0] + (double)graph[minVal][n][1] < dist[graph[minVal][n][0]-1][0]){
					dist[graph[minVal][n][0]-1][0] = dist[minVal][0] + (double)graph[minVal][n][1];
					dist[graph[minVal][n][0]-1][1] = minVal;
					heapVal[graph[minVal][n][0]-1] = dist[graph[minVal][n][0]-1][0];
					//update heap
					index66 = update(index66[graph[minVal][n][0]-1][1], index66, heapVal);
					assert(!fetestexcept(FE_INVALID | FE_OVERFLOW | FE_UNDERFLOW));
				}
			}
		}
		heapVal[index66[lastFirst][0]] = INFINITY;
		index66 = minHeapify(lastFirst, index66, heapVal);
		
	}

	free(visited);

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
double** steinerTree(size_t* terminal, size_t numberOfTerminal, size_t startTerminal){
	char* marker = malloc(numberOfTerminal*sizeof(char));
	char* inTree = malloc(graphSize*sizeof(char));
	
	double* heapVal = (double*) malloc(sizeof(double)*graphSize);
	size_t** index66 = (size_t**) malloc(sizeof(size_t*)*graphSize);
	size_t nodesInTree = 1;
	size_t noOfEdges = 1;
	double min = INFINITY;
	
	value[startTerminal] = 0;
	
	double** tree = malloc(sizeof(double*)*1);
	size_t temp = 0;	
	double** vector = malloc(sizeof(double*)*graphSize);

	for(size_t i = 0; i < graphSize; ++i){
		vector[i] = malloc(sizeof(double)*2);
		inTree[i] = 0;
	}
	
	for(size_t i = 0; i<numberOfTerminal; ++i){
		marker[i] = 0;
	}
	inTree[terminal[startTerminal]] = 1;
	vector = dijkstra(inTree, vector, heapVal, index66);
	marker[startTerminal] = 1;
	//search for cheapest next terminal
	for(size_t i = 1; i < numberOfTerminal; ++i){
		if(vector[terminal[i]][0] < min && vector[terminal[i]][1] != -1){
		min = vector[terminal[i]][0];
		temp = i;
		}
	}
	marker[temp] = 1;
	value[startTerminal] = min;
	size_t iter = terminal[temp];
	tree[0] = malloc(sizeof(double));
	while(vector[iter][1] != -1){
		inTree[iter] = 1;
		tree = realloc(tree, sizeof(double)* noOfEdges);
		tree[noOfEdges-1] = malloc(sizeof(double)* 2);
		tree[noOfEdges-1][0] = (double) iter;
		iter = (size_t) vector[iter][1]; 
		tree[noOfEdges-1][1] = (double) iter;
		++noOfEdges;
		++nodesInTree;
	}
	
	//repeat the hole procedure
	while (terminalsLeft(marker, numberOfTerminal)){
		vector = dijkstra(inTree, vector, heapVal, index66);

		min = INFINITY;

		temp = 0;
		for(size_t i = 0; i < numberOfTerminal; ++i){
			if(!marker[i]){

				if(vector[terminal[i]][0] < min && vector[terminal[i]][1] != -1){//printf("i: %ld\n",i);

					min = vector[terminal[i]][0];
					temp = i;
				}	
			}
		}

		marker[temp] = 1;

		iter = terminal[temp];

		while(vector[iter][1] != -1){

			inTree[iter] = 1;

			for(size_t h = 0; h < numberOfTerminal; ++h){
				if(iter == terminal[h]){
					marker[h] = 1;
				}
			}
			tree = realloc(tree, sizeof(double)* noOfEdges);
			tree[noOfEdges-1] = malloc(sizeof(double)* 2);
			tree[noOfEdges-1][0] = (double) iter;
			iter = (size_t) vector[iter][1]; 
			tree[noOfEdges-1][1] = (double) iter;
			//printf("sko\n");
			++noOfEdges;
			++nodesInTree;
		}		

		value[startTerminal] = value[startTerminal] + min;

	}
	
	for(size_t i = 0; i < graphSize; ++i){
		free(vector[i]);
		free(index66[i]);
	}
	free(inTree);
	free(marker);
	free(heapVal);
	free(index66);
	free(vector);
	return tree;
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
        
        size_t* terminal = malloc(sizeof(size_t)*(1));
        size_t noOfTerminal = 0;
        terminal[0] = 1;


	
	buildGraph(fp);
	assert(graphSize > 1);
	char* primes = findPrimes();
	printf("primes found\n");
	for(size_t i = 3; i <= graphSize; ++i){
		if(primes[i]){
			++noOfTerminal;		
			terminal = realloc(terminal, sizeof(size_t)*(noOfTerminal+1));
			terminal[noOfTerminal] = i -1;
		}
	}
	++noOfTerminal;		
	printf("Terminal vector fitted\n");
	value = malloc(sizeof(double)*noOfTerminal);
	value[0] = 0;
	double*** tree = malloc(sizeof(double**)*noOfTerminal);
	for(size_t i = 0; i < noOfTerminal; ++i){
		tree[i] = steinerTree(terminal, noOfTerminal, i);
	}
	
	double minValue = value[0];
	size_t j = 0;
	size_t howFar = 100;
	if(noOfTerminal < 100){
		howFar = noOfTerminal;
	}
	for(size_t i = 1; i < howFar; ++i){
		if(minValue > value[i]){
			j = i;
			minValue = value[i];
		}
	}

	
	printf("Objective value: %f\n", minValue);
	        cpu = clock() - cpu;
        assert(!gettimeofday(&time,NULL));
	wall=(double)time.tv_sec + (double)time.tv_usec * .000001 -wall;
        printf("The programm used the CPU time of: %fs\n",((float)cpu)/CLOCKS_PER_SEC);
        printf("The programm used the wall time of: %fs\n", wall);
}
