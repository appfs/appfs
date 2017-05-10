#include <iostream> // cout
#include <fstream> // ifstream
#include <sstream> // istringstream
#include <math.h> // log;exp
#include <vector> // vector
#include <algorithm> // for_each
using namespace std;

int main(int argc, char* argv[]){
    string str, a,b,c; // define variables and constants.
    int i=0, loc;
    double val1=1.0, val2=1.0, val, logval1=0, logval2=0;
    const double valMax = 1e64;
    vector<int> vectorSeqNo1,vectorSeqNo2,vectorLoc1,vectorLoc2;
    vector<double> vectorVal1, vectorVal2;
    
    ifstream file(argv[1]); // open file. Filename as argument of main.
    
    // loop over lines in file until end-of-file.
    while (getline(file,str)){
      if (str[0]=='#'||str.empty()){;}else{ // check if line should be ignored, if first char is '#' or empty line.
	istringstream ss(str); // get columns.
	ss >> a >> b >> c; // split columns at spaces. Does not handle cases with spaces in SeqNo.
	val = stod(c); 
	if ((any_of(b.begin(),b.end(),::isdigit)) && (!isnan(val))){ // error handling. loc must be digit, and nan values are not allowed.
	  loc = stoi(b);
	  if(loc==1){ // create data array corresponding to loc=1.
	    vectorSeqNo1.push_back(stoi(a));  // storing SeqNo to have data structure of file... Not sure if necessary.
	    vectorLoc1.push_back(loc); // store Loc for loc count. 
	    vectorVal1.push_back(val); // store Values for GeoMean calculations.
	  }
	  else if (loc==2){ // create data array corresponding to loc=2. Same as in loc=1.
	    vectorSeqNo2.push_back(stoi(a));
	    vectorLoc2.push_back(loc);
	    vectorVal2.push_back(val);
	  }
	  //else{if(!isnan(val)){cout<<"str: "<<str<<"      b:"<<b<<" c: "<<c<<endl;}} //show the exceptions that were not handled.
	}
      }
      loc=0; i++; // update counter for line count, and reset location for lines skipped.
    }
    
    // loop through vector containing values. Calculate GeoMean.
    for_each(vectorVal1.begin(), vectorVal1.end(), [&] (double val){
      val1 *= val; // multiply the values of the vector until close-to-overflow,
      if (val1 > valMax){logval1+=log(val1);val1=1.0;} // then log it and add onto logval.
    });
    
   // same as in for loc=1.
    for_each(vectorVal2.begin(), vectorVal2.end(), [&] (double val){ 
      val2 *= val;
      if (val2 > valMax){logval2+=log(val2);val2=1.0;}
    });
   
   // output results.
    cout << "File: " << argv[1] << " with " <<  i << " lines" << endl;
    cout << "Valid values Loc1: " << vectorLoc1.size() << " with GeoMean: " << exp((logval1+log(val1))/vectorLoc1.size()) << endl;
    cout << "Valid values Loc2: " << vectorLoc2.size() << " with GeoMean: " << exp((logval2+log(val2))/vectorLoc2.size()) << endl;
    return 0;
}

