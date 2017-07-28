#include <assert.h>
#include <ctype.h>
#include <limits.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>
#include <time.h>

#define EXT_SIZE     (1024*1024)
#define MAX_LINE_LEN 512

#define error_exit(msg)  error_exit_fun(msg, __FILE__, __LINE__)

/**
	\file
	\brief  A program to calculate the longest shortest path of a graph using Dijkstra's algorithm
	\author Tri-Peter Shrive
 */

/**
	the graphs attributes
 */
struct graph
{
	int number_of_nodes;	/**< number of vertices in the graph */
	int number_of_edges;	/**< number of edges in the graph */

	int* number_of_neighbours;	/**< number of neighbours for each tail */
	int* index_of_first_neighbour;	/**< index of first neighbour for each tail */
	int* sorted_heads;		/**< heads of each edge sorded by tail */
	int* sorted_weights;		/**< weights of each edge sorded by tail */

	int* tail;		/**< the tail corresponding to each edge */
	int* head;		/**< the head corresponding to each edge */
	int* edge_weight;	/**< the weight corresponding to each edge */

	int* predecessor;	/**< the predecessor of each vertex in shortest path tree */
	int* distance;	/**< the distance of each vertex from source */
};

void error_exit_fun(
	const char* const msg,	/**< message to be displayed */
	const char* const file,	/**< file name */
	const int lineno	/**< line number */
	);

int sift_up(
	int* heap,		/**< nodes in heap */
	int* distance,		/**< distance of nodes in heap */
	int* index,		/**< index of nodes in heap */
	int current		/**< current position of node in heap */
	);


int sift_down(
	int* heap,		/**< nodes in heap */
	int* distance,		/**< distance of nodes in heap */
	int* index,		/**< index of nodes in heap */
	int current,		/**< current position of node in heap */
	const int size		/**< size of heap */
	);

/**
	utility function that simplifies error handling
 */
void error_exit_fun(
	const char* const msg,	/**< message to be displayed */
	const char* const file,	/**< file name */
	const int lineno	/**< line number */
	)
{
	assert(NULL != msg);
	assert(NULL != file);

	fprintf(stderr, "%s(%d) ", file, lineno);
	perror(msg);
	exit(EXIT_FAILURE);
}

int dijkstra(
	struct graph* G,	/**< graph attributes */
	int source		/**< source node */
	);

/**
	sifts an entry up through binary heap
 */
int sift_up(
	int* heap,		/**< nodes in heap */
	int* distance,		/**< distance of nodes in heap */
	int* index,		/**< index of nodes in heap */
	int current		/**< current position of node in heap */
	)
{
	// store child node in temp variable
	int temp_a = heap[current];
	// store distance to child node in temp variable
	int dist = distance[temp_a];
	int parent;
	int temp_b;

	while ( 0 < current )
	{
		// calculate parents location in heap array
		parent = (current - 1) / 2;
		assert(0 <= parent);
		// store parent in temp variable 
		temp_b = heap[parent];

		// is the distance to parent node greater than the distance to child?
		// if not break out of loop
		if ( distance[temp_b] <= dist )
			break;

		// replace child node with parent node
		heap[current] = temp_b;
		// update parent nodes heap index
		index[temp_b] = current;
		// parent becomes next child
		current = parent;

	}
	// replace child with first child
	heap[current] = temp_a;
	// update child nodes heap index
	index[temp_a] = current;
	return 0;
}

/**
	sifts an entry down through binary heap
 */
