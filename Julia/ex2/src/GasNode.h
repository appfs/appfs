/*
 * GasNode.h
 *
 * 	\brief     Class for representing a GasNode.
 *  \details   Saves the hour of its measure and the measured value.
 *  \author    Julia Baumbach
 *  \date      01.05.2017
 */

#ifndef GASNODE_H_
#define GASNODE_H_

class GasNode {
public:
	int getTime();
	int getPower();
	void setTime(int);
	void setPower(int);
	void getInformation(int);

private:
	/**
	 * \var field to store the time of a gas node
	 */
	int time;
	/**
	 * \var field to store the power of a gas node
	 */
	int power;
};

#endif
