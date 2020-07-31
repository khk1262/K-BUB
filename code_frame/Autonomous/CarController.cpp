#pragma once
#include "GCS.cpp"

// 차량에 제어 신호 전달
class CarController
{
	float wheelSpeed;  // 바퀴 속도
	float wheelAngle;  // 바퀴 각도 (0 전방 / - 좌현 / + 우현) 

	CarController()
	{
		// Initialize
		wheelSpeed = 0.f;
		wheelAngle = 0.f;
	};
	CarController(const CarController& other) {};
	~CarController() {};

public:

	// Singleton
	static CarController* Get()
	{
		static CarController* instance = new CarController();
		return instance;
	}

	float GetWheelSpeed() const
	{
		return wheelSpeed;
	}

	float GetWheelAngle() const
	{
		return wheelAngle;
	}
};
