/**
 * Exercise 1 : Read in a csv-file, save the valid values und print the geometric means of position 1 & 2.
 *
 * @author FirstSanny
 */

#include <fstream>
#include <iostream>
#include <iomanip>
#include <string>
#include <cstring>
#include <algorithm>
#include <cmath>

/** Some Constants to avoid "magic"-signs */
namespace {

const char* DEFAULT_FILE_NAME = "ex1.dat";
const char* SEPERATOR = ";";

}

/** Using namespace cause of laziness. */
using namespace std;

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

/** Opens either a file, which was given by invocation, or the default file, which is hard-coded. */
bool push(vector<double> *values, const double value) {
	if(value <= 0 || isnan(value)){
		return false;
	}
	values->push_back(value);
	return true;
}

/** Computes the geometric mean via the logartihm. */
double getGeoMean(vector<double> *values) {
	double geoMean = 0.;
	const unsigned int count = values->size();

	for (unsigned int i = 0; i < count; ++i) {
		geoMean += log(values->at(i));
	}

	geoMean /= count;

	return exp(geoMean);
}

/** Print the size and the geometric mean of LocationValues */
void inline printValues(vector<double> *values) {
	cout << "Valid values Loc1: " << values->size() << " with GeoMean: "
			<< fixed << setprecision(6) << getGeoMean(values) << endl;
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

	vector<double> *valuesLocation1 = new vector<double>();
	vector<double> *valuesLocation2 = new vector<double>();

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
			push(valuesLocation1,value);
		} else if(location == 2){
			push(valuesLocation2, value);
		}
		line.clear();
	}

	cout << "File: " << filename << " with " << lineCount << " lines" << endl;

	printValues(valuesLocation1);
	printValues(valuesLocation2);

	fileStream.close();

	return 1;

}
