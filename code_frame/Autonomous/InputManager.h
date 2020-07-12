#pragma once
#include "Coordination.h"
#include "StateArbiter.h"

// 센서 입력값 저장 및 처리
class InputManager
{
	// Camera
	// TODO: 카메라 입력값 정의

	// LiDAR
	// TODO: LiDAR 입력값 정의

	// GPS
	Coordination coord;  // GPS상 차량 좌표

	InputManager() {
		// Initialize
	}

	// 센서 입력값 업데이트
	void updateRawInputs() {
		// TODO: updateRawInputs
	}

	// 카메라가 신호등을 인식했는지 확인
	bool hasDetectedTrafficLights() {
		// TODO: hasDetectedTrafficLights
		return false;
	}

	// LiDAR가 장애물을 인식했는지 확인
	bool hasDetectedObstacle() {
		// TODO: hasDetectedObstacle
		return false;
	}

public:

	// Singleton
	static InputManager* GetInstance() {
		static InputManager* instance = new InputManager();
		return instance;
	}

	// 마지막으로 확인된 GPS상 현재 좌표
	Coordination GetCoord() const {
		return coord;
	}
};