int sift_down(
	int* heap,		/**< nodes in heap */
	int* distance,		/**< distance of nodes in heap */
	int* index,		/**< index of nodes in heap */
	int current,		/**< current position of node in heap */
	const int size		/**< size of heap */
	)
{
	int dist;
	int child;
	int temp_a;
	int temp_b;

	// calculate location of child in heap array
	child = current + current + 1;
	// store parent node as temporary variable
	temp_a = heap[current];
	// store distance of current node as temporary variable
	dist = distance[temp_a];

	// while node has a child
	while ( child <= size )
	{
		// store first child node as temporary variable
		temp_b = heap[child];
		// does a second child exist?
		if ( child + 1 <= size )
		{
			// is the distance to the second child less than that of the first?
			if ( distance[heap[child + 1]] < distance[temp_b] )
			{
				child++;
				// replace first child node with second child node
				temp_b = heap[child];
			}
		}
		// is the distance to child node less than the distance to parent?
		// if not break out of loop
		if ( distance[temp_b] >= dist )
			break;

		// replace parent node with child node
		heap[current] = temp_b;
		// update child nodes heap index
		index[temp_b] = current;
		// child becomes parent
		current = child;
		// calculate new child
		child = child + child + 1;
	}
	// replace current parent with first parent
	heap[current] = temp_a;
	// update the parents heap index
	index[temp_a] = current;
	return 0;
}

/**
	implementation of the dijkstra algorithm
 */
int dijkstra(
	struct graph* G,	/**< graph attributes */
	int source		/**< source node */
	)
{
	assert(G);
	assert(G->predecessor);
	assert(G->distance);
	assert(G->sorted_heads);
	assert(G->sorted_weights);
	assert(G->number_of_neighbours);
	assert(G->index_of_first_neighbour);

	// create set containing all vetrices
	// datastructure used is a binary heap
	// index 0 is highest priority
	// if node not on heap it has lowest priority
	int* heap = NULL;
	int* index_on_heap = NULL;
	heap = malloc(G->number_of_nodes * sizeof(int));
	index_on_heap = malloc(G->number_of_nodes * sizeof(int));
	for (int i = 0; i < G->number_of_nodes; i++)
	{
		// heap entry is initially empty
		// meaning all vetrices have lowest priority
		heap[i] = -1;
		index_on_heap[i] = -1;
		// the vetrex's predecessor in shortest path tree is unknown
		G->predecessor[i] = -1;
		// the vetrex's distance from source is assumed to be infinite
		G->distance[i] = INT_MAX;
	}
	assert(source < G->number_of_nodes);
	// distance to source is zero
	G->distance[source] = 0;
	// add source to heap
	heap[0] = source;
	index_on_heap[source] = 0;
	// largest valid heap index is set to 0
	int size_of_heap = 0;
	// a count to keep track of the number of iterations
	int count = 0;

	// tempary variables for storing edge attributes 
	int tail;
	int head;
	int dist;
	int index_of_neighbour;
	int root;

	// loop while heap has entries
	while( 0 <= size_of_heap )
	{
		count++;
		// extract the vetrex with highest priority from heap
		// and setting it as current tail
		tail = heap[0];
		// replace the extracted vetrex with the vetrex of lowest priority
		root = heap[size_of_heap];
		heap[0] = root;
		index_on_heap[heap[0]] = 0;
		// decrese the largest valid heap index
		size_of_heap--;
		// sift the replacement vetrex down in the heap 
		// until it is at it's correct place in the priority queue
		assert(heap[index_on_heap[root]] == root); 
		sift_down(heap, G->distance, index_on_heap, 0, size_of_heap);
		assert(heap[index_on_heap[root]] == root);	
		

		// for each of the tail's neighbours we do the following
		for( int i = 0; i < G->number_of_neighbours[tail]; i++)
		{
			index_of_neighbour = G->index_of_first_neighbour[tail]+i;
			// set the current neighbour as the temporary head
			head = G->sorted_heads[index_of_neighbour];
			// calculate the current distance
			dist = G->distance[tail] + G->sorted_weights[index_of_neighbour];
			// can we best the current shortest path?
			if ( dist < G->distance[head] )
			{
				// update the distance to head
				G->distance[head] = dist;
				// update the heads predecessor
				G->predecessor[head] = tail;
				// is the current head already on heap?
				if( index_on_heap[head] == -1 )
				{	
					// add head at bottom of heap
					size_of_heap++;
					assert(size_of_heap < G->number_of_nodes + 1);
					heap[size_of_heap] = head;
					index_on_heap[head] = size_of_heap;
					// sift head upwards in heap
					// until at correct place in priority queue
					assert(heap[index_on_heap[head]] == head);
					sift_up(heap, G->distance, index_on_heap, size_of_heap);
					assert(heap[index_on_heap[head]] == head);
				}
				else
				{
					// decrease key
					assert(heap[index_on_heap[head]] == head);
					sift_up(heap, G->distance, index_on_heap, index_on_heap[head]);
					assert(heap[index_on_heap[head]] == head);
				}
			}					
		}		
	}
	free(heap);
	free(index_on_heap);
	return count;
}


