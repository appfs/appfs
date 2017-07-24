/**
	@file
	@brief  Definitions for APPFS ex10
	@author Tri-Peter Shrive
*/

#ifndef EX10_H
#define EX10_H

/**
	the graphs attributes
 */
struct graph
{
	long long int number_of_nodes;	/**< number of vertices in the graph */
	long long int number_of_edges;	/**< number of edges in the graph */

	long long int count;	/**< number of terminals added to subtree */
	long long int sum;	/**< sum of weights in subtree */

	long long int* number_of_neighbours;	/**< number of neighbours for each vertex */
	long long int* index_of_first_neighbour;	/**< index of first neighbour for each vertex */
	long long int* sorted_heads;		/**< heads of each edge sorded by vertex */
	long long int* sorted_weights;		/**< weights of each edge sorded by vertex */

	long long int* tail;		/**< the tail corresponding to each edge */
	long long int* head;		/**< the head corresponding to each edge */
	long long int* edge_weight;	/**< the weight corresponding to each edge */

	long long int* tree_pred;		/**< the predecessor of each vertex in steiner tree */
	long long int* predecessor;	/**< the predecessor of each vertex in shortest path tree */
	long long int* distance;	/**< the distance of each vertex from source */
};

#ifdef NDEBUG
/**
	assesses validity of heap index
 */
long long int is_heap_index_valid(
	long long int* heap,		/**< nodes in heap */
	long long int* index,		/**< index of nodes in heap */
	const long long int size		/**< size of heap */
	);
#endif

void error_exit_fun(
	const char* const msg,	/**< message to be displayed */
	const char* const file,	/**< file name */
	const long long int lineno	/**< line number */
	);

/**
	sifts an entry up through binary heap
 */
int sift_up(
	long long int* heap,		/**< nodes in heap */
	long long int* distance,		/**< distance of nodes in heap */
	long long int* index,		/**< index of nodes in heap */
	long long int current		/**< current position of node in heap */
	);

/**
	sifts an entry down through binary heap
 */
int sift_down(
	long long int* heap,		/**< nodes in heap */
	long long int* distance,		/**< distance of nodes in heap */
	long long int* index,		/**< index of nodes in heap */
	long long int current,		/**< current position of node in heap */
	const long long int size		/**< size of heap */
	);

/**
	calculates steiner tree for given graph and source terminal using dijkstra's algorithm
 */
int steiner(
	struct graph* G,	/**< static graph attributes */
	struct graph* H,	/**< variable graph attributes */
	long long int* is_prime,		/**< array where entries are 1 when index is prime */
	long long int source		/**< source node */
	);

/**
	sets entry at index of prime numbers to 1
 */
long long int get_primes(
	long long int* is_prime,		/**< allocate memory for this array of size max and set the memory to zero */
	long long int max 		/**< size of array is_prime, the largest number to be assessed for primality */
	);

#endif
