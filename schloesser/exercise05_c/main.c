/* what is up with 423 51234235? */

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<ctype.h>

#define LEN_NUMBER 30

typedef int bool;
enum { false, true };

signed int get_next_destination(signed int to_visit[], signed int n_to_visit, signed long distances[], signed int n_verts) {
	signed int mind = -1;
	signed int minv = -1;
	signed int ind = -1;
	for (int i = 0; i < n_to_visit; i++) {
		signed int v = to_visit[i];
		signed long d = distances[v];
		//printf("neighbor: index %d, vertex %d, weight %li\n", i, v+1, d); // OUTPUT
		if (d < mind || mind == -1) {
			mind = d;
			minv = v;
			ind = i;
		}
	}
	to_visit[ind] = to_visit[n_to_visit-1];
	//printf("returning %d\n", minv); // OUTPUT
	return minv;
}

void read_numbers(int n, char* line, signed int* res) {
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

int main(int argc, char **argv) {
    if (argc != 2) {
        printf("Please provide filename containing as first and only argument.");
        exit(0);
    }
    
// ##### init from file
	
    // filename is first commandlinearg
	char* file = argv[1];
    FILE *f = fopen(file, "r");
    if (NULL == f) {
        printf("File does not exist. Aborting.\n");
        exit(0);
    }
    int destination = 1;
    
	// assume line never longer than ...
    char line[3*LEN_NUMBER]; 
    // assume first line holds number of vertices and number of edges
    fgets(line, sizeof(line), f);

    signed int cardinalities[2]; // store number of vertices, number of edges
    read_numbers(2, line, cardinalities);
	signed int n_verts = cardinalities[0];
	signed int n_edges = cardinalities[1];
    //printf("Number of vertices: %d,\nNumber of edges: %d\n", n_verts, n_edges); //OUTPUT
    
    // assume vertices are numbered 1 .. n
	// assume each edge is only described once
    // assume graph is undirected
	signed int edges[n_edges][3];
	memset(edges, 0, sizeof(edges));
	signed int n_neighbors[n_verts];
	memset(n_neighbors, 0, sizeof(n_neighbors));
	signed int *neighbors[n_verts]; // for each vertex this holds a list of neighbors with weights
	float density = ((float)n_edges)/(n_verts/2 * n_verts);
	//bool sparse = (density < 0.1 ? true : false);
	//printf("Density of Adjecencymatrix: %.2f\n", density); // OUTPUT
	signed int count = 0;
	
    // read file line by line
    while (NULL != fgets(line, sizeof(line), f)) {
        // numbers contains source_vertex, target_vertex and weight of edge
        signed int *numbers = edges[count];
        read_numbers(3, line, numbers);
        //printf("adding edge between %d and %d with weight %d\n", edges[count][0], edges[count][1], edges[count][2]); //OUTPUT
		n_neighbors[numbers[0]-1]++;
		n_neighbors[numbers[1]-1]++;
		count++;
    }
    fclose(f);
	// sort edges to neighbors
	for (int i = 0; i < n_verts; i++) {
		neighbors[i] = malloc( n_neighbors[i]*sizeof(*neighbors[i])*2 );
	}
	memset(n_neighbors, 0, sizeof(n_neighbors));
	for (int i = 0; i < n_edges; i++) {
        //printf("accessing edge between %d and %d with weight %d\n", edges[i][0], edges[i][1], edges[i][2]); //OUTPUT
		signed int v[2];
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

// ##### find lengths of shortest paths to destination
	//for (int i = 0; i < n_verts; i++) { for (int j = 0; j < n_neighbors[i]; j++) { printf("Vertex %d to %d with weight %d\n", i+1, neighbors[i][2*j], neighbors[i][(2*j)+1]); } } // OUTPUT
	signed long distances[n_verts];
	signed int to_visit[n_verts]; // holds indices of vertices in queue to visit
	for (int i = 0; i < n_verts; i++) {
		distances[i] = -1;
		to_visit[i] = -1;
	}
	bool visited[n_verts];
	memset(visited, 0, sizeof(visited));

	distances[destination-1] = 0;
	to_visit[0] = destination-1;
	signed int n_to_visit = 1;

	while (n_to_visit != 0) { 
		signed int curr; // curr is the index of the current vertex
		curr = get_next_destination(to_visit, n_to_visit, distances, n_verts); 
		n_to_visit--;

		if (n_to_visit >= n_verts) return 1; // assert TODO delete
		
		visited[curr] = true;
		for (int i = 0; i < n_neighbors[curr]; i++) {
			signed int n = neighbors[curr][2*i] - 1; // n is index of neighbor vertex
			signed int w = neighbors[curr][(2*i)+1]; // w is weight of edge from curr to n
			//printf("\t neighbor %d, weight %li\n", n, w); // OUTPUT
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

// ##### free memory of graph
	for (int i = 0; i < n_verts; i++) {
		free(neighbors[i]);
	}

// ##### find longest amongst shortests
	signed long maxd = 0;
	signed int maxv = 0; // index of vert
	for (int i = 0; i < n_verts; i++) {
//		printf("distance from %d is %li\n", i+1, distances[i]); //OUTPUT
		signed long dist = distances[i];
		if (dist > maxd) {
			maxd = dist;
			maxv = i;
		}
	}

    printf("RESULT VERTEX %d\n\nRESULT DIST %li\n", maxv+1, maxd);
	return 0; // everything went fine (hopefully)
}

