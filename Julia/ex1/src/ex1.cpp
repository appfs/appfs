#include <fstream>
#include <iostream>
#include <cmath>
#include <vector>
#include <string>
#include <sstream>

using namespace std;

const string FILE_PATH= "ex1.dat";

/**
 * checks if line is empty or a comment
 */
bool lineIsValid(string line){
	if (line.empty()){
		return false;
	}
	size_t pos = line.find('#');
	if (pos == 0) {
		cout << "Ignore comment line" << endl;
			return false;
		}
	return true;
}


/*
 * computes the geoMean of a vector
 */
double geoMean(vector<double> vec, int length){
	double sum = 0.;
	for (int i = 0; i < length; ++i) {
		sum += log(vec.at(i));
	}
	sum /= length;
	return exp(sum);
}



int main(int argc, char* argv[]){
	ifstream infile;

	if(argc <=1){
		cout << "No file-path found. Try to open default file path." << endl;
		infile.open(FILE_PATH, ios::in);
	}
	else {
		infile.open(argv[1], ios::in);
	}

	if (!infile){
		cout << "File could not be opened." << endl;
		return 1;
	}

	string line;
	int lineCount=0;
	vector<double> loc1;
	vector<double> loc2;

	double location;
	double value;

	char overage[2];

	while (getline(infile, line)){
		lineCount++;
		if(lineIsValid(line)){
		 int values = sscanf(line.c_str(), "%*d ; %lf ; %lf %1s", &location, &value, overage);
		 if (values != 2) {
			cerr << "Error while parsing data" << endl;
			continue;
		 }if (isnan(value) || value <= 0){
			 cerr << "Invalid value" << endl;
			 continue;
		 }
			 if (location == 1){
				 loc1.push_back(value);
			 } else if (location == 2) {
				 loc2.push_back(value);
			 }

		}
	}

	int sizeLoc1 = loc1.size();
	int sizeLoc2 = loc2.size();


	cout << "File: ex1.dat with " << lineCount << " lines" << endl;
	cout << "Valid values Loc1: " << sizeLoc1 << " with GeoMean: " << geoMean(loc1, sizeLoc1) << endl;
	cout << "Valid values Loc2: " << sizeLoc2 << " with GeoMean: " << geoMean(loc2, sizeLoc2) << endl;

	return 1;
 }


