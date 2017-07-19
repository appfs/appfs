/** @file steiner_modified_final.c
 * @author Franziska Schlösser
 * @date July 2017
 * @brief Main programm fulfilling ex9
 */

#include <limits.h>
#include <time.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <omp.h>
#include <assert.h>
#include "graph.h"
#include "misc.h"

/** @brief Solves Exercise 9
 *
 * Reads in a graph from a file and calculates an approximation to a steiner tree on the prime nodes
 */
int main(int argc, char **argv) {
    if (argc != 3) {
        printf("Please provide filename as first and number of starting points as second argument.");
        exit(0);
    }

// ##### init from file
    // filename is first commandlinearg
    char *file = argv[1];
    // number of starting points is second commandlinearg
    unsigned int primebound = atoi(argv[2]);
    printf("primebound %d\n", primebound);
    Graph *g = malloc(sizeof(Graph));

    unsigned int **edges;
    unsigned int n_edges = read_graph_file(g, file, &edges);

    //float density = ((float)n_edges)/(n_verts/2 * n_verts);
    //bool sparse = (density < 0.1 ? true : false);

    // sort edges to neighbors
    fill_neighbors(g, edges, n_edges); // for each vertex this holds a list of neighbors with weights
    free(edges);

// ##### find lengths of shortest paths to destination
    // steiner tree connecting prime nodes
    unsigned int *mask = get_primes(g->n_verts);
	unsigned long *erg = malloc(sizeof(*erg) * g->n_verts);

    unsigned int walker = 0;
    for(int i = 0; i < g->n_verts; i++) { // find the n-th prime
        if (mask[i]==1) {
            walker++;
        }
        if (walker == primebound || i == g->n_verts-1) {
            walker = i;
            break;
        }
    }
    // walker should now be the n'th prime
    unsigned int upperbound = (g->n_verts < walker+1) ? g->n_verts : walker+1;
    printf("the %d'th prime is %d or bigger\n", primebound, upperbound);

    clock_t cpustart = clock();
    struct timespec wallstart, wallend;
    clock_gettime( CLOCK_REALTIME, &wallstart);
	# pragma omp parallel for 
	for(int i = 0; i < upperbound; i++) { // find the best result
		erg[i] = ULONG_MAX;
		if (mask[i]==0) {
			continue;
		}
	    unsigned int *vertex_mask = malloc(sizeof(*vertex_mask) * g->n_verts);
		memcpy(vertex_mask, mask, sizeof(*vertex_mask) * g->n_verts);
	    unsigned int *terminal_mask = malloc(sizeof(*vertex_mask) * g->n_verts);
		memcpy(terminal_mask, mask, sizeof(*vertex_mask) * g->n_verts);

		unsigned int *prev = steiner_modified(g, i+1, vertex_mask);
    
		// calculate objective
		signed long objective = weight_of_tree(g, vertex_mask, prev);
		erg[i] = objective;
		//printf("OBJECTIVE %li (start node %d)\n", objective, i+1);

		free(prev);
	    free(vertex_mask);
	    free(terminal_mask);
	}
    clock_t cpuend = clock();
    clock_gettime(CLOCK_REALTIME, &wallend);

	unsigned long min = ULONG_MAX;
	for(int i = 0; i < g->n_verts; i++) {
		if(erg[i] < min) {
			min = erg[i];	
		}
	}
	printf("TLEN %li\n", min);
    printf("TREE\n");
    printf("TIME %f\n", (float) (cpuend-cpustart) / CLOCKS_PER_SEC);
    double secs = (wallend.tv_sec - wallstart.tv_sec); 
    double ns = (wallend.tv_nsec - wallstart.tv_nsec)/1000000000.f;
    printf("WALL %f\n", secs + ns);
	// check result
	//assert(check_steiner(g, terminal_mask, vertex_mask, prev));
	   

// ##### free memory of graph
    free_graph(g);
    free(g);
	free(mask);

    return 0; // everything went fine (hopefully)
}

