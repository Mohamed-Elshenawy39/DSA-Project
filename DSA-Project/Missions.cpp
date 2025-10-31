#include "Missions.h"
#include <iostream>
using namespace std;

// Constructor
Missions::Missions(int _id, MissionType _type, int _fd, int _tl, int _md, int _sig)
{
    id = _id;
    type = _type;
    formulationDay = _fd;
    targetLocation = _tl;
    missionDuration = _md;
    significance = _sig;

    // Initialize calculated values
    waitingDays = 0;
    Tdays = 0;
    completionDay = 0;
    oneWayTravelTime = 0;
    assignedRover = nullptr;
}

// Getters 
int Missions::getID() const { return id; }
MissionType Missions::getType() const { return type; }
int Missions::getFormulationDay() const { return formulationDay; }
int Missions::getTargetLocation() const { return targetLocation; }
int Missions::getMissionDuration() const { return missionDuration; }
int Missions::getCompletionDay() const { return completionDay; }
int Missions::getWaitingDays() const { return waitingDays; }
int Missions::getTdays() const { return Tdays; }
int Missions::getOneWayTravelTime() const { return oneWayTravelTime; }
Rovers* Missions::getAssignedRover() const { return assignedRover; }

//  Setters 
void Missions::assignRover(Rovers* r) { assignedRover = r; }
void Missions::setCompletionDay(int day) { completionDay = day; }
void Missions::setWaitingDays(int wd) { waitingDays = wd; }
void Missions::setTdays(int td) { Tdays = td; }
void Missions::setOneWayTravelTime(int tt) { oneWayTravelTime = tt; }

// Operator Overload for Printing 
ostream& operator<<(ostream& os, const Missions* mission)
{
    string typeStr;

    // Determine mission type 
    if (mission->getType() == MISSION_POLAR)
        typeStr = "Polar";
    else if (mission->getType() == MISSION_NORMAL)
        typeStr = "Normal";
    else if (mission->getType() == MISSION_DIGGING)
        typeStr = "Digging";
    else
        typeStr = "Unknown";

    os << "[Mission ID: " << mission->getID()
        << ", Type: " << typeStr
        << ", Ready Day: " << mission->getFormulationDay()
        << ", Target: " << mission->getTargetLocation() << " km"
        << ", Duration: " << mission->getMissionDuration() << " days";

    if (mission->getAssignedRover() != nullptr)
        os << ", Assigned Rover ID: " << mission->getAssignedRover()->getID();

    os << "]";
    return os;
}
