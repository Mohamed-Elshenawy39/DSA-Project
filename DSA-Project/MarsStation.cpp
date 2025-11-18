#include "UI.h"
#include "MarsStation.h"
#include <iostream> 
#include <fstream>
#include <cmath> 
#include "Rovers.h"
#include "Missions.h"
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

            Requests* pReq = new NewRequest(Rd, id, mt, tloc, md, NEW_REQ);
            pendingRequests.enqueue(pReq);
        }
        else if (reqType == 'X') { // Abort Mission Request
            int ed, id;
            inputFile >> ed >> id;
            Requests* pReq = new AbortRequest(ed, id , ABORT_REQ);
            pendingRequests.enqueue(pReq);
        }
    }

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
    while (!ISsimdone())
    {
        // STEP 1: Process new requests scheduled for today
        processPendingRequests();

        // STEP 2: Move rover from Checkup to Available
        // (Your Step 2 logic)
        CheckupToAvailable();
       
        // CheckupToAvailable(...); // Or however you implemented it

        // STEP 3: Pick one mission from BACK to DONE
        if (!backMissions.isEmpty()) {
            BackToCompletedMissions();
        }

        // STEP 4: Pick 2 missions from EXEC to BACK
        if (!execMissions.isEmpty()) {
            ExecToBack();
        }
        if (!execMissions.isEmpty()) {
            ExecToBack();
        }

        // STEP 5: Pick 1 mission from OUT to EXEC
        if (!outMissions.isEmpty()) {
            OutToExec();
        }

        // STEP 6: Assign RDY missions to rovers
        assignMissions();

        // STEP 7: Print ALL applicable info (UI logic)
        // (UI print calls go here)
        pUI->printDay(currentDay, this);

        


        // STEP 8: Increment current_day
        incrementDay();
    }

    // Simulation is over
    cout << "\nSIMULATION ENDED." << endl;
}
    


void MarsStation::addMission(Missions* newMission)
{
    if (newMission->getType() == MISSION_POLAR) {
        readyPolarMissions.enqueue(newMission);
    }
    else if (newMission->getType() == MISSION_DIGGING) {
        readyDiggingMissions.enqueue(newMission);
    }
    else if (newMission->getType() == MISSION_NORMAL) {
        readyNormalMissions.enqueue(newMission);
    }
    else if (newMission->getType() == MISSION_ABORT)
        abortedMissions.enqueue(newMission);
}

void MarsStation::abortMission(int missionID)
{
}



