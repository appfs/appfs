/* what is up with 423 51234235? */

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<ctype.h>

#define LEN_NUMBER 30

void read_numbers(int n, char* line, signed long* res) {
    char* s = &line[0];
    char e[LEN_NUMBER];
    for (int i = 0; i < n; i++) {
        int count = 0;
        while(isdigit(*s)) {
            e[count] = *s;
            count++;
            s++;
        }
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
    FILE *f = fopen(file, "r");
    if (NULL == f) {
        printf("File does not exist. Aborting.\n");
        exit(0);
    }
    
    char line[3*LEN_NUMBER]; // assume line never longer than 100 chars
    // assume first line has number of vertices and number of edges
    fgets(line, sizeof(line), f);
    signed long lengths[2];
    read_numbers(2, line, lengths);
    printf("Number of vertices: %li,\nNumber of edges: %li\n", lengths[0], lengths[1]);
    
    // assume vertices are numbered 1 .. n
    int num_vertices = lengths[0];
    signed long edges[lengths[1]];
    
    // read line by line
    while (NULL != fgets(line, sizeof(line), f)) {
        // numbers contains source_vertex, target_vertex and weight of edge
        signed long numbers[3];
        read_numbers(3, line, numbers);
        printf("adding edge between %li and %li with weight %li\n", numbers[0], numbers[1], numbers[2]);
        // assume graph is undirected
        // TODO add edges
    }
    fclose(f);
    
    // ##### TODO find lengths of shortest paths
    int destination = 0;
    
    // ##### TODO find longest amongst shortests

    // TODO store it in res
    int res[2];
    res[0] = 1; // vertex 
    res[1] = 0; // distance
    printf("RESULT VERTEX %d\n\nRESULT DIST %d\n", res[0], res[1]);
}

