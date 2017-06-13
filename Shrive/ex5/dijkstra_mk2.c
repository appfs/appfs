//dijkstra_mk2.c

#include <assert.h>
#include <ctype.h>
#include <float.h>
#include <limits.h>
#include <math.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define EXT_SIZE     (1024*1024)
#define MAX_LINE_LEN 512

#define error_exit(msg)  error_exit_fun(msg, __FILE__, __LINE__)

// borrowed from https://github.com/appfs/appfs/blob/master/01_Exercise/ex1a.c
// maintained by TRI-PETER SHRIVE
// EMAIL: Tri.Shrive@gmail.com

static void error_exit_fun(const char* const msg, const char* const file, const int lineno)
{
	assert(NULL != msg);
	assert(NULL != file);

	fprintf(stderr, "%s(%d) ", file, lineno);
	perror(msg);
	exit(EXIT_FAILURE);
}

// borrowed from http://www.cdn.geeksforgeeks.org/greedy-algorithms-set-6-dijkstras-shortest-path-algorithm/
// maintained by TRI-PETER SHRIVE
// EMAIL: Tri.Shrive@gmail.com

// Returns the index of the vertex/node 
// that has the minimum distance value
int min_distance(double* distances, bool* shortest_path_set, int number_of_nodes)
{
	printf("in min_distance\n");
	double min = INT_MAX;
	int min_index = -1;

	for (int i = 0; i < number_of_nodes; i++)
		if (shortest_path_set[i] == false && distances[i] <= min)
			min = distances[i], min_index = i;
	printf("min = %lf, min_index = %d\n", min, min_index);
	return min_index;
}

// Prints the constructed distance array
int print_solution(double* distances, int n)
{
	printf("\nVertex\tDistance from Source\n");
	for (int i = 0; i < n; i++)
		printf("%d\t%lf\n", i, distances[i]);

	return 0;
}

/*

*/
void dijkstra(int* node_a, int* node_b, double* edge_weight, int number_of_nodes, int number_of_edges, int source)
{
	// distances[i] will hold the shortest distance from source to i
	double* distances = calloc(number_of_nodes, sizeof(double));
	// shortest_path_set[i] is true if vertex i is in set
	bool* shortest_path_set = calloc(number_of_nodes, sizeof(double));
	if ( NULL == distances || NULL == shortest_path_set )
	{
		error_exit("malloc: ");
	}

	printf("Initialise all distances as infinite and shortest_path_set as false\n");
	
	for (int i = 0; i < number_of_nodes; i++)
		distances[i] = INT_MAX, shortest_path_set[i] = false;

	distances[source] = 0;

	printf("Find the shortest path for all vertices\n");
	for (int iteration = 0; iteration < number_of_nodes; iteration++)
	{
		// get the closest vertex in the set of vertices that have not yet been processed. 
		// min is the index of the source in the first iteration
		int min = min_distance(distances, shortest_path_set, number_of_nodes);

		// mark this vertex as processed
		shortest_path_set[min] = true;
		printf("Node %d added to shortest path set\n", min);

		// Update distances value of the adjacent vertices
		for (int j = 0; j < number_of_nodes; j++)
		{
			// Skip node if in shortest path set
			if (shortest_path_set[j])
			{
				printf("shortest_path_set[%d] = %d\n", j, shortest_path_set[j]);
				continue;
			}
			// Skip node if minimum distance is infinite
			if (distances[min] == INT_MAX)
			{
				printf("distances[%d] = %lf\n", min, distances[min]);
				continue;
			}
			// new code
			for (int k = 0; k < number_of_edges; k++)
			{

				// Skip if not an adjacent node
				if( node_a[k] == min )
				{
					// Update the value of distances[node_b[k]] 
					// if sum of distance from source distances[min] and weight of edge node_a[k] to node_b[k] (i.e. edge_weight[k]) 
					// is less than the distance value of distances[node_b[k]].
					if( distances[min] + edge_weight[k] < distances[node_b[k]] )
					{
						printf("distance updated\n");
						distances[node_b[k]] = distances[min] + edge_weight[k];
					}
				}
				if( node_b[k] == min )
				{
					// Update the value of distances[node_a[k]]
					// if sum of dis frm src distance[min] and edge_weight[k]
					// is less than distances[node_a[k]].
					if( distances[min] + edge_weight[k] < distances[node_a[k]] )
					{
						printf("distance updated\n");
						distances[node_a[k]] = distances[min] + edge_weight[k];
					}
				}

			}
		}
	}
	print_solution(distances, number_of_nodes);
	free( distances );
	free( shortest_path_set );
}

