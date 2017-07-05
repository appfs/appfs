#include <string>
#include <iostream>
#include <fstream>
#include <stdexcept>
#include <stdio.h>
#include <vector>
#include <utility>                          
#include <algorithm>                      
#include <sys/times.h>

//includes from boost librabry
#include <boost/config.hpp>
#include <boost/utility.hpp>                
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/dijkstra_shortest_paths.hpp>
#include <boost/property_map/property_map.hpp>

using namespace std;
using namespace boost;

/** ex5 with timing.  
 *   
 *  @author Michael Brand  
 *  @date 20.06.2017 
 * 
 *  @version 2.0 
 *  Merged ex5_old and ex5_boost 
 */


int maxEdgeWeight = 2e9; //*SETUP VARIABLES

/**
*   TIME MEASUREMENT FUNCTIONS
*/
clock_t times(struct tms *buffer);

void start_clock(void);
void end_clock(void);

static clock_t st_time;
static clock_t en_time;
static struct tms st_cpu;
static struct tms en_cpu;

/** Function for time measurement
*   Measuring the time
*/
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

/**
* STRUCTURE FOR OWN IMPLEMENTATION
*/



//* a structure to represent a weighted edge in graph
struct Edge
{
    int src, dest, weight;
};
//* a structure to represent a connected, directed and weighted graph
struct Graph
{
    // V-> Number of vertices, E-> Number of edges
    int V, E;
    // graph is represented as an array of edges.
    struct Edge* edge;
};
 
//* Creates a graph with V vertices and E edges
struct Graph* createGraph(int V, int E)
{
    struct Graph* graph = (struct Graph*) malloc( sizeof(struct Graph) );
    graph->V = V;
    graph->E = E;
 
    graph->edge = 
       (struct Edge*) malloc( graph->E * sizeof( struct Edge ) );
 
    return graph;
}

//ALGOS

/** gets longest path value.  
   *  @param[in] dist[] int array of edges. 
   *  @param[in] n no. vertices.  
   *  @return weight of longest path    
   */
int getLongestPathValue(int dist[], int n){
  
  int high = 0;

  for(int i=0; i<n; i++){
    if(dist[i]>high && dist[i]<=maxEdgeWeight) high = dist[i];
  }

  return high;
}

/** gets vertices with longest distance to source.  
   *  @param[in] dist[] int array of edges. 
   *  @param[in] n no. vertices.
   *  @param[in] val longest path value in graph
   *  @return vector of vertices which have given path value in common    
   */
std::vector<int> getLongestPathVertices(int dist[], int n, int val){
  std::vector<int> v;
  for(int i=0; i<n; i++){
    if(dist[i] == val)
      v.push_back(i);
  }
  return v;
}

/** gets vertex of longest way.  
   *  @param[in] vertices possible vertices 
   *  @param[in] ecounter no. of edges to source
   *  @return vertice with more edges or smaller index if even    
   */
int chooseWinner(std::vector<int> vertices,int ecounter[]){

  int winner = vertices[0];

  for(int i=1; i<vertices.size(); i++){
    if(ecounter[vertices[i]] > ecounter[winner]){
      winner = vertices[i];
    } 
  }
  return winner;
}


/** BellmanFord main algo  
   *  @param[in] graph graph on which BF is executed 
   *  @param[in] src source node
   */
void BellmanFord(struct Graph* graph, int src)
{
    int V = graph->V;
    int E = graph->E;
    int dist[V];
    int ecount[V];
 
    // Initialize distances from src to all other vertices as INFINITE
    for(int i = 0; i < V; i++){
        dist[i]   = INT_MAX;
        ecount[i] = 0;
    }

    dist[src] = 0;
 
    // Relax all edges |V| - 1 times. A simple shortest 
    // path from src to any other vertex can have at-most |V| - 1 
    // edges
    for(int i = 1; i <= V-1; i++)
    {
        for (int j = 0; j < E; j++)
        {
            int u = graph->edge[j].src;
            int v = graph->edge[j].dest;
            int weight = graph->edge[j].weight;
            if (dist[u] != INT_MAX && dist[u] + weight < dist[v]){
                dist[v] = dist[u] + weight;
                ecount[v] = ecount[u]+1;
            }

        }
    }
 
    // check for negative-weight cycles.  The above step 
    // guarantees shortest distances if graph doesn't contain 
    // negative weight cycle.  If we get a shorter path, then there
    // is a cycle.
    for (int i = 0; i < E; i++)
    {
        int u = graph->edge[i].src;
        int v = graph->edge[i].dest;
        int weight = graph->edge[i].weight;
        if (dist[u] != INT_MAX && dist[u] + weight < dist[v])
            fprintf(stderr, "%s", "Graph contains negative weight cycle");
    }
 
  int lPathVal = getLongestPathValue(dist,V);

  std::vector<int> vert = getLongestPathVertices(dist, V, lPathVal);

  int win = chooseWinner(vert, ecount);

  fprintf(stdout, "NON-BOOST mode\nRESULT VERTEX %i \nRESULT DIST %i \n", win+1, dist[win]);
 
    return;
}


