#include "UI.h"

void UI::printDay(int day,  MarsStation* station) 
{
	cout << "\n Current Day: " << day << "" << endl;
	//cout << "===============" << "Requests List" << "===============" << endl;
	////cout <<  << "Requests List" << "===============" << endl;
	//MarsStation* marsStation = station;
	///*cout << station->getpendingRequests().getCount() << " Pending Requests" << endl;
	//for (int i = 0; i < 10; i++) {

	//	Requests* req;

	//	marsStation->getpendingRequests().peek(req);
	//	if (req != nullptr) {
	//		cout << req << " ";
	//		marsStation->getpendingRequests().dequeue(req);
	//	}
	//	else {
	//		break;
	//	}
	//}
	//cout << endl;*/
	cout << "===============" << "Ready List" << "===============" << endl;
	int ReadyPolarMissions = station->getReadyPolarMissions().getCount();
	int ReadyDiggingMissions = station->getReadyDiggingMissions().getCount();
	int ReadyNormalMissions = station->getReadyNormalMissions().getCount();
	int totalReadyMissions = ReadyDiggingMissions + ReadyNormalMissions + ReadyPolarMissions;
	cout << totalReadyMissions << "Missions :";
	if (ReadyNormalMissions > 0)
	{
		cout << "NMs[";
		station->getReadyNormalMissions().PrintQueue();
		cout << "]";
	}

	if (ReadyDiggingMissions > 0) 
	{
		cout << " DMs[";
		station->getReadyDiggingMissions().PrintQueue();
		cout << "]";
	}

	if (ReadyPolarMissions > 0)
	{
		cout << " PMs[";
		station->getReadyPolarMissions().PrintQueue();
		cout << "]";
	}
	cout << endl;

	cout << "===============" << "Avilable Rovers List" << "===============" << endl;
	int AvailablePolarRovers = station->getAvailablePolarRovers().getCount();
	int AvailableDiggingRovers = station->getAvailableDiggingRovers().getCount();
	int AvailableNormalRovers = station->getAvailableNormalRovers().getCount();
	int totalAvailableRovers = AvailableDiggingRovers + AvailableNormalRovers + AvailablePolarRovers;
	cout << totalAvailableRovers << " Rovers :";
	if (AvailableNormalRovers > 0)
	{
		cout << " NR[";
		station->getAvailableNormalRovers().PrintQueue();
		cout << "]";
	}
	if (AvailableDiggingRovers > 0)
	{
		cout << " DR[";
		station->getAvailableDiggingRovers().PrintQueue();
		cout << "]";
	}
	if (AvailablePolarRovers > 0)
	{
		cout << " PR[";
		station->getAvailablePolarRovers().PrintQueue();
		cout << "]";
	}
	cout << endl;
	cout << "===============" << "Out List" << "===============" << endl;
	int OutMissions = station->getOutMissions().getCount();
	cout << OutMissions << "Missions/Rovers :";
	if (OutMissions > 0){
	station->getOutMissions().printQueue();
	}
	cout << endl;

	cout << "===============" << "Execution List" << "===============" << endl;
	int ExecMissions = station->getExecMissions().getCount();
	
		cout << ExecMissions << "Missions/Rovers :";
	if (ExecMissions > 0) {
		station->getExecMissions().printQueue();
	}
	cout << endl;

	cout << "===============" << "Back List" << "===============" << endl;
	int BackMissions = station->getBackMissions().getCount();
	cout << BackMissions << "Missions/Rovers :";
	if (BackMissions > 0) 
	{
		station->getBackMissions().printQueue();
	}		
	cout << endl;

	cout << "===============" << "Aborted List" << "===============" << endl;

	int AbortedMissions = station->getAbortedMissions().getCount();
	cout << AbortedMissions << "Missions :";
	if (AbortedMissions >0)
	{
		station->getAbortedMissions().PrintQueue();
	}
	cout << endl;

	cout << "===============" << "Check up List" << "===============" << endl;
	int InCheckupRovers = station->getInCheckupRovers().getCount();
	cout << InCheckupRovers << " Rovers :";
	if (InCheckupRovers > 0)
	station->getInCheckupRovers().printQueue();

	cout << endl;

	cout << "===============" << "Done List" << "===============" << endl;
	cout << station->getCompletedMissions().getCount() << " Missions :";
	station->getCompletedMissions().PrintStack();
	cout << endl;


	//cout << "Ready Polar Missions: " << (station->getReadyPolarMissions()).getCount();
	//// (Assuming station has a method to get ready polar missions)
	//// You would implement the actual printing logic here
	//cout << endl;
	//cout << "Ready Normal Missions: " << (station->getReadyNormalMissions()).getCount();
	//// (Assuming station has a method to get ready normal missions)
	//cout << "Ready Digging Missions: " << (station->getReadyDiggingMissions()).getCount();
	//cout << endl;
	//cout << "Out Missions: " << (station->getOutMissions()).getCount();
	//cout << endl;
	//cout << "In-Execution Missions: " << (station->getExecMissions()).getCount();
	//cout << endl;
	//cout << "Back Missions: " << (station->getBackMissions()).getCount();
	//cout << endl << endl;
	//// Similarly, print other relevant information like Available Rovers, In-Progress Missions, etc.
}
