/** @file steiner_modified.c
 * @author Franziska Schlösser
 * @date July 2017
 * @brief Main programm fulfilling ex9
 */

#include <assert.h>
#include <limits.h>
#include <time.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
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

    // ############### init from file
    Graph *g = malloc(sizeof(Graph));
    init_from_graph_file(g, argv[1]);

    // ############### find lengths of shortest paths to destination
    // steiner tree connecting prime nodes
    unsigned int *mask = get_primes(g->n_verts);

    unsigned int **vertex_mask = malloc(sizeof(*vertex_mask) * g->n_verts);
    unsigned int **prev = malloc(sizeof(*prev) * g->n_verts);           

	# pragma omp parallel for 
	for(int i = 0; i < g->n_verts; i++) { // find the best result
		if (mask[i]==0) {
			continue;
		}
        
        // prepare the vertex masks
        vertex_mask[i] = malloc(sizeof(*vertex_mask[i]) * g->n_verts);
        memcpy(vertex_mask[i], mask, sizeof(*vertex_mask[i]) * g->n_verts);
        prev[i] = steiner_modified(g, i+1, vertex_mask[i]);
	}

    // ############### find and print result
	unsigned int min = 0;
    unsigned long minobjective = ULONG_MAX;
	for(int i = 0; i < g->n_verts; i++) {
        if (mask[i]==0) {
            continue;
        }
		// calculate the objective
        unsigned long objective = weight_of_tree(g, vertex_mask[i], prev[i]); 
		if (objective < minobjective) {
            minobjective = objective;
			min = i;	
		} 
	}
	// check result
	unsigned int *terminal_mask = malloc(sizeof(*terminal_mask) * g->n_verts);
	memcpy(terminal_mask, mask, sizeof(*terminal_mask) * g->n_verts);
    assert(check_steiner(g, terminal_mask, vertex_mask[min], prev[min]));
    free(terminal_mask);
	
    // print result
    printf("MINIMAL OBJECTIVE: %li\n", minobjective);


    // ############### free memory 
	for(int i = 0; i < g->n_verts; i++) {
        if (mask[i]!=0) {
		    free(prev[i]);
    	    free(vertex_mask[i]);
        }
	}
    free(prev);
    free(vertex_mask);
    free(g);
	free(mask);

    return 0; // everything went fine (hopefully)
}

