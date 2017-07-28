#ifndef UTILSJJ
#define UTILSJJ

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





class Edg{

public:

	int *edges, *nodes, *weights;
	Edg(int vertNumb, int arcNumb);
	const int size(int at);

};

long long findPrimes(std::vector<int> &primes, int vertNumb, bool* isPrime);
void initializeEdge(Edg &edges, int vertNumb,int arcNumb,int primeNumb,std::ifstream &file);
void copyArray(bool* copyTo, bool* copyFrom, int arrSize);
int nextPower(int number);
void deleteEdge(std::vector<int>* edges, int &from, int to);
bool checkArrays(bool* first, bool* second,int length);
bool isEmpty(std::vector<int>* edges, int length);
bool containsArr(bool* first, bool* second,int length);
int findSteiner(Edg &nodes,int vertNumb,int arcNumb, int primeNumb,  bool* isPrime, int threads, unsigned int starts, std::vector<int> &primes,
		std::vector<std::pair<int, int>>* steinEdges, bool** st, long long* m);


#endif
