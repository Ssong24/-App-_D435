#ifndef device_t265_H
#define device_t265_H

#include <librealsense2\rs.hpp>	// Include RealSense Cross Platform API


class Device_t265
{
public: 
	std::string FOLDER_DATA;
	std::string FILE_NAMES;
	std::string FOLDER_FISHEYE;
	std::string FOLDER_ACCEL;
	std::string FOLDER_GYRO;
	std::string FOLDER_POSE;
	std::string FOLDER_ANGLE;
	std::string FILE_FISHEYEL_NAMES;
	std::string FILE_FISHEYER_NAMES;
	std::string FILE_ACCEL_NAMES;
	std::string FILE_GYRO_NAMES;
	std::string FILE_POSE_NAMES;
	std::string FILE_ANGLE_NAMES;

	
	Device_t265() {
		FOLDER_DATA = "Data\\";
		FILE_NAMES = FOLDER_DATA + "captureTime.txt";
		FOLDER_FISHEYE	= FOLDER_DATA + "fisheye\\";
		
		FOLDER_ACCEL = FOLDER_DATA + "accel\\";
		FOLDER_GYRO = FOLDER_DATA + "gyro\\";
		FOLDER_POSE = FOLDER_DATA + "pose\\";
		FOLDER_ANGLE = FOLDER_DATA + "angle\\";
		FILE_FISHEYEL_NAMES = FOLDER_FISHEYE + "fisheye_left_%04d.jpg";
		FILE_FISHEYER_NAMES = FOLDER_FISHEYE + "fisheye_right_%04d.jpg";
		FILE_ACCEL_NAMES = FOLDER_ACCEL + "accel_%04d.txt";
		FILE_GYRO_NAMES = FOLDER_GYRO + "gyro_%04d.txt";
		FILE_POSE_NAMES = FOLDER_POSE + "pose_%04d.txt";
		FILE_ANGLE_NAMES = FOLDER_ANGLE + "angle_%04d.txt";

	}





};



#endif
