# generated from genmsg/cmake/pkg-genmsg.cmake.em

message(STATUS "velodyne_filter: 2 messages, 0 services")

set(MSG_I_FLAGS "-Ivelodyne_filter:/home/khk1262/catkin_ws/src/velodyne_filter/msg;-Istd_msgs:/opt/ros/melodic/share/std_msgs/cmake/../msg;-Igeometry_msgs:/opt/ros/melodic/share/geometry_msgs/cmake/../msg;-Isensor_msgs:/opt/ros/melodic/share/sensor_msgs/cmake/../msg")

# Find all generators
find_package(gencpp REQUIRED)
find_package(geneus REQUIRED)
find_package(genlisp REQUIRED)
find_package(gennodejs REQUIRED)
find_package(genpy REQUIRED)

add_custom_target(velodyne_filter_generate_messages ALL)

# verify that message/service dependencies have not changed since configure



get_filename_component(_filename "/home/khk1262/catkin_ws/src/velodyne_filter/msg/ParkPoint.msg" NAME_WE)
add_custom_target(_velodyne_filter_generate_messages_check_deps_${_filename}
  COMMAND ${CATKIN_ENV} ${PYTHON_EXECUTABLE} ${GENMSG_CHECK_DEPS_SCRIPT} "velodyne_filter" "/home/khk1262/catkin_ws/src/velodyne_filter/msg/ParkPoint.msg" "geometry_msgs/Point:std_msgs/Bool:std_msgs/Header"
)

get_filename_component(_filename "/home/khk1262/catkin_ws/src/velodyne_filter/msg/PointArray.msg" NAME_WE)
add_custom_target(_velodyne_filter_generate_messages_check_deps_${_filename}
  COMMAND ${CATKIN_ENV} ${PYTHON_EXECUTABLE} ${GENMSG_CHECK_DEPS_SCRIPT} "velodyne_filter" "/home/khk1262/catkin_ws/src/velodyne_filter/msg/PointArray.msg" "geometry_msgs/Point:std_msgs/Header"
)

#
#  langs = gencpp;geneus;genlisp;gennodejs;genpy
#

### Section generating for lang: gencpp
### Generating Messages
_generate_msg_cpp(velodyne_filter
  "/home/khk1262/catkin_ws/src/velodyne_filter/msg/ParkPoint.msg"
  "${MSG_I_FLAGS}"
  "/opt/ros/melodic/share/geometry_msgs/cmake/../msg/Point.msg;/opt/ros/melodic/share/std_msgs/cmake/../msg/Bool.msg;/opt/ros/melodic/share/std_msgs/cmake/../msg/Header.msg"
  ${CATKIN_DEVEL_PREFIX}/${gencpp_INSTALL_DIR}/velodyne_filter
)
_generate_msg_cpp(velodyne_filter
  "/home/khk1262/catkin_ws/src/velodyne_filter/msg/PointArray.msg"
  "${MSG_I_FLAGS}"
  "/opt/ros/melodic/share/geometry_msgs/cmake/../msg/Point.msg;/opt/ros/melodic/share/std_msgs/cmake/../msg/Header.msg"
  ${CATKIN_DEVEL_PREFIX}/${gencpp_INSTALL_DIR}/velodyne_filter
)

### Generating Services

### Generating Module File
_generate_module_cpp(velodyne_filter
  ${CATKIN_DEVEL_PREFIX}/${gencpp_INSTALL_DIR}/velodyne_filter
  "${ALL_GEN_OUTPUT_FILES_cpp}"
)

add_custom_target(velodyne_filter_generate_messages_cpp
  DEPENDS ${ALL_GEN_OUTPUT_FILES_cpp}
)
add_dependencies(velodyne_filter_generate_messages velodyne_filter_generate_messages_cpp)

