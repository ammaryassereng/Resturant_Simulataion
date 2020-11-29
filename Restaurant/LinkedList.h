#ifndef __LINKEDLIST_H_
#define __LINKEDLIST_H_

#include<iostream>
#include"Generic_DS/Node.h"
using namespace std;

template<typename T>
class LinkedList
{
	Node<T>* Head;
public:
	LinkedList()                   //constructor
	{
		Head = nullptr;
	}
	~LinkedList()					//destructor
	{
		DeleteAll();
	}
	int getCount() const	//returns the number of linked list elements
	{
		if (!Head)
			return 0;
		int c = 0;
		Node<T>* tmp = Head;
		while (tmp)
		{
			c++;
			tmp = tmp->getNext();
		}
		return c;
	}
	void DeleteAll()				//delete all elements in the list
	{
		Node<T>* P = Head;
		while (Head)
		{
			P = Head->getNext();
			delete Head;
			Head = P;
		}
	}
	void ADD(T& value)
	{
		if (Head == nullptr)	// Adds element to the begining of the list
		{
			Head = new Node<T>(value);
			return;
		}
		Node<T>* PtrToNewNode = new Node<T>(value);
		PtrToNewNode->setNext(Head);
		Head = PtrToNewNode;
	}
	void InsertEnd(const T& data)  //Adds element to the end of the list
	{
		Node<T>* ptr = Head;
		if (Head == nullptr)
		{
			Node<T>* P = new Node<T>(data);
			Head = P;
			P->setNext(nullptr);
		}
		else
		{
			while (ptr->getNext())
			{
				ptr = ptr->getNext();
			}
			Node<T>* P = new Node<T>(data);
			ptr->setNext(P);
		}
	}
	bool IsEmpty()				// Checks if the list is empty
	{

		if (Head == nullptr)
		{
			return true;
		}
		return false;
	}
	bool Peek(T & value)	// it returns true if the list has atleast one element  
	{								//assigns value to the first element in the list if the list is not empty
		if (Head == nullptr)
		{
			return false;
		}
		value = Head->getItem();
		return true;
	}
	void DeleteFirst()			// Deletes the first element in the list
	{
		if (Head == nullptr)
		{
			return;
		}
		Node<T>* ptr = Head;
		Head = Head->getNext();
		delete ptr;
		return;
	}
	bool Find(T Key)			// Find if a certain element exists in the List 
	{
		if (Head == nullptr)
		{
			return false;
		}
		else
		{
			Node<T>* ptr = Head;
			while (ptr)
			{
				if (ptr->getItem() == Key)
				{
					return true;
				}
				else
				{
					ptr = ptr->getNext();
				}
			}
			return false;
		}
	}
	bool Remove(T &item)		//delete the first nde that carries the value of item
	{
		if (Head == nullptr)
		{
			return false;
		}
		else if (Head->getItem() == item)
		{
			if (Head->getNext() == nullptr)
			{
				delete Head;
				Head = nullptr;
				return true;
			}
			else
			{
				Node<T>* p = Head;
				Head = Head->getNext();
				delete p;
				return true;
			}
		}
		Node<T>* ptr = Head;
		Node<T>* ptr2 = Head->getNext();
		while (ptr2)
		{
			if (ptr2->getItem() == item)
			{
				ptr->setNext(ptr2->getNext());
				delete ptr2;
				return true;
			}
			else
			{
				ptr = ptr2;
				ptr2 = ptr2->getNext();
			}
		}
		return false;
	}
	void InsertSorted(T item)		//insert items in the list so that it is sorted in descending order
	{
		if (Head == nullptr)
		{
			Node<T>* ptr = new Node<T>(item);
			Head = ptr;
			return;
		}
		if (item > Head->getItem())
		{
			Node<T>* ptr = new Node<T>(item);
			ptr->setNext(Head);
			Head = ptr;
			return;
		}
		Node<T>* ptr = Head;
		Node<T>* ptr2 = Head->getNext();
		while (ptr2)
		{
			if (ptr2->getItem() < item)
			{
				Node<T>* ptr3 = new Node<T>(item);
				ptr3->setNext(ptr2);
				ptr->setNext(ptr3);
				return;
			}
			else
			{
				ptr = ptr->getNext();
				ptr2 = ptr2->getNext();
			}
		}
		Node<T>* ptr3 = new Node<T>(item);
		ptr->setNext(ptr3);
	}
};

#endif