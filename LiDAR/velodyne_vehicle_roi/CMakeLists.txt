cmake_minimum_required(VERSION 2.8.3)
project(velodyne_vehicle_roi)

## Find catkin macros and libraries
## if COMPONENTS list like find_package(catkin REQUIRED COMPONENTS xyz)
## is used, also find other catkin packages
find_package(catkin REQUIRED COMPONENTS
  dynamic_reconfigure
  nav_msgs
  ##pcl
  pcl_ros
  roscpp
)
#find_package(CUDA REQUIRED)
#add dynamic reconfigure api

find_package(PCL 1.8 REQUIRED)
catkin_package(
#  INCLUDE_DIRS include
#  LIBRARIES cloud_to_map
#  CATKIN_DEPENDS dynamic_reconfigure nav_msgs pcl pcl_ros roscpp
#  DEPENDS system_lib
)

###########
## Build ##
###########

## Specify additional locations of header files
## Your package locations should be listed before other locations
# include_directories(include)
include_directories(
  ${catkin_INCLUDE_DIRS}
  ${OpenCV_INCLUDE_DIRS}
  ${PCL_INCLUDE_DIRS}
  #${CUDA_INCLUDE_DIRS}
)
link_directories(${PCL_LIBRARY_DIRS})
add_definitions(${PCL_DEFINITIONS})

add_executable(velodyne_vehicle_roi src/velodyne_vehicle_roi.cpp)
add_dependencies(velodyne_vehicle_roi ${PROJECT_NAME}_gencfg)
target_link_libraries(velodyne_vehicle_roi ${catkin_LIBRARIES} ${PCL_LIBRARIES})


