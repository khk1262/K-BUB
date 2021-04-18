#pragma once
#include <queue>
#include <fstream>
#include <iostream>
#include <fstream>
#include "json/json.h"
#include <time.h>
#include <Windows.h>

#pragma comment(lib , "lib_json.lib")
#pragma warning(disable: 4996) 

#include "Position.cpp"  // for 좌표계
#include "InputManager.cpp"  // for GPS
#include "Include/json/json.h"

using namespace std;

typedef numeric_limits<double> dbl;
typedef struct nodeGEO
{
	string ID;
	string Lane_type;
	double x;
	double y;
	double z;
};
typedef struct linkGEO
{
	string ID;
	string Lane_type;
	string length;
	string R_Link; //is Right line exist?
	string L_Link; //is Left line exist?
	string FromND;
	string ToND;
	vector<double> x;
	vector<double> y;
	vector<double> z;
};

// 글로벌 경로 저장 및 경로 생성
class GlobalPathManager
{
	// 도착 판정 허용 거리 오차
	const float  ARRIVAL_TOLERANCE = 0.0001f;
	//now GPS Data!
	double CurX;
	double CurY;
	int CurPathidx;

	//Calculate GPS Data, suggest you go to coord!
	double nextX;
	double nextY;

	//(For Destination) Path list, from link
	vector<double> pathX;
	vector<double> pathY;
	int CurLinkidx;
	int pathQueueSize;

	//JSON File
	vector<nodeGEO> nodeGeometry;
	vector<linkGEO> linkGeometry;

public:
	// 자신과 가장 가까운 노드 찾기
	// GPS가 고장났다가 복구되었을 때 or 초기 위치 설정 사용 or 목적지 설정하면 에이스타 할까말까?
	void findCurrentNode()
	{
		double x, y;
		x = y = 5;
		//x, y는 GPS좌표, node_coord는 머냐 그 nodeJSON.geojson
		for (int i = 0; i < nodeGeometry.size(); i++)
		{
			std::cout << "x = " << nodeGeometry[i].x << " y = " << nodeGeometry[i].y << std::endl;
		}
		//--------------------------------------------------
	
		double min = 9999;
		int idx = 0;
		for (int i = 0; i < nodeGeometry.size(); i++)
		{
			double temp = (nodeGeometry[i].x - x) * (nodeGeometry[i].x - x) + (nodeGeometry[i].y - y) * (nodeGeometry[i].y - y);
			if (temp < min)
			{
				min = temp;
				idx = i;
			}
		}
		CurLinkidx = idx;
		//if(curLinkNum+1<)최댓값보다 작으면
	}
	void findPath()
	{
		// cout<<"-----------PATH FIND START------------------"<<endl;
		getCurPathCOORD();
		if (isArrived())
		{
			cout << "ARRIVED" << endl;
			changeCulNode();
		}
		getCurDestiPath();
		getMoveDist();
		//cout<<"-----------PATH FIND END------------------"<<endl;
	}
	
	void compareNodePosition() {
	
	}
	
	// 목적 노드의 번호를 사용해서 현재 링크의 좌표를 쭉 얻어옴
	void getCurPathCOORD()
	{
		//if(도착하면~)
		int idx = CurLinkidx;
	
		for (int j = 0; j < linkGeometry[idx].x.size(); j++) {
			pathX = linkGeometry[idx].x;
		}
		for (int j = 0; j < linkGeometry[idx].y.size(); j++) {
			pathY = linkGeometry[idx].y;
		}
		pathQueueSize = linkGeometry[idx].x.size();
	}
	
	//노드 도착했나 안했나 확인
	bool isArrived()
	{
		double destiX = pathX[pathQueueSize - 1];
		double destiY = pathY[pathQueueSize - 1];
		double subX = abs(CurX - destiX);
		double subY = abs(CurY - destiY);
		//cout<<subX<<"   |||||    "<<subY<<endl;
		if (subX + subY > 0.00002)
		{
			//ROS_INFO("[GlobalPath] NO CHANGE");
			return false;
		}
		else
		{
			//ROS_INFO("[GlobalPath] PATH CHANGED!");
			return true;
		}
	}
	
