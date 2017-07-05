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

/** @brief Define type Graph
 *
 * Containing 
 * number of vertices as 'n_verts', 
 * number of neighbors as 'n_neighbors' 
 * and all neighbors as 'neighbors'
 */
typedef struct Graph {
    unsigned int n_verts;
    unsigned int *n_neighbors;
    unsigned int **neighbors;
} Graph;

/** @brief Define type GraphSearch
 *
 * Containing
 * a graph as 'g',
 * an array of distances as 'distances',
 * an array of bools indicating the visited status as 'visited',
 * an array of vertices that are still to visit as 'to_visit',
 * the size of to_visit as 'n_to_visit'
 * and an array 'prev' containing the predecessors of vertices.
 */
typedef struct GraphSearch {
    Graph *g;
    unsigned long *distances;
    bool *visited;
    unsigned int *to_visit;
    unsigned int n_to_visit;
    unsigned int *prev;
} GraphSearch;

/** @brief Updating the distances and to_visit list via a vertex.
 *
 * @param gs the graphsearch
 * @param curr the current vertex
 * @return the graphstructure is being updated
 */
void update_neighbor_info(
        GraphSearch *gs,
        unsigned int curr);

/** @brief Run the dijkstra algorithm.
 *
 * While still having vertices to visit, perform dijkstra steps.
 * @param gs the graphsearch
 * @return the graphstructure is being updated
 */
void run_dijkstra( 
        GraphSearch *gs);

/** @brief Add the closest terminal to a subgraph to that subgraph
 *
 * @param g a graph
 * @param vertex_mask a mask indicating which terminals are vertices (%2 = 1) 
 *     and which vertices are contained in the subgraph (> 1)
 * @param for each vertex the predecessor in a tree
 * @return the vertex_mask and the predecessors in prev are being updated
 */
void add_closest_terminal( 
        Graph *g, 
        unsigned int *vertex_mask, 
        unsigned int *prev); 

/** @brief Decide if there are unconnected terminals.
 * 
 * @param g a graph
 * @param vertex_mask a mask indicting which terminals are vertices (%2 = 1)
 *     and which vertices are contained in the subgraph (> 1)
 * @return bool if there exists terminals that are not contained in the subgraph.
 */
bool unconnected_terminals(
        Graph *g,
        unsigned int *vertex_mask);

/** @brief Calculate a steiner tree
 *
 * @param g Graph in question
 * @param vertex_mask info about which vertices are terminals 
 * @return the list of predecessors in a (hopefully good) approximation to a steiner tree.
 *     the values in vertex_mask are being updated to indicate the subgraph with values > 1.
 */
unsigned int* steiner(
        Graph *g, 
        unsigned int *vertex_mask);

/** @brief Free the memory allocated in graph.
 *
 * @param g the graph to be freed
 */
void free_graph(
        Graph *g);

/** @brief Get the closest unvisited vertex.
 *
 * @param to_visit contains elements which still have to be visited
 * @oaram n_to_visit number of elements in 'to_visit'
 * @return the element from 'to_visit' for which 'distances' has the smallest value
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
 *     edges and g are being updated
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
 *     graph g is being updated
 */
void fill_neighbors(
        Graph *g, 
        unsigned int **edges, 
        unsigned int n_edges);

/** @brief Get shortest distances from all vertices to one destination
 *
 * @param g Graph on which to work on
 * @param destination the destination vertex to which the distances should be calculated
 *     destination is being updated
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

/** @brief Join a terminal which is closest to the subgraph to it.
 *
 * @param distances an array containing the distances
 * @param n_verts the length of 'distances'
 * @param vertex_mask the mask indicating which vertices are contained in the subgraph (> 1)
 *     and which vertices are terminals (%2 = 1).
 * @param prev an array containing the predecessors of vertices on a tree
 * @return the vertex_mask is being updated
 */
void join_closest_terminal(
        unsigned long *distances, 
        int n_verts,
        unsigned int *vertex_mask,
        unsigned int *prev);

#endif
