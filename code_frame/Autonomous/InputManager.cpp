#pragma once
#include "GCS.cpp"
#include "CameraInput.cpp"
#include "LidarInput.cpp"
#include "StateArbiter.cpp"

// 센서(카메라, LiDAR, GPS) 입력값 저장 및 처리
class InputManager
{
	// Camera
	CameraInput cameraInput;

	// LiDAR
	LidarInput lidarInput;

	// GPS
	GCS gcs;  // GPS상 차량 좌표
	Position position;  // 차량 XY 좌표

	InputManager()
	{
		// Initialize
	}

	// 센서 입력값 업데이트
	void updateRawInputs()
	{
		// TODO: updateRawInputs
	}

	// 카메라가 신호등을 인식했는지 확인
	bool hasDetectedTrafficLights()
	{
		// TODO: hasDetectedTrafficLights
		return false;
	}

	// LiDAR가 장애물을 인식했는지 확인
	bool hasDetectedObstacle()
	{
		// TODO: hasDetectedObstacle
		return false;
	}

public:

	// Singleton
	static InputManager* GetInstance()
	{
		static InputManager* instance = new InputManager();
		return instance;
	}

	// 마지막으로 확인된 GPS상 현재 좌표
	GCS GetCoord() const
	{
		return gcs;
	}
};
