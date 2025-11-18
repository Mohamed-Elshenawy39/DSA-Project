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
    ReqType getRequestType() { return RequestType; }

    virtual void Operate(MarsStation* station) = 0;
};

