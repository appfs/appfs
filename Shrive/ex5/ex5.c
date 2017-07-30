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

	int* number_of_neighbours;	/**< number of neighbours for each vertex */
	int* index_of_first_neighbour;	/**< index of first neighbour for each vertex */
	int* sorted_heads;		/**< heads of each edge sorded by vertex */
	int* sorted_weights;		/**< weights of each edge sorded by vertex */

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
	while( size_of_heap >= 0 )
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

	struct graph* G = NULL;

	G = malloc(sizeof(struct graph));
	if ( NULL == G )
		error_exit("malloc: ");

	G->tail = NULL;
	G->head = NULL;
	G->edge_weight = NULL;
	G->predecessor = NULL;
	G->distance = NULL;
	G->sorted_heads = NULL;
	G->sorted_weights = NULL;
	G->number_of_neighbours = NULL;
	G->index_of_first_neighbour = NULL;

	G->number_of_nodes = 0;
	G->number_of_edges = 0;

	G->tail = malloc(array_of_edges_length * sizeof(int));
	G->head = malloc(array_of_edges_length * sizeof(int));
	G->edge_weight = malloc(array_of_edges_length * sizeof(int));
	if ( NULL == G->tail || NULL == G->head || NULL == G->edge_weight )
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
	
	int ret = sscanf(s, "%d %d", &G->number_of_nodes, &G->number_of_edges);
	assert(2 == ret);

	// undirected graph
	G->number_of_edges *= 2;
	// unused node 0
	G->number_of_nodes++;

	G->number_of_neighbours = calloc(G->number_of_nodes, sizeof(int));
	G->index_of_first_neighbour = calloc(G->number_of_nodes, sizeof(int));
	if ( NULL == G->number_of_neighbours || NULL == G->index_of_first_neighbour )
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

			G->tail = realloc(G->tail, array_of_edges_length * sizeof(int));
			G->head = realloc(G->head, array_of_edges_length * sizeof(int));
			G->edge_weight = realloc(G->edge_weight, array_of_edges_length * sizeof(int));
			if (NULL == G->tail || NULL == G->head || NULL == G->edge_weight)
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
		if ( array_of_edges_next == array_of_edges_length )
		{
			array_of_edges_length += EXT_SIZE;

			G->tail = realloc(G->tail, array_of_edges_length * sizeof(int));
			G->head = realloc(G->head, array_of_edges_length * sizeof(int));
			G->edge_weight = realloc(G->edge_weight, array_of_edges_length * sizeof(int));
			if (NULL == G->tail || NULL == G->head || NULL == G->edge_weight)
				error_exit("realloc: ");
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
	if (fclose(fp))
		error_exit("fclose: ");

	// calculate index of first neighbour for sorted lists
	for(int i = 1; i < G->number_of_nodes; i++)
		G->index_of_first_neighbour[i] = G->index_of_first_neighbour[i - 1] + G->number_of_neighbours[i - 1];

	int* neighbours_found = NULL;
	neighbours_found = calloc(G->number_of_nodes, sizeof(int));
	G->sorted_heads = malloc(G->number_of_edges * sizeof(int));
	G->sorted_weights = malloc(G->number_of_edges * sizeof(int));
	if ( NULL == neighbours_found || NULL == G->sorted_heads || NULL == G->sorted_weights )
		error_exit("malloc: ");

	int tail;
	int index;
	// sort edges by tails
	for(int i = 0; i < G->number_of_edges; i++)
	{
		tail = G->tail[i];
		index = G->index_of_first_neighbour[tail];
		G->sorted_heads[index + neighbours_found[tail]] = G->head[i];
		G->sorted_weights[index + neighbours_found[tail]] = G->edge_weight[i];
		neighbours_found[tail] += 1;
	}
	free(neighbours_found);

	G->predecessor = calloc(G->number_of_nodes, sizeof(int));
	G->distance = calloc(G->number_of_nodes, sizeof(int));
	if ( NULL == G->predecessor || NULL == G->distance )
		error_exit("calloc: ");

	int max_node = -1;
	int max_distance = 0;

	int source = 1;

	// call dijkstra algorithm
	// results are stored in graph structure G
	dijkstra(G, source);

	// find longest shortest path
	for(int i = 1; i < G->number_of_nodes - 1; i++)
	{
		if(i != source)
			if ( G->distance[i] > max_distance && INT_MAX > G->distance[1] )
			{
				max_node = i;
				max_distance = G->distance[i];
			}
	}
	struct timeval stop_wall;
	assert( gettimeofday(&stop_wall, NULL) == 0 );

	double stop_cpu = (double)clock() / (double)CLOCKS_PER_SEC;

	double duration_wall = ( stop_wall.tv_sec + stop_wall.tv_usec * 0.000001 ) -  ( start_wall.tv_sec + start_wall.tv_usec * 0.000001 );
	double duration_cpu = stop_cpu - start_cpu;

	printf("\nRESULT VERTEX\t%d\nRESULT DIST\t%d\n\nREAL TIME\t%lfs\nUSER TIME\t%lfs\n\n", max_node, max_distance, duration_wall, duration_cpu);

	free(G->tail);
	free(G->head);
	free(G->edge_weight);
	free(G->predecessor);
	free(G->distance);
	free(G->sorted_heads);
	free(G->sorted_weights);
	free(G->number_of_neighbours);
	free(G->index_of_first_neighbour);
	free(G);
	return 0;
}	
