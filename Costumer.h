/*
 * Costumer.h
 *
 *  Created on: Jun 20, 2015
 *      Author: daniel
 */

#ifndef COSTUMER_H_
#define COSTUMER_H_
#include <iostream>
#include "exception.h"
#include "Etf.h"
#include "PrintHelper.h"
using std::string;
using mtm::shared_ptr;
using mtm::Etf;
using mtm::SharePointer;
namespace mtm {
class Costumer {
	string id;
	Etf etf;
	double moneyForInvest;
	double initialMoney;

public:
	Costumer(string id, double moneyForInvest);
	Costumer(const Costumer& c);
	~Costumer()=default;
	bool operator==(const Costumer& c);
	Costumer& operator=(const Costumer& c);
	void setMoney(double money);
	double& getMoney();
	string& getId();
	void AddShare(const SharePointer& share);
	void AddBond(const BondPointer& bond);
	void changeBondValues();
	void sellPortfolio();
	void printBondDetails();
	void printShareDetails();
	void printBothDetails();
	double getSharesValue();
	double getSharesProfit();
	double getBondsValue();
	double getBondsProfit();
	double getPortfolioValue();
	double getProtfolioProfit();
	void printCustomerPortfolioBond();
	void printCustomerPortfolioShare();
	void printCustomerPortfolioBoth();
};
}


#endif /* COSTUMER_H_ */
