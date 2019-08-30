
#include <librealsense2\rs.hpp>	// Include RealSense Cross Platform API

//opencv
#include <opencv2/opencv.hpp>
#include <opencv2/calib3d.hpp>

#include <iostream>
#include <string>
#include <fstream>
#include <time.h>	// sleep()
#include <conio.h>

#include "device_t265.h"

#define isFinishWhenStopCapture true
# define M_PI           3.14159265358979323846
int captureT265Data(rs2::pipeline pipe);
void sleep(unsigned int mseconds);

int capturePoseData(rs2::pipeline pipe);


struct EulerAngles
{
	double pitch, yaw, roll;
};

EulerAngles ToRPY(rs2_quaternion q) {
	EulerAngles angles;
	angles.yaw = atan2(2 * q.y*q.w - 2 * q.x*q.z, 1 - 2 * q.y*q.y - 2 * q.z*q.z);
	angles.pitch =asin(2 * q.x*q.y + 2 * q.z*q.w);
	angles.roll = atan2(2 * q.x*q.w - 2 * q.y*q.z, 1 - 2 * q.x*q.x - 2 * q.z*q.z);
	if (q.x*q.y + q.z*q.w == 0.5) {
		angles.yaw = 2 * atan2(q.x, q.w);
		angles.roll = 0;
	}
	if (q.x*q.y + q.z*q.w == -0.5) {
		angles.yaw = -2 * atan2(q.x, q.w);
		angles.roll = 0;
	}
	return angles;
}

EulerAngles ToEulerAngles(rs2_quaternion q)
{
	EulerAngles angles;

	// roll (x-axis rotation)
	double sinr_cosp = +2.0 * (q.w * q.x + q.y * q.z);
	double cosr_cosp = +1.0 - 2.0 * (q.x * q.x + q.y * q.y);
	angles.roll = atan2(sinr_cosp, cosr_cosp);

	// pitch (y-axis rotation)
	double sinp = +2.0 * (q.w * q.y - q.z * q.x);
	if (fabs(sinp) >= 1)
		angles.pitch = copysign(M_PI / 2, sinp); // use 90 degrees if out of range
	else
		angles.pitch = asin(sinp);

	// yaw (z-axis rotation)
	double siny_cosp = +2.0 * (q.w * q.z + q.x * q.y);
	double cosy_cosp = +1.0 - 2.0 * (q.y * q.y + q.z * q.z);
	angles.yaw = atan2(siny_cosp, cosy_cosp);

	return angles;
}

