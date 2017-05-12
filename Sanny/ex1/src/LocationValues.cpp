/*
 * LocationValues.cpp
 *
 *  Created on: 24.04.2017
 *      Author: alesan
 */

#include "LocationValues.h"

namespace std {

double LocationValues::getGeoMean() {
	double geoMean = 0.;
	const unsigned int count = vector::size();

	for (unsigned int i = 0; i < count; ++i) {
		geoMean += log(vector::at(i));
	}

	geoMean /= count;

	return exp(geoMean);
}

void LocationValues::push_back(const double value) {
	if(value <= 0 || isnan(value)){
		return;
	}
	vector::push_back(value);
}

} /* namespace std */
