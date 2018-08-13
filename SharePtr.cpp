/*
 * SharePtr.cpp
 *
 *  Created on: Jun 23, 2015
 *      Author: daniel
 */
#include "SharePtr.h"
using namespace mtm;

mtm::SharePointer::SharePointer(shared_ptr<Share> share,int amount):
		sharePtr(share),amount(amount),initVal(share->getValue()) {}
mtm::SharePointer::SharePointer(const SharePointer& p):
		sharePtr(p.sharePtr),amount(p.amount),initVal(p.initVal) {}
void  mtm::SharePointer::setValue(const double& value) {
	this->sharePtr->setValue(value);
}

string mtm::SharePointer::getName() {
	return this->sharePtr->getName();
}
double mtm::SharePointer::getValue() const{
	return this->sharePtr->getValue();
}
const int& mtm::SharePointer::getAmount() const{
	return this->amount;
}
const shared_ptr<Share>& mtm::SharePointer::getPointer() const {
	return this->sharePtr;
}
const double& mtm::SharePointer::getInitVal() const{
	return this->initVal;
}
SharePointer& mtm::SharePointer::operator=(const SharePointer& sp) {
	if (this==&sp) {
		return *this;
	}
	this->amount=sp.getAmount();
	this->initVal=sp.getInitVal();
	this->sharePtr=sp.getPointer();
	return *this;
}

bool mtm::SharePointer::operator==(const SharePointer& sp) {
	return sp.getPointer()==this->getPointer();
}

double mtm::SharePointer::getTotalValue() const{
	return this->getValue()*this->amount;
}



