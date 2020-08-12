#include <ros/ros.h>
#include "velodyne_filter/PointArray.h"
#include "velodyne_filter/ParkPoint.h"
#include <geometry_msgs/Point.h>
#include <std_msgs/Bool.h>
#include <sensor_msgs/PointCloud2.h>

#include <sensor_msgs/PointCloud.h>
#include <sensor_msgs/point_cloud_conversion.h>


#include <cmath>
#include <iostream>
#include <utility>

using namespace std;

#define X first
#define Y second
using ld = long double;
using Point = pair<ld, ld>;


ros::Publisher park_info_pub;

velodyne_filter::ParkPoint park_point_info;

bool parking_lot = true; // 비전 or gps로부터 주차장 플래그 받음

float P_first_x;
float P_first_y;

float P_second_x;
float P_second_y;

float P_third_x;
float P_third_y;

float P_fourth_x;
float P_fourth_y;


uint32_t park_info_seq_ = 0;
bool t = false;

bool can_park = false;

// ccw 알고리즘을 이용하여 a, b 벡터 오른쪽에 c 벡터의 존재 여부 판단
int ccw(const Point& a, const Point& b, const Point& c) {

	ld tmp = (b.X - a.X) * (c.Y - a.Y) - (b.Y - a.Y) * (c.X - a.X);

	if (tmp < 0)
		return -1;
	else if (tmp > 0)
		return 1;
	else
	    return 0;
}




// void parking_cb(const velodyne_filter::PointArray::ConstPtr& input){

//     //bool temp_park=false;
    
//     // parking space - 4 edge points
// 	Point p1 = make_pair(P_first_x, P_first_y);
// 	Point p2 = make_pair(P_second_x, P_second_y);
// 	Point p3 = make_pair(P_third_x, P_third_y);
// 	Point p4 = make_pair(P_fourth_x, P_fourth_y);

//     Point obstacle_center;
//     Point obstacle_min;
//     Point obstacle_max;

//     float error_rate = 0;
//     int true_count, false_count;
//     true_count = false_count = 0;


//     if(parking_lot){

//         bool park_temp = false;

//         for(int i = 0; i < input->cluster_center.size(); i++){

//             obstacle_center = make_pair(input->cluster_center[i].x, input->cluster_center[i].y);
//             obstacle_min = make_pair(input->cluster_min[i].x, input->cluster_min[i].y);
//             obstacle_max = make_pair(input->cluster_max[i].x, input->cluster_max[i].y);

//             int judge_center = 0;
//             int judge_min = 0;
//             int judge_max = 0;

//             judge_center += ccw(p1, p2, obstacle_center);
//             judge_center += ccw(p2, p3, obstacle_center);
//             judge_center += ccw(p3, p4, obstacle_center);
//             judge_center += ccw(p4, p1, obstacle_center);


//             judge_min += ccw(p1, p2, obstacle_min);
//             judge_min += ccw(p2, p3, obstacle_min);
//             judge_min += ccw(p3, p4, obstacle_min);
//             judge_min += ccw(p4, p1, obstacle_min);


//             judge_max += ccw(p1, p2, obstacle_max);
//             judge_max += ccw(p2, p3, obstacle_max);
//             judge_max += ccw(p3, p4, obstacle_max);
//             judge_max += ccw(p4, p1, obstacle_max);
            

//             std::cout << "judge_center : " << judge_center <<'\n';
//             std::cout << "judge_max : " << judge_max << '\n';
//             std::cout << "judge_min : " << judge_min << '\n';


//             if(judge_center != 4 && judge_max != 4 && judge_min != 4){
//                 std::cout << "no obstacle in parking area" << '\n';
//                 park_temp = true;
//                 true_count++;
//             }

//             if(judge_center == 4 || judge_max == 4 || judge_min == 4){
//             // else{
//                 std::cout << "obstacle in parking area" << '\n';
//                 park_temp = false;
//                 false_count++;
//             }
//         }

//         if(park_temp)
//             park_point_info.can_park.data = true;
        
//         else
//             park_point_info.can_park.data = false;
//     }

//     else{
//         std::cout << "here is not parking lot" << '\n';
//         park_point_info.can_park.data = false;
//     }


// // ==== parking lot point ======
//     geometry_msgs::Point park_first;
//     geometry_msgs::Point park_second;
//     geometry_msgs::Point park_third;
//     geometry_msgs::Point park_fourth;

//     park_first.x = P_first_x;
//     park_first.y = P_first_y;

//     park_second.x = P_second_x;
//     park_second.y = P_second_y;

