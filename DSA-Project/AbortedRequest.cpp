#include "AbortedRequest.h"
#include "MarsStation.h"
void AbortRequest::Operate(MarsStation* station)
{
    Missions* pMission = new Missions (this->missionID,MISSION_ABORT,this->requestDay,0,0,0);
    station->addMission(pMission); // Adds to the Aborted Missions List
    station->abortMission(this->missionID);
}

void AbortRequest::print() const
{
    cout << "[X,";
    cout << getRequestDay();
    cout << ",M" << getID() << "]";
    
}