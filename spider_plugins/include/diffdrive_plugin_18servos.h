#ifndef DIFFDRIVE_PLUGIN_HH
#define DIFFDRIVE_PLUGIN_HH

#include <map>

#include <gazebo/common/Plugin.hh>
#include <gazebo/common/Time.hh>

// ROS 
#include <ros/ros.h>

// Custom Callback Queue
#include <ros/callback_queue.h>
#include <ros/advertise_options.h>

// Boost
#include <boost/thread.hpp>
#include <boost/bind.hpp>

//Subscriber crab_maestro_controller
#include <crab_msgs/LegsJointsState.h>
#include "PolstroSerialInterface.h"

#include <algorithm>
#include <assert.h>
#include <gazebo/common/Events.hh>
#include <gazebo/physics/physics.hh>
#include <math.h>

#define N_CHANNELS 18


namespace gazebo
{

class DiffDrivePlugin18servos : public ModelPlugin
{

public:
  DiffDrivePlugin18servos();
  virtual ~DiffDrivePlugin18servos();

protected:
  virtual void Load(physics::ModelPtr _model, sdf::ElementPtr _sdf);
  virtual void Reset();
  virtual void Update();

private:
  //Subscriber crab_maestro_controller
  ros::Subscriber sub;
  void chatterLegsState (const crab_msgs::LegsJointsStateConstPtr &legs_jnts);
  int channels[N_CHANNELS];
  double joint_lower_limit, joint_upper_limit, limit_coef;
  const static unsigned int num_joints = 3;
  const static unsigned int num_legs = 6;
  std::string port_name;

  physics::LinkPtr link;
  physics::WorldPtr world;
  physics::JointPtr joints[18];

  float torque;

  // Simulation time of the last update
  common::Time prevUpdateTime;

  // ROS STUFF
  ros::NodeHandle* rosnode_;
  ros::NodeHandle node;

  boost::mutex lock;

  std::string namespace_;
  std::string topic_;
  std::string link_name_;

  // Custom Callback Queue
  ros::CallbackQueue queue_;
  boost::thread callback_queue_thread_;
  void QueueThread();

  float x_;
  float rot_;
  bool alive_;

  // Pointer to the update event connection
  event::ConnectionPtr updateConnection;
};
}
#endif

