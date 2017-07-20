#include "findSteiner.h"


// This removes every Edge which is between two steiner nodes (actually it doesnt improve the time that much, maybe for dense graphs, for rand14-5 from 18 to 15 secs)
void simplifyGraph(std::vector<Edge_wW>* nodes,bool* steinTree,int vertNumb){

	for(int i = 1;i<vertNumb;i++){

		std::vector<Edge_wW>::iterator it=nodes[i].begin();
		if(steinTree[i]){

			int j=0;
			for(;it!=nodes[i].end();it++){

				if(steinTree[(*it).first]){		
					
					nodes[i].erase(it);
					it=nodes[i].begin()+j-1;
				}
				else{

					j++;
				}
			}
		}
	}
}



/**
* 
* updates the Edges and nodes of the Steiner Tree
*/
void updateSteiner(std::vector<Edge_wW>* nodes, bool* steinTree, std::vector<int> &steinerNodes, int* pre, int* dijPre, int vertNumb, std::vector<Edge_wW> &steinEdges){

	int vertex=vertNumb-1;
	int parent=dijPre[vertex];


	///////// remove Edge from prime to sink
	nodes[parent].pop_back();

	std::vector<Edge_wW>::iterator it, end;
	it=nodes[vertNumb-1].begin();
	end=nodes[vertNumb-1].end();
	
	for(;it!=end;it++){				//TODO this can be improved

		if((*it).first==parent){
			nodes[vertNumb-1].erase(it);
		}
	}


	///////// puts in the new nodes and edges to the steiner tree and updates the predecessor map
	while(!steinTree[parent]){
	 		
	 		steinerNodes.push_back(parent);
	 		steinTree[parent]=true;


	 		// create a new Edge from source node to parent node with weight 0
	 		nodes[0].push_back(Edge_wW(parent,0));
	 		nodes[parent].push_back(Edge_wW(0,0));
	 		


	 		pre[vertex]=parent;
	 		vertex=parent;
	 		parent=dijPre[parent];
	 		steinEdges.push_back(Edge_wW(vertex,parent));
	}

	pre[vertex]=parent;
}


/*
* copy an array copyFrom to an array copyTo, both arrays have length arrSize
*/ 
void copyArray(bool* copyTo, bool* copyFrom, int arrSize){
	for(int i=0; i<arrSize; i++){
		copyTo[i]=copyFrom[i];
	}
}


/*
* determines i^2 with (i-1)^2 < number <= i*2
*/
int nextPower(int number){

	int i = 0;
	while(true){

		if(pow(2,i)<number && number<=pow(2,i+1)){
			return pow(2,i+1);
		}
		i++;
	}
}



/*
*
*
* Steiner Tree Heuristic from Takahashi, Matsuyama
*
* writes the graph to the steinEdges and steinTree
*
* @param nodes: The Edges corresponding to the nodes
* @param vertNumb: number of verxes in the graph
* @param primeNumb: number of terminals
* @param steinerEdges: Edges-vector to write the steiner Edges in
* @param steinTree: The nodes in the Steiner Tree
* @param  start: The terminal to start with
*
* @return: The weight(length) of the Tree
*/

long long findSteiner(std::vector<Edge_wW> nodes[],int vertNumb, int primeNumb, std::vector<Edge_wW> &steinEdges, bool* steinTree, int start){



	// remove edge from start to the sink
	nodes[start].pop_back();

	// add node from 0 to start node
	nodes[0].push_back(Edge_wW(start,0));
	nodes[start].push_back(Edge_wW(0,0));

	
	// remove the edge from the start terminal to the sink
	std::vector<Edge_wW>::iterator it= nodes[vertNumb-1].begin();
	bool search=true;
	while(search){

		if((*it).first==start){
			nodes[vertNumb-1].erase(it);
			search=false;
		}
		it++;
	}



	// add the start node to the steiner Tree
	steinTree[start]=true;
	
	#if BIG
		int* pre=new int[vertNumb]();		// the predecessor map of steiner Tree
		int* dijPre=new int[vertNumb]();	// the predecessor map after every dijkstra
		bool* used= new bool[vertNumb]();
	#else
		int pre[vertNumb];		// the predecessor map of steiner Tree
		int dijPre[vertNumb];		// the predecessor map after every dijkstra
		bool used[vertNumb];
	#endif

	std::vector<int> steinerNodes;

	steinerNodes.push_back(0);
	steinerNodes.push_back(start);

	

	//calculates the heapLeangth , vertNumb+1 because the zero entry
	int heapLength=nextPower(vertNumb+1);		

	long long weightST=0;
	
	for(int i =1; i<primeNumb;i++){

		copyArray(used,steinTree,vertNumb);

			 		//std::cout <<  i<<"   "<< std::endl;


		weightST= weightST+dijkstra(nodes,steinTree ,used, dijPre, vertNumb, vertNumb-steinerNodes.size(), steinerNodes, heapLength);

		updateSteiner(nodes,steinTree,steinerNodes,pre,dijPre, vertNumb, steinEdges);

	#if BIG	
	 	if(i%500==0){
	 		//clock_t sg=clock();
	 		simplifyGraph(nodes,steinTree,vertNumb);
	 		std::cout <<"STEP "<<i << std::endl;

	 	}
	#endif	
	
		
	}
	#if BIG
		delete[] pre;
		delete[] dijPre;
		delete[] used;
	#endif
	return weightST;
}

