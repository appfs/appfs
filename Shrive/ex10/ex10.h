/**
	@file
	@brief  Definitions for APPFS ex10
	@author Tri-Peter Shrive
*/

#ifndef EX10_H
#define EX10_H

#define EXT_SIZE	(1024*1024)
#define MAX_LINE_LEN	512
#define error_exit(msg)	error_exit_fun(msg, __FILE__, __LINE__)

#define INTEGER		unsigned long long int
#define INTEGER_MAX	LLONG_MAX

/**
	the graphs attributes
 */
struct graph
{
	unsigned int number_of_terminals;
	unsigned int number_of_nodes;	/**< number of vertices in the graph */
	unsigned int number_of_edges;	/**< number of edges in the graph */

	unsigned int count;	/**< number of terminals added to subtree */
	INTEGER sum;	/**< sum of weights in subtree */

	unsigned int* number_of_neighbours;	/**< number of neighbours for each vertex */
	unsigned int* index_of_first_neighbour;	/**< index of first neighbour for each vertex */
	unsigned int* sorted_heads;		/**< heads of each edge sorded by vertex */
	INTEGER* sorted_weights;		/**< weights of each edge sorded by vertex */

	unsigned int* tail;		/**< the tail corresponding to each edge */
	unsigned int* head;		/**< the head corresponding to each edge */
	INTEGER* edge_weight;	/**< the weight corresponding to each edge */

	unsigned int* tree_pred;		/**< the predecessor of each vertex in steiner tree */
	unsigned int* predecessor;	/**< the predecessor of each vertex in shortest path tree */
	INTEGER* distance;	/**< the distance of each vertex from source */
};

void error_exit_fun(
	const char* const msg,	/**< message to be displayed */
	const char* const file,	/**< file name */
	const unsigned int lineno	/**< line number */
	);

/**
	sifts an entry up through binary heap
 */
int sift_up(
	unsigned int* heap,		/**< nodes in heap */
	INTEGER* distance,		/**< distance of nodes in heap */
	unsigned int* index,		/**< index of nodes in heap */
	unsigned int current		/**< current position of node in heap */
	);

/**
	sifts an entry down through binary heap
 */
int sift_down(
	unsigned int* heap,		/**< nodes in heap */
	INTEGER* distance,	/**< distance of nodes in heap */
	unsigned int* index,		/**< index of nodes in heap */
	unsigned int current,		/**< current position of node in heap */
	const int size		/**< size of heap */
	);

#ifdef NDEBUG
/**
	assesses validity of heap index
 */
int is_heap_index_valid(
	unsigned int* heap,		/**< nodes in heap */
	unsigned int* index,		/**< index of nodes in heap */
	const unsigned int size		/**< size of heap */
	);
#endif

/**
	calculates steiner tree for given graph and source terminal using dijkstra's algorithm
 */
int steiner(
	struct graph* G,	/**< static graph attributes */
	struct graph* H,	/**< variable graph attributes */
	unsigned int* is_prime,		/**< array where entries are 1 when index is prime */
	unsigned int source		/**< source node */
	);

/**
	sets entry at index of prime numbers to 1
 */
int get_primes(
	unsigned int* is_prime,		/**< allocate memory for this array of size max and set the memory to zero */
	unsigned int max 		/**< size of array is_prime, the largest number to be assessed for primality */
	);

#endif
