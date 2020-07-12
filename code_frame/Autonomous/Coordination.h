#pragma once
#include <iostream>
#include <cmath>
#include <string>

using namespace std;

// 위도/경도 좌표
class Coordination
{
public:

	float Latitude;  // 위도
	float Longitude;  // 경도

	Coordination() {
		Latitude = 0.f;
		Longitude = 0.f;
	}
	Coordination(float _latitude, float _longitude)
		: Latitude(_latitude), Longitude(_longitude) { }

	// UNDONE: 거리 계산 정확도 개선
	static float GetDistance(Coordination& c1, Coordination& c2) {
		return sqrt(pow(c1.Latitude - c2.Latitude, 2) + pow(c1.Longitude - c2.Longitude, 2));
	}
	float GetDistance(Coordination& other) const {
		return sqrt(pow(Latitude - other.Latitude, 2) + pow(Longitude - other.Longitude, 2));
	}
};
