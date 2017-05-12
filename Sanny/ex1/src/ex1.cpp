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
#include <algorithm>
#include "LocationValues.h"

namespace {

const char* DEFAULT_FILE_NAME = "recources/ex1.dat";
const char* SEPERATOR = ";";
const char* COMMENT_SIGN = "#";

}

using namespace std;

void removeSeperator(string& locationString) {

}

/** Opens either a file, which was given by invocation, or the default file, which is hard-coded. */
string inline openFile(const int argn, char* argv[], fstream& fileStream) {
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

void inline removeWhitespaces(string& line) {
	line.erase(std::remove_if(line.begin(), line.end(), &::isspace),
			line.end());
}



/** Print the size and the geometric mean of LocationValues */
void inline printValues(LocationValues valuesLocation) {
	cout << "Valid values Loc1: " << valuesLocation.size() << " with GeoMean: "
			<< fixed << setprecision(4) << valuesLocation.getGeoMean() << endl;
}

/** Converts a String to an double */
double inline getDoubleFromString(const string& locationString, istringstream& strin) {
	double d;
	strin.str(locationString);
	strin >> d;
	strin.clear();
	return d;
}


 /** Opens a file, parse the values and returns the geometric means of the locations */
int main(int argn, char *argv[]) {

	fstream fileStream;
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

		const string::size_type indexOfCommentSign = line.find_first_of(COMMENT_SIGN);

		if(indexOfCommentSign == 0){
			continue;
		}

		if(indexOfCommentSign == string::npos){
			line = line.substr(0,indexOfCommentSign);
		}

		removeWhitespaces(line);

		const string::size_type indexOfFirstSeperator = line.find(SEPERATOR);
		const string::size_type indexOfSecondSeperator = line.find(SEPERATOR, indexOfFirstSeperator+1);

		if(indexOfFirstSeperator == string::npos || indexOfSecondSeperator == string::npos){
			continue;
		}

		string locationString = line.substr(indexOfFirstSeperator, indexOfSecondSeperator - indexOfFirstSeperator);
		string valueString = line.substr(indexOfSecondSeperator);

		locationString.erase(0,1);
		valueString.erase(0,1);

		double location = getDoubleFromString(locationString, strin);

		if(location == 1.){
			valuesLocation1.push_back(getDoubleFromString(valueString, strin));
		}

		if(location == 2.){
			valuesLocation2.push_back(getDoubleFromString(valueString, strin));
		}

		line.clear();
	}

	cout << "File: " << filename << " with " << lineCount << " lines" << endl;

	printValues(valuesLocation1);
	printValues(valuesLocation2);

	fileStream.close();

	return 1;

}
