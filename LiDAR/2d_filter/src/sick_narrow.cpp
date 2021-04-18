#include <ros/ros.h>
#include <geometry_msgs/Point.h>
#include "sick_filter/PointArray.h"
#include <vector>
#include <utility>
#include <cmath>
#include <algorithm>

#define X first
#define Y second

using namespace std;

std::vector<pair<float, float> > right_ob_v;
std::vector<pair<float, float> > left_ob_v;

std::vector<pair<float, float> > coord_v;


ros::Publisher coord_pub;

// 비교 함수
// bool compare(std::pair<float, float> p1, std::pair<float, float> p2){
//     if(p1.X == p2.X)
//         return p1.Y < p2.Y;
//     return p1.X < p2.X;
// }

// 맨하탄 거리 함수
float manhattan(std::pair<float, float> p1, std::pair<float, float> p2){
    return abs(p2.X - p1.X) + abs(p2.Y - p1.Y);
}


void narrow_cb(const sick_filter::PointArray::ConstPtr &input){

    std::pair<float, float> right_ob;
    std::pair<float, float> left_ob;

    std::pair<float, float> coord;

    right_ob_v.clear();
    left_ob_v.clear();

    for(int i = 0; i < input->cluster_center.size(); i++){

        float max_x = input->cluster_max[i].x;
        float max_y = input->cluster_max[i].y;
        float min_x = input->cluster_min[i].x;
        float min_y = input->cluster_min[i].y;
        float cen_x = input->cluster_center[i].x;
        float cen_y = input->cluster_center[i].y;

    
        float ob_size = sqrt(pow(max_x - min_x, 2) + pow(max_y - min_y, 2)); // 장애물 크기

        if(0.01 < ob_size && ob_size < 0.2){
            if(cen_y > 0){
                left_ob = make_pair(cen_x, cen_y);

                if(left_ob_v.size() == 0){
                    left_ob_v.push_back(left_ob);
                }
                else{
                    if(manhattan(left_ob_v.back(), left_ob) < 0.6)
                        left_ob_v.push_back(left_ob);
                    
                    else
                        right_ob_v.push_back(left_ob);
                }
            }
            else{
                right_ob = make_pair(cen_x, cen_y);

                if(right_ob_v.size() == 0){
                    right_ob_v.push_back(right_ob);
                }
                else{
                    if(manhattan(right_ob_v.back(), right_ob) < 0.6)
                        right_ob_v.push_back(right_ob);
                    
                    else
                        left_ob_v.push_back(right_ob);
                }
            }
        }
        // vector sorting
        sort(right_ob_v.begin(), right_ob_v.end());
        sort(left_ob_v.begin(), left_ob_v.end());
    }
    

    coord_v.clear();

    // 양쪽 벡터에 아무런 데이터가 들어있지 않으면 내보내는게 아무것도 없게 만듬
    // unknown이 문제를 일으키면 추후 다른 방식으로 pub하도록 변경
    if(right_ob_v.size() == 0 && left_ob_v.size() == 0){
        std::cout << "empty vector" << std::endl;
        return;
    }
    
    else if(right_ob_v.size() != 0 && left_ob_v.size() != 0){
        std::cout << "[case_1] entering narrow path" << std::endl;
        
        if(right_ob_v.size() > left_ob_v.size()){
            for(int i = 0; i < left_ob_v.size(); i++){
                for(int j = 0; j < right_ob_v.size(); j++){
                    float coord_x = 0;
                    if(i == j){
                        coord_x = (left_ob_v[i].X + right_ob_v[j].X) / 2.0; 
                        coord = make_pair(coord_x,abs(left_ob_v[i].Y - right_ob_v[j].Y) / 2.0);
                        coord_v.push_back(coord);
                    }
                    if(i == left_ob_v.size()-1 && left_ob_v.size() - 1 < j){
                        coord_x = (left_ob_v[i].X + right_ob_v[j].X) / 2.0; 
                        coord = make_pair(coord_x,abs(left_ob_v[i].Y - right_ob_v[j].Y) / 2.0);
                        coord_v.push_back(coord);                      
                    }
                }
            }
        }
        else{
            for(int i = 0; i < right_ob_v.size(); i++){
                for(int j = 0; j < left_ob_v.size(); j++){
                    float coord_x = 0;
                    if(i == j){
                        coord_x = (right_ob_v[i].X + left_ob_v[j].X) / 2.0; 
                        coord = make_pair(coord_x,abs(right_ob_v[i].Y - left_ob_v[j].Y) / 2.0);
                        coord_v.push_back(coord);
                    }

                    if(i == right_ob_v.size()-1 && right_ob_v.size() - 1 < j){
                        coord_x = (right_ob_v[i].X + left_ob_v[j].X) / 2.0; 
                        coord = make_pair(coord_x,abs(right_ob_v[i].Y - left_ob_v[j].Y) / 2.0);
                        coord_v.push_back(coord);                       
                    }
                }
            }
        }
    }

    else if(right_ob_v.size() != 0 && left_ob_v.size() == 0){
        std::cout << "[case_2] only find right wall" << std::endl;
        for(int i = 0 ; i < right_ob_v.size(); i++){
            coord = make_pair(right_ob_v[i].X, right_ob_v[i].Y - 0.3);
            coord_v.push_back(coord);         
        }
    }

    else if(right_ob_v.size() == 0 && left_ob_v.size() != 0){
        std::cout << "[case_2] only find left wall" << std::endl;
        for(int i = 0 ; i < left_ob_v.size(); i++){
            coord = make_pair(right_ob_v[i].X, right_ob_v[i].Y + 0.3);
            coord_v.push_back(coord);         
        }
    }

    std::cout << "===========[오른쪽 벡터]==============" << '\n';
    std::cout << "right size : " << right_ob_v.size() << '\n';

    for(int i = 0; i < right_ob_v.size(); i++){
        std::cout << i+1 << "번째 : " << right_ob_v[i].X << right_ob_v[i].Y << '\n';
    }

    std::cout << "====================================" << '\n';



    std::cout << "===========[왼쪽 벡터]==============" << '\n';
    std::cout << "left size : " << left_ob_v.size() << '\n';

    for(int i = 0; i < left_ob_v.size(); i++){
        std::cout << i+1 << "번째 : " << left_ob_v[i].X << left_ob_v[i].Y << '\n';
    }

    std::cout << "====================================" << '\n';



// ======= 개선 필요 ===============
// 현재는 일단 벡터에서 가장 앞의 데이터를 퍼블리시하는 형태
// 추후 벡터내 데이터들을 필요에 따라 가공해야할듯
    geometry_msgs::Point coord_point;

    coord_point.x = coord_v.front().X;
    coord_point.y = coord_v.front().Y;
    coord_point.z = 0.0;

    coord_pub.publish(coord_point);

    ROS_INFO("published");
}

int main(int argc, char **argv){
    ros::init(argc, argv, "sick_narrow");
    ros::NodeHandle nh;

    ros::Subscriber sub = nh.subscribe("/sick_cluster_info", 10, narrow_cb);

    coord_pub = nh.advertise<geometry_msgs::Point>("/narrow_coord", 10);

    ros::spin();

    return 0;
}