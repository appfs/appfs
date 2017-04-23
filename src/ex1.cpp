#include <fstream>
#include <iostream>
#include <iomanip>
#include <sstream>
#include <string>

namespace {

const char* DEFAULT_FILE_NAME = "ex1.dat";
const char* SEPERATOR = "; ";
const char* COMMENT_SIGN = "#";

}

using namespace std;

/** Check if the value is greater 0. If this is correct, the line is added. */
void addLineIfValid(const string line, const string::size_type indexOfSecondSeperator, istringstream& strin,
		int& countValidLocation1, double& valueLocation1) {
	string valueString = line.substr(indexOfSecondSeperator);
	valueString.replace(0,2,"");
	double value;
	strin.str(valueString);
	strin >> value;
	if (value > 0) {
		countValidLocation1++;
		valueLocation1 += value;
	}
	strin.clear();
}

int main(int argn, char *argv[]) {

	fstream fileStream;
	string filename;

	if(argn <= 1){
		cout << "There was no filename. The standard "<< DEFAULT_FILE_NAME << " will be used." << endl;
		fileStream.open(DEFAULT_FILE_NAME, ios::in);
		filename = DEFAULT_FILE_NAME;
	} else {
		fileStream.open(argv[1], ios::in);
		filename = argv[1];
	}

	if ( (fileStream.rdstate() & ifstream::badbit ) != 0 ){
	    cerr << "ERROR: Could not open the file " << DEFAULT_FILE_NAME << endl;
	    return 0;
	}

	istringstream strin;
	string line;

	int lineCount = 0;

	int countValidLocation1 = 0;
	double valueLocation1 = 0.;

	int countValidLocation2 = 0;
	double valueLocation2 = 0.;

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
			addLineIfValid(line, indexOfSecondSeperator, strin, countValidLocation1,
					valueLocation1);
		}

		if(location == 2){
			addLineIfValid(line, indexOfSecondSeperator, strin, countValidLocation2,
					valueLocation2);
		}

	}

	cout << "File: " << filename << " with " << lineCount << " lines" << endl;

	cout << "Valid values Loc1: " << countValidLocation1 << " with GeoMean: " << fixed << setw(8) << setprecision(4) << valueLocation1/countValidLocation1 << endl;
	cout << "Valid values Loc2: " << countValidLocation2 << " with GeoMean: " << fixed << setw(8) << setprecision(4) << valueLocation2/countValidLocation2  << endl;

	fileStream.close();

	return 0;

}
