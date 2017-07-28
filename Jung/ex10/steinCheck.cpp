#include "steinCheck.h"


typedef std::pair<int,int> Edge;




// depth first search from vertInd as root
bool dfs(std::vector<int>* edges, int &vertInd, bool* check, bool* alr){

	std::vector<int>::iterator it=edges[vertInd].begin();

	for(; it != edges[vertInd].end(); ++it){

		int to=*it;

		if(alr[to]){
			
			std::cout<<" here is a cycle"<<std::endl;
			return false;
		}

		alr[to]=true;
		check[to]=true;

		// remove edge
		it=edges[vertInd].erase(it)-1;
		deleteEdge(edges,to,vertInd);

		// recursion for depth first search
		if( !dfs(edges, to, check, alr)){
			return false;
		}
	}
	return true;

}


/**
* This function checks if a given Graph is a tree and if all terminals are included
*
*
*
* @param nodes: The Edges of the given graph
* @param steinTree: the nodes of the graph
* @param isPrime: The terminals of the given gaph
* @return: True if its a tree and all terminals are included false if not
**/
bool steinCheck(std::vector<Edge> &steinEdges, bool* steinTree, bool* isPrime, int vertNumb){

		bool* check=new bool[vertNumb]();	// checks if every node in the Steinertree is reached by edges

		for(int i=0;i<vertNumb; i++){
		check[i]=false;
		}

		std::vector<int>* edges= new std::vector<int>[vertNumb]();

		bool* alr=new bool[vertNumb]();		//	checks if node was already visited

		for(int i=0;i<vertNumb; i++){
			check[i]=false;
		}	


	////////////// fill in the edges

	int edgNumb= steinEdges.size();

	for(int i=0;i<edgNumb;i++){

		edges[steinEdges[i].second].push_back(steinEdges[i].first);
		edges[steinEdges[i].first].push_back(steinEdges[i].second);
	}


	check[2]=true;
	alr[2]=true;
	int start=2;
	bool noCyc=dfs(edges,start, check, alr);
	bool arr=checkArrays(steinTree, check, vertNumb);
	bool empty=isEmpty(edges, vertNumb);
	bool contains=containsArr(steinTree, isPrime, vertNumb);


	
	delete[] alr;
	delete[] edges;
	delete[] check;

	if(noCyc && arr && empty && contains){

		return true;
	}
	else{

		return false;
	}




}
