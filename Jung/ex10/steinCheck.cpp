#include "steinCheck.h"


typedef std::pair<int,int> Edge;



int countNodes(bool* steinTree, int length){


	int count=0;
	for(int i=0;i<length;i++){

		if(steinTree[i]){
			count++;
		}
	}

	return count;
}


// deletes an edge
void deleteEdge(std::vector<int>* edges, int from, int to){

	std::vector<int>::iterator it=edges[from].begin();

	for(; it != edges[from].end(); it++){

		if((*it)==to){
			edges[from].erase(it);
			return;
		}
	}


}


// depth first search from vertInd as root
bool dfs(std::vector<int>* edges, int vertInd, bool* check, bool* alr){

	std::vector<int>::iterator it=edges[vertInd].begin();

	int j=0;

	for(; it != edges[vertInd].end(); it++){

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

		if(vertInd ==1){

			std::cout<<"";
		}


		// set iterator to the right index
		//it=edges[vertInd].begin()+j-1;



		j++;

		

	}
	return true;

}

// checks if the arrays have the same entrys
bool checkArrays(bool* first, bool* second,int length){



		for(int i = 0; i < length; i++){
			if(first[i] != second[i]){

				std::cout<<" check array :"<<i<< "  "<<first[i]<<std::endl;
				return false;
			}
		}
	

	return true;
}


bool isEmpty(std::vector<int>* edges, int length){


	for(int i = 0; i < length; i++){

		if(edges[i].size()!=0){

			std::cout<<"is Empty"<<std::endl;
			return false;
		}
	}
	return true;
}


bool steinCheck(std::vector<Edge> &steinEdges, bool* steinTree, int vertNumb){


	#if BIG
		bool* check=new bool[vertNumb]();	// checks if every node in the Steinertree is reached by edges

		for(int i=0;i<vertNumb; i++){
		check[i]=false;
		}

		std::vector<int>* edges= new std::vector<int>[vertNumb]();

		bool* alr=new bool[vertNumb]();		//	checks if node was already visited

		for(int i=0;i<vertNumb; i++){
			check[i]=false;
		}	
	
	#else

		bool check[vertNumb]={false};
		std::vector<int> edges[vertNumb];
		bool alr[vertNumb]={false};		//	checks if node was already visited
	#endif






	//int vertNumb2= countNodes(steinTree);

	

	////////////// fill in the edges

	int edgNumb= steinEdges.size();

	for(int i=0;i<edgNumb;i++){

		edges[steinEdges[i].second].push_back(steinEdges[i].first);
		edges[steinEdges[i].first].push_back(steinEdges[i].second);
	}


	check[2]=true;
	alr[2]=true;
	bool noCyc=dfs(edges,2, check, alr);

	

	if(noCyc && checkArrays(steinTree, check, vertNumb) && isEmpty(edges, vertNumb)){
		return true;

	}
	else{
		return false;
		
	}




}