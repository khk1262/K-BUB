#include <ros/ros.h>
#include <visualization_msgs/Marker.h>
#include "velodyne_filter/PointArray.h"
#include "velodyne_filter/ParkPoint.h"
#include <cmath>
/*
float targetMinX=1;
float targetMaxX=2.5;

float targetMaxY=-1.5;
float targetMinY=-4;

*/
float scaleX=0;
float scaleY=0;
float positionX=0;
float positionY=0;

float xMin=0;
float yMin=0;
float xMax=0;
float yMax=0;


float changeR=0;
float changeG=0;
float changeB=0;


void color_mark(const velodyne_filter::ParkPoint::ConstPtr& msg){

  xMin=msg->park_points[1].x;
  yMin=msg->park_points[1].y;

  xMax=msg->park_points[3].x;
  yMax=msg->park_points[3].y;
  scaleX = std::abs(xMax-xMin);
  scaleY = std::abs(yMax-yMin);
  positionX = std::abs((xMax+xMin))/2;
  positionY = ((yMax+yMin))/2;

  if(msg->can_park.data){
    changeR=0.53f;
    changeG=0.81f;
    changeB=0.98f;
  }
  else{
    changeR=1.0f;
    changeG=0.0f;
    changeB=0.0f;
  }
}


int main(int argc, char** argv)
{
  ros::init(argc, argv, "velodyne_parking_visual");
  ros::NodeHandle nh;


  ros::Subscriber subMin = nh.subscribe("park_point", 10, color_mark);


  ros::Publisher marker_pub = nh.advertise<visualization_msgs::Marker>("marker", 10);

  ros::Rate loop_rate(10);
  while (ros::ok())
  {

    visualization_msgs::Marker marker;
    marker.header.frame_id = "/velodyne";
    marker.header.stamp = ros::Time::now();
    marker.ns = "basic_shapes";
    marker.id = 0;

    marker.type = visualization_msgs::Marker::CUBE;
    marker.action = visualization_msgs::Marker::ADD;
    marker.lifetime = ros::Duration();

    marker.scale.x = scaleX;
    marker.scale.y = scaleY;
    marker.scale.z = 0.1;

    marker.pose.position.x = positionX;
    marker.pose.position.y = positionY;
    marker.pose.position.z = 0;

    marker.pose.orientation.x = 0;
    marker.pose.orientation.y = 0;
    marker.pose.orientation.z = 0;
    marker.pose.orientation.w = 1;

    marker.color.r = changeR;
    marker.color.g = changeG;
    marker.color.b = changeB;
    marker.color.a = 0.7f;
    marker_pub.publish(marker);

    ros::spinOnce();
    loop_rate.sleep();
  }
  return 0;
}