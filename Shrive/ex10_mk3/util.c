/**
	@file
	@brief Ulitity funcitons for APPFS ex10.
	@author Tri-Peter Shrive
*/

#include "ex10.h"

/**
	sets entry at index of prime numbers to 1
*/
int get_primes(
	unsigned int* is_prime,	/**< pointer to array of size ceiling, with memory set to zero */
	unsigned int ceiling	/**< only numbers below this value will be assessed for primality */
	)
{
	unsigned int rest;
	is_prime[2] = 1;
	unsigned int count = 1;
	unsigned int last_div;
	unsigned int i;
	unsigned int j;

#ifdef THREADS
	#pragma omp parallel for default(none) shared(is_prime, ceiling, count) private(i, j, rest, last_div) num_threads(THREADS)
#endif
		for( i = 3; i < ceiling; i += 2 )
		{
			last_div = ( unsigned int ) ceil( sqrt( i ) );
			for( j = 2; j <= last_div; j++ )
			{
				rest = i % j;
				if( 0 == rest ) break;
				else if(j == last_div )
				{
#ifdef THREADS
					# pragma omp atomic
#endif
						count++;
					is_prime[i] = 1;
				}
			}
		}

	return count;
}

/**
	sifts entry up in binary heap
*/
int sift_up(
	unsigned int* heap,	/**< heap entries */
	unsigned int* heap_index,	/**< nodes index in heap */
	const unsigned long long int* const distance,	/**< nodes distance from source */
	unsigned int current	/**< node to be sifted */
	)
{
	unsigned int a = heap[current];
	unsigned long long int dist = distance[a];
	unsigned int parent;
	unsigned int b;

	while( 1 < current )
	{
		parent = current / 2;
		b = heap[parent];
		if( distance[b] <= dist )
			break;

		heap[current] = b;
		heap_index[b] = current;
		current = parent;
	}
	heap[current] = a;
	heap_index[a] = current;

	return 0;
}

/**
	sifts entry down in binary heap
*/
int sift_down(
	unsigned int* heap,	/**< heap entries */
	unsigned int* heap_index,	/**< nodes index in heap */
	const unsigned long long int* const distance,	/**< nodes distance from source */
	unsigned int current,	/**< node to be sifted */
	const unsigned int heap_size	/**< size of heap */
	)
{
	unsigned int child = current + current;
	unsigned int a = heap[current];
	unsigned long long int dist = distance[a];
	unsigned int b;

	while( child <= heap_size )
	{
		b = heap[child];
		if( child + 1 <= heap_size )
		{
			if( distance[ heap[ child + 1 ] ] < distance[b] )
			{
				child++;
				b = heap[child];
			}
		}
		if( distance[b] >= dist )
			break;

		heap[current] = b;
		heap_index[b] = current;
		current = child;
		child += child;
	}
	heap[current] = a;
	heap_index[a] = current;

	return 0;	
}

