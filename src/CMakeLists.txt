cmake_minimum_required(VERSION 2.8.3)
project(mocap)
find_package(catkin REQUIRED COMPONENTS
    libmavconn
    eigen_conversions
    geometry_msgs
    std_msgs
    tf2_eigen
)

find_package(Boost REQUIRED COMPONENTS
	system
	thread
)

message(WARNING "asdf ${mavlink_INCLUDE_DIRS}")
include_directories(
    ${catkin_INCLUDE_DIRS}
    ${mavlink_INCLUDE_DIRS}
)
SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -pthread")

add_library(UDP STATIC UDPclient.cpp)
target_link_libraries(UDP
    ${Boost_LIBRARIES}
    ${catkin_LIBRARIES}
)

add_executable(test test.cpp)
message(WARNING ${Boost_LIBRARIES})
target_link_libraries(test
    ${Boost_LIBRARIES}
    ${catkin_LIBRARIES}
    ${UDP_LIBRARIES}
)



add_executable(sender sender.cpp)
target_link_libraries(sender
    ${Boost_LIBRARIES}
    ${catkin_LIBRARIES}
    ${UDP_LIBRARIES}
)







