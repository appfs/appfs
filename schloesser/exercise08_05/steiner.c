/**
 * @file main.c
 * @author Franziska Schlösser
 * @date July 2017
 * @brief Main programm fulfilling ex8
 */

#include <time.h>
#include <math.h>
#include "graph.h"

/** @brief Returns a list of primes
 *
 * @param upper_bound upper bound, not included 
 * @return sieve number of primes found
 */
unsigned int* get_primes(unsigned int upper_bound) {
    assert(upper_bound > 1);
    unsigned int *sieve = malloc(sizeof(*sieve) * upper_bound);
    for(int i = 0; i < upper_bound; i++) {
        sieve[i] = 1;
    }
    sieve[0] = 0; // 1 is not prime
    unsigned int bound = sqrt(upper_bound);
    for(int i = 2; i <= bound; i++) {
        if(sieve[i-1]==1) { // if i is prime
            // multiples of i are not prime
            unsigned int j = 2*i;
            while(j <= upper_bound) {
                sieve[j-1] = 0;
                j += i;
            }
        }
    }
    return sieve;
}

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
    unsigned int *vertex_mask = get_primes(g->n_verts);
    // steiner tree connecting prime nodes
    unsigned int *prev = steiner(g, vertex_mask);
    for (int i = 0; i < g->n_verts; i++) {
        printf("vertex %d, mask %d, predecessor %d\n", i+1, vertex_mask[i]/2, prev[i]+1);
    }
    clock_t end = clock();
    float seconds = (float)(end - start) / CLOCKS_PER_SEC;

// ##### free memory of graph
    free_graph(g);
    free(g);
    free(prev);

    free(vertex_mask);
    printf("RESULT TIME %f\n", seconds);
    return 0; // everything went fine (hopefully)
}

