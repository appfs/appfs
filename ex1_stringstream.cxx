// Solution using log for values close to overflow.
#include <iostream> // cout
#include <fstream> // ifstream
#include <sstream> // istringstream
#include <math.h> // log;exp
using namespace std;

int main(){
    ifstream file("ex1.dat"); // open file.
    string str,a,b,c; // define variables and consts.
    int loc1=0, loc2=0, loc, i=0;
    double val1=1.0, val2=1.0, val, logval1=0, logval2=0;
    const double valMax = 1e64;

    for(int j = 0; j < 3; j++){getline(file,str);} // loop over first 3 lines.
    
    while (getline(file,str)){ // loop over remaining lines.
      istringstream ss(str); // get columns.
      ss >> a >> b >> c;
      val = stof(c);
      if ((b.size() == 2) && (isnan(val)==false)){ // error handling.
	loc = stoi(b);
      }
      else{loc=0;}
      if (loc == 1){  // sort by location.
	loc1 += 1; // update count for respective location.
	val1 *= val; // get values multiplied until close-to-overflow.
	if (val1 > valMax){logval1+=log(val1);val1=1.0;} // then log it and add onto logval.
      }
      else if (loc == 2){
	loc2 += 1;
	val2 *= val;
	if (val2 > valMax){logval2+=log(val2);val2=1.0;}
      }
      i++;
    }
    cout << "File: ex1.dat with " <<  i << " lines" << endl; // output results.
    cout << "Valid values Loc1: " << loc1 << " with GeoMean: " << exp((logval1+log(val1))/loc1) << endl;
    cout << "Valid values Loc2: " << loc2 << " with GeoMean: " << exp((logval2+log(val2))/loc2) << endl;
}