# add dependencies to all check dependencies targets
get_filename_component(_filename "/home/khk1262/catkin_ws/src/velodyne_filter/msg/ParkPoint.msg" NAME_WE)
add_dependencies(velodyne_filter_generate_messages_cpp _velodyne_filter_generate_messages_check_deps_${_filename})
get_filename_component(_filename "/home/khk1262/catkin_ws/src/velodyne_filter/msg/PointArray.msg" NAME_WE)
add_dependencies(velodyne_filter_generate_messages_cpp _velodyne_filter_generate_messages_check_deps_${_filename})

# target for backward compatibility
add_custom_target(velodyne_filter_gencpp)
add_dependencies(velodyne_filter_gencpp velodyne_filter_generate_messages_cpp)

# register target for catkin_package(EXPORTED_TARGETS)
list(APPEND ${PROJECT_NAME}_EXPORTED_TARGETS velodyne_filter_generate_messages_cpp)

### Section generating for lang: geneus
### Generating Messages
_generate_msg_eus(velodyne_filter
  "/home/khk1262/catkin_ws/src/velodyne_filter/msg/ParkPoint.msg"
  "${MSG_I_FLAGS}"
  "/opt/ros/melodic/share/geometry_msgs/cmake/../msg/Point.msg;/opt/ros/melodic/share/std_msgs/cmake/../msg/Bool.msg;/opt/ros/melodic/share/std_msgs/cmake/../msg/Header.msg"
  ${CATKIN_DEVEL_PREFIX}/${geneus_INSTALL_DIR}/velodyne_filter
)
_generate_msg_eus(velodyne_filter
  "/home/khk1262/catkin_ws/src/velodyne_filter/msg/PointArray.msg"
  "${MSG_I_FLAGS}"
  "/opt/ros/melodic/share/geometry_msgs/cmake/../msg/Point.msg;/opt/ros/melodic/share/std_msgs/cmake/../msg/Header.msg"
  ${CATKIN_DEVEL_PREFIX}/${geneus_INSTALL_DIR}/velodyne_filter
)

### Generating Services

### Generating Module File
_generate_module_eus(velodyne_filter
  ${CATKIN_DEVEL_PREFIX}/${geneus_INSTALL_DIR}/velodyne_filter
  "${ALL_GEN_OUTPUT_FILES_eus}"
)

add_custom_target(velodyne_filter_generate_messages_eus
  DEPENDS ${ALL_GEN_OUTPUT_FILES_eus}
)
add_dependencies(velodyne_filter_generate_messages velodyne_filter_generate_messages_eus)

# add dependencies to all check dependencies targets
get_filename_component(_filename "/home/khk1262/catkin_ws/src/velodyne_filter/msg/ParkPoint.msg" NAME_WE)
add_dependencies(velodyne_filter_generate_messages_eus _velodyne_filter_generate_messages_check_deps_${_filename})
get_filename_component(_filename "/home/khk1262/catkin_ws/src/velodyne_filter/msg/PointArray.msg" NAME_WE)
add_dependencies(velodyne_filter_generate_messages_eus _velodyne_filter_generate_messages_check_deps_${_filename})

# target for backward compatibility
add_custom_target(velodyne_filter_geneus)
add_dependencies(velodyne_filter_geneus velodyne_filter_generate_messages_eus)

# register target for catkin_package(EXPORTED_TARGETS)
list(APPEND ${PROJECT_NAME}_EXPORTED_TARGETS velodyne_filter_generate_messages_eus)

### Section generating for lang: genlisp
### Generating Messages
_generate_msg_lisp(velodyne_filter
  "/home/khk1262/catkin_ws/src/velodyne_filter/msg/ParkPoint.msg"
  "${MSG_I_FLAGS}"
  "/opt/ros/melodic/share/geometry_msgs/cmake/../msg/Point.msg;/opt/ros/melodic/share/std_msgs/cmake/../msg/Bool.msg;/opt/ros/melodic/share/std_msgs/cmake/../msg/Header.msg"
  ${CATKIN_DEVEL_PREFIX}/${genlisp_INSTALL_DIR}/velodyne_filter
)
_generate_msg_lisp(velodyne_filter
  "/home/khk1262/catkin_ws/src/velodyne_filter/msg/PointArray.msg"
  "${MSG_I_FLAGS}"
  "/opt/ros/melodic/share/geometry_msgs/cmake/../msg/Point.msg;/opt/ros/melodic/share/std_msgs/cmake/../msg/Header.msg"
  ${CATKIN_DEVEL_PREFIX}/${genlisp_INSTALL_DIR}/velodyne_filter
)

