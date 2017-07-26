#ifndef GRAPH_H
#define GRAPH_H

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
#include<omp.h>

#include "heap.h"


#define MAX_LINE_LEN 512
/**
* The graph data structure, with the amount of nodes, the amount of edges per node and a structure, which represents the graph.
*/
struct Graphs{
	size_t graphSize;
	size_t* vertexNo;
	size_t** graph;
};

/**
* The steiner tree struct, with and array with all the edges, the weight of the tree and the number of edges.
*/
struct SteinerTree{
	double value;
	size_t* tree;
	size_t noOfEdges;
};

/**
* Dijkstra data structure, which has of course the distance array and a marker to the first terminal reached.
*/
struct Dijkstra{
	double** dist;
	size_t firstNewTerminal;
};

/**
* Builds and fits the graph
*
* @param The input file.
*@return The graph.
*/
struct Graphs buildGraph(FILE *fp){
    	struct Graphs gra;	
	size_t i = 0;
        char delimiter[] = " ";
	char *ptr;
    	char *bla;
    	size_t len = 0;
    	char *line = (char*) malloc(MAX_LINE_LEN * sizeof(char));
    	getline(&line, &len, fp);
    	gra.graphSize = strtol(line, &bla, 10);
    	gra.edgeNo = strtol(bla, &bla, 10);
    	int temp1 = 0;
    	int temp2 = 0;
    	long int temp3 = 0;

    	gra.vertexNo =(size_t*) malloc(gra.graphSize*sizeof(size_t));
    	gra.graph = (size_t**) malloc(gra.graphSize*sizeof(size_t*));
    	
    	#pragma omp for
    	for (i = 0; i < gra.graphSize; ++i){
    		gra.graph[i] = (size_t*) malloc(sizeof(size_t*));
    		gra.vertexNo[i] = 0;
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

    			
			//adding one way
			gra.graph[temp1-1] = realloc(gra.graph[temp1-1], (gra.vertexNo[temp1-1]+1) * sizeof(size_t)*2);
			assert(gra.graph[temp1-1]);
			gra.graph[temp1-1][gra.vertexNo[temp1-1]*2 +0] = temp2;
			gra.graph[temp1-1][gra.vertexNo[temp1-1]*2+1] = temp3;   
			++gra.vertexNo[temp1-1];
			
    			
			//adding the other way
			gra.graph[temp2-1] = realloc(gra.graph[temp2-1], (gra.vertexNo[temp2-1]+1) * sizeof(size_t)*2);
			assert(gra.graph[temp2-1]);			
			gra.graph[temp2-1][gra.vertexNo[temp2-1]*2+0] = temp1;
			gra.graph[temp2-1][gra.vertexNo[temp2-1]*2+1] = temp3;   
			++gra.vertexNo[temp2-1];

		
		
				
		}
	}
	printf("Graph fitted\n");
	return gra;
}

/**
* Computes the shortest paths from nodes already in the steiner tree to every other else
* @param nodes already in steinertree, the graph, and the Terminals 
* @return shortest paths distance from the current steiner tree as a source at [0] and [1] the previus node
*
*/
struct Dijkstra dijkstra(struct Graphs gra, size_t* isTerminal, struct Dijkstra dij, char* inTree){
	//struct BinaryHeap heap;

	size_t minVal;

	

	//the dijkstra algo
	for(size_t j = 0; j < gra.graphSize; ++j)
	{	
		minVal = dij.heap.index66[0];
		if(!inTree[minVal] && (isTerminal[minVal+1] || minVal == 1)){
			dij.firstNewTerminal = minVal;
			return dij;
		}
		dij.heap.lastFirst = 0;
		dij.visited[minVal] = 1;
		for (size_t n= 0; n < gra.vertexNo[minVal]; n++){
			if(!dij.visited[gra.graph[minVal][2*n + 0]-1] && dij.dist[minVal] != INFINITY){
				if(dij.dist[minVal] + (double)gra.graph[minVal][2*n+1] < dij.dist[gra.graph[minVal][2*n+0]-1]){
					dij.dist[gra.graph[minVal][2*n+0]-1] = dij.dist[minVal] + (double)gra.graph[minVal][2*n+1];
					assert(!fetestexcept(FE_INVALID | FE_OVERFLOW | FE_UNDERFLOW));
					dij.prev[2*(gra.graph[minVal][2*n +0]-1)] = minVal;	
					dij.prev[2*(gra.graph[minVal][2*n +0]-1)+1] = gra.graph[minVal][2*n+1];
					dij.heap.heapVal[gra.graph[minVal][2*n+0]-1] = dij.dist[gra.graph[minVal][2*n + 0]-1];
					//update heap
					dij.heap = update(dij.heap.reversedIndex[gra.graph[minVal][2*n+0]-1], dij.heap);

				}
			}
		}
		dij.heap = popHeap(dij.heap, gra.graphSize);
		
	}
	return dij;
}

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
* @param All the terminal nodes, the number of terminal nodes, the current start terminal, the graph, and an array which says if this current node is a terminal
* @return The steiner tree with the objective value
*/
struct SteinerTree steinerTree(size_t* terminal, size_t numberOfTerminal, size_t startTerminal, struct Graphs gra, size_t* isTerminal, double bestStein){

