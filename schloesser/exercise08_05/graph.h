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
    signed int n_verts;
    signed int *n_neighbors;
    signed int **neighbors;
} Graph;

/** @brief Calculate a steiner tree
 *
 * @param g Graph in question
 * @param vertex_mask info about which vertices are terminals
 */
void steiner(Graph *g, signed int *vertex_mask);

/** @brief Free the memory allocated in graph.
 *
 * @param g the graph to be freed
 */
void free_graph(Graph *g);

/** @brief Get next destination.
 * Get next destination from elements in 'to_visit'.
 *
 * 'to_visit' contains 'n_to_visit' elements
 * return the element from 'to_visit' for which 'distances' has the lowest value
 */
signed int get_next_destination(signed int *to_visit, signed int n_to_visit, signed long *distances);

/** @brief Read numbers from chararry.
 *
 * @param n number of numbers
 * @param line chararray from which to read
 * @param res store the result here
 */
void read_numbers(signed int *res, int n, char *line);

/** @brief Read in a graph from file.
 *
 * @param file Read graph from file with this filename
 * @param edges store edges
 * @return number of edges
 */
signed int read_graph_file(Graph *g, char *file, signed int ***edges);

/** @brief Sort edges into neighbors lists
 *
 * @param g Graph on which to work on
 * @param edges is array containing edge triples
 * @param n_edges is number of edges
 * @return Return array containing for each vertex an array of neighbors with weights
 */
void fill_neighbors(Graph *g, signed int** edges, signed int n_edges);

/** @brief Get shortest distances from all vertices to one destination
 *
 * @param g Graph on which to work on
 * @param destination the destination vertex to which the distances should be calculated
 */
signed long* shortest_distances_to(Graph *g, signed int destination);

/** @brief Find the maximum value in an array and return value and position
 *
 * @param distances an array containing values
 * @param n_verts the length of distances
 * @return an array of length two containing value and position of first maximal value in distances
 */
signed long* find_longest(signed long *distances, int n_verts);

#endif
