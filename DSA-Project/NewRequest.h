#pragma once
#include "Requests.h"
#include "DEFS.h"
class NewRequest : public Requests {
private:
    MissionType missionType;
    int targetLocation;
    int missionDuration;
    int significance;

public:
    NewRequest(int Rd, int id, MissionType mt, int tl, int md)
        : Requests(Rd, id), missionType(mt), targetLocation(tl), missionDuration(md), significance(0) {
    }

    // Implementation of the pure virtual function from the base class.
    virtual void Operate(MarsStation* station);
};