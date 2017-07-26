/*!  @author Michael Brand
*    @Excercise 10 - final
*    @date 26.07.2017
*
*   Algorithm to find Steiner tree on a graph where primes count as termainals.
*
*   I decided to leave all code in one file since its mainly consistent of two bigger algorithms
*   1. Dijkstra_mod
*   2. Analyzing Dijkstra output in main-function
*   I don't think it gets to complicated reading it from top to bottom.
*
*   Parallel computing 
*   Take #n random source nodes from parameter input and start the heurisitic parallely from these sources
*/


#include <iostream>
#include <fstream>
#include <stdexcept>
#include <cstdio>
#include <ctime>
#include <chrono>
#include <string>
#include <vector>
#include <climits>
#include <utility>                          // for std::pair
#include <random>

#include <boost/graph/adjacency_list.hpp>

#ifdef _OPENMP
#include <omp.h>
#endif

using namespace std;

using vertex_ = int;
using weight_ = int;

/**Edge
 * pair of ints containing the weight and
 * the vertex pointed to. This is useful
 * for the adjacency list
 */
using Edge = pair<vertex_, weight_>;

/**Graph
 * An adjacency list representing the graph.
 * graph[i] returns a list with Edge elements.
 *
 */
using Graph = vector< vector<Edge> >;


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
 *  \brief lists all primes <= numV in a vector.
 *  \param numV number to be checked.
*/
std::vector<int> getRequiredPrimes(int numV){
  std::vector<int> primes;
  for(int i=2; i<=numV; i++){
    if(isPrime(i)){
      primes.push_back(i);
    } 
  }
  return primes;
}

