#include "diffdrive_plugin_18servos.h"

const int rotation_direction[18] = 	{1,-1, 1,
					 1,-1, 1,
					 1,-1, 1,
			 	 	 1, 1,-1,
			 	 	 1, 1,-1,
				 	 1, 1,-1};


//Declaracion de variables
gazebo::math::Pose pose;
float servo_position[6][3];
float pi=3.14159216;

//Declaracion de variables
namespace gazebo {

enum
{
  CR1,
  CR2,
  CR3,
  CL1,
  CL2,
  CL3,
  FR1,
  FR2,
  FR3,
  FL1,
  FL2,
  FL3,
  TR1,
  TR2,
  TR3,
  TL1,
  TL2,
  TL3
};

// Constructor
DiffDrivePlugin18servos::DiffDrivePlugin18servos(){
	node.param("joint_lower_limit", joint_lower_limit, -1.570796327);
	node.param("joint_upper_limit", joint_upper_limit, 1.570796327);
	limit_coef = 127 / ((joint_upper_limit - joint_lower_limit) / 2);

	sub = node.subscribe("joints_to_controller", 1, &DiffDrivePlugin18servos::chatterLegsState, this); //Tenemos seis vectores de tres componentes 
}

// Destructor
DiffDrivePlugin18servos::~DiffDrivePlugin18servos()
{
  rosnode_->shutdown();
  delete rosnode_;
}

void DiffDrivePlugin18servos::chatterLegsState (const crab_msgs::LegsJointsStateConstPtr &legs_jnts){

	float target_value;
	int s_num;

	for (int i=0; i<num_legs; i++){
		for (int j=0; j<num_joints; j++){
			s_num = i*3+j;
			//target_value = 127 + rotation_direction[s_num] * legs_jnts->joints_state[i].joint[j] * limit_coef;

			if (j==1 || j==2) servo_position[i][j] = rotation_direction[s_num]*legs_jnts->joints_state[i].joint[j] * limit_coef/12.7;

			if(j==0) servo_position[i][j] = rotation_direction[s_num]*legs_jnts->joints_state[i].joint[j] * limit_coef/6.85;
		}
	}
}


void DiffDrivePlugin18servos::Load(physics::ModelPtr _model, sdf::ElementPtr _sdf)
{
  world = _model->GetWorld();

  // default parameters
  namespace_.clear();

  torque = 1.0;

  // load parameters
  if (_sdf->HasElement("robotNamespace"))
    namespace_ = _sdf->GetElement("robotNamespace")->GetValue()->GetAsString();

  if (_sdf->HasElement("topicName"))
    topic_ = _sdf->GetElement("topicName")->GetValue()->GetAsString();

  if (_sdf->HasElement("bodyName"))
  {
    link_name_ = _sdf->GetElement("bodyName")->GetValue()->GetAsString();
    link = _model->GetLink(link_name_);
  } else {
    link = _model->GetLink();
    link_name_ = link->GetName();
  }

  // assert that the body by link_name_ exists
  if (!link)
  {
    ROS_FATAL("DiffDrivePlugin18servos plugin error: bodyName: %s does not exist\n", link_name_.c_str());
    return;
  }

  if (_sdf->HasElement("coxar1Joint"))  joints[CR1]  = _model->GetJoint(_sdf->GetElement("coxar1Joint")->GetValue()->GetAsString());
  if (_sdf->HasElement("coxar2Joint"))  joints[CR2]  = _model->GetJoint(_sdf->GetElement("coxar2Joint")->GetValue()->GetAsString());
  if (_sdf->HasElement("coxar3Joint"))  joints[CR3]  = _model->GetJoint(_sdf->GetElement("coxar3Joint")->GetValue()->GetAsString());
  if (_sdf->HasElement("coxal1Joint"))  joints[CL1]  = _model->GetJoint(_sdf->GetElement("coxal1Joint")->GetValue()->GetAsString());
  if (_sdf->HasElement("coxal2Joint"))  joints[CL2]  = _model->GetJoint(_sdf->GetElement("coxal2Joint")->GetValue()->GetAsString());
  if (_sdf->HasElement("coxal3Joint"))  joints[CL3]  = _model->GetJoint(_sdf->GetElement("coxal3Joint")->GetValue()->GetAsString());
  if (_sdf->HasElement("femurr1Joint")) joints[FR1]  = _model->GetJoint(_sdf->GetElement("femurr1Joint")->GetValue()->GetAsString());
  if (_sdf->HasElement("femurr2Joint")) joints[FR2]  = _model->GetJoint(_sdf->GetElement("femurr2Joint")->GetValue()->GetAsString());
  if (_sdf->HasElement("femurr3Joint")) joints[FR3]  = _model->GetJoint(_sdf->GetElement("femurr3Joint")->GetValue()->GetAsString());
  if (_sdf->HasElement("femurl1Joint")) joints[FL1]  = _model->GetJoint(_sdf->GetElement("femurl1Joint")->GetValue()->GetAsString());
  if (_sdf->HasElement("femurl2Joint")) joints[FL2]  = _model->GetJoint(_sdf->GetElement("femurl2Joint")->GetValue()->GetAsString());
  if (_sdf->HasElement("femurl3Joint")) joints[FL3]  = _model->GetJoint(_sdf->GetElement("femurl3Joint")->GetValue()->GetAsString());
  if (_sdf->HasElement("tibiar1Joint")) joints[TR1]  = _model->GetJoint(_sdf->GetElement("tibiar1Joint")->GetValue()->GetAsString());
  if (_sdf->HasElement("tibiar2Joint")) joints[TR2]  = _model->GetJoint(_sdf->GetElement("tibiar2Joint")->GetValue()->GetAsString());
  if (_sdf->HasElement("tibiar3Joint")) joints[TR3]  = _model->GetJoint(_sdf->GetElement("tibiar3Joint")->GetValue()->GetAsString());
  if (_sdf->HasElement("tibial1Joint")) joints[TL1]  = _model->GetJoint(_sdf->GetElement("tibial1Joint")->GetValue()->GetAsString());
  if (_sdf->HasElement("tibial2Joint")) joints[TL2]  = _model->GetJoint(_sdf->GetElement("tibial2Joint")->GetValue()->GetAsString());
  if (_sdf->HasElement("tibial3Joint")) joints[TL3]  = _model->GetJoint(_sdf->GetElement("tibial3Joint")->GetValue()->GetAsString());

  if (!joints[CR1])  ROS_FATAL("diffdrive_plugin_18servos error: The controller couldn't get coxa_r1 joint");
  if (!joints[CR2])  ROS_FATAL("diffdrive_plugin_18servos error: The controller couldn't get coxa_r2 joint");
  if (!joints[CR3])  ROS_FATAL("diffdrive_plugin_18servos error: The controller couldn't get coxa_r3 joint");
  if (!joints[CL1])  ROS_FATAL("diffdrive_plugin_18servos error: The controller couldn't get coxa_l1 joint");
  if (!joints[CL2])  ROS_FATAL("diffdrive_plugin_18servos error: The controller couldn't get coxa_l2 joint");
  if (!joints[CL3])  ROS_FATAL("diffdrive_plugin_18servos error: The controller couldn't get coxa_l3 joint");
  if (!joints[FR1])  ROS_FATAL("diffdrive_plugin_18servos error: The controller couldn't get femur_r1 joint");
  if (!joints[FR2])  ROS_FATAL("diffdrive_plugin_18servos error: The controller couldn't get femur_r2 joint");
  if (!joints[FR3])  ROS_FATAL("diffdrive_plugin_18servos error: The controller couldn't get femur_r3 joint");
  if (!joints[FL1])  ROS_FATAL("diffdrive_plugin_18servos error: The controller couldn't get femur_l1 joint");
  if (!joints[FL2])  ROS_FATAL("diffdrive_plugin_18servos error: The controller couldn't get femur_l2 joint");
  if (!joints[FL3])  ROS_FATAL("diffdrive_plugin_18servos error: The controller couldn't get femur_l3 joint");
  if (!joints[TR1])  ROS_FATAL("diffdrive_plugin_18servos error: The controller couldn't get tibia_r1 joint");
  if (!joints[TR2])  ROS_FATAL("diffdrive_plugin_18servos error: The controller couldn't get tibia_r2 joint");
  if (!joints[TR3])  ROS_FATAL("diffdrive_plugin_18servos error: The controller couldn't get tibia_r3 joint");
  if (!joints[TL1])  ROS_FATAL("diffdrive_plugin_18servos error: The controller couldn't get tibia_l1 joint");
  if (!joints[TL2])  ROS_FATAL("diffdrive_plugin_18servos error: The controller couldn't get tibia_l2 joint");
  if (!joints[TL3])  ROS_FATAL("diffdrive_plugin_18servos error: The controller couldn't get tibia_l3 joint");

  if (_sdf->HasElement("torque"))
    _sdf->GetElement("torque")->GetValue()->Get(torque);

  // Make sure the ROS node for Gazebo has already been initialized
  if (!ros::isInitialized())
  {
    ROS_FATAL_STREAM("A ROS node for Gazebo has not been initialized, unable to load plugin. "
      << "Load the Gazebo system plugin 'libgazebo_ros_api_plugin.so' in the gazebo_ros package)");
    return;
  }

  rosnode_ = new ros::NodeHandle(namespace_);

  callback_queue_thread_ = boost::thread(boost::bind(&DiffDrivePlugin18servos::QueueThread, this));

  Reset();

  // New Mechanism for Updating every World Cycle
  // Listen to the update event. This event is broadcast every
  // simulation iteration.
  updateConnection = event::Events::ConnectWorldUpdateBegin(boost::bind(&DiffDrivePlugin18servos::Update, this));


  joints[CR1]->SetMaxForce(0, torque);
  joints[CR2]->SetMaxForce(0, torque);
  joints[CR3]->SetMaxForce(0, torque);
  joints[CL1]->SetMaxForce(0, torque);
  joints[CL2]->SetMaxForce(0, torque);
  joints[CL3]->SetMaxForce(0, torque);
  joints[FR1]->SetMaxForce(0, torque);
  joints[FR2]->SetMaxForce(0, torque);
  joints[FR3]->SetMaxForce(0, torque);
  joints[FL1]->SetMaxForce(0, torque);
  joints[FL2]->SetMaxForce(0, torque);
  joints[FL3]->SetMaxForce(0, torque);
  joints[TR1]->SetMaxForce(0, torque);
  joints[TR2]->SetMaxForce(0, torque);
  joints[TR3]->SetMaxForce(0, torque);
  joints[TL1]->SetMaxForce(0, torque);
  joints[TL2]->SetMaxForce(0, torque);
  joints[TL3]->SetMaxForce(0, torque);

}

// Initialize the controller
void DiffDrivePlugin18servos::Reset() {  

  prevUpdateTime = world->GetSimTime();

}

// Update the controller
void DiffDrivePlugin18servos::Update() { 

  // TODO: Step should be in a parameter of this function
  double d1, d2;
  double dr, da;
  common::Time stepTime;

  stepTime = world->GetSimTime() - prevUpdateTime;
  prevUpdateTime = world->GetSimTime();

//std::cout << "Valor de angulo:" << (float) servo_position[0][0]*pi/180 << "\n";

  joints[CR1]->SetVelocity(0, 0); joints[CR1]->SetAngle(0, (float) servo_position[0][0]*pi/180);
  joints[CR2]->SetVelocity(0, 0); joints[CR2]->SetAngle(0, (float) servo_position[1][0]*pi/180);
  joints[CR3]->SetVelocity(0, 0); joints[CR3]->SetAngle(0, (float) servo_position[2][0]*pi/180);
  joints[CL1]->SetVelocity(0, 0); joints[CL1]->SetAngle(0, (float) servo_position[3][0]*pi/180);
  joints[CL2]->SetVelocity(0, 0); joints[CL2]->SetAngle(0, (float) servo_position[4][0]*pi/180);
  joints[CL3]->SetVelocity(0, 0); joints[CL3]->SetAngle(0, (float) servo_position[5][0]*pi/180);
  joints[FR1]->SetVelocity(0, 0); joints[FR1]->SetAngle(0, (float) servo_position[0][1]*pi/180);
  joints[FR2]->SetVelocity(0, 0); joints[FR2]->SetAngle(0, (float) servo_position[1][1]*pi/180);
  joints[FR3]->SetVelocity(0, 0); joints[FR3]->SetAngle(0, (float) servo_position[2][1]*pi/180);
  joints[FL1]->SetVelocity(0, 0); joints[FL1]->SetAngle(0, (float) servo_position[3][1]*pi/180);
  joints[FL2]->SetVelocity(0, 0); joints[FL2]->SetAngle(0, (float) servo_position[4][1]*pi/180);
  joints[FL3]->SetVelocity(0, 0); joints[FL3]->SetAngle(0, (float) servo_position[5][1]*pi/180);
  joints[TR1]->SetVelocity(0, 0); joints[TR1]->SetAngle(0, (float) servo_position[0][2]*pi/180);
  joints[TR2]->SetVelocity(0, 0); joints[TR2]->SetAngle(0, (float) servo_position[1][2]*pi/180);
  joints[TR3]->SetVelocity(0, 0); joints[TR3]->SetAngle(0, (float) servo_position[2][2]*pi/180);
  joints[TL1]->SetVelocity(0, 0); joints[TL1]->SetAngle(0, (float) servo_position[3][2]*pi/180);
  joints[TL2]->SetVelocity(0, 0); joints[TL2]->SetAngle(0, (float) servo_position[4][2]*pi/180);
  joints[TL3]->SetVelocity(0, 0); joints[TL3]->SetAngle(0, (float) servo_position[5][2]*pi/180);

}

// NEW: custom callback queue thread
void DiffDrivePlugin18servos::QueueThread() { 

  static const double timeout = 0.01;

  while (alive_ && rosnode_->ok())
  {
    queue_.callAvailable(ros::WallDuration(timeout));
  }
}

GZ_REGISTER_MODEL_PLUGIN(DiffDrivePlugin18servos)

} // namespace gazebo
