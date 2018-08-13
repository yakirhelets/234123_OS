/*
 * InvestmentBroker.cpp
 *
 *  Created on: Jun 23, 2015
 *      Author: daniel
 */
#include "InvestmentBroker.h"
#include "CompareDouble.h"
#include <limits>
#ifndef INT_MAX
#define INT_MAX 2147483647
#endif
class EqualsTo {
	string str;
public:
	EqualsTo(string str):
		str(str){}
	bool operator()(const shared_ptr<Share>& share) const{
		return share->getName()==str;
	}
	bool operator()(const shared_ptr<Bond>& bond) const{
		return bond->getName()==str;
	}
	bool operator()(const shared_ptr<Request>& request) const{
		return request->getId()==str;
	}
};

class RequestsEqual {
public:
	bool operator()(shared_ptr<Request> r1,shared_ptr<Request> r2) const{
		if (r1->isNewClient()) {
			if (r2->isNewClient()) {
				return r1->getPriority()>=r2->getPriority();
			}
			return true;
		}
		if (r2->isNewClient()) {
			return false;
		}
		return r1->getPriority()>=r2->getPriority();
	}
};

void mtm::InvestmentBroker::addShare(const string& name, double value, int amount) {
	Iterator<shared_ptr<Share> > it=this->shares.find(EqualsTo(name));
	if (it!=this->shares.end()) {
		throw SecurityAlreadyExistException();
	}
	shared_ptr<Share> sharePtr(new Share(name,value,amount,this->quarter));
	this->shares.insert(sharePtr,this->shares.end());
}

void mtm::InvestmentBroker::addBond(const string& name, double value, int amount) {
	Iterator<shared_ptr<Bond> > it=this->bonds.find(EqualsTo(name));
	if (it!=this->bonds.end()) {
		throw SecurityAlreadyExistException();
	}
	shared_ptr<Bond> sharePtr(new Bond(name,value,amount));
	this->bonds.insert(sharePtr,this->bonds.end());
}

void mtm::InvestmentBroker::addCustomer(const string& id, double money,
			RequestType type) {
	map<string,Costumer>::iterator it=this->costumers.find(id);
	if (it!=this->costumers.end()) {
		throw CustomerAlreadyExistException();
	}
	this->costumers.insert(make_pair(id,Costumer(id,money)));
	if (type==LOW_RISK) {
		shared_ptr<Request> reqPtr(new LowRisk(id,INT_MAX,true));
		this->requests.insert(reqPtr,this->requests.end());
	}
	else {
		shared_ptr<Request> reqPtr(new HighRisk(id,INT_MAX,true));
		this->requests.insert(reqPtr,this->requests.end());
	}
	this->requests.sort(RequestsEqual());
}

void mtm::InvestmentBroker::sellPortfolio(const string& id) {
	map<string,Costumer>::iterator it=this->costumers.find(id);
	if (it==this->costumers.end()) {
		throw CustomerNotExistException();
	}
	it->second.sellPortfolio();
}

