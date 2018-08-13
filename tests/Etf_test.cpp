/*
 * Etf_test.cpp
 *
 *  Created on: Jun 25, 2015
 *      Author: daniel
 */
#include "Etf.h"
#include "mtmtest.h"
using mtm::shared_ptr;
using mtm::Bond;
using mtm::BondPointer;
//this test checks if adding works by printing the list
bool testAddShare() {
	Etf etf;
	shared_ptr<Share> ptr1(new Share("MSDN",135,6,FIRST_QUARTER));
	int amount=3;
	SharePointer ptr(ptr1,amount);
	etf.addShare(ptr);
	etf.printShares();
	ptr.setValue(156.56);
	etf.printShares();
	return true;
}
bool testAddBond() {
	Etf etf;
	shared_ptr<Bond> ptr1(new Bond("MSDN",135,6));
	int amount=3;
	BondPointer ptr(ptr1,amount);
	etf.addBond(ptr);
	etf.printBonds();
	ptr.setValue(165);
	etf.printBonds();
	return true;
}

bool testPrintBonds() {
	Etf etf;
	shared_ptr<Bond> ptr1(new Bond("MSDN",135,6));
	int amount=3;
	BondPointer ptr(ptr1,amount);
	shared_ptr<Bond> ptr2(new Bond("INTL",135,6));
	BondPointer ptr3(ptr2,amount);
	shared_ptr<Bond> ptr4(new Bond("IBM",135,6));
	BondPointer ptr5(ptr4,amount);
	etf.addBond(ptr);
	etf.addBond(ptr3);
	etf.addBond(ptr5);
	std::cout<<endl;
	etf.printBonds();
	return true;
}

bool testPrintShares() {
	Etf etf;
	shared_ptr<Share> ptr1(new Share("MSDN",135,6,FIRST_QUARTER));
	int amount=3;
	SharePointer ptr(ptr1,amount);
	shared_ptr<Share> ptr2(new Share("INTL",135,6,FIRST_QUARTER));
	SharePointer ptr3(ptr2,amount);
	shared_ptr<Share> ptr4(new Share("IBM",135,6,FIRST_QUARTER));
	SharePointer ptr5(ptr4,amount);
	etf.addShare(ptr);
	etf.addShare(ptr3);
	etf.addShare(ptr5);
	ptr3.setValue(125);
	std::cout<<endl;
	etf.printShares();
	return true;
}


