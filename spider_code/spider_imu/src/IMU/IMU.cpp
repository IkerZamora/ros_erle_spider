/*
 * IMU.cpp
 *
 *  Created on: 03.10.2013
 *      Author: tuuzdu
 */

#include "L3G.h"
#include "LSM303.h"
#include "IMU.h"
#include <ros/ros.h>

L3G gyro;
LSM303 compass;

void IMU_Init() {
	gyro.enableDefault();
	compass.enableDefault();
}

void Read_Gyro() {
	gyro.read();

	AN[0] = gyro.g.x;
	AN[1] = gyro.g.y;
	AN[2] = gyro.g.z;
	ROS_INFO("Received: gyro: x=%f y=%f z=%f", AN[0] , AN[1] , AN[2]);//xabi
	gyro_x = SENSOR_SIGN[0] * (AN[0] - AN_OFFSET[0]);
	gyro_y = SENSOR_SIGN[1] * (AN[1] - AN_OFFSET[1]);
	gyro_z = SENSOR_SIGN[2] * (AN[2] - AN_OFFSET[2]);
	ROS_INFO("gyro final: x=%f y=%f z=%f\n", AN[0] , AN[1] , AN[2]);//xabi
}

// Reads x,y and z accelerometer registers
void Read_Accel() {
	compass.readAcc();

	AN[3] = compass.a.x;
	AN[4] = compass.a.y;
	AN[5] = compass.a.z;
	ROS_INFO("Received: accel: x=%f y=%f z=%f\n", AN[3] , AN[4] , AN[5]);//xabi
	accel_x = SENSOR_SIGN[3] * (AN[3] - AN_OFFSET[3]);
	accel_y = SENSOR_SIGN[4] * (AN[4] - AN_OFFSET[4]);
	accel_z = SENSOR_SIGN[5] * (AN[5] - AN_OFFSET[5]);
	ROS_INFO("accel final: x=%f y=%f z=%f\n", AN[3] , AN[4] , AN[5]);//xabi
}

void Read_Compass() {
	compass.readMag();
#if CALIBRMODE == 0
	magnetom_x = SENSOR_SIGN[6] * compass.m.x;
	magnetom_y = SENSOR_SIGN[7] * compass.m.y;
	magnetom_z = SENSOR_SIGN[8] * compass.m.z;
#else
	magnetom_x = compass.m.x;
	magnetom_y = compass.m.y;
	magnetom_z = compass.m.z;
#endif
}

