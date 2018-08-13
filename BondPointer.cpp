/*
 * BondPointer.cpp
 *
 *  Created on: Jun 25, 2015
 *      Author: daniel
 */
#include "BondPointer.h"
using mtm::shared_ptr;
using mtm::BondPointer;


mtm::BondPointer::BondPointer(shared_ptr<Bond> bond,int amount):
		bondPtr(bond),value(bond->getValue()),initVal(bond->getValue()),amount(amount) {}
mtm::BondPointer::BondPointer(const BondPointer& bp):
		bondPtr(bp.bondPtr),value(bp.bondPtr->getValue()),initVal(bp.bondPtr->getValue()),amount(bp.amount) {}
shared_ptr<Bond> mtm::BondPointer::operator->() {
	return this->bondPtr;
}
const double& mtm::BondPointer::getValue() const {
	return this->value;
}
const double& mtm::BondPointer::getInitVal() const {
	return this->initVal;
}
const int& mtm::BondPointer::getAmount() const {
	return this->amount;
}
void mtm::BondPointer::setValue(const double& value) {
	this->value=value;
}
bool mtm::BondPointer::operator==(const BondPointer& bp) {
	return this->bondPtr==bp.bondPtr;
}
BondPointer& mtm::BondPointer::operator=(const BondPointer& bp) {
	if (this==&bp) {
		return *this;
	}
	this->bondPtr=bp.bondPtr;
	this->value=bp.value;
	this->initVal=bp.initVal;
	this->amount=bp.amount;
	return *this;
}
double mtm::BondPointer::getTotalValue() const{
	return this->amount*this->value;
}