LinkedQueue<Missions*>& MarsStation::getReadyPolarMissions() 
{
	return readyPolarMissions ;  
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

// NEW:COV

void MarsStation::BackToCompletedMissions()  
{
    Missions* mission;
    int pri;
    backMissions.dequeue(mission, pri);
    completedMissions.push(mission);
  
    Rovers* rover  = mission->getAssignedRover();
	int randomnumber = (rand() % 100) + 1; // Random number between 1 and 100
    rover->setMissionsDone(randomnumber);
	bool test=rover->needsCheckup(rover);
    if (test)
        AddRoverToCheckup(rover);
    else
		AddRoverToAvailable(rover);



}

void MarsStation::AddRoverToCheckup(Rovers* rover)
{
	inCheckupRovers.enqueue(rover, currentDay + rover->getCheckupDuration());
}

void MarsStation::AddRoverToAvailable(Rovers* rover)
{
    RoverType Type = rover->getType();

    if (Type== ROVER_POLAR)
    {
        availablePolarRovers.enqueue(rover);
    };
    if (Type == ROVER_NORMAL)
    {
        availableNormalRovers.enqueue(rover);
    };
    if (Type == ROVER_DIGGING)
    {
        availableDiggingRovers.enqueue(rover);
    };


}


void MarsStation::ExecToBack()
{
    Missions* mission;
	int pri;
	execMissions.dequeue(mission, pri);
    
    backMissions.enqueue(mission, pri);
	
}

void MarsStation::OutToExec()
{
    Missions* mission;
    int pri;
	outMissions.dequeue(mission, pri);
	execMissions.enqueue(mission, pri);
}


void MarsStation::CheckupToAvailable()
{
    Rovers* rover;
    int pri;

    // STEP 1: Check if the first rover in the queue is done with its checkup duration.
    // The priority 'pri' is the completion day, set in AddRoverToCheckup.
    if (inCheckupRovers.peek(rover, pri) && pri <= currentDay)
    {
        // STEP 2: The rover is DONE with checkup time. Dequeue it for final processing.
        inCheckupRovers.dequeue(rover, pri);

        // The original checkup duration is over.
        // We now apply the 30% random chance, but for a different purpose:
        // A rover passing checkup should move to available, 
        // A rover failing checkup should be reassigned a NEW checkup duration (e.g., for repair).

        int randomnumber = (rand() % 100) + 1;

        if (randomnumber < 70)
        {
            // 70% chance of being fine: Move to available
            AddRoverToAvailable(rover);
        }
        else
        {
            // 30% chance of requiring more work: Re-enqueue for a second checkup duration
            // This is the correct use of AddRoverToCheckup if repair is needed.
            AddRoverToCheckup(rover);
        }
    }
    // IMPORTANT: If the rover is NOT done (pri > currentDay), we do nothing and exit. 
    // It remains in the checkup queue, waiting for the next day.
}

void MarsStation::incrementDay()
{
	currentDay++;
}

bool MarsStation::ISsimdone() const {
    // "End the simulation when ALL missions are in the DONE list"
    // This means all pending requests are handled and all active mission lists are empty.

    return pendingRequests.isEmpty() &&
        readyPolarMissions.isEmpty() &&
        readyNormalMissions.getCount() == 0 &&
        readyDiggingMissions.isEmpty() &&
        outMissions.getCount() == 0 &&
        execMissions.isEmpty() &&
        backMissions.isEmpty();
}

int MarsStation::getCurrentDay()
{
    return currentDay;
}


void MarsStation::assignMissions()
{
    Missions* pMission;
    Rovers* pRover;

    if (!readyPolarMissions.isEmpty() && !availablePolarRovers.isEmpty()) {

        readyPolarMissions.dequeue(pMission);
        availablePolarRovers.dequeue(pRover);
        pMission->assignRover(pRover);

 
        int oneWayTravelTime = ceil(pMission->getTargetLocation() / (pRover->getSpeed() * 25));

        int arrivalAtTargetDay = currentDay + oneWayTravelTime;

        int executionEndDay = arrivalAtTargetDay + pMission->getMissionDuration();

        int completionDay = executionEndDay + oneWayTravelTime;

        pMission->setOneWayTravelTime(oneWayTravelTime);
        pMission->setCompletionDay(completionDay);
        pMission->setWaitingDays(currentDay - pMission->getFormulationDay());

        outMissions.enqueue(pMission, arrivalAtTargetDay);
    }

    // --- 2. Assign Digging Missions (DM) ---
    // (Logic is identical to Polar)
    if (!readyDiggingMissions.isEmpty() && !availableDiggingRovers.isEmpty()) {

        readyDiggingMissions.dequeue(pMission);
        availableDiggingRovers.dequeue(pRover);

        pMission->assignRover(pRover);

        int oneWayTravelTime = ceil(pMission->getTargetLocation() / (pRover->getSpeed() * 25));
        int arrivalAtTargetDay = currentDay + oneWayTravelTime;
        int executionEndDay = arrivalAtTargetDay + pMission->getMissionDuration();
        int completionDay = executionEndDay + oneWayTravelTime;

        pMission->setOneWayTravelTime(oneWayTravelTime);
        pMission->setCompletionDay(completionDay);
        pMission->setWaitingDays(currentDay - pMission->getFormulationDay());

        outMissions.enqueue(pMission, arrivalAtTargetDay);
    }

    // --- 3. Assign Normal Missions (NM) ---
    // (Logic is identical to Polar)
    if (!readyNormalMissions.isEmpty() && !availableNormalRovers.isEmpty()) {

        readyNormalMissions.dequeue(pMission);
        availableNormalRovers.dequeue(pRover);

        pMission->assignRover(pRover);

        int oneWayTravelTime = ceil(pMission->getTargetLocation() / (pRover->getSpeed() * 25));
        int arrivalAtTargetDay = currentDay + oneWayTravelTime;
        int executionEndDay = arrivalAtTargetDay + pMission->getMissionDuration();
        int completionDay = executionEndDay + oneWayTravelTime;

        pMission->setOneWayTravelTime(oneWayTravelTime);
        pMission->setCompletionDay(completionDay);
        pMission->setWaitingDays(currentDay - pMission->getFormulationDay());

        outMissions.enqueue(pMission, arrivalAtTargetDay);
    }
}


