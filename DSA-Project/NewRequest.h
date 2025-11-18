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
    NewRequest(int Rd, int id, MissionType mt, int tl, int md, ReqType type)
        : Requests(Rd, id, type), missionType(mt), targetLocation(tl), missionDuration(md), significance(0) {
    }

	MissionType getType() const { return missionType; }

    // Implementation of the pure virtual function from the base class.
    virtual void Operate(MarsStation* station);
};

inline ostream& operator<<(ostream& os, const NewRequest* request) {
    {
        cout << "[ R";
		if (request->getType() == MISSION_POLAR)
            cout << "P";
        else if (request->getType() == MISSION_NORMAL)
            cout << "N";
        else if (request->getType() == MISSION_DIGGING)
            cout << "D";
        cout << request->getRequestDay();
        cout << "M" << request->getID() << " ] ,";

        return os;
    }
}

