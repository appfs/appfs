/** @file steiner.c
 * @author Franziska Schlösser
 * @date July 2017
 * @brief Main programm fulfilling ex8
 */

#include <time.h>
#include <stdlib.h>
#include <stdio.h>
#include "graph.h"
#include "misc.h"

/** @brief Solves Exercise 8
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
    // measure time
    clock_t start = clock();

    // steiner tree connecting prime nodes
    unsigned int *vertex_mask = get_primes(g->n_verts);
    unsigned int *prev = steiner(g, vertex_mask);
    clock_t end = clock();
    float seconds = (float)(end - start) / CLOCKS_PER_SEC;
    
	// check result
	unsigned int *terminal_mask = get_primes(g->n_verts);
	if (check_steiner(g, terminal_mask, vertex_mask, prev)) {
		printf("Check succeeded.\n");
	} else {
		printf("Check failed.\n");
	}
   
    // calculate objective
	signed long objective = weight_of_tree(g, vertex_mask, prev);

// ##### free memory of graph
    free_graph(g);
    free(g);
    free(prev);

    free(vertex_mask);
    free(terminal_mask);
    printf("\n");
    printf("RESULT OBJECTIVE %li\n", objective);
    printf("RESULT TIME %f\n", seconds);
    return 0; // everything went fine (hopefully)
}

