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

public:
    Requests(int Rd, int id) : requestDay(Rd), missionID(id) {}

    int getEventDay() const { return requestDay; }
    int getMissionID() const { return missionID; }

    virtual void Operate(MarsStation* station) = 0;
};
ostream& operator<<(ostream& os, const Requests* request);

