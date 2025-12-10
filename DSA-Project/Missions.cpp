#include "Missions.h"

Missions::Missions(int id, MissionType type, int fd, int tl, int md, int sig)
    : id(id), type(type), formulationDay(fd), targetLocation(tl),
    missionDuration(md), significance(sig), assignedRover(nullptr),
    waitingDays(0), Tdays(0), oneWayTravelTime(0), completionDay(-1)
{ 
    Assigned_extra_for_complex[0] = nullptr;
    Assigned_extra_for_complex[1] = nullptr;
    extraRoverCount = 0;
    if (type == MISSION_COMPLEX) {
        roversNeeded = (rand() % 3) + 1;
    }
    else {
        roversNeeded = 1;
    }
}
// --- Getters ---
int Missions:: getID() const { return id; }
MissionType Missions::getType() const { return type; }
int Missions:: getFormulationDay() const { return formulationDay; }
int Missions:: getTargetLocation() const { return targetLocation; }
int Missions:: getMissionDuration() const { return missionDuration; }
int Missions:: getCompletionDay() const { return completionDay; }
int Missions:: getWaitingDays() const { return waitingDays; }
int Missions:: getTdays() const { return Tdays; }
int Missions:: getOneWayTravelTime() const { return oneWayTravelTime; }
int Missions::getRoversNeeded() const { return roversNeeded; }
Rovers* Missions:: getAssignedRover() const { return assignedRover; }
Rovers* Missions:: getExtraRover(int index) const {
    if (index < 0 || index >= extraRoverCount) return nullptr;
    return Assigned_extra_for_complex[index];
}
int Missions::getExtraRoverCount() const { return extraRoverCount; }
// --- Setters ---
void Missions:: assignRover(Rovers* r) { assignedRover = r; }
void Missions:: setCompletionDay(int day) { completionDay = day; }
void Missions:: setWaitingDays(int wd) { waitingDays = wd; }
void Missions:: setTdays(int td) { Tdays = td; }
void Missions:: setOneWayTravelTime(int tt) { oneWayTravelTime = tt; }
void Missions:: addExtraRover(Rovers* r) {
    if (extraRoverCount < 2) {
        Assigned_extra_for_complex[extraRoverCount] = r;
        extraRoverCount++;
    }
}
// Missions.h
void Missions :: setMissionDuration(int d) { missionDuration = d; }
// --- Extra Rover Count Getter ---
// Functions
