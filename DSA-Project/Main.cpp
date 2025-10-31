#include <iostream>
#include "MarsStation.h"
int main() {
	MarsStation *station = new MarsStation();
	station->runSimulation();
	LinkedQueue<Rovers*> Test = station->getAvailableDiggingRovers();
	Test.PrintQueue();
	return 0;
}