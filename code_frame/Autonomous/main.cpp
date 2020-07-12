#include <iostream>
#include "CarController.h"
#include "GlobalPathManager.h"
#include "InputManager.h"

using namespace std;

int main() {
	cout << "--- Autonomous ---\n";
	cout << "wheelSpeed: " << CarController::GetInstance()->GetWheelSpeed() << "\n";

	return 0;
}
