#ifndef capture_H
#define capture_H

#include <iostream>
#include <librealsense2\rs.hpp>

class Capture
{
public:
	std::string FOLDER_DATA;
	std::string FOLDER_IMAGE;
	//std::string FOLDER_INTRINSIC;
	//std::string FOLDER_EXTRINSIC;

	std::string FILE_IMAGE_NAMES;
	std::string FILE_DETECTED_NAMES;
	//std::string FILE_INTRINSIC_NAMES;
	//std::string FILE_EXTRINSIC_NAMES;
	void capture_board(rs2::colorizer color_map, rs2::pipeline pipe_color);
	


	Capture::Capture() {
		FOLDER_DATA = "calibration\\";
		FOLDER_IMAGE = FOLDER_DATA + "image\\";
		FILE_IMAGE_NAMES = FOLDER_IMAGE + "rgb_%04d.bmp";
		FILE_DETECTED_NAMES = FOLDER_IMAGE + "d_%04d.bmp";

	}
	
	

};

#endif
