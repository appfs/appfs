
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<ctype.h>
#include<assert.h>

#define LEN_NUMBER 30

typedef int bool;
enum { false, true };

signed int get_next_destination(signed int *to_visit, signed int n_to_visit, signed long *distances) {
/// Get next destination.
/** 
 * Get next destination from elements in 'to_visit'.
 *
 * 'to_visit' contains 'n_to_visit' elements
 * return the element from 'to_visit' for which 'distances' has the lowest value
 */
	signed int mind = -1;
	signed int minv = -1;
	signed int ind = -1;
	for (int i = 0; i < n_to_visit; i++) {
		signed int v = to_visit[i];
		signed long d = distances[v];
		if (d < mind || mind == -1) {
			mind = d;
			minv = v;
			ind = i;
		}
	}
	to_visit[ind] = to_visit[n_to_visit-1];
	return minv;
}

void read_numbers(int n, char *line, signed int *res) {
/// Read numbers from chararry.
/**
 * Read 'n' numbers from 'line' and store them in 'res'
 */
    char* s = &line[0];
    char e[LEN_NUMBER];
	memset(e, 0, sizeof(e));
    for (int i = 0; i < n; i++) {
        int count = 0;
        while(isdigit(*s)) {
            e[count] = *s;
            count++;
            s++;
        }
		e[count] = '\0';
        s++;
        res[i] = atoi(e);
    }
}

signed int read_graph_file(char *file, signed int **n_neighbors, signed int ***edges, signed int *n_verts) {
/// Read in a graph from file.
/* 
 * Read from file with name 'file'
 * Store edges in 'edges' and numbers of neighbors in 'n_neighbors'
 * Store number of vertices in 'n_verts'
 * Return number of edges
 */
    FILE *f = fopen(file, "r");
    if (NULL == f) {
        printf("File does not exist. Aborting.\n");
        exit(0);
    }
    
    // assume vertices are numbered 1 .. n
	// assume each edge is only described once
    // assume graph is undirected

	// assume line never longer than ...
    char line[3*LEN_NUMBER]; 
 
    // assume first line holds number of vertices and number of edges
    fgets(line, sizeof(line), f);

    signed int cardinalities[2]; // store number of vertices, number of edges
    read_numbers(2, line, cardinalities);
    *n_verts = cardinalities[0];
    signed int n_edges = cardinalities[1];

    // alloc memory according to given sizes
    *n_neighbors = malloc(sizeof(**n_neighbors) * n_edges); 
	memset(*n_neighbors, 0, sizeof(**n_neighbors) * (*n_verts));
    
    *edges = malloc(sizeof(**edges) * n_edges); 
	for(int i = 0; i < n_edges; i++) {
		(*edges)[i] = malloc(sizeof(*((*edges)[i])) * 3);
		memset((*edges)[i], 0, sizeof(*((*edges)[i])) * 3);
	}
	
    // read file line by line
    // count number of neighbors for each vertex
	signed int count = 0;
    while (NULL != fgets(line, sizeof(line), f)) {
        // numbers contains source_vertex, target_vertex and weight of edge
        signed int *numbers = (*edges)[count];
        read_numbers(3, line, numbers);
		(*n_neighbors)[numbers[0]-1]++;
		(*n_neighbors)[numbers[1]-1]++;
		count++;
    }
    fclose(f);
    return n_edges;
}

signed int** fill_neighbors(signed int n_verts, signed int n_edges, signed int* n_neighbors, signed int** edges) {
/// Sort edges into neighbors lists
/* 
 * 'n_verts' is number of vertices
 * 'n_edges' is number of edges
 * 'n_neighbors' is array containing number of neighbors of vertices
 * 'edges' is array containing edge triples
 * Return array containing for each vertex an array of neighbors with weights
 */
	signed int **neighbors = malloc(sizeof(*neighbors) * n_verts); 
	for(int i = 0; i < n_verts; i++) {
    	neighbors[i] = malloc(sizeof(*neighbors[i]) * n_neighbors[i] * 2);
		memset(neighbors[i], 0, sizeof(*neighbors[i]) * n_neighbors[i] * 2);
	}
	memset(n_neighbors, 0, sizeof(*n_neighbors) * n_verts);
	for (int i = 0; i < n_edges; i++) {
		signed int v[2];
		memset(v, 0, sizeof(*v) * 2);
		v[0] = edges[i][0];
		v[1] = edges[i][1];
		signed int weight = edges[i][2];
		for (int j = 0; j < 2; j++) {
			signed int pos = n_neighbors[v[j]-1]*2;
			neighbors[v[j]-1][pos] = v[(j+1)%2];
			neighbors[v[j]-1][pos+1] = weight;
			n_neighbors[v[j]-1]++;
		}
	}
	for(int i = 0; i < n_edges; i++) {
		free(edges[i]);
	}
    return neighbors;
} 

