#include <string>
#include <iostream>
#include <fstream>
#include <stdexcept>
#include <stdio.h>
#include <cstdio>
#include <ctime>
#include <chrono>
#include <vector>
#include <climits>
#include <utility>                          // for std::pair
#include "algo.hh"

using namespace std;

int main (int argc, char* argv[]) {

  //start timers for cpu and wall time
  clock_t cpu0 = clock();
  auto   wall0 = chrono::system_clock::now();

  //check for right number of arguments
  if( argc != 3){
    cout << "Call the program as: "
         << argv[0] << " filename.gph m1/m2" << endl;
    exit(EXIT_FAILURE);
  }
  
  string mod = argv[2];
  int      m = 0;
  
  //choose modus
  if(mod.compare("m1") == 0){
    fprintf(stdout, "Using data structures and dijkstra-algorithm from boost library.\n");
    m = 1;
  } else if (mod.compare("m2") == 0) {
    fprintf(stdout, "Using own data structures and dijkstra-algorithm.\n");
    m = 2;
  } else {
    fprintf(stderr, "Please enter 'm1' for boost algorithm and 'm2' for the own algorithm.");
    return 1;
  }
  
  //result vertex and distance to root
  pair<int,int> result;
  
  switch (m){
    //using boost library
    case 1: result = algo1(argv[1]);
    //using own algorithm
    case 2: result = algo2(argv[1]);
  }
  
  fprintf(stdout, "RESULT VERTEX %i \nRESULT DIST %i \n", result.first, result.second);

  double cpuTime = (clock() - cpu0) / (double) CLOCKS_PER_SEC;
  chrono::duration<double> wallDur = (chrono::system_clock::now() - wall0);
  double wallTime = wallDur.count();

  fprintf(stdout, "Finished in %f seconds [CPU Clock] and %f seconds [Wall Clock] \n", cpuTime, wallTime);

  return 0;
  
}
