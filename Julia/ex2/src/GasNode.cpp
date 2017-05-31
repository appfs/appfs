/*
 * GasNode.cpp
 *
 * 	\brief     Class for representing a GasNode.
 *  \details   Saves the hour of its measure and the measured value.
 *  \author    Julia Baumbach
 *  \date      01.05.2017
 */

#include "GasNode.h"
#include <string>
#include <sstream>

/**
 * \fn int GasNode::getPower()
 * \brief gets the power of a gasNode
 * \return the power of a gasNode
 */
int GasNode::getPower(){
	return this->power;
}

/**
 * \fn void GasNode::setPower(int power)
 * \brief sets the power of a gasNode to the given value
 * \param power the power to set
 */
void GasNode::setPower(int power){
	this->power = power;
}

/**
 * \fn int GasNode::getTime()
 * \brief gets the time of a gasNode
 * \return the time of a gasNode
 */
int GasNode::getTime(){
	return this->time;
}


/**
 * \fn void GasNode::setTime(int time)
 * \brief sets the time of a gasNode to the given value
 * \param time the time to set
 */
void GasNode::setTime(int time){
	this->time = time;
}


