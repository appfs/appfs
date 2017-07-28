#include "utils.h"



//typedegs for edges
typedef std::pair<int, int> Edge;


/**
 * The class that represents the edges in the tree
 *
 *@param edges: the edges of the tree
 *@param nodes: tells you where you can find the edges of the given node in the edges array
 *@param weights: tells you which weight the edges have
 */

Edg::Edg(int vertNumb, int arcNumb){

	nodes=new int[vertNumb]();
	edges=new int[arcNumb*2]();
	weights=new int[arcNumb*2]();

}


const int Edg::size(int at){
	return  nodes[at+1]-nodes[at];
}



/**
 * function which determines all primes up to a given number
 *
 */
long long findPrimes(std::vector<int> &primes, int vertNumb, bool* isPrime){

	std::string str;
	std::ifstream file;
	int primesNumb=0;


	file.open("primes1.txt");

	if (!file.is_open()) {

		std::cerr << "You need the primes1.txt in the same Directory as your ex10.cpp" << std::endl;
		return 0;
	}


	getline(file, str,'\r');

	int searchingPrimes=true;

	while(searchingPrimes){

		int numsInLine=8;
		getline(file, str,'\r');

		for(int i=0;i<numsInLine;i++){

			long nextPrime = std::stoll(str.substr(10*i,10+10*i));

			if(nextPrime<=vertNumb){

				isPrime[nextPrime]=true;
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







/**
 * The class that represents the edges in the tree
 *
 *@param edges: the edges of the tree
 *@param nodes: tells you where you can find the edges of the given node in the edges array
 *@param weights: tells you which weight the edges have
 */
void initializeEdge(Edg &edges, int vertNumb,int arcNumb,int primeNumb,std::ifstream &file){

	std::vector<Edge>* nodes=new std::vector<Edge>[vertNumb]();

	//////////////// Put the edges in nodes
	try {

		std::string str;
		for (int i=0; i<arcNumb; i++) {


			getline(file, str, ' ');
			int from = std::stol(str);

			getline(file, str, ' ');
			int to = std::stol(str);

			getline(file,str);
			int dist=std::stol(str);



			nodes[from].push_back(Edge(to,dist));
			nodes[to].push_back(Edge(from,dist));

		}

		/////////////////////// Put the Edges in edges array

		int count=0;
		for(int i=0;i<vertNumb;i++){

			int size=nodes[i].size();
			edges.nodes[i]=count;

			for(int j=0; j<size; j++){

				edges.edges[count]=nodes[i][j].first;
				edges.weights[count]=nodes[i][j].second;
				count++;
			}
		}
	}
	catch (...) {
	}

	delete[] nodes;
}





/**
* copy an array copyFrom to an array copyTo, both arrays have length arrSize
**/
void copyArray(bool* copyTo, bool* copyFrom, int arrSize){
	for(int i=0; i<arrSize; i++){
		copyTo[i]=copyFrom[i];
	}
}


/**
* determines i^2 with (i-1)^2 < number <= i*2
**/
int nextPower(int number){

	int i = 0;
	while(true){

		if(pow(2,i)<number && number<=pow(2,i+1)){
			return pow(2,i+1);
		}
		i++;
	}
}


/**
* deletes an edge
**/
void deleteEdge(std::vector<int>* edges, int &from, int to){

	std::vector<int>::iterator it=edges[from].begin();

	for(; it != edges[from].end(); ++it){

		if((*it)==to){
			edges[from].erase(it);
			return;
		}
	}
}

/**
* checks if the arrays have the same entries
**/
bool checkArrays(bool* first, bool* second,int length){


	for(int i = 0; i < length; i++){
			if(first[i] != second[i]){

				std::cout<<" check array :"<<i<< "  "<<first[i]<<std::endl;
				return false;
			}
		}

	return true;
}


/**
* checks if every vector has size zero
**/
bool isEmpty(std::vector<int>* edges, int length){


	for(int i = 0; i < length; i++){

		if(edges[i].size()!=0){

			std::cout<<"is Empty"<<std::endl;
			return false;
		}
	}
	return true;
}

/**
*  checks if the first array is always true if the second array is true
**/
bool containsArr(bool* first, bool* second,int length){

	for(int i=0;i<length; i++){

		if(!first[i] && second[i]){
			return false;
		}
	}
	return true;
}



