#pragma once
#include "priNode.h"
#include <iostream>
using namespace std;
//This class impelements the priority queue as a sorted list (Linked List)
//The item with highest priority is at the front of the queue
template <typename T>
class priQueue
{
    priNode<T>* head;
    int count;
public:
    priQueue() : head(nullptr), count(0) {}

    ~priQueue() {
        T tmp;
        int p;
        while (dequeue(tmp,p));
    }

	int getCount() const {
		return count;
	}

    //insert the new node in its correct position according to its priority
    void enqueue(const T& data, int priority) {
        priNode<T>* newNode = new priNode<T>(data, priority);

        if (head == nullptr || priority > head->getPri()) {
            
            newNode->setNext(head);
            head = newNode;
            count++;
            return;
        }
       
        priNode<T>* current = head;        
        while (current->getNext() && priority <= current->getNext()->getPri()) {
            current = current->getNext();
        }
        newNode->setNext( current->getNext());
        current->setNext( newNode);  
		count++;
    }

    bool dequeue(T& topEntry, int& pri) {
        if (isEmpty())
            return false;

        topEntry = head->getItem(pri);
        priNode<T>* temp = head;
        head = head->getNext();
        delete temp;
		count--;
        return true;

    }

    bool peek(T& topEntry, int& pri) {
        if (isEmpty())
            return false;

        topEntry = head->getItem(pri);
        return true;
    }

    bool isEmpty() const {
        return head == nullptr;
    }
	void printQueue() {
		priNode<T>* current = head;
		while (current) {
			int p = current ->getPri();
			T item = current->getItem(p);
			cout << item;
            //cout << "Priority: " << current->getPri();
			current = current->getNext();
		}
	}
};