/**
	sets entry at index of prime numbers to 1
 */
int get_primes(
	int* is_prime,		/**< allocate memory for this array of size max and set the memory to zero */
	int max 		/**< size of array is_prime, the largest number to be assessed for primality */
	)
{
	int rest;
	int * prime;

	is_prime[2] = 1;
	int count = 1;
	int last_div;
	for ( int i = 3; i <= max; i += 2 )
	{
		last_div =  (int)ceil(sqrt(i));
		for ( int j = 2; j <= last_div ; j++ )
		{
			rest = i % j;
			if ( 0 == rest )
				break;

			if ( j == last_div )
			{
				count++;
				is_prime[i] = 1;
			}
		}
	}
	return count;
}

int print_distances(struct graph** G, int* terminals, int number_of_terminals)
{
	printf("\n");
	int count;
	for( int i = 0; i < number_of_terminals; i++ )
	{
		count = 0;
		printf("\n\nTerminal: %d\n", terminals[i]);
		for( int j = 0 ; j < G[0]->number_of_nodes; j++ )
		{
			if( 0 == (count % 3) )
				printf("\n");
			printf("Distance to node[%d]: %d\t\t", j, G[i]->distance[j]);
			count++;
		}
	}
	return 0;
}

int print_predecessors(struct graph** G, int* terminals, int number_of_terminals)
{
	printf("\n");
	int count;
	for( int i = 0; i < number_of_terminals; i++ )
	{
		count = 0;
		printf("\n\nTerminal: %d\n", terminals[i]);
		for( int j = 0 ; j < G[0]->number_of_nodes; j++ )
		{
			if( 0 == (count % 3) )
				printf("\n");
			printf("Predecessor of node[%d]: %d\t", j, G[i]->predecessor[j]);
			count++;
		}
	}
	return 0;
}


/**
	repeatedly finds terminal closest to current subtree and connects it
 */

int get_tree(
	struct graph** G,
	int* terminals,
	int number_of_terminals,
	int source
	)
{
	int* tree_pred = NULL;
	tree_pred = malloc( G[0]->number_of_nodes * sizeof(int) );
	
	for (int i = 0; i < G[0]->number_of_nodes; i++)
	{
		tree_pred[i] = -1;
	}

	int* heap = NULL;
	heap = calloc( number_of_terminals, sizeof(int) );
	int* index_on_heap = NULL;
	index_on_heap = calloc( number_of_terminals, sizeof(int) );
	int* tree_dist = NULL;
	tree_dist = malloc( number_of_terminals * sizeof(int) );
	int* closest_node = NULL;
	closest_node = malloc( number_of_terminals * sizeof(int) );

	for (int i = 0; i < number_of_terminals; i++)
	{
		tree_dist[i] = INT_MAX;
		closest_node[i] = -1;
	}

	// initialise heap by adding all terminals

	int size_of_heap = -1;
	for (int i = 0; i < number_of_terminals; i++)
	{
		if( terminals[i] != terminals[source] )
		{
			size_of_heap++;
			heap[size_of_heap] = i;
			index_on_heap[i] = size_of_heap;
			tree_dist[i] = G[source]->distance[ terminals[i] ];
			closest_node[i] = terminals[source];

			sift_up( heap, tree_dist, index_on_heap, size_of_heap );
		}

	}

	int t;
	int dist;
	int term;
	int term_index;
	int root;
	int close;
	int next;
	int sum = 0;
	while( 0 <= size_of_heap )
	{
		// extract terminal with highest priority
		term_index = heap[0];
		term = terminals[term_index];
		root = heap[size_of_heap];
		heap[0] = root;
		index_on_heap[ heap[0] ] = 0;
		size_of_heap--;
		sift_down(heap, tree_dist, index_on_heap, 0, size_of_heap);

		// add terminal and its predecessors to subtree
		close = closest_node[ term_index ];
		next = G[term_index]->predecessor[close];
		sum += G[term_index]->distance[close];
		while( -1 != next )
		{			
			tree_pred[next] = close;

			// update heap entries
			for (int i = 0; i <= size_of_heap; i++)
			{
				t = heap[i] ;
				dist = G[t]->distance[ close ];

				if( dist < tree_dist[t] )
				{
					tree_dist[i] = dist;
					closest_node[t] = close;
					sift_up( heap, tree_dist, index_on_heap, index_on_heap[i] );
				}
			}
			close = next;
			next = G[term_index]->predecessor[close];
		}
	}

	free(tree_pred);
	free(heap);
	free(index_on_heap);
	free(tree_dist);
	free(closest_node);

	return sum;
}

