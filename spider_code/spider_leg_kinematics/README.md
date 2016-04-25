SPIDER_LEG_KINEMATICS
======

This package is responsible for providing the final positions for each servo to enable the spider movements.

It provides a service that calculates the inverse kinematics for any request made by [/spider_gait](https://github.com/erlerobot/erle-spider/tree/master/software/src/spider_gait) or [/spider_body_kinematics](https://github.com/erlerobot/erle-spider/tree/master/software/src/spider_body_kinematics). It's uses [Kinematics and Dynamics Library](https://github.com/orocos/orocos_kinematics_dynamics/tree/master/orocos_kdl).

How it works
-----

It waits for any request made by [/spider_gait](https://github.com/erlerobot/erle-spider/tree/master/software/src/spider_gait) or [/spider_body_kinematics](https://github.com/erlerobot/erle-spider/tree/master/software/src/spider_body_kinematics) in form of six vectors of the target position of the tip of feet. Then its algorithm uses KDL's software to provide a 18 values (one for each servo) like an answer.

Service
-----

service `/spider_leg_kinematics/get_ik`

 * srv type: [spider_msgs/GetLegIKSolver](https://github.com/erlerobot/erle-spider/blob/spider/software/src/spider_msgs/srv/GetLegIKSolver.srv)

 * clients: [/spider_gait](https://github.com/erlerobot/erle-spider/tree/master/software/src/spider_gait) and [/spider_body_kinematics](https://github.com/erlerobot/erle-spider/tree/master/software/src/spider_body_kinematics)

Graph
----

![alt tag](../../../docs/img/rqt_graph_captures/leg_kinematics.png?raw=true)

Details
----

**rosnode info /spider_leg_kinematics**

```
--------------------------------------------------------------------------------
Node [/spider_leg_kinematics]
Publications: 
 * /rosout [rosgraph_msgs/Log]

Subscriptions: None

Services: 
 * /spider_leg_kinematics/get_ik
 * /spider_leg_kinematics/get_loggers
 * /spider_leg_kinematics/set_logger_level


contacting node http://xabi-XPS-13-9343:48445/ ...
Pid: 18977
Connections:
 * topic: /rosout
    * to: /rosout
    * direction: outbound
    * transport: TCPROS


```