/*! \fn bool edgeInSteinerAlEx(Graph &stGph, vertex_ u, vertex_ v){
*   \brief Checks if Edge (u,v) already exists in SteinerGraph
*	  \param stGph Steiner Tree Graph that is checked for existing edge
*   \param u source-vertex
*   \param v dest-vertex
*/
bool edgeInSteinerAlEx(Graph &stGph, vertex_ u, vertex_ v){
	for(vector<Edge>::const_iterator ni = stGph[u].begin(); ni != stGph[u].end(); ni++){
		if(ni->first==v)
			return true;
	}
	return false;
}
/*! \fn std::vector<vertex_> dijkstra_mod(const Graph &graph, vertex_ root, vector<int> &remPrimes)
*   \brief modified dijkstra algorithm used for steiner tree problem
*   \param &graph the graph we are working on.
*   \param &steinerGraph an empty graph of size(graph) that will be used to build steinerGraph
*   \param root root-vertex
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
          //Add Path to u to SteinerTree Structure
          int lV = u;
          do{
            int preV = pre[lV];
            for(unsigned int eIndx = 0; eIndx < graph[lV].size(); eIndx++){
              if(graph[lV][eIndx].first==preV){
              	if(!(edgeInSteinerAlEx(steinerGraph, lV,preV))){
            			steinerGraph[lV].push_back(Edge(preV, graph[lV][eIndx].second));
	                steinerGraph[preV].push_back(Edge(lV, graph[lV][eIndx].second));
	                wghtLocTree += graph[lV][eIndx].second;
	                pq.insert(Edge(preV,0));
            		}
                else{
                  //abort inserting edges since they are all part of graph
                  break;
                }
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
		  	return 0;
		  } 
			visited[u] = true;
			for(vector<Edge>::const_iterator ni = gphToChk[u].begin(); ni != gphToChk[u].end(); ni++){
				v  = ni->first;
	     	wt = ni->second;
	     	pq.insert(Edge(v,wt));
	     	//Delete ṕarrallel edge from minSteinerGraph
	     	for(int k=0; k<gphToChk[v].size(); k++){
	     		if(gphToChk[v][k].first == u){
	     			gphToChk[v].erase(gphToChk[v].begin()+k);
	     		}
	     	}
			}
    }

    for(int j=1; j<gphToChk.size(); j++){
    	if(isPrime(j-1)){
    		if(!(gphToChk[j-1].empty()))
    			stTreePrimeCount++;
    	}
    }

    if(stTreePrimeCount = totPrimes){
    	return 1;
    }
    else{
    	return 0;
    }
  }
  else{
		return 0;
	}
}

/*!
*
*		INPUT n_root_nodes gives an int-number that defines the number of chosen terminals
*/
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

  /**
  * read number of vertices and edges
  */
  getline(file, line, ' ');
  const int numV = stoi(line);
  getline(file, line, '\n');
  const int numE = stoi(line);
  
  /**
  *	check if no nodes doesnt exceed vertices of graph
  */
  int n_nodes = 0;

  try{
  	if((stoi(argv[2]) > 0) && (stoi(argv[2]) <= numV))
  		n_nodes = stoi(argv[2]);
  	else{
  		fprintf(stderr, "Make sure parameter n_root_nodes is positive and does not exceed graph size.\n");
      exit(EXIT_FAILURE);
  	}
  }
  catch(...){
  	fprintf(stderr, "Call the program as: %s 'filename.gph' 'n_root_nodes' (-s)\n", argv[0]);
    exit(EXIT_FAILURE);
  }
  
  /*!
  *   FILL UP sourceVert by random numbers in range 0<->graph.size())
  */
  vector<int> sourceVert;
	std::random_device rd;     // only used once to initialise (seed) engine
	std::mt19937 rng(rd());    // random-number engine used (Mersenne-Twister in this case)
	for(int k=0; k<n_nodes; k++){
		std::uniform_int_distribution<int> uni(0,numV-1);
		sourceVert.push_back(uni(rng));
	}

  /**
  * create graph
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

  //Input Done. Start Wall Time counter
  auto   wall0 = chrono::system_clock::now(); 

  int minStVal = INT_MAX;
  Graph minSteiner(numV);
  int locStTrWght[sourceVert.size()];
  
  //Parallel section (ex10)
  #pragma omp parallel for shared(locStTrWght, minStVal, minSteiner)
  
  for(unsigned int initroot=0; initroot<sourceVert.size(); initroot++){
    //Create local SteinerGraph as emty Graph
    Graph stGraph(numV);
    //Check if initroot is in Graph-range
    if(sourceVert[initroot]>=graph.size()){
      fprintf(stderr, "Invalid source node: %i. Please make sure that root node indices are lower than graph size!\n", sourceVert[initroot]+1);
      exit(EXIT_FAILURE);
    }
    //Call Dijkstra with initroot-vertex as source
    locStTrWght[initroot] = dijkstra_mod(graph, stGraph, sourceVert[initroot]);
    
    //If return value is new min, save SteinerGraph
    if(locStTrWght[initroot]<minStVal){
    	minSteiner = stGraph;
    	minStVal = locStTrWght[initroot];
    }
  }
  
  //End Wall-Timer
  chrono::duration<double> wallDur = (chrono::system_clock::now() - wall0);
  double wallTime = wallDur.count();
  
  /*!
  *   RUN EXTRA TEST
  *   Checks if graph is a tree + if all primes are found in the tree
  *   returns 1 if successful, 0 if unsuccessful
  */
  int validate = validateStGraph(minSteiner);

  if(validate==0) fprintf(stdout, "ERROR.\nSteiner Graph is not a tree\n");
  
  /*
  *   OUTPUT
  */
  int ocount=0;
  fprintf(stdout, "TLEN:\t %i\n", minStVal);

  //Output of MinSteiner-Edges (only in manual call (-s).)
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
  double cpuTime = (clock() - cpu0) / (double) CLOCKS_PER_SEC;
  fprintf(stdout, "TIME:\t %f\n", cpuTime);
  fprintf(stdout, "WALL:\t %f\n", wallTime);
  return 0;
}