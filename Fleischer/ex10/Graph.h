/**
*@file Graph.h
*@author Sven Fleischer
*There I've coded everything, which has to do anything with the graph
*/

#ifndef GRAPH_H
#define GRAPH_H



#include "dataStruct.h"


#define MAX_LINE_LEN 512

/**
* The graph data structure, with the amount of nodes, the amount of edges per node and a structure, which represents the graph.
*/
struct Graphs{
	size_t graphSize;
	size_t* vertexNo;
	size_t edgeNo;
	unsigned int** graph;//an array of every node length, where every node is stored, with the cost, with which it is connected
};

/**
* The steiner tree struct, with an array with all the edges, the weight of the tree and the number of edges.
*/
struct SteinerTree{
	double value;//objective value
	long int* tree;//store here always the precessor. Is enough for a tree
};

/**
* varForDijkstra data structure, which has the distance array and a marker to the first terminal reached.
*/
struct varForDijkstra{
	double* dist;//distance vector
	size_t firstNewTerminal;//first reached terminal
	long int* prev;//array, where the path is saved+the cost every edge adds
	char* visited;
	struct BinaryHeap heap;
};

/**
* Parses the graph from a gph file.
*
* @param The input file.
*@return The graph.
*/
struct Graphs buildGraph(FILE *fp){
    	struct Graphs gra;	

    	char *tmp; //pointer used for cutting strings
    	char *line = malloc(MAX_LINE_LEN * sizeof(char));
    	int from = 0;
    	int to = 0;
    	long int weightOfEdge = 0;	
	size_t i = 0;
	size_t len = 0;//needed for getline
    	
    	getline(&line, &len, fp);
    	
    	//readout the numer of nodes and edges
    	gra.graphSize = strtol(line, &tmp, 10);
    	gra.edgeNo = strtol(tmp, &tmp, 10);
    	

    	gra.vertexNo = malloc(gra.graphSize*sizeof(size_t));
    	gra.graph = malloc(gra.graphSize*sizeof(size_t*));
    	
    	//fitting graph
    	#pragma omp for
    	for (i = 0; i < gra.graphSize; ++i)
    	{
    		gra.graph[i] = malloc(sizeof(size_t*));
    		gra.vertexNo[i] = 0;
    	}
    	//filling the graph by reading out line by line
	while ((getline(&line, &len, fp)) != -1) 
	{
		if (*(line) != '\n'){
			//interprete the line, and fit it into the graph
			from = strtol(line, &tmp, 10);
			to = strtol(tmp, &tmp, 10);
			weightOfEdge = strtol(tmp, &tmp, 10);
			assert(from >=0);
			assert(to >=0);
			assert(weightOfEdge >=0);

    			
			//adding one way
			gra.graph[from-1] = realloc(gra.graph[from-1], (gra.vertexNo[from-1]+1) * sizeof(size_t)*2);
			assert(gra.graph[from-1]);
			gra.graph[from-1][gra.vertexNo[from-1]*2 +0] = to;
			gra.graph[from-1][gra.vertexNo[from-1]*2+1] = weightOfEdge;   
			++gra.vertexNo[from-1];
			
    			
			//adding the other way
			gra.graph[to-1] = realloc(gra.graph[to-1], (gra.vertexNo[to-1]+1) * sizeof(size_t)*2);
			assert(gra.graph[to-1]);			
			gra.graph[to-1][gra.vertexNo[to-1]*2+0] = from;
			gra.graph[to-1][gra.vertexNo[to-1]*2+1] = weightOfEdge;   
			++gra.vertexNo[to-1];
		
		}
	}
	printf("Graph read\n");
	return gra;
}

/**
* Computes the shortest paths from nodes already in the steiner tree to the next terminal, and saves the path, too.
* @param The graph, and the Terminals, the variables for Dijkstra and the nodes already in the tree
* @return The shortest path from the current nodes in the tree to the closest steiner tree, with the path and the weight of it
*
*/
struct varForDijkstra dijkstra(struct Graphs gra, size_t* isTerminal, struct varForDijkstra dij, char* inTree){

	size_t minVal;

	//the dijkstra algo
	//this should run till it reaches the first terminal, therefore breakup condition written into the loop
	while(1)
	{	
		//getting currently cheapest connected node
		minVal = dij.heap.positionInHeap[0];
		
		//breakup condition if termial is reached
		if(!inTree[minVal] && (isTerminal[minVal+1] || minVal == 1))
		{
			dij.firstNewTerminal = minVal;
			return dij;
		}
		//set it visited and reset lastFirst 0
		dij.heap.lastFirst = 0;
		dij.visited[minVal] = 1;
		//update if a new shorter way is found
		for (size_t n= 0; n < gra.vertexNo[minVal]; n++){
			if(!dij.visited[gra.graph[minVal][2*n + 0]-1] && dij.dist[minVal] != INFINITY && dij.dist[minVal] + (double)gra.graph[minVal][2*n+1] < dij.dist[gra.graph[minVal][2*n+0]-1])
			{
				dij.dist[gra.graph[minVal][2*n+0]-1] = dij.dist[minVal] + (double)gra.graph[minVal][2*n+1];
				assert(!fetestexcept(FE_INVALID | FE_OVERFLOW | FE_UNDERFLOW));
				dij.prev[2*(gra.graph[minVal][2*n +0]-1)] = minVal;	
				dij.prev[2*(gra.graph[minVal][2*n +0]-1)+1] = gra.graph[minVal][2*n+1];
				dij.heap.heapVal[gra.graph[minVal][2*n+0]-1] = dij.dist[gra.graph[minVal][2*n + 0]-1];
				
				//update heap
				dij.heap = update(dij.heap.reversedIndex[gra.graph[minVal][2*n+0]-1], dij.heap);
			}
			
		}
		//remove root element, be setting is infinty and pushing it down in the heap
		dij.heap = makeFirstIrrelevant(dij.heap, gra.graphSize);
		
	}
	return dij;
}


