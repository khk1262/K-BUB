#include <ros/ros.h>
#include <tf/tf.h>
#include <nav_msgs/Odometry.h>

#include <geometry_msgs/Pose2D.h>
#include <geometry_msgs/Point.h>

#include <std_msgs/Float64.h>

using namespace std;
ros::Publisher pub_z_rad;
//ros::Publisher pub_heading;
struct EulerAngles {
    double roll, pitch, yaw;
};

EulerAngles ToEulerAngles(double x, double y, double z, double w) {
    EulerAngles angles;

    // roll (x-axis rotation)
    double sinr_cosp = 2 * (w * x + y * z);
    double cosr_cosp = 1 - 2 * (x * x + y * y);
    angles.roll = std::atan2(sinr_cosp, cosr_cosp);

    // pitch (y-axis rotation)
    double sinp = 2 * (w * y - z * x);
    if (std::abs(sinp) >= 1)
        angles.pitch = std::copysign(M_PI / 2, sinp); // use 90 degrees if out of range
    else
        angles.pitch = std::asin(sinp);

    // yaw (z-axis rotation)
    double siny_cosp = 2 * (w * z + x * y);
    double cosy_cosp = 1 - 2 * (y * y + z * z);
    angles.yaw = std::atan2(siny_cosp, cosy_cosp);

    return angles;
}

void odometryCallback_(const nav_msgs::Odometry::ConstPtr &msg){
    geometry_msgs::Pose2D pose2d;
    pose2d.x = msg->pose.pose.position.x;
    pose2d.y = msg->pose.pose.position.y;

    tf::Quaternion q(
        msg->pose.pose.orientation.x,
        msg->pose.pose.orientation.y,
        msg->pose.pose.orientation.z,
        msg->pose.pose.orientation.w);
    
    tf::Matrix3x3 m(q);
    double roll, pitch, yaw;
    m.getRPY(roll, pitch, yaw);

    EulerAngles angles = ToEulerAngles(msg->pose.pose.orientation.x,msg->pose.pose.orientation.y,msg->pose.pose.orientation.z,msg->pose.pose.orientation.w);
    
    double yaw_degrees = angles.yaw * 180.0 / 3.141592; // conversion to degrees
    //if( yaw_degrees < 0 ) yaw_degrees += 360.0; // convert negative to positive angles
    cout<<angles.yaw<<endl;
    cout<<yaw_degrees<<endl;
    // pose2d.theta = yaw_degrees;
    // pub_pose_.publish(pose2d);
    pose2d.theta = angles.yaw;
    //pub_z_rad.publish(angles.yaw);
    pub_z_rad.publish(pose2d);

}

int main(int argc, char**argv){
ros::init(argc, argv, "roi_angle");
ros::NodeHandle nh;

ros::Subscriber sub_odom_ = nh.subscribe("/odom", 1, odometryCallback_);
//pub_pose_ = nh.advertise<geometry_msgs::Pose2D>("/z_axis_angle", 1);
// pub_

ros::spin ();
}