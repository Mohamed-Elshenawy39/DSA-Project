#include "UI.h"
#include "MarsStation.h"
#include <iostream> 
#include <fstream>
#include <cmath> 
#include "Rovers.h"
#include "Missions.h"
using namespace std;

MarsStation::MarsStation() : currentDay(1), maxMissionsBeforeCheckup(0), AutoAbortCount(0) {
    pUI = new UI();
}

void MarsStation::loadFromFile(const string& filename)
{
    // Create an ifstream object
    ifstream inputFile(filename);
    if (!inputFile.is_open()) {
        std::cout << "Error: Could not open input file. Terminating." << std::endl;
        return;
    }

    int numDR, numPR, numNR, numRR; // RR for Rescue Rovers
    inputFile >> numDR >> numPR >> numNR >> numRR;

    int speedDR, speedPR, speedNR, speedRR; // Speed for RR
    inputFile >> speedDR >> speedPR >> speedNR >> speedRR;

    int checkupDR, checkupPR, checkupNR, checkupRR; // Checkup for RR

    // Reading M, Checkup durations
    inputFile >> maxMissionsBeforeCheckup >> checkupDR >> checkupPR >> checkupNR >> checkupRR;

    // Create Rovers
    int roverID = 1;
    for (int i = 0; i < numDR; ++i) {
        Rovers* pRov = new Rovers(roverID++, ROVER_DIGGING, speedDR, checkupDR, maxMissionsBeforeCheckup);
        availableDiggingRovers.enqueue(pRov);
    }
    for (int i = 0; i < numPR; ++i) {
        Rovers* pRov = new Rovers(roverID++, ROVER_POLAR, speedPR, checkupPR, maxMissionsBeforeCheckup);
        availablePolarRovers.enqueue(pRov);
    }
    for (int i = 0; i < numNR; ++i) {
        Rovers* pRov = new Rovers(roverID++, ROVER_NORMAL, speedNR, checkupNR, maxMissionsBeforeCheckup);
        availableNormalRovers.enqueue(pRov);
    }
    for (int i = 0; i < numRR; ++i) { // Creating Rescue Rovers
        Rovers* pRov = new Rovers(roverID++, ROVER_RESCUE, speedRR, checkupRR, 0); // Rescue rovers don't need M check
        availableRescueRovers.enqueue(pRov);
    }


    int numRequests;
    inputFile >> numRequests;
    for (int i = 0; i < numRequests; ++i) {
        char reqType;
        inputFile >> reqType;
        if (reqType == 'R') {
            char missionTypeChar;
            int Rd, id, tloc, md, sig = 10;

            inputFile >> missionTypeChar >> Rd >> id >> tloc >> md;

            MissionType mt;
            if (missionTypeChar == 'P') mt = MISSION_POLAR;
            else if (missionTypeChar == 'N') mt = MISSION_NORMAL;
            else if (missionTypeChar == 'D') mt = MISSION_DIGGING;
            else {
                mt = MISSION_COMPLEX;
                inputFile >> sig;
            }

            Requests* pReq = new NewRequest(Rd, id, mt, tloc, md, NEW_REQ);
            pendingRequests.enqueue(pReq);
        }
        else if (reqType == 'X') {
            int ed, id;
            inputFile >> ed >> id;
            Requests* pReq = new AbortRequest(ed, id, ABORT_REQ);
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

        if (!execMissions.isEmpty()) {
            ExecToBack();
        }

        // STEP 5: Pick 1 mission from OUT to EXEC
        if (!outMissions.isEmpty()) {
            OutToExec();
        }

        // STEP 6: Assign RDY missions to rovers
       // checkMissionFailure();
        assignMissions();

        // STEP 7: Print ALL applicable info (UI logic)
        // (UI print calls go here)
        pUI->printDay(currentDay, this);

        


        // STEP 8: Increment current_day
        incrementDay();
    }

    generateOutputFile("Output.txt");
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
    else if (newMission->getType() == MISSION_COMPLEX) {
        readyComplexMissions.enqueue(newMission);
    }
    else if (newMission->getType() == MISSION_RESCUE) {
        rescueMissions.enqueue(newMission);
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
        inCheckupRovers.enqueue(pRov, -BackDay);
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
    return readyComplexMissions;
}
// NEW:COV

void MarsStation::BackToCompletedMissions()
{
    Missions* pMission;
    int completionDay;

    while (backMissions.peek(pMission, completionDay) && pMission->getCompletionDay() <= currentDay)
    {
        backMissions.dequeue(pMission, completionDay);

        completedMissions.push(pMission);

        Rovers* pRover = pMission->getAssignedRover();
        pRover->incrementMissionsCompleted();

       
       
        if (pMission->getType() == MISSION_COMPLEX)
        {
            int restDays = 3; // Fixed rest period
            int restEndDay = currentDay + restDays;
            for (int i = 0; i < pMission->getExtraRoverCount(); i++)
            {
                Rovers* extraRover = pMission->getExtraRover(i);
                if (extraRover)
                {
                    extraRover->incrementMissionsCompleted();
                    inCheckupRovers.enqueue(extraRover, -restEndDay);
                }
            }

            pRover->resetMissionsCompleted();

            inCheckupRovers.enqueue(pRover, -restEndDay);
        }
        else
        {
            // STANDARD LOGIC (Normal/Polar/Digging)
            // Only goes to checkup if missionsCompleted reached the limit 'M'
            if (pRover->needsCheckup())
            {
                int checkupEndDay = currentDay + pRover->getCheckupDuration();

                inCheckupRovers.enqueue(pRover, -checkupEndDay);

                pRover->resetMissionsCompleted();
            }

            else
            {
                AddRoverToAvailable(pRover);
            }
        }
    }
}



void MarsStation::AddRoverToCheckup(Rovers* rover)
{
	inCheckupRovers.enqueue(rover, -(currentDay + rover->getCheckupDuration()));
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
    if (Type == ROVER_RESCUE)
    {
        availableRescueRovers.enqueue(rover);
	};


}


void MarsStation::ExecToBack()
{
    Missions* pMission;
    int completionDay; // This is the priority from the exec queue

    while (execMissions.peek(pMission, completionDay) && (pMission->getCompletionDay() - pMission->getOneWayTravelTime()) <= currentDay)
    {
        // 1. Remove from Exec Queue
        execMissions.dequeue(pMission, completionDay);

        Rovers* pRover = pMission->getAssignedRover();
        double speed = pRover->getSpeed();

        if (pMission->getType() == MISSION_COMPLEX)
        {
            for (int i = 0; i < pMission->getExtraRoverCount(); i++)
            {
                Rovers* extra = pMission->getExtraRover(i);
                if (extra && extra->getSpeed() < speed)
                {
                    speed = extra->getSpeed();
                }
            }
        }

        
		int backTravelDays = pMission->getOneWayTravelTime();

        // 5. Move to Back Queue
        backMissions.enqueue(pMission, -pMission->getCompletionDay());
    }
}

void MarsStation::OutToExec()
{
    Missions* pMission;
    int oldPriority; // We won't use this for the execution queue

    // Process all missions currently in the 'Out' buffer
    while (outMissions.peek(pMission, oldPriority) && (pMission->getFormulationDay() + pMission->getWaitingDays() + pMission->getOneWayTravelTime() <= currentDay))
    {
        int executionDays = pMission->getMissionDuration();

        int completionDay = currentDay + executionDays;
        
        outMissions.dequeue(pMission, oldPriority);

        execMissions.enqueue(pMission, -(pMission->getCompletionDay() - pMission->getOneWayTravelTime()));
    }
}


void MarsStation::CheckupToAvailable() //Shenawy-2 Points
{
    Rovers* pRov;
    int EndofCheckup;

    // 1. Check Rovers finishing Checkup (Checkup -> Available)
    while (inCheckupRovers.peek(pRov, EndofCheckup) && -EndofCheckup <= currentDay) {
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
            int originalLaunchDay = originalFinishDay - originalDuration - (2 * originalOneWayDays);
            int arrivalAtSiteDay = originalLaunchDay + originalOneWayDays;
            int finishExecDay = arrivalAtSiteDay + originalDuration;

            // Variables for calculations
            int failedSpeed = pFailedRover->getSpeed();
            int rescueSpeed = pRover->getSpeed();

            
            int fullDistance = originalOneWayDays * failedSpeed;

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
                int distToFailure = daysTraveled * 25 * failedSpeed;

                // Distance Remaining to Target
                int distRemaining = fullDistance - distToFailure;

                // 1. Rescue Rover Travel (Base -> Failure Point)
                timeRescueTravelsToTarget = ceil(distToFailure / (25 * rescueSpeed));

                // 2. Failed Rover Return (Failure Point -> Base) using ITS OWN speed
                timeFailedRoverReturns = ceil(distToFailure / (25 * failedSpeed));

                // 3. New Schedule for Rescue Rover
                int timeToCompleteTravel = ceil(distRemaining / (25 * rescueSpeed));
                int durationWork = originalDuration;
                int timeRescueReturn = ceil(fullDistance / (25 * rescueSpeed));

                // New Completion = (Arrival at Failure) + (Travel to Target) + (Duration) + (Return)
                int rescueArrivalDate = currentDay + timeRescueTravelsToTarget;
                newCompletionDay = rescueArrivalDate + timeToCompleteTravel + durationWork + timeRescueReturn;

                newOneWayTravel = timeRescueTravelsToTarget + timeToCompleteTravel;
            }
            

            else if (currentDay <= finishExecDay)
            {
                // 1. Rescue Rover Travel (Base -> Target)
                timeRescueTravelsToTarget = ceil(fullDistance / (25 * rescueSpeed));

                // 2. Failed Rover Return (Target -> Base) using ITS OWN speed
                timeFailedRoverReturns = ceil(fullDistance / (25 * failedSpeed)); // Should equal originalOneWayDays

                // 3. New Schedule
                int daysWorked = currentDay - arrivalAtSiteDay;
                int remainingDuration = originalDuration - daysWorked;
                if (remainingDuration < 0) remainingDuration = 0;

                int timeRescueReturn = ceil(fullDistance / (25*rescueSpeed));

                int rescueArrivalDate = currentDay + timeRescueTravelsToTarget;
                newCompletionDay = rescueArrivalDate + remainingDuration + timeRescueReturn;

                newOneWayTravel = timeRescueTravelsToTarget;
            }
            
            else
            {
               
                // How many days it has been traveling back
                int daysReturning = currentDay - finishExecDay;

                // Distance covered on return
                int distTraveledBack = daysReturning * failedSpeed;

                // Distance Remaining to Base (Failure Point -> Base)
                int distRemainingToBase = fullDistance - distTraveledBack;
                if (distRemainingToBase < 0) distRemainingToBase = 0;

                // 1. Rescue Rover Travel (Base -> Failure Point)
                timeRescueTravelsToTarget = ceil(distRemainingToBase / (25 * rescueSpeed));

                // 2. Failed Rover Return (Failure Point -> Base) using ITS OWN speed
                timeFailedRoverReturns = ceil(distRemainingToBase / (25 * failedSpeed));

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
            inCheckupRovers.enqueue(pFailedRover, -failedRoverArrivalAtBase);

            // 2. Update Mission
            pMission->assignRover(pRover);
            pMission->setTdays(2 * newOneWayTravel + pMission->getMissionDuration());
            pMission->setOneWayTravelTime(newOneWayTravel);
            pMission->setWaitingDays(currentDay - pMission->getFormulationDay());
            pMission->setCompletionDay(pMission->getFormulationDay() + pMission->getWaitingDays() + pMission->getTdays());
			

            // 3. Move to OUT List
            outMissions.enqueue(pMission, -rescueArrivalDate);
        }
        else
        {
            break;
        }
    }

    while (!readyComplexMissions.isEmpty())
    {
        // 1. Peek at the mission to see how many rovers it needs
        readyComplexMissions.peek(pMission);
        int roversNeeded = pMission->getRoversNeeded();

        // 2. We need to gather 'roversNeeded' rovers.
        Rovers* gatheredRovers[3] = { nullptr, nullptr, nullptr };
        bool possible = true;

        // --- Step A: Get the Leader (Digging Rover) ---
        if (availableDiggingRovers.isEmpty()) {
            possible = false;
        }
        else {
            availableDiggingRovers.dequeue(gatheredRovers[0]);
        }

        // --- Step B: Get Extra Rovers (if needed) ---
        if (possible && roversNeeded > 1)
        {
            // Lambda to pick any available rover (excluding Rescue)
            auto getExtraRover = [&]() -> Rovers* {
                Rovers* r = nullptr;
                if (!availableNormalRovers.isEmpty()) availableNormalRovers.dequeue(r);
                else if (!availablePolarRovers.isEmpty()) availablePolarRovers.dequeue(r);
                else if (!availableDiggingRovers.isEmpty()) availableDiggingRovers.dequeue(r);
                return r;
                };

            for (int i = 1; i < roversNeeded; ++i) {
                gatheredRovers[i] = getExtraRover();
                if (gatheredRovers[i] == nullptr) {
                    possible = false; // Not enough extras available
                    break;
                }
            }
        }

        // --- Step C: Finalize or Rollback ---
        if (possible)
        {
            // SUCCESS: We have all required rovers
            readyComplexMissions.dequeue(pMission);

            // Assign Leader
            pMission->assignRover(gatheredRovers[0]);

            // Assign Extras
            for (int i = 1; i < roversNeeded; ++i) {
                pMission->addExtraRover(gatheredRovers[i]);
            }

            // Calculate Speed (Slowest of the group determines pace)
            int slowestSpeed = gatheredRovers[0]->getSpeed();
            for (int i = 1; i < roversNeeded; ++i) {
                if (gatheredRovers[i]->getSpeed() < slowestSpeed) {
                    slowestSpeed = gatheredRovers[i]->getSpeed();
                }
            }

            // Calculate Timings
            int oneWay = ceil(pMission->getTargetLocation() / (slowestSpeed * 25.0));
            int arrival = currentDay + oneWay;
            int completion = arrival + pMission->getMissionDuration() + oneWay;

            pMission->setOneWayTravelTime(oneWay);
            pMission->setTdays(2 * oneWay + pMission->getMissionDuration());
            pMission->setWaitingDays(currentDay - pMission->getFormulationDay());
            pMission->setCompletionDay(pMission->getFormulationDay() + pMission->getWaitingDays() + pMission->getTdays());
            outMissions.enqueue(pMission, -arrival);
        }
        else
        {
            // FAILURE: Not enough rovers. Return everyone we took to their lists.
            for (int i = 0; i < 3; ++i) {
                if (gatheredRovers[i] != nullptr) {
                    AddRoverToAvailable(gatheredRovers[i]);
                }
            }

            // Stop checking complex missions for today
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
            pMission->setTdays(2 * oneWayTravelTime + pMission->getMissionDuration());
            pMission->setWaitingDays(currentDay - pMission->getFormulationDay());
            pMission->setWaitingDays(currentDay - pMission->getFormulationDay());
            pMission->setCompletionDay(pMission->getFormulationDay() + pMission->getWaitingDays() + pMission->getTdays());

            // Move to OUT list (Priority = Arrival Day)
            outMissions.enqueue(pMission, -arrivalAtTargetDay);
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
            pMission->setTdays(2 * oneWayTravelTime + pMission->getMissionDuration());
            pMission->setWaitingDays(currentDay - pMission->getFormulationDay());
            pMission->setCompletionDay(pMission->getFormulationDay() + pMission->getWaitingDays() + pMission->getTdays());

            outMissions.enqueue(pMission, -arrivalAtTargetDay);
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

            pMission->setOneWayTravelTime(oneWayTravelTime);
            pMission->setTdays(2 * oneWayTravelTime + pMission->getMissionDuration());
            pMission->setWaitingDays(currentDay - pMission->getFormulationDay());
            pMission->setCompletionDay(pMission->getFormulationDay() + pMission->getWaitingDays() + pMission->getTdays());

            outMissions.enqueue(pMission, -arrivalAtTargetDay);
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

    // === 1. CHECK EXEC MISSIONS ===
    priQueue<Missions*> tempExecQueue;

    while (execMissions.dequeue(pMission, priority))
    {
        // Generate random number (1-100)
        int randVal = rand() % 100 + 1;

        if (randVal <= failureThreshold)
        {
            // Mission Fails! Move to Rescue Queue
            rescueMissions.enqueue(pMission);
        }
        else
        {
            // Mission Survives, put back in temp queue
            tempExecQueue.enqueue(pMission, priority);
        }
    }
    // Restore EXEC missions
    while (tempExecQueue.dequeue(pMission, priority))
    {
        execMissions.enqueue(pMission, priority);
    }

    // === 2. CHECK OUT MISSIONS ===
    priQueue<Missions*> tempOutQueue;

    while (outMissions.dequeue(pMission, priority))
    {
        // Generate random number (1-100)
        int randVal = rand() % 100 + 1;

        if (randVal <= failureThreshold)
        {
            // Mission Fails! Move to Rescue Queue
            rescueMissions.enqueue(pMission);
        }
        else
        {
            // Mission Survives, put back in temp queue
            tempOutQueue.enqueue(pMission, priority);
        }
    }

    // Restore OUT missions
    while (tempOutQueue.dequeue(pMission, priority))
    {
        outMissions.enqueue(pMission, priority);
    }
}

void MarsStation::generateOutputFile(const string& filename)
{
    ofstream OutputFile(filename);
    if (!OutputFile.is_open()) {
        cout << "Error: Could not open output file." << endl;
        return;
    }

    OutputFile << "Fday\tID\tRday\tWdays\tMDUR\tTdays\n";

    ArrayStack<Missions*>temp = completedMissions;
    Missions* m;
    double sumW = 0;
    double sumMDUR = 0;
    double sumT = 0;
    int countCompleted = completedMissions.getCount();
    int doneN = 0;
    int doneP = 0;
    int doneD = 0;
    int doneC = 0;
    int abortedCount = abortedMissions.getCount();
    int countNR = availableNormalRovers.getCount();
    int countPR = availablePolarRovers.getCount();
    int countDR = availableDiggingRovers.getCount();
    int countRR = availableRescueRovers.getCount();

    while (!temp.isEmpty())
    {
        temp.pop(m);
        OutputFile << m->getCompletionDay() << "\t"
            << m->getID() << "\t"
            << m->getFormulationDay() << "\t"
            << m->getWaitingDays() << "\t"
            << m->getMissionDuration() << "\t"
            << m->getTdays() << "\n";

        sumMDUR += m->getMissionDuration();
        sumW += m->getWaitingDays();
        sumT += m->getTdays();
        if (m->getType() == MISSION_NORMAL)
            doneN++;
        else if (m->getType() == MISSION_POLAR)
            doneP++;
        else if (m->getType() == MISSION_DIGGING)
            doneD++;
        else if (m->getType() == MISSION_COMPLEX)
            doneC++;
    }
    double avgW = sumW / countCompleted;
    double avgMDUR = sumMDUR / countCompleted;
    double avgT = sumT / countCompleted;
    double ratioW_MDUR = (sumW / sumMDUR) * 100;
    int totalMissions = countCompleted + abortedCount;
    double Auto_Aborted = (AutoAbortCount / abortedCount) * 100;
    int totalRovers = countNR + countPR + countDR + countRR;

    OutputFile << "\nMissions: " << totalMissions;

    OutputFile << " [N: " << doneN
        << ", P: " << doneP
        << ", D: " << doneD
        << ", C: " << doneC << "] ";

    OutputFile << "[" << countCompleted << " DONE, "
        << abortedCount << " Aborted]\n\n";

    OutputFile << "Rovers: " << totalRovers;
    OutputFile << " [N: " << countNR << ", P: " << countPR << ", D: " << countDR << ", RR: " << countRR << "]\n";

    OutputFile << "Avg Wdays = " << avgW
        << ", Avg MDUR = " << avgMDUR
        << ", Avg Tdays = " << avgT << "\n";

    OutputFile << "% Avg_Wdays/ Avg_MDUR = " << ratioW_MDUR << "%, ";
    OutputFile << "Auto-Aborted Missions = " << Auto_Aborted << "%\n";


    OutputFile.close();
}