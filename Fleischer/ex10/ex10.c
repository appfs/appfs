/**
 * @file
 * @author  Sven Fleischer
 * @version 2.0

 *
 * @section DESCRIPTION
 * compile with:  gcc -std=c11 -O3 -Wall -Wextra -Wpedantic -LDFLAGS -fopenmp  ex10.c -o ex10 -lm
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
#include<time.h>
#include<sys/time.h>
#include<omp.h>

#include"Graph.h"
#include"misc.h"


/**
* Calls the functions, which parse the graph computes and checks if the solution is a steiner tree.
*
* @param The input file. At argv[1] should be input file, at argv[2] the number of start terminals, which should be tried out, at argv[3] the number of threats. If number is not chosen the max number available on is systhem is chosen. At the last position is optionally -s, which says that the tree should be printed.
* @return The most expensive shortest path. It's given out on the shell
*/

int main(int argc, char *argv[]) {
  	clock_t cpu; //cpu time
  	double wall; //wall time of pc
  	struct timeval time;
   	assert(!gettimeofday(&time,NULL));
	cpu = clock();
	
	int numThreads = omp_get_max_threads();
	
	FILE *fp;
	
	if (argc <= 2)
	{
		exit(EXIT_FAILURE);
	}
		
	fp = fopen(argv[1], "r");
	
    	if (fp == NULL)	
    	{
    		exit(EXIT_FAILURE);
    	}
    	
        size_t* terminal = malloc(sizeof(size_t)*(1));
        size_t noOfTerminal = 0;
        terminal[0] = 1;
	
	size_t howFar = strtol(argv[2], NULL, 10);
	
	if(argc > 3) 
	{
		if(*argv[3] !='-') 
		{
			numThreads = strtol(argv[3], NULL, 10);
		}
	}
	//set num of threads
	omp_set_num_threads(numThreads);
	
	//generating graph
	struct Graphs gra = buildGraph(fp);
	assert(gra.graphSize > 1);
	wall=(double)time.tv_sec + (double)time.tv_usec * .000001;
	size_t* isTerminal = findPrimes(gra.graphSize, numThreads);
	
	printf("primes found\n");
	
	//fitting of the terminal vector
	for(size_t i = 3; i <= gra.graphSize; ++i){
		if(isTerminal[i])
		{
			++noOfTerminal;		
			terminal = realloc(terminal, sizeof(size_t)*(noOfTerminal+1));
			isTerminal[i] = noOfTerminal;
			terminal[noOfTerminal] = i -1;
		}

		
	}

	isTerminal[2] = 0;
	++noOfTerminal;	

	printf("Terminal vectors fitted\n");

	//check of numer of start terminals we should try is not too large and set it eventually to the number of terminals available
	if(noOfTerminal < howFar)
	{
		howFar = noOfTerminal;
	}
	
	
	struct SteinerTree stein[howFar];//steiner tree array


	
	//steiner tree for the first terminal, and set of the termination bound, which is the currently cheapest steiner tree
	stein[0] = getSteinerTree(terminal, noOfTerminal, 0, gra, isTerminal, INFINITY);
	double bestStein = stein[0].value;
	size_t temp = 0;
	
	//for the others parallelized
	#pragma omp parallel for schedule(runtime)
	for(size_t i = 1; i < howFar; ++i)
	{
		stein[i] = getSteinerTree(terminal, noOfTerminal, i, gra, isTerminal, bestStein);
		//check if the current one, delete the previous cheapest one
		if(bestStein > stein[i].value)
		{
			bestStein = stein[i].value;
			free(stein[temp].tree);
			temp = i;	
		}
		
	}


	
        //calculation of the final wall time
        assert(!gettimeofday(&time,NULL));	
	wall=(double)time.tv_sec + (double)time.tv_usec * .000001 -wall;
	printf("TLEN: %f\n", stein[temp].value);



	//checks if the given out tree is even a steiner tree
	char decider = solChecker(stein[temp], gra, terminal, noOfTerminal, numThreads);
	if(decider){
		//check if it is wanted, that the tree should be printed, and print the tree
		if(*argv[argc-1] == '-'){
			if(*(argv[argc-1]+1)=='s'){
				printf("Tree: ");
				for(size_t i = 0; i< gra.graphSize; ++i){
					if(stein[temp].tree[i] != -1)
						printf("(%lu,%lu)", stein[temp].tree[i]+1, i+1);
				}
				printf("\n");
			}	
		}
	}
	else
	{
		printf("Something went wrong. Solution is not a Steiner tree!");
	}



	//calculation of the final cpu time
	cpu = clock() - cpu;
	printf("TIME: %fs\n",((float)cpu)/CLOCKS_PER_SEC);
        printf("WALL: %fs\n", wall);
}
