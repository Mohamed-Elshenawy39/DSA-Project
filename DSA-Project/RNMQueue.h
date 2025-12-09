#pragma once
#include "LinkedQueue.h"
#include "Missions.h"

class ReadyNormalMissionsQueue : public LinkedQueue<Missions*> {
public:
	ReadyNormalMissionsQueue() : LinkedQueue<Missions*>() {

	}

    bool AbortMission(int missionID, Missions*& abortedMission) {
        if (isEmpty()) {
            abortedMission = nullptr;
            return false;
        }

        Node<Missions*>* prev = nullptr;
        Node<Missions*>* current = frontPtr;

        // 1. Search the list
        while (current) {
            if (current->getItem()->getID() == missionID) {
                // Found the mission. Now unlink it.
                abortedMission = current->getItem(); // Get the mission pointer

                if (current == frontPtr) {
                    // Case 1: Mission is at the front
                    frontPtr = current->getNext();
                    if (frontPtr == nullptr) { // List is now empty
                        backPtr = nullptr;
                    }
                }
                else {
                    // Case 2/3: Mission is in the middle or at the back
                    prev->setNext(current->getNext());

                    // Update backPtr if the removed node was the last one
                    if (current == backPtr) {
                        backPtr = prev;
                    }
                }

                delete current; // Delete the node
                count--;
                return true;
            }

            // Move to the next node
            prev = current;
            current = current->getNext();
        }

        abortedMission = nullptr;
        return false; // Mission not found

    }
};