/** Felix Baumann, ex5
 *
 * The boost library is used for this program
 * and needs to be installed.
 *
 * compilation: make
 *
 * run: ./ex5 filename.gph
 *
 * doxygen: make doc
 *
 */

#include <string>
#include <iostream>
#include <fstream>
#include <stdexcept>
#include <stdio.h>
#include <cstdio>
#include <ctime>
#include <chrono>
#include <vector>
#include <utility>                          // for std::pair

//includes from boost librabry
#include <boost/config.hpp>
#include <boost/utility.hpp>                // for boost::tie
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/dijkstra_shortest_paths.hpp>
#include <boost/property_map/property_map.hpp>

//output of vertices and weights
#define OUTPUT 0

using namespace std;
using namespace boost;


int main (int argc, char* argv[]) {

  //start timers for cpu and wall time
  clock_t cpu0 = clock();
  auto   wall0 = chrono::system_clock::now();

  //check for right number of arguments
  if( argc != 2){
      cout << "Call the program as: "
           << argv[0] << " filename.gph " << endl;
      exit(EXIT_FAILURE);
  }

  ifstream    file(argv[1]);
  string      line;

  if(!file){
    cout << "Could not open file" << endl;
    return 1;
  }

  //property weighted graph
  typedef property <edge_weight_t, int> EdgeWeightProperty;
  //typedef for graph type: undirected, weighted graph
  typedef adjacency_list <listS, vecS, undirectedS, no_property, EdgeWeightProperty> Graph;
  //typedef to describe vertecies in boost library
  typedef graph_traits <Graph>::vertex_descriptor vertex_descriptor;
  //typedef for edges in the graph
  typedef pair<int, int> Edge;

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

  Edge* edges;           //array containig edge-pairs
  int*  weights;         //array containing corresponding weights
  int  edgecount = 0;

  try{
    //dynamic memory allocation
    edges   = new Edge [numE];
    weights = new int [numE];

  } catch (bad_alloc& ba) {
    fprintf(stderr, "Too many edges. try a smaller graph");
  }

  //loop over all lines in the file
  while( getline(file, line) ){

    stringstream linestream(line);
    string       vertex1, vertex2, weight;

    try{
      getline(linestream, vertex1, ' ');
      getline(linestream, vertex2, ' ');
      edges[edgecount] = Edge(stoi(vertex1), stoi(vertex2));     //store edges as pairs of vertices


      getline(linestream, weight, '\n');
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
  Graph g (edges, edges + numE, weights, numV);

  //setup for shortest path solver
  property_map<Graph, edge_weight_t>::type weightmap = get(edge_weight, g);
  std::vector<vertex_descriptor> pre(num_vertices(g));
  //vector to store distances to source vertex
  std::vector<int> dist(num_vertices(g));
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
  graph_traits < Graph >::vertex_iterator vi, vend;
  tie(vi, vend) = vertices(g);
  vi++;
  for (; vi != vend; ++vi) {

    if(dist[*vi] > maxdist){
      maxdist = dist[*vi];
      vertex  = *vi;
    }
  }

  fprintf(stdout, "RESULT VERTEX %i \nRESULT DIST %i \n", vertex, maxdist);

  double cpuTime = (clock() - cpu0) / (double) CLOCKS_PER_SEC;
  chrono::duration<double> wallDur = (chrono::system_clock::now() - wall0);
  double wallTime = wallDur.count();

  fprintf(stdout, "Finished in %f seconds [CPU Clock] and %f seconds [Wall Clock] \n", cpuTime, wallTime);


  return 0;

}
