## spider_gazebo

ROS package providing Gazebo simulation of the Phantom X Hexapod robot.
Also provides a Python interface to the joints and some walk capabilities.

These have been tested in simulation and need some work to be used on the real robot, do not use as-is.


## Install

Clone in your catkin workspace and catkin_make it.
    
## Usage

You can launch the simulation with:

    roslaunch spider_gazebo spider_gazebo.launch
    
PRESS PLAY IN GAZEBO ONLY WHEN EVERYTHING IS LOADED (wait for controllers)


## ROS API

All topics are provided in the /spider namespace.

Sensors:

    /spider/joint_states

Actuators (radians for position control, arbitrary normalized speed for cmd_vel):