	char* marker = malloc(numberOfTerminal*sizeof(char));
	char* inTree = malloc(gra.graphSize*sizeof(char));
	size_t counter = 2;
	struct SteinerTree stein;
	stein.tree = malloc(sizeof(double));
	size_t nodesInTree = 1;
	stein.noOfEdges = 1;
	stein.value = 0;
	stein.tree = realloc(stein.tree, sizeof(double)* gra.edgeNo*2);
	

	struct Dijkstra dij;
	dij.dist = malloc(sizeof(double)*gra.graphSize);
	dij.prev = malloc(sizeof(long int)*gra.graphSize*2);
	dij.visited =(char*) malloc(sizeof(char)*gra.graphSize);
	dij.heap.heapVal = malloc(sizeof(double)*gra.graphSize);
	dij.heap.index66 = malloc(sizeof(size_t)*gra.graphSize);
	dij.heap.reversedIndex = malloc(sizeof(size_t)*gra.graphSize);
	dij.firstNewTerminal = 0;
	
		//fitting the necesary vectors
	for (size_t i = 0; i < gra.graphSize; ++i){
		dij.visited[i] = 0;
		dij.heap.heapVal[i] = INFINITY;
		dij.heap.index66[i] = i;
		dij.heap.reversedIndex[i] = i;
		inTree[i] = 0;
			dij.dist[i] = INFINITY;
			dij.heap.heapVal[i] = INFINITY;
			dij.prev[2*i] = -2;
			dij.prev[2*i+1] = 0;

	}
	
	dij.heap.heapVal[terminal[startTerminal]] = 0;
	dij.dist[terminal[startTerminal]] = 0;
	dij.prev[2*terminal[startTerminal]] = -1;
	dij.prev[2*terminal[startTerminal]+1] = 0;
	//build the heap
	dij.heap = buildMinHeap(dij.heap, gra.graphSize);
	
	
	for(size_t i = 0; i<numberOfTerminal; ++i){
		marker[i] = 0;
	}

	
	inTree[terminal[startTerminal]] = 1;
	
	dij = dijkstra(gra, isTerminal, dij, inTree);
	marker[startTerminal] = 1;

	marker[isTerminal[dij.firstNewTerminal+1]] = 1;
	stein.value = 0;
	long int iter = dij.firstNewTerminal;
	size_t overload = iter;
	//adding a path to the new terminal to the tree
	while(dij.prev[2*iter] != -1){
		inTree[iter] = 1;
		overload = iter;
		stein.value += dij.prev[2*iter+1];
		if(bestStein <= stein.value){
			stein.value = INFINITY;
			free(stein.tree);
			free(dij.heap.index66);
			free(dij.heap.reversedIndex);
			free(dij.visited);
			free(dij.dist);
			free(dij.heap.heapVal);
			free(inTree);
			free(marker);
			return stein;
		}
		dij.heap.heapVal[iter] = 0;
		dij.dist[iter] = 0;
		dij.visited[iter] = 0;
		dij.heap = update(dij.heap.reversedIndex[iter], dij.heap);
		stein.tree[2*(stein.noOfEdges-1)] =  (size_t) iter;
		iter = dij.prev[2*iter]; 
		dij.prev[2*overload] = -1;		
		stein.tree[2*(stein.noOfEdges-1)+1] = (size_t) iter;
		++stein.noOfEdges;
		++nodesInTree;
	}
	//dij.prev[2*overload] = -1;		
	for( ;counter < numberOfTerminal; ++counter){
		//printf("hi\n");	
		dij = dijkstra(gra, isTerminal, dij, inTree);
		marker[isTerminal[dij.firstNewTerminal+1]] = 1;
		//++counter;

		iter = dij.firstNewTerminal;
		overload = iter;
		//adding a path to the new terminal to the tree
		while(dij.prev[2*iter] != -1){
			inTree[iter] = 1;
			overload = iter;
			stein.value += dij.prev[2*iter+1];
			if(bestStein <= stein.value){
					stein.value = INFINITY;
					free(stein.tree);
					free(dij.heap.index66);
					free(dij.heap.reversedIndex);
					free(dij.visited);
					free(dij.dist);
					free(dij.heap.heapVal);
					free(inTree);
					free(marker);
					return stein;
			}
			dij.heap.heapVal[iter] = 0;
			dij.dist[iter] = 0;

			dij.visited[iter] = 0;
			dij.heap = update(dij.heap.reversedIndex[iter], dij.heap);
			stein.tree[2*(stein.noOfEdges-1)] =  (size_t) iter;
			iter = dij.prev[2*iter]; 
			dij.prev[2*overload] = -1;		
			stein.tree[2*(stein.noOfEdges-1)+1] = (size_t) iter;
			++stein.noOfEdges;
			++nodesInTree;
		}		
		//adding objective value which gets added through the new node
	
	}
	
