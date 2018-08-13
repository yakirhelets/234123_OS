/*
 * Etf.cpp
 *
 *  Created on: Jun 23, 2015
 *      Author: daniel
 */
#include <iostream>
#include "PrintHelper.h"
using std::cin;
using std::cout;
using std::endl;
#include "Etf.h"
using namespace mtm;
#define epsilon 0.0001
double abs(double num) {
	if (num<0) {
		return -num;
	}
	return num;
}
class IsEqual {
public:
	bool operator()(BondPointer val1,BondPointer val2) const {
		if (val1.getTotalValue()>val2.getTotalValue()) {
			return true;
		}
		if(abs(val1.getTotalValue()-val2.getTotalValue())<epsilon) {
			if(val1->getName()>val2->getName()) {
				return true;
			}
		}
		return false;
	}
	bool operator()(SharePointer val1,SharePointer val2) const{
		if (val1.getTotalValue()>val2.getTotalValue()) {
			return true;
		}
		if(abs(val1.getTotalValue()-val2.getTotalValue())<epsilon) {
			if(val1.getPointer()->getName()>val2.getPointer()->getName()) {
				return true;
			}
		}
				return false;
	}
};
bool compare1(BondPointer& val1,BondPointer& val2){
		if (val1.getTotalValue()<val2.getTotalValue()) {
		return true;
		}
		else if(abs(val1->getTotalValue()-val2->getTotalValue())<epsilon) {
			if(val1->getName()>val2->getName()) {
				return true;
			}
		}
		return false;
	}

mtm::Etf::Etf():
		shares(),bonds(){}
mtm::Etf::Etf(const Etf& e):
	shares(),
	bonds() {
		shares.listCopy(e.shares);
		bonds.listCopy(e.bonds);
}

void mtm::Etf::printBonds() {
	this->bonds.sort(IsEqual());
	for (Iterator<BondPointer> i =this->bonds.begin();
			i!=this->bonds.end();++i) {
		printBondCustomerDetails((*i)->getName(),(*i).getTotalValue(),
				(*i).getTotalValue()-(*i).getInitVal()*(*i).getAmount());
	}
}

void mtm::Etf::printShares() {
	this->shares.sort(IsEqual());
	for (Iterator<SharePointer> i = this->shares.begin();
			i!=this->shares.end();++i) {
		printShareCustomerDetails((*i).getPointer()->getName(),(*i).getTotalValue(),
				(*i).getTotalValue()-(*i).getInitVal()*(*i).getAmount());
	}
}
void mtm::Etf::printBoth() {

	this->printBonds();
	this->printShares();
}

double mtm::Etf::sell() {
	//cout<<endl;
	//cout<<"----------------------------------"<<endl;
	double money=0.0;
	for (Iterator<SharePointer> i = this->shares.begin();
			i!=this->shares.end();++i) {
		money+=(*i).getTotalValue();
//		cout<<"share "<<(*i).getName()<<endl;
//		cout<<"exp tot val="<<(*i).getTotalValue()<<endl;
//		cout<<"amount is="<<(*i).getAmount()<<endl;
//		cout<<"value is="<<(*i).getValue()<<endl;
		if ((*i).getAmount()<=100) {
		(*i).setValue((*i).getValue()*(1-(*i).getAmount()*0.01));
		//cout<<"val after sell="<<(*i).getValue()<<endl;
		}
		else {
			(*i).setValue(0);
		}
		(*i).getPointer()->setAmount((*i).getPointer()->getAmount()+(*i).getAmount());
		//cout<<endl;
		//cout<<"Amount for "<<(*i).getName()<<" is "<<(*i).getPointer()->getAmount()<< "value is "<<(*i).getPointer()->getValue()<<endl;
	}

//	cout<<"----------------------------------"<<endl;
//	cout<<endl;
	this->shares.clear();
	for (Iterator<BondPointer> i =this->bonds.begin();
			i!=this->bonds.end();++i) {
		money+=(*i).getTotalValue();
		(*i)->setAmount((*i)->getAmount()+(*i).getAmount());
	}
	this->bonds.clear();
	return money;
}

void mtm::Etf::updateBonds() {
	for (Iterator<BondPointer> i =this->bonds.begin();
		i!=this->bonds.end();++i) {
		double newVal=(*i).getValue()*1.01;
		(*i).setValue(newVal);
	}
}

void mtm::Etf::addBond(BondPointer bond) {
	this->bonds.insert(bond,this->bonds.end());
	//cout<<this->bonds.getSize()<<endl;
}

void mtm::Etf::addShare(SharePointer share) {
	this->shares.insert(share,this->shares.end());
}

double mtm::Etf::getTotalBondsValue() {
	double value=0.0;
	for (Iterator<BondPointer> i =this->bonds.begin();
		i!=this->bonds.end();++i) {
		value+=(*i).getTotalValue();
	}
	return value;
}

double mtm::Etf::getTotalBondsProfit() {
	double value=this->getTotalBondsValue();
	for (Iterator<BondPointer> i =this->bonds.begin();
		i!=this->bonds.end();++i) {
		value-=(*i).getInitVal()*(*i).getAmount();
	}
	return value;
}

double mtm::Etf::getTotalSharesValue() {
	double value=0.0;
	for (Iterator<SharePointer> i =this->shares.begin();
		i!=this->shares.end();++i) {
		value+=(*i).getTotalValue();
	}
	return value;
}

double mtm::Etf::getTotalSharesProfit() {
	double value=this->getTotalSharesValue();
	for (Iterator<SharePointer> i =this->shares.begin();
		i!=this->shares.end();++i) {
		value-=(*i).getInitVal()*(*i).getAmount();
	}
	return value;
}

Etf& mtm::Etf::operator=(const Etf& e) {
	if (this==&e) {
		return *this;
	}
	this->bonds=e.bonds;
	this->shares=e.shares;
	return *this;
}
