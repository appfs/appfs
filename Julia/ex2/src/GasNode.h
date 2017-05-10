
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
	int time;
	int power;
};

#endif
