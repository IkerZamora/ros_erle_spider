SPIDER_JOINT_PUBLISHER
======

This package + [joint_state_publisher](https://github.com/ros/robot_model/tree/indigo-devel/joint_state_publisher) + [robot_state_publisher](https://github.com/ros/robot_state_publisher.git) are used for visualization in RVIZ.

How it works
-----

They receives data from the topic `/joints_to_controller`, that contains the position desired for each servo, and between the three make possible the visualization of the spider's model in RVIZ.

Input
-----

topic `/joints_to_controller`

 * msg type: [LegsJointsState](https://github.com/erlerobot/erle-spider/blob/spider/software/src/spider_msgs/msg/LegsJointsState.msg)

 * publishers: [/spider_body_kinematics](https://github.com/erlerobot/erle-spider/tree/master/software/src/spider_body_kinematics) and [/spider_gait](https://github.com/erlerobot/erle-spider/tree/master/software/src/spider_gait)


Output (of the 3 package)
------

![alt tag](http://robocraft.ru/uploads/images/1/5/4/d/3356/6ccf919733.png)

Graph
----

![alt tag](../../../docs/img/rqt_graph_captures/rviz.png?raw=true)

Details
----

**rosnode info /spider_joint_publisher**

```
--------------------------------------------------------------------------------
Node [/spider_joint_publisher]
Publications: 
 * /rosout [rosgraph_msgs/Log]
 * /spider_joint_publisher [sensor_msgs/JointState]

Subscriptions: 
 * /joints_to_controller [spider_msgs/LegsJointsState]

Services: 
 * /spider_joint_publisher/set_logger_level
 * /spider_joint_publisher/get_loggers


contacting node http://xabi-XPS-13-9343:47184/ ...
Pid: 18998
Connections:
 * topic: /rosout
    * to: /rosout
    * direction: outbound
    * transport: TCPROS
 * topic: /spider_joint_publisher
    * to: /joint_state_publisher
    * direction: outbound
    * transport: TCPROS
 * topic: /joints_to_controller
    * to: /spider_body_kinematics (http://xabi-XPS-13-9343:33757/)
    * direction: inbound
    * transport: TCPROS
 * topic: /joints_to_controller
    * to: /spider_gait (http://xabi-XPS-13-9343:59382/)
    * direction: inbound
    * transport: TCPROS

```

**rosnode info /joint_state_publisher**

```
--------------------------------------------------------------------------------
Node [/joint_state_publisher]
Publications: 
 * /joint_states [sensor_msgs/JointState]
 * /rosout [rosgraph_msgs/Log]

Subscriptions: 
 * /spider_joint_publisher [sensor_msgs/JointState]

Services: 
 * /joint_state_publisher/set_logger_level
 * /joint_state_publisher/get_loggers


contacting node http://xabi-XPS-13-9343:46348/ ...
Pid: 18997
Connections:
 * topic: /joint_states
    * to: /robot_state_publisher
    * direction: outbound
    * transport: TCPROS
 * topic: /rosout
    * to: /rosout
    * direction: outbound
    * transport: TCPROS
 * topic: /spider_joint_publisher
    * to: /spider_joint_publisher (http://xabi-XPS-13-9343:47184/)
    * direction: inbound
    * transport: TCPROS

```

**rosnode info /robot_state_publisher**

```
--------------------------------------------------------------------------------
Node [/robot_state_publisher]
Publications: 
 * /rosout [rosgraph_msgs/Log]
 * /tf [tf2_msgs/TFMessage]

Subscriptions: 
 * /joint_states [sensor_msgs/JointState]

Services: 
 * /robot_state_publisher/get_loggers
 * /robot_state_publisher/set_logger_level


contacting node http://xabi-XPS-13-9343:36916/ ...
Pid: 19015
Connections:
 * topic: /rosout
    * to: /rosout
    * direction: outbound
    * transport: TCPROS
 * topic: /tf
    * to: /rviz
    * direction: outbound
    * transport: TCPROS
 * topic: /joint_states
    * to: /joint_state_publisher (http://xabi-XPS-13-9343:46348/)
    * direction: inbound
    * transport: TCPROS

```