### Generating Services

### Generating Module File
_generate_module_lisp(velodyne_filter
  ${CATKIN_DEVEL_PREFIX}/${genlisp_INSTALL_DIR}/velodyne_filter
  "${ALL_GEN_OUTPUT_FILES_lisp}"
)

add_custom_target(velodyne_filter_generate_messages_lisp
  DEPENDS ${ALL_GEN_OUTPUT_FILES_lisp}
)
add_dependencies(velodyne_filter_generate_messages velodyne_filter_generate_messages_lisp)

# add dependencies to all check dependencies targets
get_filename_component(_filename "/home/khk1262/catkin_ws/src/velodyne_filter/msg/ParkPoint.msg" NAME_WE)
add_dependencies(velodyne_filter_generate_messages_lisp _velodyne_filter_generate_messages_check_deps_${_filename})
get_filename_component(_filename "/home/khk1262/catkin_ws/src/velodyne_filter/msg/PointArray.msg" NAME_WE)
add_dependencies(velodyne_filter_generate_messages_lisp _velodyne_filter_generate_messages_check_deps_${_filename})

# target for backward compatibility
add_custom_target(velodyne_filter_genlisp)
add_dependencies(velodyne_filter_genlisp velodyne_filter_generate_messages_lisp)

# register target for catkin_package(EXPORTED_TARGETS)
list(APPEND ${PROJECT_NAME}_EXPORTED_TARGETS velodyne_filter_generate_messages_lisp)

### Section generating for lang: gennodejs
### Generating Messages
_generate_msg_nodejs(velodyne_filter
  "/home/khk1262/catkin_ws/src/velodyne_filter/msg/ParkPoint.msg"
  "${MSG_I_FLAGS}"
  "/opt/ros/melodic/share/geometry_msgs/cmake/../msg/Point.msg;/opt/ros/melodic/share/std_msgs/cmake/../msg/Bool.msg;/opt/ros/melodic/share/std_msgs/cmake/../msg/Header.msg"
  ${CATKIN_DEVEL_PREFIX}/${gennodejs_INSTALL_DIR}/velodyne_filter
)
_generate_msg_nodejs(velodyne_filter
  "/home/khk1262/catkin_ws/src/velodyne_filter/msg/PointArray.msg"
  "${MSG_I_FLAGS}"
  "/opt/ros/melodic/share/geometry_msgs/cmake/../msg/Point.msg;/opt/ros/melodic/share/std_msgs/cmake/../msg/Header.msg"
  ${CATKIN_DEVEL_PREFIX}/${gennodejs_INSTALL_DIR}/velodyne_filter
)

### Generating Services

### Generating Module File
_generate_module_nodejs(velodyne_filter
  ${CATKIN_DEVEL_PREFIX}/${gennodejs_INSTALL_DIR}/velodyne_filter
  "${ALL_GEN_OUTPUT_FILES_nodejs}"
)

add_custom_target(velodyne_filter_generate_messages_nodejs
  DEPENDS ${ALL_GEN_OUTPUT_FILES_nodejs}
)
add_dependencies(velodyne_filter_generate_messages velodyne_filter_generate_messages_nodejs)

# add dependencies to all check dependencies targets
get_filename_component(_filename "/home/khk1262/catkin_ws/src/velodyne_filter/msg/ParkPoint.msg" NAME_WE)
add_dependencies(velodyne_filter_generate_messages_nodejs _velodyne_filter_generate_messages_check_deps_${_filename})
get_filename_component(_filename "/home/khk1262/catkin_ws/src/velodyne_filter/msg/PointArray.msg" NAME_WE)
add_dependencies(velodyne_filter_generate_messages_nodejs _velodyne_filter_generate_messages_check_deps_${_filename})

