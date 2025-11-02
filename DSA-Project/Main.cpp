#include <iostream>
#include "Rovers.h"
#include "Missions.h"

using namespace std;

int main() {
	MarsStation *station = new MarsStation();
	station->runSimulation();
	LinkedQueue<Rovers*> Test = station->getAvailableDiggingRovers();
	Test.PrintQueue();
	return 0;


	//test
}