#include "Security.h"

/*Aux functions:*/
static bool stringValid(string str);
void mtm::Security::setValue(const double newValue){
	if (newValue<=0) {
	 	 throw NegetiveSecurityValueException();
	}
	this->value = newValue;
}

const string& mtm::Security::getName() const{
	return this->name;
}


void mtm::Security::cancelQuarterPrediction(int quarter){
	if (quarter<0||quarter>3) {
		throw QuarterOutOfBound();
	}
	this->assessment[quarter]=-1;
}

const int& mtm::Security::getAmount() const{
	return amount;
}

const double& mtm::Security::getValue() const{
	return value;
}

mtm::Security& mtm::Security::operator+=(int amount){
	if (amount<0){ //should not be negative number
		throw InvalidSecurityAmountException();
	}
	this->amount += amount;
	return *this;
}

mtm::Security& mtm::Security::operator+=(const mtm::Security& security){
	//printf("%d\n", security.amount);
	this->amount += security.amount;

	return *this;
}

mtm::Security& mtm::Security::operator-=(int amount){
	if (amount>=this->amount){
		throw InvalidSecurityAmountException();
	}
	this->amount -= amount;
	return *this;
}

mtm::Security& mtm::Security::operator-=(const mtm::Security& security){
	if (security.amount>=this->amount){
		throw InvalidSecurityAmountException();
	}
	this->amount -= security.amount;
	return *this;
}

mtm::Security& mtm::Security::operator*=(int amount){
	if (amount<=0) {
		throw InvalidSecurityAmountException();
	}
	this->amount *= amount;
	return *this;
}

mtm::Security& mtm::Security::operator*=(const mtm::Security& security){
	this->amount *= security.amount;
	return *this;
}
 mtm::Security::Security(const string name,const double value,const int amount):
		 name(name), value(value), amount(amount) {
	 if (!stringValid(this->name)) {
		 throw IllegalSecurityNameException();
	 }
	 if (this->amount<=0) {
		 throw InvalidSecurityAmountException();
	 }
	 if (this->value<=0) {
		 throw NegetiveSecurityValueException();
	 }
	 this->assessment=new Quarter[4];
	 for (int i=0;i<4;++i) {
		 this->assessment[i]=-1.0;
	 }
 }
 mtm::Security::~Security() {
	 delete[] this->assessment;
 }
 void mtm::Security::setAmount(const int newAmount) {
	 if (newAmount<=0) {
	 	 throw InvalidSecurityAmountException();
	 	}
	  this->amount=newAmount;
 }

const double mtm::Security::getTotalValue() const {
	 return this->value*double(this->amount);
 }

 mtm::Security::operator double() {
 	double cast = getTotalValue();
 	return cast;
 }


 Quarter& mtm::Security::operator[](int i) {
	 if (i<0) {
		 throw QuarterOutOfBound();
	 }
	 return this->assessment[i];
 }

 const Quarter& mtm::Security::operator[](int i) const {
	 if (i<0) {
	 	 throw QuarterOutOfBound();
	  }
	  return this->assessment[i];
 }

 double mtm::Security::risk() {
	 double sumOfDiff=0.0;//holds the total sum of difference current stock price and his assessment in each quarter
	 int numOfAssessments=0;//holds the number of assessments that are not canceled.
//	 cout<<endl;
//	 cout<<"for share "<<this->name<<endl;
	 for (int i=0;i<4;++i) {
		 //cout<<"prediction is "<<this->assessment[i]<<endl;
		 sumOfDiff+=(this->assessment[i]>-1.0)*(this->assessment[i].getValue()-this->value);
		 numOfAssessments+=(this->assessment[i]>-1.0);
	 }
	// cout<<endl;
	 return numOfAssessments>0? sumOfDiff/double(numOfAssessments):0.0;
 }
 void mtm::Security::clearAssessment() {
	 for (int i=0;i<4;++i) {
		 this->assessment[i]=-1;
	 }
 }

 bool mtm::Security::operator<(const Security& s) {
	 if(this->value==s.value) {
		 if (this->amount==s.amount) {
			 if (this->name>=s.name) {
				 return false;
			 }
			 return true;
		 }
		 else if (this->amount<s.amount) {
			 return true;
		 }
		 return false;
	 }
	 else if (this->value<s.value) {
		 return true;
	 }
	 return false;
 }
 bool mtm::Security::operator>(const Security& s) {
	 if(this->value==s.value) {
	 	 if (this->amount==s.amount) {
	 		 if (this->name<=s.name) {
	 			 return false;
	 		 }
	 		 return true;
	 	 }
	 	 else if (this->amount>s.amount) {
	 		 return true;
	 	 }
	 	 return false;
	  }
	  else if (this->value>s.value) {
	 	 return true;
	  }
	  return false;
}

 bool mtm::Security::operator<=(const Security& s) {
	 return !(*this>s);
 }
bool mtm::Security::operator>=(const Security& s) {
	return !(*this<s);
}
bool mtm::Security::operator==(const Security& s) {
	return (*this<=s&&*this>=s);
}
 static bool stringValid(string str) {
	 if (str.length()>4) {
		 return false;
	 }
	 for (unsigned int i=0;i<str.length(); ++i) {
		 if (str[i]>'Z'||str[i]<'A') {
			 return false;
		 }
	 }
	 return true;
 }




