#pragma once
#include "priQueue.h"
#include "Missions.h"

class OutMissionsPriQueue : public priQueue<Missions*> {
public:
    OutMissionsPriQueue() : priQueue<Missions*>() {

    }

    Missions* AbortMission(int missionID) {

    }
};