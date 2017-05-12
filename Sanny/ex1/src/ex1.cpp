/**
 * Exercise 1 : Read in a csv-file, save the valid values und print the geometric means of position 1 & 2.
 *
 * @author FirstSanny
 */

#include <fstream>
#include <iostream>
#include <iomanip>
#include <sstream>
#include <string>
#include <cstring>
#include <algorithm>
#include "LocationValues.h"

namespace {

const char* DEFAULT_FILE_NAME = "recources/ex1.dat";
const char* SEPERATOR = ";";

}

using namespace std;

void removeSeperator(string& locationString) {

}

/** Opens either a file, which was given by invocation, or the default file, which is hard-coded. */
string inline openFile(const int argn, char* argv[], ifstream& fileStream) {
	if (argn <= 1) {
		cout << "There was no filename. The standard " << DEFAULT_FILE_NAME
				<< " will be used." << endl;
		fileStream.open(DEFAULT_FILE_NAME, ios::in);
		return DEFAULT_FILE_NAME;
	} else {
		fileStream.open(argv[1], ios::in);
		return argv[1];
	}
}


/** Print the size and the geometric mean of LocationValues */
void inline printValues(LocationValues valuesLocation) {
	cout << "Valid values Loc1: " << valuesLocation.size() << " with GeoMean: "
			<< fixed << setprecision(4) << valuesLocation.getGeoMean() << endl;
}


 /** Opens a file, parse the values and returns the geometric means of the locations */
int main(int argn, char *argv[]) {

	ifstream fileStream;
	const string filename = openFile(argn, argv, fileStream);

	if ( (fileStream.rdstate()) != 0 ){
	    perror("Encoutered Problem");
	    return 0;
	}

	cout << "Opened File "<< filename << " successfully" << endl;

	istringstream strin;
	string line;

	int lineCount = 0;

	LocationValues valuesLocation1 = LocationValues();
	LocationValues valuesLocation2 = LocationValues();

	while (getline(fileStream, line)){

		lineCount++;

		const char* shortenedline = strpbrk(line.c_str(), "#\n");
		if(shortenedline != nullptr){
			line = shortenedline;
		}

		const string::size_type indexOfFirstSeperator = line.find(SEPERATOR);
		const string::size_type indexOfSecondSeperator = line.find(SEPERATOR, indexOfFirstSeperator+1);
		if(indexOfFirstSeperator == string::npos || indexOfSecondSeperator == string::npos){
			line.clear();
			continue;
		}

		int location = 0;
		double value = 0;
		char rest[2];

		int values = sscanf(line.c_str(), "%*d ; %d ; %lf %1s", &location, &value, rest);
		if(values!=2){
			line.clear();
			continue;
		}

		if(location == 1){
			valuesLocation1.push_back(value);
		} else if(location == 2){
			valuesLocation2.push_back(value);
		}
		line.clear();
	}

	cout << "File: " << filename << " with " << lineCount << " lines" << endl;

	printValues(valuesLocation1);
	printValues(valuesLocation2);

	fileStream.close();

	return 1;

}
