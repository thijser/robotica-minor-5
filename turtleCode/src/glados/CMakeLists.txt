cmake_minimum_required(VERSION 2.8.3)
project(glados)
include_directories(include ${catkin_INCLUDE_DIRS})
find_package(catkin REQUIRED COMPONENTS roscpp std_msgs)


catkin_package(
INCLUDE_DIRS include
CATKIN_DEPENDS roscpp std_msgs geometry_msgs
)
set(CMAKE_CXX_FLAGS "-std=c++0x ${CMAKE_CXX_FLAGS}")
add_executable(core src/core.cpp)
target_link_libraries(core ${catkin_LIBRARIES})

#AUDIO
add_executable(sound_player src/audio/soundPlayer.cpp)
target_link_libraries(sound_player ${catkin_LIBRARIES})