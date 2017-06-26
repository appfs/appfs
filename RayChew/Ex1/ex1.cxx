//////////////////////////////
//
// compile:	g++ -std=c++11 -O3 ex1.cxx -o ex1
// run:		./ex1 ex1.dat
//
//////////////////////////////

/**
 * \file ex1.cxx
 * \author Ray Chew
 * \date 5 June 2017
 * \brief Reads semicolon-delimited file, sort by location and output geometric means.
 */

/**
 * \mainpage Ex1
 * 
 * \section Description
 * 
 * Reads in a semicolon-delimited file with sequence no.; location; value.<br>
 *
 * Validity:
 * -# Sequence no. contains no spaces<br>
 * -# Location equals 1 or 2<br>
 * -# Value is not NaN, has no spaces.<br>
 * -# Ignore lines starting with `#`.<br>
 * -# Each line must contain only one set of SeqNo; Loc; Value<br>
 *
 * 
 * * compile: `g++ -std=c++11 -O3 ex1.cxx -o ex1`<br>
 * * run: `./ex1 ex1.dat`
 */

/** -- Includes -- */
#include <iostream> // cout
#include <fstream> // ifstream
#include <sstream> // istringstream
#include <math.h> // log;exp
#include <vector> // vector
#include <algorithm> // for_each

using namespace std;

int main(int argc, char* argv[]){
  
  string str; ///< to store line in getline().
  string a; ///< store SeqNo.
  string str0; ///< store line with exceptions.
  int i=0, loc, ja=0, jb=0; // i iteration of loop. ja and jb are number of SeqNo's with first or last char as whitespace.
  size_t pos, pos1; // positions of string.
  char b; // loc as str.
  float val;
  const double valMax = 1e64;
  double mulVal[2] = {1.0,1.0}, logVal[2] = {0.0,0.0};
  vector<int> vecL[2];
  vector<double> vecV[2];
  
  ifstream file(argv[1]); // open file. Filename as argument of main.
  
  while (getline(file,str)){
    str0 = str; // store original string as str0, to output in case of exception.
    str = str.substr(0, str.find("#", 0)); // remove all characters after hex.
    if (str.empty()){i++;continue;} // skip line if line is empty.
    
    pos = str.find(";"); // find first ";".
    a = str.substr(0,pos); // assume everything before that is SeqNo.
    str.erase(0,pos+2); // remove it from string + semicolon and following space.
    
    pos = a.find(" "); // see if there's a whitespace in SeqNo.
    
    b = str[0]; // get loc.
    loc = b-48; // convert char to int.
    
    if (isspace((a.back()))&&(loc==1)){ja++;} // check if whitespace is first or last character of SeqNo. For comparison to sample solution.
    if (isspace(a[0])&&(loc==1)){ja++;}
    if (isspace((a.back()))&&(loc==2)){jb++;}
    if (isspace(a[0])&&(loc==2)){jb++;}
    
    if (string::npos!=pos){i++;cout<<str0<<endl;continue;} // reject line if there's whitespace in SeqNo.
    if ((loc!=1)&&(loc!=2)){i++;cout<<str0<<endl;continue;} // reject line if loc is not valid.
    pos = str.find(";"); // lets find next delimiter... Assume that that is loc.
    str.erase(0,pos+1); // remove loc and the following semicolon.
    
    pos = str.find_first_not_of(" "); // find end of leading whitespaces.
    pos1 = str.find_last_not_of(" "); // find start of trailing whitespaces.
    str.erase(pos1+1); // remove trailing whitespaces.
    str = str.substr(pos,str.length()); // remove leading white spaces.
    pos = str.find_first_of(" ;"); // check if values are valid, i.e. if there's no space or semicolon.
    if (string::npos!=pos){i++;cout<<str0<<endl;continue;} // remove these offending values.
    
    val = stof(str); ///< convert value to float.
    if (isnan(val)){i++;continue;} ///< reject line if value is nan.
    vecL[loc-1].push_back(loc); ///< store Loc for loc count. 
    vecV[loc-1].push_back(val); ///< store Values for GeoMean calculations.
    
    loc=0; i++; // reset location for lines skipped and update counter for line count.
  }
  
  for (int j=0; j < 2;j++){
    for_each(vecV[j].begin(), vecV[j].end(), [&] (double val){
      mulVal[j] *= val; // multiply the values of the vector until close-to-overflow,
      if (mulVal[j] > valMax){
	logVal[j] += log(mulVal[j]);
	mulVal[j]=1.0;} // then log it and add onto logval.
    });
  }
  
  // output results.
  cout << "File: " << argv[1] << " with " <<  i << " lines" << endl;
  cout << "Valid values Loc1: " << vecL[0].size() << " with GeoMean: " << exp((logVal[0]+log(mulVal[0]))/vecL[0].size()) << endl;
  cout << "Valid values Loc2: " << vecL[1].size() << " with GeoMean: " << exp((logVal[1]+log(mulVal[1]))/vecL[1].size()) << endl;
  cout << "# of Loc1 lines with whitespace at start or end: " << ja << endl;
  cout << "# of Loc2 lines with whitespace at start or end: " << jb << endl;
  return 0;
}

