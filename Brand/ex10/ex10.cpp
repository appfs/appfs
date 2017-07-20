/*!  @author Michael Brand
*    @Excercise 10
*    @date 19.07.2017
*
*   Algorithm to find Steiner tree on a graph where primes count as termainals.
*
*   I decided to leave all code in one file since its mainly consistent of two bigger algorithms
*   1. Dijkstra
*   2. Analyzing Dijkstra output in main-function
*   I don't think it gets to complicated reading it from top to bottom.
*
*   Parallel computing 
*   Take different source nodes from parameter input and start the heurisitic parallely from those sources
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
#include <omp.h>

#include <boost/config.hpp>
#include <boost/utility.hpp>                // for boost::tie
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/dijkstra_shortest_paths.hpp>
#include <boost/property_map/property_map.hpp>

using namespace std;


typedef int vertex_;  /*!< defines a vertex as an int */
typedef int weight_;  /*!< defines a weight as an int */

/**Edge
 * pair of ints containing the weight and
 * the vertex pointed to. This is useful
 * for the adjacency list
 */
typedef pair<vertex_, weight_> Edge;

/**Graph
 * An adjacency list representing the graph.
 * graph[i] returns a list with Edge elements.
 *
 */
typedef vector< vector<Edge> > Graph;


/**struct pq_compare
 * compare structure for edges
 * allows me to compare two edges in a graph
 * this way I can sort edges for any vertex under consideration of their weight
 */   
struct pq_compare {
    bool operator() (const Edge i, const Edge j) const{
    return (i.second <= j.second); }
};

/*! \fn bool isPrime(int number)
    \brief checks if number is a prime.
    \param number number to be checked.
*/
bool isPrime(int number){
    if(number < 2) return false;
    if(number == 2) return true;
    if(number % 2 == 0) return false;
    for(int i=3; (i*i)<=number; i+=2){
        if(number % i == 0 ) return false;
    }
    return true;

}

/*! \fn std::vector<int> getRequiredPrimes(int numV)
    \brief lists all primes <= numV in a vector.
    \param numV number to be checked.
    \return vector of primes
*/
std::vector<int> getRequiredPrimes(int numV){
  //fprintf(stdout, "Primes: [");
  std::vector<int> primes;
  for(int i=2; i<=numV; i++){
    if(isPrime(i)){
      //fprintf(stdout, "%i ", i);
      primes.push_back(i);
    } 
  }
  //fprintf(stdout, "]\n");
  return primes;
}

/*! \fn std::vector<vertex_> dijkstra_mod(const Graph &graph, vertex_ root, vector<int> &remPrimes)
*   \brief modified dijkstra algorithm used for steiner tree problem
*   \param &graph the graph we are working on.
*   \param &steinerGraph an empty graph of size(graph) that will be used to build steinerGraph
*   \param root root vertex
*   \return objective Value of SteinerTree
*/
int dijkstra_mod(const Graph &graph, Graph &steinerGraph, vertex_ root) {
  int wghtLocTree = 0; /*!< return value */
  std::vector<int> remPrimes = getRequiredPrimes(graph.size());
  std::vector<weight_> dist(graph.size(), INT_MAX);
  /* A set helps insertion and insert/erase/find operations in logarithmic time.
   * This set maintains Edge(distance,vertex number) sorted on basis of distance
   */
  set< Edge , pq_compare> pq;
  set< Edge , pq_compare > ::iterator it;

  vector<vertex_> pre(graph.size(), (-1)); /*!< vevtor of predecessors */
  int u,v,wt;

  dist[root] = 0;
  pq.insert(Edge(root,0));
  while(pq.size() != 0){
    if(!(remPrimes.size()>0)) break;
    bool pFound = false;
    it = pq.begin();
    u = it->first;
    pq.erase(it);
    
    if((u != root) && (isPrime(u+1))){
      for(unsigned int chk=0; chk<remPrimes.size(); chk++){
        if((remPrimes[chk])==(u+1)){
          //NEW PRIME FOUND
          pFound = true;
          //delete u from remainingPrimes
          remPrimes.erase(remPrimes.begin()+chk);
          pq.insert(Edge(u,0));
          dist[u]=0;
          //Add Edges to u to SteinerTree Structure
          int lV = u;
          do{
            int preV = pre[lV];
            for(unsigned int eIndx = 0; eIndx < graph[lV].size(); eIndx++){
              if(graph[lV][eIndx].first==preV){
                steinerGraph[lV].push_back(Edge(preV, graph[lV][eIndx].second));
                steinerGraph[preV].push_back(Edge(lV, graph[lV][eIndx].second));
                wghtLocTree += graph[lV][eIndx].second;
              }
            }
            lV = pre[lV];
          }while(dist[lV] != 0);
          break;
        }
      }
    }
    if(pFound){
      continue;
    } 
    for(vector<Edge>::const_iterator ni = graph[u].begin(); ni != graph[u].end(); ni++){
      v  = ni->first;
      wt = ni->second;
      if(dist[v] > dist[u] + wt){
        pre[v] = u;
        if(dist[v] != INT_MAX){
          pq.erase(Edge(v,dist[v]));
        }
        dist[v] = dist[u] + wt;
        pq.insert(Edge(v,dist[v]));
      }
    
    }
  }
  return wghtLocTree;
}