void mtm::InvestmentBroker::changePreference(const string& id, RequestType type
		, int priority=1) {
	if(this->costumers.find(id)==this->costumers.end()) {
		throw CustomerNotExistException();
	}
	Iterator<shared_ptr<Request> > it=this->requests.find(EqualsTo(id));
	if (it!=this->requests.end()) {
		if (!(*it)->isNewClient()) {
			this->requests.remove(it);
		}
	}
	if (type==LOW_RISK) {
		shared_ptr<Request> reqPtr(new LowRisk(id,priority,false,true));
		this->requests.insert(reqPtr,this->requests.end());
	}
	else {
		shared_ptr<Request> reqPtr(new HighRisk(id,priority,false,true));
		this->requests.insert(reqPtr,this->requests.end());
	}
	this->requests.sort(RequestsEqual());
}
double Abs(double num) {
	if (num<0) {
		return -num;
	}
	return num;
}
#define epsilon 0.0001
//function object to sort according to high risk.
class EqualsLowRisk {
public:
	bool operator()(const shared_ptr<Bond>& b1,const shared_ptr<Bond>& b2) const{
		if(b1->getTotalValue()>b2->getTotalValue()) {
			return true;
		}
		if(Abs(b1->getTotalValue()-b2->getTotalValue())<epsilon) {
			if(b1->getName()>b2->getName()) {
				return true;
			}
		}
	return false;
	}
	bool operator()(const shared_ptr<Share>& s1,const shared_ptr<Share>& s2) const{
		if(Abs(s1->risk())<Abs(s2->risk())) {
			return true;
		}
		if (Abs(Abs(s1->risk())-Abs(s2->risk()))<epsilon) {//if absolute value is equal
			if (s1->risk()<s2->risk()) {
				return true;
			}//if both positive, check if equal.
			if (Abs(s1->risk()-s2->risk())<epsilon) {
				if (s1->getTotalValue()<s2->getTotalValue()) {
					return true;
				}
				if (Abs(s1->getTotalValue()-s2->getTotalValue())<epsilon) {
					if (s1->getName()>=s2->getName()) {
						return true;
					}
					return false;
				}
				return false;
			}
			return false;
		}
		return false;
	}
};

void mtm::InvestmentBroker::buyBondsForClientLowRisk(string& id) {
	map<string,Costumer>::iterator customerPair=this->costumers.find(id);
	this->bonds.sort(EqualsLowRisk());
	for (Iterator<shared_ptr<Bond> > it=this->bonds.begin();it!=this->bonds.end();
			++it) {
		for (int numOfSecurities=(*it)->getAmount()-1;numOfSecurities>0;--numOfSecurities) {
			if(customerPair->second.getMoney()>=numOfSecurities*(*it)->getValue()) {
				(*it)->setAmount((*it)->getAmount()-numOfSecurities);
				BondPointer bondP((*it),numOfSecurities);
				customerPair->second.AddBond(bondP);
				break;
			}
		}
	}
}

void mtm::InvestmentBroker::buySharesForClientLowRisk(string& id) {
	map<string,Costumer>::iterator customerPair=this->costumers.find(id);
	this->shares.sort(EqualsLowRisk());
	for (Iterator<shared_ptr<Share> > it=this->shares.begin();it!=this->shares.end();
			++it) {
		for (int numOfSecurities=(*it)->getAmount()-1;numOfSecurities>0;--numOfSecurities) {
			if (customerPair->second.getMoney()>=
					numOfSecurities*((*it)->getValue()*(1+numOfSecurities*0.01))) {//check if customer can buy the stock after rise
				(*it)->setValue((*it)->getValue()*(1+numOfSecurities*0.01));
				(*it)->setAmount((*it)->getAmount()-numOfSecurities);
				SharePointer shareP(*it,numOfSecurities);
				customerPair->second.AddShare(shareP);
				break;
			}
		}
	}
}


class EqualsHighRisk {
public:
	bool operator()(const shared_ptr<Share>& s1,const shared_ptr<Share>& s2) const{
		if (s1->risk()>s2->risk()) {
			return true;
		}
		if (Abs(s1->risk()-s2->risk())<epsilon) {
			if (s1->getTotalValue()<s2->getTotalValue()) {
				return true;
			}
			if (Abs(s1->getTotalValue()-s2->getTotalValue())<epsilon) {
				if (s1->getName()>s2->getName()) {
					return true;
				}
				return false;
			}
			return false;
		}
		return false;
	}

