/* Bellman-Ford's shortest path algorithm on defined input
INPUT FORMAT
vertices 	edges
from to weight
from to weight
.
.
.
*/

/*
The input file starts with node 1 whereas the algo works with start vertex 0
We convert the vertex indices and transform back in the end
*/
 

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <math.h>
#include <vector>

using namespace std;

// a structure to represent a weighted edge in graph
struct Edge
{
    int src, dest, weight;
};
 
// a structure to represent a connected, directed and weighted graph
struct Graph
{
    // V-> Number of vertices, E-> Number of edges
    int V, E;
 
    // graph is represented as an array of edges.
    struct Edge* edge;
};
 
// Creates a graph with V vertices and E edges
struct Graph* createGraph(int V, int E)
{
    struct Graph* graph = 
         (struct Graph*) malloc( sizeof(struct Graph) );
    graph->V = V;
    graph->E = E;
 
    graph->edge = 
       (struct Edge*) malloc( graph->E * sizeof( struct Edge ) );
 
    return graph;
}
 
// A utility function used to print the solution
void printArr(int dist[], int n, int ecount[])
{
    fprintf(stdout, "%s", "Vertex   Distance from Source     No edges to src\n");
    for (int i = 0; i < n; ++i)
    	fprintf(stdout, "%d \t\t %d \t\t %d\n", i+1, dist[i], ecount[i]);
        
}

// Returns value of shortest path (weight)
int getShortestPathValue(int dist[], int n){
	
	int low = INT_MAX;

	for(int i=0; i<n; i++){
		if(dist[i]<low && dist[i]>0) low = dist[i];
	}

	return low;
}

// Returns value of longest path (weight)
int getLongestPathValue(int dist[], int n){
	
	int high = 0;

	for(int i=0; i<n; i++){
		if(dist[i]>high && dist[i]<=2000000000) high = dist[i];
	}

	return high;
}

// Returns vertices which have shortest path (weight) in common
std::vector<int> getShortestPathVertices(int dist[], int n, int val){
	std::vector<int> v;
	for(int i=0; i<n; i++){
		if(dist[i] == val)
			v.push_back(i);
	}
	return v;
}

// Returns vertices which have longest path (weight) in common
std::vector<int> getLongestPathVertices(int dist[], int n, int val){
	std::vector<int> v;
	for(int i=0; i<n; i++){
		if(dist[i] == val)
			v.push_back(i);
	}
	return v;
}

//Returns vertex among shortestPathVertices that has the longest way (#edges)
int chooseWinner(std::vector<int> vertices,int ecounter[]){

	int winner = vertices[0];

	for(int i=1; i<vertices.size(); i++){
		if(ecounter[vertices[i]] > ecounter[winner]){
			winner = vertices[i];
		} 
	}
	return winner;
}


// The main function that finds shortest distances from src to
// all other vertices using Bellman-Ford algorithm.  The function
// also detects negative weight cycle
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
 
    // Step 2: Relax all edges |V| - 1 times. A simple shortest 
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
                ecount[v] = ecount[u]+1; //vcount[v]++?
            }

        }
    }
 
    // Step 3: check for negative-weight cycles.  The above step 
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
 
 	//get vertices with shortest connection
 	
 	//int sPathVal = getShortestPathValue(dist,V);
 	int lPathVal = getLongestPathValue(dist,V);

 	//std::vector<int> vert = getShortestPathVertices(dist, V, sPathVal);
 	std::vector<int> vert = getLongestPathVertices(dist, V, lPathVal);

 	//int win = chooseMinWinner(vert, ecount);
	int win = chooseWinner(vert, ecount);


 	fprintf(stdout, "%s \t %s \t %d\n", "RESULT", "VERTEX", win+1);
 	fprintf(stdout, "%s \t %s \t\t %d\n", "RESULT", "DIST", dist[win]);
 	
    //printArr(dist, V, ecount);
 
    return;
}


int main (int argc, char* argv[]) {

	//Find shortest path to vertex of index...
	int src=1;

	string line;
	
	int err=0;
	
	int lcout = 0;
	int ecount = 0;
	int edgesTot;
	int vertTot;

	struct Graph* graph;

    ifstream f (argv[1]);
	
    //* Read input file and create graph

	while(getline(f, line)) {

		lcout++;

		stringstream buffer(line);
		
		int start = 0;
		int end = 0;
		int weight = 0;
		
		
		buffer >> start >> end >> weight;

		
		

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

	BellmanFord(graph, src-1);
	 
    return 0;
}
