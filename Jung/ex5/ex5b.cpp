#include "ex5b.h"

using namespace boost;





void ex5b(int argc, char* argv[]){



	std::ifstream file;
	file.open(argv[1]);
		

	if (!file.is_open()) {
			std::cerr << "there is no file of the name \"" << argv[1] << "\" in the directory" << std::endl;
			return;
	}


	//typedegs for graph and edges
	typedef adjacency_list < listS, vecS, undirectedS,no_property, 
	property < edge_weight_t, int > > graph_t;
	typedef graph_traits < graph_t >::vertex_descriptor vertex_descriptor;
  	typedef std::pair<int, int> Edge;


	clock_t tStart2 = clock();
	std::string str;
	int count = 0;





	getline(file, str, ' ');
	const int vertNumb = std::stoi(str);


	getline(file, str);
	int arcNumb=std::stoi(str);

		
	Edge *edge_array;
	edge_array = (Edge*)malloc(2097152* sizeof(Edge));

	int *weights;
	weights = (int*)malloc(2097152* sizeof(int));
	


	try {

		

		while (!file.eof()) {

			getline(file, str, ' ');
			int from = std::stoi(str);

			getline(file, str, ' ');
			int to = std::stoi(str);

			getline(file,str);
			int dist=std::stoi(str);

			edge_array[count]=Edge(from,to);
			weights[count]=dist;	


			count++;
		}
	}
	catch (...) {
	
	}



	std::cout <<"INFORM TIME INITIALIZING GRAPH: "<< (double)(clock() - tStart2) / CLOCKS_PER_SEC << " seconds" << std::endl;


  	graph_t g(edge_array, edge_array + arcNumb, weights, vertNumb);
  	property_map<graph_t, edge_weight_t>::type weightmap = get(edge_weight, g);


  		
  	std::vector<vertex_descriptor> p(num_vertices(g));
  	std::vector<int> d(num_vertices(g));
  	vertex_descriptor s = vertex(1, g);




	dijkstra_shortest_paths(g, s,
                          predecessor_map(boost::make_iterator_property_map(p.begin(), get(boost::vertex_index, g))).
                          distance_map(boost::make_iterator_property_map(d.begin(), get(boost::vertex_index, g))));
  int maxdist = 0;
  int vertex  = 0;
  
  
  graph_traits < graph_t >::vertex_iterator vi, vend;
  
 
  
  for (tie(vi, vend) = vertices(g); vi != vend; ++vi) { 
	    
	    if(d[*vi] > maxdist){
	     
	      maxdist = d[*vi];
	      vertex  = *vi;
	    }
  } 


  free(edge_array);
  free(weights);

  		
	std::cout<< "RESULT VERTEX "<< vertex<<std::endl<<"RESULT DIST  "<< maxdist<<std::endl;
	
 		
}
