#include <string>
#include <iostream>
#include <fstream>
#include <stdexcept>
#include <stdio.h>
#include <cstdio>
#include <ctime>
#include <chrono>
#include <vector>
#include <climits>
#include <utility>                          // for std::pair
#include "algo.hh"

//includes from boost librabry
#include <boost/config.hpp>
#include <boost/utility.hpp>                // for boost::tie
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/dijkstra_shortest_paths.hpp>
#include <boost/property_map/property_map.hpp>

#define OUTPUT 0

using namespace std;
using namespace boost;

template<class F, class T>
void dijkstra(const vector<F> &graph, int root, vector<T> &dist);

/* Reads data from a gph-file
 * and computes the longest shortest path
 * to the vertex 1.
 * The boost-graph library is used for
 * the shortest path computation.
 * 
 */
pair<int,int> algo1(char* filename){
  
  ifstream    file(filename);
  string      line;
  
  if(!file){
    cout << "Could not open file" << endl;
    exit(EXIT_FAILURE);
  }
  
  //property weighted graph
  typedef property <edge_weight_t, int> EdgeWeightProperty;
  //typedef for graph type: undirected, weighted graph
  typedef adjacency_list <listS, vecS, undirectedS, no_property, EdgeWeightProperty> BoostGraph;
  //typedef to describe vertecies in boost library
  typedef graph_traits <BoostGraph>::vertex_descriptor vertex_descriptor;
  //typedef for edges in the graph
  typedef pair<int, int> VPair;
  
  //read number of vertices and edges
  getline(file, line, ' ');
  const int numV = stoi(line);
  getline(file, line, '\n');
  const int numE = stoi(line);
#if OUTPUT
  fprintf(stdout, "number of vertices: %i | number of edges: %i\n",  numV, numE);
  fprintf(stdout, "Edges \t\t Weight\n");
  fprintf(stdout, "---- \t\t ----\n");
#endif
  
  VPair* edges;          //edges as pair of vertices
  int*  weights;         //weights of an edge

  try{
    //dynamic memory allocation
    edges   = new VPair [numE];
    weights = new int [numE];

  } catch (bad_alloc& ba) {
    fprintf(stderr, "Too many edges. try a smaller graph");
  }
  
  int edgecount = 0;
  
  //loop over all lines in the file
  while( getline(file, line) ){

    stringstream linestream(line);
    string       vertex1, vertex2, weight;

    try{
      getline(linestream, vertex1, ' ');
      getline(linestream, vertex2, ' ');
      getline(linestream, weight, '\n');

      edges[edgecount] = VPair(stoi(vertex1), stoi(vertex2));    //store edges as pairs of vertices
      weights[edgecount] = stoi(weight);                         //store corresponding weights

      #if OUTPUT
      fprintf(stdout, "(%i,%i) \t\t %i \n", edges[edgecount].first, edges[edgecount].second, weights[edgecount]);
      #endif

    } catch (invalid_argument& ia){
      //when data is not a digit,
      //std::stoi throws an invalid argument exception
    } catch ( ... ){}

    edgecount++;
  }//while

  file.close();

  //create graph containig edges and weights
  BoostGraph g (edges, edges + numE, weights, numV);

  //setup for shortest path solver
  property_map<BoostGraph, edge_weight_t>::type weightmap = get(edge_weight, g);
  vector<vertex_descriptor> pre(num_vertices(g));
  //vector to store distances to source vertex
  vector<int> dist(num_vertices(g));
  //define source vertex to which shortest path shall be
  //computed from all other vertecies in graph g
  vertex_descriptor source = vertex(edges[0].first, g);

  //use dijkstra algorithm, since all edges are positive
  dijkstra_shortest_paths(g, source,
                          predecessor_map(make_iterator_property_map(pre.begin(), get(vertex_index, g))).
                          distance_map(make_iterator_property_map(dist.begin(), get(vertex_index, g))));

  delete[] edges;
  delete[] weights;
  
  int maxdist = 0;
  int vertex  = 0;

  //iterate over all vertecies and update
  //maximum disntance, if needed
  graph_traits < BoostGraph >::vertex_iterator vi, vend;
  tie(vi, vend) = vertices(g);
  vi++;
  for (; vi != vend; ++vi) {

    if(dist[*vi] > maxdist){
      maxdist = dist[*vi];
      vertex  = *vi;
    }
  }
  
  return VPair(vertex, maxdist);
  
}




