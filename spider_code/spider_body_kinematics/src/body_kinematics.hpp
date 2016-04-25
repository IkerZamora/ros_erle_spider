
#ifndef BODY_KINEMATICS_HPP_
#define BODY_KINEMATICS_HPP_

#include <ros/ros.h>
#include <kdl_parser/kdl_parser.hpp>
#include <spider_msgs/GetLegIKSolver.h>
#include <spider_msgs/LegsJointsState.h>
#include <spider_msgs/BodyState.h>
#include <spider_msgs/BodyCommand.h>

#define NUM_LEGS 6
#define NUM_JOINTS 3

class BodyKinematics {
	public:
		BodyKinematics();
		bool init();

	private:
		ros::NodeHandle node;
		std::string root_name, tip_name;
		std::vector<KDL::Frame> frames;
		spider_msgs::BodyState bs;
		spider_msgs::LegsJointsState legs;
		spider_msgs::GetLegIKSolver srv;
		const static unsigned int num_joints = NUM_JOINTS;
		const static unsigned int num_legs = NUM_LEGS;
		double z;

		KDL::Rotation rotation;
		KDL::Frame tibia_foot_frame, femur_frame;
		KDL::Vector offset_vector, rotate_correction, final_vector [num_legs];

		ros::ServiceClient client;
		ros::Publisher joints_pub;
		ros::Subscriber body_move_sub;
		ros::Subscriber body_cmd_sub;

		bool loadModel(const std::string xml);
		bool calculateKinematics (spider_msgs::BodyState* body_ptr);
		bool callService (KDL::Vector* vector);
		void teleopBodyMove (const spider_msgs::BodyStateConstPtr &body_state);
		void teleopBodyCmd (const spider_msgs::BodyCommandConstPtr &body_cmd);
};



#endif /* BODY_KINEMATICS_HPP_ */
