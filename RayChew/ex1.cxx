#include <iostream> // cout
#include <fstream> // ifstream
#include <math.h> // log;exp
using namespace std;

int main(){
    ifstream file("ex1.dat"); // open file.
    string str,seqno; // define variables and consts.
    int loc1=0, loc2=0, loc, length=100001235;
    double val1=1.0, val2=1.0, val, logval1=0, logval2=0;
    const double valMax = 1e64;

    for(int j = 0; j < 3; j++){getline(file,str);} // loop over first 3 lines.
    
    while(getline(file,seqno,';')){ // get seq. no, discard.
      getline(file,str,';'); // get location.
      loc = (int)str[1]-48; // convert to int.
      getline(file,str); // get value.
      val = stof(str); // convert to float.
      if ((loc == 1)&&(isnan(val)==false)){  // sort by location.
	loc1 += 1; // update count for respective location.
	val1 *= val; // get values multiplied until close-to-overflow.
	if (val1 > valMax){logval1+=log(val1);val1=1.0;} // then log it and add onto logval.
      }
      else if ((loc == 2)&&(isnan(val)==false)){
	loc2 += 1;
	val2 *= val;
	if (val2 > valMax){logval2+=log(val2);val2=1.0;}
      }
    }
    cout << "File: ex1.dat with " <<  length << " lines" << endl; // output results.
    cout << "Valid values Loc1: " << loc1 << " with GeoMean: " << exp((logval1+log(val1))/loc1) << endl;
    cout << "Valid values Loc2: " << loc2 << " with GeoMean: " << exp((logval2+log(val2))/loc2) << endl;
}