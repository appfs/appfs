/**
 * @file
 * @author  Sven Fleischer
 * @version 2.0

 *
 * @section DESCRIPTION
 *
 * This program parses a graph and build a steiner tree, with prime nodes (node index start at 1) as terminal.
 */

#define _GNU_SOURCE
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<math.h> 
#include<assert.h>
#include<ctype.h>
#include<fenv.h>
#include<time.h>
#include<sys/time.h>
#include<omp.h>

#include"Graph.h"
#include"heap.h"
#include"misc.h"


/**
* Calls the functions, which parse the graph computes and checks if the solution is a steiner tree.
*
* @param The input file. At argv[1] should be input file, at argv[2] the number of start terminals, which should be tried out, at argv[3] the number of threats. If number is not chosen the max number available on is systhem is chosen. At the last position is optionally -s, which says that the tree should be printed.
* @return The most expensive shortest path. It's given out on the shell
*/

int main(int argc, char *argv[]){
  	clock_t cpu;
  	double wall;
  	struct timeval time;
   	assert(!gettimeofday(&time,NULL));
	cpu = clock();
	int numProcessors = omp_get_max_threads();
	
	FILE *fp;
	int argcCheck = 1;
	if (argc<=argcCheck+1){ 
		exit(EXIT_FAILURE);
	}
	fp = fopen(argv[argcCheck], "r");
    	if (fp == NULL)	exit(EXIT_FAILURE);
        ++argcCheck;
        size_t* terminal = malloc(sizeof(size_t)*(1));
        size_t noOfTerminal = 0;
        terminal[0] = 1;
	
	size_t howFar = strtol(argv[argcCheck], NULL, 10);
	++argcCheck;
	if(argc <= argcCheck+1){
		if(*argv[3] =='-'){
		numProcessors = strtol(argv[argcCheck], NULL, 10);
		++argcCheck;	
	}}
	++argcCheck;
	omp_set_num_threads(numProcessors);
	
	//graph generated
	struct Graphs gra = buildGraph(fp);
	assert(gra.graphSize > 1);
	wall=(double)time.tv_sec + (double)time.tv_usec * .000001;
	size_t* isTerminal = findPrimes(gra.graphSize, numProcessors);
	printf("primes found\n");

	for(size_t i = 3; i < gra.graphSize; ++i){
		if(isTerminal[i]){
			++noOfTerminal;		
			terminal = realloc(terminal, sizeof(size_t)*(noOfTerminal+1));
			isTerminal[i] = noOfTerminal;
			terminal[noOfTerminal] = i -1;
		}

		
	}
	if(isTerminal[gra.graphSize]){
			++noOfTerminal;		
			terminal = realloc(terminal, sizeof(size_t)*(noOfTerminal+1));
			isTerminal[gra.graphSize] = noOfTerminal;
			terminal[noOfTerminal] = gra.graphSize -1;
	}
	isTerminal[2] = 0;
	++noOfTerminal;	

	printf("Terminal vectors fitted\n");

	//check of numer of start terminals is not too large
	if(noOfTerminal < howFar){
		howFar = noOfTerminal;
	}
	struct SteinerTree stein[howFar];
	struct BinaryHeap binary;
	binary.heapVal = malloc(sizeof(double)*howFar);
	binary.index66 = malloc(sizeof(size_t)*howFar);
	binary.reversedIndex = malloc(sizeof(size_t)*howFar);
	
	#pragma omp parallel for ordered schedule(runtime)
	for(size_t i = 0; i < howFar; ++i){
		stein[i] = steinerTree(terminal, noOfTerminal, i, gra, isTerminal);
		binary.heapVal[i] = stein[i].value;
		binary.index66[i] = i;
		binary.reversedIndex[i] = i;
	}
	binary = buildMinHeap(binary, howFar);
	double minValue = stein[binary.index66[0]].value;

	printf("TLEN: %f\n", minValue);
	cpu = clock() - cpu;
	printf("TIME: %fs\n",((float)cpu)/CLOCKS_PER_SEC);
	//checks if the given out tree is even a steiner tree
	char decider = solChecker(stein[binary.index66[0]], gra, terminal, noOfTerminal);
	if(decider){
		if(*argv[argc-1] == '-'){
			if(*(argv[argc-1]+1)=='s'){
				printf("Tree: ");
				#pragma omp for 
				for(size_t i = 0; i< stein[binary.index66[0]].noOfEdges-1; ++i){
					printf("(%ld,%ld)", stein[binary.index66[0]].tree[2*i]+1, stein[binary.index66[0]].tree[2*i+1]+1);
				}
			}	
		}
	}
	else{
		printf("Something went wrong. Solution is not a Steiner tree!");
	}


        assert(!gettimeofday(&time,NULL));
	wall=(double)time.tv_sec + (double)time.tv_usec * .000001 -wall;

        printf("\nWALL: %fs\n", wall);
}

