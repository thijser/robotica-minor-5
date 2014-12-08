cmake_minimum_required(VERSION 2.8.3)
project(glados)
include_directories(include ${catkin_INCLUDE_DIRS})

find_package(catkin REQUIRED COMPONENTS roscpp message_generation threemxl std_msgs  rospy)

# Generate messages in the 'msg' folder
add_message_files(
   FILES
   music.msg
 )

generate_messages(
   DEPENDENCIES
   std_msgs
 )
#find_package(catkin REQUIRED COMPONENTS message_generation)
#catkin_package()	


catkin_package(
INCLUDE_DIRS include
CATKIN_DEPENDS roscpp std_msgs geometry_msgs message_runtime)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

include_directories(include ${catkin_INCLUDE_DIRS})
include_directories(include ${roscpp_INCLUDE_DIRS})

######
#CORE
######

add_executable(core src/core.cpp)
target_link_libraries(core ${catkin_LIBRARIES})

#######
#MIDLEVEL-LOGIC
#######

#dance 
add_executable(dance src/dancer.cpp)
target_link_libraries(dance ${catkin_LIBRARIES})

#######
#SENSORS
#######

#Switchsensor
add_library(switchSensor src/sensors/switchSensor.cpp)
target_link_libraries(switchSensor ${catkin_LIBRARIES})

#UltrasoneSensor
add_library(UltrasoneSensor src/sensors/UltrasoneSensor.cpp) 		
target_link_libraries(UltrasoneSensor ${catkin_LIBRARIES})

#RFIDSensor
add_library(RFIDSensor src/sensors/RFIDSensor.cpp)
target_link_libraries(RFIDSensor ${catkin_LIBRARIES})

#########
#DRIVERS
#########

#Conveyor
add_library(conveyorDriver src/drivers/conveyorDriver.cpp)
target_link_libraries(conveyorDriver ${roscpp_LIBRARIES} switchSensor)

#Audio
add_library(audioDriver src/drivers/audioDriver.cpp)
target_link_libraries(audioDriver ${catkin_LIBRARIES})

#Launch
add_executable(launchDriver src/drivers/launchDriver.cpp)
target_link_libraries(launchDriver ${roscpp_LIBRARIES} switchSensor)

#Driving
add_executable(drivingDriver src/drivers/drivingDriver.cpp)
target_link_libraries(drivingDriver ${catkin_LIBRARIES})

#######
#LOGIC
#######

#Ball ask
#add_executable(mathAsker src/mathAsker.cpp)
#target_link_libraries(mathAsker ${catkin_LIBRARIES})

######
#Tests
######

#Sensortests
#catkin_add_gtest(sensorutest src/sensors/test/utest.cpp)
#target_link_libraries(sensorutest ${roscpp_LIBRARIES} switchSensor RFIDSensor UltrasoneSensor)

#Drivertests
#catkin_add_gtest(driverutest src/sensors/test/utest.cpp)
#target_link_libraries(driverutest ${roscpp_LIBRARIES} conveyorDriver audioDriver )
#add_executable(driverutest launchDriver.c) #ja iets met libraries die niet gelinked kunnen worden aan executables ofzo...

#catkin_add_gtest(driverutest src/sensors/test/utest.cpp)
#target_link_libraries(driverutest ${roscpp_LIBRARIES} conveyorDriver audioDriver launchDriver)

