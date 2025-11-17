#pragma once
#include <iostream>
#include <string>
#include "DEFS.h"
#include "priQueue.h"
#include "Missions.h"
#include "Rovers.h"
#include "MarsStation.h"
using namespace std;

// Forward declare MarsStation to avoid circular include
class MarsStation;

// The UI class is responsible for all console input and output.
class UI {
private:
    // Prints the specialized format for Priority Queues
    void printPQ_Missions(const MarsStation* station, priQueue< Missions* >& pq) const;
    void printPQ_Rovers(const MarsStation* station, priQueue< Rovers* >& pq) const;

public:
    // --- New functions to get filenames ---
    string getInFile() const;
    string getOutFile() const;

    int getMode() const;
    void waitForEnter() const;
    void printDay(int day,  MarsStation* station) ;
    void printSilent() const;
    void printEndMessage() const;
};
