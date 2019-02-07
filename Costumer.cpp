/*
 * Costumer.cpp
 *
 *  Created on: Jun 20, 2015
 *      Author: daniel
 */
#include "Costumer.h"
#include "CompareDouble.h"
#include <map>
using std::map;
using mtm::Costumer;
static bool idValid(string id);
mtm::Costumer::Costumer(string id,double moneyForInvest):
id(id),etf(),moneyForInvest(moneyForInvest),
initialMoney(moneyForInvest){
	if (!idValid(id)) {
		throw IllegalCustomerIdException();
	}
}
mtm::Costumer::Costumer(const Costumer& c):
		id(c.id),etf(c.etf),moneyForInvest(c.moneyForInvest),
		initialMoney(c.initialMoney) {}

double& mtm::Costumer::getMoney() {
	return this->moneyForInvest;
}
void mtm::Costumer::setMoney(double money) {
	this->moneyForInvest=money;
}

string& mtm::Costumer::getId() {
	return this->id;
}

Costumer& mtm::Costumer::operator=(const Costumer& c) {
	if (this==&c) {
		return *this;
	}
	this->etf=c.etf;
	this->id=c.id;
	this->initialMoney=c.initialMoney;
	this->moneyForInvest=c.moneyForInvest;
	return *this;
}

bool mtm::Costumer::operator==(const Costumer& c) {
	return this->id==c.id;
}

void mtm::Costumer::AddBond(const BondPointer& bond) {
	this->etf.addBond(bond);
	this->moneyForInvest-=bond.getTotalValue();
}
//
void mtm::Costumer::AddShare(const SharePointer& share) {
	this->etf.addShare(share);
	this->moneyForInvest-=share.getTotalValue();
}
void mtm::Costumer::changeBondValues() {
	this->etf.updateBonds();
}
void mtm::Costumer::sellPortfolio() {
	//cout<<"money before="<<this->moneyForInvest<<endl;
	this->moneyForInvest+=this->etf.sell();
	//cout<<"money after="<<this->moneyForInvest<<endl;
}

static bool idValid(string id) {
	if (id.length()!=9) {
		return false;
	}
	for (unsigned int i=0;i<id.length(); ++i) {
		if (id[i]>'9'||id[i]<'0') {
			return false;
		}
	}
	return true;
}

void mtm::Costumer::printBondDetails() {
	this->etf.printBonds();
}
void mtm::Costumer::printShareDetails() {
	this->etf.printShares();
}
void mtm::Costumer::printBothDetails() {
	this->etf.printBoth();
}
double mtm::Costumer::getSharesValue() {
	return this->etf.getTotalSharesValue();
}
double mtm::Costumer::getSharesProfit() {
	return this->etf.getTotalSharesProfit();
}
double mtm::Costumer::getBondsValue() {
	return this->etf.getTotalBondsValue();
}
double mtm::Costumer::getBondsProfit() {
	return this->etf.getTotalBondsProfit();
}

double mtm::Costumer::getPortfolioValue() {
	return this->etf.getTotalBondsValue()+this->etf.getTotalSharesValue();
}
double mtm::Costumer::getProtfolioProfit() {
	return this->etf.getTotalBondsProfit()+this->etf.getTotalSharesProfit();
}

void mtm::Costumer::printCustomerPortfolioBond() {
	printCustomerHeader(this->id);
	this->printBondDetails();
	printCustomerFooter(this->getBondsValue()+this->getSharesValue(),
			compareIsBigger(this->getBondsProfit()+this->getSharesProfit(),0)?0:
					this->getBondsProfit()+this->getSharesProfit(),this->moneyForInvest);
}
void mtm::Costumer::printCustomerPortfolioShare() {
	printCustomerHeader(this->id);
	this->printShareDetails();
	printCustomerFooter(this->getBondsValue()+this->getSharesValue(),
			compareIsBigger(this->getBondsProfit()+this->getSharesProfit(),0)?0:
						this->getBondsProfit()+this->getSharesProfit()
					,this->moneyForInvest);
}
void mtm::Costumer::printCustomerPortfolioBoth() {
	printCustomerHeader(this->id);
	this->printBondDetails();
	this->printShareDetails();
	printCustomerFooter(this->getBondsValue()+this->getSharesValue(),
			compareIsBigger(this->getBondsProfit()+this->getSharesProfit(),0)?0:
				this->getBondsProfit()+this->getSharesProfit(),this->moneyForInvest);
}
