#include "Imu_control.hpp"

float ANi[6]={}; 
bool imu_on = false;


// Uncomment the below line to use this axis definition:
   // X axis pointing forward
   // Y axis pointing to the right
   // and Z axis pointing down.
// Positive pitch : nose up
// Positive roll : right wing down
// Positive yaw : clockwise
int SENSOR_SIGN[9] = {1,1,1,-1,-1,-1,1,1,1}; //Correct directions x,y,z - gyro, accelerometer, magnetometer
// Uncomment the below line to use this axis definition:
   // X axis pointing forward
   // Y axis pointing to the left
   // and Z axis pointing up.
// Positive pitch : nose down
// Positive roll : right wing down
// Positive yaw : counterclockwise
//int SENSOR_SIGN[9] = {1,-1,-1,-1,1,1,1,-1,-1}; //Correct directions x,y,z - gyro, accelerometer, magnetometer

float G_Dt=0.02;    // Integration time (DCM algorithm)  We will run the integration loop at 50Hz if possible

ros::Time timer(0);   //general purpuse timer
ros::Time timer_old(0);
//long timer24=0; //Second timer used to print values
int AN[6]; //array that stores the gyro and accelerometer data
int AN_OFFSET[6]={0,0,0,0,0,0}; //Array that stores the Offset of the sensors

int gyro_x;
int gyro_y;
int gyro_z;
int accel_x;
int accel_y;
int accel_z;
int magnetom_x;
int magnetom_y;
int magnetom_z;
float c_magnetom_x;
float c_magnetom_y;
float c_magnetom_z;
float MAG_Heading;

float Accel_Vector[3]= {0,0,0}; //Store the acceleration in a vector
float Gyro_Vector[3]= {0,0,0};//Store the gyros turn rate in a vector
float Omega_Vector[3]= {0,0,0}; //Corrected Gyro_Vector data
float Omega_P[3]= {0,0,0};//Omega Proportional correction
float Omega_I[3]= {0,0,0};//Omega Integrator
float Omega[3]= {0,0,0};

// Euler angles
float roll;
float pitch;
float yaw;

float errorRollPitch[3]= {0,0,0};
float errorYaw[3]= {0,0,0};

unsigned int counter=0;

float DCM_Matrix[3][3]= {
  {
    1,0,0  }
  ,{
    0,1,0  }
  ,{
    0,0,1  }
};
float Update_Matrix[3][3]={{0,1,2},{3,4,5},{6,7,8}}; //Gyros here


float Temporary_Matrix[3][3]={
  {
    0,0,0  }
  ,{
    0,0,0  }
  ,{
    0,0,0  }
};

bool readFlag=true;


Imu_control::Imu_control(){

	body_cmd_sub = node.subscribe("/teleop/body_command", 1, &Imu_control::teleopBodyCmd, this);//No probado
	move_body_pub = node.advertise<spider_msgs::BodyState>("/teleop/move_body",1);//No probado
	pxf_imu_sub = node.subscribe("apm_inertial_sensor", 100, &Imu_control::imuCallback, this);
	ROS_INFO("Spider_imu_dev is ready...");


	ros::Rate loop_rate(45);

	body_state.z = -0.08;
	body_state.leg_radius = 0.11;


	while (node.ok()){
		this->run();
		ros::spinOnce();
    	loop_rate.sleep();
	}

}


void Imu_control::run(){
	
		if (imu_on && !readFlag){
			readFlag = true;
			counter++;
			timer_old = timer;
			timer=ros::Time::now();
			if (timer>timer_old)
			  G_Dt = (timer.toSec()-timer_old.toSec());    // Real time of loop run. We use this on the DCM algorithm (gyro integration time)
			else
			  G_Dt = 0;

			// *** DCM algorithm
			// Data adquisition
			Read_Gyro();   // This read gyro data
			Read_Accel();     // Read I2C accelerometer
/*
			if (counter > 5)  // Read compass data at 10Hz... (5 loop runs)
			  {
			  counter=0;
			  Read_Compass();    // Read I2C magnetometer
			  Compass_Heading(); // Calculate magnetic heading
			  }
*/
			// Calculations...
			Matrix_update();
			Normalize();
			Drift_correction();
			Euler_angles();
			// ***
	//	    printf ("\033[1Aroll: %.2f \tpitch: %.2f  \tyaw: %.2f\n", roll, pitch, yaw);

			if (roll > 0.015) body_state.roll = body_state.roll + 0.1 * roll;
			if (roll < -0.015) body_state.roll = body_state.roll + 0.1 * roll;
			if (pitch > 0.015) body_state.pitch = body_state.pitch - 0.1 * pitch;
			if (pitch < -0.015) body_state.pitch = body_state.pitch - 0.1 * pitch;

			if (body_state.roll > 0.35) body_state.roll = 0.35;
			if (body_state.roll < -0.35) body_state.roll = -0.35;
			if (body_state.pitch > 0.35) body_state.pitch = 0.35;
			if (body_state.pitch < -0.35) body_state.pitch = -0.35;

			move_body_pub.publish(body_state);

		}

}


