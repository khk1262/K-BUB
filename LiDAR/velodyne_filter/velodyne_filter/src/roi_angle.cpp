#include <ros/ros.h>
#include <geometry_msgs/Point.h>

int main(int argc, char**argv){
ros::init(argc, argv, "publish_z_axis_angle");
ros::NodeHandle nh;

ros::Publisher pub = nh.advertise<geometry_msgs::Point>("/z_axis_angle", 1);


ros::Rate loop_rate(2);
float tmp = 0.0;

while(ros::ok()){

    geometry_msgs::Point msg;

    msg.x = 0.0;
    msg.y = 0.0;
    tmp += 1;
    msg.z = tmp;


    pub.publish(msg);

    loop_rate.sleep();
    }
ros::spin ();
}