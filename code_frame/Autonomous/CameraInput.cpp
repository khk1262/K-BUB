#pragma once
#include <string>
#include "Position.cpp"

using namespace std;

struct CameraInput
{
public:

	string className;  // 관측된 클래스명
	float probability;  // 클래스 정확도 확률
	Position LeftDown;  // 바운딩 좌하단 좌표
	Position RightUp;  // 바운딩 우상단 좌표
};
