cmake_minimum_required(VERSION 2.8.3)
project(demos)

find_package(catkin REQUIRED)

catkin_package()

install(PROGRAMS src/demo1.py 
  DESTINATION ${CATKIN_PACKAGE_BIN_DESTINATION}
)
