#ifndef GRAPH_H
#define GRAPH_H

/**
 * @file graph.h
 * @author Franziska Schlösser
 * @date July 2017
 * @brief Header file of graph.c
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <assert.h>

/** @brief define the maximum length of a number to be read from a string
  */
#define LEN_NUMBER 30

/** @brief Define bool type
  */
typedef enum { false, true } bool;

typedef struct Graph {
    unsigned int n_verts;
    unsigned int *n_neighbors;
    unsigned int **neighbors;
} Graph;

typedef struct GraphSearch {
    Graph g;
    unsigned long *distances;
    bool *visited;
    unsigned int *to_visit;
    unsigned int *n_to_visit;
    unsigned int *prev;
} GraphSearch;

/** TODO
 *
 */
void update_neighbor_info(
         Graph *g, 
         unsigned long *distances, 
         bool *visited, 
         unsigned int *to_visit, 
         unsigned int *n_to_visit,
         unsigned int *prev,
         unsigned int curr);

/** TODO
 *
 */
void run_dijkstra( 
        Graph *g, 
        unsigned long *distances, 
        bool *visited, 
        unsigned int *to_visit, 
        unsigned int n_to_visit,
        unsigned int *prev); 

/** TODO
 *
 */
void add_closest_terminal( 
        Graph *g, 
        unsigned int *vertex_mask, 
        unsigned int *prev); 

/** TODO
 *
 */
bool unconnected_terminals(
        Graph *g,
        unsigned int *vertex_mask);

/** @brief Calculate a steiner tree
 *
 * @param g Graph in question
 * @param vertex_mask info about which vertices are terminals
 */
void steiner(
        Graph *g, 
        unsigned int *vertex_mask);

/** @brief Free the memory allocated in graph.
 *
 * @param g the graph to be freed
 */
void free_graph(
        Graph *g);

/** @brief Get next destination.
 * Get next destination from elements in 'to_visit'.
 *
 * 'to_visit' contains 'n_to_visit' elements
 * return the element from 'to_visit' for which 'distances' has the lowest value
 */
unsigned int get_next_destination(
        unsigned int *to_visit, 
        unsigned int n_to_visit, 
        unsigned long *distances);

/** @brief Read numbers from chararry.
 *
 * @param n number of numbers
 * @param line chararray from which to read
 * @param res store the result here
 */
void read_numbers(
        unsigned int *res, 
        int n, 
        char *line);

/** @brief Read in a graph from file.
 *
 * @param file Read graph from file with this filename
 * @param edges store edges
 * @return number of edges
 */
unsigned int read_graph_file(
        Graph *g, 
        char *file, 
        unsigned int ***edges);

/** @brief Sort edges into neighbors lists
 *
 * @param g Graph on which to work on
 * @param edges is array containing edge triples
 * @param n_edges is number of edges
 * @return Return array containing for each vertex an array of neighbors with weights
 */
void fill_neighbors(
        Graph *g, 
        unsigned int** edges, 
        unsigned int n_edges);

/** @brief Get shortest distances from all vertices to one destination
 *
 * @param g Graph on which to work on
 * @param destination the destination vertex to which the distances should be calculated
 */
unsigned long* shortest_distances_to(
        Graph *g, 
        unsigned int destination,
        unsigned int *paths);

/** @brief Find the maximum value in an array and return value and position
 *
 * @param distances an array containing values
 * @param n_verts the length of distances
 * @return an array of length two containing value and position of first maximal value in distances
 */
unsigned long* find_longest(
        unsigned long *distances, 
        int n_verts);

/** @brief Find the minimum positive value in an array and return value and position
 *
 * @param distances an array containing values
 * @param n_verts the length of distances
 * TODO
 */
void join_closest_terminal(
        unsigned long *distances, 
        int n_verts,
        unsigned int *vertex_mask,
        unsigned int *prev);

#endif
