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
ostream& operator<<(ostream& os, const Rovers* rover);
