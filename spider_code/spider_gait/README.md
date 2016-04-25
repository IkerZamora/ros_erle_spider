SPIDER_GAIT
======

This package abstract the spider movements when it is moving.

It receives messages containing the type of gait desired, direction, velocity and step size, and generates the position vectors of all the legs to implement two types of gait (ripple/tripod).

How it works
-----

When the spider is walking, it's reads `/teleop/gait_control` topic, calls to the service for kinematic calculations that offers `spider_leg_kinematics`. And finally write on `/joints_to_controller` to move servos or visualize with RVIZ.

Input
-----

topic `/teleop/gait_control`

 * msg type: [GaitCommand](https://github.com/erlerobot/erle-spider/blob/spider/software/src/spider_msgs/msg/GaitCommand.msg)

 * publishers: [/spider_teleop_joy](https://github.com/erlerobot/erle-spider/tree/master/software/src/spider_teleop_joy)

Service
-----

service `/spider_leg_kinematics/get_ik`

 * srv type: [spider_msgs/GetLegIKSolver](https://github.com/erlerobot/erle-spider/blob/spider/software/src/spider_msgs/srv/GetLegIKSolver.srv)

 * advertiser: [spider_leg_kinematics](https://github.com/erlerobot/erle-spider/tree/master/software/src/spider_leg_kinematics) 

Output
------

topic `/joints_to_controller`

 * msg type: [LegsJointsState](https://github.com/erlerobot/erle-spider/blob/spider/software/src/spider_msgs/msg/LegsJointsState.msg)

 * subscribers: [/spider_joint_publisher](https://github.com/erlerobot/erle-spider/tree/master/software/src/spider_joint_publisher) and [/spider_maestro_controller](https://github.com/erlerobot/erle-spider/tree/master/software/src/spider_maestro_controller)

Graph
----

![alt tag](../../../docs/img/rqt_graph_captures/gait.png?raw=true)

Details
----

**rosnode info /spider_gait**

```
--------------------------------------------------------------------------------
Node [/spider_gait]
Publications: 
 * /joints_to_controller [spider_msgs/LegsJointsState]
 * /rosout [rosgraph_msgs/Log]

Subscriptions: 
 * /teleop/gait_control [spider_msgs/GaitCommand]

Services: 
 * /spider_gait/get_loggers
 * /spider_gait/set_logger_level


contacting node http://xabi-XPS-13-9343:44378/ ...
Pid: 24528
Connections:
 * topic: /rosout
    * to: /rosout
    * direction: outbound
    * transport: TCPROS
 * topic: /joints_to_controller
    * to: /spider_joint_publisher
    * direction: outbound
    * transport: TCPROS
 * topic: /joints_to_controller
    * to: /spider_maestro_controller
    * direction: outbound
    * transport: TCPROS
 * topic: /teleop/gait_control
    * to: /spider_teleop_joy (http://xabi-XPS-13-9343:44007/)
    * direction: inbound
    * transport: TCPROS

```
