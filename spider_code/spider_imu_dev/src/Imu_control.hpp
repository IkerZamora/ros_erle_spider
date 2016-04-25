

#ifndef IMU_CONTROL_HPP_
#define IMU_CONTROL_HPP_

#include <ros/ros.h>
#include <unistd.h>
#include <spider_msgs/apm_imu.h>
#include <spider_msgs/BodyCommand.h>
#include <spider_msgs/BodyState.h>
#include "../IMU/DCM.h"


#define GRAVITY 256  //this equivalent to 1G in the raw data coming from the accelerometer


class Imu_control {
	public:
		Imu_control();


	private:
		void imuCallback(const spider_msgs::apm_imu imu_sample);
		void teleopBodyCmd(const spider_msgs::BodyCommandConstPtr body_cmd);
		void setup_IMU();
		void run();


		void Read_Gyro();
		void Read_Accel();


		ros::NodeHandle node;
		ros::Subscriber pxf_imu_sub;
		ros::Subscriber body_cmd_sub;
		ros::Publisher move_body_pub;


		spider_msgs::BodyState body_state;


};


#endif /* IMU_CONTROL_HPP_ */
