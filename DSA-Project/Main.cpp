#include <iostream>
#include "Rovers.h"
#include "Missions.h"
#include "MarsStation.h"

using namespace std;

int main() {
	MarsStation *station = new MarsStation();
	station->runSimulation();
	LinkedQueue<Rovers*> Test = station->getAvailableDiggingRovers();
	Test.PrintQueue();
	LinkedQueue<Missions*> Test2 = station->getReadyDiggingMissions();
	Test2.PrintQueue();
	LinkedQueue<Missions*> Test3 = station->getReadyPolarMissions();
	Test3.PrintQueue();
	LinkedQueue<Missions*> Test4 = station->getReadyNormalMissions();
	Test4.PrintQueue();

	priQueue<Missions*> Test5 = station->getBackMissions();
	Test5.printQueue();
	return 0;
}