#include <fstream>
#include <iostream>
#include <cmath>
#include <vector>
#include <string>
#include <sstream>

using namespace std;



bool lineIsValid(string line){
	if (line.empty()){
		return false;
	}
	if (line[0] == '#'){
		return false;
	}
	return true;
}

int getLocation(string line){
	int location = 0;
	size_t pos = line.find_first_of(';');
	if (pos != string::npos){
		line = line.substr(pos + 2);
		if (pos != string::npos){
			pos = line.find_first_of(';');
			stringstream loc(line.substr(0, pos));
			loc >> location;
		}
	}
	return location;
}

float getValue(string line){
	float value = 0;
	size_t pos = line.find_first_of(';');
	if (pos != string::npos){
		line = line.substr(pos + 2);
		if (pos != string::npos){
			pos = line.find_first_of(';');
			stringstream val(line.substr(pos + 2));
			val >> value;
		}
	}
	return value;
}


float geoMean(vector<float> vec, long length){
	float sum = 0.f;
	for (long i = 0; i < length; ++ i) {
		sum += log(vec.at(i));
	}
	sum /= length;
	return exp(sum);
}




int main(){
	ifstream infile("ex1.dat", ios::in);
	if (!infile){
		cout << "Datei konnte nicht geöffnet werden" << endl;
		return 1;
	}

	string line;
	vector<float> loc1;
	vector<float> loc2;


	while (getline(infile, line)){
		if (lineIsValid(line)){
			int location = getLocation(line);
			float value = getValue(line);
			if (location == 1){
				loc1.push_back(value);
			} if (location == 2) {
				loc2.push_back(value);
			}
		}
	}

	long sizeLoc1 = loc1.size();
	long sizeLoc2 = loc2.size();


	cout << "File: ex1.dat with " << sizeLoc1 + sizeLoc2 << " lines" << endl;
	cout << "Valid values Loc1: " << sizeLoc1 << " with GeoMean: " << geoMean(loc1, sizeLoc1) << endl;
	cout << "Valid values Loc2: " << sizeLoc2 << " with GeoMean: " << geoMean(loc2, sizeLoc2) << endl;

	return 0;
 }


