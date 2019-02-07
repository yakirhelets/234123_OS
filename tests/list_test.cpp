///*
// * listTest.cpp
//
// *
// *  Created on: 23 ביונ 2015
// *      Author: יקיר
// */
//
//#include "list.h"
//#include <assert.h>
//#include "mtmtest.h"
//#include <string>
//using namespace std;
////std::string location;
//
//class StringEquals
//{
//	string str;
//public:
//	StringEquals(string str) : str(str) {}
//	bool operator()(const string& x) const
//	{
//		return (x==str);
//	}
//};
//
//class StringCompare
//{
//public:
//	bool operator()(const string& str1, const string& str2) const
//	{
//		return str1 < str2;
//	}
//};
//
//bool testNodeCreate() {
//	Node<string> node1 ();
//	Node<string> node2("alison");
//	Node<string> node3("burgers");
//	ASSERT_EQUALS(node2.getValue(), "alison");
//	ASSERT_EQUALS(node3.getValue(), "burgers");
//	return true;
//}
//
//bool testNodeGetSetNext() {
//	Node<string> node1("some");
//	Node<string> node2("string");
//	ASSERT_EQUALS(node1.getValue(), "some");
//	ASSERT_EQUALS(node2.getValue(), "string");
//	node1.setNext(node2);
//	ASSERT_EQUALS(node1.getNext()->getValue(), "string");
//	return true;
//}
//
//bool testNodeOperatorIsEqual() {
//	Node<string> node1("this is equal");
//	Node<string>* node2=&node1;
//	ASSERT_EQUALS(node1.getValue(), "this is equal");
//	ASSERT_EQUALS(node2->getValue(), "this is equal");
//	ASSERT_EQUALS(node1==*node2, true);
//	return true;
//}
//
//bool testListBeginAndDereference() {
//	List<string> list;
//	ASSERT_NO_THROW(list.insert("generic value", list.begin()));
//	ASSERT_EQUALS(*list.begin(), "generic value");
//	ASSERT_NO_THROW(list.insert("another generic value", list.begin()));
//	ASSERT_EQUALS(*list.begin(), "another generic value");
//	ASSERT_THROW(ListExceptions::ElementNotFound, *list.end());
//	return true;
//}
//
//bool testListEndAndIncrement() {
//	List<string> list;
//	ASSERT_NO_THROW(list.insert("1", list.end()));
//	ASSERT_EQUALS("1", *list.begin());
//	ASSERT_NO_THROW(list.insert("2", list.end()));
//	Iterator<string> i = list.begin();
//	ASSERT_EQUALS("2", *++i);
//	ASSERT_NO_THROW(list.insert("3", list.end()));
//	ASSERT_EQUALS("2", *i++);
//	return true;
//}
//
//bool testListInsert() {
//	List<string> list;
//	ASSERT_NO_THROW(list.insert("wrong", list.end()));
//	ASSERT_NO_THROW(list.insert("iterator", list.end()));
//	ASSERT_NO_THROW(list.insert("dude", list.end()));
//	List<string> list2;
//	ASSERT_NO_THROW(list2.insert("fake element", list2.begin()));
//	Iterator<string> anotherListIterator = list2.begin();
//	ASSERT_THROW(ListExceptions::ElementNotFound,
//			list.insert("fake element", anotherListIterator));
//	return true;
//}
//
//
//bool testIteratorEquality1() {
//	List<string> list;
//	Iterator<string> i = list.end();
//	Iterator<string> i2 = list.end();
//	ASSERT_EQUALS(true, i==i2);
//	return true;
//}
//
//bool testIteratorEquality2() {
//	List<string> list;
//	ASSERT_NO_THROW(list.insert("both are equal", list.end()));
//	Iterator<string> i = list.begin();
//	Iterator<string> i2 = list.begin();
//	ASSERT_EQUALS(true, i==i2);
//	return true;
//}
//
//bool testIteratorEquality3() {
//	List<string> list;
//	ASSERT_NO_THROW(list.insert("not", list.end()));
//	ASSERT_NO_THROW(list.insert("equal", list.end()));
//	Iterator<string> i = list.begin();
//	Iterator<string> i2 = list.begin();
//	ASSERT_NO_THROW(++i2);
//	ASSERT_EQUALS(true, i!=i2);
//	return true;
//}
//
//bool testListGetSize() {
//	List<string> list;
//	ASSERT_EQUALS(0, list.getSize());
//	ASSERT_NO_THROW(list.insert("reached", list.end()));
//	ASSERT_EQUALS(1, list.getSize());
//	ASSERT_NO_THROW(list.insert("halfway", list.end()));
//	ASSERT_EQUALS(2, list.getSize());
//	return true;
//}
//
//bool testListRemove() {
//	List<string> list;
//
//	ASSERT_THROW(ListExceptions::ElementNotFound, list.remove(list.begin()));
//	//can't remove from an empty list
//
//	ASSERT_NO_THROW(list.insert("eggs", list.end()));//"eggs"
//	ASSERT_NO_THROW(list.insert("and", list.end()));//"eggs and"
//	ASSERT_NO_THROW(list.insert("bacon!", list.end()));//"eggs and bacon!"
//	ASSERT_EQUALS(3, list.getSize());
//	Iterator<string> i = list.begin();
//	ASSERT_NO_THROW(i++);
//	ASSERT_NO_THROW(list.remove(i));//removes "and"
//	ASSERT_EQUALS(2, list.getSize());
//	ASSERT_EQUALS("eggs", *list.begin());//=="eggs"
//	ASSERT_EQUALS("bacon!", *++list.begin());//=="bacon!"
//	//shouldn't remove "bacon!"
//
//	ASSERT_THROW(ListExceptions::ElementNotFound, list.remove(list.end()));
//	List<string> list2;
//	Iterator<string> anotherListIterator = list2.begin();
//	ASSERT_THROW(ListExceptions::ElementNotFound,
//			list.remove(anotherListIterator));
//
//	//should remove nodes
//	Iterator<string> i2 = list.begin();
//	ASSERT_NO_THROW(list.remove(++i2));//removes "bacon!"
//	ASSERT_EQUALS(1, list.getSize());
//	ASSERT_NO_THROW(list.remove(list.begin()));//removes "eggs"
//	ASSERT_EQUALS(0, list.getSize());
//	return true;
//}
//
//bool testListSort() {
//	List<string> list;
//	ASSERT_NO_THROW(list.insert("jesse", list.end()));
//	ASSERT_NO_THROW(list.insert("pinkman", list.end()));
//	ASSERT_NO_THROW(list.insert("in", list.end()));
//	ASSERT_NO_THROW(list.insert("the", list.end()));
//	ASSERT_NO_THROW(list.insert("house", list.end()));
//	//"Jesse" "Pinkman" "in" "the" "house"
//	ASSERT_EQUALS(5, list.getSize());
//
//	// sorts a list in lexicographic order
//	//"Jesse" "house" "in" "Pinkman" "the"
//	ASSERT_NO_THROW(list.sort(StringCompare()));
//	string check[] = { "house" , "in" , "jesse" , "pinkman" , "the" };
//	int i=0;
//	for (Iterator<string> it = list.begin(); it != list.end(); ++it, i++)
//	{
//		ASSERT_EQUALS(check[i], *it);
//	}
//	ASSERT_EQUALS(5, list.getSize());
//	return true;
//}
//
//bool testListFind() {
//	List<string> list;
//	ASSERT_NO_THROW(list.insert("shame", list.end()));
//	ASSERT_NO_THROW(list.insert("shame", list.end()));
//	ASSERT_NO_THROW(list.insert("shame", list.end()));
//	ASSERT_NO_THROW(list.insert("DING!", list.end()));
//	ASSERT_NO_THROW(list.insert("DING!", list.end()));
//	ASSERT_EQUALS(5, list.getSize());
//	ASSERT_EQUALS(list.begin(),list.find(StringEquals("shame")));
//	Iterator<string> iterator = list.find(StringEquals("shame"));
//	for (int i=0 ; i<3 ; ++i){
//		++iterator;
//	}
//	ASSERT_EQUALS(iterator, list.find(StringEquals("DING!")));
//	ASSERT_EQUALS(list.end(),list.find(StringEquals("DONG!")));
//	return true;
//}
//
//int main() {
//	RUN_TEST(testNodeCreate);
//	RUN_TEST(testNodeGetSetNext);
//	RUN_TEST(testNodeOperatorIsEqual);
//	RUN_TEST(testListBeginAndDereference);
//	RUN_TEST(testListEndAndIncrement);
//	RUN_TEST(testListInsert);
//	RUN_TEST(testIteratorEquality1);
//	RUN_TEST(testIteratorEquality2);
//	RUN_TEST(testIteratorEquality3);
//	RUN_TEST(testListGetSize);
//	RUN_TEST(testListRemove);
//	RUN_TEST(testListSort);
//	RUN_TEST(testListFind);
//	return 0;
//}
