#include <iostream>
#include <fstream>
#include "json/json.h"
#include <time.h>
#include <Windows.h>

//ros 부분
#include <jsoncpp/json.h>
#include <ros/ros.h>

#pragma comment(lib , "lib_json.lib")
#pragma warning(disable: 4996) 

using namespace std;

int main()
{

	ros::init(argc, argv, "roscpp");
	
	ros::NodeHandle nh;

	try {
		while (nh.ok()) {
			ifstream json("linkJSON.geojson", ifstream::binary);

			Json::Reader reader;
			Json::Value root;
			bool parsingRet = reader.parse(json, root);

			if (!parsingRet)
			{
				std::cout << "Failed to parse Json : " << reader.getFormattedErrorMessages();
				getchar();
				return 0;
			}

			//전체 출력
			//cout << root["features"] << endl;

			// 멤버 추출
			/*
			vector<string> item = root.getMemberNames(); //crs, features, name, type

			for (int i = 0; i < item.size(); i++) {
				cout << item[i] << endl;
			}
			*/

			printf("Parsing\n\n");

			const Json::Value items = root["features"]; //type, geometry, properties
			for (auto i = items.begin(); i != items.end(); i++)
			{
				if (i->isObject())
				{
					//cout << (*i)["geometry"] << " : " << (*i)["properties"] << endl; //출력은 됨.

					const Json::Value Grids = (*i)["geometry"]; //type, coordinates
					for (auto i = Grids.begin(); i != Grids.end(); i++) {
						if (i->isObject())
						{

						}
						else if (i->isArray()) {	//coordinates는 array
							printf("Array!\n");
							Sleep(1000);
							cout << (*i) << endl;
							Sleep(1000);
						}
						else if (i->isString()) {	//type는 string
							printf("string!\n");
							Sleep(1000);
							cout << (*i) << endl;
							Sleep(1000);
						}
						else {
							printf("NO~\n");
						}
					}
				}
			}

			printf("Parsing End\n");
			getchar();
			ros::spinOnce();
		}
	}

	catch (std::runtime_error& e) {
		ROS_ERROR("Exception: %s", e.what());
		return -1;
	}
	return 0;
}