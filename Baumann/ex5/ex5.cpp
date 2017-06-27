/** Felix Baumann, ex5
 *
 * The boost library is used for this program
 * and needs to be installed.
 *
 * compilation: make
 *
 * run: ./ex5 filename.gph mod1/mod2
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
#include <climits>
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


//***************************************typedefs for mod1
  //property weighted graph
  typedef property <edge_weight_t, int> EdgeWeightProperty;
  //typedef for graph type: undirected, weighted graph
  typedef adjacency_list <listS, vecS, undirectedS, no_property, EdgeWeightProperty> BoostGraph;
  //typedef to describe vertecies in boost library
  typedef graph_traits <BoostGraph>::vertex_descriptor vertex_descriptor;
  //typedef for edges in the graph
  typedef pair<int, int> VPair;


//***************************************typedefs for mod2
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
 *
 */
typedef vector< vector<Edge> > Graph;


void dijkstra(const Graph &graph, vertex_ root, vector<weight_> &dist) {

  /* A set helps insertion and insert/erase/find operations in logarithmic time.
   * This set maintains Edge(distance,vertex number) sorted on basis of distance
   */
  set< Edge > pq;
  set< Edge > ::iterator it;

  int u,v,wt;

  dist[root-1] = 0;
  pq.insert(Edge(root,0));

  while(pq.size() != 0){
    it = pq.begin();
    u = it->first;
    pq.erase(it);

    for(vector<Edge>::const_iterator i = graph[u-1].begin(); i != graph[u-1].end(); i++){
      v  = i->first;
      wt = i->second;

      if(dist[v-1] > dist[u-1] + wt){
        if(dist[v-1] != INT_MAX){
          pq.erase(Edge(v,dist[v-1]));
        }
        dist[v-1] = dist[u-1] + wt;
        pq.insert(Edge(v,dist[v-1]));
      }
    }
  }//while

} //dijkstra






int main (int argc, char* argv[]) {

  //start timers for cpu and wall time
  clock_t cpu0 = clock();
  auto   wall0 = chrono::system_clock::now();

  //check for right number of arguments
  if( argc != 3){
      cout << "Call the program as: "
           << argv[0] << " filename.gph mod1/mod2" << endl;
      exit(EXIT_FAILURE);
  }

  string mod = argv[2];
  bool mod1 = true;

  //choose modus
  if(mod.compare("mod1") == 0){
    fprintf(stdout, "Using data strctures and djikstra-algorithm from boost library.\n");
  } else {
    fprintf(stdout, "Using own data structures and djikstra-algorithm.\n");
    mod1 = false;
  }

  ifstream    file(argv[1]);
  string      line;

  if(!file){
    cout << "Could not open file" << endl;
    return 1;
  }

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

#if(!mod1)
  Graph graph(numV);
#endif

#if(mod1)
  VPair* edges;           //array containig edge-pairs
  int*  weights;         //array containing corresponding weights

  try{
    //dynamic memory allocation
    edges   = new VPair [numE];
    weights = new int [numE];

  } catch (bad_alloc& ba) {
    fprintf(stderr, "Too many edges. try a smaller graph");
  }
#endif

  int  edgecount = 0;
  //loop over all lines in the file
  while( getline(file, line) ){

    stringstream linestream(line);
    string       vertex1, vertex2, weight;

    try{
      getline(linestream, vertex1, ' ');
      getline(linestream, vertex2, ' ');
      getline(linestream, weight, '\n');

#if(mod1)
      edges[edgecount] = VPair(stoi(vertex1), stoi(vertex2));     //store edges as pairs of vertices
      weights[edgecount] = stoi(weight);                         //store corresponding weights

      #if OUTPUT
      fprintf(stdout, "(%i,%i) \t\t %i \n", edges[edgecount].first, edges[edgecount].second, weights[edgecount]);
      #endif
#endif

#if(!mod1)
      //add both directions, since undirected graph
      graph[stoi(vertex1)-1].push_back(Edge(stoi(vertex2), stoi(weight)));
      graph[stoi(vertex2)-1].push_back(Edge(stoi(vertex1), stoi(weight)));
#endif

    } catch (invalid_argument& ia){
      //when data is not a digit,
      //std::stoi throws an invalid argument exception
    } catch ( ... ){}

    edgecount++;

  }//while

  file.close();

  int maxdist = 0;
  int vertex  = 0;

#if(mod1)
  //create graph containig edges and weights
  BoostGraph g (edges, edges + numE, weights, numV);

  //setup for shortest path solver
  property_map<BoostGraph, edge_weight_t>::type weightmap = get(edge_weight, g);
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
  graph_traits < BoostGraph >::vertex_iterator vi, vend;
  tie(vi, vend) = vertices(g);
  vi++;
  for (; vi != vend; ++vi) {

    if(dist[*vi] > maxdist){
      maxdist = dist[*vi];
      vertex  = *vi;
    }
  }
#endif


#if(!mod1)
  //initialize distance
  vector<weight_> dist(numV, INT_MAX);

  dijkstra(graph, 1,  dist);

  for(int i = 0; i < dist.size(); ++i){
    if(dist[i] > maxdist){
      maxdist = dist[i];
      vertex  = i+1;
    }
  }
#endif

  fprintf(stdout, "RESULT VERTEX %i \nRESULT DIST %i \n", vertex, maxdist);

  double cpuTime = (clock() - cpu0) / (double) CLOCKS_PER_SEC;
  chrono::duration<double> wallDur = (chrono::system_clock::now() - wall0);
  double wallTime = wallDur.count();

  fprintf(stdout, "Finished in %f seconds [CPU Clock] and %f seconds [Wall Clock] \n", cpuTime, wallTime);


  return 0;

}
