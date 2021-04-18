
#include <tf2_ros/static_transform_broadcaster.h>
#include <tf2/LinearMath/Quaternion.h>

// ROS specific includes

#include <ros/ros.h>
#include <sensor_msgs/PointCloud2.h>
#include <pcl_conversions/pcl_conversions.h>
#include "sick_filter/PointArray.h"
#include <geometry_msgs/Point.h>
#include <tf/transform_broadcaster.h>
#include <std_msgs/Bool.h>
#include <cmath>

// PCL specific includes

#include <pcl/ros/conversions.h>
#include <pcl/point_cloud.h>
#include <pcl/point_types.h>

#include <pcl/ModelCoefficients.h>
#include <pcl/filters/extract_indices.h>
#include <pcl/filters/voxel_grid.h>
#include <pcl/filters/statistical_outlier_removal.h>

#include <pcl/filters/crop_box.h>

#include <pcl/features/normal_3d.h>
#include <pcl/kdtree/kdtree.h>
#include <pcl/sample_consensus/method_types.h>
#include <pcl/sample_consensus/model_types.h>
#include <pcl/segmentation/sac_segmentation.h>
#include <pcl/segmentation/extract_clusters.h>
#include <pcl/common/common.h>
#include <pcl/common/centroid.h>

ros::Publisher pub;

ros::Publisher cluster_info_pub_;

// 전방 장애물 검출되면 stop 보내는 것
ros::Publisher isStop_pub_;


typedef pcl::PointXYZ PointT;

bool print_fps_;

float x_axis_min_;
float x_axis_max_;

float y_axis_min_;
float y_axis_max_;

float z_axis_angle_;

float cluster_meter_;
int cluster_size_min_;
int cluster_size_max_;

uint32_t cluster_info_seq_ = 0;


std::string frame_id_;


std_msgs::Bool isStop_first;



int frames; clock_t start_time; bool reset = true;//fps

