#include <iostream> // cout
#include <fstream> // ifstream
#include <sstream> // istringstream
#include <math.h> // log;exp
using namespace std;

int main(){
    ifstream file("ex1.dat"); // open file.
    string str,a,b,c; // define variables and consts.
    int loc1=0, loc2=0, loc, i=0;
    double val1=1.0, val2=1.0, val;
    const double valMax = 1e64;
    int exp1 = 0, exp2 = 0;
    
    for(int j = 0; j < 3; j++){getline(file,str);} // remove first 3 lines.
    
    while (getline(file,str)){ // loop over remaining lines.
      istringstream ss(str); // get columns.
      ss >> a >> b >> c; 
      val = stof(c); 
      if ((b.size() == 2) && (isnan(val)==false)){ 
	loc = stoi(b);
      }
      else{loc=0;} // error handling.
      if (loc == 1){ // divide by valMax when val is close-to-overflow, then add one onto exponential.
	loc1 += 1;
	val1 *= val;
	if (val1 > valMax){val1=val1/valMax;exp1++;}
      }
      else if (loc == 2){
	loc2 += 1;
	val2 *= val;
	if (val2 > valMax){val2=val2/valMax;exp2++;}
      }
      i++;
    }
    cout << "File: ex1.dat with " <<  i << " lines" << endl; // output results and time taken.
    cout << "Valid values Loc1: " << loc1 << " with GeoMean: " << pow(val1,1.0/loc1) * pow (valMax, double(exp1)/loc1) << endl;
    cout << "Valid values Loc2: " << loc2 << " with GeoMean: " << pow(val2,1.0/loc2) * pow (valMax, double(exp2)/loc2) << endl;
}
      