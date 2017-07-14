#include <boost/config.hpp>
#include <iostream>
#include <fstream>
#include <string>
#include <exception>
#include <vector>
#include <time.h>
#include <utility>
#include <limits>
#include <tuple>
#include <list>

#include <boost/math/special_functions/prime.hpp>
#include <boost/graph/graph_traits.hpp>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/dijkstra_shortest_paths.hpp>
#include <boost/property_map/property_map.hpp>

using namespace boost;






#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/breadth_first_search.hpp>



//typedegs for graph and edges
typedef adjacency_list < listS, vecS, undirectedS,no_property, 
	property < edge_weight_t, int > > graph_t;
typedef graph_traits < graph_t >::vertex_descriptor vertex_descriptor;
typedef std::pair<int, int> Edge;

//edge with weight
typedef std::tuple<int,int,int> Edge_wW;


// Graph Definitions
//typedef boost::adjacency_list<boost::vecS,boost::vecS,boost::directedS> Graph;
typedef boost::graph_traits<graph_t>::vertex_descriptor Vertex;
typedef boost::graph_traits<graph_t>::edge_descriptor EdgeDes;




// Visitor that throw an exception when finishing the destination vertex
class my_visitor : boost::default_bfs_visitor{
protected:
  Vertex destination_vertex_m;
public:
  my_visitor(Vertex destination_vertex_l)
    : destination_vertex_m(destination_vertex_l) {};

  void initialize_vertex(const Vertex &s, const graph_t &g) const {}
  void discover_vertex(const Vertex &s, const graph_t &g) const {}
  void examine_vertex(const Vertex &s, const graph_t &g) const {}
  void examine_edge(const EdgeDes &e, const graph_t &g) const {}
  void edge_relaxed(const EdgeDes &e, const graph_t &g) const {}
  void edge_not_relaxed(const EdgeDes &e, const graph_t &g) const {}
  void finish_vertex(const Vertex &s, const graph_t &g) const {
    if (destination_vertex_m == s)
      throw(2);
  }
};









//find the prime vertexes
int findPrimes(std::vector<int> &primes, int vertNumb){

		std::ifstream file;
		file.open("primes1.txt");
		int searchingPrimes=true;

		int primesNumb=0;

		std::string str;
		getline(file, str,'\r');


	while(searchingPrimes){

		int numsInLine=8;
		getline(file, str,'\r');

		for(int i=0;i<numsInLine;i++){
			

			
			int nextPrime = std::stoi(str.substr(10*i,10+10*i));
			
			if(nextPrime<=vertNumb){
				primes.push_back(nextPrime);
				primesNumb++;
			}
			else{
				searchingPrimes=false;
				break;
			}
			
		}
		getline(file, str,'\r');
		
	}
	file.close();

	return primesNumb;
}









// fills the edge array with all the edges from the node array
void createEdgeArray(std::vector<Edge>* nodes,Edge* edge_array,int* weights, int vertNumb ){

	int count=0;
	for(int i=0;i<vertNumb;i++){
		int size=nodes[i].size();
		for(int j=0;j<size;j++){
			Edge edg=nodes[i].at(j); 
		

			edge_array[count]=Edge(i,edg.first);
			weights[count]=edg.second;
			count++;
		}
	}
}







// reads in and initialize the edges
void initializeEdges(std::vector<Edge>* nodes,std::vector<int> primes, int vertNumb,int arcNumb,int primeNumb,std::ifstream &file){

	try {

		int count=0;


		//edges from the source node to 2 with weithg 0
		nodes[0].push_back(Edge(2,0));
		count++;


		std::string str;
		
		while (count<arcNumb-primeNumb+1) {

			getline(file, str, ' ');
			int from = std::stoi(str);

			getline(file, str, ' ');
			int to = std::stoi(str);

			getline(file,str);
			int dist=std::stoi(str);

			if(from>to){
				int help=from;
				from = to;
				to = help;
			}

			nodes[from].push_back(Edge(to,dist));	

			count++;
		}


		//Edges from all primes except 2 to the sink
		for(int i=1;i<primeNumb;i++){

			int prime=primes.at(i);
			nodes[prime].push_back(Edge(vertNumb-1,0));
		}
	}
	catch (...) {
	}
}










// removes an Edge out of nodes from vertex to parent
void removeEdge(std::vector<Edge>* nodes, int vertex, int parent){

	if(parent<vertex){
		int help=vertex;
		vertex=parent;
		parent=help;
	}



	std::vector<Edge>::iterator it=nodes[vertex].begin();

	for(it;it!=nodes[vertex].end();it++){
		if((*it).first==parent){
			nodes[vertex].erase(it);
			break;
		}
	}
}








