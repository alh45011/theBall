// BallDetect.cpp : Defines the entry point for the console application.
//

#include <opencv2/core.hpp>
#include <opencv2/opencv.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include "opencv2/imgproc/imgproc.hpp"
#include <iostream>

using namespace cv;
using namespace std;

//void getMask(InputArray input, OutputArray output)
//{
//	GaussianBlur(input, output, Size(11, 11), 1.5, 1.5);
//	Mat3b hsv;
//	cvtColor(output, hsv, COLOR_BGR2HSV/*COLOR_BGR2GRAY*/);
//	inRange(hsv, Scalar(10, 10, 10), Scalar(50, 255, 255), output);
//
//	erode(output, output, Mat());
//	dilate(output, output, Mat());
//}
//
//void getBiggestContour(InputOutputArray input, vector<Point>& contour, vector<Vec4i> hierarchy)
//{
//	contour.clear();
//
//	vector<vector<Point> > contours;
//	findContours(input, contours, hierarchy, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE, Point(0, 0));
//
//	if (contours.size() > 0)
//	{
//		int largestArea = 0;
//		int largestContourIndex = -1;
//		for (int i = 0; i < contours.size(); i++) // iterate through each contour. 
//		{
//			double a = contourArea(contours[i], false);  //  Find the area of contour
//			if (a > largestArea) {
//				largestArea = a;
//				largestContourIndex = i;                //Store the index of largest contour
//			}
//		}
//		if (largestContourIndex >= 0)
//		{
//			contour = vector<Point>(std::move(contours[largestContourIndex]));
//		}
//	}
//}

//int main(int argc, char** argv)
//{
//	VideoCapture cam(0);
//	if (!cam.isOpened())  // check if we succeeded
//		return -1;
//
//	namedWindow("Cam", 1);
//	namedWindow("mask", 1);
//	while (true)
//	{
//		// Start timer
//		double timer = (double)getTickCount();
//
//		Mat frame;
//		cam >> frame; // get a new frame from camera
//
//		Mat mask;
//		getMask(frame, mask);
//
//		vector<Point> contour;
//		vector<Vec4i> hierarchy;
//
//		getBiggestContour(mask.clone(), contour, hierarchy);
//
//		// draw rect over the biggest contour
//		if (contour.size() > 0)
//		{
//			Rect rect = boundingRect(contour); // Find the bounding rectangle for biggest contour   
//			rectangle(frame, rect, Scalar(0, 255, 0), 1, 8, 0);
//		}
//
//		double fps = getTickFrequency() / ((double)getTickCount() - timer);
//
//		// Display FPS on frame
//		putText(frame, "FPS : " + format("%d", int(fps)), Point(100, 100), FONT_HERSHEY_SIMPLEX, 0.75, (50, 170, 50), 2);
//
//
//		Mat curves;
//		cvtColor(frame, curves, COLOR_BGR2GRAY);
//		GaussianBlur(curves, curves, Size(7, 7), 1.5, 1.5);
//		Canny(curves, curves, 0, 30, 3);
//		imshow("curves", curves);
//		if (waitKey(30) >= 0) break;
//
//		//GaussianBlur(edges, edges, Size(7, 7), 1.5, 1.5);
//		//Canny(edges, edges, 0, 30, 3);
//		imshow("Cam", frame);
//		imshow("mask", mask);
//		if (waitKey(30) >= 0) break;
//
//	}
//
//	return 0;
//}

//void getCircle(InputOutputArray input, vector<vector<Point>>& contour, vector<Vec4i> hierarchy)
//{
//	contour.clear();
//
//	vector<vector<Point> > contours;
//	findContours(input, contours, hierarchy, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE, Point(0, 0));
//
//	if (contours.size() > 0)
//	{
//		int largestArea = 0;
//		int largestContourIndex = -1;
//		for (int i = 0; i < contours.size(); i++) // iterate through each contour. 
//		{
//			double a = contourArea(contours[i], false);  //  Find the area of contour
//			if (a > largestArea) {
//				largestArea = a;
//				largestContourIndex = i;                //Store the index of largest contour
//			}
//		}
//		if (largestContourIndex >= 0)
//		{
//			contour = vector<Point>(std::move(contours[largestContourIndex]));
//		}
//	}
//}

int main()
{
	Mat image = imread("../ball_on_table2.jpg");
	int rows = image.rows;
	int cols = image.cols;

	if (image.empty())
	{
		cout << "Cannot load image!" << endl;
		return -1;
	}


	Mat image_gray;
	/// Convert it to gray
	cvtColor(image, image_gray, COLOR_BGR2GRAY);

	/// Reduce the noise so we avoid false circle detection
	GaussianBlur(image_gray, image_gray, Size(7, 7), 2, 2);
	Canny(image_gray, image_gray, 0, 100, 3);
	vector<Vec3f> circles;

	/// Apply the Hough Transform to find the circles
	HoughCircles(image_gray, circles, HOUGH_GRADIENT, 2, image_gray.rows / 8, 200, 100, 10, 0);

	/// Draw the circles detected
	for (size_t i = 0; i < circles.size(); i++)
	{
		Point center(cvRound(circles[i][0]), cvRound(circles[i][1]));
		int radius = cvRound(circles[i][2]);
		// circle center
		circle(image, center, 3, Scalar(0, 255, 0), -1, 8, 0);
		// circle outline
		circle(image, center, radius, Scalar(0, 0, 255), 3, 8, 0);
	}

	cout << circles.size();

	/// Show your results
	namedWindow("Hough Circle Transform Demo", WINDOW_AUTOSIZE);
	imshow("input_image", image);
	imshow("Hough Circle Transform Demo", image_gray);

	waitKey(0);
	return 0;
}