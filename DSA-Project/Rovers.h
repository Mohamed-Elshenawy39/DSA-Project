#pragma once
#include "DEFS.h"
#include <iostream>
using namespace std;
// Defines the Rover object.
class Rovers {
private:
    int id;
    RoverType type;
    int speed; // in km/hour
    int checkupDuration; // in days

    // 'M' value from the input file
    int maxMissionsBeforeCheckup;
    int missionsCompleted; // Counter since last checkup

public:
    // Constructor now takes 'M' (maxMissions)
    Rovers(int id, RoverType type, int speed, int cdu, int maxMissions);

    // --- Getters ---
    int getID() const;
    RoverType getType() const;
    int getSpeed() const;
    int getCheckupDuration() const;

    // --- Checkup Logic ---
    void incrementMissionsCompleted();

    // Check if the rover needs maintenance
    bool needsCheckup() const;

    // Reset counter after checkup is assigned
    void resetMissionsCompleted();


};
// Operator Overload
inline ostream& operator<<(ostream& os, const Rovers* rover)
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
        << "]\n";
    return os;
}