	bool operator()(const shared_ptr<Bond>& b1,const shared_ptr<Bond>& b2) const {
		if (b1->getTotalValue()<b2->getTotalValue()) {
			return true;
		}
		if (Abs(b1->getTotalValue()-b2->getTotalValue())<epsilon) {
			if (b1->getName()>=b2->getName()) {
				return true;
			}
		return false;
		}
		return false;
	}
};
void mtm::InvestmentBroker::buySharesForClientHighRisk(string& id) {
	map<string,Costumer>::iterator customerPair=this->costumers.find(id);
	this->shares.sort(EqualsHighRisk());
	for (Iterator<shared_ptr<Share> > it=this->shares.begin();it!=this->shares.end();
			++it) {
		for (int numOfSecurities=(*it)->getAmount()-1;numOfSecurities>0;--numOfSecurities) {
			if (customerPair->second.getMoney()>=
					numOfSecurities*((*it)->getValue()*(1+numOfSecurities*0.01))) {//check if customer can buy the stock after rise
				(*it)->setValue((*it)->getValue()*(1+numOfSecurities*0.01));
				(*it)->setAmount((*it)->getAmount()-numOfSecurities);
				SharePointer shareP(*it,numOfSecurities);
				customerPair->second.AddShare(shareP);
				break;
			}
		}
	}
}

void mtm::InvestmentBroker::buyBondsForClientHighRisk(string& id) {
	map<string,Costumer>::iterator customerPair=this->costumers.find(id);
	this->bonds.sort(EqualsHighRisk());
	for (Iterator<shared_ptr<Bond> > it=this->bonds.begin();it!=this->bonds.end();
			++it) {
		for (int numOfSecurities=(*it)->getAmount()-1;numOfSecurities>0;--numOfSecurities) {
			if(customerPair->second.getMoney()>=numOfSecurities*(*it)->getValue()) {
				(*it)->setAmount((*it)->getAmount()-numOfSecurities);
				BondPointer bondP((*it),numOfSecurities);
				customerPair->second.AddBond(bondP);
				break;
			}
		}
	}
}
void mtm::InvestmentBroker::setNextQuarterInShares() {
	for (Iterator<shared_ptr<Share> >it=this->shares.begin();it!=this->shares.end();
			++it) {
		(*it)->nextQuarter();
	}
}
void mtm::InvestmentBroker::nextTradeDay() {
	this->updateBondsInClients();
	this->setNextDayInShares();
	for (int i=0;i<2;++i) {
		Iterator<shared_ptr<Request> > it=this->requests.begin();

		if (it==this->requests.end()) {
			break;
		}
		if ((*it)->toSellPortfolio()) {
			this->sellPortfolio((*it)->getId());
		}
		if (typeid(*(*it))==typeid(LowRisk)) {
			this->buyBondsForClientLowRisk((*it)->getId());
			this->buySharesForClientLowRisk((*it)->getId());
		}
		else {
			this->buySharesForClientHighRisk((*it)->getId());
			this->buyBondsForClientHighRisk((*it)->getId());
		}
		this->requests.remove(it);
	}
	//cout<<"%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%"<<endl;
	//cout<<"%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%"<<endl;
//	for (Iterator<shared_ptr<Share> > it=this->shares.begin();it!=this->shares.end();++it) {
//		cout<<"for share "<<(*it)->getName()<<", risk is "<<(*it)->risk()<<" value is "<<(*it)->getValue()<<endl;
//	}
	//cout<<"%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%"<<endl;
	//cout<<"%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%"<<endl;
	this->days++;
	//cout<<"day is "<<this->days<<endl;
	if (this->days%5==0) {
		//cout<<"WHEN??????????"<<endl;
		this->setNextQuarterInShares();
		switch(this->quarter) {
			case FIRST_QUARTER:this->quarter=SECOND_QUARTER;break;
			case SECOND_QUARTER:this->quarter=THIRD_QUARTER;break;
			case THIRD_QUARTER:this->quarter=FOURTH_QUARTER;break;
			case FOURTH_QUARTER:this->quarter=FIRST_QUARTER;break;
		}
	}
}
void mtm::InvestmentBroker::setNextDayInShares() {
	for (Iterator<shared_ptr<Share> >it=this->shares.begin();it!=this->shares.end();
			++it) {
		(*it)->setNextDay();
	}
}
void mtm::InvestmentBroker::updateBondsInClients() {
	for ( map<string,Costumer>::iterator it=this->costumers.begin();
			it!=this->costumers.end();++it) {
		//cout<<"id "<<it->second.getId()<<endl;
		it->second.changeBondValues();
	}
}
void mtm::InvestmentBroker::printCustomerReport(const string& id,PaperType paperType) {
	map<string,Costumer>::iterator customerPair=this->costumers.find(id);
	if (customerPair==this->costumers.end()) {
		throw CustomerNotExistException();
	}
	switch(paperType) {
	case PAPER_BOND:customerPair->second.printCustomerPortfolioBond();break;
	case PAPER_SHARE:customerPair->second.printCustomerPortfolioShare();break;
	default:customerPair->second.printCustomerPortfolioBoth();break;
	}

}
class SortLists {
public:
	bool operator() (const shared_ptr<Bond>& b1,const shared_ptr<Bond>& b2) const{
		return b1->getName()>=b2->getName();
	}
	bool operator() (const shared_ptr<Share>& b1,const shared_ptr<Share>& b2) const{
		return b1->getName()>=b2->getName();
	}
};
void mtm::InvestmentBroker::printBondDetails() {
	this->bonds.sort(SortLists());
	printFullReportTotalBonds(this->bonds.getSize());
	for (Iterator<shared_ptr<Bond> > it=this->bonds.begin();it!=this->bonds.end();++it) {
		printFullReportBondDetails((*it)->getName(),(*it)->getValue(),
				(*it)->getAmount());
	}
}