//     park_third.x = P_third_x;
//     park_third.y = P_third_y;

//     park_fourth.x = P_fourth_x;
//     park_fourth.y = P_fourth_y;   

//     if(!t){
//         park_point_info.park_points.push_back(park_first);
//         park_point_info.park_points.push_back(park_second);
//         park_point_info.park_points.push_back(park_third);
//         park_point_info.park_points.push_back(park_fourth);
//         t++;
//     }

//     park_point_info.header.seq = ++park_info_seq_;
//     park_point_info.header.stamp = ros::Time::now();
//     park_point_info.header.frame_id = "velodyne_park";



//     park_info_pub.publish(park_point_info);


//     ROS_INFO("park published");
// }


void parking_second_cb(const sensor_msgs::PointCloud2::ConstPtr& input){
    
    sensor_msgs::PointCloud total;
    sensor_msgs::convertPointCloud2ToPointCloud(*input, total);


	Point p1 = make_pair(P_first_x, P_first_y);
	Point p2 = make_pair(P_second_x, P_second_y);
	Point p3 = make_pair(P_third_x, P_third_y);
	Point p4 = make_pair(P_fourth_x, P_fourth_y);

    Point obstacle;


    int true_count = 0;

    if(can_park) std::cout << "can park : true" << '\n';
    else std::cout << "can park : false" << '\n';

    if(parking_lot){
        if(can_park){
            park_point_info.can_park.data = true;           
        }
        else{
            bool park_temp = false;

            for(int i = 0; i < total.points.size(); i++){
                obstacle = make_pair(total.points[i].x, total.points[i].y);

                int judge = 0;

                judge += ccw(p1, p2, obstacle);
                judge += ccw(p2, p3, obstacle);
                judge += ccw(p3, p4, obstacle);
                judge += ccw(p4, p1, obstacle);

                // std::cout << "point" << i << " : " << judge << '\n';

                if(judge == 4){
                    true_count++;
                }

            }



            std::cout << "total point size : " << total.points.size() << '\n';
            std::cout << "true_count : " << true_count << '\n';

            if(true_count <= 0){
                park_point_info.can_park.data = true;
            }
            else
                park_point_info.can_park.data = false;

// ==== parking lot point ======
    geometry_msgs::Point park_first;
    geometry_msgs::Point park_second;
    geometry_msgs::Point park_third;
    geometry_msgs::Point park_fourth;

    park_first.x = P_first_x;
    park_first.y = P_first_y;

    park_second.x = P_second_x;
    park_second.y = P_second_y;

    park_third.x = P_third_x;
    park_third.y = P_third_y;

    park_fourth.x = P_fourth_x;
    park_fourth.y = P_fourth_y;   

    if(!t){
        park_point_info.park_points.push_back(park_first);
        park_point_info.park_points.push_back(park_second);
        park_point_info.park_points.push_back(park_third);
        park_point_info.park_points.push_back(park_fourth);
        t = true;
    }

    park_point_info.header.seq = ++park_info_seq_;
    park_point_info.header.stamp = ros::Time::now();
    park_point_info.header.frame_id = "velodyne_park";


    park_info_pub.publish(park_point_info);

    ROS_INFO("park published");            

    }

    }
    else
        std::cout << "here is not parking area" << '\n';

}

void can_park_cb(const std_msgs::Bool::ConstPtr& input){
    can_park = input->data;
}


int main(int argc, char **argv){

    ros::init(argc, argv, "velodyne_parking");
    ros::NodeHandle nh;


    ros::Subscriber sub_park = nh.subscribe ("/can_park", 10, can_park_cb);

 
    // ros::Subscriber sub = nh.subscribe ("/velodyne_cluster_node/velodyne_cluster_info", 1, parking_cb);
    ros::Subscriber sub_cluster = nh.subscribe ("/velodyne_cluster", 10, parking_second_cb);

    park_info_pub = nh.advertise<velodyne_filter::ParkPoint>("park_point", 10);


    //parameter
    nh.param<float>("P_first_x", P_first_x, 1.25);
    nh.param<float>("P_first_y", P_first_y, -1.0);

    nh.param<float>("P_second_x", P_second_x, 1.25);
    nh.param<float>("P_second_y", P_second_y, -5.0);

    nh.param<float>("P_third_x", P_third_x, 5.0);
    nh.param<float>("P_third_y", P_third_y, -5.0);

    nh.param<float>("P_fourth_x", P_fourth_x, 5.0);
    nh.param<float>("P_fourth_y", P_fourth_y, -1.0);


  // Spin
  ros::spin ();
}