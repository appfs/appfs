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

		if (line.empty()) {			// empty lines are ignored
			continue;
		}

		size_t foundComment = line.find("#");

		if (foundComment == 0) {		// comment lines are ignored
			continue;
		}

		if (foundComment != string::npos) {			// anything after # is ignored
			line = line.substr(0, foundComment);
		}


		size_t pos1 = line.find(";");
		if (pos1 == string::npos) {
			err++;
			continue;
		}												// test if line contains two ';'
		size_t pos2 = line.find(";", pos1+1);
		if (pos2 == string::npos) {
			err++;
			continue;
		}

		int seqNum = 0;
		int location = 0;
		double value = 0;

		try {
			string parts[] = {"","",""};
			parts[0] = line.substr(0, pos1);
			parts[1] = line.substr(pos1+1, pos2-pos1-1);					// split line at ';' and trim
			parts[2] = line.substr(pos2+1, line.length()-pos2-1);			// the parts
			bool foundSpace = false;
			for (int i = 0; i < 3; i++) {
				size_t first = parts[i].find_first_not_of(' ');				// if one part still contains
				size_t last = parts[i].find_last_not_of(' ');				// a whitespace, the line is
				parts[i] = parts[i].substr(first, last-first+1);			// treated as an error
				if (parts[i].find(' ') != string::npos) {
					foundSpace = true;
				}
			}
			if (foundSpace) {
				err++;
				continue;
			}
			seqNum = stoi(parts[0]);
			location = stoi(parts[1]);			// if parsing fails, exception is thrown and caught
			value = stod(parts[2]);

		} catch (...) {
			err++;
			continue;
		}


		if (location < 1 || location > 2 || !isnormal(value) || value <= 0. ) {			// value must be positive
			err++;
			continue;
		}

		// here the values are stored
		values[--location].push_back(value);
		geoMean[location] += log(value);
		nLoc[location]++;


	}

	// geometric means are calculated
	for (int i = 0; i < 2; i++) {
		geoMean[i] = exp(geoMean[i] / (double)nLoc[i]);
	}

	// writing output
	cout << "File: " << args[1] << " with " << nLines << " lines" << endl;
	for (int i = 0; i < 2; i++) {
		cout << "Valid values Loc" << (i+1) << ": " << nLoc[i] << " with GeoMean " << geoMean[i] << endl;
	}
	cout << "Number of errors in the file: " << err << endl;

	exit(EXIT_SUCCESS);
}

