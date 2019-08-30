#include <iostream>
#include "capture.h"
#include "calibration.h"
#include <vector>
#include <opencv2\opencv.hpp>


using namespace cv;

int Calibration::set_calibration() {
	Capture cap;

	/*
	const string inputSettingsFile = cap.FOLDER_IMAGE;
	FileStorage fs(inputSettingsFile, FileStorage::READ);

	if (!fs.isOpened())
	{
		cout << " Could not open the configuration file: \"" << inputSettingsFile << "\"" << endl;
		return -1;
	}
	*/


	int numBoards = 10;		// Number of chessboard pictures
	int numCornerW = 8;		// Number of horizontal corners
	int numCornerH = 6;		// Number of vertical corners
	int numSquares = numCornerH * numCornerW;
	
	Size patternSize(numCornerW, numCornerH);		// Interior number of corners

	vector<Point2f> corners;	// Detected corners
	vector<Point3f> obj;
	vector<vector<Point3f>> object_points;
	vector<vector<Point2f>> image_points;

	// Set each chessboard corner as world coordinate !!
	for (int j = 0; j < numSquares; j++)
		obj.push_back(Point3f(j / numCornerW, j%numCornerW, 0.0f));	// Point3f-> If you want to know pos b/w corners, 
																	// you need to set corner position as float

	//cout << obj << endl;

	Mat image, gray;

	// Read the image files
	char buf[256];

	string folderName = cap.FOLDER_DATA;

	const auto file_image = cap.FILE_IMAGE_NAMES.c_str();
	const auto file_detected_image = cap.FILE_DETECTED_NAMES.c_str();

	for (int i = 0; i < numBoards; i++) {

		string img_path;
		string detected_path;

		sprintf(buf, file_image, i);
		img_path = buf;

		sprintf(buf, file_detected_image, i);
		detected_path = buf;
	
		image = imread(img_path);

		if (!image.data) {
			cout << "Could not open" << img_path << endl;
			return -1;
		}

		cvtColor(image, gray, CV_BGR2GRAY);

		//CALIB_CB_ADAPTIVE_THRESH:	Use adaptive threshold for black->white, rather than a fixed threshold level
		//CALIB_CB_NORMALIZE_IMAGE: Normalize the image gamma with equalizeHist() before applying fixed or adaptive thresholding.
		//CALIB_CB_FAST_CHECK:		Run a fast check on the image that looks for chessboard corners, 
		//      ?????????			and shortcut the call if none is found.This can drastically speed up the call in the degenerate condition when no chessboard is observed.
		
		bool patternFound = findChessboardCorners(gray, patternSize, corners, //CALIB_CB_ADAPTIVE_THRESH);
			CALIB_CB_ADAPTIVE_THRESH + CALIB_CB_NORMALIZE_IMAGE + CALIB_CB_FAST_CHECK);
		cout << patternFound << endl;

		int key = waitKey(10);

		if (key == 27) // esc key
			return 0;

		if (patternFound)
		{
			cout << img_path;
			cout << " Corners detected!" << endl;
			//cout << "Point3f Corners: " << corners << endl;
			//cout << "Point3f obj: " << obj << endl;
			cornerSubPix(gray, corners, Size(11, 11), Size(-1, -1),					// winSize, /// No zero Zone-> Size(-1,-1)
				TermCriteria(CV_TERMCRIT_EPS + CV_TERMCRIT_ITER, 30, 0.1));

			image_points.push_back(corners);		// image_points, corners: Point3f ÇØ¾ßÇÔ
			object_points.push_back(obj);			// object_points, obj: Point3f

			drawChessboardCorners(image, patternSize, Mat(corners), patternFound);
			imshow("Result ", image);
			
			//imwrite(detected_path, image);
		
		}

	}

	Mat intrinsic = Mat::eye(3, 3, CV_64F);		// Camera intrinsic parameter matrix 3x3
	Mat distCoeffs= Mat::zeros(8, 1, CV_64F);;	// Distortion matrix 8x1
	

	vector<Mat> rvecs, tvecs;

	intrinsic.ptr<float>(0)[0] = 1;
	intrinsic.ptr<float>(1)[1] = 1;


	
	double rms = calibrateCamera(object_points, image_points, image.size(), intrinsic, distCoeffs, rvecs, tvecs);

	Mat R1, T1;
	

	cout << "rvecs: " << endl << rvecs.at(1) << endl;
	Rodrigues(rvecs.at(1), R1);
	cout << "R1: " << endl <<  R1 << endl;
	cout << "tves: " << endl << tvecs.at(1) << endl;

	/*
	vector<float> reproj_Errs;
	double totalAvgErr;

	totalAvgErr = computeReprojectionErrors(object_points, image_points,
		rvecs, tvecs, intrinsic, distCoeffs, reproj_Errs);
	*/
	
	// Display intrinsic matrix
	for (int i = 0; i < 3; i++) {
		cout << "[";
		for (int j = 0; j < 3; j++) {

			cout << setprecision(4) << setw(5) << intrinsic.at<double>(i, j) << "  ";
		}
		cout << "]";
		cout << endl;
	}


	// Display distortion coefficients matrix 
	for (int i = 0; i < 8; i++) {
		if (i == 0)
			cout << "[";

		cout << setprecision(6) << setw(8) << distCoeffs.at<double>(i) << " ";


		if (i == 7)
			cout << "]" << endl;
	}

	// Display reprojection errors
	cout << "Re-projection error reported by calibrateCamera(rms): " << rms << endl;
	cout << "Image size: " << image.size() << endl;
	
	 // object_points.size(): num of detected pictures
	cout << "object_points size: " << object_points[0].size() << endl;

	cout << "image_points size: " << image_points[0].size() << endl;

	Mat imageUndistorted;

	for (int i = 0; i < numBoards; i++) {
		string img_path;

		sprintf(buf, file_image, i);
		img_path = buf;

		image = imread(img_path);

		if (!image.data) {
			cout << "Could not open " << img_path << endl;
			return -1;
		}

		undistort(image, imageUndistorted, intrinsic, distCoeffs);

		imshow("Original image", image);
		imshow("Undistorted image", imageUndistorted);

		waitKey(2);
	}

	return 0;

}


double Calibration::computeReprojectionErrors(const vector<vector<Point3f> >& objectPoints,
	const vector<vector<Point2f> >& imagePoints,
	const vector<Mat>& rvecs, const vector<Mat>& tvecs,
	const Mat& cameraMatrix, const Mat& distCoeffs,
	vector<float>& perViewErrors)
{
	vector<Point2f> imagePoints2;
	int i, totalPoints = 0;
	double totalErr = 0, err;
	//perViewErrors.resize(objectPoints.size());

	for (i = 0; i < (int)objectPoints.size(); ++i)
	{
		projectPoints(Mat(objectPoints[i]), rvecs[i], tvecs[i], cameraMatrix,
			distCoeffs, imagePoints2);
		err = norm(Mat(imagePoints[i]), Mat(imagePoints2), CV_L2);	// imagePoints[i]: Detected corners by algorithms
																	// imagePoints2 : Projected corner points by extrinsic and intrinsic param. matrix
		int n = (int)objectPoints[i].size();
		//perViewErrors[i] = (float)std::sqrt(err*err / n);
		totalErr += err*err;
		totalPoints += n;
	}

	return std::sqrt(totalErr / totalPoints);
}
