#include <opencv2/opencv.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <iostream>

using namespace cv;
using namespace std;

int main() {
	for (int i = 0; i <= 15; i++) {
		string s = to_string(i) + ".jpg";
		string img_path;

		if (i < 10)
			img_path = "calibration\\image\\rgb_000" + s;
		else
			img_path = "calibration\\image\\rgb_00" + s;
		cout << " " << img_path << " " << endl;
		Mat image = imread(img_path);
		if (image.data) {
			imshow("hey", image);
			waitKey(0);
		}
	}
}