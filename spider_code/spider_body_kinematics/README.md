SPIDER_BODY_KINEMATICS
======

This package abstract the spider movements when it is in a static position.

Calculates the position vectors of all the feet, based on the robot model and data from the message that contains the desired linear and angular deviations from the initial position of the frame, as well as the radius of the opening leg. Also realized the team lifting of the starting position (when the robot is in the frame, and the legs are folded) and return to the starting position.

How it works
-----

When the IMU is active or you are doing behaviours (L1 or  R1), it reads `/teleop/move_body` topic, calls to the service for kinematic calculations that offers `spider_leg_kinematics`. And finally write on `/joints_to_controller` to move servos or visualize with RVIZ.

Input
-----

topic `/teleop/move_body`

 * msg type: [spider_msgs/BodyState](https://github.com/erlerobot/erle-spider/blob/spider/software/src/spider_msgs/msg/BodyState.msg)

 * publishers: [/spider_imu](https://github.com/erlerobot/erle-spider/tree/master/software/src/spider_imu) and [/spider_teleop_joy](https://github.com/erlerobot/erle-spider/tree/master/software/src/spider_teleop_joy)

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

![alt tag](../../../docs/img/rqt_graph_captures/body_kinematics.png?raw=true)

Details
----

**rosnode info /spider_body_kinematics:**

```
--------------------------------------------------------------------------------
Node [/spider_body_kinematics]
Publications: 
 * /joints_to_controller [spider_msgs/LegsJointsState]
 * /rosout [rosgraph_msgs/Log]

Subscriptions: 
 * /teleop/move_body [spider_msgs/BodyState]
 * /teleop/body_command [spider_msgs/BodyCommand]

Services: 
 * /spider_body_kinematics/set_logger_level
 * /spider_body_kinematics/get_loggers


contacting node http://xabi-XPS-13-9343:51525/ ...
Pid: 24509
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
 * topic: /teleop/move_body
    * to: /spider_imu (http://xabi-XPS-13-9343:44000/)
    * direction: inbound
    * transport: TCPROS
 * topic: /teleop/move_body
    * to: /spider_teleop_joy (http://xabi-XPS-13-9343:44007/)
    * direction: inbound
    * transport: TCPROS
 * topic: /teleop/body_command
    * to: /spider_teleop_joy (http://xabi-XPS-13-9343:44007/)
    * direction: inbound
    * transport: TCPROS

```