/*! \fn int validateStGraph(&gphToChk)
*   \brief tests if gphToChk is connected and inherits all primes
*   \param &grphToChk minimal SteinerTree found
*   \return 1 if successful, 0 if unsuccessful
*/
int validateStGraph(Graph &gphToChk){

	std::vector<int> remPrimes = getRequiredPrimes(gphToChk.size());
	int totPrimes = remPrimes.size();
	int stTreePrimeCount = 0;

	bool visited[gphToChk.size()];
	for(int i=0; i<gphToChk.size(); i++)
		visited[i]=false;

	

	//start at vertex 2
	if(!(gphToChk[1].empty())){

		set< Edge , pq_compare> pq;
		set< Edge , pq_compare > ::iterator it;

		int u,v,wt;

		pq.insert(Edge(1,0));

		while(pq.size() != 0){
	    it = pq.begin();
		  u = it->first;
		  pq.erase(it);
		  if(visited[u]){
		  	//fprintf(stderr, "vertex %i already visited.\n", u+1);
		  	return 0;
		  } 
			visited[u] = true;
			for(vector<Edge>::const_iterator ni = gphToChk[u].begin(); ni != gphToChk[u].end(); ni++){
				v  = ni->first;
	     	wt = ni->second;
	     	//fprintf(stdout, "Insert edge (%i,%i)\n", u+1,v+1);
	     	pq.insert(Edge(v,wt));
	     	//Delete ṕarrallel edge from minSteinerGraph
	     	for(int k=0; k<gphToChk[v].size(); k++){
	     		if(gphToChk[v][k].first == u){
	     			//fprintf(stdout, "Delete edge (%i,%i) from Graph\n", v+1,u+1);
	     			gphToChk[v].erase(gphToChk[v].begin()+k);
	     		}
	     	}
			}
    }

    //fprintf(stderr, "stGraph is Tree.\n");

    for(int j=1; j<gphToChk.size(); j++){
    	if(isPrime(j-1)){
    		if(!(gphToChk[j-1].empty()))
    			stTreePrimeCount++;
    	}
    }

    if(stTreePrimeCount = totPrimes){
    	//fprintf(stderr, "stGraph inherits all primes.\n");
    	return 1;
    }
    else{
    	//fprintf(stderr, "too little prime no.\n");
    	return 0;
    }
  }
  else{
		return 0;
	}
}