/** Main Method
*   runrunrunrunrun
*
*/
int main (int argc, char* argv[]) {
    
  start_clock(); //*start timer

  //check for right number of arguments
  if( argc != 3){
      fprintf(stderr, "%s", "Call programm as ex5 mode filename.gph, where mode is integer 1(boost) or 2(non-boost)\n");
      exit(EXIT_FAILURE);
  }
  
  int mode = stoi(argv[1]);

  if(mode!=1 && mode!=2){
    fprintf(stderr, "%s", "mode argument needs to be 1(for boost) or 2(non-boost)\n");
    exit(EXIT_FAILURE);
  }

  if(mode==1){
  //Boost
  ifstream    file(argv[2]);
  string      line;

  if(!file){
    fprintf(stderr, "%s", "Could not open file.\n");
    exit(EXIT_FAILURE);

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
  
  //*create graph containig edges and weights
  Graph g (edges, edges + numE, weights, numV);
  //*setup for shortest path solver
  property_map<Graph, edge_weight_t>::type weightmap = get(edge_weight, g);
  std::vector<vertex_descriptor> pre(num_vertices(g));
  //*vector to store distances to source vertex
  std::vector<int> dist(num_vertices(g));
  //*define source vertex to which shortest path shall be
  //*computed from all other vertecies in graph g
  vertex_descriptor source = vertex(edges[0].first, g);
  
  //*use dijkstra algorithm, since all edges are positive
  dijkstra_shortest_paths(g, source,
                          predecessor_map(make_iterator_property_map(pre.begin(), get(vertex_index, g))).
                          distance_map(make_iterator_property_map(dist.begin(), get(vertex_index, g))));
  int maxdist = 0;
  int vertex  = 0;
  
  //*iterate over all vertecies and update
  //*maximum disntance, if needed
  graph_traits < Graph >::vertex_iterator vi, vend;
  tie(vi, vend) = vertices(g);
  vi++;
  for (; vi != vend; ++vi) { 
    
    if(dist[*vi] > maxdist){
      maxdist = dist[*vi];
      vertex  = *vi;
    }
  }
  
  fprintf(stdout, "BOOST mode\nRESULT VERTEX %i \nRESULT DIST %i \n", vertex, maxdist);

  end_clock();
  return 0;

  }


  //OWN BELLMANFORD FROM COMA
  else if(mode==2){
    //nonboost

  //Find shortest path to vertex of index...
  int src=1;

  string line;
  
  int err=0;
  
  int lcout = 0;
  int ecount = 0;
  int edgesTot;
  int vertTot;

  struct Graph* graph;

    ifstream f (argv[2]);
  
    //* Read input file and create graph

  while(getline(f, line)) {

    lcout++;

    stringstream buffer(line);
    
    int start = 0;
    int end = 0;
    int weight = 0;
    
    
    buffer >> start >> end >> weight;
    
    //FROM EX1
    if(lcout==1){
      //std::cout << "Graph setup" << std::endl;
        graph = createGraph(start, 2*end);
        //fprintf(stdout, "%s %d %s %d %s\n", "Creating undirected Graph with ",start," nodes, ",end," edges.");
        edgesTot = end;
        vertTot = start;
    }
    else{

      if(start < 1 || start>vertTot || end < 1 || end > vertTot){
        fprintf(stderr, "%s \t %d\n", "errorline: " , lcout);
        err++;
        continue;
      }

      if((weight <= 0 || weight>=2000000000 ) && lcout>1 ){
        fprintf(stderr, "%s \t %d\n", "errorline: " , lcout);
        err++;
        continue;
    }

      if(ecount>=2*edgesTot){
        cout << "too many edges " << buffer.str() << endl;
        err++;
        continue;
      } 
    // INDEX SWITCH 1->0,...   
      graph->edge[ecount].src = int(start-1);
      graph->edge[ecount].dest = int(end-1);
      graph->edge[ecount].weight = weight;

      ecount++;

      graph->edge[ecount].src = int(end-1);
      graph->edge[ecount].dest = int(start-1);
      graph->edge[ecount].weight = weight;

      ecount++;
    }
    
  }

  f.close();

  if(err>0) fprintf(stderr, "%s \t\t %d\n", "ERRORS" , err);

  if(ecount < 2*edgesTot){
     fprintf(stderr, "%s \t\t %s\n", "INFO", "NOT ENOUGH EDGES DEFINED");
     return -1;
  }

// BELLMAN FORD FROM COMA

  BellmanFord(graph, src-1);
  
  end_clock(); //* end timing and print stats

  return 0;
  }

}
