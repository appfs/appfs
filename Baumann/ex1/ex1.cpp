/*
 * APPFS
 * ex1.cpp
 *
 * Author: Felix Baumann
 *
 * Compile: g++ ex1.cpp -o ex1 -std=c++14
 *
 * Run : ./ex1 ex1.dat
 *
 */


#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <algorithm>
#include <cmath>
#include <vector>

//routine to compute the geometric mean of a std::vector<double> containig data
long double computeGeoMean(std::vector<double>& values){

  double size = values.size();
  double mean = 0;
  for( uint i = 0; i < size; ++i){
    mean += log2( values.at(i) );
  }
  return exp2(mean/size);
}


int main(int argc, char *argv[]){

  if( argc != 2){
    std::cout << "Call the program as: " << argv[0] << " filename" << std::endl;
    exit(EXIT_FAILURE);
  }

  std::ifstream    file(argv[1]);
  std::string      line;

  if(!file){
    std::cout << "Couldn't open file" << std::endl;
    return 1;
  }

  int         nlines = 0;
  int         loc;
  int         loc1 = 0;
  int         loc2 = 0;

  //vectors to save input data
  std::vector<double> val1;
  std::vector<double> val2;

  double logVal1 = 0.;
  double logVal2 = 0.;

  double mean1;
  double mean2;


  //loop over all lines in the file
  while( std::getline(file, line) ){

    nlines++;

    std::stringstream linestream(line);
    std::string       data;

    try{

      //line number
      std::getline(linestream, data, ';');                              //read until next semicolon
      data.erase(std::remove(data.begin(),data.end(),' '),data.end());  //remove whitespaces

      //location
      std::getline(linestream, data, ';');                              //read until next semicolon
      data.erase(std::remove(data.begin(),data.end(),' '),data.end());
      loc = std::stoi(data);

      //value
      std::getline(linestream, data, ';');                              //read until next semicolon
      data.erase(std::remove(data.begin(),data.end(),' '),data.end());
      if( std::stod(data) <= 0.) continue;
      if(data == "nan" || data == "-nan" || data == "inf" || data == "-inf") continue;

      //add values to vector
      if(loc == 1){
        val1.push_back( std::stod(data) );
        logVal1 += log2( std::stod(data) );
        loc1++;

      } else if (loc == 2) {
        val2.push_back( std::stod(data) );
        logVal2 += log2( std::stod(data) );
        loc2++;
      } else continue;


  } catch (std::invalid_argument& ia){
    //when data is not a digit,
    //std::stoi or std::stol throws an invalid argument exception
  } catch (std::bad_alloc& e){
//    std::cout << "line: " << nlines << "\n";
//    std::cout << "Allocation failed: " << e.what() << "\n";
  }


  }
  file.close();

  mean1 = exp2 (logVal1/double(loc1));
  mean2 = exp2 (logVal2/double(loc2));

  std::cout << "File: " << argv[1] << " with " << nlines << " lines\n";
  std::cout << "Valid values Loc1: " << val1.size() << " with GeoMean: " << mean1 << "\n";
  std::cout << "Valid values Loc2: " << val2.size() << " with GeoMean: " << mean2 << std::endl;

  return 0;
}
