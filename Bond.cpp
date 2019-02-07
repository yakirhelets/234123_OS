/*
 * Bond.cpp
 *
 *  Created on: Jun 21, 2015
 *      Author: daniel
 */

#include "Bond.h"
using mtm::Bond;
mtm::Bond::Bond(string name,double value,int amount):
Security(name,value,amount),initValue(value) {}

mtm::Bond::Bond(const Bond& b):
		Security(b.name,b.value,b.amount),initValue(b.initValue) {}
bool mtm::Bond::operator==(const Bond& s) {
		return name==s.name;
	}

Bond& mtm::Bond::operator=(const Bond& b) {
		if (this==&b) {
			return *this;
		}
		name=b.name;
		value=b.value;
		initValue=b.initValue;
		amount=b.amount;
		return *this;
	}

double& mtm::Bond::getInitVal() {
		return initValue;
	}
