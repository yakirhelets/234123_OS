/*
 * list.h
 *
 *  Created on: 19 ביונ 2015
 *      Author: יקיר
 */

#ifndef LIST_H_
#define LIST_H_

#include <cstdlib>

#include "exception.h"
template <class T> class List;
template <class T> class Node;
template <class T> class Iterator;

template <class T>
class Node {
  T data;
  Node<T> *next;
public:
  friend class Iterator<T>;
  friend class List<T>;
  //node c'tor
  Node(const T& data):
	  data(data), next(NULL) {}
  //node destroyer
  ~Node()=default;
  Node<T>* getNext(){
	  if (this->next==NULL){
		  return NULL;
	  }
	  return this->next;
  }
  void setNext(Node<T>& next){
	  this->next=&next;
  }
  T& getValue(){
	  if (this==NULL){
		  throw ListExceptions::ElementNotFound();
	  }
	  return this->data;
  }
  void setValue(const T& value){
	  this->data = value;
  }
  bool operator==(Node<T>& n){
	  return (this==&n);
  }
};

template <class T>
class Iterator {
  Node<T>* node;
  List<T>* list;
public:
  friend class Node<T>;
  friend class List<T>;
  Iterator(Node<T>* node, List<T>* list){
	  this->node=node;
	  this->list=list;
  }
  ~Iterator(){
	  this->node=NULL;
	  this->list=NULL;
  }
  Iterator& operator++(){ //++i
	  this->node = this->node->next;
	  return *this;
  }
  Iterator operator++(int i){ //i++
	  Iterator<T> result = *this;
	  this->node = this->node->next;
	  return result;
  }

  Node<T> current(){
	  return this->node;
  }
  T& operator*(){
	  if (this->node==NULL){
		  throw ListExceptions::ElementNotFound();
	  }
	  return this->node->getValue();
  }
  bool operator==(const Iterator<T>& i){
	  return (this->node==i.node && this->list==i.list);
  }
  bool operator!=(const Iterator<T>& i){
	  return !(this->node==i.node && this->list==i.list);
  }
  Iterator<T>& operator=(const Iterator<T>& iterator){
	  this->node=iterator.node;
	  this->list=iterator.list;
	  return *this;
  }
};


template <class T>
class List {
  Node<T>* head;
public:
  friend class Iterator<T>;
  friend class Node<T>;
  List(const Node<T>& head){
	  this->head=head;
  }
  List(){
	  this->head=NULL;
  }
  ~List(){
	  Node<T>* toDel = this->head;
	  while (this->head!=NULL){
		  this->head = this->head->getNext();
		  delete toDel;
		  toDel = this->head;
	  }
	  delete this->head;
  }
  Iterator<T> begin(){
	  if (this->head == NULL){
		  return this->end();
	  }
	  return Iterator<T>(this->head, this);
  }
  Iterator<T> end(){
	  return Iterator<T>(NULL, this);
  }
  //checks if a node belongs to a given list
  bool existInList (Node<T>& node, List<T>& list){
	  Node<T>* current = list.head;
	  int size = list.getSize();
	  for (int i=0 ; i<size ; i++){
		  if (current->getValue()==node.getValue()){
			  return true;
		  }
		  current=current->getNext();
	  }
	  return false;
  }
  //checks if an iterator points to a given list
  bool listCompare (Iterator<T>& iterator, List<T>* list){
	  return iterator.list==list;
  }
  //get the previous node of a given iterator
  Node<T>* getPrev(Iterator<T>& iterator){
	  Node<T>* current = iterator.list->head;
	  while (current!=NULL){
		  if(current->next==iterator.node){
			  return current;
		  }
		  current = current->getNext();
	  }
	  return NULL;
  }
  void insert (const T& data, Iterator<T> iterator){
	  if (!listCompare(iterator, this)){
		  throw ListExceptions::ElementNotFound();
	  }
	  else {
		  Node<T>* newNode = new Node<T>(data);
		  newNode->next=iterator.node;
		  if (this->getSize()==0){
			  this->head=newNode;
			  return;
		  }
		  if (getPrev(iterator)==NULL){
			  newNode->next=this->head;
			  this->head=newNode;
			  return;
		  }
		  Node<T>* prev = getPrev(iterator);
		  prev->next = newNode;
	  }
  }
  void remove (Iterator<T> iterator){
	  if (iterator.node==NULL || (!listCompare(iterator, this)) ||
			  (!existInList(*iterator.node, *iterator.list))){
		  throw ListExceptions::ElementNotFound();
	  }
	  else {
		  Node<T>* next = iterator.node->getNext();
		  Node<T>* toDel = iterator.node;
		  if (getPrev(iterator)==NULL){
			  this->head = next;
			  delete toDel;
			  return;
		  }
		  delete toDel;
		  Node<T>* prev = getPrev(iterator);
		  prev->next = next;
	  }
  }
  void clear() {
	  while (head!=NULL) {
		  remove(begin());
	  }
  }

  template <class Predicate>
  Iterator<T> find (const Predicate& predicate){
	  for (Iterator<T> toFind = this->begin() ; toFind.node!=NULL ; toFind++){
		  if (predicate(*toFind)==true){
			  return toFind;
		  }
	  }
	  return this->end();
  }
  void swapNodes(Node<T>& node1, Node<T>& node2){
	  T temp = node1.getValue();
	  node1.setValue(node2.getValue());
	  node2.setValue(temp);
  }
  template <class Compare>
  void sort (const Compare& compare){
		int size = this->getSize();
		for (int i=0; i<size; i++){
			Iterator<T> curr = this->begin();
			Node<T>* node1;// = curr.node;
			Node<T>* node2;// = curr.node->getNext();
			while(curr.node!=NULL && curr.node->next!=NULL){
				node1 = curr.node;
				node2 = curr.node->getNext();
				if (compare(*curr, curr.node->next->getValue())==false){
					swapNodes(*node1, *node2);
				}
					curr++;
			}
		}
  }



  List<T>& operator=(const List<T>& list) {
	  if (this==&list)
		  return *this;
	  delete this->head;
	  this->head=new Node<T>(*list.head);
	  return *this;
  }

  int getSize(){
	  int i = 0;
	  Node<T>* Curr = this->head;
	  while (Curr!=NULL){
		  i++;
		  Curr = Curr->getNext();
	  }
	  return i;
  }
  List<T>& listCopy(List<T> toCopy) {
	  for (Iterator<T> it=toCopy.begin();it!=toCopy.end();++it) {
		  this->insert(*it,this->end());
	  }
	  return *this;
  }
};




#endif /* LIST_H_ */
