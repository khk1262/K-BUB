#pragma once
#include <vector>
#include "Position.cpp"

using namespace std;

struct LidarInput
{
public:

	vector<pair<Position, Position>> obstacles;  // 장애물들의 최소/최대 XY 좌표 배열
};
