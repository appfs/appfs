/** @file graph.c
 * @author Franziska Schlösser
 * @date July 2017
 * @brief Short c file containing graph functionality
 */

#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>
#include <assert.h>
#include "misc.h"
#include "graph.h"

void update_neighbor_info(
        GraphSearch *gs,
        unsigned int curr) {

    for (int i = 0; i < gs->g->n_neighbors[curr]; i++) {
		// n is index of neighbor vertex
        unsigned int n = gs->g->neighbors[curr][2*i] - 1; 
		// w is weight of edge from curr to n
        unsigned int w = gs->g->neighbors[curr][(2*i)+1]; 
		// if a neighbor is not visited yet then observe it
        if (!gs->visited[n]) {
            unsigned long newdist = gs->distances[curr] + w;
            unsigned long olddist = gs->distances[n];
            if (newdist < olddist) {
				// we have found a shorter path
                gs->distances[n] = newdist;
                gs->prev[n] = curr;
				decrease_value(gs->to_visit, n, gs->distances[n]);
            }
        }
    }
}

void run_dijkstra(
        GraphSearch *gs) {

    while (gs->to_visit->n != 0) { 
		// curr is the index of the current vertex
        unsigned int curr; 
        curr = pop(gs->to_visit);
        gs->visited[curr] = true;
        update_neighbor_info(gs, curr);
    }
}

void run_steiner_dijkstra(
        unsigned int *vertex_mask,
        unsigned int *prev,
        GraphSearch *gs) {

    while (gs->to_visit->n != 0) { 
		// curr is the index of the current vertex
        unsigned int curr; 
        curr = pop(gs->to_visit);
		if (vertex_mask[curr] == 1) {
			// if the current vertex is an unvisited terminal then next look at the shortest path to it from the current subgraph
			unsigned int walker = curr;
			gs->distances[walker] = 0;
			vertex_mask[walker] = vertex_mask[walker]+2;
			while (vertex_mask[prev[walker]] < 2) {
				walker = prev[walker];
				gs->distances[walker] = 0;
				vertex_mask[walker] = vertex_mask[walker]+2;
				push(gs->to_visit, walker, 0);
			}
		}
        gs->visited[curr] = true;
        update_neighbor_info(gs, curr);
    }
}

void add_closest_terminal(
        Graph *g, 
        unsigned int *vertex_mask,
		unsigned long *distances,
        unsigned int *prev) {
    // at i vertex_mask is 0 or 1 on vertices not contained in subgraph, 2 or 3 on vertices that are contained

    // prepare
	bool *visited = malloc(sizeof(*visited) * g->n_verts);
    Heap *to_visit = malloc(sizeof(*to_visit));
    construct_heap(to_visit, g->n_verts);
    // copy vertex_mask to mask (all visited and dist 0)
    for (int i = 0; i < g->n_verts; i++) {
        if (vertex_mask[i] > 1) {
            // on subgraph everything is visited
            visited[i] = true;
        } else {
            // on complement of subgraph everything is unvisited
            visited[i] = false;
			push(to_visit, i, distances[i]);
        }
    }

    GraphSearch *gs = malloc(sizeof(*gs));
    gs->g = g;
    gs->distances = distances;
    gs->visited = visited;
    gs->to_visit = to_visit; 
    gs->prev = prev;

	// main call
    for (int i = 0; i < g->n_verts; i++) {
        if (vertex_mask[i] > 1) {
            update_neighbor_info(gs, i);
        }
    }
    
    run_dijkstra(gs);
    join_closest_terminal(distances, g->n_verts, vertex_mask, prev);

	// postcare
    delete_heap(gs->to_visit);
    free(gs->to_visit);
    free(visited);
    free(gs);
}

bool unconnected_terminals(
        Graph *g, 
        unsigned int *vertex_mask) {
	
	// look at all terminals
    for (int i = 0; i < g->n_verts; i++) {
        if (vertex_mask[i] == 1) {
            return true;
        } 
    }
    return false;
}

unsigned int* steiner(
        Graph *g, 
		unsigned int start,
        unsigned int *vertex_mask) {
    
	// prepare
	vertex_mask[start-1] = 3;
    unsigned int *prev = malloc(sizeof(*prev) * g->n_verts); // holds indices of predecessor in steiner tree
    unsigned long *distances = malloc(sizeof(*distances) * g->n_verts);
    for (int i = 0; i < g->n_verts; i++) {
        prev[i] = UINT_MAX;
        if (vertex_mask[i] > 1) {
            // distances on subgraph are 0
            distances[i] = 0;
        } else {
            // distances are inf on complement of subgraph
            distances[i] = ULONG_MAX;
        }
    }

	// main call
    while (unconnected_terminals(g, vertex_mask)) { 
        add_closest_terminal(g, vertex_mask, distances, prev);
    }

	// postpare
    free(distances);
    return prev;
}

