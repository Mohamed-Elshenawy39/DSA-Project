#include "UI.h"
#include "MarsStation.h"
#include <iostream> 
#include <fstream>
using namespace std;

MarsStation::MarsStation() : currentDay(1), maxMissionsBeforeCheckup(0) {
    pUI = new UI();
}

void MarsStation::loadFromFile(const string& filename)
{

    // Create an ifstream object
    ifstream inputFile(filename);

    // Check if the file opened successfully
    if (!inputFile.is_open()) {
        cout << "Error: Could not open input file." << endl;
        return;
    }

    int numDR, numPR, numNR;
    inputFile >> numDR >> numPR >> numNR;

    int speedDR, speedPR, speedNR;
    inputFile >> speedDR >> speedPR >> speedNR;

    int checkupDR, checkupPR, checkupNR;

    inputFile >> this->maxMissionsBeforeCheckup >> checkupDR >> checkupPR >> checkupNR;

    // Create and enqueue Digging Rovers
    for (int i = 0; i < numDR; ++i) {
        Rovers* pRov = new Rovers(i + 1, ROVER_DIGGING, speedDR, checkupDR, maxMissionsBeforeCheckup);
        availableDiggingRovers.enqueue(pRov);
    }
    // Create and enqueue Polar Rovers
    for (int i = 0; i < numPR; ++i) {
        Rovers* pRov = new Rovers(i + 1 + numDR, ROVER_POLAR, speedPR, checkupPR, maxMissionsBeforeCheckup);
        availablePolarRovers.enqueue(pRov);
    }
    // Create and enqueue Normal Rovers
    for (int i = 0; i < numNR; ++i) {
        Rovers* pRov = new Rovers(i + 1 + numDR + numPR, ROVER_NORMAL, speedNR, checkupNR, maxMissionsBeforeCheckup);
        availableNormalRovers.enqueue(pRov);
    }

    // Loop to read all the requests
    int numRequests;
    inputFile >> numRequests;
    for (int i = 0; i < numRequests; ++i) {
        char reqType;
        inputFile >> reqType;
        if (reqType == 'R') { // New Mission Request
            char missionType;
            int Rd, id, tloc, md;
            inputFile >> missionType >> Rd >> id >> tloc >> md;

            MissionType mt;
            if (missionType == 'P') 
                mt = MISSION_POLAR;

            else if (missionType == 'N') 
                mt = MISSION_NORMAL;

            else 
                mt = MISSION_DIGGING;

            Requests* pReq = new NewRequest(Rd, id, mt, tloc, md);
            pendingRequests.enqueue(pReq);
        }
        else if (reqType == 'X') { // Abort Mission Request
            int ed, id;
            inputFile >> ed >> id;
            Requests* pReq = new AbortRequest(ed, id);
            pendingRequests.enqueue(pReq);
        }
    }

    // Close the file
    inputFile.close();
}

void MarsStation::processPendingRequests()
{
    Requests* pReq;
    while (pendingRequests.peek(pReq) && pReq->getEventDay() <= currentDay) {
        pendingRequests.dequeue(pReq);
        pReq->Operate(this);
        delete pReq;
    }
}


void MarsStation::runSimulation()
{
	loadFromFile("Input.txt");
    processPendingRequests();
}

void MarsStation::addMission(Missions* newMission)
{
    if (newMission->getType() == MISSION_POLAR) {
        readyPolarMissions.enqueue(newMission);
    }
    else if (newMission->getType() == MISSION_DIGGING) {
        readyDiggingMissions.enqueue(newMission);
    }
    else {
        readyNormalMissions.enqueue(newMission);
    }
}

void MarsStation::abortMission(int missionID)
{
}

LinkedQueue<Missions*>& MarsStation::getReadyPolarMissions() 
{
	return readyPolarMissions;
}

ReadyNormalMissionsQueue& MarsStation::getReadyNormalMissions()
{
	return readyNormalMissions;
}
LinkedQueue<Missions*>& MarsStation::getReadyDiggingMissions()
{
	return readyDiggingMissions;
}

OutMissionsPriQueue& MarsStation::getOutMissions()
{
	return outMissions;
}

priQueue<Missions*>& MarsStation::getExecMissions()
{
	return execMissions;
}

priQueue<Missions*>& MarsStation::getBackMissions()
{
	return backMissions;
}

LinkedQueue<Rovers*>& MarsStation::getAvailablePolarRovers()
{
	return availablePolarRovers;
}

LinkedQueue<Rovers*>& MarsStation::getAvailableNormalRovers()
{
	return availableNormalRovers;
}

LinkedQueue<Rovers*>& MarsStation::getAvailableDiggingRovers()
{
	return availableDiggingRovers;
}

priQueue<Rovers*>& MarsStation::getInCheckupRovers()
{
	return inCheckupRovers;
}

ArrayStack<Missions*>& MarsStation::getCompletedMissions()
{
	return completedMissions;
}

LinkedQueue<Missions*>& MarsStation::getAbortedMissions()
{
    return abortedMissions;
}

LinkedQueue<Requests*> MarsStation::getpendingRequests()
{
    return pendingRequests;
}


