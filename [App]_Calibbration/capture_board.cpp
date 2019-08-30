#include <librealsense2\rs.hpp>
#include <opencv2/opencv.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include "capture.h"

using namespace rs2;
using namespace cv;

void Capture::capture_board(colorizer color_map, pipeline pipe_color) {
	
	Capture capture;

	int index = 0;
	int key;

	char buf[256];

	// Directory name of each file
	std::string folderName = capture.FOLDER_DATA;
	std::string imageName = capture.FILE_IMAGE_NAMES;

	rs2::config cfg_color;
	cfg_color.enable_stream(RS2_STREAM_COLOR, 1920, 1080, RS2_FORMAT_BGR8, 30);
	pipe_color.start(cfg_color);

	const auto color_name = "Color Image";
	namedWindow(color_name, WINDOW_AUTOSIZE);
	
	while (1) {

		//RGB color frame
		frameset data_color = pipe_color.wait_for_frames();
		frame color = data_color.get_color_frame();

		const int w_color = color.as<video_frame>().get_width();
		const int h_color = color.as<video_frame>().get_height();


		// Create OpenCV matrix of size(w_color, h_color) from color data
		Mat img_c(Size(w_color, h_color), CV_8UC3, (void*)color.get_data(), Mat::AUTO_STEP);

		// Update the window with new data
		imshow(color_name, img_c);

		char new_win[14] = "capture c-img";
		const auto file_image = imageName.c_str();

		if (cvWaitKey(10) == 'c') {
			
			cvNamedWindow(new_win, CV_WINDOW_AUTOSIZE);
			imshow(new_win, img_c);

			//if (img_c.data != 0) {
				sprintf(buf, file_image, index);
				imwrite(buf, img_c);

				std::cout << index <<"  One time capture done" << std::endl;
				
				index++;
			//}
			
		}

		else if (cvWaitKey(10) == 27)
			break;
	}
}