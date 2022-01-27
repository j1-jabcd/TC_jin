cmake_minimum_required(VERSION 3.10.2)
project(car_planner)
set (CMAKE_CXX_STANDARD 11)
set(CMAKE_BUILD_TYPE "Release")
#ADD_COMPILE_OPTIONS(-std=c++11 )

#set(CMAKE_CXX_FLAGS_RELEASE "-O3 -Wall -g")



find_package(OpenCV REQUIRED)
find_package(Eigen3 REQUIRED)
find_package(PCL  REQUIRED)
find_package(OMPL REQUIRED)
find_package(Boost REQUIRED COMPONENTS system filesystem)
find_package(catkin REQUIRED COMPONENTS
  roscpp
  roslib
  rospy
  std_msgs
  visualization_msgs
  cv_bridge
  geometry_msgs
sensor_msgs
)
include_directories(${OMPL_INCLUDE_DIRS})#
link_directories(${OMPL_LIBRARY_DIRS})
add_definitions(${OMPL_DEFINITIONS})
catkin_package(
 INCLUDE_DIRS include
 DEPENDS OpenCV Eigen Boost
)
include_directories( 
    SYSTEM 
    include 
    ${PCL_INCLUDE_DIRS}
    ${catkin_INCLUDE_DIRS} 
    ${PROJECT_SOURCE_DIR}/include
    ${Eigen_INCLUDE_DIRS}
    ${Eigen3_INCLUDE_DIRS} 

)
find_package(Boost)
add_executable(jlc_node src/keyboard.cpp)
#add_library( car_search 
    #src/car_search.cpp
    #)
#target_link_libraries( car_search
 #   ${catkin_LIBRARIES}
  #  ompl
#)
target_link_libraries(jlc_node ${catkin_LIBRARIES} ${Boost_LIBRARIES})
add_executable(car_node
    src/car_keyboard_control.cpp
)
target_link_libraries(car_node
  ${PCL_LIBRARIES}
  ${catkin_LIBRARIES}
  #car_search
)


# add_library()
