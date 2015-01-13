cmake_minimum_required(VERSION 2.8.3)
project(glados)
#include_directories(include ${catkin_INCLUDE_DIRS})

find_package(catkin REQUIRED COMPONENTS roscpp message_generation threemxl std_msgs geometry_msgs rospy)
#find_package(catkin REQUIRED COMPONENTS roscpp message_generation threemxl std_msgs  rospy)

# Generate messages in the 'msg' folder
add_message_files(
   FILES
   #music.msg
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

#include_directories(include ${catkin_INCLUDE_DIRS})
#include_directories(include ${roscpp_INCLUDE_DIRS})

######
#CORE
######

#add_executable(newCore src/newCore.cpp)
#target_link_libraries(newCore ${catkin_LIBRARIES})

#######
#MIDLEVEL-LOGIC
#######

#dance 
#add_executable(dance src/dancer.cpp)
#target_link_libraries(dance ${catkin_LIBRARIES})

#launchManager
add_executable(launchManager src/launchManager.cpp)
target_link_libraries(launchManager ${catkin_LIBRARIES})

#######
#SENSORS
#######

#Switchsensor
add_executable(switchSensor src/sensors/switchSensor.cpp)
target_link_libraries(switchSensor ${catkin_LIBRARIES})

#EndSwitchSensor
add_executable(endSwitchSensor src/sensors/endSwitchSensor.cpp)
target_link_libraries(endSwitchSensor ${catkin_LIBRARIES})

#UltrasoneSensor
#add_library(UltrasoneSensor src/sensors/UltrasoneSensor.cpp) 		
#target_link_libraries(UltrasoneSensor ${catkin_LIBRARIES})

#########
#DRIVERS
#########
#serial
add_executable(serial src/drivers/serialListener)
target_link_libraries(serial ${catkin_LIBRARIES})

#Conveyor
add_executable(conveyorDriver src/drivers/conveyorDriver.cpp)
target_link_libraries(conveyorDriver ${catkin_LIBRARIES})

#Audio
add_library(audioDriver src/drivers/audioDriver.cpp)
target_link_libraries(audioDriver ${catkin_LIBRARIES})

#Launch
add_executable(launchDriver src/drivers/launchDriver.cpp)
target_link_libraries(launchDriver ${roscpp_LIBRARIES} )

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

