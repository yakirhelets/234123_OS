#include "Security.h"
#include "mtmtest.h"
#include "exception.h"

using mtm::Security;

bool testCreate1() {
	ASSERT_THROW(IllegalSecurityNameException, Security("ABCDE", 10, 10));
	ASSERT_THROW(IllegalSecurityNameException, Security("Abc", 10, 10));
	ASSERT_THROW(NegetiveSecurityValueException, Security("WHAT", -7));
	ASSERT_THROW(InvalidSecurityAmountException, Security("WHAT", 9, 0));
	ASSERT_THROW(NegetiveSecurityValueException, Security("WHAT", 0, 8));
	ASSERT_THROW(InvalidSecurityAmountException, Security("WHAT", -23, 0));
	return true;
}

bool testCreate2() {
	Security empty = Security("", 50, 1);
	Security google = Security("GOOG", 65.04);
	ASSERT_EQUALS(1, google.getAmount());
	Security dell = Security("DELL", 57.61, 3);
	return true;
}

bool testSetValueGetValue() {
	Security google = Security("GOOG", 65.04);
	Security dell = Security("DELL", 57.61, 3);
	ASSERT_DOUBLE_EQUALS(65.04, google.getValue());
	ASSERT_THROW(NegetiveSecurityValueException, google.setValue(-1));
	ASSERT_THROW(NegetiveSecurityValueException,google.setValue(0));
	ASSERT_NO_THROW(google.setValue(130));
	ASSERT_DOUBLE_EQUALS(130, google.getValue());
	return true;
}

bool testSetAmountGetAmount() {
	Security google = Security("GOOG", 65.04);
	Security dell = Security("DELL", 57.61, 3);
	ASSERT_EQUALS(1, google.getAmount());
	ASSERT_EQUALS(3, dell.getAmount());
	ASSERT_THROW(InvalidSecurityAmountException, google.setAmount(0));
	ASSERT_THROW(InvalidSecurityAmountException, google.setAmount(-1));
	ASSERT_NO_THROW(google.setAmount(2));
	ASSERT_EQUALS(2, google.getAmount());
	ASSERT_NO_THROW(google.setAmount(5));
	ASSERT_EQUALS(5, google.getAmount());
	//what happens if we set an empty amount?
	return true;
}

bool testGetTotalValue() {
	Security dell = Security("DELL", 57.61, 3);
	Security google = Security("GOOG", 65.04);
	ASSERT_DOUBLE_EQUALS(65.04, google.getTotalValue());
	ASSERT_DOUBLE_EQUALS(172.83, dell.getTotalValue());
	ASSERT_NO_THROW(dell.setAmount(10));
	ASSERT_NO_THROW(dell.setValue(200.5));
	ASSERT_DOUBLE_EQUALS(2005, dell.getTotalValue());
	return true;
}

bool testGetName() {
	Security google = Security("GOOG", 65.04);
	ASSERT_EQUALS(true, "GOOG" == google.getName());
	Security empty = Security("", 50, 1);
	ASSERT_EQUALS(true, "" == empty.getName());
	return true;
}

bool testCancelQuarterPrediction() {
	Security google = Security("GOOG", 65.04);
	ASSERT_NO_THROW(google[FIRST_QUARTER] = 12);
	ASSERT_DOUBLE_EQUALS(google[FIRST_QUARTER], 12);
	ASSERT_NO_THROW(google.cancelQuarterPrediction(FIRST_QUARTER));
	//ASSERT_THROW(PredictionNotExistException, google[FIRST_QUARTER]);
	return true;
}

bool testPredictionsOperator() {
	Security google = Security("GOOG", 65.04);
	ASSERT_THROW(NegetiveSecurityValueException, google[FIRST_QUARTER] = -5);
	ASSERT_NO_THROW(google[FIRST_QUARTER] = 0);
	ASSERT_NO_THROW(google[FIRST_QUARTER] = (google[FIRST_QUARTER]) + 90);
	ASSERT_NO_THROW(google[SECOND_QUARTER] = 123.4567);
	ASSERT_DOUBLE_EQUALS(google[SECOND_QUARTER], 123.4567);
	ASSERT_NO_THROW(google.cancelQuarterPrediction(SECOND_QUARTER));
	ASSERT_NO_THROW(
			google[FIRST_QUARTER] = (google[SECOND_QUARTER]) + 90);
	ASSERT_NO_THROW(google.cancelQuarterPrediction(FIRST_QUARTER));
	return true;
}

