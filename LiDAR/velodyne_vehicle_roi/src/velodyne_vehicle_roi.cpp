#include <iostream>
#include <fstream>
#include <stdint.h>
#include <math.h>

#include <ros/ros.h>
#include <pcl_ros/point_cloud.h>
#include <pcl_conversions/pcl_conversions.h>
#include <pcl/point_cloud.h>
#include <pcl/point_types.h>
#include <pcl/common/common_headers.h>
#include <pcl/io/pcd_io.h>
#include <pcl/point_types.h>
#include <pcl/features/normal_3d.h>
#include <pcl/console/parse.h>
#include <pcl/filters/passthrough.h>
#include <pcl/PCLPointCloud2.h>
#include <pcl/conversions.h>
#include <pcl_ros/transforms.h>
#include <sensor_msgs/point_field_conversion.h>

pcl::PointCloud<pcl::PointXYZ> cloud;
pcl::PointCloud<pcl::PointXYZ> cloud2;
int flag = 0;
int i;
int cnt;
sensor_msgs::PointCloud2 cloud_out;
sensor_msgs::PointCloud test;

void callback(const sensor_msgs::PointCloud2ConstPtr& msg) 
{ 
  std::cerr<<"[why] whywhywhy!!"<<std::endl;
  
  pcl::PCLPointCloud2 pcl_pc2;
  pcl_conversions::toPCL(*msg,pcl_pc2);
  pcl::PointCloud<pcl::PointXYZ>::Ptr temp_cloud(new pcl::PointCloud<pcl::PointXYZ>);
  pcl::fromPCLPointCloud2(pcl_pc2,*temp_cloud);

  cloud = *temp_cloud;
  cloud2.clear();


  std::cerr<<"[why] whywhywhy!!"<<std::endl;

  cnt = 0;
  for(i = 0;i<cloud.width;i++)
  {
    if(!(cloud.at(i).x < 1 & cloud.at(i).x > -1 & cloud.at(i).y < 0.7 & cloud.at(i).y > -0.7))
    {
      std::cerr<<"[tf] tftftftftftftf!!"<<std::endl;
      cloud[i].x += 1;
      cloud2.push_back(cloud.at(i));
      cnt++;
    }
  } 
  cloud2.width = cnt;
  cloud2.height = 1;
  cloud2.header = cloud.header;
  flag = 1;
}


// --------------
// -----Main-----
// --------------
int main(int argc, char** argv) {
  /* Initialize ROS */
  ros::init(argc, argv, "velodyne_vehicle_roi");
  ros::NodeHandle nh;
  ros::Subscriber sub = nh.subscribe("/cloud", 1, callback);
  ros::Publisher points_pub = nh.advertise<sensor_msgs::PointCloud2>("/roi_scan", 32);
  ros::Rate r(10); 
    while(ros::ok())
    {
      if(flag == 1)
      {
            points_pub.publish(cloud2);
            flag = 0;
      }
         ros::spinOnce();
    }
}