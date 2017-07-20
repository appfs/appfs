/** @file steiner_modified_final.c
 * @author Franziska Schlösser
 * @date July 2017
 * @brief Main programm fulfilling ex10
 */

#include <argp.h>
#include <assert.h>
#include <limits.h>
#include <omp.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "../library/graph.h"
#include "../library/misc.h"

/** @brief Solves Exercise 10
 *
 * Reads in a graph from a file and calculates an approximation to a steiner tree on the prime nodes
 */
int main(int argc, char **argv) {

    // ############### parse commandlineargs
    char *help = "Please provide filename as first argument, number of starting points a positive integer as optional second argument and '-s' as parameter indicating that the best approximation to the steiner tree should be printed.\n";
    if (argc < 2) {
        printf("%s", help);
        exit(0);
    }
    // filename is first commandlinearg
    char *file = argv[1];
    // number of starting points is second commandlinearg
    unsigned int primebound = 100;
    if (argc == 3) {
        primebound = atoi(argv[2]);
        if (primebound < 1) {
            printf("%s", help);
        }
    }
    bool verbose = false;
    if (argc == 4) {
        if (strcmp(argv[3], "-s") == 0) {
            verbose = true;
        } else {
            printf("Can't parse argument '%s'", argv[3]);
            printf("%s", help);
            exit(0);
        }
    }
    

    // ############### init from file
    Graph *g = malloc(sizeof(Graph));
    init_from_graph_file(g, file);


    // ############### find upper bound on startvertices
    // steiner tree connecting prime nodes
    unsigned int *mask = get_primes(g->n_verts);

    unsigned int walker = 0;
    for(int i = 0; i < g->n_verts; i++) { // find the bound up to the n'th prime
        if (mask[i]==1) {
            walker++;
        }
        if (walker == primebound || i == g->n_verts-1) {
            walker = i;
            break;
        }
    }
    // walker should now be the n'th prime or index of last vertex of g
    unsigned int upperbound = walker+1;

    
    // ############### start steiner search 
    // start the clocks
    clock_t cpustart = clock();
    struct timespec wallstart, wallend;
    clock_gettime( CLOCK_REALTIME, &wallstart);

	unsigned int **vertex_mask = malloc(sizeof(*vertex_mask) * g->n_verts);
	unsigned int **prev = malloc(sizeof(*prev) * g->n_verts);

	# pragma omp parallel for 
    for(int i = 0; i < upperbound; i++) { // search in parallel for startterminals smaller than n'th prime
		if (mask[i]==0) { // only search with terminals as start vertices
			continue;
		}

        // prepare the vertex masks
	    vertex_mask[i] = malloc(sizeof(*vertex_mask[i]) * g->n_verts);
		memcpy(vertex_mask[i], mask, sizeof(*vertex_mask[i]) * g->n_verts);
		prev[i] = steiner_modified(g, i+1, vertex_mask[i]);
	}
    // stop the clocks
    clock_t cpuend = clock();
    clock_gettime(CLOCK_REALTIME, &wallend);


    // ############### find and print result
	unsigned int min = 0;
    unsigned long minobjective = ULONG_MAX;
	for(int i = 0; i < upperbound; i++) {
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
    printf("TLEN: %li\n", minobjective);
    if (verbose) {
        printf("TREE: ");
        print_tree(g, vertex_mask[min], prev[min]);
    }
    printf("TIME: %f\n", (float) (cpuend-cpustart) / CLOCKS_PER_SEC);
    double secs = (wallend.tv_sec - wallstart.tv_sec); 
    double ns = (wallend.tv_nsec - wallstart.tv_nsec)/1000000000.f;
    printf("WALL: %f\n", secs + ns);
	   

    // ############### free memory 
	for(int i = 0; i < upperbound; i++) {
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

