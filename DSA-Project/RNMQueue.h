#pragma once
#include "LinkedQueue.h"
#include "Missions.h"

class ReadyNormalMissionsQueue : public LinkedQueue<Missions*> {
public:
	ReadyNormalMissionsQueue() : LinkedQueue<Missions*>() {

	}

    Missions* AbortMission(int missionID) {
    }
};