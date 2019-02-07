/*
 * Share.h
 *
 *  Created on: Jun 21, 2015
 *      Author: daniel
 */

#ifndef SHARE_H_
#define SHARE_H_
#include "Security.h"
namespace mtm{

class Share:public Security {
	double initValue;
	int days;
	double sum;
	QuarterType quarter;
public:
	Share(string name,double value,int amount,QuarterType quarter);
	~Share() = default;
	bool operator==(const Share& s);
	Share& operator=(const Share& b);
	double& getInitVal();
	void setNextDay();
	void nextQuarter();
	int& getDays();
};
}

#endif /* SHARE_H_ */
