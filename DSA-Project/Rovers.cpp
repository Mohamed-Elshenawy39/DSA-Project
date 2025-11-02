#include "Rovers.h"

    Rovers::Rovers(int id, RoverType type, int speed, int cdu, int missionsBeforeCheckup)
    : id(id), type(type), speed(speed), checkupDuration(cdu),
		maxMissionsBeforeCheckup(missionsBeforeCheckup), missionsCompleted(0), isDamaged(false)
	{
	}

    int Rovers::getID() const
    {
        return id;
    }

    RoverType Rovers::getType() const { return type; }
    int Rovers::getSpeed() const { return speed; }
    int Rovers::getCheckupDuration() const { return checkupDuration; }

    void Rovers::incrementMissionsCompleted() {
        missionsCompleted++;
    }

    bool Rovers::needsCheckup() const {
        if (maxMissionsBeforeCheckup == 0) return false;
        return missionsCompleted >= maxMissionsBeforeCheckup;
    }

    void Rovers::resetMissionsCompleted() {
        missionsCompleted = 0;
    }


    // --- BONUS ---

    // A function to handle the rover failing (for Rescue Mission bonus)
    void Rovers::setDamagedStatus(bool status) {
        isDamaged = status;
    }

    bool Rovers::isRoverDamaged() const {
        return isDamaged;
    }