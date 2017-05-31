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

/** Class for combining the values of a measurement */
class MeasurePoint {

private:
	/** measured power */
	double power;
	/** hour for measurement */
	int hour;
	/** measurement date */
	/**tm*/ string date;

public:
	/** constructor with all values */
	MeasurePoint(double power, int hour, /**tm*/ string date);
	/** deconstructor */
	virtual ~MeasurePoint();
	/** Getter for the power */
	double getPower();
	/** Getter for the hour */
	int getHour();
	/** Getter for the date */
	string getDate();
};

#endif /* C_MEASUREPOINT_H_ */
