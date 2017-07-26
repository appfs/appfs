/** @file longest_shortest.c
 * @author Franziska Schlösser
 * @date July 2017
 * @brief Main programm fulfilling ex5, 6 and 7
 */

#include <time.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include "graph.h"

/** @brief Solve Exercise 5
 *
 * Reads in a file containing a graph and calulates the longest shortest path.
 */
int main(int argc, char **argv) {
    if (argc != 2) {
        printf("Please provide filename as first and only argument.");
        exit(0);
    }

    // ############### init from file
    Graph *g = malloc(sizeof(Graph));
    init_from_graph_file(g, argv[1]);
    unsigned int destination = 1;

    // ############### find lengths of shortest paths to destination
    unsigned int *prev = malloc(sizeof(*prev) * g->n_verts);
    // measure time
    clock_t start = clock();
    unsigned long *distances = shortest_distances_to(g, destination, prev);
    clock_t end = clock();
    float seconds = (float)(end - start) / CLOCKS_PER_SEC;

    // ############### free memory of graph
    free_graph(g);

    // ############### find longest amongst shortests
    unsigned long *res = find_longest(distances, g->n_verts);

    // ############### print result
    printf("RESULT VERTEX %li\nRESULT DIST %li\nRESULT TIME %.12f\n", res[1]+1, res[0], seconds);

    // ############### free memory
    free(g);
    free(prev);
    free(distances);
    free(res);
    return 0; // everything went fine (hopefully)
}

