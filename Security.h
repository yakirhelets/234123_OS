
#ifndef SECURITY_H_
#define SECURITY_H_
#include <iostream>
#include "exception.h"
using namespace std;
 enum QuarterType{
		FIRST_QUARTER,
		SECOND_QUARTER,
		THIRD_QUARTER,
		FOURTH_QUARTER
	};
 class Quarter {
	 double value;
 public:
	 Quarter(double value=0):
		 value(value) {}
	 Quarter& operator=(const double& q) {
		 if (q<0&&(q!=-1.0)) {
			 //cout<<"lol wtf"<<endl;
			 throw NegetiveSecurityValueException();
		 }

		 value=q;
		 return *this;
	 }
	 Quarter& operator=(Quarter& q) {
	 	 value=q.getValue();
	 	 return *this;
	  }
	  bool operator==(const Quarter& q1) {
		 return value==q1.value;
	 }
	  bool operator==(const double& val) {
	   return value==val;
	  	 }
	  bool operator>(const Quarter& q1) {
	  	 return value>q1.value;
	  }
	  bool operator>(const double& val) {
	  	   return value>val;
	  	 }
	  double& getValue() {
		  return value;
	  }
	  operator double() {
		  return value;
	  }
 };

namespace mtm {
class Security {
protected:
	string name;
	double value;
	int amount;
	Quarter *assessment;
public:
	//was protected: before - changed for the tests
	const int& getAmount() const;
	const double& getValue() const;
	//*****
	Security(const string name,const double value,const int amount=1);
	~Security();
	void setValue(const double newValue);
	void setAmount(const int newAmount);
	const string& getName() const;
	const double getTotalValue() const;
	void clearAssessment();
	operator double();
	Quarter& operator[](int i);
	const Quarter& operator[](int i) const;
	void cancelQuarterPrediction(int quarter);
	double risk();
	Security& operator+=(int amount);
	Security& operator+=(const Security& security);
	Security& operator-=(int amount);
	Security& operator-=(const Security& securty);
	Security& operator*=(int amount);
	Security& operator*=(const Security& securty);
	bool operator==(const Security&);
	bool operator<(const Security&);
	bool operator>(const Security&);
	bool operator<=(const Security&);
	bool operator>=(const Security&);
	};
}




#endif /* SECURITY_H_ */
