#include <iostream>
#include <fstream>
#include "CarController.cpp"
#include "GlobalPathManager.cpp"
#include "InputManager.cpp"

using namespace std;

int main()
{
	cout << "--- Autonomous ---\n";
	cout << "wheelSpeed: " << CarController::Get()->GetWheelSpeed() << "\n";

	GlobalPathManager::Get()->ReadJson();

	return 0;
}
