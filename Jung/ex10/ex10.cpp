
#include <iostream>
#include <fstream>
#include <string>
#include <exception>
#include <vector>
#include <time.h>
#include <ctime>
#include <utility>
#include <limits>
#include <tuple>
#include <list>
#include <climits>
#include <cmath>
#include <omp.h>
#include <sys/time.h>
#include <algorithm> 
#include "dijkstra.h"
#include "findSteiner.h"
#include "steinCheck.h"




//Edge with weight, first the node pointing to, second the weigth
typedef std::pair<int,int> Edge_wW;



void copyVecArray(std::vector<Edge_wW>* copyTo,std::vector<Edge_wW>* copyFrom, int arrSize){
	for(int i=0; i<arrSize; i++){
	
			copyTo[i]=copyFrom[i];
		
		
	}
}


//find the prime vertexes
int findPrimes(std::vector<int> &primes, int vertNumb){

		std::ifstream file;
		file.open("primes1.txt");

		if (!file.is_open()) {
			std::cerr << "You need the primes1.txt in the same Directory as your ex10.cpp" << std::endl;
			return 0;
		}

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








// reads in and initialize the Edges
void initializeEdge(std::vector<Edge_wW>* nodes,std::vector<int> primes, int vertNumb,int arcNumb,int primeNumb,std::ifstream &file){

	try {

		int count=0;


		std::string str;
		
		while (count<arcNumb-primeNumb) {

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

			nodes[from].push_back(Edge_wW(to,dist));
			nodes[to].push_back(Edge_wW(from,dist));

			count++;
		}


		//Edges from all primes except to the sink
		for(int i=0;i<primeNumb;i++){

			int prime=primes[i];

			nodes[prime].push_back(Edge_wW(vertNumb-1,0));
			nodes[vertNumb-1].push_back(Edge_wW(prime,0));
		}
	}
	catch (...) {
	}
}








 
int main(int argc, char* argv[]){



	clock_t cc=clock();
	
	// number of threads , default=1
	int threads=1;
	omp_set_num_threads(1);
	// number of terminals to start with (100 default)
	unsigned int starts=100;
	if(argc>2){
		try{
			starts=std::stoi(argv[2]);
		}
		catch(...){
			std::cout<<"Wrong input for terminalNumber";
		}
	}
	

	

	////////////////////////// read input arguments

	if(argc<2){

		std::cout<<"please enter a file name"<<std::endl;
		return 0;
	}

	std::ifstream file;
	std::string fileString(argv[1]);
	std::string dir="SP/";
	fileString=dir+fileString;
	file.open(fileString);
		

	if (!file.is_open()) {
			std::cerr << "there is no file of the name \"" << argv[1] << "\" in the directory" << std::endl;
			return 0;
	}
	
	if(argc>3){

		try{

			// set the number of threads which should be used
			omp_set_num_threads(std::stoi(argv[3]));
			threads=std::stoi(argv[3]);
		}
		catch(...){

			std::cout<<"wrong input for threads"<<std::endl;
		}
	}






	///////////////////////////  Read the gph and primes file


	// get the Number of vertexes
  	std::string str;
	getline(file, str, ' ');
	int vertNumb = std::stoi(str)+2;

	// get the prime nodes
	std::vector<int> primes;
	int primeNumb=findPrimes(primes,vertNumb-2);

	//vector array , for each node the edges are stored here, node 0 is the (added) source and vertNumb-1 is the (added) sink

	#if BIG
		std::vector<Edge_wW>* nodes=new std::vector<Edge_wW>[vertNumb]();
	#else
		std::vector<Edge_wW> nodes[vertNumb];
	#endif

	// get the Number of arcs
	getline(file, str);
	int arcNumb=std::stoi(str)+primeNumb;

	// stores the Edges to the nodes array
	initializeEdge(nodes,primes,vertNumb, arcNumb, primeNumb,file);






	/////////////////// Set the variables


	// Set the loops how often the algorithmus should run
	int loops;
	if(primes.size()<starts){
		loops=primes.size();
	}
	else{
		loops=starts;
	}
	
	#if BIG
		bool* steinTree= new bool[vertNumb]();
	#else
		bool steinTree[vertNumb];
	#endif

	// the Weight of the Steiner tree
	long long weightST=0;


	// The Edges which are finally in the Steiner Tree, this is needed for checking if its a tree
	std::vector<Edge_wW> steinEdges[loops];




	////////////////////////// Run the algorithm


	long long min= LONG_LONG_MAX;
	int minPos=0;

	#if BIG
		bool** st = new bool*[threads];
		for(int i = 0; i < threads; ++i){
	   		st[i] = new bool[vertNumb];
		}
	#else
		bool st[threads][vertNumb];
	#endif


	long long m[threads];

	int mp[threads];

	for(int i=0;i< threads;i++){

		m[i]= LONG_LONG_MAX;
	}



	double delta;
	struct timeval start, end;
	gettimeofday(&start, NULL);



	#pragma omp parallel for //reduction(min:min)
	for(int i=0; i<loops; i++){

		#if BIG
			bool* steinTreeCopy= new bool[vertNumb]();
			std::fill_n(steinTreeCopy, vertNumb, false); 
			std::vector<Edge_wW>* nodesCopy=new std::vector<Edge_wW>[vertNumb]();
			copyVecArray(nodesCopy,nodes,vertNumb);
		#else
			bool steinTreeCopy[vertNumb]={false};
			std::vector<Edge_wW> nodesCopy[vertNumb]=nodes;
		#endif

		

		
		

		weightST=findSteiner(nodesCopy, vertNumb, primeNumb,steinEdges[i], steinTreeCopy, primes[i]);


		if(weightST<m[omp_get_thread_num()]){

			m[omp_get_thread_num()]=weightST;
			mp[omp_get_thread_num()]=i;
			copyArray(st[omp_get_thread_num()],steinTreeCopy, vertNumb);
		}

		#if BIG
			delete[] steinTreeCopy;
			delete[] nodesCopy;
		#endif
	}




	//////// compare thread results

	int j=0;
		
	for(int i =0; i< threads; i++){

		if(m[i]<min){
				
			j=i;
			min=m[i];
		}
	}
	

	
	

	copyArray(steinTree,st[j],vertNumb);
	minPos=mp[j];



	////// get the time
	gettimeofday(&end, NULL);
	delta = ((end.tv_sec  - start.tv_sec) * 1000000u + 
         end.tv_usec - start.tv_usec) / 1.e6;
	


	/////////////// Checking if its a Tree

	std::cout << std::endl;
	std::cout << std::endl;
	std::cout<< "checking if its a Tree...";

	if(steinCheck(steinEdges[minPos],steinTree, vertNumb)){
		std::cout<< "done"<<std::endl;
	}
	else{
		std::cout<<std::endl;
		std::cout<< "This is not a Tree!"<<std::endl;
	}
	std::cout << std::endl;
	std::cout << std::endl;



	////////////////////// Output

	std::cout <<"TLEN: "<< min << std::endl;
	std::cout << std::endl;
	std::cout <<"TREE: "<< "  " ;		

	int size=steinEdges[minPos].size();
	for(int i=0;i<size;i++){

		std::cout <<"("<< steinEdges[minPos][i].first <<","<< steinEdges[minPos][i].second<< ") ";

	}

	std::cout << std::endl;
	std::cout << std::endl;
	std::cout <<"TIME: "<< (double)(clock() - cc) / CLOCKS_PER_SEC<< " seconds" << std::endl;
	std::cout <<"WALL: "<<delta<< " seconds" << std::endl;


 	return 0;	
}


