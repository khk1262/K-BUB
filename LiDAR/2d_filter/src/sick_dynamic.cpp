#include <cmath>
#include <iostream>

#include <ros/ros.h>
#include <geometry_msgs/Point.h>
#include "sick_filter/PointArray.h"
#include <std_msgs/Bool.h>
#include <std_msgs/Float32.h>

ros::Publisher pub_dynamic_;
std_msgs::Bool isStop;

void dynamic_cb(const std_msgs::Bool::ConstPtr &input){
  
  if(input->data){
      std::cout<<" [Dynamic Obstacle] : true" <<std::endl;
      isStop.data = 1;
  }

  else{
      std::cerr<<" [Dynamic Obstacle] : false" <<std::endl;
      isStop.data = 0;
      }
      
      pub_dynamic_.publish(isStop);
}

int main(int argc, char** argv) {
  /* Initialize ROS */
  ros::init(argc, argv, "sick_dynamic");
  ros::NodeHandle nh;
  
  pub_dynamic_ = nh.advertise<std_msgs::Bool>("/isStop", 10);
  ros::Subscriber sick_sub = nh.subscribe<std_msgs::Bool>("/isStop_temp", 10, dynamic_cb);
  
  ros::spin();
}