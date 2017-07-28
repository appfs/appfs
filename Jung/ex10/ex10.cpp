
/*
* This is a program for the Steiner Tree Heuristic from Takahashi, Matsuyama for the APPFS course at the TU Berlin
*
* @author Johannes Jung
*
* I just started C++ Programming this semester, so this program has probably not the best style
*/

#include <iostream>
#include <fstream>
#include <string>
#include <exception>
#include <vector>
#include <time.h>
#include <utility>
#include <limits>
#include <tuple>
#include <climits>
#include <cmath>
#include <omp.h>
#include <sys/time.h>
#include <algorithm> 
#include "utils.h"
#include "steinCheck.h"
#include "dijkstra.h"




typedef std::pair<int,int> Edge;


/**
*
*
* Steiner Tree Heuristic from Takahashi, Matsuyama on multiple threads
*
* writes the graph to the steinEdges and st array
*
* @param nodes: The Edges corresponding to the nodes
* @param vertNumb: Number of verxes in the graph
* @param arcNumb: Number of arcs in the graph
* @param primeNumb: number of terminals
* @param isPrime: True if a number is prime
* @param threads: Number of Threads
* @param starts: Number of how often the algorithm should run from distinct start nodes
* @param primes: a vector of primes
* @param steinerEdges: Edges-vector to write the steiner Edges in
* @param st: The nodes in the Steiner Tree per thread
* @param  m: The weight of the minimum steiner tree per thread
*
* @return The Thread with the best solution
**/

