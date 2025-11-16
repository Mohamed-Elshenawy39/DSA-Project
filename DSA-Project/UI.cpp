#include "UI.h"

void UI::printDay(int day, const MarsStation* station) const
{
	cout << "----- Day " << day << " -----" << endl;
	// Example: Print Ready Missions
	cout << "Ready Polar Missions: ";
	// (Assuming station has a method to get ready polar missions)
	// You would implement the actual printing logic here
	cout << endl;
	cout << "Ready Normal Missions: ";
	// (Assuming station has a method to get ready normal missions)
	cout << endl;
	cout << "Ready Digging Missions: ";
	// (Assuming station has a method to get ready digging missions)
	cout << endl;
	// Similarly, print other relevant information like Available Rovers, In-Progress Missions, etc.
}
