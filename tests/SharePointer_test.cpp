///*
// * SharePointer_test.cpp
// *
// *  Created on: Jun 25, 2015
// *      Author: daniel
// */
//
//#include "SharePtr.h"
//#include "mtmtest.h"
//#include <iostream>
//using std::cin;
//using std::cout;
//using std::endl;
//using mtm::shared_ptr;
//bool testGetName() {
//	shared_ptr<Share> ptr1(new Share("MSDN",135,6));
//	int amount=3;
//	SharePointer ptr(ptr1,amount);
//	ASSERT_EQUALS(ptr.getName(),"MSDN");
//	return true;
//}
//
//bool testGetValue() {
//	shared_ptr<Share> ptr1(new Share("MSDN",135,6));
//	int amount=3;
//	SharePointer ptr(ptr1,amount);
//	ASSERT_DOUBLE_EQUALS(ptr.getValue(),135);
//	return true;
//}
//
//bool testGetAmount() {
//	shared_ptr<Share> ptr1(new Share("MSDN",135,6));
//	int amount=3;
//	SharePointer ptr(ptr1,amount);
//	ASSERT_EQUALS(ptr.getAmount(),3);
//	return true;
//}
//
//bool testGetPointer() {
//	shared_ptr<Share> ptr1(new Share("MSDN",135,6));
//	int amount=3;
//	SharePointer ptr(ptr1,amount);
//	ASSERT_EQUALS(ptr.getPointer(),ptr1);
//	return true;
//}
//
//bool testAssignmentOp() {
//	shared_ptr<Share> ptr1(new Share("MSDN",135,6));
//	int amount=3;
//	SharePointer ptr(ptr1,amount);
//	SharePointer ptr2=ptr;
//	ASSERT_EQUALS(ptr.getName(),"MSDN");
//	ASSERT_DOUBLE_EQUALS(ptr.getValue(),135);
//	ASSERT_EQUALS(ptr.getAmount(),3);
//	ASSERT_EQUALS(ptr.getPointer(),ptr1);
//	return true;
//}
//bool testEqualsOp() {
//	shared_ptr<Share> ptr1(new Share("MSDN",135,6));
//	int amount=3;
//	SharePointer ptr(ptr1,amount);
//	shared_ptr<Share> ptr3(new Share("INTL",138,7));
//	amount=5;
//	SharePointer ptr2(ptr3,amount);
//	ASSERT_EQUALS(ptr2==ptr,false);
//	ptr2=ptr;
//	ASSERT_EQUALS(ptr2==ptr,true);
//	return true;
//}
//
//bool testTotalValue() {
//	shared_ptr<Share> ptr1(new Share("MSDN",135,6));
//	int amount=3;
//	SharePointer ptr(ptr1,amount);
//	ASSERT_DOUBLE_EQUALS(ptr.getTotalValue(),135*3);
//	ptr.setValue(167);
//	ASSERT_DOUBLE_EQUALS(ptr.getTotalValue(),167*3);
//	return true;
//}
//
//int main() {
//	RUN_TEST(testGetName);
//	RUN_TEST(testGetValue);
//	RUN_TEST(testGetAmount);
//	RUN_TEST(testGetPointer);
//	RUN_TEST(testAssignmentOp);
//	RUN_TEST(testEqualsOp);
//	RUN_TEST(testTotalValue);
//	return 0;
//}