	//노드에 도착했을 경우 다음 목적 노드를 바꿈
	bool changeCulNode()
	{
		CurPathidx = 0;
		CurLinkidx++;
		//ROS_INFO("[GlobalPath] desti Node Changed!!");
		cout << "cur node name = " << linkGeometry[CurLinkidx].FromND << endl;
	
		/* 이해가 안되~
		if (CurLinkidx == pathQueueSize)
		{
			//ROS_INFO("[ARRIVED] FINISHED!!!!!!!!!!!!!!!!!!!!!");
			finished = true;
			return false;
		}
		else
		{
			std::cout << "next node name = " << linkGeometry[CurLinkidx].ToND << endl;
		}
		*/
	
		getCurPathCOORD();
		//std::cout << "link id = " << totalLink[pathLink[curLinkNum]]["properties"]["ID"] << std::endl;
		return false;
	}
	
	//현재 링크 위에서 자신이 가야하는 지점 idx구함
	int getCurDestiPath()
	{
		//cout<<setprecision(15)<<globalCoordX<<" , "<<globalCoordY<<endl;
		//cout<<setprecision(15)<<gpsX<<" , "<<gpsY<<endl;
	
		double destiX = pathX[pathQueueSize - 1];
		double destiY = pathY[pathQueueSize - 1];
		double subX = abs(CurX - destiX);
		double subY = abs(CurY - destiY);
	
		double distance = subX + subY;
	
		// cout<<"FROM = "<<curPathIdx<<endl;
		double targetX, targetY, targetDis;
		targetX = targetY = targetDis = 0;
		for (int i = CurPathidx + 1; i < pathQueueSize; i++)
		{
			// cout<<" idx = "<<i<<endl;
			// cout<<"XX = "<<pathX[i]<<endl;
			// cout<<"YY = "<<pathY[i]<<endl;
			targetX = abs(destiX - pathX[i]);
			targetY = abs(destiY - pathY[i]);
			targetDis = targetX + targetY;
			// cout<<setprecision(15)<<"dist = "<<distancegetCurPathCOORD*100000<<endl;
			// cout<<setprecision(15)<<"target dist = "<<targetDis*100000<<endl;
			if (distance > targetDis)
			{
	
				//cout<<"TO "<<i<<endl;
				CurPathidx = i;
				return i;
			}
		}
		//abcabc  여기 어떻게 줘야 맞을까 의문이다
		//curPathIdx=pathQueueSize-1;
		CurPathidx = 0;
		return 0;
	}
	//이동할 좌표 구하기
	void getMoveDist()
	{
		//cout<<"cur idx = "<<curPathIdx<<endl;
		double dirLat = pathX[CurPathidx] - CurX;
		double dirLon = pathY[CurPathidx] - CurY;
		// 이 부분 값 조정 필요함. 너무 낮으면 이리저리 이동하고 너무 높으면 이동안함
		// if(dirLat<0.000004 && dirLat>-0.000004 )
		//     dirLat=0;
		// if(dirLon<0.000004 && dirLon>-0.000004)
		//     dirLon=0;
	
		nextX = dirLat;
		nextY = dirLon;
		//count++;
	}
	
	// 지금 GPS없어서 그냥 만들어놓은거고 나중에 GPS받는 방향으로 바꿔야됨
	void tempMove()
	{
		CurX = pathX[CurPathidx];
		CurY = pathY[CurPathidx];
		//cout<<"cur link idx = "<<curPathIdx<<endl;
		// cout<<setprecision(15)<<"move to "<<globalCoordX<<" , "<<globalCoordY<<endl;
	}
	
	// totalNode에서 id사용해서 인덱스 찾기
	// int getIdxTotalNode(std::string nodeID){
	//     int re=0;
	//     for(int i=0;i<totalLink.size();i++){
	//       //  if(totalLink)
	//     }
	//     // for(totalNode 처음부터 끝까지){
	//     //     if(nodeID==totalNode.nodeID)
	//     //         return i;
	//     // }
	//     std::cout<<"NO Node name "<<nodeID<<std::endl;
	//    return re;
	// }
	
	// totalLink의 ToNode와 nodeId사용해서 link의 idx찾기
	int getIdxTotalLink(std::string fromNodeID, std::string toNodeID)
	{
		int re = 0;
	
		for (int i = 0; i < linkGeometry.size(); i++)
		{
			if (linkGeometry[i].ToND == toNodeID && linkGeometry[i].FromND == fromNodeID)
			{
				return i;
			}
		}
		cout << "ERROR - No NODE" << endl;
		return re;
	}
	
