cmake_minimum_required(VERSION 2.8.3)
project(glados)
include_directories(include ${catkin_INCLUDE_DIRS})
find_package(catkin REQUIRED COMPONENTS roscpp std_msgs)

catkin_package(
INCLUDE_DIRS include
CATKIN_DEPENDS roscpp std_msgs geometry_msgs)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

include_directories(include ${catkin_INCLUDE_DIRS})
include_directories(include ${roscpp_INCLUDE_DIRS})
######
#CORE
######

add_executable(core src/core.cpp)
target_link_libraries(core ${catkin_LIBRARIES})

#######
#LOGIC
#######

#Ball ask
add_executable(mathAsker src/mathAsker.cpp)
target_link_libraries(mathAsker ${catkin_LIBRARIES})

#########
#DRIVERS
#########

#Switchsensor
add_library(switchSensor src/drivers/switchSensor.cpp)
target_link_libraries(switchSensor ${catkin_LIBRARIES})

#Conveyor
add_library(conveyorDriver src/drivers/conveyorDriver.cpp)
target_link_libraries(conveyorDriver ${roscpp_LIBRARIES} switchSensor)

#Audio
add_executable(sound_player src/drivers/audioDriver.cpp)
target_link_libraries(sound_player ${catkin_LIBRARIES})