#include <iostream>
#include "Rovers.h"
#include "Missions.h"

using namespace std;

int main() {
    cout << " Testing Rovers Class " << endl;

    // rover objects
    Rovers rover1(1, ROVER_NORMAL, 60, 7, 3);
    Rovers rover2(2, ROVER_POLAR, 40, 8, 3);
    Rovers rover3(3, ROVER_DIGGING, 20, 9, 3);

    // Print by overload 
    cout << &rover1 << endl;
    cout << &rover2 << endl;
    cout << &rover3 << endl;

    // Test checkup logic
    rover1.incrementMissionsCompleted();
    rover1.incrementMissionsCompleted();
    rover1.incrementMissionsCompleted();

    cout << "Checking if rover1 needs checkup..." << endl;
    if (rover1.needsCheckup()) {
        cout << "Rover1 needs checkup." << endl;
    }
    else {
        cout << "Rover1 does not need checkup." << endl;
    }

    rover1.resetMissionsCompleted();

    cout << "After reset..." << endl;
    if (rover1.needsCheckup()) {
        cout << "Rover1 needs checkup." << endl;
    }
    else {
        cout << "Rover1 does not need checkup." << endl;
    }

    cout << "\n Testing Missions Class " << endl;

    // Create missions
    Missions m1(101, MISSION_NORMAL, 2, 5000, 10, 5);
    Missions m2(102, MISSION_POLAR, 3, 8000, 15, 8);
    Missions m3(103, MISSION_DIGGING, 5, 2000, 7, 3);

    // Assign rovers
    m1.assignRover(&rover1);
    m2.assignRover(&rover2);

    // Print using overloaded <<
    cout << &m1 << endl;
    cout << &m2 << endl;
    cout << &m3 << endl;

    return 0;
}