	//차선차선 차선이동 - 이거 Local plan에 추가해야댐
	void changeLRLink()
	{
		/*
			1. 왼쪽가 오른쪽가?
			2. 현재 링크 + 다음 노드 변경
			3. 끝~
			*/
	}
	// Singleton
	static GlobalPathManager* Get()
	{
		static GlobalPathManager* instance = new GlobalPathManager();
		return instance;
	}

	// 다음으로 이동할 웨이포인트
	Position GetNextWaypoint()
	{
		return waypoints.front();
	}

	// 웨이포인트에 도달했는지 확인하고 처리
	// pos: 현재 좌표
	bool CheckAndUpdateArrival(Position& pos)
	{
		if (pos.CalcDistance(waypoints.front()) < ARRIVAL_TOLERANCE)
		{
			ArriveWaypoint();
			return true;
		}
		return false;
	}

	// 웨이포인트 도달
	void ArriveWaypoint()
	{
		if (waypoints.size() == 0) return;
		waypoints.pop();
	}

	// json 읽어들이기
	void ReadJson()
	{
	//ifstream linejson("/home/administator/linkJSON.geojson", ifstream::binary);
	//ifstream nodejson("/home/administator/nodeJSON.geojson", ifstream::binary);

	ifstream linejson("linkJSON.geojson", ifstream::binary);
	ifstream nodejson("nodeJSON.geojson", ifstream::binary);

	Json::Reader linereader, nodereader;
	Json::Value lineroot, noderoot;
	bool checkparsingRet_line = linereader.parse(linejson, lineroot);
	bool checkparsingRet_node = nodereader.parse(nodejson, noderoot);

	if (!checkparsingRet_line && !checkparsingRet_node) {
		std::cout << "Failed to parse Json : " << linereader.getFormattedErrorMessages();
		std::cout << "Failed to parse Json : " << nodereader.getFormattedErrorMessages();
		getchar();
		exit(0);
	}
	printf("Parsing\n\n");

	//line vector save
	const Json::Value lineitems = lineroot["features"]; //type, geometry, properties
	int cnt = 0;
	for (auto i = lineitems.begin(); i != lineitems.end(); i++) {

		linkGEO g;
		Json::Value ID = (*i)["properties"]["ID"];
		Json::Value LID = (*i)["properties"]["L_LinkID"];
		Json::Value RID = (*i)["properties"]["R_LinkID"];
		Json::Value FND = (*i)["properties"]["FromNodeID"];
		Json::Value TND = (*i)["properties"]["ToNodeID"];
		cnt++;
		cout << cnt << "번째 "
			<< "Start! \n"
			<< endl;

		for (auto j = 0; j != (*i)["geometry"]["coordinates"][0].size(); j++) {
			Json::Value arryX = (*i)["geometry"]["coordinates"][0][j][0]; // 0 ~ 2
			Json::Value arryY = (*i)["geometry"]["coordinates"][0][j][1];
			Json::Value arryZ = (*i)["geometry"]["coordinates"][0][j][2];

			cout << j << " : " << arryX << ", " << arryY << endl;

			g.x.push_back(stod(arryX.asString()));
			g.y.push_back(stod(arryY.asString()));
			g.z.push_back(stod(arryZ.asString()));
		}

		g.ID = ID.asString();
		g.Lane_type = g.ID.substr(0, 2);
		g.FromND = FND.asString();
		g.ToND = TND.asString();
		g.L_Link = LID.asString();
		g.R_Link = RID.asString();


		linkGeometry.push_back(g);
	}
	//--------------end-------------------------

	//node vector save
	const Json::Value nodeitems = noderoot["features"]; //type, geometry, properties
	for (auto i = nodeitems.begin(); i != nodeitems.end(); i++) {
		nodeGEO g;
		Json::Value ID = (*i)["properties"]["ID"];
		Json::Value arryX = (*i)["geometry"]["coordinates"][0]; // 0 ~ 2
		Json::Value arryY = (*i)["geometry"]["coordinates"][1];
		Json::Value arryZ = (*i)["geometry"]["coordinates"][2];

		g.ID = ID.asString();
		g.Lane_type = g.ID.substr(0, 2);
		g.x = stod(arryX.asString());
		g.y = stod(arryY.asString());
		g.z = stod(arryZ.asString());

		//cout << "type : " << g.Lane_type << ", " << g.ID << " : " << g.x << ", " << g.y << ", " << g.z << endl;

		nodeGeometry.push_back(g);
	}
	}
};
