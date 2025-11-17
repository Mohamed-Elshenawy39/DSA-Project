#include "UI.h"

void UI::printDay(int day,  MarsStation* station) 
{
	cout << "\n----- Day " << day << " -----" << endl;
	// Example: Print Ready Missions
	cout << "Ready Polar Missions: " << (station->getReadyPolarMissions()).getCount();
	// (Assuming station has a method to get ready polar missions)
	// You would implement the actual printing logic here
	cout << endl;
	cout << "Ready Normal Missions: " << (station->getReadyNormalMissions()).getCount();
	// (Assuming station has a method to get ready normal missions)
	cout << endl;
	cout << "Ready Digging Missions: " << (station->getReadyDiggingMissions()).getCount();
	// (Assuming station has a method to get ready digging missions)
	cout << endl << endl;
	// Similarly, print other relevant information like Available Rovers, In-Progress Missions, etc.
}
