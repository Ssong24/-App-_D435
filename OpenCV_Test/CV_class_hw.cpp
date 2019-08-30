#include <opencv2/opencv.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/features2d.hpp>

#include <iostream>

using namespace cv;


using namespace std;

int termProj0_Todo1();

int main() {

	printf("Program start! \n");
	termProj0_Todo1();


}

int termProj0_Todo1() {
	/*
	Mat inImg, gray, outImg;
	inImg = imread("flower.jpg");

	if (!inImg.data) {
		cout << " --(!) Error reading images " << endl;
		return -1;
	}

	cvtColor(inImg, gray, CV_BGR2GRAY);

	Ptr<SIFT> sift = SIFT::create();
	vector<KeyPoint> keypoints_1;

	sift->detect(gray, keypoints_1);

	drawKeypoints(gray, keypoints_1, outImg, Scalar::all(-1), DrawMatchesFlags::DEFAULT);

	imshow("sample image and SIFT features", outImg);

	waitKey(0);
	*/
	return 0;


}