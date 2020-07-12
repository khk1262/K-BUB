#pragma once
#include "CarController.h"

// 운행 모드 결정
static class StateArbiter
{
	static DRIVE_MODE driveMode;

public:

	DRIVE_MODE GetDriveMode() const {
		return driveMode;
	}

	// 운행 모드 결정
	static DRIVE_MODE DetermineDriveMode(bool bSignal, bool bObstacle) {
		// UNDONE: 신호등과 장애물이 동시에 인식되는 경우
		DRIVE_MODE driveMode = DRIVE_MODE::DRIVE;
		if (bObstacle) {  // 장애물 인식
			driveMode = DRIVE_MODE::AVOID;
		}
		else if (bSignal) {  // 신호등 인식
			driveMode = DRIVE_MODE::SIGNAL;
		}
		return driveMode;
	}
};