// uses boost dijkstra and returns the distance from the graph to the new prime
int dijkstra(graph_t &g,int vertNumb, bool* steinTree, std::vector<Edge>* nodes){
	

	property_map<graph_t, edge_weight_t>::type weightmap = get(edge_weight, g);
	std::vector<vertex_descriptor> p(num_vertices(g));
	std::vector<int> d(num_vertices(g));
	vertex_descriptor s = vertex(0, g);
	my_visitor vis(vertNumb-1);


   	//clock_t tStartd1 = clock();


   	try{
		dijkstra_shortest_paths(g, s,
	                          predecessor_map(boost::make_iterator_property_map(p.begin(), get(boost::vertex_index, g))).
	                          distance_map(boost::make_iterator_property_map(d.begin(), get(boost::vertex_index, g))).
	                          visitor(vis));
	}
	catch(...){
	}
			

	//std::cout <<"INFORM TIME DIJKSTRA: "<< (double)(clock() - tStartd1) / CLOCKS_PER_SEC << " seconds" << std::endl;

	///////////// remove edges
	//clock_t rem=clock();
	int vertex=vertNumb-1;
	int parent=p[vertex];

	remove_edge(vertex,parent,g);
	removeEdge(nodes,vertex,parent);

	while(!steinTree[parent]){
	 		
	 		steinTree[parent]=true;

	 		// create a new Edge from source node to parent node with weight 0
	 		add_edge(0,parent,0,g);

	 		//removes the Edge from vertex to parent
	 		remove_edge(vertex,parent,g);
	 		removeEdge(nodes,vertex,parent);

	 		vertex=parent;
	 		parent=p[parent];
	}

	remove_edge(vertex,parent,g);
	removeEdge(nodes,vertex,parent);

	//std::cout <<"INFORM TIME REMOVE EDGES: "<< (double)(clock() - rem) / CLOCKS_PER_SEC << " seconds" << std::endl;
	//std::cout<< "RESULT Parent  "<< p[vertNumb-1]<<std::endl;

	return d[vertNumb-1];
}












// TODO is not working
void simplifyGraph(graph_t &g,std::vector<Edge>* nodes,bool* steinTree,int vertNumb){
	bool hasEdge[vertNumb]={false};

	for(int i = 1;i<vertNumb;i++){

		std::vector<Edge>::iterator it=nodes[i].begin();
		if(steinTree[i]){

			
			for(it;it!=nodes[i].end();it++){

				std::cout<<i<<std::endl;
				std::cout<<((*it).first)<<std::endl;  //This gives out some strange values 
				std::cout<<(nodes[i].size())<<std::endl;
				if(steinTree[(*it).first]){		//segmentation fault in this line
					
					std::cout<<"lalala"<<std::endl;
					remove_edge(i,(*it).first,g);
					it=nodes[i].erase(it);
					
				}
				else{
					std::cout<<"lululu"<<std::endl;
					hasEdge[(*it).first]=true;
				}
				std::cout<<"hehehehe"<<std::endl;
			}
		}
		else{

			std::cout<<"huhuhuhu"<<std::endl;
			for(it;it!=nodes[i].end();it++){
				hasEdge[(*it).first]=true;
			}
		}

		if(nodes[i].size()==0 && !hasEdge){
			std::cout<<"weweweew"<<std::endl;
			remove_vertex(i,g);
		}
	}
}




















int main(int argc, char* argv[]){


	clock_t c=clock();
	//////// read input arguments

	std::ifstream file;
	file.open(argv[1]);
		

	if (!file.is_open()) {
			std::cerr << "there is no file of the name \"" << argv[1] << "\" in the directory" << std::endl;
			return 0;
	}
	////////


	// get the Number of vertexes
  	std::string str;
	getline(file, str, ' ');
	int vertNumb = std::stoi(str)+2;


	// get the prime nodes
	std::vector<int> primes;
	int primeNumb=findPrimes(primes,vertNumb-2);


	//vector for the Edges to the nodes, first node is source last node is sink( this is needed for simplify graph)
	std::vector<Edge> nodes[vertNumb];


	// get the Number of arcs
	getline(file, str);
	int arcNumb=std::stoi(str)+primeNumb;


	//the vertexes in the steiner tree
	bool steinTree[vertNumb]={false};
	steinTree[0]=true;
	steinTree[2]=true;


	//////// create the graph
	initializeEdges(nodes, primes, vertNumb, arcNumb, primeNumb,file);

	Edge *edge_array;
	edge_array = (Edge*)malloc((arcNumb)* sizeof(Edge));

	int *weights;
	weights = (int*)malloc((arcNumb)* sizeof(int));

	createEdgeArray(nodes,edge_array,weights,vertNumb);

	graph_t g(edge_array, edge_array + arcNumb, weights, vertNumb);
	////////





	//clock_t cl[primeNumb];
	//cl[0]=clock();

	//weight of steinertree
	long long weightST=0;

	for(int i =1; i<primeNumb;i++){
		
		weightST= weightST+dijkstra(g, vertNumb, steinTree,nodes);


	/*
	 	if(i%100==0){
	 		simplifyGraph(g,nodes,steinTree,vertNumb);
	 	}

	*/

		//cl[i]=clock();
		//std::cout <<"INFORM TIME STEP: "<<  i<<"   "<< (double)(cl[i] - cl[i-1]) / CLOCKS_PER_SEC << " seconds" << std::endl;

	}


		clock_t tStart5 = clock();
		std::cout <<"INFORM WEIGHT OF STEINER TREE: "<< weightST << std::endl;
		std::cout <<"INFORM TIME: "<< (double)(tStart5 - c) / CLOCKS_PER_SEC << " seconds" << std::endl;



 	return 0;	
}