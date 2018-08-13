/*
 * SharePtr.h
 *
 *  Created on: Jun 23, 2015
 *      Author: daniel
 */

#ifndef SHAREPTR_H_
#define SHAREPTR_H_

#include "shared_ptr.h"
#include "Share.h"
using namespace mtm;
//holds a smart pointer to a share, for updating the value.
namespace mtm {
	class SharePointer {
		shared_ptr<Share> sharePtr;
		int amount;//amount of shares per costumer
		double initVal;//inital value at buy.
	public:
		SharePointer(shared_ptr<Share> share,int amount);
		SharePointer(const SharePointer& p);
		~SharePointer()=default;
		void setValue(const double& value);
		const shared_ptr<Share>& getPointer() const;
		string getName();
		double getValue() const;
		const double& getInitVal() const;
		const int& getAmount() const;
		double getTotalValue() const;
		SharePointer& operator=(const SharePointer& sp);
		bool operator==(const SharePointer& sp);
	};
}


#endif /* SHAREPTR_H_ */