	free(dij.heap.index66);
	free(dij.heap.reversedIndex);
	free(dij.visited);
	free(dij.dist);
	free(dij.heap.heapVal);
	free(inTree);
	free(marker);


	return stein;
}

/**
* Checks if the graph is a tree. This is the recursive part for dfs
*
* @param A graph, the father node, the node where are currently standing at and the char array, where the visited nodes are marked
* @return The char array with the visited nodes. If at the first position is a f, then it is not a tree and cycle was found
*/
char* dfsRec(struct Graphs gra, size_t father, size_t start,char* visited){
	visited[start] = 1;
	for(size_t i = 0; i < gra.vertexNo[start]; ++i){
		if(gra.graph[start][i] != father){
			if(!visited[gra.graph[start][i]]){
				visited = dfsRec(gra, start, gra.graph[start][i], visited);
			}
			else{
				visited[0] = 'f';
				return visited;
			}
		}
	}
	return visited;
}		

/**
* Checks if the graph is a tree.
*
* @param A graph, the start node and the char array, where the visited nodes are marked
* @return The char array with the visited nodes. If at the first position is a f, then it is not a tree and cycle was found
*/
char* dfs(struct Graphs gra, size_t start, char* visited){
	visited[start] = 1;
	if(gra.vertexNo[start] == 0){
		visited[0] = 'f';
		
		return visited;
	}
	for(size_t i = 0; i < gra.vertexNo[start]; ++i){
		visited = dfsRec(gra, start, gra.graph[start][i], visited);
	}
	return visited;                 
}


/**
* Checks for the correctness of the solution
*
* @param The supposetely steiner tree, the graph, the terminal set and the amount of terminals
* @return 1 if the solution is a steiner tree and 0 otherwise
*/
char solChecker(struct SteinerTree stein, struct Graphs gra, size_t* terminal, size_t noOfTerminal){
	//overwriting now the original graph.
	for(size_t i = 0; i < gra.graphSize; ++i){
		free(gra.graph[i]);
		gra.vertexNo[i] = 0;
		gra.graph[i] = malloc(sizeof(size_t));
	}
	
	//building now the tree, which we want to check by DPI
	for(size_t i = 0; i < stein.noOfEdges-1; ++i){
	
		gra.graph[stein.tree[2*i]] = realloc(gra.graph[stein.tree[2*i]], sizeof(size_t)*(gra.vertexNo[stein.tree[2*i]]+1));
		gra.graph[stein.tree[2*i]][gra.vertexNo[stein.tree[2*i]]] = stein.tree[2*i +1];
		++gra.vertexNo[stein.tree[2*i]];
		
		gra.graph[stein.tree[2*i+1]] = realloc(gra.graph[stein.tree[2*i+1]], sizeof(size_t)*(gra.vertexNo[stein.tree[2*i+1]]+1));
		gra.graph[stein.tree[2*i+1]][gra.vertexNo[stein.tree[2*i+1]]] = stein.tree[2*i];
		++gra.vertexNo[stein.tree[2*i+1]];		
	}
	
	//markes every visited node and marks if the steiner-tree is not a tree
	char* visited = malloc(sizeof(char)*gra.graphSize);
	for(size_t i = 0; i< gra.graphSize; ++i){
		*(visited+i) = 0;
	}
	visited = dfs(gra, 1, visited);
	if(visited[0] == 'f') return 0;
	
	//checks if every terminal is beeing visited
	for(size_t i = 0; i < noOfTerminal; ++i){
		
		if(!visited[terminal[i]])
		{ 	
			
			printf("Terminal not connected! %ld, should be connected, %ld\n", terminal[i]+1, gra.vertexNo[terminal[i]]);
			for(size_t j = 0; j< 2*stein.noOfEdges; ++j){
			if(stein.tree[j] == terminal[i])
				printf("been here!\n");
			}
			
			return 0;
		}
	}
	return 1;
}
#endif
