#include <fstream>
#include <iostream>
#include <iomanip>
#include <sstream>
#include <string>
#include "LocationValues.h"

namespace {

const char* DEFAULT_FILE_NAME = "ex1.dat";
const char* SEPERATOR = "; ";
const char* COMMENT_SIGN = "#";

}

using namespace std;

/** Check if the value is greater 0. If this is correct, the line is added. */
void addLine(const string line, const string::size_type indexOfSecondSeperator, istringstream& strin,
		LocationValues& locationValues) {
	string valueString = line.substr(indexOfSecondSeperator);
	valueString.replace(0,2,"");
	double value;
	strin.str(valueString);
	strin >> value;
	locationValues.push_back(value);
	strin.clear();
}

void inline printValues(LocationValues valuesLocation) {
	cout << "Valid values Loc1: " << valuesLocation.size() << " with GeoMean: "
			<< fixed << setprecision(4) << valuesLocation.getGeoMean() << endl;
}

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

int main(int argn, char *argv[]) {

	fstream fileStream;
	const string filename = openFile(argn, argv, fileStream);

	if ( (fileStream.rdstate()) != 0 ){
	    perror("Encoutered Problem");
	    return 0;
	}

	istringstream strin;
	string line;

	int lineCount = 0;

	LocationValues valuesLocation1 = LocationValues();
	LocationValues valuesLocation2 = LocationValues();

	while (!fileStream.eof()){
		getline(fileStream, line);

		lineCount++;

		if(line.find(COMMENT_SIGN) == 0){
			continue;
		}

		const string::size_type indexOfFirstSeperator = line.find(SEPERATOR);
		const string::size_type indexOfSecondSeperator = line.find(SEPERATOR, indexOfFirstSeperator+1);

		if(indexOfFirstSeperator == string::npos || indexOfSecondSeperator == string::npos){
			continue;
		}

		string locationString = line.substr(indexOfFirstSeperator, indexOfSecondSeperator - indexOfFirstSeperator);
		locationString.replace(0,2,"");

		int location;
		strin.str(locationString);
		strin >> location;
		strin.clear();

		if(location == 1){
			addLine(line, indexOfSecondSeperator, strin, valuesLocation1);
		}

		if(location == 2){
			addLine(line, indexOfSecondSeperator, strin, valuesLocation2);
		}

	}

	cout << "File: " << filename << " with " << lineCount << " lines" << endl;

	printValues(valuesLocation1);
	printValues(valuesLocation2);

	fileStream.close();

	return 0;

}