int findSteiner(Edg &edges,int vertNumb,int arcNumb, int primeNumb,  bool* isPrime, int threads, unsigned int starts, std::vector<int> &primes,
						std::vector<Edge>* steinEdges, bool** st, long long* m){


		bool** isPrimeCopy= new bool*[threads]();	// Primes for every thread
		long long min= LONG_LONG_MAX;				// Overall found minimum Weight of the steiner Tree
		int minThread=0;							// the number of the thread which found the minimum steiner tree
		int loops;									// Set the loops how often the algorithmus should run

		// set the amount of loops
		if(primes.size()<starts){
			loops=primes.size();
		}
		else{
			loops=starts;
		}

		// Initialize the arrays
		for(int i = 0; i < threads; i++){

		   	st[i] = new bool[vertNumb]();
		   	isPrimeCopy[i]= new bool[vertNumb]();
		   	m[i]= LONG_LONG_MAX;
		}





		////// Find the minimum Steiner Tree for each Thread
		#pragma omp parallel for //reduction(min:min)
		for(int i=0; i<loops; i++){

			int* pre=new int[vertNumb]();					// The predecessor map of steiner Tree
			bool* steinTreeCopy= new bool[vertNumb]();		// Steiner tree copy for the step
			std::vector<Edge> steinEdgs;					// The Edges in the Steiner tree
			int start=primes[i];

			copyArray(isPrimeCopy[omp_get_thread_num()],isPrime, vertNumb);
			std::fill_n(steinTreeCopy, vertNumb, false);

			// add the start node to the steiner Tree
			steinTreeCopy[start]=true;
			pre[start]=start;

			//calculates the heapLeangth , vertNumb+1 because the zero entry
			int heapLength=nextPower(arcNumb+1)*2;



			long long weightST=0;
			dijkstra(edges,steinTreeCopy ,pre, vertNumb, start, steinEdgs, heapLength, isPrimeCopy[omp_get_thread_num()], primeNumb, weightST);

			// compare to furhter results
			if(weightST<m[omp_get_thread_num()]){

				m[omp_get_thread_num()]=weightST;
				steinEdges[omp_get_thread_num()]=steinEdgs;
				copyArray(st[omp_get_thread_num()],steinTreeCopy, vertNumb);
			}

			delete[] pre;
			delete[] steinTreeCopy;
		}


		//////// compare thread results

		for(int i =0; i< threads; i++){

			if(m[i]<min){

				minThread=i;
				min=m[i];
			}
		}


		for(int i = 0; i < threads; i++){

				delete[] isPrimeCopy[i];
			}
		delete[] isPrimeCopy;

	return minThread;
}



 
int main(int argc, char* argv[]){



	clock_t cc=clock();			// CPU-Time
	double wcTime;				// Wall clock time
	std::ifstream file;			// gph file to read from
	bool print=false;			// If true the tree is printed out
	unsigned int starts=100;	// number of terminals to start with (100 default)
	long vertNumb;				//Number of vertexes
	long arcNumb;				//Number of Edges
	bool* isPrime;				//determines if a vertex is an prime or not
	int primeNumb;				//Number of Primes(Terminals)
	std::vector<int> primes;	//Vector of the Terminals
	int threads=1;				// number of threads , default=1
	omp_set_num_threads(1);



	////////////////////////// read input arguments

	std::string fileString;
	std::string dir="SP/";
	std::string lastArg=argv[argc-1];


	if(lastArg=="-s"){
		print=true;
	}

	if(argc<2){

		std::cout<<"please enter a file name, check README.md for right input"<<std::endl;
		return 0;
	}

	if(argc>2){

		try{
			starts=std::stoi(argv[2]);
		}
		catch(...){

			if(!print || argc>3){

				std::cout<<"Wrong input for terminalNumber, check README.md";
				return 0;
			}
		}
	}

	fileString=(argv[1]);
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

			if(!print || argc> 4){

				std::cout<<"wrong input for threads, check README.md"<<std::endl;
				return 0;
			}
		}
	}




	///////////////////////////  Read the gph and primes file


	// get the Number of vertexes
  	std::string str;
	getline(file, str, ' ');
	vertNumb = std::stol(str)+2;		// The start and end node a imaginary nodes

	// get the prime nodes
	std::cout<<"Get terminals..."<<std::endl;
	isPrime=new bool[vertNumb]();
	primeNumb=findPrimes(primes,vertNumb-2,isPrime);
	std::cout<<"done: There are "<< primeNumb<<" Terminals"<<std::endl;


	// get the Number of arcs
	getline(file, str);
	arcNumb=std::stol(str);

	Edg edges(vertNumb,arcNumb);		// The edges from all the nodes TODO

	// stores the Edges to the nodes array
	std::cout<<std::endl;
	std::cout<<"Initialize Graph..."<<std::endl;
	initializeEdge(edges,vertNumb, arcNumb, primeNumb,file);
	std::cout<<"done"<<std::endl;





	/////////////////// Set the variables


	struct timeval start, end;					// Variables for time measurement

	std::vector<Edge> steinEdges[threads]; 	// The Edges which are finally in the Steiner Tree
	bool** st = new bool*[threads]();			// Steiner Nodes of the minimum steiner Tree per thread
	long long m[threads];						// Minimum weight steiner tree found per thread


	// Start time
	gettimeofday(&start, NULL);
	
	//////////////////// Run the algorithm

	std::cout<<std::endl;
	std::cout<<"Run the Algorithm..."<<std::endl;

	int minThread=findSteiner(edges,vertNumb,arcNumb,primeNumb, isPrime, threads, starts, primes, steinEdges, st,m);

	// get the time
	gettimeofday(&end, NULL);
	wcTime = ((end.tv_sec  - start.tv_sec) * 1000000u +
         end.tv_usec - start.tv_usec) / 1.e6;
	

	std::cout<<"done"<<std::endl;



	//////////////////// Checking if its a Tree

	std::cout<<std::endl;
	std::cout<< "checking if its a Tree..."<<std::endl;

	// Erase the first Edge because its not a real Edge
	std::vector<Edge>::iterator begin=steinEdges[minThread].begin();
	steinEdges[minThread].erase(begin);


	if(steinCheck(steinEdges[minThread],st[minThread], isPrime, vertNumb)){
		std::cout<< "done"<<std::endl;
	}
	else{
		std::cout<<std::endl;
		std::cout<< "This is not a Tree!"<<std::endl;
	}


	std::cout << std::endl;



	////////////////////// Output

	std::cout <<"TLEN: "<< m[minThread] << std::endl;

	if(print){

		int size=steinEdges[minThread].size();
		std::cout << std::endl;
		std::cout <<"TREE: ";
		for(int i=0;i<size;i++){


			std::cout <<"("<< steinEdges[minThread][i].first <<","<< steinEdges[minThread][i].second<< ") ";

		}
	}
	std::cout << std::endl;
	std::cout << std::endl;
	std::cout <<"TIME: "<< (double)(clock() - cc) / CLOCKS_PER_SEC<< " seconds" << std::endl;
	std::cout <<"WALL: "<<wcTime<< " seconds" << std::endl;


	for(int i = 0; i < threads; i++){
		delete[] st[i];
	}
	delete[] st;
	delete[] isPrime;
	delete[] edges.edges;
	delete[] edges.weights;
	delete[] edges.nodes;


 	return 0;	
}