# target for backward compatibility
add_custom_target(velodyne_filter_gennodejs)
add_dependencies(velodyne_filter_gennodejs velodyne_filter_generate_messages_nodejs)

# register target for catkin_package(EXPORTED_TARGETS)
list(APPEND ${PROJECT_NAME}_EXPORTED_TARGETS velodyne_filter_generate_messages_nodejs)

### Section generating for lang: genpy
### Generating Messages
_generate_msg_py(velodyne_filter
  "/home/khk1262/catkin_ws/src/velodyne_filter/msg/ParkPoint.msg"
  "${MSG_I_FLAGS}"
  "/opt/ros/melodic/share/geometry_msgs/cmake/../msg/Point.msg;/opt/ros/melodic/share/std_msgs/cmake/../msg/Bool.msg;/opt/ros/melodic/share/std_msgs/cmake/../msg/Header.msg"
  ${CATKIN_DEVEL_PREFIX}/${genpy_INSTALL_DIR}/velodyne_filter
)
_generate_msg_py(velodyne_filter
  "/home/khk1262/catkin_ws/src/velodyne_filter/msg/PointArray.msg"
  "${MSG_I_FLAGS}"
  "/opt/ros/melodic/share/geometry_msgs/cmake/../msg/Point.msg;/opt/ros/melodic/share/std_msgs/cmake/../msg/Header.msg"
  ${CATKIN_DEVEL_PREFIX}/${genpy_INSTALL_DIR}/velodyne_filter
)

### Generating Services

### Generating Module File
_generate_module_py(velodyne_filter
  ${CATKIN_DEVEL_PREFIX}/${genpy_INSTALL_DIR}/velodyne_filter
  "${ALL_GEN_OUTPUT_FILES_py}"
)

add_custom_target(velodyne_filter_generate_messages_py
  DEPENDS ${ALL_GEN_OUTPUT_FILES_py}
)
add_dependencies(velodyne_filter_generate_messages velodyne_filter_generate_messages_py)

# add dependencies to all check dependencies targets
get_filename_component(_filename "/home/khk1262/catkin_ws/src/velodyne_filter/msg/ParkPoint.msg" NAME_WE)
add_dependencies(velodyne_filter_generate_messages_py _velodyne_filter_generate_messages_check_deps_${_filename})
get_filename_component(_filename "/home/khk1262/catkin_ws/src/velodyne_filter/msg/PointArray.msg" NAME_WE)
add_dependencies(velodyne_filter_generate_messages_py _velodyne_filter_generate_messages_check_deps_${_filename})

# target for backward compatibility
add_custom_target(velodyne_filter_genpy)
add_dependencies(velodyne_filter_genpy velodyne_filter_generate_messages_py)

# register target for catkin_package(EXPORTED_TARGETS)
list(APPEND ${PROJECT_NAME}_EXPORTED_TARGETS velodyne_filter_generate_messages_py)



if(gencpp_INSTALL_DIR AND EXISTS ${CATKIN_DEVEL_PREFIX}/${gencpp_INSTALL_DIR}/velodyne_filter)
  # install generated code
  install(
    DIRECTORY ${CATKIN_DEVEL_PREFIX}/${gencpp_INSTALL_DIR}/velodyne_filter
    DESTINATION ${gencpp_INSTALL_DIR}
  )
endif()
if(TARGET std_msgs_generate_messages_cpp)
  add_dependencies(velodyne_filter_generate_messages_cpp std_msgs_generate_messages_cpp)
endif()
if(TARGET geometry_msgs_generate_messages_cpp)
  add_dependencies(velodyne_filter_generate_messages_cpp geometry_msgs_generate_messages_cpp)
endif()
if(TARGET sensor_msgs_generate_messages_cpp)
  add_dependencies(velodyne_filter_generate_messages_cpp sensor_msgs_generate_messages_cpp)
endif()

