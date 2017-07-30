/**
	\file
	\brief  A program for APPFS ex10
	\author Tri-Peter Shrive
 */

#include <assert.h>
#include <ctype.h>
#include <limits.h>
#include <math.h>
#include <omp.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>
#include <time.h>

#include "ex10.h"

/**
	utility function that simplifies error handling
 */
void error_exit_fun(
	const char* const msg,		/**< message to be displayed */
	const char* const file,		/**< file name */
	const unsigned int lineno	/**< line number */
	)
{
	assert( NULL != msg );
	assert( NULL != file );

	fprintf( stderr, "%s(%u) ", file, lineno );
	perror( msg );
	exit( EXIT_FAILURE );
}

/**
	sifts an entry up through binary heap
 */
int sift_up(
	unsigned int* heap,		/**< nodes in heap */
	INTEGER* distance,	/**< distance of nodes in heap */
	unsigned int* index,		/**< index of nodes in heap */
	unsigned int current		/**< current position of node in heap */
	)
{
	// store child node in temp variable
	unsigned int temp_a = heap[current];
	// store distance to child node in temp variable
	INTEGER dist = distance[temp_a];
	int parent;
	unsigned int temp_b;

	while( 0 < current )
	{
		// calculate parents location in heap array
		parent = ( current - 1 ) / 2;
		assert( 0 <= parent );
		// store parent in temp variable 
		temp_b = heap[parent];

		// is the distance to parent node greater than the distance to child?
		// if not break out of loop
		if( distance[temp_b] <= dist )
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
	unsigned int* heap,		/**< nodes in heap */
	INTEGER* distance,	/**< distance of nodes in heap */
	unsigned int* index,		/**< index of nodes in heap */
	unsigned int current,		/**< current position of node in heap */
	const int size		/**< size of heap */
	)
{
	INTEGER dist;
	int child;
	unsigned int temp_a;
	unsigned int temp_b;

	// calculate location of child in heap array
	child = current + current + 1;
	// store parent node as temporary variable
	temp_a = heap[current];
	// store distance of current node as temporary variable
	dist = distance[temp_a];

	// while node has a child
	while( child <= size )
	{
		// store first child node as temporary variable
		temp_b = heap[child];
		// does a second child exist?
		if( child + 1 <= size )
		{
			// is the distance to the second child less than that of the first?
			if( distance[ heap[child + 1] ] < distance[temp_b] )
			{
				child++;
				// replace first child node with second child node
				temp_b = heap[child];
			}
		}
		// is the distance to child node less than the distance to parent?
		// if not break out of loop
		if( distance[temp_b] >= dist )
			break;

		// replace parent node with child node
		heap[current] = temp_b;
		// update child nodes heap index
		index[temp_b] = current;
		// child becomes next parent
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

#ifndef NDEBUG
/**
	assesses validity of heap index
 */
int is_heap_index_valid(
	unsigned int* heap,		/**< nodes in heap */
	unsigned int* index,		/**< index of nodes in heap */
	const int size			/**< size of heap */
	)
{
	unsigned int sum = 0;
	for( int i = 0; i <= size; i++ )
	{
		sum += ( heap[ index[heap[i]] ] != heap[i] );
	}
	return ( 0 == sum);
}

int is_heap_order_valid(
	unsigned int* heap,
	INTEGER* distance,
	const int size
	)
{
	unsigned int sum = 0;
	int current = 0;
	int child = current + current + 1;
	while( child <= size )
	{
		sum += ( distance[ heap[ current ] ] > distance[ heap [ child ] ] );
		child++;
		if( child <= size )
		{
			sum += ( distance[ heap[ current ] ] > distance[ heap [ child ] ] );
		}
		current++;
		child= current + current + 1;
	}
	return ( 0 == sum );
}

int is_tree_valid(
	unsigned int* predecessor,
	unsigned int* terminal,
	unsigned int* source,
	const unsigned int n_nodes,
	const unsigned int n_term
	)
{
	int next;
	int current;
	for( unsigned int i = 0; i < n_term; i++ )
	{
		current = terminal[i];
		next = predecessor[ current ];
		while( INT_MAX > next )
		{
			current = next;
			next = predecessor[ current ];
			if( current == next )
				return -1;
		}
		source[i] = current;		
	}
	// there should only be one source
	unsigned int sum = 0;
	for( unsigned int i = 1; i < n_term; i++ )
	{
		sum += ( source[0] != source[i] );
	}
	return ( 0 == sum );
}
#endif

/**
	calculates steiner tree for given graph and source terminal using dijkstra's algorithm
 */
int steiner(
	struct graph* G,	/**< static graph attributes */
	struct graph* H,	/**< variable graph attributes */
	unsigned int* is_prime,		/**< array where entries are 1 when index is prime */
	unsigned int source		/**< source node */
	)
{
	assert( G );
	assert( H->predecessor );
	assert( H->distance );
	assert( H->tree_pred );
	assert( G->sorted_heads );
	assert( G->sorted_weights );
	assert( G->number_of_neighbours );
	assert( G->index_of_first_neighbour );

	// create set containing all vetrices
	// datastructure used is a binary heap
	// index 0 is highest priority
	// if node not on heap it has lowest priority
	unsigned int* heap = NULL;
	heap = malloc( G->number_of_nodes * sizeof(unsigned int) );
	unsigned int* index_on_heap = NULL;
	index_on_heap = malloc( G->number_of_nodes * sizeof(unsigned int) );

	for( unsigned int i = 0; i < G->number_of_nodes; i++ )
	{
		// heap entry is initially empty
		// meaning all vetrices have lowest priority
		heap[i] = INT_MAX;
		index_on_heap[i] = INT_MAX;
		// the vetrex's predecessor in shortest path tree is unknown
		H->predecessor[i] = INT_MAX;
		// the vetre's predecessor in steiner tree is unknown
		H->tree_pred[i] = INT_MAX;
		// the vetrex's distance from source is assumed to be infinite
		H->distance[i] = INTEGER_MAX;
	}
	assert( source < G->number_of_nodes );
	// distance to source is zero
	H->distance[source] = 0;
	// add source to heap
	heap[0] = source;
	index_on_heap[source] = 0;
	// largest valid heap index is set to 0
	int size_of_heap = 0;

	// tempary variables for storing edge attributes 
	unsigned int tail;
	unsigned int head;
	INTEGER dist;
	unsigned int index_of_neighbour;
	unsigned int root;

	unsigned int close;
	unsigned int next;

	H->sum = 0;
	H->count = 0;

	// loop while heap has entries
	while( size_of_heap >= 0 )
	{

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
		sift_down( heap, H->distance, index_on_heap, 0, size_of_heap );
		assert( is_heap_index_valid( heap, index_on_heap, size_of_heap ) );
		assert( is_heap_order_valid( heap, H->distance, size_of_heap ) );

		// if tail is terminal add all nodes on shortest path to source to subtree
		// i.e. set distance to zero and then add to heap or decrease key
		if( 1 == is_prime[tail] )
		{
			// increase count of terminals in subtree
			H->count++;
			// add weights on path from subtree to terminal to objective value
			H->sum += H->distance[tail];
			// ensure we don't add this terminal twice 
			is_prime[tail] = 0;

			// loop through nodes on shortest path to subtree
			close = tail;
			next = H->predecessor[close];
			while( INT_MAX != next )
			{
				// record this node as child nodes' predecessor
				H->tree_pred[close] = next;
				// is the node already on heap?
				if( INT_MAX == index_on_heap[close] )
				{
					// add node with distance zero
					size_of_heap++;
					heap[size_of_heap] = close;
					index_on_heap[close] = size_of_heap;
					H->distance[close] = 0;

					sift_up( heap, H->distance, index_on_heap, size_of_heap );
					assert( is_heap_index_valid( heap, index_on_heap, size_of_heap ) );
					assert( is_heap_order_valid( heap, H->distance, size_of_heap ) );
				}
				else
				{
					// node already on heap, set distance to zero and sift up from current possition
					H->distance[close] = 0;

					sift_up( heap, H->distance, index_on_heap, index_on_heap[close] );
					assert( is_heap_index_valid( heap, index_on_heap, size_of_heap ) );
					assert( is_heap_order_valid( heap, H->distance, size_of_heap ) );
				}
				// move along shortest path toward subtree
				close = next;
				next = H->predecessor[close];
			}
			// are all terminals connected?
			if( H->count == G->number_of_terminals )
				break;
		}


		// for each of the tail's neighbours we do the following
		for( unsigned int i = 0; i < G->number_of_neighbours[tail]; i++ )
		{
			index_of_neighbour = G->index_of_first_neighbour[tail]+i;
			// set the current neighbour as the temporary head
			head = G->sorted_heads[index_of_neighbour];
			// calculate the current distance
			dist = H->distance[tail] + G->sorted_weights[index_of_neighbour];
			// can we best the current shortest path?
			if( dist < H->distance[head] )
			{
				// update the distance to head
				H->distance[head] = dist;
				// update the heads predecessor
				H->predecessor[head] = tail;
				// is the current head already on heap?
				if( index_on_heap[head] == INT_MAX )
				{	
					// add head at bottom of heap
					size_of_heap++;
					assert( size_of_heap < G->number_of_nodes + 1 );
					heap[size_of_heap] = head;
					index_on_heap[head] = size_of_heap;
					// sift head upwards in heap
					// until at correct place in priority queue
					sift_up( heap, H->distance, index_on_heap, size_of_heap );
					assert( is_heap_index_valid( heap, index_on_heap, size_of_heap ) );
					assert( is_heap_order_valid( heap, H->distance, size_of_heap ) );
				}
				else
				{
					// decrease key
					sift_up( heap, H->distance, index_on_heap, index_on_heap[head] );
					assert( is_heap_index_valid( heap, index_on_heap, size_of_heap ) );
					assert( is_heap_order_valid( heap, H->distance, size_of_heap ) );
				}
			}					
		}

	}
	free( heap );
	free( index_on_heap );
	return 0;
}

/**
	sets entry at index of prime numbers to 1
 */
int get_primes(
	unsigned int* is_prime,		/**< allocate memory for this array of size max and set the memory to zero */
	unsigned int max 		/**< size of array is_prime, the largest number to be assessed for primality */
	)
{
	unsigned int rest;

	is_prime[2] = 1;
	unsigned int count = 1;
	unsigned int last_div;
	unsigned int i;
	unsigned int j;
#ifdef THREADS
	#pragma omp parallel for default(none) shared(is_prime, max, count) private(i, j, last_div, rest) num_threads(THREADS)
#else
	#pragma omp parallel for default(none) shared(is_prime, max, count) private(i, j, last_div, rest) 
#endif
	for( i = 3; i < max; i += 2 )
	{
		last_div = (unsigned int) ceil( sqrt(i) );
		for( j = 2; j <= last_div ; j++ )
		{
			rest = i % j;
			if( 0 == rest )
				break;

			if( j == last_div )
			{
				#pragma omp atomic
					count++;
				is_prime[i] = 1;
			}
		}
	}
	return count;
}

/**
	reads data from file storing nodes and weights in graph structure. then calls dijkstra's algorithm and assesses longest shortes path
 */
int main( 
	int argc, 
	const char* const* const argv 
	)
{
	if( argc < 2 )
	{
		fprintf( stderr, "usage: %s *.gph (n_start_points) (-s)\n", argv[1] );
		exit( EXIT_FAILURE );
	}

	size_t array_of_edges_next = 0;
	size_t array_of_edges_length = EXT_SIZE;

	struct graph* G = NULL;

	G = malloc( sizeof(struct graph) );
	if( NULL == G )
		error_exit( "malloc: " );

	G->tail = NULL;
	G->head = NULL;
	G->edge_weight = NULL;
	G->predecessor = NULL;
	G->sorted_heads = NULL;
	G->sorted_weights = NULL;
	G->number_of_neighbours = NULL;
	G->index_of_first_neighbour = NULL;

	G->number_of_nodes = 0;
	G->number_of_edges = 0;

	G->tail = malloc( array_of_edges_length * sizeof(unsigned int) );
	G->head = malloc( array_of_edges_length * sizeof(unsigned int) );
	G->edge_weight = malloc( array_of_edges_length * sizeof(INTEGER) );
	if( NULL == G->tail || NULL == G->head || NULL == G->edge_weight )
		error_exit( "malloc: " );

	// open file for reading
	FILE* fp = fopen( argv[1], "r" );

	if( NULL == fp )
		error_exit( "fopen: " );

	unsigned int lineno = 0;
	char line[MAX_LINE_LEN];

	fgets( line, sizeof(line), fp );

	lineno++;

	// remove comments and anything after newline
	char* s = strpbrk( line, "#\n" );

	if( NULL != s )
		*s = '\0';

	// skip initial spaces
	for( s = &line[0]; isspace( *s ); s++ );

	assert( '\0' != *s );
	
	int ret = sscanf( s, "%u %u", &G->number_of_nodes, &G->number_of_edges );
	assert( 2 == ret );

	// undirected graph
	G->number_of_edges *= 2;
	// unused node 0
	G->number_of_nodes++;

	G->number_of_neighbours = calloc( G->number_of_nodes, sizeof(unsigned int) );
	G->index_of_first_neighbour = calloc( G->number_of_nodes, sizeof(unsigned int) );
	if( NULL == G->number_of_neighbours || NULL == G->index_of_first_neighbour )
		error_exit( "malloc: " );
	
	while( NULL != fgets( line, sizeof(line), fp ) )
	{
		lineno++;

		// remove comments and anything after newline
		char* s = strpbrk( line, "#\n" );

		if( NULL != s )
			*s = '\0';

		// skip initial spaces
		for( s = &line[0]; isspace(*s); s++ )
			;

		// skip line if empty
		if( '\0' == *s )
			continue;

		unsigned int temp_tail = 0;
		unsigned int temp_head = 0;
		INTEGER temp_edge_weight = INTEGER_MAX;

		ret = sscanf( s, "%u %u %llu", &temp_tail, &temp_head, &temp_edge_weight );

		if( 3 != ret )
		{
			fprintf( stderr, "\nWarning:  Line %u, sscanf returned %u != 3\n", lineno, ret );
			continue;
		}
		if( 0 > temp_tail || 0 > temp_head )
		{
			fprintf( stderr, "\nWarning: Line %u, tail = %u, head = %u\n", lineno, temp_tail, temp_head );
			continue;
		}
		if( 2000000000 <= temp_edge_weight )
		{
			fprintf( stderr, "\nWarning: Line %u, edge_weight = %llu\n", lineno, temp_edge_weight ); 
			continue;
		}

		// check there's enought space in the array, if not enlarge the array
		if( array_of_edges_next == array_of_edges_length )
		{
			array_of_edges_length += EXT_SIZE;

			G->tail = realloc( G->tail, array_of_edges_length * sizeof(unsigned int) );
			G->head = realloc( G->head, array_of_edges_length * sizeof(unsigned int) );
			G->edge_weight = realloc( G->edge_weight, array_of_edges_length * sizeof(INTEGER) );
			if( NULL == G->tail || NULL == G->head || NULL == G->edge_weight )
				error_exit("realloc: ");
		}

		assert( G->number_of_nodes > temp_tail );
		assert( G->number_of_nodes > temp_head );

		// store the edge in memory
		G->tail[array_of_edges_next] = temp_tail;
		G->head[array_of_edges_next] = temp_head;
		G->edge_weight[array_of_edges_next] = temp_edge_weight;

		G->number_of_neighbours[temp_tail]++;
	
		array_of_edges_next++;

		// repeat with head and tail inverted for undirected graph
		if( array_of_edges_next == array_of_edges_length )
		{
			array_of_edges_length += EXT_SIZE;

			G->tail = realloc( G->tail, array_of_edges_length * sizeof(unsigned int) );
			G->head = realloc( G->head, array_of_edges_length * sizeof(unsigned int) );
			G->edge_weight = realloc( G->edge_weight, array_of_edges_length * sizeof(INTEGER) );
			if ( NULL == G->tail || NULL == G->head || NULL == G->edge_weight )
				error_exit( "realloc: " );
		}

		assert( G->number_of_nodes > temp_tail );
		assert( G->number_of_nodes > temp_head );

		// store reversed edge in memory
		G->tail[array_of_edges_next] = temp_head;
		G->head[array_of_edges_next] = temp_tail;
		G->edge_weight[array_of_edges_next] = temp_edge_weight;

		G->number_of_neighbours[temp_head]++;
	
		array_of_edges_next++;
	}
	if( fclose(fp) )
		error_exit("fclose: ");

	// calculate index of first neighbour for sorted lists
	for( unsigned int i = 1; i < G->number_of_nodes; i++ )
		G->index_of_first_neighbour[i] = G->index_of_first_neighbour[i - 1] + G->number_of_neighbours[i - 1];

	unsigned int* neighbours_found = NULL;
	neighbours_found = calloc( G->number_of_nodes, sizeof(unsigned int) );
	G->sorted_heads = malloc( G->number_of_edges * sizeof(unsigned int) );
	G->sorted_weights = malloc( G->number_of_edges * sizeof(INTEGER) );
	if( NULL == neighbours_found || NULL == G->sorted_heads || NULL == G->sorted_weights )
		error_exit( "malloc/calloc: " );

	// sort edges by tails
	unsigned int tail;
	unsigned int index;
	for( unsigned int i = 0; i < G->number_of_edges; i++ )
	{
		tail = G->tail[i];
		index = G->index_of_first_neighbour[tail];
		G->sorted_heads[index + neighbours_found[tail]] = G->head[i];
		G->sorted_weights[index + neighbours_found[tail]] = G->edge_weight[i];
		neighbours_found[tail] += 1;
	}

#ifndef NDEBUG
	unsigned int total_neighbours = 0;
	for( unsigned int i = 0; i < G->number_of_nodes; i++ )
		total_neighbours += neighbours_found[i];

	assert( G->number_of_edges == total_neighbours );
#endif


	free(neighbours_found);
	free( G->tail );
	free( G->head );
	free( G->edge_weight );

	// calculate prime numbers
	unsigned int* is_prime = NULL;
	is_prime = calloc( G->number_of_nodes, sizeof(unsigned int) );
	G->number_of_terminals = get_primes( is_prime, G->number_of_nodes );
	// create array of terminal nodes
	unsigned int* terminals = NULL;
	terminals = malloc( G->number_of_terminals * sizeof(unsigned int) );
	if( NULL == terminals || NULL == is_prime )
		error_exit( "malloc: " );

	unsigned int k = 0;
	for( unsigned int i = 0; i < G->number_of_nodes; i++ )
		if( 1 == is_prime[i] )
		{
			terminals[k] = i;
			k++;
		}

	assert( k == G->number_of_terminals );

	// process the argv[2] and if present the argv[3] 
	unsigned int n = 0;
	unsigned int m = 1;
	unsigned int s_flag = 0;
	unsigned int max_t = INT_MAX;
	for( unsigned int i = 2; i < argc; i++ )
	{
		// optionally print steiner tree
		m = strcmp( argv[i], "-s" );
		if( 0 == m ) s_flag = 1;
		// read in max number of terminals to use as source
		else if( 2 == i )
		{
			// the number of terminals to start from must be equal or less than the total number of terminals
			// the default is 100 terminals if that is less than the total number of terminals
			// otherwise the default is all terminals
			n = sscanf( argv[i], "%u", &max_t );
			if( max_t < k && 1 == n ) k = max_t;
			else if( 100 < k && 1 != n ) k = 100;
		}
	}

	// declare variables and allocate memory used in parallel for loop
	unsigned int t;
	unsigned int* prime = NULL;
	INTEGER obj_val = INTEGER_MAX;
	struct graph* H = NULL;
	G->distance = NULL;
	G->distance = calloc( G->number_of_nodes, sizeof(INTEGER) );
	G->tree_pred = NULL;
	G->tree_pred = malloc( G->number_of_nodes * sizeof(unsigned int) );
	if ( NULL == G->tree_pred || NULL == G->distance )
		error_exit( "calloc: " );

	// start wall clock
	struct timeval start_wall;
	unsigned int fail = gettimeofday( &start_wall, NULL );
	assert( 0 == fail );
	// start counting cpu clocks
	double start_cpu = (double)clock() / (double)CLOCKS_PER_SEC;

#ifdef THREADS		
	#pragma omp parallel for default(none) shared(is_prime, k, G, obj_val, terminals) private(t, H, prime) num_threads(THREADS)
#else
	#pragma omp parallel for default(none) shared(is_prime, k, G, obj_val, terminals) private(t, H, prime)
#endif
		for( t = 0; t < k; t++ )
		{
			H = malloc(sizeof(struct graph));
			if(NULL == H)
				error_exit("malloc: ");

			H->predecessor = malloc( G->number_of_nodes * sizeof(unsigned int) );
			H->distance = malloc( G->number_of_nodes * sizeof(INTEGER) );
			H->tree_pred = malloc( G->number_of_nodes * sizeof(unsigned int) );
			if ( NULL == H->predecessor || NULL == H->distance || NULL == H->tree_pred )
				error_exit( "malloc: " );

			prime = malloc( G->number_of_nodes * sizeof(unsigned int) );
			if( NULL == prime )
				error_exit("malloc: ");

			memcpy( prime, is_prime, G->number_of_nodes * sizeof(unsigned int) );

			steiner( G, H, prime,  terminals[t] );

			free( H->predecessor );
			free( H->distance );

			for( int j = 0; j < G->number_of_terminals; j++ )
				assert( 0 == prime[j] );

			free( prime );
			#pragma omp critical ( objective_value )
			{
				// can we best our current objective value
				if( obj_val > H->sum )
				{
					obj_val = H->sum;
					memcpy( G->tree_pred, H->tree_pred, G->number_of_nodes * sizeof(unsigned int) );
				}
			}

			free( H->tree_pred );
			free( H );
		}

	// stop the wall clock
	struct timeval stop_wall;
	fail = gettimeofday( &stop_wall, NULL );
	assert( 0 == fail );
	// stop counting cpu clocks
	double stop_cpu = (double)clock() / (double)CLOCKS_PER_SEC;
	// calculate durations of both
	double duration_wall = ( stop_wall.tv_sec + stop_wall.tv_usec * 0.000001 ) -  ( start_wall.tv_sec + start_wall.tv_usec * 0.000001 );
	double duration_cpu = stop_cpu - start_cpu;

	unsigned int* source = NULL;
	source = calloc( G->number_of_terminals, sizeof(unsigned int) );
	if( NULL == source )
		error_exit("calloc: ");

	assert( is_tree_valid( G->tree_pred, terminals, source, G->number_of_nodes, G->number_of_terminals ) );

	// print results
	printf( "\n" );
	printf( "TLEN:\t%llu\n", obj_val );

	if( s_flag )
	{
		printf( "TREE:\t" );
		for( unsigned int i = 0; i < G->number_of_nodes; i++ )
			if(  INT_MAX != G->tree_pred[i] )
				printf( "(%u,%u) ", i, G->tree_pred[i] );

		printf( "\n\n" );
	}

	printf( "TIME:\t%lf sec\n", duration_cpu );
	printf( "WALL:\t%lf sec\n\n", duration_wall );

	free( is_prime );
	free( terminals );

	free( G->tree_pred );
	free( G->distance );
	free( G->sorted_heads );
	free( G->sorted_weights );
	free( G->number_of_neighbours );
	free( G->index_of_first_neighbour );
	free( G );
	return 0;
}	
