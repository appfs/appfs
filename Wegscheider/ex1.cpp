/* @author: Fabian Wegscheider */

#include <iostream>
#include <fstream>
#include <math.h>
#include <vector>

using namespace std;



int main(int numargs, char *args[]) {

	if (numargs != 2) {
		cerr << "Usage: " << args[0] << " filename" << endl;
		exit(EXIT_FAILURE);
	}

	ifstream inputFile;
	inputFile.open(args[1]);
	if (inputFile.fail()) {
		cerr << "file could not be read" << endl;
		exit(EXIT_FAILURE);
	}

	string line;
	vector<double> values[2];
	double geoMean[2] {.0, .0};
	int nLoc[2] {0, 0};
	long nLines = 0;
	int err = 0;


	while (getline(inputFile, line)) {
		nLines++;

		if (line.empty()) {			//empty lines are ignored
			continue;
		}

		if (line.find("#") == 0) {		//comment lines are ignored
			continue;
		}


		int pos1 = line.find(";");
		int pos2 = line.find(";", pos1+1);
		int seqNum = 0;
		int location = 0;
		double value = 0;

		try {
			seqNum = stoi(line.substr(0,pos1));
			location = stoi(line.substr(pos1+1, pos2-pos1-1));
			value = stod(line.substr(pos2+1, line.length()-pos2-1));
		} catch (...) {
			err++;												//test format of line
			continue;
		}


		if (location < 1 || location > 2 || isnan(value) || value <=0 ) {			//location must be positive
			err++;
			continue;
		}

		values[--location].push_back(value);
		geoMean[location] += log(value);
		nLoc[location]++;


	}

	for (int i = 0; i < 2; i++) {
		geoMean[i] = exp(geoMean[i] / (double)nLoc[i]);
	}

	// writing output
	cout << "File: " << args[1] << " with " << nLines << " lines" << endl;
	for (int i = 0; i < 2; i++) {
		cout << "Valid values Loc " << (i+1) << ": " << nLoc[i] << " with GeoMean " << geoMean[i] << endl;
	}
	cout << "Number of errors in the file: " << err << endl;

	exit(EXIT_SUCCESS);
}

