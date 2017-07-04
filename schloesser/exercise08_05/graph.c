/**
 * @file graph.c
 * @author Franziska Schlösser
 * @date July 2017
 * @brief Short c file containing graph functionality
 */

#include "graph.h"

void steiner(Graph *g, signed int *vertex_mask) {
    /*
    save own edges
    mask vertices (in vertex_mask?)

    mark first terminal
    for every new terminal
        mark shortest path to subgraph
    */
}

void free_graph(Graph *g) {
    for (int i = 0; i < g->n_verts; i++) {
        free(g->neighbors[i]);
    }
    free(g->neighbors);
    free(g->n_neighbors);
}

signed int get_next_destination(signed int *to_visit, signed int n_to_visit, signed long *distances) {
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

void read_numbers(signed int *res, int n, char *line) {
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

signed int read_graph_file(Graph *g, char *file, signed int ***edges) {
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
    read_numbers(cardinalities, 2, line);
    g->n_verts = cardinalities[0];
    signed int n_edges = cardinalities[1];

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
    signed int count = 0;
    while (NULL != fgets(line, sizeof(line), f)) {
        // numbers contains source_vertex, target_vertex and weight of edge
        signed int *numbers = (*edges)[count];
        read_numbers(numbers, 3, line);
        g->n_neighbors[numbers[0]-1]++;
        g->n_neighbors[numbers[1]-1]++;
        count++;
    }
    fclose(f);
    return n_edges;
}

void fill_neighbors(Graph *g, signed int** edges, signed int n_edges) {
    g->neighbors = malloc(sizeof(*(g->neighbors)) * g->n_verts);
    for(int i = 0; i < g->n_verts; i++) {
        g->neighbors[i] = malloc(sizeof(*(g->neighbors[i])) * g->n_neighbors[i] * 2);
        memset(g->neighbors[i], 0, sizeof(*(g->neighbors[i])) * g->n_neighbors[i] * 2);
    }
    memset(g->n_neighbors, 0, sizeof(*g->n_neighbors) * g->n_verts);
    for (int i = 0; i < n_edges; i++) {
        signed int v[2];
        memset(v, 0, sizeof(*v) * 2);
        v[0] = edges[i][0];
        v[1] = edges[i][1];
        signed int weight = edges[i][2];
        for (int j = 0; j < 2; j++) {
            signed int pos = g->n_neighbors[v[j]-1]*2;
            g->neighbors[v[j]-1][pos] = v[(j+1)%2];
            g->neighbors[v[j]-1][pos+1] = weight;
            g->n_neighbors[v[j]-1]++;
        }
    }
    for(int i = 0; i < n_edges; i++) {
        free(edges[i]);
    }
}

signed long* shortest_distances_to(Graph *g, signed int destination) {
    signed long *distances = malloc(sizeof(*g->neighbors) * g->n_verts);
    signed int to_visit[g->n_verts]; // holds indices of vertices in queue to visit
    for (int i = 0; i < g->n_verts; i++) {
        distances[i] = -1;
        to_visit[i] = -1;
    }
    bool *visited = malloc(sizeof(*visited) * g->n_verts);// [n_verts];
    memset(visited, 0, sizeof(*visited) * g->n_verts);

    distances[destination-1] = 0;
    to_visit[0] = destination-1;
    signed int n_to_visit = 1;

    while (n_to_visit != 0) { 
        signed int curr; // curr is the index of the current vertex
        curr = get_next_destination(to_visit, n_to_visit, distances); 
        n_to_visit--;

        assert(n_to_visit <= g->n_verts);

        visited[curr] = true;
        for (int i = 0; i < g->n_neighbors[curr]; i++) {
            signed int n = g->neighbors[curr][2*i] - 1; // n is index of neighbor vertex
            signed int w = g->neighbors[curr][(2*i)+1]; // w is weight of edge from curr to n
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
