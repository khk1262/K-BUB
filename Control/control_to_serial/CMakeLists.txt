cmake_minimum_required(VERSION 3.0.2)
project(control_to_serial)

find_package(catkin REQUIRED COMPONENTS
  message_generation
  roscpp
  rospy
  serial
  std_msgs
)


add_message_files(FILES control2serial.msg)

generate_messages(DEPENDENCIES std_msgs)

catkin_package(CATKIN_DEPENDS message_runtime roscpp rospy serial std_msgs)




include_directories(${catkin_INCLUDE_DIRS})

add_executable(Serial src/Serial.cpp)
add_dependencies(Serial ${${PROJECT_NAME}_EXPORTED_TARGEETS} ${catkin_EXPORTED_TARGETS} )
target_link_libraries(Serial ${catkin_LIBRARIES})
