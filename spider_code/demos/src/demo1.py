#!/usr/bin/python
import rospy
import sys
import time
from spider_msgs.msg import *
from sensor_msgs.msg import Joy
 
def talker():

##PARAMS:
  standup_time=20
  walking_time=120
  turning_time=125

 
  rospy.init_node("joy_simulate", anonymous=True)
  pub = rospy.Publisher("/joy", Joy, queue_size=10)
  msg = Joy()
  msg.header.stamp = rospy.Time.now()
  rate = rospy.Rate(10)
 
################
## INITIALIZE ##
################
  valueAxe = 0.0
  valueButton = 0
  for i in range (0, 20):
    msg.axes.append(valueAxe)
  for e in range (0, 17):
    msg.buttons.append(valueButton)

  time.sleep(1)

####################
## PONERSE EN PIE ##
####################
  msg.buttons[3] = 1
  i=0
  bo=True
  print "STAND_UP"
  while not rospy.is_shutdown() and bo:
    i=i+1
    if (i>standup_time):
      bo=False
      msg.buttons[3] = 0
    pub.publish(msg)
    rate.sleep()

  time.sleep(2)


#################
## ANDAR 30 CM ##
#################
  msg.axes[1] =  1
  i=0
  bo=True
  print "WALKING"
  while not rospy.is_shutdown() and bo:
    i=i+1
    if (i>walking_time):
      bo=False
      msg.axes[1] = 0
    pub.publish(msg)
    rate.sleep()


###########
## GIRAR ##
###########
  msg.axes[2] = -1
  i=0
  bo=True
  print "TURN"
  while not rospy.is_shutdown() and bo:
    i=i+1
    if (i>turning_time):
      bo=False
      msg.axes[2] = 0
    pub.publish(msg)
    rate.sleep()


#################
## ANDAR 30 CM ##
#################
  msg.axes[1] =  1
  i=0
  bo=True
  print "WALKING"
  while not rospy.is_shutdown() and bo:
    i=i+1
    if (i>walking_time):
      bo=False
      msg.axes[1] = 0
    pub.publish(msg)
    rate.sleep()


####################
## SENTARSE ##
####################
  msg.buttons[3] = 1
  i=0
  bo=True
  standup_time=standup_time/3
  print "SEAT_DOWN"
  while not rospy.is_shutdown() and bo:
    i=i+1
    if (i>standup_time):
      bo=False
      msg.buttons[3] = 0
    pub.publish(msg)
    rate.sleep()



if __name__ == "__main__": 
  talker();
