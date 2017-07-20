#ifndef GRAPH_H
#define GRAPH_H

/** @file graph.h
 * @author Franziska Schlösser
 * @date July 2017
 * @brief Header file of graph.c
 */

#include "misc.h"
#include "heap.h"

/** @brief define the maximum length of a number to be read from a string
  */
#define LEN_NUMBER 30

/** @brief Define type Graph
 */
typedef struct Graph {
    /** number of vertices */
    unsigned int n_verts; 
    /** number of neighbors */
    unsigned int *n_neighbors; 
    /** and all neighbors */
    unsigned int **neighbors; 
} Graph;

/** @brief Define type GraphSearch
 */
typedef struct GraphSearch {
    /** a graph */
    Graph *g; 
    /** an array of distances */
    unsigned long *distances; 
    /** an array of bools indicating the visited status */
    bool *visited; 
    /** a heap keeping track of vertices that are still to visit */
    Heap *to_visit; 
    /** an array containing the predecessors of vertices. */
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

/** @brief Run the dijkstra algorithm.
 *
 * While still having vertices to visit, perform dijkstra steps.
 * @param vertex_mask mask indicating which vertices are terminals and in subgraph 
 * @param prev store predecessors in minimal tree
 * @param gs the graphsearch
 * @return the graphstructure is being updated
 */
void run_steiner_dijkstra( 
		unsigned int *vertex_mask,
		unsigned int *prev,
        GraphSearch *gs);

/** @brief Add the closest terminal to a subgraph to that subgraph
 *
 * @param g a graph
 * @param vertex_mask a mask indicating which terminals are vertices (%2 = 1) 
 *     and which vertices are contained in the subgraph (> 1)
 * @param distances the (previously calculated) distances (helper)
 * @param prev for each vertex the predecessor in a tree
 * @return the vertex_mask and the predecessors in prev are being updated
 *     modifies distances
 */
void add_closest_terminal( 
        Graph *g, 
        unsigned int *vertex_mask, 
		unsigned long *distances,
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

/** @brief Calculate a steiner tree by connecting the nearest terminal iterative
 *
 * @param g Graph in question
 * @param start startvertex
 * @param vertex_mask info about which vertices are terminals 
 * @return the list of predecessors in a (hopefully good) approximation to a steiner tree.
 *     the values in vertex_mask are being updated to indicate the subgraph with values > 1.
 */
unsigned int* steiner(
        Graph *g, 
		unsigned int start,
        unsigned int *vertex_mask);

/** @brief Calculate a steiner tree modified version
 *
 * @param g Graph in question
 * @param start startvertex
 * @param vertex_mask info about which vertices are terminals 
 * @return the list of predecessors in a (hopefully good) approximation to a steiner tree.
 *     the values in vertex_mask are being updated to indicate the subgraph with values > 1.
 */
unsigned int* steiner_modified(
        Graph *g, 
		unsigned int start,
        unsigned int *vertex_mask);

/** @brief Free the memory allocated in graph.
 *
 * @param g the graph to be freed
 */
void free_graph(
        Graph *g);

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

/** @brief Initialize a graph from file.
 *
 * @param g a graph
 * @param file Read graph from file with this filename
 * @return modifies g
 */
void init_from_graph_file(
        Graph *g, 
        char *file); 

/** @brief Read in a graph from file.
 *
 * @param g a graph
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
 * @param paths the location where to store the paths
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
 *     modifies distances
 */
void join_closest_terminal(
        unsigned long *distances, 
        int n_verts,
        unsigned int *vertex_mask,
        unsigned int *prev);

/** @brief print the edges of the tree
 *
 * @param g the underlying graph
 * @param vertex_mask the mask indicating the subgraph via values > 1
 * @param prev the list of predecessors in the tree
 */
void print_tree(
        Graph *g,
        unsigned int *vertex_mask,
        unsigned int *prev);

/** @brief Calculate the weight of a spanning tree.
 *
 * @param g the underlying graph
 * @param vertex_mask the mask indicating the subgraph via values > 1
 * @param prev the list of predecessors in the tree
 * @return the total weight
 */
unsigned long weight_of_tree(
        Graph *g,
        unsigned int *vertex_mask,
        unsigned int *prev);

/** @brief Find out the edgeweight between two vertices.
 *
 * @param g a graph
 * @param v1 (index of) one vertex
 * @param v2 (index of) another vertex
 * @param directed asking for directed edges?
 * @return weight of edge between v1 and v2
 */
unsigned long edgeweight(
        Graph *g, 
        unsigned int v1, 
        unsigned int v2,
        bool directed);

/** @brief check if the subset is a steiner tree
 * 
 * @param g a graph
 * @param terminal_mask masks terminals by 1, 0 elsewise
 * @param tree_mask masks tree terminals by values > 1, terminals by values %2 = 1
 * @param prev remembers predecessors of vertices
 * @return if input describes a connected tree which attains all terminals. 
 */
bool check_steiner(
		Graph *g,
		unsigned int *terminal_mask,
		unsigned int *tree_mask,
		unsigned int *prev);
#endif
