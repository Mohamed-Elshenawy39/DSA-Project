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
    if (mission->getAssignedRover() == nullptr) {
        os << mission->getID();
    }
    if (mission->getAssignedRover() != nullptr) {
        os << "[";
        os << mission->getID();
        os << "/" << mission->getAssignedRover()->getID() << "";
        os << "," << mission->getMissionDuration();
        os << "Days] ";
    }
    return os;
}
