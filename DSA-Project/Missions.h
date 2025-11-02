#pragma once
#include "DEFS.h"
#include "Rovers.h"
using namespace std;
#include <iostream>
class Missions
{
private:
    int id;
    MissionType type;
    int formulationDay; // Rday (Ready Day)
    int targetLocation;
    int missionDuration; // MDUR (Mission Duration at target)
    int significance;
    
    // Calculated values
    int waitingDays;     // Wdays
    int Tdays;           // Tdays (Total Turnaround Days)
    int completionDay;   // Fday (Finish Day)
    int oneWayTravelTime; // Stores the calculated travel time

    Rovers* assignedRover; // Pointer to the rover executing this mission

public:
    Missions(int id, MissionType type, int fd, int tl, int md, int sig);

    // --- Getters ---
    int getID() const;
    MissionType getType() const;
    int getFormulationDay() const;
    int getTargetLocation() const;
    int getMissionDuration() const;
    int getCompletionDay() const;
    int getWaitingDays() const;
    int getTdays() const;
    int getOneWayTravelTime() const;

    Rovers* getAssignedRover() const;

    // --- Setters ---
    void assignRover(Rovers* r);
    void setCompletionDay(int day);
    void setWaitingDays(int wd);
    void setTdays(int td);
    void setOneWayTravelTime(int tt);
    


};
// Operator Overload for Printing 
inline ostream& operator<<(ostream& os, const Missions* mission)
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

    os << "]\n";
    return os;
}