unsigned int* steiner_modified(
        Graph *g, 
		unsigned int start,
        unsigned int *vertex_mask) {

	// prepare
	vertex_mask[start-1] = 3;
    unsigned int *prev = malloc(sizeof(*prev) * g->n_verts); // holds indices of predecessor in steiner tree
    unsigned long *distances = malloc(sizeof(*distances) * g->n_verts);
    Heap *to_visit = malloc(sizeof(*to_visit));
    construct_heap(to_visit, g->n_verts);
    bool *visited = malloc(sizeof(*visited) * g->n_verts);
    memset(visited, 0, sizeof(*visited) * g->n_verts);
    for (int i = 0; i < g->n_verts; i++) {
        distances[i] = ULONG_MAX;
		push(to_visit, i, distances[i]);
        prev[i] = UINT_MAX;
    }

    GraphSearch *gs = malloc(sizeof(*gs));
    gs->g = g;
    gs->distances = distances;
    gs->visited = visited;
    gs->to_visit = to_visit;; 
    gs->prev = prev;

	// main call
    distances[start-1] = 0;
    decrease_value(gs->to_visit, start-1, 0);

	run_steiner_dijkstra(vertex_mask, prev, gs);
    
	// postpare
    delete_heap(gs->to_visit);
    free(gs->to_visit);
    free(visited);
    free(gs);
    free(distances);

    return prev;
}

void free_graph(
        Graph *g) {
	    
    for (int i = 0; i < g->n_verts; i++) {
        free(g->neighbors[i]);
    }
    free(g->neighbors);
    free(g->n_neighbors);
}

