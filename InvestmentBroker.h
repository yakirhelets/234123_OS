/*
 * InvesmentBroker.h
 * Module to handle trade. 
 */

#ifndef INVESTMENTBROKER_H_
#define INVESTMENTBROKER_H_

#include <string>
#include "Costumer.h"
#include "Request.h"
using std::string;
#include <map>
using std::map;
using std::pair;

namespace mtm {
	
	enum RequestType{ LOW_RISK, HIGH_RISK } ;
	enum PaperType { PAPER_SHARE,PAPER_BOND,PAPER_BOTH };
	
class InvestmentBroker {
public:
		InvestmentBroker():
			costumers(),requests(),shares(),bonds(),days(),quarter(FIRST_QUARTER) {}
	/*
	 * Adds a new share to the broker's shares database.
	 * */
	void addShare(const string& name, double value, int amount = 1);

	/*
	 *  Adds a new bond to the broker's bonds database.
	 *  */
	void addBond(const string& name, double value, int amount = 1);

	/*
	 *  Adds a new customer to the broker's customer database
	 *   */
	void addCustomer(const string& id, double money,
			RequestType type);

	/* indication that a new trade day started.
	 * updates the customers' bonds,
	 * execute two users request.
	 */
	void nextTradeDay();

	/*
	 * sell the customer portfolio and adds the sell value to the customer
	 * */
	void sellPortfolio(const string& id);

	/*
	 * Let the given user change his investment type, we can add priority to handle the request.
	 */
	void changePreference(const string& id, RequestType type, int priority);
	
	/*
	 * print customer report with specific type of securities
	 */
	void printCustomerReport(const string& id,PaperType paperType=PAPER_BOTH);

	/*
	 * print full report
	 */
	void printFullReport();
	/*
	 * buys bonds according to low risk request
	 */
	void buyBondsForClientLowRisk(string& id);
	/*
	 * buys shares according to low risk request
	 */
	void buySharesForClientLowRisk(string& id);
	/*
	* buys shares according to high risk request
	*/
	void buySharesForClientHighRisk(string& id);
	/*
	* buys bonds according to high risk request
	*/
	void buyBondsForClientHighRisk(string& id);
	/*
	 * set next day in every share
	 */
	void setNextDayInShares();
	/*
	 * update bonds in every client in system
	 */
	void updateBondsInClients();

	void printCustomerDetails();

	void printShareDetails();

	void printBondDetails();

	void setNextQuarterInShares();

	string* mapSort();

private:
	map<string,Costumer> costumers;
	List<shared_ptr<Request> > requests;
	List<shared_ptr<Share> > shares;
	List<shared_ptr<Bond> > bonds;
	int days;
	QuarterType quarter;
};

} /* namespace mtm */

#endif /* INVESTMENTBROKER_H_ */
