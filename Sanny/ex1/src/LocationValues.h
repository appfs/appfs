/*
 * LocationValues.h
 *
 *  Created on: 24.04.2017
 *      Author: alesan
 */

#include <vector>
#include <cmath>
#include <iostream>

#ifndef LOCATIONVALUES_H_
#define LOCATIONVALUES_H_

namespace std {

class LocationValues : public vector<double> {

public:
	double getGeoMean();
	bool push_back(const double);
};

} /* namespace std */

#endif /* LOCATIONVALUES_H_ */
