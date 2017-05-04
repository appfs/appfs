/* @author: Fabian Wegscheider */

#include <iostream>
#include <fstream>
#include <math.h>

using namespace std;



int main(int numargs, char *args[]) {

	ifstream inputFile;
	inputFile.open(args[1]);
	if (inputFile.fail()) {
		cerr << "file could not be read" << endl;
		exit(1);
	}

	string line;
	int n1 = 0;
	int n2 = 0;
	double logSum1 = 0;
	double logSum2 = 0;
	double geoMean1 = 0;
	double geoMean2 = 0;
	long numOfLines = 0;
	int err = 0;


	while (getline(inputFile, line)) {
		numOfLines++;

		if (line.empty()) {			//empty lines are ignored
			continue;
		}

		if (line.find("#") == 0) {		//comment lines are ignored
			continue;
		}


		int pos1 = line.find(";");
		int pos2 = line.find(";", pos1+1);
		int seqNum = 0;
		int geoNum = 0;
		double location = 0;

		try {
			seqNum = stoi(line.substr(0,pos1));
			geoNum = stoi(line.substr(pos1+1, pos2-pos1-1));
			location = stod(line.substr(pos2+1, line.length()-pos2-1));
		} catch (...) {
			err++;												//test format of line
			continue;
		}


		if (location != location || location <=0 ) {			//location must be positive
			err++;
			continue;
		}

		if (geoNum == 1) {
			n1++;
			logSum1 += log(location);
		}
		else if (geoNum == 2) {
			n2++;
			logSum2 += log(location);
		}
		else {
			err++;
			continue;
		}


	}

	geoMean1 = exp(logSum1/n1);
	geoMean2 = exp(logSum2/n2);

	// writing output
	cout << "File: " << args[1] << " with " << numOfLines << " lines" << endl;
	cout << "Valid values Loc1: " << n1 << " with GeoMean " << geoMean1 << endl;
	cout << "Valid values Loc2: " << n2 << " with GeoMean " << geoMean2 << endl;
	cout << "Number of errors in the file: " << err << endl;

}