/**
	reads data from file storing nodes and weights in graph structure. then calls dijkstra's algorithm and assesses longest shortes path
 */
int main(int argc, const char* const* const argv)
{
	struct timeval start_wall;
	assert( gettimeofday(&start_wall, NULL) == 0 );

	double start_cpu = (double)clock() / (double)CLOCKS_PER_SEC;

	if ( argc < 2 )
	{
		fprintf(stderr, "usage: %s filename\n", argv[1]);
		exit(EXIT_FAILURE);
	}

	size_t array_of_edges_next = 0;
	size_t array_of_edges_length = EXT_SIZE;

	struct graph** G = NULL;

	G = malloc( sizeof(struct graph*) );

	if ( NULL == G )
		error_exit("malloc: ");

	G[0] = malloc( sizeof(struct graph) );

	G[0]->tail = NULL;
	G[0]->head = NULL;
	G[0]->edge_weight = NULL;
	G[0]->predecessor = NULL;
	G[0]->distance = NULL;
	G[0]->sorted_heads = NULL;
	G[0]->sorted_weights = NULL;
	G[0]->number_of_neighbours = NULL;
	G[0]->index_of_first_neighbour = NULL;

	G[0]->number_of_nodes = 0;
	G[0]->number_of_edges = 0;

	G[0]->tail = malloc(array_of_edges_length * sizeof(int));
	G[0]->head = malloc(array_of_edges_length * sizeof(int));
	G[0]->edge_weight = malloc(array_of_edges_length * sizeof(int));
	if ( NULL == G[0]->tail || NULL == G[0]->head || NULL == G[0]->edge_weight )
		error_exit("malloc: ");

	// Open file for reading
	FILE* fp = fopen(argv[1], "r");

	if (NULL == fp)
		error_exit("fopen: ");

	int lineno = 0;
	char line[MAX_LINE_LEN];

	fgets(line, sizeof(line), fp);

	lineno++;

	// Remove comments and anything after newline
	char* s = strpbrk(line, "#\n");

	if (NULL != s)
		*s = '\0';

	// Skip initial spaces
	for (s = &line[0]; isspace(*s); s++);

	assert('\0' != *s);
	
	int ret = sscanf(s, "%d %d", &G[0]->number_of_nodes, &G[0]->number_of_edges);
	assert(2 == ret);

	// undirected graph
	G[0]->number_of_edges *= 2;
	// unused node 0
	G[0]->number_of_nodes++;

	G[0]->number_of_neighbours = calloc(G[0]->number_of_nodes, sizeof(int));
	G[0]->index_of_first_neighbour = calloc(G[0]->number_of_nodes, sizeof(int));
	if ( NULL == G[0]->number_of_neighbours || NULL == G[0]->index_of_first_neighbour )
		error_exit("malloc: ");
	
	while (NULL != fgets(line, sizeof(line), fp))
	{
		lineno++;

		// Remove comments and anything after newline
		char* s = strpbrk(line, "#\n");

		if (NULL != s)
			*s = '\0';

		// Skip initial spaces
		for (s = &line[0]; isspace(*s); s++)
			;

		// Skip line if empty
		if ('\0' == *s)
			continue;

		int temp_tail = 0, temp_head = 0;
		int temp_edge_weight = INT_MAX;

		ret = sscanf(s, "%d %d %d", &temp_tail, &temp_head, &temp_edge_weight);

		if (3 != ret)
		{
			fprintf(stderr, "\nLine %d: ret = %d != 3\n", lineno, ret);
			continue;
		}
		if (temp_tail < 0 || temp_head < 0)
		{
			fprintf(stderr, "\nLine %d: tail = %d, head = %d\n", lineno, temp_tail, temp_head);
			continue;
		}
		if (temp_edge_weight < 0)
		{
			fprintf(stderr, "\nLine %d, edge_weight = %d\n", lineno, temp_edge_weight); 
			continue;
		}

		// check there's enought space in the array, if not enlarge the array
		if ( array_of_edges_next == array_of_edges_length )
		{
			array_of_edges_length += EXT_SIZE;

			G[0]->tail = realloc(G[0]->tail, array_of_edges_length * sizeof(int));
			G[0]->head = realloc(G[0]->head, array_of_edges_length * sizeof(int));
			G[0]->edge_weight = realloc(G[0]->edge_weight, array_of_edges_length * sizeof(int));
			if (NULL == G[0]->tail || NULL == G[0]->head || NULL == G[0]->edge_weight)
				error_exit("realloc: ");
		}

		assert( G[0]->number_of_nodes > temp_tail );
		assert( G[0]->number_of_nodes > temp_head );

		// store the edge in memory
		G[0]->tail[array_of_edges_next] = temp_tail;
		G[0]->head[array_of_edges_next] = temp_head;
		G[0]->edge_weight[array_of_edges_next] = temp_edge_weight;

		G[0]->number_of_neighbours[temp_tail]++;
	
		array_of_edges_next++;

		// repeat with head and tail inverted for undirected graph
		if ( array_of_edges_next == array_of_edges_length )
		{
			array_of_edges_length += EXT_SIZE;

			G[0]->tail = realloc(G[0]->tail, array_of_edges_length * sizeof(int));
			G[0]->head = realloc(G[0]->head, array_of_edges_length * sizeof(int));
			G[0]->edge_weight = realloc(G[0]->edge_weight, array_of_edges_length * sizeof(int));
			if (NULL == G[0]->tail || NULL == G[0]->head || NULL == G[0]->edge_weight)
				error_exit("realloc: ");
		}

		assert( G[0]->number_of_nodes > temp_tail );
		assert( G[0]->number_of_nodes > temp_head );

		// store reversed edge in memory
		G[0]->tail[array_of_edges_next] = temp_head;
		G[0]->head[array_of_edges_next] = temp_tail;
		G[0]->edge_weight[array_of_edges_next] = temp_edge_weight;

		G[0]->number_of_neighbours[temp_head]++;
	
		array_of_edges_next++;
	}
	if (fclose(fp))
		error_exit("fclose: ");

	// calculate index of first neighbour for sorted lists
	for(int i = 1; i < G[0]->number_of_nodes; i++)
		G[0]->index_of_first_neighbour[i] = G[0]->index_of_first_neighbour[i - 1] + G[0]->number_of_neighbours[i - 1];

	int* neighbours_found = NULL;
	neighbours_found = calloc(G[0]->number_of_nodes, sizeof(int));
	G[0]->sorted_heads = malloc(G[0]->number_of_edges * sizeof(int));
	G[0]->sorted_weights = malloc(G[0]->number_of_edges * sizeof(int));
	if ( NULL == neighbours_found || NULL == G[0]->sorted_heads || NULL == G[0]->sorted_weights )
		error_exit("malloc: ");

	// sort edges by tails
	int tail;
	int index;
	for(int i = 0; i < G[0]->number_of_edges; i++)
	{
		tail = G[0]->tail[i];
		index = G[0]->index_of_first_neighbour[tail];
		G[0]->sorted_heads[index + neighbours_found[tail]] = G[0]->head[i];
		G[0]->sorted_weights[index + neighbours_found[tail]] = G[0]->edge_weight[i];
		neighbours_found[tail] += 1;
	}
	free(neighbours_found);

	G[0]->predecessor = calloc(G[0]->number_of_nodes, sizeof(int));
	G[0]->distance = calloc(G[0]->number_of_nodes, sizeof(int));
	if ( NULL == G[0]->predecessor || NULL == G[0]->distance )
		error_exit("calloc: ");

	// calculate prime numbers
	int* is_prime = calloc(G[0]->number_of_nodes, sizeof(int));

	// G[0]->number_of_nodes - 1 as not counting from 0
	int number_of_terminals = get_primes( is_prime, G[0]->number_of_nodes - 1 );
	printf("\nNUM TERMINALS\t%d\n", number_of_terminals); 

	int* terminals = malloc( number_of_terminals * sizeof(int) );
	int k = 0;
	for (int i = 0; i < G[0]->number_of_nodes; i++)
		if( 1 == is_prime[i] )
		{
			terminals[k] = i;
			k++;
		}

	G = realloc( G, G[0]->number_of_nodes * sizeof(struct graph*) );
	assert(G);

	for ( int i = 1; i < number_of_terminals; i++)
	{
		G[i] = malloc( sizeof(struct graph) );
		G[i]->number_of_nodes = G[0]->number_of_nodes;
		G[i]->number_of_edges = G[0]->number_of_edges;
		G[i]->number_of_neighbours = G[0]->number_of_neighbours;
		G[i]->index_of_first_neighbour = G[0]->index_of_first_neighbour;
		G[i]->sorted_heads = G[0]->sorted_heads;
		G[i]->sorted_weights = G[0]->sorted_weights;
		G[i]->tail = G[0]->tail;
		G[i]->head = G[0]->head;
		G[i]->edge_weight = G[0]->edge_weight;
		G[i]->predecessor = calloc(G[0]->number_of_nodes, sizeof(int));
		G[i]->distance = calloc(G[0]->number_of_nodes, sizeof(int));
	}

	// compute all shortest paths from terminals (nodes that are prime)
	for( int i = 0; i < number_of_terminals; i++ )
	{
		dijkstra(G[i], terminals[i]);
	}
//	print_distances(G, terminals, number_of_terminals);
//	print_predecessors(G, terminals, number_of_terminals);

	// look for terminal closest to current subtree and connect it
	printf("OBJ VALUE\t%d\n", get_tree( G, terminals, number_of_terminals, 0 ) );
	

	struct timeval stop_wall;
	assert( gettimeofday(&stop_wall, NULL) == 0 );

	double stop_cpu = (double)clock() / (double)CLOCKS_PER_SEC;

	double duration_wall = ( stop_wall.tv_sec + stop_wall.tv_usec * 0.000001 ) -  ( start_wall.tv_sec + start_wall.tv_usec * 0.000001 );
	double duration_cpu = stop_cpu - start_cpu;

	printf("\nREAL TIME\t%lfs\nUSER TIME\t%lfs\n\n", duration_wall, duration_cpu);

	free(is_prime);
	free(terminals);
	
	for ( int i = 1; i < number_of_terminals; i++)
	{
		free(G[i]->predecessor);
		free(G[i]->distance);
		free(G[i]);
	}

	free(G[0]->tail);
	free(G[0]->head);
	free(G[0]->edge_weight);
	free(G[0]->predecessor);
	free(G[0]->distance);
	free(G[0]->sorted_heads);
	free(G[0]->sorted_weights);
	free(G[0]->number_of_neighbours);
	free(G[0]->index_of_first_neighbour);
	free(G[0]);
	free(G);
	return 0;
}	
