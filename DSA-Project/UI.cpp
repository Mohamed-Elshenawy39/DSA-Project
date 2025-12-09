#include "UI.h"

void UI::printDay(int day,  MarsStation* station)
{
	cout << "\n Current Day: " << day << "" << endl;
	cout << "================== Requests List ===================" << endl;
	LinkedQueue<Requests*> tempQueue = station->getpendingRequests();

	int pendingRequests = tempQueue.getCount();
	cout << pendingRequests << " Requests :";

	int count = 0;
	Requests* req;
	while (count < 10 && tempQueue.dequeue(req)) // dequeue returns false if empty
	{
		req->print();
		if (count < pendingRequests - 1 && count < 9) // if not the last request and less than 10
			cout << ", ";
		count++;
	}

cout << endl;
cout << "==================== Ready List ====================" << endl;
	int ReadyPolarMissions = station->getReadyPolarMissions().getCount();
	int ReadyDiggingMissions = station->getReadyDiggingMissions().getCount();
	int ReadyNormalMissions = station->getReadyNormalMissions().getCount();
	int totalReadyMissions = ReadyDiggingMissions + ReadyNormalMissions + ReadyPolarMissions;
	cout << totalReadyMissions << " Missions :";
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

	cout << "============== Available Rovers List ===============" << endl;
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
	cout << "===================== Out List =====================" << endl;
	int OutMissions = station->getOutMissions().getCount();
	cout << OutMissions << " Missions/Rovers :";
	if (OutMissions > 0){
	station->getOutMissions().printQueue();
	}
	cout << endl;

	cout << "================== Execution List ==================" << endl;
	int ExecMissions = station->getExecMissions().getCount();
	
	cout << ExecMissions << " Missions/Rovers :";
	if (ExecMissions > 0) {
		station->getExecMissions().printQueue();
	}
	cout << endl;

	cout << "==================== Back List =====================" << endl;
	int BackMissions = station->getBackMissions().getCount();
	cout << BackMissions << " Missions/Rovers :";
	if (BackMissions > 0) 
	{
		station->getBackMissions().printQueue();
	}		
	cout << endl;

	cout << "=================== Aborted List ===================" << endl;
	int AbortedMissions = station->getAbortedMissions().getCount();
	cout << AbortedMissions << " Missions :";
	if (AbortedMissions >0)
	{
		cout << "[";
		station->getAbortedMissions().PrintQueue();
		cout << "]";
	}
	cout << endl;

	cout << "================== Check up List ===================" << endl;
	int InCheckupRovers = station->getInCheckupRovers().getCount();
	cout << InCheckupRovers << " Rovers :";
	if (InCheckupRovers > 0) {
		cout << "[";
		station->getInCheckupRovers().printQueue();
		cout << "]";
	}

	cout << endl;

	cout << "==================== Done List =====================" << endl;
	cout << station->getCompletedMissions().getCount() << " Missions :";
	if (station->getCompletedMissions().getCount() > 0)
	station->getCompletedMissions().PrintStack();
	cout << endl;
}

void UI::printSilent() const
{
	cout << "Silent Mode" << endl;
	cout << "Simulation Starts..." << endl;
}

void UI::printEndMessage() const
{
	cout << "Simulation Ends..." << endl;
}

int UI::getMode() const
{
	int mode;
	cout << "Select Program Mode:" << endl;
	cout << "1. Interactive Mode" << endl;
	cout << "2. Silent Mode" << endl;
	cout << "Enter choice: ";
	cin >> mode;

	// Input validation (Optional but recommended)
	while (mode != 1 && mode != 2)
	{
		cout << "Invalid choice. Please enter 1 or 2: ";
		cin >> mode;
	}

	cin.ignore(numeric_limits<streamsize>::max(), '\n');

	return mode;
}

// In DSA-Project/UI.cpp

string UI::getInFile() const
{
	string filename;
	cout << "Enter Input File Name (e.g., Input.txt): ";
	cin >> filename;
	return filename;
}

string UI::getOutFile() const
{
	string filename;
	cout << "Enter Output File Name (e.g., Output.txt): ";
	cin >> filename;
	return filename;
}

void UI::waitForEnter() const
{
	cout << "Press Enter to continue to next day...";

	if (cin.peek() == '\n') {
		cin.ignore();
	}

	cin.get();
}