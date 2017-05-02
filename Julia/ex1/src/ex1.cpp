#include <fstream>
#include <iostream>
#include <cmath>
#include <vector>
#include <string>
#include <sstream>

using namespace std;

const char delimiter = ';';

/**
 * checks if line is empty or a comment
 */
bool lineIsValid(string line){
	if (line.empty()){
		return false;
	}
	if (line[0] == '#'){
		return false;
	}
	return true;
}

/**
 * returns the location of a line
 */
int getLocation(string line){
	int location = 0;
	istringstream stream(line);
	string e1, e2, e3;
	stream >> e1 >> location >> e3;
	return location;
}

/*
 * returns the value of a line
 */
double getValue(string line){
	double value = 0;
	size_t pos = line.find_first_of(';');
	stringstream val;
	if (pos != string::npos){
		line = line.substr(pos + 2);
		pos = line.find_first_of(';');
		if (pos != string::npos){
			val.str(line.substr(pos + 2));
			val >> value;
			val.clear();
		}
	}
	return value;
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
		infile.open("ex1.dat", ios::in);
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

	int location;
	double value;

	while (getline(infile, line)){
		lineCount++;
		if (lineIsValid(line)){
			location = getLocation(line);
			value = getValue(line);
			if (!isnan(value) && !isinf(value) && value > 0 ){
				if (location == 1){
					loc1.push_back(value);
				}else if (location == 2) {
					loc2.push_back(value);
				}
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


