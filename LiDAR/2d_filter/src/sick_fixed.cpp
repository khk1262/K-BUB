#include <iostream>
#include <cmath>

#include <ros/ros.h>

#include <sensor_msgs/PointCloud2.h>
#include "sick_filter/PointArray.h"
#include <geometry_msgs/Point.h>
#include <std_msgs/Bool.h>
#include <std_msgs/Float32.h>

ros::Publisher static_steer;

float distance_offset;
float angle_offset;
bool ob_find;

void ob_find_cb(const std_msgs::Bool::ConstPtr& input){
  ob_find = input->data;
}

void distance_cb(const std_msgs::Float32::ConstPtr& input){
  if(input->data != 0)
    distance_offset = input->data*-1;  
}

void angle_cb(const std_msgs::Float32::ConstPtr& input){
    if (input->data!=0)
      angle_offset = input->data;
}

void static_cb(const sick_filter::PointArray::ConstPtr& input){

  std_msgs::Float32 des_steer;

  float rot_coord = 0;

  if(ob_find){

    for(int i = 0; i < input->cluster_center.size();i++){
      float ob_size = abs(input->cluster_max[i].y - input->cluster_min[i].y);
        if(  ob_size > 0.6 && ob_size < 1.5 ){
          float rot_angle = angle_offset * M_PI / 180;
          rot_coord += sin(angle_offset) * input->cluster_center[i].x + cos(angle_offset) * input->cluster_center[i].y;
          }
      }

      if(rot_coord+distance_offset >= 0){
        des_steer.data = 15.0;
        ROS_INFO("avoid right\n");
        ROS_INFO("steer : %f", des_steer.data);
      }
      else{
        des_steer.data = 15.0;
        ROS_INFO("avoid left\n");
        ROS_INFO("steer : %f", des_steer.data);
      }
    }
    else
        des_steer.data = 0.0;

  static_steer.publish(des_steer);

}


int main(int argc, char** argv){
  ros::init(argc, argv, "sick_fixed");
  ros::NodeHandle nh;

  ros::Subscriber sick_sub = nh.subscribe<sick_filter::PointArray>("/sick_cluster_info", 10, static_cb);
  ros::Subscriber st_phi_sub = nh.subscribe<std_msgs::Float32>("/phi", 10, angle_cb);
  ros::Subscriber st_e_sub = nh.subscribe<std_msgs::Float32>("/e", 10, distance_cb);
  ros::Subscriber ob_find_sub = nh.subscribe<std_msgs::Bool>("/isStop_temp", 10, ob_find_cb);
  
  
  static_steer = nh.advertise<std_msgs::Float32>("sick_static_steer", 10);


  ros::spin();

}