#include "NewRequest.h"
#include "MarsStation.h"

void NewRequest::Operate(MarsStation* station)
{
    Missions* pMission = new Missions(
        this->missionID,
        this->missionType,
        this->requestDay,
        this->targetLocation,
        this->missionDuration,
        this->significance
    );
    station->addMission(pMission);
}

void NewRequest::print() const
{
    cout << "[R,";
    if (getType() == MISSION_POLAR)
        cout << "P,";
    else if (getType() == MISSION_NORMAL)
        cout << "N,";
    else if (getType() == MISSION_DIGGING)
        cout << "D,";
    cout << getRequestDay();
    cout << ",M" << getID() << "]";
}