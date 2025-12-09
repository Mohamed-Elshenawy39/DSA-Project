#include "AbortedRequest.h"
#include "MarsStation.h"
void AbortRequest::Operate(MarsStation* station)
{
    station->abortMission(this->missionID);
}

void AbortRequest::print() const
{
    cout << "[X,";
    cout << getRequestDay();
    cout << ",M" << getID() << "]";
    
}