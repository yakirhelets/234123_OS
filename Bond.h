/*
 * SecurityType.h
 *
 *  Created on: Jun 20, 2015
 *      Author: daniel
 */

#ifndef BOND_H_
#define BOND_H_
#include "Security.h"
namespace mtm{
class Bond:public Security {
	double initValue;
public:
	Bond(string name,double value,int amount);
	Bond(const Bond& b);
	~Bond() = default;
	bool operator==(const Bond& s);
	Bond& operator=(const Bond& b);
	double& getInitVal();
};
}



#endif /* BOND_H_ */
