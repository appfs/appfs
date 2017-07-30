/**
	@file
	@brief Definitions for APPFS ex10
	@author Tri-Peter Shrive
*/

#ifndef EX10_H
#define EX10_H

#include <assert.h>
#include <ctype.h>
#include <limits.h>
#include <omp.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>
#include <time.h>

#define EXTENSION_LENGTH (1024*1024)
#define MAX_LINE_LENGTH 512

/**
	sets entry at index of prime numbers to 1
*/
int get_primes(
	unsigned int* is_prime,	/**< pointer to array of size ceiling, with memory set to zero */
	unsigned int ceiling	/**< only numbers below this value will be assessed for primality */
	);

/**
	sifts entry up in binary heap
*/
int sift_up(
	unsigned int* heap,	/**< heap entries */
	unsigned int* heap_index,	/**< nodes index in heap */
	const unsigned long long int* const distance,	/**< nodes distance from source */
	unsigned int current	/**< node to be sifted */
);


/**
	sifts entry down in binary heap
*/
int sift_down(
	unsigned int* heap,	/**< heap entries */
	unsigned int* heap_index,	/**< nodes index in heap */
	const unsigned long long int* const distance,	/**< nodes distance from source */
	unsigned int current,	/**< node to be sifted */
	const unsigned int heap_size	/**< size of heap */
	);

/**
	calculates steiner tree using dijkstra algorithm and additional heuristic
*/
int get_steiner_tree(
	const unsigned int* const is_prime,	/**< if index is prime entry is one else zero */
	const unsigned int* const terminals,	/**< terminal nodes */
	const unsigned int num_terminals, 	/**< number of terminal nodes */
	const unsigned int num_nodes, 	/**< number of nodes */
	const unsigned int num_edges,	/**< number of edges */
	const unsigned int* const first_neighbours_index,	/**< index of tails first neighbour in sorted arrays */
	const unsigned int* const num_neighbours,	/**< number of neighbours for each node */
	const unsigned long long int* const sorted_weights,	/**< array of weights sorted by tail */
	const unsigned int* const sorted_heads, 	/**< array of heads sorted by tail */
	unsigned int* temp_tree,	/**< entries will be set to 1 if sorted lists enties are part of steiner tree else 0*/
	unsigned int* predecessor,	/**< nodes predecessor in steiner tree */
	const unsigned int source 	/**< source terminal */
	);

/**
	verifies steiner tree
	@returns 1 if steiner tree is connected else 0
*/
int is_tree_valid(
	const unsigned int* const predecessor,	/**< each nodes predecessor in steiner tree*/
	const unsigned int* const terminal,	/**< terminal nodes */
	unsigned int* end,	/**< an empty array to hold each terminals source */
	const unsigned int num_nodes,	/**< number of nodes */
	const unsigned int num_terminals,	/**< number of terminals */
	const unsigned int source	/**< the source terminal */
	);

#ifndef NDEBUG
/**
	verifies heap index
	@returns 1 if heap index is valid else 0
*/
int is_heap_index_valid(
	const unsigned int* const heap,	/**< heap entries */
	const unsigned int* const heap_index,	/**< nodes index in heap */
	const unsigned int heap_size	/**< size of heap */
	);

/**
	verifies heap order
	@returns 1 if heap order is valid else 0
*/
int is_heap_order_valid(
	const unsigned int* const heap,	/**< heap entries */
	const unsigned long long int* const distance,	/**< nodes distance from source */
	const unsigned int heap_size	/**< size of heap */
	);

#endif

#endif