int main (int argc, char* argv[]) {
  /**
  * start timers for cpu and wall time
  */
  clock_t cpu0 = clock();
  bool output = false;

  /*
  *   Check input arguments
  */
  if( argc < 3){
      fprintf(stderr, "Call the program as: %s 'filename.gph' 'n_root_nodes' (-s)\n", argv[0]);
      exit(EXIT_FAILURE);
  }
  if(std::string(argv[argc-1]) == "-s"){
  	output=true;
  }

  ifstream    file(argv[1]);
  string      line;

  if(!file){
    fprintf(stderr, "Could not open file.\n");
    return -1;
  }

  /*!
  * Check input nodes and save in array
  */
  if(output) argc--;
  std::vector<int> sourceVert;
  for(int i=2; i<argc; i++){
  	try{
  		if(stoi(argv[i]) > 0){
      	sourceVert.push_back(stoi(argv[i])-1);
   		}
    	else{
      	fprintf(stderr, "Invalid source node. Please make sure that root node indices match ((>=1) && <=graph.size())\n");
      	exit(EXIT_FAILURE);
    	}
  	}
    catch(...){
    	fprintf(stderr, "Call the program as: %s 'filename.gph' 'n_root_nodes' (-s)\n", argv[0]);
      exit(EXIT_FAILURE);
    }

  }

  /**
  * read number of vertices and edges
  */
  
  getline(file, line, ' ');
  const int numV = stoi(line);
  getline(file, line, '\n');
  const int numE = stoi(line);
  
  /**
  * create graph and steiner subgraph structures
  */
  Graph graph(numV);
  /**
  * read ín given gph file
  */
  while( getline(file, line) ){
    stringstream linestream(line);
    string       vertex1, vertex2, weight;

    try{
      getline(linestream, vertex1, ' ');
      getline(linestream, vertex2, ' ');
      getline(linestream, weight, '\n');

      /**
      * add both directions of edge to the graph since undirected
      * index switch applies: 1 --> 0, 1 --> 2, etc.
      */
      graph[stoi(vertex1)-1].push_back(Edge(stoi(vertex2)-1, stoi(weight)));
      graph[stoi(vertex2)-1].push_back(Edge(stoi(vertex1)-1, stoi(weight)));
    }catch (...){
    	fprintf(stderr, "Corrupt data in .gph file.");
      exit(EXIT_FAILURE);
    }

  }
  file.close();

  int minStVal = INT_MAX;
  Graph minSteiner(numV);
  //int minSrcNode;

  auto   wall0 = chrono::system_clock::now(); 
  
  int locStTrWght[sourceVert.size()];
  
  #pragma omp parallel for shared(locStTrWght, minStVal, minSteiner)
  for(unsigned int initroot=0; initroot<sourceVert.size(); initroot++){
    Graph stGraph(numV);
    if(sourceVert[initroot]>=graph.size()){
      fprintf(stderr, "Invalid source node: %i. Please make sure that root node indices are lower than graph size!\n", sourceVert[initroot]+1);
      exit(EXIT_FAILURE);
    }
    locStTrWght[initroot] = dijkstra_mod(graph, stGraph, sourceVert[initroot]);
    
    if(locStTrWght[initroot]<minStVal){
    	minSteiner = stGraph;
    	minStVal = locStTrWght[initroot];
    	//minSrcNode = sourceVert[initroot]+1;
    }
  }

  double cpuTime = (clock() - cpu0) / (double) CLOCKS_PER_SEC;
  chrono::duration<double> wallDur = (chrono::system_clock::now() - wall0);
  double wallTime = wallDur.count();

  /*
  *   OUTPUT
  */
  int ocount=0;
  fprintf(stdout, "TLEN:\t %i\n", minStVal);
  if(output){
  	fprintf(stdout, "TREE:\t");
  	for(unsigned int i=0; i<minSteiner.size(); i++){
  		if(!(minSteiner[i].empty())){
  			for(unsigned int j=0; j<minSteiner[i].size(); j++){
  				fprintf(stdout, "(%i,%i), ", i+1, minSteiner[i][j].first+1);
  				ocount++;
  				if((ocount%10)==0) fprintf(stdout, "\n\t");
  			}
  		}
  	}
  	fprintf(stdout, "\n");
  }
  fprintf(stdout, "TIME:\t %f\n", cpuTime);
  fprintf(stdout, "WALL:\t %f\n", wallTime);

  /*!
  *		RUN TEST
  *		returns 1 if successful, 0 if unsuccessful
  */
  int validate = validateStGraph(minSteiner);

  if(validate>0) fprintf(stdout, "Test successful\n");
  else fprintf(stdout, "Test unsuccessful\n");
  return 0;
}

