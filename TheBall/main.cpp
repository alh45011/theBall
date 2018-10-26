#include <opencv2/core.hpp>
#include <opencv2/opencv.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include "opencv2/imgproc/imgproc.hpp"
#include <iostream>

using namespace cv;
using namespace std;

void getMask(InputArray input, OutputArray output)
{
	GaussianBlur(input, output, Size(11, 11), 1.5, 1.5);
	Mat3b hsv;
	cvtColor(output, hsv, COLOR_BGR2HSV/*COLOR_BGR2GRAY*/);
	inRange(hsv, Scalar(10, 150, 150), Scalar(25, 255, 255), output);

	erode(output, output, Mat());
	dilate(output, output, Mat());
}

void getBiggestContour(InputOutputArray input, vector<Point>& contour, vector<Vec4i> hierarchy)
{
	contour.clear();

	vector<vector<Point> > contours;
	findContours(input, contours, hierarchy, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE, Point(0, 0));

	if (contours.size() > 0)
	{
		int largestArea = 0;
		int largestContourIndex = -1;
		for (int i = 0; i < contours.size(); i++) // iterate through each contour. 
		{
			double a = contourArea(contours[i], false);  //  Find the area of contour
			if (a > largestArea) {
				largestArea = a;
				largestContourIndex = i;                //Store the index of largest contour
			}
		}
		if (largestContourIndex >= 0)
		{
			contour = vector<Point>(std::move(contours[largestContourIndex]));
		}
	}
}

int main(int argc, char** argv)
{
	VideoCapture cam(0);    
	if (!cam.isOpened())  // check if we succeeded
		return -1;    

	namedWindow("Cam", 1);
	namedWindow("mask", 1);
	while (true)
	{
		// Start timer
		double timer = (double)getTickCount();

		Mat frame;
		cam >> frame; // get a new frame from camera

		Mat mask;
		getMask(frame, mask);

		vector<Point> contour;  
		vector<Vec4i> hierarchy;

		getBiggestContour(mask.clone(), contour, hierarchy);

		// draw rect over the biggest contour
		if (contour.size() > 0)
		{
			Rect rect = boundingRect(contour); // Find the bounding rectangle for biggest contour   
			rectangle(frame, rect, Scalar(0, 255, 0), 1, 8, 0);
		}
		
		double fps = getTickFrequency() / ((double)getTickCount() - timer);
        
        // Display FPS on frame
        putText(frame, "FPS : " + format("%d", int(fps)), Point(100,100), FONT_HERSHEY_SIMPLEX, 0.75, (50, 170, 50), 2);

		//GaussianBlur(edges, edges, Size(7, 7), 1.5, 1.5);
		//Canny(edges, edges, 0, 30, 3);
		imshow("Cam", frame);
		imshow("mask", mask);
		if (waitKey(30) >= 0) break;

	}

	return 0;
}