int capturePoseData(rs2::pipeline pipe) {
	// In this 'captureT265Data' function
	// We will show two monochrome fisheye images and one pose information
	// and store them as .jpg and .txt file 

	Device_t265 device_t265;

	// Directory name of each file
	std::string folderName = device_t265.FOLDER_DATA;
	std::string fisheyeLeftFileName = device_t265.FILE_FISHEYEL_NAMES;
	std::string fisheyeRightFileName = device_t265.FILE_FISHEYER_NAMES;
	std::string accelFileName = device_t265.FILE_ACCEL_NAMES;
	std::string gyroFileName = device_t265.FILE_GYRO_NAMES;
	std::string poseFileName = device_t265.FILE_POSE_NAMES;
	std::string angleFileName = device_t265.FILE_ANGLE_NAMES;
	std::string fileTimes = device_t265.FILE_NAMES;
	

	// Start streaming with default recommended configuration
	pipe.start();

	// For the change of file's name
	int index = 0;

	// For capturing images in sequence by keypad
	bool storeImg = false;
	std::fstream file_names;

	// For storing the time when capture is processing
	clock_t startTime;
	startTime = clock();

	while (1) // Application still alive?
	{
		rs2::frameset data = pipe.wait_for_frames();	// Wait for next set of frames from the camera

														// Name of the fisheye windows

		// Name of the images and text
		const auto file_accel = accelFileName.c_str();			// %3d하면 000이 아닌 0부터 시작
		const auto file_gyro = gyroFileName.c_str();
		const auto file_pose = poseFileName.c_str();
		const auto file_angle = angleFileName.c_str();

		// For file name
		char buf[256];
		std::string accel_data = "";
		std::string gyro_data = "";
		std::string pose_data = "";
		std::string angle_data = "";




		// Find and retrieve IMU and/or tracking data
		if (rs2::motion_frame accel_frame = data.first_or_default(RS2_STREAM_ACCEL))
		{
			rs2_vector accel_sample = accel_frame.get_motion_data();
			accel_data = std::to_string(accel_sample.x) + " " +
				std::to_string(accel_sample.y) + " " +
				std::to_string(accel_sample.z) + " ";
			//std::cout << "Accel : " << accel_data << std::endl;

		}

		if (rs2::motion_frame gyro_frame = data.first_or_default(RS2_STREAM_GYRO))
		{
			rs2_vector gyro_sample = gyro_frame.get_motion_data();
			gyro_data = std::to_string(gyro_sample.x) + " " +
				std::to_string(gyro_sample.y) + " " +
				std::to_string(gyro_sample.z) + " ";
			std::cout << "Gyro  : " << gyro_data << std::endl;
		}

		if (rs2::pose_frame pose_frame = data.first_or_default(RS2_STREAM_POSE))
		{
			rs2_pose pose_sample = pose_frame.get_pose_data();
			pose_data = std::to_string(pose_sample.translation.x) + " " +
				std::to_string(pose_sample.translation.y) + " " +
				std::to_string(pose_sample.translation.z) + " ";
			//std::cout << "Pose : " << pose_data << std::endl;

			rs2_quaternion q = pose_sample.rotation;
			EulerAngles angles;
			angles = ToRPY(q);
			angle_data = std::to_string(angles.roll* 180.0 / M_PI) + " " +
				std::to_string(angles.yaw * 180.0 / M_PI) + " " +
				std::to_string(angles.pitch * 180.0 / M_PI) + " ";
			std::cout << "Angles : " << angle_data << std::endl;


		}
		std::cout << std::endl;

		sleep(1000);
		int key = cv::waitKey(100);
		int key2;
		if(kbhit())
			key2 = getch();


		// esc key : exit
		if (key == 27 || key2 == 27)
		{
			break;
		}
		if (key2 == 27) {
			printf("hello\n");
		}
		// key : Enter
		if (key == 13 || key2 == 13)
		{
			storeImg = !storeImg;
			if (storeImg) {
				std::cout << "Start Capture" << std::endl;
				file_names.open(fileTimes, std::ios::out);
			}
			else
			{
				std::cout << "Finished capture" << std::endl;
				file_names.close();
				if (isFinishWhenStopCapture)
				{
					return 0;
				}
			}

		}

		// key : a
		if (key == 97 || key2 == 97)
		{
			
				// Store accel, gyro, and pose data in each .txt file
				sprintf(buf, file_accel, index);
				std::ofstream out_text(buf);
				out_text << accel_data;
				out_text.close();

				sprintf(buf, file_gyro, index);
				std::ofstream out_gyro(buf);
				out_gyro << gyro_data;
				out_gyro.close();

				sprintf(buf, file_pose, index);
				std::ofstream out_pose(buf);
				out_pose << pose_data;
				out_pose.close();





				std::cout << "Finished capture " << std::endl;
				index++; // change name of the files

			
		}

		// Write the fisheye images and position files when enter key is pressed one time
		if (storeImg)
		{

			clock_t nowsTime = clock() - startTime + (10 * CLOCKS_PER_SEC);
			float nowsTime_str = ((float)nowsTime) / CLOCKS_PER_SEC;


			std::string outputFileName = std::to_string(nowsTime_str).substr(0, std::to_string(nowsTime_str).size() - 3);
			std::cout << "Capture pose data: " << outputFileName << std::endl;

	

			// Store accel, gyro, and pose data in each .txt file
			sprintf(buf, file_accel, index);
			std::ofstream out_text(buf);
			out_text << accel_data;
			out_text.close();

			sprintf(buf, file_gyro, index);
			std::ofstream out_gyro(buf);
			out_gyro << gyro_data;
			out_gyro.close();

			sprintf(buf, file_pose, index);
			std::ofstream out_pose(buf);
			out_pose << pose_data;
			out_pose.close();



			file_names << outputFileName << "\n";
			index++; // change name of the files
		}



	}	// while(1)
	return 0;
}

