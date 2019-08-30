#ifndef CALIBRATION_H
#define CALIBRATION_H
#include <vector>
#include <iostream>
#include <opencv2\opencv.hpp>

using namespace cv;
using namespace std;
class Calibration
{
	
public:

	Calibration(){ }
	int set_calibration();

	double computeReprojectionErrors(const vector<vector<Point3f> >& objectPoints,
		const vector<vector<Point2f> >& imagePoints,
		const vector<Mat>& rvecs, const vector<Mat>& tvecs,
		const Mat& cameraMatrix, const Mat& distCoeffs,
		vector<float>& perViewErrors);

};
#endif