void cloud_cb(const sensor_msgs::PointCloud2ConstPtr &input){

  if(print_fps_)if(reset){frames=0;start_time=clock();reset=false;}//fps


    // static tf::TransformBroadcaster br;
    // tf::Transform transform;
    // transform.setOrigin( tf::Vector3(1.0, 0.0, 0.0) );
    // tf::Quaternion q;
    // q.setRPY(0, 0, 0);

    // transform.setRotation(q);
    // br.sendTransform(tf::StampedTransform(transform, ros::Time::now(), "map", frame_id_));


 // Convert the sensor_msgs/PointCloud2 data to pcl/PointCloud
  pcl::PointCloud<PointT> cloud;
  pcl::fromROSMsg (*input, cloud);
 
  // Data containers used
  // Create the filtering object: downsample the dataset using a leaf size of 1cm
  
  pcl::VoxelGrid<PointT> vg;
  pcl::PointCloud<PointT>::Ptr cloud_filtered (new pcl::PointCloud<PointT>);
  vg.setInputCloud (cloud.makeShared());
  vg.setLeafSize (0.1f, 0.1f, 0.1f);
  vg.filter (*cloud_filtered);
  std::cout << "PointCloud after filtering has: " << cloud_filtered->points.size ()  << " data points." << std::endl;

// crop box filter
  pcl::PointCloud<PointT>::Ptr cloud_filtered2 (new pcl::PointCloud<PointT>);

  pcl::CropBox<PointT> cropFilter;
  cropFilter.setInputCloud(cloud_filtered);

  cropFilter.setMin(Eigen::Vector4f(x_axis_min_, y_axis_min_, 0, 0));
  cropFilter.setMax(Eigen::Vector4f(x_axis_max_, y_axis_max_, 0, 0));

  cropFilter.setTranslation(Eigen::Vector3f(0.0, 0.0, 0.0));
  cropFilter.setRotation(Eigen::Vector3f(0.0, 0.0, z_axis_angle_));
  cropFilter.filter(*cloud_filtered2);


  std::cout << "PointCloud after ROI: " << cloud_filtered2->points.size ()  << " data points." << std::endl;

  if((*cloud_filtered2).size() <= 0){

      ROS_INFO("[none obstacle]");

      isStop_first.data = false;
      isStop_pub_.publish(isStop_first);

  }

  else{

    pcl::PointCloud<PointT>::Ptr cloud_ROI (new pcl::PointCloud<PointT>);

    // 오브젝트 생성 
    pcl::StatisticalOutlierRemoval<PointT> sor;
    sor.setInputCloud (cloud_filtered2);            //입력 
    sor.setMeanK (50);                    //분석시 고려한 이웃 점 수(50개)
    sor.setStddevMulThresh (1.0);         //Outlier로 처리할 거리 정보 
    sor.filter (*cloud_ROI);         // 필터 적용 

  std::cout << "PointCloud after OutLier: " << cloud_ROI->points.size ()  << " data points." << std::endl;





  // Creating the KdTree object for the search method of the extraction
  
    // 탐색을 위한 KdTree 오브젝트 생성
    pcl::search::KdTree<PointT>::Ptr tree (new pcl::search::KdTree<PointT>);
    tree->setInputCloud (cloud_ROI); //KdTree 생성

    std::vector<pcl::PointIndices> cluster_indices; // 군집화된 결과물의 Index 저장
    // 군집화 오브젝트 생성
    pcl::EuclideanClusterExtraction<PointT> ec;
    ec.setClusterTolerance (cluster_meter_); // 30cm
    ec.setMinClusterSize (cluster_size_min_); // 최소 포인트 수
    ec.setMaxClusterSize (cluster_size_max_); // 최대 포인트 수
    ec.setSearchMethod (tree); // 위에서 정의한 탐색 방법 지정
    ec.setInputCloud (cloud_ROI); // 입력
    ec.extract (cluster_indices); // 군집화 적용

    std::cout << "Number of clusters is equal to " << cluster_indices.size () << std::endl;
    pcl::PointCloud<pcl::PointXYZI> TotalCloud;


  // 클러스터별 정보 수집, 출력, 저장
    std::vector<pcl::PointCloud<PointT>::Ptr, Eigen::aligned_allocator<pcl::PointCloud<PointT>::Ptr > > clusters;
    int j = 0;

    for (std::vector<pcl::PointIndices>::const_iterator it = cluster_indices.begin (); it != cluster_indices.end (); ++it)
    {
      pcl::PointCloud<PointT>::Ptr cluster(new pcl::PointCloud<PointT>);

      for (std::vector<int>::const_iterator pit = it->indices.begin (); pit != it->indices.end (); ++pit)
      {
          cluster->points.push_back(cloud_ROI->points[*pit]);


          pcl::PointXYZ pt = cloud_ROI->points[*pit];
              pcl::PointXYZI pt2;
              pt2.x = pt.x, pt2.y = pt.y, pt2.z = pt.z;
              pt2.intensity = (float)(j + 1);

              TotalCloud.push_back(pt2);
      }
      cluster->width = cluster->size();
      cluster->height = 1;
      cluster->is_dense = true;
      clusters.push_back(cluster);
      j++;
    }

  // PointArray_vector
    sick_filter::PointArray cluster_info;

    for(int i =0; i<clusters.size(); i++){

        Eigen::Vector4f centroid;
        Eigen::Vector4f min_p;
        Eigen::Vector4f max_p;


        pcl::compute3DCentroid(*clusters[i], centroid);
        pcl::getMinMax3D(*clusters[i], min_p, max_p);
      
        geometry_msgs::Point center_point;
        center_point.x = centroid[0];
        center_point.y = centroid[1];
        center_point.z = 0;

        geometry_msgs::Point min_point;
        min_point.x = min_p[0];
        min_point.y = min_p[1];
        min_point.z = 0;

        geometry_msgs::Point max_point;
        max_point.x = max_p[0];
        max_point.y = max_p[1];
        max_point.z = 0;

        ROS_INFO("===================================");

        ROS_INFO("cluster : %d", i+1); 
        ROS_INFO("center x position : %lf", center_point.x); 
        ROS_INFO("center y position : %lf", center_point.y); 
        ROS_INFO("center z position : %lf", center_point.z); 

        ROS_INFO("===================================");

        ROS_INFO("min x position : %lf", min_point.x); 
        ROS_INFO("min y position : %lf", min_point.y); 
        ROS_INFO("min z position : %lf", min_point.z); 

        ROS_INFO("===================================");

        ROS_INFO("max x position : %lf", max_point.x); 
        ROS_INFO("max y position : %lf", max_point.y); 
        ROS_INFO("max z position : %lf", max_point.z); 

        cluster_info.cluster_center.push_back(center_point);
        cluster_info.cluster_min.push_back(min_point);
        cluster_info.cluster_max.push_back(max_point);

    }

    if(cluster_info.cluster_center.size()) {
      cluster_info.header.seq = ++cluster_info_seq_;
      cluster_info.header.stamp = ros::Time::now();
      cluster_info.header.frame_id = frame_id_;
      cluster_info_pub_.publish(cluster_info);
    }



//============== 동적 장애물 미션 수행 부분 코드 ====================
//=============================================================

      ROS_INFO("Mission : dynamic obstacle");

      // 전방 장애물 검출시 boolean 보내주는 메시지
      int temp = 0;
      std_msgs::Bool isStop_second;

      for(int i = 0; i < cluster_info.cluster_center.size(); i++){
          if(abs(cluster_info.cluster_max[i].y - cluster_info.cluster_min[i].y) > 0.30 || abs(cluster_info.cluster_max[i].x - cluster_info.cluster_min[i].x) > 0.60){
              if( cluster_info.cluster_min[i].x < 6.0)
              {   
                  temp++;
                  // isStop_second.data = true;
                  std::cerr<<"[dynmaic_obstacle_2] Object appeared"<<std::endl;
              }
              else
              {
                  // isStop_second.data = false;
                  std::cerr<<"[dynmaic_obstacle_2] Object disappeared"<<std::endl;
              }
          }
          else{
                  // isStop_second.data = false;
                  std::cerr<<"[dynmaic_obstacle_3] Object is small"<<std::endl;
          }
          if(temp)
            isStop_second.data = true;
          else
            isStop_second.data = false;

          isStop_pub_.publish(isStop_second);

      }

//=============================================================
      
      // Convert To ROS data type
      pcl::PCLPointCloud2 cloud_p;
      pcl::toPCLPointCloud2(TotalCloud, cloud_p);

      sensor_msgs::PointCloud2 output;
      pcl_conversions::fromPCL(cloud_p, output);

      output.header.frame_id = "laser";

      pub.publish(output);

      if(print_fps_)if(++frames>10){std::cerr<<"[sick_clustering] fps = "<<float(frames)/(float(clock()-start_time)/CLOCKS_PER_SEC)<<", timestamp = "<<clock()/CLOCKS_PER_SEC<<std::endl;reset = true;}//fps


      ROS_INFO("published it.");
  }
}



