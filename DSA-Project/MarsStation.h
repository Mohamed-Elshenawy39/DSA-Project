#pragma once
#include <string>
#include "LinkedQueue.h"
#include "ArrayStack.h"
#include "priQueue.h"
#include "Requests.h"
#include "Missions.h"
#include "Rovers.h"
#include "RNMQueue.h" 
#include "OMpriQueue.h" 
#include "DEFS.h"
#include "AbortedRequest.h"
#include "NewRequest.h"


class UI;
class MarsStation
{
private:
    int currentDay;
    UI* pUI; // Pointer to the UI object
    int maxMissionsBeforeCheckup; // The 'M' value

    // --- Lists of Pointers to Objects ---

    // 1. Pending Requests (Queue because input file is pre-sorted)
    LinkedQueue<Requests*> pendingRequests;

    // 2. Waiting (Ready) MissionsS
    LinkedQueue<Missions*> readyPolarMissions;
    ReadyNormalMissionsQueue readyNormalMissions;
    LinkedQueue<Missions*> readyDiggingMissions;

    // 3. Available Rovers
    LinkedQueue<Rovers*> availablePolarRovers;
    LinkedQueue<Rovers*> availableNormalRovers;
    LinkedQueue<Rovers*> availableDiggingRovers;

    // 4. In-Progress Missions (Per your final design)
    OutMissionsPriQueue outMissions;            // MODIFIED: Use derived class
    priQueue<Missions*> execMissions;   // Priority: Execution End Day
    priQueue<Missions*> backMissions;   // Priority: Final Completion Day

    // 5. In-Progress Rovers
    priQueue<Rovers*> inCheckupRovers;  // Priority: Checkup End Day

    // 6. Final Logs
    ArrayStack<Missions*> completedMissions; // Stack for descending order output
    LinkedQueue<Missions*> abortedMissions;

    // --- Private Helper Functions ---
    void loadFromFile(const string& filename);
    void processPendingRequests();
    void processCompletions();
    void assignMissions();
    void generateOutputFile(const std::string& filename);

public:
    MarsStation();
    ~MarsStation();

    void runSimulation();

    // Public function to be called by Request::Operate
    void addMission(Missions* newMission);

    // NEW: Public function to be called by AbortRequest::Operate
    void abortMission(int missionID);

    // --- Getters for UI Printing ---
     LinkedQueue<Missions*>& getReadyPolarMissions() const;
     ReadyNormalMissionsQueue& getReadyNormalMissions() const;
     LinkedQueue<Missions*>& getReadyDiggingMissions() const;

     OutMissionsPriQueue& getOutMissions() const; 
     priQueue<Missions*>& getExecMissions() const;
     priQueue<Missions*>& getBackMissions() const;

     LinkedQueue<Rovers*>& getAvailablePolarRovers() const;
     LinkedQueue<Rovers*>& getAvailableNormalRovers() const;
     LinkedQueue<Rovers*>& getAvailableDiggingRovers() const;

     priQueue<Rovers*>& getInCheckupRovers() const;
     ArrayStack<Missions*>& getCompletedMissions() const;
     LinkedQueue<Missions*>& getAbortedMissions() const;
};