void mtm::InvestmentBroker::printShareDetails() {
	this->shares.sort(SortLists());
	printFullReportTotalShares(this->shares.getSize());
	for (Iterator<shared_ptr<Share> > it=this->shares.begin();it!=this->shares.end();++it) {
		printFullReportBondDetails((*it)->getName(),(*it)->getValue(),
			(*it)->getAmount());
	}
}

class SortCustomers {
public:
	bool operator()(Costumer& c1,Costumer& c2) const{
		if (c1.getPortfolioValue()>c2.getPortfolioValue()) {
			return true;
		}
		if (c1.getPortfolioValue()<c2.getPortfolioValue()) {
			return false;
		}
		if (Abs(c1.getPortfolioValue()-c2.getPortfolioValue())<epsilon) {
			if (c1.getId()>c2.getId()) {
			return true;
			}
		}
		return false;
	}
};

string* mtm::InvestmentBroker::mapSort() {
	string* arr=new string[this->costumers.size()];
	int i=0;
	for (map<string,Costumer>::iterator it=this->costumers.begin();it!=this->costumers.end();++it) {
		arr[i]=it->first;
		++i;
	}
	SortCustomers sort;
	for (int i=this->costumers.size();i>=0;--i) {
		for (int j=0;j<i-1;++j) {
			if ( sort( this->costumers.find(arr[j])->second,this->costumers.find(arr[j+1])->second)==false) {
				string temp=arr[j];
				arr[j]=arr[j+1];
				arr[j+1]=temp;
			}
		}
	}
	return arr;
}

void mtm::InvestmentBroker::printCustomerDetails() {
	printFullReportTotalCustomers(this->costumers.size());
	double totalValue=0.0,totalProfit=0.0;
	string* arr=this->mapSort();
	for (unsigned int i=0;i<this->costumers.size();++i) {
		map<string,Costumer>::iterator it=this->costumers.find(arr[i]);
		totalValue+=it->second.getPortfolioValue();
		totalProfit+=it->second.getProtfolioProfit();
		printFullReportCustomer(it->second.getId(), it->second.getPortfolioValue(),
		compareIsBigger(it->second.getProtfolioProfit(),0)?0:it->second.getProtfolioProfit());
	}
	printFullReportFooter(totalValue,compareIsBigger(totalProfit,0)?0:totalProfit);
	//delete arr;
}

void mtm::InvestmentBroker::printFullReport() {
	printFullReportHeader();
	this->printBondDetails();
	this->printShareDetails();
	this->printCustomerDetails();

}
