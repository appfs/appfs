/*
 * ex1.cpp
 *
 * 	\brief     First exercixe
 *  \details   This class reads in a data document and counts the valid lines
 *  \pre       Create the data file
 *  \author    Julia Baumbach
 *  \date      25.04.2017
 */

#include <fstream>
#include <iostream>
#include <cmath>
#include <vector>
#include <string>
#include <sstream>

using namespace std;

/**
 * \paran SOURCE_FILE_PATH default file name
 * if none is set while starting the program
 */
const string SOURCE_FILE_PATH= "ex1.dat";

/**
 * \fn bool lineIsValid(string line)
 * \brief checks if line is empty or a comment
 * \param line a string representing a line
 * \return returns if the given line is valid
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


/**
 * \fn double geoMean(vector<double> vec, int length)
 * \brief computes the geoMean of a vector
 * \param vec a vector with the data as double
 * \param length the length of the given vector
 * \return the geometric mean of the data of the vector
 */
double geoMean(vector<double> vec, int length){
	double sum = 0.;
	for (int i = 0; i < length; ++i) {
		sum += log(vec.at(i));
	}
	sum /= length;
	return exp(sum);
}


/**
 * Main function which reads in a document file
 * and stores its data for computing the geometric mean
 * Shows a warning if no file path name is given while
 * starting the program.
 * Stops if no file could be found.
 */
int main(int argc, char* argv[]){
	ifstream infile;

	if(argc <=1){
		cout << "No file-path found. Try to open default file path." << endl;
		infile.open(SOURCE_FILE_PATH, ios::in);
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

	return 0;
 }


