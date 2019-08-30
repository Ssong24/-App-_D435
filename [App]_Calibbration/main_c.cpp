#include "capture.h"
#include "calibration.h"



int main(int argc, char *argv[]) try
{
	// Declare object of class 'Capture'
	Capture cap;
	Calibration cal;

	// Declare depth colorizer for pretty visualization of depth data
	rs2::colorizer color_map;
	
	// Color
	// Declare RealSense pipeline, encapsulating the actual device and sensors
	rs2::pipeline pipe_color;

	cal.set_calibration();
	//cap.capture_board(color_map, pipe_color);
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