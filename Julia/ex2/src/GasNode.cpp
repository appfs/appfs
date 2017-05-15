/**
 * Class for representing a GasNode.
 * Saves the hour of its measure and the measured value.
 */

#include "GasNode.h"
#include <string>
#include <sstream>

int GasNode::getPower(){
	return this->power;
}


void GasNode::setPower(int power){
	this->power = power;
}

int GasNode::getTime(){
	return this->time;
}

void GasNode::setTime(int time){
	this->time = time;
}


