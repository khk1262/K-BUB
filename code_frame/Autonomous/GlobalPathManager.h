#pragma once
#include <queue>
#include "Coordination.h"
#include "InputManager.h"

// 글로벌 경로 저장 및 경로 생성
class GlobalPathManager
{
	// 도착 판정 허용 거리 오차
	const float  ARRIVAL_TOLERANCE = 0.0001f;

	// 글로벌 경로 좌표(웨이포인트) 큐. front가 다음으로 이동할 좌표
	queue<Coordination> waypoints;

	GlobalPathManager() {};
	GlobalPathManager(const GlobalPathManager& other) {};
	~GlobalPathManager() {};

public:

	// Singleton
	static GlobalPathManager* GetInstance() {
		static GlobalPathManager* instance = new GlobalPathManager();
		return instance;
	}

	// 다음으로 이동할 웨이포인트
	Coordination GetNextWaypoint() {
		return waypoints.front();
	}

	// 웨이포인트에 도달했는지 확인하고 처리
	// pos: 현재 좌표
	bool CheckArrival(Coordination& pos) {
		if (pos.GetDistance(waypoints.front()) < ARRIVAL_TOLERANCE) {
			ArriveWaypoint();
			return true;
		}
		return false;
	}

	// 웨이포인트 도달
	void ArriveWaypoint() {
		if (waypoints.size() == 0) return;
		waypoints.pop();
	}
};
