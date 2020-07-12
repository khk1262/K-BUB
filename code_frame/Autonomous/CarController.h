#pragma once
#include "Coordination.h"

// 운행 모드. 우선순위 오름차순.
enum DRIVE_MODE {
	STOP,  // 정차
	DRIVE,  // 차선을 따라 주행 (카메라)
	SIGNAL,  // 교통 신호 처리 (카메라)
	AVOID,  // 도로 장애물 감지 (LiDAR)
};

// 차량에 제어 신호 전달
class CarController
{
	float wheelSpeed;  // 바퀴 속도
	float wheelAngle;  // 바퀴 각도 (0 전방 / - 좌현 / + 우현)

	CarController() {
		// Initialize
		wheelSpeed = 0.f;
		wheelAngle = 0.f;
	};
	CarController(const CarController& other) {};
	~CarController() {};

public:

	// Singleton
	static CarController* GetInstance() {
		static CarController* instance = new CarController();
		return instance;
	}

	float GetWheelSpeed() const {
		return wheelSpeed;
	}

	float GetWheelAngle() const {
		return wheelAngle;
	}
};
