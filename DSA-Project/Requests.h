#pragma once
#include <iostream>
#include "Missions.h"
using namespace std;
class MarsStation; 
class Requests
{
protected:
    int requestDay;
    int missionID;
    ReqType RequestType;

public:
    Requests(int Rd, int id, ReqType Type) : requestDay(Rd), missionID(id), RequestType(Type) {}

    int getEventDay() const { return requestDay; }
    int getMissionID() const { return missionID; }
    int getRequestDay() const { return requestDay; }
    int getID() const { return missionID; }
    //ReqType getRequestType() { return RequestType; }
    ReqType getRequestType() const { return RequestType; }

    virtual void Operate(MarsStation* station) = 0;
    virtual void print() const = 0;

};


/*nline ostream& operator<<(ostream& os, const Requests* request) {
    if (request->getRequestType() == NEW_REQ)
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
        
    }
    else if (request->getRequestType() == ABORT_REQ)
    {
        
    }

    return os;
}*/