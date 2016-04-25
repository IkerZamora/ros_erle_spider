SPIDER_TELEOP_JOY
======

This package inplements the logic behind any interaction with the joystick.

Node that processes the data from the joystick and converts them into posts to control the hexapod. For example, the position of the two axes "analog" pen is calculated heading angle and distance from the center of its effect on stride length. These values are then used in the gait generator. Also builds the logic of using certain combinations and sequences of button presses. Received messages are passed on to the three topic.

How it works
-----

Read `/joy` topic, and depending on the actual situation of the spider, delegate on `spider_imu`, `spider_gait` or `body_kinematics`.

Input
-----

topic `/joy`

 * msg type: [sensor_msgs/Joy](https://github.com/ros/common_msgs/blob/jade-devel/sensor_msgs/msg/Joy.msg)

 * publishers: [//joy_node](https://github.com/ros-drivers/joystick_drivers/tree/indigo-devel/joy)

Output
------

topic `/teleop/move_body`

 * msg type: [BodyState](https://github.com/erlerobot/erle-spider/blob/master/software/src/spider_msgs/msg/BodyState.msg)

 * subscribers: [/spider_body_kinematics](https://github.com/erlerobot/erle-spider/tree/master/software/src/spider_body_kinematics)

topic `/teleop/body_command`

 *  msg type: [BodyCommand](https://github.com/erlerobot/erle-spider/blob/master/software/src/spider_msgs/msg/BodyCommand.msg)

 * subscribers: [/spider_imu](https://github.com/erlerobot/erle-spider/tree/master/software/src)

topic `/teleop/gait_control`

 * msg type: [GaitCommand](https://github.com/erlerobot/erle-spider/blob/master/software/src/spider_msgs/msg/GaitCommand.msg)

 * subscribers: [/spider_gait](https://github.com/erlerobot/erle-spider/tree/master/software/src)

Graph
----

![alt tag](../../../docs/img/rqt_graph_captures/teleop_joy.png?raw=true)

Details
----

**rosnode info /spider_teleop_joy:**

```
--------------------------------------------------------------------------------
Node [/spider_teleop_joy]
Publications: 
 * /teleop/move_body [spider_msgs/BodyState]
 * /rosout [rosgraph_msgs/Log]
 * /teleop/body_command [spider_msgs/BodyCommand]
 * /teleop/gait_control [spider_msgs/GaitCommand]

Subscriptions: 
 * /joy [sensor_msgs/Joy]

Services: 
 * /spider_teleop_joy/set_logger_level
 * /spider_teleop_joy/get_loggers


contacting node http://xabi-XPS-13-9343:57220/ ...
Pid: 9875
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
    * to: /spider_imu
    * direction: outbound
    * transport: TCPROS
 * topic: /teleop/body_command
    * to: /spider_body_kinematics
    * direction: outbound
    * transport: TCPROS
 * topic: /teleop/gait_control
    * to: /spider_gait
    * direction: outbound
    * transport: TCPROS
 * topic: /joy
    * to: /joy_node (http://xabi-XPS-13-9343:48463/)
    * direction: inbound
    * transport: TCPROS

```
