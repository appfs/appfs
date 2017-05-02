/*
 * MeasurePoint.h
 *
 *  Created on: 02.05.2017
 *      Author: alesan
 */

#ifndef C_MEASUREPOINT_H_
#define C_MEASUREPOINT_H_

#include <ctime>

using namespace std;

class MeasurePoint {

private:
	double power;
	int hour;
	time_t date;

public:
	MeasurePoint(double power, int hour, time_t date);
	virtual ~MeasurePoint();
};

#endif /* C_MEASUREPOINT_H_ */