// borrowed from https://github.com/appfs/appfs/blob/master/01_Exercise/ex1a.c
// maintained by TRI-PETER SHRIVE
// EMAIL: Tri.Shrive@gmail.com

int main(int argc, const char* const* const argv)
{
	if (argc < 2)
	{
		fprintf(stderr, "usage: %s filename\n", argv[0]);
		exit(EXIT_FAILURE);
	}

	//Initialise arrays
	size_t array_of_edges_next;
	size_t array_of_edges_length;

	int* node_a;
	int* node_b;
	double* edge_weight;

	array_of_edges_next = 0;
	array_of_edges_length = EXT_SIZE;

	node_a = malloc(array_of_edges_length * sizeof(int));
	node_b = malloc(array_of_edges_length * sizeof(int));
	edge_weight = malloc(array_of_edges_length * sizeof(double));

	if ( NULL == node_a || NULL == node_b || NULL == edge_weight )
	{
		error_exit("malloc: ");
	}
	// Open file for reading
	FILE* fp = fopen(argv[1], "r");

	if (NULL == fp)
	{
		error_exit("fopen: ");
	}
	int lineno = 0;
	char line[MAX_LINE_LEN];

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

		int temp_node_a, temp_node_b = 0;
		double temp_edge_weight = NAN;

		int ret = sscanf(s, "%d %d %lf", &temp_node_a, &temp_node_b, &temp_edge_weight);

		if (3 != ret)
		{
			fprintf(stderr, "\nLine %d: ret = %d != 3\n", lineno, ret);
			continue;
		}
		if (temp_node_a < 0 || temp_node_b < 0)
		{
			fprintf(stderr, "\nLine %d: node_a = %d, node_b = %d\n", lineno, temp_node_a, temp_node_b);
			continue;
		}
		if (temp_edge_weight < -DBL_EPSILON)
		{
			fprintf(stderr, "\nLine %d, edge_weight = %lf\n", lineno, temp_edge_weight); 
			continue;
		}

		// check we have enought space in the array, otherwise enlarge the array
		if ( array_of_edges_next == array_of_edges_length )
		{
			array_of_edges_length += EXT_SIZE;

			node_a = realloc(node_a, array_of_edges_length * sizeof(int));
			node_b = realloc(node_b, array_of_edges_length * sizeof(int));
			edge_weight = realloc(edge_weight, array_of_edges_length * sizeof(double));
			if (NULL == node_a || NULL == node_b || NULL == edge_weight)
				error_exit("realloc: ");
		}
		assert( array_of_edges_next < array_of_edges_length);

		// finally, store the value
		node_a[array_of_edges_next] = temp_node_a;
		node_b[array_of_edges_next] = temp_node_b;
		edge_weight[array_of_edges_next] = temp_edge_weight;

		array_of_edges_next++;
	}
	if (fclose(fp))
		error_exit("fclose: ");

	for(int i = 0; i < array_of_edges_next; i++)
	{
//		printf("Line %d: node_a = %d; node_b = %d; edge_weight %lf\n", i+1, node_a[i], node_b[i], edge_weight[i]);
	}

	printf("\nFile: %s with %d lines\n", argv[1], lineno);
	printf("Total number of edges = %d\n", array_of_edges_next);

	dijkstra(node_a, node_b, edge_weight, array_of_edges_next, array_of_edges_next, 0);

	free(node_a);
	free(node_b);
	free(edge_weight); 

	return 0;
}

