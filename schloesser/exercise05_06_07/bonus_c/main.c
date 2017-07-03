/**
 * @file main.c
 * @author Franziska Schlösser
 * @date July 2017
 * @brief Main programm fulfilling ex5, 6 and 7
 */

#include <time.h>
#include "graph.h"

int main(int argc, char **argv) {
    if (argc != 2) {
        printf("Please provide filename containing as first and only argument.");
        exit(0);
    }

// ##### init from file

    // filename is first commandlinearg
    char *file = argv[1];
    // destination is source node
    signed int destination = 1;

    signed int *n_neighbors;
    signed int **edges;

    signed int n_verts = 0;
    signed int n_edges = read_graph_file(file, &n_neighbors, &edges, &n_verts);

    //float density = ((float)n_edges)/(n_verts/2 * n_verts);
    //bool sparse = (density < 0.1 ? true : false);

    // sort edges to neighbors
    signed int **neighbors = fill_neighbors(n_verts, n_edges, n_neighbors, edges); // for each vertex this holds a list of neighbors with weights
    free(edges);

// ##### find lengths of shortest paths to destination
    // measure time
    clock_t start = clock();
    signed long *distances = shortest_distances_to(destination, n_verts, neighbors, n_neighbors); 
    clock_t end = clock();
    float seconds = (float)(end - start) / CLOCKS_PER_SEC;

// ##### free memory of graph
    for (int i = 0; i < n_verts; i++) {
        free(neighbors[i]);
    }
    free(neighbors);
    free(n_neighbors);

// ##### find longest amongst shortests
    signed long *res = find_longest(distances, n_verts);

    free(distances);

    // TODO timething
    printf("RESULT VERTEX %li\nRESULT DIST %li\nRESULT TIME %.12f\n", res[1]+1, res[0], seconds);
    free(res);
    return 0; // everything went fine (hopefully)
}

