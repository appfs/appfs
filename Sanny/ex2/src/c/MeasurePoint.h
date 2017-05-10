/*
 * MeasurePoint.h
 *
 *  Created on: 02.05.2017
 *      Author: alesan
 */

#ifndef C_MEASUREPOINT_H_
#define C_MEASUREPOINT_H_

#include <ctime>
#include <string>

using namespace std;

class MeasurePoint {

private:
	double power;
	int hour;
	/**tm*/ string date;

public:
	MeasurePoint(double power, int hour, /**tm*/ string date);
	virtual ~MeasurePoint();
	double getPower();
	int getHour();
	string getDate();
};

#endif /* C_MEASUREPOINT_H_ */
