/*
 * BondPointer.h
 *
 *  Created on: Jun 25, 2015
 *      Author: daniel
 */

#ifndef BONDPOINTER_H_
#define BONDPOINTER_H_
#include "shared_ptr.h"
#include "Bond.h"
using mtm::shared_ptr;
using mtm::Bond;
namespace mtm {
	class BondPointer {
		shared_ptr<Bond> bondPtr;//a pointer to save the amount globally, rest is per costumer
		double value;
		double initVal;
		int amount;
	public:
		BondPointer(shared_ptr<Bond> bond,int amount);
		BondPointer(const BondPointer& bp);
		~BondPointer()=default;
		shared_ptr<Bond> operator->();
		const double& getValue() const;
		const double& getInitVal() const;
		const int& getAmount() const;
		void setValue(const double& value);
		bool operator==(const BondPointer& bp);
		BondPointer& operator=(const BondPointer& bp);
		double getTotalValue() const;
	};

}


#endif /* BONDPOINTER_H_ */
