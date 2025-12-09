#pragma once
#include "priQueue.h"
#include "Missions.h"

class OutMissionsPriQueue : public priQueue<Missions*> {
public:
    OutMissionsPriQueue() : priQueue<Missions*>() {

    }

    bool AbortMission(int missionID, Missions*& abortedMission) {
        int dummyPri;
        if (isEmpty()) {
            abortedMission = nullptr;
            return false;
        }

        // Case 1: The mission to abort is at the head
        if (head->getItem(dummyPri)->getID() == missionID) {

            dequeue(abortedMission, dummyPri); // Use base class dequeue
            return true;
        }

        // Case 2: The mission is in the middle or at the end
        priNode<Missions*>* previous = head;
        priNode<Missions*>* current = head->getNext();

        while (current) {
            if (current->getItem(dummyPri)->getID() == missionID) {
                // Found it. Unlink the node.
                previous->setNext(current->getNext()); // Link previous to next
                abortedMission = current->getItem(dummyPri); // Get the mission pointer
                delete current; // Delete the node
                count--;
                return true;
            }
            previous = current;
            current = current->getNext();
        }

        abortedMission = nullptr;
        return false; // Mission not found
    }
};