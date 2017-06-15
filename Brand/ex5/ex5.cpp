#include <string>
#include <iostream>
#include <fstream>
#include <stdexcept>
#include <stdio.h>
#include <vector>
#include <utility>                          // for std::pair
#include <algorithm>                        // for std::for_each

//includes from boost librabry
#include <boost/config.hpp>
#include <boost/utility.hpp>                // for boost::tie
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/dijkstra_shortest_paths.hpp>
#include <boost/property_map/property_map.hpp>

#include <sys/times.h>


//output of vertices and weights

using namespace std;
using namespace boost;

clock_t times(struct tms *buffer);

void start_clock(void);
void end_clock(char *msg);

static clock_t st_time;
static clock_t en_time;
static struct tms st_cpu;
static struct tms en_cpu;

void
start_clock()
{
    st_time = times(&st_cpu);
}


void
end_clock()
{
    en_time = times(&en_cpu);

    printf("\nREAL TIME: %jd ms,\nUSER TIME %jd ms,\nSYSTEM TIME %jd ms.\n",
        (intmax_t)(en_time - st_time)*10,
        (intmax_t)(en_cpu.tms_utime - st_cpu.tms_utime)*10,
        (intmax_t)(en_cpu.tms_stime - st_cpu.tms_stime))*10;
}



int main (int argc, char* argv[]) {
  start_clock();
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
  
  Edge* edges;
  int* weights;


//  Edge edges[numE];     //array containig edge-pairs
//  int  weights[numE];   //array containing corresponding weights
  int  edgecount = 0;
  
  edges = new Edge[numE];
  weights = new int[numE];

    //loop over all lines in the file
  while( getline(file, line) ){

    stringstream linestream(line);
    string       vertex1, vertex2, weight;

    try{
      getline(linestream, vertex1, ' ');
      getline(linestream, vertex2, ' ');
      edges[edgecount] = Edge(stoi(vertex1), stoi(vertex2));     //store edges as pairs of vertices
      

      
      getline(linestream, weight, '\n');
      weights[edgecount] = stoi(weight); //store corresponding weights

      
    } catch (std::invalid_argument& ia){
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

  end_clock();
  return 0;
  
}