/**
* Finds a steiner tree.
*
* @param All the terminal nodes, the number of terminal nodes, the current start terminal, the graph, and an array which says if this current node is a terminal, and an upper bound, if this is violated, the function stops
* @return The steiner tree with the objective value
*/
struct SteinerTree getSteinerTree(size_t* terminal, size_t numberOfTerminal, size_t startTerminal, struct Graphs gra, size_t* isTerminal, double bestStein){
	
	//setting intermal variables for the steiner tree, if this node is already in the tree
	char* inTree = malloc(gra.graphSize*sizeof(char));
	size_t counter = 2;
	
	//declaring tree
	struct SteinerTree stein;
	stein.tree = malloc(sizeof(stein.tree)*gra.graphSize);

	//setting objective value 0 at the beginning
	stein.value = 0;

	
	//fitting everything for dijkstra, so heap indexholder, previous node and visited
	struct varForDijkstra dij;
	dij.dist = malloc(sizeof(double)*gra.graphSize);
	dij.prev = malloc(sizeof(long int)*gra.graphSize*2);
	dij.visited = malloc(sizeof(char)*gra.graphSize);
	dij.heap.heapVal = malloc(sizeof(double)*gra.graphSize);
	dij.heap.positionInHeap = malloc(sizeof(dij.heap.positionInHeap)*gra.graphSize);
	dij.heap.reversedIndex = malloc(sizeof(dij.heap.reversedIndex)*gra.graphSize);
	dij.firstNewTerminal = 0;
	
	//fitting the necesary vectors
	for (size_t i = 0; i < gra.graphSize; ++i)
	{
		stein.tree[i] = -1;
		dij.visited[i] = 0;
		dij.heap.heapVal[i] = INFINITY;
		dij.heap.positionInHeap[i] = i;
		dij.heap.reversedIndex[i] = i;
		inTree[i] = 0;
		dij.dist[i] = INFINITY;
		dij.heap.heapVal[i] = INFINITY;
		dij.prev[2*i] = -2;
		dij.prev[2*i+1] = 0;
	}
	
	//mark for dijkstra the source
	dij.heap.heapVal[terminal[startTerminal]] = 0;
	dij.dist[terminal[startTerminal]] = 0;
	dij.prev[2*terminal[startTerminal]] = -1;
	dij.prev[2*terminal[startTerminal]+1] = 0;
	
	//build the heap
	dij.heap = buildMinHeap(dij.heap, gra.graphSize);


	//show that the start terminal is in the tree
	inTree[terminal[startTerminal]] = 1;
	
	//calculate shortest way till first other terminal
	dij = dijkstra(gra, isTerminal, dij, inTree);
	
	
	//setting iterator, which node is added to the tree
	long int iter = dij.firstNewTerminal;
	size_t overload = iter;
	
	//adding a path to the new terminal to the tree, and update everything precisely for the dijkstra, that it can start there, where it ended
	while(dij.prev[2*iter] != -1)
	{
		inTree[iter] = 1;
		overload = iter;
		stein.value += dij.prev[2*iter+1];
		assert(!fetestexcept(FE_INVALID | FE_OVERFLOW | FE_UNDERFLOW));
		
		//check for break up condition
		if(bestStein <= stein.value)
		{
			stein.value = INFINITY;
			free(stein.tree);
			free(dij.heap.positionInHeap);
			free(dij.heap.reversedIndex);
			free(dij.visited);
			free(dij.dist);
			free(dij.heap.heapVal);
			free(inTree);
			return stein;
		}
		dij.heap.heapVal[iter] = 0;
		dij.dist[iter] = 0;
		dij.visited[iter] = 0;
		dij.heap = update(dij.heap.reversedIndex[iter], dij.heap);
		stein.tree[iter] =  dij.prev[2*iter];
		
		//making the next step
		iter = dij.prev[2*iter]; 
		dij.prev[2*overload] = -1;		
	}
	//each step a terminal is added, so repeat is so often till we should have enough termianls
	for( ;counter < numberOfTerminal; ++counter)
	{
		dij = dijkstra(gra, isTerminal, dij, inTree);
		
		//setting new iterator
		iter = dij.firstNewTerminal;
		overload = iter;
		
		//adding a path to the new terminal to the tree, and setting their(on the path laying) nodes to 0 for dijkstra
		while(dij.prev[2*iter] != -1)
		{
			inTree[iter] = 1;
			overload = iter;
			stein.value += dij.prev[2*iter+1];
			assert(!fetestexcept(FE_INVALID | FE_OVERFLOW | FE_UNDERFLOW));			
			
			//check for break up condition
			if(bestStein <= stein.value)
			{
					stein.value = INFINITY;
					free(stein.tree);
					free(dij.heap.positionInHeap);
					free(dij.heap.reversedIndex);
					free(dij.visited);
					free(dij.dist);
					free(dij.heap.heapVal);
					free(inTree);
					return stein;
			}
			dij.heap.heapVal[iter] = 0;
			dij.dist[iter] = 0;
			dij.visited[iter] = 0;
			dij.heap = update(dij.heap.reversedIndex[iter], dij.heap);
			assert(stein.tree[iter] == -1);//this has to hold for a tree
			stein.tree[iter] =  dij.prev[2*iter];
			iter = dij.prev[2*iter]; 
			dij.prev[2*overload] = -1;		
		}		
	}
	
