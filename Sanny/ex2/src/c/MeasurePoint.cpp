/*
 * MeasurePoint.cpp
 *
 *  Created on: 02.05.2017
 *      Author: alesan
 */

#include "MeasurePoint.h"


MeasurePoint::MeasurePoint(double power, int hour, /**tm*/ string date) {
	this->power = power;
	this->hour = hour;
	this->date = date;

}

MeasurePoint::~MeasurePoint() {
	//nop
}

double MeasurePoint::getPower(){
	return power;
}
int MeasurePoint::getHour(){
	return hour;
}

string MeasurePoint::getDate(){
	//	char buff[10];
	//	strftime(buff, 10, "%Y-%m-%d", &date);
	//	return buff;
	return date;
}

