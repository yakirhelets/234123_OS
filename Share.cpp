/*
 * Share.cpp
 *
 *  Created on: Jun 21, 2015
 *      Author: daniel
 */
#include "Share.h"
using mtm::Share;
mtm::Share::Share(string name,double value,int amount,QuarterType quarter):
	Security(name,value,amount),initValue(value),days(),sum(),quarter(quarter) {}

bool mtm::Share::operator==(const Share& s) {
	return name==s.name;
}

Share& mtm::Share::operator=(const Share& b) {
	if (this==&b) {
		return *this;
	}
	name=b.name;
	value=b.value;
	initValue=b.initValue;
	amount=b.amount;
	return *this;
}

double& mtm::Share::getInitVal() {
	return initValue;
}

void mtm::Share::setNextDay() {
	this->days++;
	this->sum+=this->value;
	if (this->days==5) {
		//cout<<"what quartedfgdfgmsdglkndsfkgndsfkgnsdkjgnkfngkjngkdfgnkdgnkdgndkgnkjr "<<this->quarter<<endl;
		this->assessment[quarter]=(double)this->sum/5;
	}
}

void mtm::Share::nextQuarter() {
	this->sum=0;
	this->days=0;
	switch(this->quarter) {
	case FIRST_QUARTER:this->quarter=SECOND_QUARTER;break;
	case SECOND_QUARTER:this->quarter=THIRD_QUARTER;break;
	case THIRD_QUARTER:this->quarter=FOURTH_QUARTER;break;
	case FOURTH_QUARTER:this->quarter=FIRST_QUARTER;break;
	}
}

int& mtm::Share::getDays() {
	return this->days;
}

