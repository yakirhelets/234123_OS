/*
 * Request.h
 *
 *  Created on: Jun 21, 2015
 *      Author: daniel
 */

#ifndef REQUEST_H_
#define REQUEST_H_
#include "Costumer.h"
namespace mtm {

class Request {
protected:
	string costumerId;
	int priority;
	bool newClient;
	bool sellPortfolio;
public:
	Request(string Id,int priority, bool newClient=false,bool sellPortfolio=false):
		costumerId(Id),priority(priority),newClient(newClient),sellPortfolio(sellPortfolio){}
	virtual ~Request() {}
	int& getPriority() {
		return priority;
	}
	bool isNewClient() {
		return newClient;
	}
	bool toSellPortfolio() {
		return sellPortfolio;
	}
	string& getId() {
		return costumerId;
	}
};

class LowRisk: public Request {
public:
	LowRisk(string Id,int priority, bool newClient=false,bool sellportfolio=false):
		Request(Id,priority,newClient,sellportfolio) {}
	~LowRisk()=default;
};

class HighRisk: public Request {
public:
	HighRisk(string Id,int priority, bool newClient,bool sellportfolio=false):
		Request(Id,priority,newClient,sellportfolio) {}
	~HighRisk()=default;
};
}
#endif /* REQUEST_H_ */
