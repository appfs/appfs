#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <algorithm>
#include <cmath>

int main(){

  std::ifstream    file("ex1.dat");
  std::string      line;

  int         counter = 0;
  int         loc;
  int         loc1 = 0;
  int         loc2 = 0;
  long double val1 = 0;
  long double val2 = 0;
  double      mean1;
  double      mean2;


  if(!file){
    std::cout << "Couldn't open file" << std::endl;
    return 1;
  }

  //loop over all lines in the file
  while( std::getline(file, line) ){

    counter++;

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
      if(data == "nan" || data == "-nan" || data == "inf" || data == "-inf") continue;
      if( std::stol(data) <= 0.) continue;

      //sum up logarithmic values of loc1 and loc2
      if(loc == 1){
        val1 += log( std::stol(data) );
        loc1++;
      } else if (loc == 2) {
        val2 += log( std::stol(data) );
        loc2++;
      }


  } catch (std::invalid_argument& ia){
    //when data is not a digit,
    //std::stoi or std::stol throws an invalid argument exception
  }


  }
  file.close();

  //geometric mean of loc1 and loc2
  mean1 = exp(val1/loc1);
  mean2 = exp(val2/loc2);

  std::cout << "File: ex1.dat with " << counter << " lines\n";
  std::cout << "Valid values Loc1: " << loc1 << " with GeoMean: " << mean1 << "\n";
  std::cout << "Valid values Loc2: " << loc2 << " with GeoMean: " << mean2 << std::endl;

  return 0;
}
