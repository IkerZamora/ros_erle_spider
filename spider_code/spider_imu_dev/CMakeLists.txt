cmake_minimum_required(VERSION 2.8.3)
project(spider_imu_dev)

find_package(catkin REQUIRED COMPONENTS
	roscpp
#	crab_msgs
#	genmsg
#    rospy
#    std_msgs
)

#find_package(Boost REQUIRED COMPONENTS thread)



## Declare ROS messages and services
#add_message_files(DEPENDENCIES crab_msgs DIRECTORY msg FILES apm_imu.msg)


#generate_messages(DEPENDENCIES crab_msgs)


catkin_package()


include_directories(include src/IMU ${catkin_INCLUDE_DIRS})

add_library(imu		 	src/IMU/DCM.cpp
				src/IMU/Matrix.cpp
				src/IMU/Vector.cpp)
target_link_libraries(imu ${catkin_LIBRARIES})



add_executable(Imu_control src/Imu_control.cpp)
target_link_libraries(Imu_control imu ${catkin_LIBRARIES})
#add_dependencies(Imu_control spider_imu_dev_generate_messages_cpp)

#install(
#	TARGETS Imu_control
#	ARCHIVE DESTINATION ${CATKIN_PACKAGE_LIB_DESTINATION}
#	LIBRARY DESTINATION ${CATKIN_PACKAGE_LIB_DESTINATION}
#	RUNTIME DESTINATION ${CATKIN_PACKAGE_BIN_DESTINATION}
#	)