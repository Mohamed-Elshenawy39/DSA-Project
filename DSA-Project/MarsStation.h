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
#include <random>


class UI;
class MarsStation
{
private:
    int currentDay;
    UI* pUI; // Pointer to the UI object
    int maxMissionsBeforeCheckup; // The 'M' value
    double AutoAbortCount;

    // --- Lists of Pointers to Objects ---

    // 1. Pending Requests (Queue because input file is pre-sorted)
    LinkedQueue<Requests*> pendingRequests;

    // 2. Waiting (Ready) MissionsS
    LinkedQueue<Missions*> readyPolarMissions;
    ReadyNormalMissionsQueue readyNormalMissions;
    LinkedQueue<Missions*> readyDiggingMissions;
    LinkedQueue<Missions*> rescueMissions;
	LinkedQueue<Missions*> readyComplexMissions; 

    // 3. Available Rovers
    LinkedQueue<Rovers*> availablePolarRovers;
    LinkedQueue<Rovers*> availableNormalRovers;
    LinkedQueue<Rovers*> availableDiggingRovers;
    LinkedQueue<Rovers*> availableRescueRovers;

    // 4. In-Progress Missions (Per your final design)
    OutMissionsPriQueue outMissions;    // MODIFIED: Use derived class
    priQueue<Missions*> execMissions;   // Priority: Execution End Day
    priQueue<Missions*> backMissions;   // Priority: Final Completion Day

    // 5. In-Progress Rovers
    priQueue<Rovers*> inCheckupRovers;  // Priority: Checkup End Day

    // 6. Final Logs
    ArrayStack<Missions*> completedMissions; // Stack for descending order output
    LinkedQueue<Missions*> abortedMissions;

    // --- Private Helper Functions ---
    bool loadFromFile(const string& filename);
    void processPendingRequests();

    void assignMissions();
    void generateOutputFile(const std::string& filename);

public:
    MarsStation();
    ~MarsStation();


    void runSimulation();

    void AutoAbortPolarMissions();
    // Public function to be called by Request::Operate
    void addMission(Missions* newMission);

    // NEW: Public function to be called by AbortRequest::Operate
    void abortMission(int missionID);
    
    //COV 
    
	// NEW: Done mission pushed to completed missions
	void BackToCompletedMissions();

	//NEW: Check for mission failure and handle rescue
    void checkMissionFailure();

    //NEW: Add rover to checkup queue 
	void AddRoverToCheckup(Rovers* rover);

	//NEW: Add rover to its available queue
	void AddRoverToAvailable(Rovers* rover);
	
	//Add missions from exec to back
    void ExecToBack();

    //Add missions from out to exec
	void OutToExec();

	//move rover from checkup to available Logic needs to be rewritten in next phase
	void CheckupToAvailable();

    // Increment Current day
    void incrementDay(); 

    bool ISsimdone() const;

    int getCurrentDay();






    // --- Getters for UI Printing ---
     LinkedQueue<Missions*>& getReadyPolarMissions();
     ReadyNormalMissionsQueue& getReadyNormalMissions();
     LinkedQueue<Missions*>& getReadyDiggingMissions();
     LinkedQueue<Missions*>& getRescueMissions();
	 LinkedQueue<Missions*>& getReadyComplexMissions();

     OutMissionsPriQueue& getOutMissions(); 
     priQueue<Missions*>& getExecMissions();
     priQueue<Missions*>& getBackMissions();

     LinkedQueue<Rovers*>& getAvailablePolarRovers();
     LinkedQueue<Rovers*>& getAvailableNormalRovers();
     LinkedQueue<Rovers*>& getAvailableDiggingRovers();

     priQueue<Rovers*>& getInCheckupRovers();
     ArrayStack<Missions*>& getCompletedMissions();
     LinkedQueue<Missions*>& getAbortedMissions();
     LinkedQueue<Requests*> getpendingRequests();
     LinkedQueue<Rovers*>& getAvailableRescueRovers();

};