/* Reads data from a gph-file
 * and computes the longest shortest path
 * to vertex with index 1.
 * This method uses a self-written
 * Dijkstra-algorithm.
 * 
 */
pair<int,int> algo2(char*filename){
  
  ifstream    file(filename);
  string      line;

  if(!file){
    cout << "Could not open file" << endl;
    exit(EXIT_FAILURE);
  }
  
  typedef int vertex_;
  typedef int weight_;

  /* Edge
   * pair of ints containing the weight and
   * the vertex pointed to. This is useful
   * for the adjacency list
   */
  typedef pair<vertex_, weight_> Edge;

  /* Graph
   * An adjacency list representing the graph.
   * graph[i] returns a list with Edge elements.
   */
  typedef vector< vector<Edge> > Graph;
  
  //read number of vertices and edges
   getline(file, line, ' ');
   const int numV = stoi(line);
   getline(file, line, '\n');
   const int numE = stoi(line);
   
 #if OUTPUT
   fprintf(stdout, "number of vertices: %i | number of edges: %i\n",  numV, numE);
   fprintf(stdout, "Edges \t\t Weight\n");
   fprintf(stdout, "---- \t\t ----\n");
 #endif


   Graph graph(numV);

   int  edgecount = 0;
   //loop over all lines in the file
   while( getline(file, line) ){

     stringstream linestream(line);
     string       vertex1, vertex2, weight;

     try{
       getline(linestream, vertex1, ' ');
       getline(linestream, vertex2, ' ');
       getline(linestream, weight, '\n');

       //add both directions, since undirected graph
       graph[stoi(vertex1)-1].push_back(Edge(stoi(vertex2)-1, stoi(weight)));
       graph[stoi(vertex2)-1].push_back(Edge(stoi(vertex1)-1, stoi(weight)));

     } catch (invalid_argument& ia){
       //when data is not a digit,
       //std::stoi throws an invalid argument exception
     } catch ( ... ){}

     edgecount++;
   }//while

   file.close();

   int maxdist = 0;
   int vertex  = 0;
  
   //initialize distance
   vector<weight_> dist(numV, INT_MAX);

   dijkstra<vector<Edge>, weight_>(graph, 0,  dist);

   for(uint i = 0; i < dist.size(); ++i){
     if(dist[i] > maxdist){
       maxdist = dist[i];
       vertex  = i+1;
     }
   }
  
   return pair<int,int>(vertex, maxdist);
  
}

template<class F, class T>
void dijkstra(const vector<F> &graph, int root, vector<T> &dist) {
  
  typedef pair<int,int> Edge;

  /* A set helps insertion and insert/erase/find operations in logarithmic time.
   * This set maintains Edge(distance,vertex number) sorted on basis of distance
   */
  set< Edge > pq;
  set< Edge > ::iterator it;

  int u,v,wt;

  dist[root] = 0;
  pq.insert(Edge(root,0));

  while(pq.size() != 0){
    it = pq.begin();
    u = it->first;
    pq.erase(it);

    for(vector<Edge>::const_iterator i = graph[u].begin(); i != graph[u].end(); i++){
      v  = i->first;
      wt = i->second;

      if(dist[v] > dist[u] + wt){
        if(dist[v] != INT_MAX){
          pq.erase(Edge(v,dist[v]));
        }
        dist[v] = dist[u] + wt;
        pq.insert(Edge(v,dist[v]));
      }
    }
  }//while

} //dijkstra