/**
	calculates steiner tree using dijkstra algorithm and additional heuristic
	@returns number of terminals connected to steiner tree
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
	const unsigned int* const sorted_tails, 	/**< array of tails with an entry for each of their edges */
	unsigned int* tree_edges,	/**< entries will be set to 1 if sorted lists entries are part of steiner tree else 0*/
	unsigned int* prev_edge_index,	/**< nodes previous edge in dijkstra */
	const unsigned int source 	/**< source terminal */
	)
{
	assert( is_prime );
	assert( terminals );
	assert( first_neighbours_index );
	assert( sorted_weights );
	assert( sorted_heads );
	assert( tree_edges );
	assert( prev_edge_index );

	assert( source < num_nodes );

	unsigned int i;

	unsigned int* heap = NULL;
	unsigned int* heap_index = NULL;
	unsigned int* tree_nodes = NULL; 
	unsigned long long int* distance = NULL;
	// heap[0] reserved for indicating empty heap
	heap = malloc( ( num_nodes + 1 ) * sizeof( unsigned int ) );
	heap_index = malloc( ( num_nodes + 1) * sizeof( unsigned int ) );
	tree_nodes = calloc( num_nodes, sizeof( unsigned int ) );
	distance = malloc( num_nodes * sizeof( unsigned long long int ) );
	assert( heap );
	assert( heap_index );
	assert( tree_nodes );
	assert( distance );


	for( i = 0; i < num_nodes; i++ )
	{
		heap[i] = INT_MAX;
		heap_index[i] = INT_MAX;
		prev_edge_index[i] = INT_MAX;
		distance[i] = LLONG_MAX;
	}
	heap[num_nodes] = INT_MAX;
	heap_index[num_nodes] = INT_MAX;

	unsigned int heap_size = 0;
	distance[source] = 0;
	heap_size++;
	heap[heap_size] = source;
	heap_index[source] = heap_size;
	tree_nodes[source] = 1;


	unsigned int count = 1;
	unsigned int current;
	unsigned int tail;
	unsigned int head;
	unsigned int root;
	unsigned int edge_index;
	unsigned long long int dist;
	// loop while heap if not empty
	while( heap_size != 0 )
	{
		// pop priority one node from heap
		tail = heap[1];
		assert( tail < num_nodes );
		heap_index[tail] = INT_MAX;
		root = heap[heap_size];
		assert( root < num_nodes );
		heap[1] = root;
		heap_index[root] = 1;
		heap_size--;
		sift_down( heap, heap_index, distance, 1, heap_size );

		// steiner heuristic
		// is tail a terminal?
		if( 1 == is_prime[tail] )
		{
			current = tail;

			// is this node aleady part of subtree?
			while( 0 == tree_nodes[current] )
			{

				edge_index = prev_edge_index[current];
	
				// add edge to steiner tree
				tree_edges[edge_index] = 1;
				tree_nodes[current] = 1;

				if( current == tail )
				{
					count++;
				}

				// is current node on heap?
				if( INT_MAX == heap_index[current] )
				{	
					// add node with distance zero
					distance[current] = 0;
					heap_size++;
					assert( heap_size != num_nodes + 1 );
					heap[heap_size] = current;
					heap_index[current] = heap_size;
					sift_up( heap, heap_index, distance, heap_size );
				}
				else
				{
					// node already on heap
					// set distance to zero
					// and sift up from current position
					sift_up( heap, heap_index, distance, heap_index[current] );
				}
				current = sorted_tails[edge_index];
			}
			if( count == num_terminals ) break;
		}
				
		// dijkstra algorithm
		for( i = 0; i < num_neighbours[tail]; i++ )
		{
			// get neighbour
			edge_index = first_neighbours_index[tail] + i;
			assert( edge_index < num_edges );
			head = sorted_heads[edge_index];
			assert( head < num_nodes );
			// is node already part of steiner tree? 
			if( 0 == tree_nodes[head] )
			{
				dist = distance[tail] + sorted_weights[edge_index];
				assert( dist < LLONG_MAX );
				// have we beaten the current best distance?
				if( dist < distance[head] )
				{
					distance[head] = dist;
					prev_edge_index[head] = edge_index;
					// is head already on heap?
					if( heap_index[head] == INT_MAX )
					{
						// add head at bottom of heap and sift up 
						// until at correct place in priority queue
						heap_size++;
						assert( heap_size != num_nodes + 1 );
						heap[heap_size] = head;
						heap_index[head] = heap_size;
						sift_up( heap, heap_index, distance, heap_size );
					}
					else
					{
						// find heap entry for head using heap_index
						// and sift up
						sift_up( heap, heap_index, distance, heap_index[head] );
					}
				}
			}
		}
	}
	free( heap );
	free( heap_index );
	free( tree_nodes );
	free( distance );

	return count;
}

/**
	verifies steiner tree
	@returns 1 if steiner tree is connected else 0
*/
int is_tree_valid(
	const unsigned int* const sorted_heads,	/** array of heads sorted by tail */
	const unsigned int* const sorted_tails,	/** array of tails one for each edge */
	const unsigned int* const prev_edge_index,	/** nodes previous edge in dijkstra */
	const unsigned int* const terminals,	/**< terminal nodes */
	const unsigned int num_terminals,	/**< number of terminals */
	const unsigned int num_nodes,	/**< number of nodes */
	const unsigned int num_edges	/**< number of nodes */

	)
{
	unsigned int i;
	unsigned int t;
	unsigned int prev;
	unsigned int current;
	unsigned int* visited = NULL;
	visited = calloc( num_edges, sizeof( unsigned int ) );
	assert( visited );
	for( i = 0; i < num_terminals; i++ )
	{
		t = terminals[i];
		current = t;
		while( 0 == visited[current] )
		{
			visited[current] = 1;
			prev = prev_edge_index[current];
			if( INT_MAX == prev )
			{
				if( 0 == i ) break;
				else
				{
					printf("ERROR: terminal %u not connected\n", t);
					return 0;
				}
			}		
			current = sorted_tails[prev];
		}
	}
	free( visited );

	return 1;	
}