int captureT265Data(rs2::pipeline pipe) {
	// In this 'captureT265Data' function
	// We will show two monochrome fisheye images and one pose information
	// and store them as .jpg and .txt file 

	Device_t265 device_t265;

	// Directory name of each file
	std::string folderName = device_t265.FOLDER_DATA;
	std::string fisheyeLeftFileName = device_t265.FILE_FISHEYEL_NAMES;
	std::string fisheyeRightFileName = device_t265.FILE_FISHEYER_NAMES;
	std::string accelFileName = device_t265.FILE_ACCEL_NAMES;
	std::string gyroFileName = device_t265.FILE_GYRO_NAMES;
	std::string poseFileName = device_t265.FILE_POSE_NAMES;
	std::string fileTimes = device_t265.FILE_NAMES;

	// Start streaming with default recommended configuration
	pipe.start();

	// For the change of file's name
	int index = 0;

	// For capturing images in sequence by keypad
	bool storeImg = false;
	std::fstream file_names;

	// For storing the time when capture is processing
	clock_t startTime;
	startTime = clock();

	while (1) // Application still alive?
	{
		rs2::frameset data = pipe.wait_for_frames();	// Wait for next set of frames from the camera

														// Name of the fisheye windows
		const auto window_fisheyeL = "Fisheye Left Image";
		cv::namedWindow(window_fisheyeL, cv::WINDOW_AUTOSIZE);

		const auto window_fisheyeR = "Fisheye Right Image";
		cv::namedWindow(window_fisheyeR, cv::WINDOW_AUTOSIZE);

		// Name of the images and text
		const auto file_fisheyeL = fisheyeLeftFileName.c_str();
		const auto file_fisheyeR = fisheyeRightFileName.c_str();
		const auto file_accel = accelFileName.c_str();			// %3d하면 000이 아닌 0부터 시작
		const auto file_gyro = gyroFileName.c_str();
		const auto file_pose = poseFileName.c_str();

		// For file name
		char buf[256];
		std::string accel_data = "";
		std::string gyro_data = "";
		std::string pose_data = "";

		// To connect fisheye image, we need to use 'first_or_default(RS_STREAM_FISHEYE)' or get_fisheye_frame(idx of image)
		auto fisheyeR = data.first_or_default(RS2_STREAM_FISHEYE);	// default: fisheye right image
		auto fisheyeL = data.get_fisheye_frame(2);					// [idx] 1 : 2 = <frame> right : left

																	// Cast current instance as the type of 'rs2::video_frame'
		auto fl = fisheyeL.as<rs2::video_frame>();
		auto fr = fisheyeR.as<rs2::video_frame>();

		// Query frame size (width and height) of fisheye and pose video frame
		const int w_fish = fr.get_width();
		const int h_fish = fr.get_height();

		// Creat OpenCV Matrix of size (w,h) from the fisheye frame data
		cv::Mat fisheyeL_MAT = cv::Mat(cv::Size(w_fish, h_fish), CV_8UC1, (void*)fisheyeL.get_data(), cv::Mat::AUTO_STEP);
		cv::Mat fisheyeR_MAT = cv::Mat(cv::Size(w_fish, h_fish), CV_8UC1, (void*)fisheyeR.get_data(), cv::Mat::AUTO_STEP);

		// Update the window with new data
		cv::imshow(window_fisheyeL, fisheyeL_MAT);
		cv::imshow(window_fisheyeR, fisheyeR_MAT);

		// Find and retrieve IMU and/or tracking data
		if (rs2::motion_frame accel_frame = data.first_or_default(RS2_STREAM_ACCEL))
		{
			rs2_vector accel_sample = accel_frame.get_motion_data();
			accel_data = std::to_string(accel_sample.x) + " " +
				std::to_string(accel_sample.y) + " " +
				std::to_string(accel_sample.z) + " ";
			std::cout << "Accel : " << accel_data << std::endl;

		}

		if (rs2::motion_frame gyro_frame = data.first_or_default(RS2_STREAM_GYRO))
		{
			rs2_vector gyro_sample = gyro_frame.get_motion_data();
			gyro_data = std::to_string(gyro_sample.x) + " " +
				std::to_string(gyro_sample.y) + " " +
				std::to_string(gyro_sample.z) + " ";
			std::cout << "Gyro  : " << gyro_data << std::endl;
		}

		if (rs2::pose_frame pose_frame = data.first_or_default(RS2_STREAM_POSE))
		{
			rs2_pose pose_sample = pose_frame.get_pose_data();
			pose_data = std::to_string(pose_sample.translation.x) + " " +
				std::to_string(pose_sample.translation.y) + " " +
				std::to_string(pose_sample.translation.z) + " ";
			std::cout << "Pose : " << pose_data << std::endl;
		}
		std::cout << std::endl;

		int key = cv::waitKey(1);


		sleep(10);
		


		// esc key : exit
		if (key == 27)
		{
			break;
		}

		// key : Enter
		if (key == 13)
		{

			storeImg = !storeImg;
			if (storeImg) {
				std::cout << "Start Capture" << std::endl;
				file_names.open(fileTimes, std::ios::out);


			}
			else
			{
				std::cout << "Finished capture" << std::endl;
				file_names.close();
				if (isFinishWhenStopCapture)
				{
					return 0;
				}
			}

		}

		// key : a
		if (key == 97)
		{
			// capture one of all data
			std::cout << !fisheyeL_MAT.data << std::endl;

			if (fisheyeL_MAT.data == 0 || fisheyeR_MAT.data == 0) {
				std::cout << "Cannot save the image" << std::endl;
			}
			else {

				// Store two fisheye images as .jpg
				sprintf(buf, file_fisheyeL, index);
				cv::imwrite(buf, fisheyeL_MAT);

				sprintf(buf, file_fisheyeR, index);
				cv::imwrite(buf, fisheyeR_MAT);

				// Store accel, gyro, and pose data in each .txt file
				sprintf(buf, file_accel, index);
				std::ofstream out_text(buf);
				out_text << accel_data;
				out_text.close();

				sprintf(buf, file_gyro, index);
				std::ofstream out_gyro(buf);
				out_gyro << gyro_data;
				out_gyro.close();

				sprintf(buf, file_pose, index);
				std::ofstream out_pose(buf);
				out_pose << pose_data;
				out_pose.close();





				std::cout << "Finished capture " << std::endl;
				index++; // change name of the files

			}
		}

		// Write the fisheye images and position files when enter key is pressed one time
		if (storeImg)
		{

			clock_t nowsTime = clock() - startTime + (10 * CLOCKS_PER_SEC);
			float nowsTime_str = ((float)nowsTime) / CLOCKS_PER_SEC;


			std::string outputFileName = std::to_string(nowsTime_str).substr(0, std::to_string(nowsTime_str).size() - 3);
			std::cout << "Capture picture: " << outputFileName << std::endl;

			// Store two fisheye images as .jpg
			sprintf(buf, file_fisheyeL, index);
			cv::imwrite(buf, fisheyeL_MAT);
			sprintf(buf, file_fisheyeR, index);
			cv::imwrite(buf, fisheyeR_MAT);


			// Store accel, gyro, and pose data in each .txt file
			sprintf(buf, file_accel, index);
			std::ofstream out_text(buf);
			out_text << accel_data;
			out_text.close();

			sprintf(buf, file_gyro, index);
			std::ofstream out_gyro(buf);
			out_gyro << gyro_data;
			out_gyro.close();

			sprintf(buf, file_pose, index);
			std::ofstream out_pose(buf);
			out_pose << pose_data;
			out_pose.close();



			file_names << outputFileName << "\n";
			index++; // change name of the files
		}



	}	// while(1)
	return 0;
}


// Capture Example demonstrates how to
// capture depth and color video streams and render them to the screen
int main(int argc, char * argv[]) try
{
	rs2::log_to_console(RS2_LOG_SEVERITY_ERROR);

	// Declare depth colorizer for pretty visualization of depth data
	//rs2::colorizer color_map;

	// Declare RealSense pipeline, encapsulating the actual device and sensors
	rs2::pipeline pipe;

	// Run function here
	capturePoseData(pipe);
	//captureT265Data(pipe);

	return EXIT_SUCCESS;
}
catch (const rs2::error & e)
{
	std::cerr << "RealSense error calling " << e.get_failed_function() << "(" << e.get_failed_args() << "):\n    " << e.what() << std::endl;
	return EXIT_FAILURE;
}
catch (const std::exception& e)
{
	std::cerr << e.what() << std::endl;
	return EXIT_FAILURE;
}

void sleep(unsigned int mseconds) {
	clock_t goal = mseconds + clock();
	while (goal > clock());
}