	//check if tree is still smaller, if delete it so save memory 
	if(bestStein <= stein.value)
	{
		stein.value = INFINITY;
		free(stein.tree);
		free(dij.heap.positionInHeap);
		free(dij.heap.reversedIndex);
		free(dij.visited);
		free(dij.dist);
		free(dij.heap.heapVal);
		free(inTree);
		return stein;
	}
	//free everything
	free(dij.heap.positionInHeap);
	free(dij.heap.reversedIndex);
	free(dij.visited);
	free(dij.dist);
	free(dij.heap.heapVal);
	free(inTree);

	return stein;
}
		

/**
* Checks if the graph is a tree.
*
* @param A graph, the start node and the char array, where the visited nodes are marked
* @return The char array with the visited nodes. If at the first position is a f, then it is not a tree and cycle was found
*/


char* dfs(struct Graphs gra, size_t start, char* visited){
	struct Stack stk;
	stk.data = malloc(sizeof(unsigned int)*gra.graphSize);
	stk.parents = malloc(sizeof(unsigned int)*gra.graphSize);
    	stk.topElement = 0;
    	stk.maxSize = gra.graphSize +1;
    	stk = push(start, start,stk);
    	size_t next;
    	size_t parent;
    	//as long the stack is not empty
    	while(stk.topElement != 0)
    	{
    		next = stk.data[stk.topElement];
    		parent = stk.parents[stk.topElement];
    		stk = pop(stk);
    		visited[next] = 1;
	    	for(size_t i = 0; i < gra.vertexNo[next]; ++i)
		{	
			if(gra.graph[next][i] != parent)
			{
				//connecting to new ones, quit if already connected
				if(!visited[gra.graph[next][i]])
				{
					assert(next < gra.graphSize);
					stk = push(gra.graph[next][i], next,stk);
				}
				else
				{
					//set that dfs failed
					visited[0] = 'f';
					return visited;
				}
			}
		}
    	}
    	return visited;
}


/**
* Checks for the correctness of the solution
*
* @param The supposetely steiner tree, the graph, the terminal set and the amount of terminals
* @return 1 if the solution is a steiner tree and 0 otherwise
*/
char solChecker(struct SteinerTree stein, struct Graphs gra, size_t* terminal, size_t noOfTerminal, int numThreads){
	omp_set_num_threads(numThreads);
	char* visited = malloc(sizeof(char)*gra.graphSize);
	//overwriting now the original graph.
	for(size_t i = 0; i < gra.graphSize; ++i)
	{
		free(gra.graph[i]);
		gra.vertexNo[i] = 0;
		gra.graph[i] = malloc(sizeof(unsigned int));
		visited[i] = 0;
	}
	
	//building now the tree, which we want to check by dfs
	for(size_t i = 0; i < gra.graphSize; ++i){
		if(stein.tree[i] !=-1)
		{
		//add one way
		gra.graph[i] = realloc(gra.graph[i], sizeof(unsigned int)*(gra.vertexNo[i]+1));
		gra.graph[i][gra.vertexNo[i]] = stein.tree[i];
		++gra.vertexNo[i];
		//and another
		gra.graph[stein.tree[i]] = realloc(gra.graph[stein.tree[i]], sizeof(unsigned int)*(gra.vertexNo[stein.tree[i]]+1));
		gra.graph[stein.tree[i]][gra.vertexNo[stein.tree[i]]] = i;
		++gra.vertexNo[stein.tree[i]];
		}		
	}
	
	//markes every visited node and marks if the steiner-tree is not a tree
	visited = dfs(gra, 1, visited);
	if(visited[0] == 'f') return 0;
	
	//checks if every terminal is beeing visited
	for(size_t i = 0; i < noOfTerminal; ++i)
	{
		//is not complain, and say which one
		if(!visited[terminal[i]])
		{ 	
			return 0;
		}
	}
	return 1;
}
#endif
