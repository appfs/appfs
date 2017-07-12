/** @file steiner.c
 * @author Franziska Schlösser
 * @date July 2017
 * @brief Main programm fulfilling ex8
 */

#include <limits.h>
#include <time.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <omp.h>
#include "graph.h"
#include "misc.h"

/** @brief Solves Exercise 9
 *
 * Reads in a graph from a file and calculates an approximation to a steiner tree on the prime nodes
 */
int main(int argc, char **argv) {
    if (argc != 2) {
        printf("Please provide filename as first and only argument.");
        exit(0);
    }

// ##### init from file
    // filename is first commandlinearg
    char *file = argv[1];

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

	# pragma omp parallel for 
	for(int i = 0; i < g->n_verts; i++) { // find the best result
		erg[i] = ULONG_MAX;
		if (mask[i]==0) {
			continue;
		}
	    unsigned int *vertex_mask = malloc(sizeof(*vertex_mask) * g->n_verts);
		memcpy(vertex_mask, mask, sizeof(*vertex_mask) * g->n_verts);
	    unsigned int *terminal_mask = malloc(sizeof(*vertex_mask) * g->n_verts);
		memcpy(terminal_mask, mask, sizeof(*vertex_mask) * g->n_verts);

		unsigned int *prev = steiner_modified(g, i+1, vertex_mask);
    
		// check result
		/*if (check_steiner(g, terminal_mask, vertex_mask, prev)) {
			printf("Check succeeded. (start node %d)\n", i+1);
		} else {
			printf("Check failed.\n");
		}*/
	   
		// calculate objective
		signed long objective = weight_of_tree(g, vertex_mask, prev);
		erg[i] = objective;
		//printf("OBJECTIVE %li (start node %d)\n", objective, i+1);

		free(prev);
	    free(vertex_mask);
	    free(terminal_mask);
	}

	unsigned long min = ULONG_MAX;
	for(int i = 0; i < g->n_verts; i++) {
		if(erg[i] < min) {
			min = erg[i];	
		}
	}
	printf("MINIMAL OBJECTIVE VALUE %li\n", min);

// ##### free memory of graph
    free_graph(g);
    free(g);
	free(mask);

    return 0; // everything went fine (hopefully)
}