void read_numbers(
        unsigned int *res, 
        int n, 
        char *line) {

    char* s = &line[0];
    char e[LEN_NUMBER];
    memset(e, 0, sizeof(e));
	// for each number try to read it
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

void init_from_graph_file(
        Graph *g, 
        char *file) {
    
    unsigned int **edges;
    unsigned int n_edges = read_graph_file(g, file, &edges);
    
    // sort edges to neighbors
    fill_neighbors(g, edges, n_edges);
    
    free(edges);
}

unsigned int read_graph_file(
        Graph *g, 
        char *file, 
        unsigned int ***edges) {

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

    unsigned int cardinalities[2]; // store number of vertices, number of edges
    read_numbers(cardinalities, 2, line);
    g->n_verts = cardinalities[0];
    unsigned int n_edges = cardinalities[1];

    // alloc memory according to given sizes
    g->n_neighbors = malloc(sizeof(*(g->n_neighbors)) * g->n_verts);
    memset(g->n_neighbors, 0, sizeof(*(g->n_neighbors)) * g->n_verts);

    *edges = malloc(sizeof(**edges) * n_edges);
    for(int i = 0; i < n_edges; i++) {
        (*edges)[i] = malloc(sizeof(*((*edges)[i])) * 3);
        memset((*edges)[i], 0, sizeof(*((*edges)[i])) * 3);
    }

    // read file line by line
    // count number of neighbors for each vertex
    unsigned int count = 0;
    while (NULL != fgets(line, sizeof(line), f)) {
        // numbers contains source_vertex, target_vertex and weight of edge
        unsigned int *numbers = (*edges)[count];
        read_numbers(numbers, 3, line);
        g->n_neighbors[numbers[0]-1]++;
        g->n_neighbors[numbers[1]-1]++;
        count++;
    }
    fclose(f);
    return n_edges;
}

void fill_neighbors(
        Graph *g, 
        unsigned int** edges, 
        unsigned int n_edges) {

	// prepare
    g->neighbors = malloc(sizeof(*(g->neighbors)) * g->n_verts);
    for(int i = 0; i < g->n_verts; i++) {
        g->neighbors[i] = malloc(sizeof(*(g->neighbors[i])) * g->n_neighbors[i] * 2);
        memset(g->neighbors[i], 0, sizeof(*(g->neighbors[i])) * g->n_neighbors[i] * 2);
    }
    memset(g->n_neighbors, 0, sizeof(*g->n_neighbors) * g->n_verts);

	// loop through the edges
    for (int i = 0; i < n_edges; i++) {
        unsigned int v[2];
        memset(v, 0, sizeof(*v) * 2);
        v[0] = edges[i][0];
        v[1] = edges[i][1];
        unsigned int weight = edges[i][2];
        for (int j = 0; j < 2; j++) {
            unsigned int pos = g->n_neighbors[v[j]-1]*2;
            g->neighbors[v[j]-1][pos] = v[(j+1)%2];
            g->neighbors[v[j]-1][pos+1] = weight;
            g->n_neighbors[v[j]-1]++;
        }
    }

	// postpare
    for(int i = 0; i < n_edges; i++) {
        free(edges[i]);
    }
}

unsigned long* shortest_distances_to(
        Graph *g, 
        unsigned int destination,
        unsigned int *prev) {

	// prepare
    unsigned long *distances = malloc(sizeof(*distances) * g->n_verts);
    Heap *to_visit = malloc(sizeof(*to_visit));
    construct_heap(to_visit, g->n_verts);
    for (int i = 0; i < g->n_verts; i++) {
        distances[i] = ULONG_MAX;
        prev[i] = UINT_MAX;
		push(to_visit, i, distances[i]);
    }
    bool *visited = malloc(sizeof(*visited) * g->n_verts);
    memset(visited, 0, sizeof(*visited) * g->n_verts);

    GraphSearch *gs = malloc(sizeof(*gs));
    gs->g = g;
    gs->distances = distances;
    gs->visited = visited;
    gs->to_visit = to_visit; 
    gs->prev = prev;

	// main call
    distances[destination-1] = 0;
    decrease_value(gs->to_visit, destination-1, 0);
    run_dijkstra(gs);
    
	// postpare
    delete_heap(gs->to_visit);
    free(gs->to_visit);
    free(visited);
    free(gs);
    return distances;
}

unsigned long* find_longest(
        unsigned long *distances, 
        int n_verts) {

    unsigned long maxd = 0;
    unsigned int maxv = 0; // index of vert
    for (int i = 0; i < n_verts; i++) {
        unsigned long dist = distances[i];
        if (dist > maxd) {
            maxd = dist;
            maxv = i;
        }
    }
    unsigned long* res = malloc(sizeof(*res) * 2);
    res[0] = maxd;
    res[1] = maxv;
    return res;
}

void join_closest_terminal(
        unsigned long *distances, 
        int n_verts,
        unsigned int *vertex_mask,
        unsigned int *prev) {

    unsigned long mind = ULONG_MAX;
    unsigned int minv = UINT_MAX; // index of vert
    // loop through vertices and find min
	for (int i = 0; i < n_verts; i++) {
        unsigned long dist = distances[i];
        if (dist < mind && dist != 0 && vertex_mask[i]==1) {
            mind = dist;
            minv = i;
        }
    }
	// add pathinfo to vertex_mask and distances
    unsigned int walker = minv;
    // minv holds closest neighboring terminal
    while (vertex_mask[walker] < 2) {
        vertex_mask[walker] = vertex_mask[walker]+2;
		distances[walker] = 0;
        walker = prev[walker];
    }
}

unsigned long weight_of_tree(
        Graph *g,
        unsigned int *vertex_mask,
        unsigned int *prev) {
     
    unsigned long treeweight = 0;
    for(int i = 0; i < g->n_verts; i++) {
		// for all vertices that are not root and that are in subgraph
        if(vertex_mask[i] > 1 && prev[i] != UINT_MAX) { 
			// add path to predecessor
            treeweight = treeweight + edgeweight(g, i, prev[i], false);
        }
    }
    return treeweight;
}

void print_tree(
        Graph *g,
        unsigned int *vertex_mask,
        unsigned int *prev) {
    
    for(int i = 0; i < g->n_verts; i++) {
		// for all vertices that are not root and that are in subgraph
        if(vertex_mask[i] > 1 && prev[i] != UINT_MAX) { 
            printf("(%d,%d) ", i+1, prev[i]+1);
        }
    }
    printf("\n");
}

unsigned long edgeweight(
        Graph *g,
        unsigned int v1,
        unsigned int v2, 
        bool directed) {

    if (true == directed) {
		// find directed edgeweight
        for (int i = 0; i < g->n_neighbors[v1]; i++) {
            if (g->neighbors[v1][2*i] == v2 + 1) {
                return g->neighbors[v1][(2*i)+1];
            }
        }
        return ULONG_MAX;
    } else {
        // for undirected weights find minimum directed weight
		unsigned long w1 = edgeweight(g, v1, v2, true);
        unsigned long w2 = edgeweight(g, v2, v1, true);
        unsigned long weight = (w1 < w2 ? w1 : w2);
        assert(weight < ULONG_MAX);
        return weight;
    }
}

bool check_steiner(
	Graph *g,
	unsigned int *terminal_mask,
	unsigned int *tree_mask,
	unsigned int *prev
	) {
	
	for (int i = 0; i < g->n_verts; i++) {
		// attains all terminals
		if (terminal_mask[i] == 1) {
			if( tree_mask[i] != 3 ) {
				return false;
			}
		}
		if (tree_mask[i] > 1) {
			// is connected and tree
			unsigned int walker = i;
			unsigned int count = 0;
			while (prev[walker] != UINT_MAX && count < g->n_verts) {
				assert( edgeweight(g, walker, prev[walker], false) < ULONG_MAX );
				walker = prev[walker];
				count++;
			}
			if (count > g->n_verts) {
				return false;
			}
		}
	}
	return true;
}
