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

    // --- BONUS ---
    // A state to track if the rover is damaged (for Rescue Mission bonus)
    bool isDamaged;


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


    // --- BONUS ---

    // A function to handle the rover failing (for Rescue Mission bonus)
    void setDamagedStatus(bool status);

    bool isRoverDamaged() const;
};
ostream& operator<<(ostream& os, const Rovers& rover);
