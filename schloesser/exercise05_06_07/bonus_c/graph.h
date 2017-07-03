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

/** @brief Get next destination.
 * Get next destination from elements in 'to_visit'.
 *
 * 'to_visit' contains 'n_to_visit' elements
 * return the element from 'to_visit' for which 'distances' has the lowest value
 */
signed int get_next_destination(signed int *to_visit, signed int n_to_visit, signed long *distances);

/** @brief Read numbers from chararry.
 *
 * Read 'n' numbers from 'line' and store them in 'res'
 */
void read_numbers(int n, char *line, signed int *res);

/** @brief Read in a graph from file.
 *
 * Read from file with name 'file'
 * Store edges in 'edges' and numbers of neighbors in 'n_neighbors'
 * Store number of vertices in 'n_verts'
 * Return number of edges
 */
signed int read_graph_file(char *file, signed int **n_neighbors, signed int ***edges, signed int *n_verts);

/** @brief Sort edges into neighbors lists
 *
 * @param n_verts is number of vertices
 * @param n_edges is number of edges
 * @param n_neighbors is array containing number of neighbors of vertices
 * @param edges is array containing edge triples
 * @return Return array containing for each vertex an array of neighbors with weights
 */
signed int** fill_neighbors(signed int n_verts, signed int n_edges, signed int* n_neighbors, signed int** edges);

/** @brief Get shortest distances from all vertices to one destination
 *
 * @param destination the destination vertex to which the distances should be calculated
 * @param n_verts the number of vertices in the graph
 * @param neighbors an array containing the neighbors of each vertex
 * @param n_neighbors an array containing the numbers of neighbors of each vertex
 */
signed long* shortest_distances_to(signed int destination, signed int n_verts, signed int** neighbors, signed int* n_neighbors);

/** @brief Find the maximum value in an array and return value and position
 *
 * @param distances an array containing values
 * @param n_verts the length of distances
 * @return an array of length two containing value and position of first maximal value in distances
 */
signed long* find_longest(signed long *distances, int n_verts);

#endif