void Imu_control::setup_IMU()
{

	ROS_INFO ("Start initialization IMU");
	//IMU_Init();
	usleep (20000);
	for(int i=0;i<32;i++)    // We take some readings...
    {
		Read_Gyro();
		Read_Accel();
		for(int y=0; y<6; y++)   // Cumulate values
			AN_OFFSET[y] += AN[y];
		usleep (20000);
    }

	for(int y=0; y<6; y++)
		AN_OFFSET[y] = AN_OFFSET[y]/32;

	AN_OFFSET[5]-=GRAVITY*SENSOR_SIGN[5];

	timer=ros::Time::now();
	usleep (20000);
	counter=0;
}




void Imu_control::teleopBodyCmd(const spider_msgs::BodyCommandConstPtr body_cmd){

	if (body_cmd->cmd == body_cmd->IMU_START_CMD){
		setup_IMU();
		ros::Rate r(25);
		while (body_state.z >= -0.08){
			body_state.z -= 0.0025;
			r.sleep();
			move_body_pub.publish(body_state);
		}
		imu_on = true;
//		ros::Duration(2.5).sleep();

	ROS_INFO("IMU activated");//xabi

	}
	if (body_cmd->cmd == body_cmd->IMU_STOP_CMD){
		ros::Rate r(25);
		body_state.roll = 0;
		body_state.pitch = 0;
		body_state.yaw = 0;
		body_state.x = 0;
		body_state.y = 0;
		while (body_state.z <= -0.016){
			body_state.z += 0.0025;
			r.sleep();
			move_body_pub.publish(body_state);
		}
		imu_on = false;
//		ros::Duration(2).sleep();

	ROS_INFO("IMU desactivated");//xabi

	}


}

// XABI

void Imu_control::imuCallback(const spider_msgs::apm_imu imu_sample){
/*
	AN[0] = int(imu_sample.gyro[0] + 0.5); 
	AN[1] = int(imu_sample.gyro[1] + 0.5);
	AN[2] = int(imu_sample.gyro[2] + 0.5);
	AN[3] = int(imu_sample.accel[0] + 0.5);
	AN[4] = int(imu_sample.accel[1] + 0.5);
	AN[5] = int(imu_sample.accel[2] + 0.5);
*/
/*
	AN[0] = static_cast<int>(imu_sample.gyro[0]);
	AN[1] = static_cast<int>(imu_sample.gyro[1]);
	AN[2] = static_cast<int>(imu_sample.gyro[2]);
	AN[3] = static_cast<int>(imu_sample.accel[0]);
	AN[4] = static_cast<int>(imu_sample.accel[1]);
	AN[5] = static_cast<int>(imu_sample.accel[2]);
*/

	//array that stores the gyro and accelerometer data
  if (readFlag) {

  	ANi[0] = imu_sample.gyro[0];
	ANi[1] = imu_sample.gyro[1];
	ANi[2] = imu_sample.gyro[2];
	ANi[3] = imu_sample.accel[0];
	ANi[4] = imu_sample.accel[1];
	ANi[5] = imu_sample.accel[2];

	ROS_INFO("Received: gyro: x=%f y=%f z=%f", ANi[0] , ANi[1] , ANi[2]);//xabi
	ROS_INFO("Received: accel: x=%f y=%f z=%f\n", ANi[3] , ANi[4] , ANi[5]);//xabi

	for( int a = 0; a < 6; a = a + 1 ) {
		//AN[a] = static_cast<int>(ANi[a]);
		AN[a] = ANi[a];
    }

  	readFlag = false;

  }

}

// XABI



int main(int argc, char **argv)
{
	ros::init(argc, argv, "imu_control");
	ROS_INFO("Initializing imu_dev");
	Imu_control i;
	ros::spin();
}


/////////////

void Imu_control::Read_Gyro() {
/*	gyro.read();

	AN[0] = gyro.g.x;
	AN[1] = gyro.g.y;
	AN[2] = gyro.g.z;      XABI        */
	gyro_x = SENSOR_SIGN[0] * (AN[0] - AN_OFFSET[0]);
	gyro_y = SENSOR_SIGN[1] * (AN[1] - AN_OFFSET[1]);
	gyro_z = SENSOR_SIGN[2] * (AN[2] - AN_OFFSET[2]);
	printf("gyro final: x=%i y=%i z=%i\n", AN[0] , AN[1] , AN[2]);//xabi
}



void Imu_control::Read_Accel() {
/*	compass.readAcc();

	AN[3] = compass.a.x;
	AN[4] = compass.a.y;
	AN[5] = compass.a.z;      XABI        */
	accel_x = SENSOR_SIGN[3] * (AN[3] - AN_OFFSET[3]);
	accel_y = SENSOR_SIGN[4] * (AN[4] - AN_OFFSET[4]);
	accel_z = SENSOR_SIGN[5] * (AN[5] - AN_OFFSET[5]);
	printf("accel final: x=%i y=%i z=%i\n", AN[3] , AN[4] , AN[5]);//xabi
}
