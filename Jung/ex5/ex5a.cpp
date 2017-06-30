
#include "ex5a.h"



/**
* @brief finds the vertex which is unused and have the smallest distance in the current dijsktra step
* @param vert the vector of the vertexes
* returns the iterator of the unused vertex with the smallest distance in the vector
*/

std::vector<vertex>::iterator unusedMin(std::vector<vertex>& vert) {				//returns the unused vertex with minimum distance, if everyone is used returns vert.begin()
	
	std::vector<vertex>::iterator it=vert.begin();
	std::vector<vertex>::iterator i=vert.begin();
	bool is = false;

	while (!is&&i!=vert.end()) {			//searches the first unused vertex
		if (!((*i).getUsed())) {
			it = i;
			is = true;
		}
		i++;
	}

	if ((*it).getUsed()) {			//returns it when every vertex is used
		return it;
	}

	for (i; i != vert.end(); i++) {				//finds the minimum distance unused vertex
		if (!(*i).getUsed() && (*i).getDist() < (*it).getDist()) {
			it = i;
		}
	}
	return it;
}

/**
* @brief finds the maximum distance and maximum weight
* @param vert the vector of the vertexes
* returns the iterator of the unused vertex with the smallest distance in the vector
*/

void findMax(std::vector<vertex>& vert, int& maxD, int& maxP, int& maxW) {
	std::vector<vertex>::iterator it;


	for (it = vert.begin() + 1; it != vert.end(); it++) {				//go through vertexes
		if ((*it).getDist() > maxD) {
			maxD = (*it).getDist();
			maxP = it - vert.begin();
		}

		std::vector<std::pair<int, int>> edges = (*it).getEdges();
		std::vector<std::pair<int,int>>::iterator i;
		for (i = edges.begin(); i != edges.end(); i++) {			//go through edges connecetd with the vertex
			if ((*i).first == (*it).getPrev() && (*i).second > maxW) {
				maxW = (*i).second;
			}
		}

	}
}


/**
* @brief dijkstra algorithm to find the longest shortest path to vertex 1
* @param vert the vector of the vertexes
*/

void dijkstra(std::vector<vertex>& vert) {
	
	vert.at(0).setDist(0);

	std::vector<vertex>::iterator it=vert.begin();
	
	while (!(*it).getUsed()) {
		
		(*it).setUsed(true);										//label current vertex as used

		int pos = it - vert.begin();								//position of the current vertex
		std::vector<std::pair<int, int>> edges = (*it).getEdges();
		std::vector<std::pair<int, int>>::iterator itEdg;


		for (itEdg = edges.begin(); itEdg != edges.end(); itEdg++) {	//iterate through neighbors
			std::vector<vertex>::iterator neighb = vert.begin() + (*itEdg).first;				//set iterator to the neighbor
			if (!(*neighb).getUsed()) {
				long newDist = (*itEdg).second + (*it).getDist();


				if (newDist < (*neighb).getDist()) {											//set new distance if its smaller and change the prev to the current
					(*neighb).setDist(newDist);
					(*neighb).setPrev(pos);

				}
				
			}
		}
			it = unusedMin(vert);
		
	}

}


/**
* @brief reads in the file and creates a graph
* @param file the input file to read from
* @param vert vector where the graphs vertexes will be stored
*/
void initialEdges(std::ifstream& file, std::vector<vertex>& vert) {
	clock_t tStart2 = clock();
	std::string str;
	try {
		
		while (!file.eof()) {

			getline(file, str, ' ');
			int from = std::stoi(str);
			getline(file, str, ' ');
			int to = std::stoi(str);
			getline(file, str);
			int dist = std::stoi(str);

			vert.at(from - 1).setEdge(to - 1, dist);
			vert.at(to - 1).setEdge(from - 1, dist);

		}

	}
	catch (...) {
		//std::cout << "something wrong" << std::endl;
	}
	std::cout <<"INFORM TIME INITIALIZING GRAPH: "<< (double)(clock() - tStart2) / CLOCKS_PER_SEC << " seconds" << std::endl;

}

/**
* @brief find the longest shortest path from a graph to vertex 1 using the dijkstra algo
* @param argv the name of the input file to read from 
* prints out the node with the longest shortest path to vertex 1 as "RESULT NODE"
* prints out the distance of that node to node 1 as "RESULT TIME"
* prints out the maximum weight of the edges which are in a shortest path from any node to node 1
* prints out the time is needed for reading and for the whole program
*/

void ex5a(int argc, char* argv[]) {
	try {


		std::string str;
		std::ifstream file;
		file.open(argv[1]);
		

		if (!file.is_open()) {
			std::cerr << "there is no file of the name \"" << argv[1] << "\" in the directory" << std::endl;
			return ;
		}

		//TODO is the file valid??
		
		int maxWeight=0;
		int maxDist = 0;
		int maxPos = 0;

		getline(file, str, ' ');
		int vertNumb = std::stoi(str);
		std::vector<vertex> vert(vertNumb);
		getline(file, str);


		
		initialEdges(file, vert);
		dijkstra(vert);
		findMax(vert, maxDist, maxPos, maxWeight);

		std::cout << "RESULT NODE " << maxPos+1 << std::endl;
		std::cout <<"RESULT DIST "<< maxDist << std::endl;
		std::cout <<"RESULT MARC "<< maxWeight << std::endl;
		

		return ;


	}
	catch (std::exception& e) {

		std::cerr<<"error:  " << e.what() << std::endl;
	}
}
