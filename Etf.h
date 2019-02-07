/*
 * Etf.h
 *
 *  Created on: Jun 23, 2015
 *      Author: daniel
 */

#ifndef ETF_H_
#define ETF_H_
#include "list.h"
#include "SharePtr.h"
#include "BondPointer.h"
using namespace mtm;
namespace mtm {
	class Etf {
		List<SharePointer> shares; //map that links share pointers to their initial value
		List<BondPointer> bonds; //map that links bonds to their initial value value
	public:
		/*
		 * c'tor
		 */
		Etf();

		Etf(const Etf& e);

		Etf& operator=(const Etf& e);
		/*
		 * prints the bond list, by the requirements in the assignment
		 */
		void printBonds();
		/*
		 * prints the share list, by the requirements in the assignment
		 */
		void printShares();
		/*
		 * prints both of the list
		 */
		void printBoth();
		/*
		 * sells the Etf
		 */
		double sell();
		/*
		 * adds a bond to the etf
		 */
		void addBond(BondPointer bond);
		/*
		 * adds a share to the etf
		 */
		void addShare(SharePointer share);
		/*
		 * return total profit of etf
		 */
		double totalProfit();
		/*
		 * update the bond values
		 */
		void updateBonds();
		/*
		 * return total bonds value
		 */
		double getTotalBondsValue();
		/*
		* return total bonds profit
		*/
		double getTotalBondsProfit();
		/*
		* return total shares value
		*/
		double getTotalSharesValue();
		/*
		* return total shares profit
		*/
		double getTotalSharesProfit();


	};
}



#endif /* ETF_H_ */
