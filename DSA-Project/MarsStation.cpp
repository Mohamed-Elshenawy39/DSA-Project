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
    
void MarsStation::AutoAbortPolarMissions() { //Shenawy - 1 Point

    LinkedQueue<Missions*> tempQueue;
    Missions* pMis;

    while (readyPolarMissions.dequeue(pMis)) {
        // Condition: PM waiting in the ready list > double its duration

        // Waiting Days = Current Day - Formulation Day (Rday)
        int waitingDays = currentDay - pMis->getFormulationDay();
        int abortThreshold = 2 * pMis->getMissionDuration();

        if (waitingDays > abortThreshold) {
            // Abort condition met
            abortedMissions.enqueue(pMis);
        }
        else {
            // Condition not met, mission is safe, put in temp queue
            tempQueue.enqueue(pMis);
        }
    }

    // Restore the non-aborted missions to the original queue
    while (tempQueue.dequeue(pMis)) {
        readyPolarMissions.enqueue(pMis);
    }
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

void MarsStation::abortMission(int missionID) //Sheno - 4 Points
{
    Missions* pMis = nullptr;
    Rovers* pRov = nullptr;

    // 1. Try to find and abort from RDY_NM list
    if (readyNormalMissions.AbortMission(missionID, pMis)) {
        if (pMis) abortedMissions.enqueue(pMis);
        return;
    }

    // 2. Try to find and abort from OUT_missions list
    if (outMissions.AbortMission(missionID, pMis)) {
        if (!pMis) return;

        abortedMissions.enqueue(pMis);
        pRov = pMis->getAssignedRover();

        int returnTime = pMis->getOneWayTravelTime();
        int BackDay = currentDay + returnTime;

        // Rover is now returning (forced checkup state until return day)
        inCheckupRovers.enqueue(pRov, BackDay);
        return;
    }
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

LinkedQueue<Rovers*>& MarsStation::getAvailableRescueRovers()
{
    return availableRescueRovers;
}

LinkedQueue<Missions*>& MarsStation::getRescueMissions()
{
    return rescueMissions;
}
LinkedQueue<Missions*>& MarsStation::getReadyComplexMissions()
{
    return complexMissions;
}
// NEW:COV

void MarsStation::BackToCompletedMissions()
{
    Missions* pMission;
    int completionDay; 

    while (backMissions.peek(pMission, completionDay) && completionDay <= currentDay)
    {
        backMissions.dequeue(pMission, completionDay);

        completedMissions.push(pMission);

        Rovers* pRover = pMission->getAssignedRover();
        pRover->incrementMissionsCompleted();

        if (pRover->needsCheckup())
        {
            int checkupEndDay = currentDay + pRover->getCheckupDuration();

            inCheckupRovers.enqueue(pRover, checkupEndDay);

            pRover->resetMissionsCompleted();
        }
        else
        {
            AddRoverToAvailable(pRover);
        }
    }
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


void MarsStation::CheckupToAvailable() //Shenawy-2 Points
{
    Rovers* pRov;
    int EndofCheckup;

    // 1. Check Rovers finishing Checkup (Checkup -> Available)
    while (inCheckupRovers.peek(pRov, EndofCheckup) && EndofCheckup <= currentDay) {
        inCheckupRovers.dequeue(pRov, EndofCheckup);
        AddRoverToAvailable(pRov);
    }
}

void MarsStation::incrementDay()
{
	currentDay++;
}

bool MarsStation::ISsimdone() const {
    
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


void MarsStation::assignMissions() //Aty 3 points
{
    Missions* pMission;
    Rovers* pRover;
    while (!rescueMissions.isEmpty())
    {
        // Try to find a Rescue Rover
        if (availableRescueRovers.dequeue(pRover))
        {
            rescueMissions.dequeue(pMission);

            // --- 1. RETRIEVE FAILED ROVER ---
            Rovers* pFailedRover = pMission->getAssignedRover();

            // --- 2. RECONSTRUCT ORIGINAL SCHEDULE ---
            int originalOneWayDays = pMission->getOneWayTravelTime();
            int originalDuration = pMission->getMissionDuration();
            int originalFinishDay = pMission->getCompletionDay();

            // Calculate Key Days
            // (Assuming Fday included the return trip)
            int originalLaunchDay = originalFinishDay - originalDuration - (2 * originalOneWayDays);
            int arrivalAtSiteDay = originalLaunchDay + originalOneWayDays;
            int finishExecDay = arrivalAtSiteDay + originalDuration;

            // Variables for calculations
            double failedSpeed = pFailedRover->getSpeed();
            double rescueSpeed = pRover->getSpeed();

            
            double fullDistance = originalOneWayDays * failedSpeed;

            int timeRescueTravelsToTarget = 0;
            int timeFailedRoverReturns = 0;

            int newOneWayTravel = 0;    // For the Rescue Rover's stats
            int newCompletionDay = 0;   // For the Mission

          
            if (currentDay <= arrivalAtSiteDay)
            {
                // How many days it had traveled
                int daysTraveled = currentDay - originalLaunchDay;
                if (daysTraveled < 0) daysTraveled = 0;

                // Distance to the Failure Point
                double distToFailure = daysTraveled * failedSpeed;

                // Distance Remaining to Target
                double distRemaining = fullDistance - distToFailure;

                // 1. Rescue Rover Travel (Base -> Failure Point)
                timeRescueTravelsToTarget = ceil(distToFailure / rescueSpeed);

                // 2. Failed Rover Return (Failure Point -> Base) using ITS OWN speed
                timeFailedRoverReturns = ceil(distToFailure / failedSpeed);

                // 3. New Schedule for Rescue Rover
                int timeToCompleteTravel = ceil(distRemaining / rescueSpeed);
                int durationWork = originalDuration;
                int timeRescueReturn = ceil(fullDistance / rescueSpeed);

                // New Completion = (Arrival at Failure) + (Travel to Target) + (Duration) + (Return)
                int rescueArrivalDate = currentDay + timeRescueTravelsToTarget;
                newCompletionDay = rescueArrivalDate + timeToCompleteTravel + durationWork + timeRescueReturn;

                newOneWayTravel = timeRescueTravelsToTarget + timeToCompleteTravel;
            }
            

            else if (currentDay <= finishExecDay)
            {
                // 1. Rescue Rover Travel (Base -> Target)
                timeRescueTravelsToTarget = ceil(fullDistance / rescueSpeed);

                // 2. Failed Rover Return (Target -> Base) using ITS OWN speed
                timeFailedRoverReturns = ceil(fullDistance / failedSpeed); // Should equal originalOneWayDays

                // 3. New Schedule
                int daysWorked = currentDay - arrivalAtSiteDay;
                int remainingDuration = originalDuration - daysWorked;
                if (remainingDuration < 0) remainingDuration = 0;

                int timeRescueReturn = ceil(fullDistance / rescueSpeed);

                int rescueArrivalDate = currentDay + timeRescueTravelsToTarget;
                newCompletionDay = rescueArrivalDate + remainingDuration + timeRescueReturn;

                newOneWayTravel = timeRescueTravelsToTarget;
            }
            
            else
            {
               
                // How many days it has been traveling back
                int daysReturning = currentDay - finishExecDay;

                // Distance covered on return
                double distTraveledBack = daysReturning * failedSpeed;

                // Distance Remaining to Base (Failure Point -> Base)
                double distRemainingToBase = fullDistance - distTraveledBack;
                if (distRemainingToBase < 0) distRemainingToBase = 0;

                // 1. Rescue Rover Travel (Base -> Failure Point)
                timeRescueTravelsToTarget = ceil(distRemainingToBase / rescueSpeed);

                // 2. Failed Rover Return (Failure Point -> Base) using ITS OWN speed
                timeFailedRoverReturns = ceil(distRemainingToBase / failedSpeed);

                // 3. New Schedule
                
                int timeRescueReturn = timeRescueTravelsToTarget;

                int rescueArrivalDate = currentDay + timeRescueTravelsToTarget;
                newCompletionDay = rescueArrivalDate + timeRescueReturn;

                newOneWayTravel = timeRescueTravelsToTarget;
            }

            // --- APPLY LOGIC ---

            // 1. Failed Rover Start Time
            int rescueArrivalDate = currentDay + timeRescueTravelsToTarget;
            int failedRoverArrivalAtBase = rescueArrivalDate + timeFailedRoverReturns;

            // Enqueue Failed Rover for Checkup
            inCheckupRovers.enqueue(pFailedRover, failedRoverArrivalAtBase);

            // 2. Update Mission
            pMission->assignRover(pRover);
            pMission->setOneWayTravelTime(newOneWayTravel);
            pMission->setCompletionDay(newCompletionDay);

            // 3. Move to OUT List
            outMissions.enqueue(pMission, rescueArrivalDate);
        }
        else
        {
            break;
        }
    }


    while (!readyPolarMissions.isEmpty())
    {
        pRover = nullptr;

        // Try to find a rover in order 
        if (!availablePolarRovers.isEmpty())
            availablePolarRovers.dequeue(pRover);
        else if (!availableNormalRovers.isEmpty())
            availableNormalRovers.dequeue(pRover);
        else if (!availableDiggingRovers.isEmpty())
            availableDiggingRovers.dequeue(pRover);

        // assign it
        if (pRover)
        {
            readyPolarMissions.dequeue(pMission);
            pMission->assignRover(pRover);

            int oneWayTravelTime = ceil(pMission->getTargetLocation() / (pRover->getSpeed() * 25));
            int arrivalAtTargetDay = currentDay + oneWayTravelTime;
            int executionEndDay = arrivalAtTargetDay + pMission->getMissionDuration();
            int completionDay = executionEndDay + oneWayTravelTime;

            pMission->setOneWayTravelTime(oneWayTravelTime);
            pMission->setCompletionDay(completionDay);
            pMission->setWaitingDays(currentDay - pMission->getFormulationDay());

            // Move to OUT list (Priority = Arrival Day)
            outMissions.enqueue(pMission, arrivalAtTargetDay);
        }
        else
        {
            break;
        }
    }

    while (!readyDiggingMissions.isEmpty())
    {
        pRover = nullptr;

        if (!availableDiggingRovers.isEmpty())
        {
            availableDiggingRovers.dequeue(pRover);
            readyDiggingMissions.dequeue(pMission);
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
        else
        {
            break;
        }
    }

 
    while (!readyNormalMissions.isEmpty())
    {
        pRover = nullptr;

        if (!availableNormalRovers.isEmpty())
            availableNormalRovers.dequeue(pRover);
        else if (!availablePolarRovers.isEmpty())
            availablePolarRovers.dequeue(pRover);

        if (pRover)
        {
            readyNormalMissions.dequeue(pMission);
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
        else
        {
            break;
        }
    }
}

void MarsStation::checkMissionFailure()
{
    int failureThreshold = 5; // 5% chance
    Missions* pMission;
    int priority;
    priQueue<Missions*> tempQueue;

    while (execMissions.dequeue(pMission, priority))
    {
        // Generate random number (1-100)
        int randVal = rand() % 100 + 1;

        if (randVal <= failureThreshold)
        {
         
            rescueMissions.enqueue(pMission);
        }
        else
        {
            tempQueue.enqueue(pMission, priority);
        }
    }

    while (tempQueue.dequeue(pMission, priority))
    {
        execMissions.enqueue(pMission, priority);
    }
}