if(geneus_INSTALL_DIR AND EXISTS ${CATKIN_DEVEL_PREFIX}/${geneus_INSTALL_DIR}/velodyne_filter)
  # install generated code
  install(
    DIRECTORY ${CATKIN_DEVEL_PREFIX}/${geneus_INSTALL_DIR}/velodyne_filter
    DESTINATION ${geneus_INSTALL_DIR}
  )
endif()
if(TARGET std_msgs_generate_messages_eus)
  add_dependencies(velodyne_filter_generate_messages_eus std_msgs_generate_messages_eus)
endif()
if(TARGET geometry_msgs_generate_messages_eus)
  add_dependencies(velodyne_filter_generate_messages_eus geometry_msgs_generate_messages_eus)
endif()
if(TARGET sensor_msgs_generate_messages_eus)
  add_dependencies(velodyne_filter_generate_messages_eus sensor_msgs_generate_messages_eus)
endif()

if(genlisp_INSTALL_DIR AND EXISTS ${CATKIN_DEVEL_PREFIX}/${genlisp_INSTALL_DIR}/velodyne_filter)
  # install generated code
  install(
    DIRECTORY ${CATKIN_DEVEL_PREFIX}/${genlisp_INSTALL_DIR}/velodyne_filter
    DESTINATION ${genlisp_INSTALL_DIR}
  )
endif()
if(TARGET std_msgs_generate_messages_lisp)
  add_dependencies(velodyne_filter_generate_messages_lisp std_msgs_generate_messages_lisp)
endif()
if(TARGET geometry_msgs_generate_messages_lisp)
  add_dependencies(velodyne_filter_generate_messages_lisp geometry_msgs_generate_messages_lisp)
endif()
if(TARGET sensor_msgs_generate_messages_lisp)
  add_dependencies(velodyne_filter_generate_messages_lisp sensor_msgs_generate_messages_lisp)
endif()

if(gennodejs_INSTALL_DIR AND EXISTS ${CATKIN_DEVEL_PREFIX}/${gennodejs_INSTALL_DIR}/velodyne_filter)
  # install generated code
  install(
    DIRECTORY ${CATKIN_DEVEL_PREFIX}/${gennodejs_INSTALL_DIR}/velodyne_filter
    DESTINATION ${gennodejs_INSTALL_DIR}
  )
endif()
if(TARGET std_msgs_generate_messages_nodejs)
  add_dependencies(velodyne_filter_generate_messages_nodejs std_msgs_generate_messages_nodejs)
endif()
if(TARGET geometry_msgs_generate_messages_nodejs)
  add_dependencies(velodyne_filter_generate_messages_nodejs geometry_msgs_generate_messages_nodejs)
endif()
if(TARGET sensor_msgs_generate_messages_nodejs)
  add_dependencies(velodyne_filter_generate_messages_nodejs sensor_msgs_generate_messages_nodejs)
endif()

if(genpy_INSTALL_DIR AND EXISTS ${CATKIN_DEVEL_PREFIX}/${genpy_INSTALL_DIR}/velodyne_filter)
  install(CODE "execute_process(COMMAND \"/usr/bin/python2\" -m compileall \"${CATKIN_DEVEL_PREFIX}/${genpy_INSTALL_DIR}/velodyne_filter\")")
  # install generated code
  install(
    DIRECTORY ${CATKIN_DEVEL_PREFIX}/${genpy_INSTALL_DIR}/velodyne_filter
    DESTINATION ${genpy_INSTALL_DIR}
  )
endif()
if(TARGET std_msgs_generate_messages_py)
  add_dependencies(velodyne_filter_generate_messages_py std_msgs_generate_messages_py)
endif()
if(TARGET geometry_msgs_generate_messages_py)
  add_dependencies(velodyne_filter_generate_messages_py geometry_msgs_generate_messages_py)
endif()
if(TARGET sensor_msgs_generate_messages_py)
  add_dependencies(velodyne_filter_generate_messages_py sensor_msgs_generate_messages_py)
endif()
