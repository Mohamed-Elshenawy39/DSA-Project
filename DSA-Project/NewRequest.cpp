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