int main(int argc, char ** argv)
{
    // Initialize ROS
    ros::init(argc, argv, "sick_cluster");
    ros::NodeHandle nh;
    ros::NodeHandle private_nh("~");

    // Create a ROS subscriber for the input point cloud
    ros::Subscriber sub = nh.subscribe("/roi_scan", 10, cloud_cb);

    pub = private_nh.advertise<sensor_msgs::PointCloud2> ("/sick_cluster", 10);

    cluster_info_pub_ = private_nh.advertise<sick_filter::PointArray> ("/sick_cluster_info", 10);

    isStop_pub_ = private_nh.advertise<std_msgs::Bool>("/isStop_temp", 10);


//=========================== parameter ==================================
//========================================================================


    private_nh.param<std::string>("frame_id", frame_id_, "laser");
    private_nh.param<bool>("print_fps", print_fps_, false);

    private_nh.param<float>("x_axis_min", x_axis_min_, -0.0);
    private_nh.param<float>("x_axis_max", x_axis_max_, 7.0);

    private_nh.param<float>("y_axis_min", y_axis_min_, -1.0);
    private_nh.param<float>("y_axis_max", y_axis_max_, 1.0);

    private_nh.param<float>("z_axis_angle_", z_axis_angle_, 0.0);


    private_nh.param<float>("cluster_meter", cluster_meter_, 0.3);
    private_nh.param<int>("cluster_size_min", cluster_size_min_, 3);
    private_nh.param<int>("cluster_size_max", cluster_size_max_, 5000);

    // Spin
    ros::spin();
}