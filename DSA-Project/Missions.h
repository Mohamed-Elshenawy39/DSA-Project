#pragma once
#include "DEFS.h"
#include "Rovers.h"
using namespace std;
#include <cstdlib>
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
    Rovers* Assigned_extra_for_complex[2];
    int extraRoverCount; // Keeps track (0 for normal, 2 for complex)
	int roversNeeded;

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
    Rovers* getExtraRover(int index) const;
    void addExtraRover(Rovers* r);
    int getExtraRoverCount() const;
    int getRoversNeeded() const;

    // --- Setters ---
    void assignRover(Rovers* r);
    void setCompletionDay(int day);
    void setWaitingDays(int wd);
    void setTdays(int td);
    void setOneWayTravelTime(int tt);
    // Missions.h
    void setMissionDuration(int d);


};
// Operator Overload for Printing 
inline ostream& operator<<(ostream& os, const Missions* mission)
{
    if (mission->getAssignedRover() == nullptr) {
        os << mission->getID();
    }
    else {
        os << "[";
        os << mission->getID();
        os << "/";

        os << mission->getAssignedRover()->getID();

        // Print Extras if they exist
        for (int i = 0; i < mission->getExtraRoverCount(); i++) {
            os << "," << mission->getExtraRover(i)->getID();
        }

        os << "," << mission->getMissionDuration();
        os << "Days] ";
    }
    return os;
}