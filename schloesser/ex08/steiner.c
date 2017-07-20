/** @file steiner.c
 * @author Franziska Schlösser
 * @date July 2017
 * @brief Main programm fulfilling ex8
 */

#include <assert.h>
#include <time.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "../library/graph.h"
#include "../library/misc.h"

/** @brief Solves Exercise 8
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
    // measure time
    clock_t start = clock();

    // steiner tree connecting prime nodes
    unsigned int *vertex_mask = get_primes(g->n_verts);
	unsigned int *terminal_mask = malloc(sizeof(*terminal_mask) * g->n_verts);
    memcpy(terminal_mask, vertex_mask, sizeof(*terminal_mask) * g->n_verts);

    unsigned int *prev = steiner(g, 2, vertex_mask);

    // stop clock
    clock_t end = clock();
    float seconds = (float)(end - start) / CLOCKS_PER_SEC;
    
	// check result
	assert(check_steiner(g, terminal_mask, vertex_mask, prev)); 

    // calculate objective
	unsigned long objective = weight_of_tree(g, vertex_mask, prev);

    // ############### free memory
    free_graph(g);
    free(g);
    free(prev);
    free(vertex_mask);
    free(terminal_mask);

    // ############### print result
    printf("RESULT OBJECTIVE %li\n", objective);
    printf("RESULT TIME %f\n", seconds);

    return 0; // everything went fine (hopefully)
}