signed long* shortest_distances_to(signed int destination, signed int n_verts, signed int** neighbors, signed int* n_neighbors) {
///
/* TODO 
 *
 */
	signed long *distances = malloc(sizeof(*neighbors) * n_verts);
	signed int to_visit[n_verts]; // holds indices of vertices in queue to visit
	for (int i = 0; i < n_verts; i++) {
		distances[i] = -1;
		to_visit[i] = -1;
	}
	bool *visited = malloc(sizeof(*visited) * n_verts);// [n_verts];
	memset(visited, 0, sizeof(*visited) * n_verts);

	distances[destination-1] = 0;
	to_visit[0] = destination-1;
	signed int n_to_visit = 1;

	while (n_to_visit != 0) { 
		signed int curr; // curr is the index of the current vertex
		curr = get_next_destination(to_visit, n_to_visit, distances); 
		n_to_visit--;

		assert(n_to_visit <= n_verts);
		
		visited[curr] = true;
		for (int i = 0; i < n_neighbors[curr]; i++) {
			signed int n = neighbors[curr][2*i] - 1; // n is index of neighbor vertex
			signed int w = neighbors[curr][(2*i)+1]; // w is weight of edge from curr to n
			if (!visited[n]) {  
				signed int newdist = distances[curr] + w;
				signed int olddist = distances[n];
				if (olddist == -1 || newdist < olddist) { 
					distances[n] = newdist;
				}
				bool found = false;
				for (int k = 0; k < n_to_visit; k++) {
					if (to_visit[k] == n) {
						found = true;
					}
				}
				if (!found) {
					to_visit[n_to_visit] = n;
					n_to_visit++;
				}
			}	
		}
	}
    return distances;
}

signed long* find_longest(signed long *distances, int n_verts) {
	signed long maxd = 0;
	signed int maxv = 0; // index of vert
	for (int i = 0; i < n_verts; i++) {
		signed long dist = distances[i];
		if (dist > maxd) {
			maxd = dist;
			maxv = i;
		}
	}
    signed long* res = malloc(sizeof(*res) * 2);
    res[0] = maxd;
    res[1] = maxv;
    return res;
}

int main(int argc, char **argv) {
    if (argc != 2) {
        printf("Please provide filename containing as first and only argument.");
        exit(0);
    }
    
// ##### init from file
	
    // filename is first commandlinearg
	char *file = argv[1];
    // destination is source node
    signed int destination = 1;

    signed int *n_neighbors;
	signed int **edges;

    signed int n_verts = 0;
    signed int n_edges = read_graph_file(file, &n_neighbors, &edges, &n_verts);

	//float density = ((float)n_edges)/(n_verts/2 * n_verts);
	//bool sparse = (density < 0.1 ? true : false);	

	// sort edges to neighbors
	signed int **neighbors = fill_neighbors(n_verts, n_edges, n_neighbors, edges); // for each vertex this holds a list of neighbors with weights
	free(edges);

// ##### find lengths of shortest paths to destination
	signed long *distances = shortest_distances_to(destination, n_verts, neighbors, n_neighbors); 

// ##### free memory of graph
	for (int i = 0; i < n_verts; i++) {
		free(neighbors[i]);
	}
	free(neighbors);
	free(n_neighbors);

// ##### find longest amongst shortests
    signed long *res = find_longest(distances, n_verts);

    free(distances);

    // TODO timething
    printf("RESULT VERTEX %li\n\nRESULT DIST %li\nRESULT TIME %d\n", res[1]+1, res[0], 10);
    free(res);
	return 0; // everything went fine (hopefully)
}