bool testRisk() {
	Security google = Security("GOOG", 65);
	ASSERT_DOUBLE_EQUALS(0, google.risk());
	google[FIRST_QUARTER] = 65;
	ASSERT_DOUBLE_EQUALS(0, google.risk());
	google[SECOND_QUARTER] = 35;
	ASSERT_DOUBLE_EQUALS(-15, google.risk());
	google[THIRD_QUARTER] = 35;
	ASSERT_DOUBLE_EQUALS(-20, google.risk());
	google[FOURTH_QUARTER] = 130;
	ASSERT_DOUBLE_EQUALS(1.25, google.risk());
	ASSERT_NO_THROW(google.cancelQuarterPrediction(FIRST_QUARTER));
	ASSERT_DOUBLE_EQUALS(1.66667, google.risk());
	ASSERT_NO_THROW(google.cancelQuarterPrediction(SECOND_QUARTER));
	ASSERT_DOUBLE_EQUALS(17.5, google.risk());
	return true;
}

bool testMinusOperator() {
	Security google = Security("GOOG", 65.04, 5);
	ASSERT_NO_THROW(google -= 4);
	ASSERT_THROW(InvalidSecurityAmountException, google -= 1);
	ASSERT_EQUALS(1, google.getAmount());
	return true;
}

bool testPlusOperator() {
	Security bacon = Security("BCON", 65.04, 5);
	Security dell = Security("DELL", 57.61, 3);
	Security fakeon = Security("BCON", 65.04, 5);
	ASSERT_NO_THROW( bacon += dell);
	ASSERT_THROW(InvalidSecurityAmountException, dell += -7);
	ASSERT_NO_THROW(bacon += 2);
	ASSERT_NO_THROW(bacon += fakeon);
	ASSERT_EQUALS(15, bacon.getAmount());
	return true;
}

bool testMultiplyOperator() {
	Security bacon = Security("BCON", 65.04, 5);
	Security dell = Security("DELL", 57.61, 3);
	Security fakeon = Security("BCON", 65.04, 5);
	ASSERT_NO_THROW( bacon *= dell);
	ASSERT_THROW(InvalidSecurityAmountException, dell *= 0);
	ASSERT_THROW(InvalidSecurityAmountException, dell *= -7);
	ASSERT_NO_THROW(bacon *= 2);
	ASSERT_NO_THROW(bacon *= fakeon);
	ASSERT_EQUALS(150, bacon.getAmount());
	return true;
}

bool testComparisonOperators() {
	Security bacon = Security("BCON", 65.04, 5);
	Security dell = Security("DELL", 65, 3);
	Security fakeon = Security("BCON", 65.04, 3);
	ASSERT_EQUALS(true, fakeon < bacon);
	ASSERT_NO_THROW(fakeon += 2);
	ASSERT_EQUALS(true, fakeon == bacon);
	//value comparison
	ASSERT_NO_THROW(dell += 4);
	ASSERT_EQUALS(true, bacon > dell);
	//amount comparison
	ASSERT_NO_THROW(dell.setValue(65.04));
	ASSERT_NO_THROW(dell.setAmount(4));
	ASSERT_EQUALS(true, bacon > dell);
	ASSERT_NO_THROW(dell.setAmount(6));
	ASSERT_EQUALS(true, dell > bacon);
	//lexicographic comparison
	ASSERT_NO_THROW(dell.setAmount(5));
	ASSERT_EQUALS(true, dell > bacon);
	ASSERT_NO_THROW(dell -= 1);
	ASSERT_EQUALS(true, dell < bacon);
	return true;
}

bool testDoubles() {
	Security bacon = Security("BCON", 65.04, 5);
	Security dell = Security("DELL", 45, 11);
	ASSERT_DOUBLE_EQUALS((double)(65.04*5), double(bacon));
	ASSERT_DOUBLE_EQUALS(45*11, double(dell));
	return true;
}

//int main() {
//	RUN_TEST(testCreate1);
//	RUN_TEST(testCreate2);
//	RUN_TEST(testSetValueGetValue);
//	RUN_TEST(testSetAmountGetAmount);
//	RUN_TEST(testGetTotalValue);
//	RUN_TEST(testGetName);
//	RUN_TEST(testCancelQuarterPrediction);
//	RUN_TEST(testPredictionsOperator);
//	RUN_TEST(testRisk);
//	RUN_TEST(testMinusOperator);
//	RUN_TEST(testPlusOperator);
//	RUN_TEST(testMultiplyOperator);
//	RUN_TEST(testComparisonOperators);
//	RUN_TEST(testDoubles);
//	return 0;
//}

