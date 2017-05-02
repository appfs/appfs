/*
 * MeasurePoint.cpp
 *
 *  Created on: 02.05.2017
 *      Author: alesan
 */

#include "MeasurePoint.h"


MeasurePoint::MeasurePoint(double power, int hour, time_t date) {
	this->power = power;
	this->hour = hour;
	this->date = date;

}

MeasurePoint::~MeasurePoint() {
	// nop
}

