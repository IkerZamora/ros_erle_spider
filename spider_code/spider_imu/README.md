SPIDER_IMU
======

The purpose of this node is to stabilize the spider using this [IMU](https://www.pololu.com/product/1268).

Driver for [Minimu-9](https://www.pololu.com/product/1268), assembled from parts Arduino sketch. Contains an algorithm for calculating the Euler angles on the basis of data from the MEMS sensors. Also contains a simple proportional controller that generates exposure to compensate for deviations in pitch and roll.

How it works
-----

When `/spider_telop_joy` give him the control, it read from the [IMU](https://www.pololu.com/product/1268) and write the data on `/teleop/move_body` topic.

Input
-----

topic `/teleop/body_command`

 * msg type: [BodyCommand](https://github.com/erlerobot/erle-spider/blob/master/software/src/spider_msgs/msg/BodyCommand.msg)

 * publishers: [/spider_teleop_joy](https://github.com/erlerobot/erle-spider/tree/master/software/src/spider_teleop_joy)

Output
------

topic `/teleop/move_body`

 * msg type: [BodyState](https://github.com/erlerobot/erle-spider/blob/master/software/src/spider_msgs/msg/BodyState.msg)

 * subscribers: [/spider_body_kinematics](https://github.com/erlerobot/erle-spider/tree/master/software/src/spider_body_kinematics)

Graph
----

![alt tag](../../../docs/img/rqt_graph_captures/imu.png?raw=true)

Details
----

**rosnode info /spider_imu:**

```
--------------------------------------------------------------------------------
Node [/spider_imu]
Publications: 
 * /teleop/move_body [spider_msgs/BodyState]
 * /rosout [rosgraph_msgs/Log]

Subscriptions: 
 * /teleop/body_command [spider_msgs/BodyCommand]

Services: 
 * /spider_imu/set_logger_level
 * /spider_imu/get_loggers


contacting node http://xabi-XPS-13-9343:38467/ ...
Pid: 9837
Connections:
 * topic: /rosout
    * to: /rosout
    * direction: outbound
    * transport: TCPROS
 * topic: /teleop/move_body
    * to: /spider_body_kinematics
    * direction: outbound
    * transport: TCPROS
 * topic: /teleop/body_command
    * to: /spider_teleop_joy (http://xabi-XPS-13-9343:57220/)
    * direction: inbound
    * transport: TCPROS

```
