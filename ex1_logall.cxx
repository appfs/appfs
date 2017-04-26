#include <iostream>
#include <fstream>
#include <sstream>
#include <math.h>
#include <chrono>
using namespace std;

int main(){
    chrono::steady_clock::time_point start = chrono::steady_clock::now(); // start timer.
    ifstream file("ex1.dat"); // open file.
    string str,a,b,c; // define variables.
    int loc1=0, loc2=0, loc0=0, loc, i=0;
    float val;
    double val1=0, val2=0;

    for(int j = 0; j < 3; j++){getline(file,str);} // remove first 3 lines.
    
    while (getline(file,str)){ // loop over remaining lines.
    //while(i < 100000){
      //getline(file,str);
      //cout << str << endl;
      istringstream ss(str); // get columns.
      ss >> a >> b >> c; 
      val = stof(c); 
      //cout << b.size() << endl;
      if ((b.size() == 2) && (isnan(val)==false)){ // error handling.
	loc = stoi(b);
	val = log(val);
	//cout << "loc: " << loc << endl;
	//cout << "val: " << val << endl;
      }
      else{loc=0;}
      if (loc == 1){
	loc1 += 1;
	val1 += val;
	//cout << "val1: " << val1 << endl;
      }
      else if (loc == 2){
	loc2 += 1;
	val2 += val;
	//cout << "val2: " << val2 << endl;
      }
      //else{loc0+=1;}
      //if((isnan(val1)) || (isnan(val2))){break;}
      i++;
    }
    chrono::steady_clock::time_point end = chrono::steady_clock::now(); // end timer.
    cout << "File: ex1.dat with " <<  i << " lines" << endl; // output results and time taken.
    cout << "Valid values Loc1: " << loc1 << " with GeoMean: " << exp(val1/loc1) << endl;
    cout << "Valid values Loc2: " << loc2 << " with GeoMean: " << exp(val2/loc2) << endl;
    cout << "Time taken: " << chrono::duration_cast<chrono::milliseconds> (end - start).count() <<"ms"<< endl;
}
      