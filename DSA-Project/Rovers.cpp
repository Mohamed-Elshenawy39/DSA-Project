#include "Rovers.h"
#include<iostream>
using namespace std;

// Constructor 
Rovers::Rovers(int _id, RoverType _type, int _speed, int _cdu, int _maxMissions)
{
    id =_id;
    type = _type;
    speed = _speed;
    checkupDuration = _cdu;
    maxMissionsBeforeCheckup = _maxMissions;
    missionsCompleted = 0;
}

// Getters 
int Rovers::getID() const
{
    return id;
}

RoverType Rovers::getType() const
{
    return type;
}

int Rovers::getSpeed() const
{
    return speed;
}

int Rovers::getCheckupDuration() const
{
    return checkupDuration;
}

// Checkup Logic
void Rovers::incrementMissionsCompleted()
{
    missionsCompleted++;
}

bool Rovers::needsCheckup() const
{
    return missionsCompleted >= maxMissionsBeforeCheckup;
}

void Rovers::resetMissionsCompleted()
{
    missionsCompleted = 0;
}

// Operator Overload
ostream& operator<<(ostream& os, const Rovers* rover)
{
    string typeStr;

    if (rover->getType() == ROVER_NORMAL)
        typeStr = "Normal";
    else if (rover->getType() == ROVER_POLAR)
        typeStr = "Polar";
    else
        typeStr = "Digging";

    os << "[Rover " << rover->getID()
        << " | Type: " << typeStr
        << " | Speed: " << rover->getSpeed()
        << " | Checkup Duration: " << rover->getCheckupDuration()
        << "]";
    return os;
}
