#include <iostream>
#include "Rovers.h"
#include "Missions.h"
#include "MarsStation.h"

using namespace std;

int main() {
	MarsStation *station = new MarsStation();
	station->runSimulation();
	return 0;
}