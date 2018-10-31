#include "ColorDetector.h"

using namespace cv;
using namespace std;

void ColorDetector::DetectByColor(const Scalar &lowColorEdge, const Scalar &highColorEdge, Mat &out)
{
    Mat mask;
    GetMask(out, lowColorEdge, highColorEdge, mask);

    vector<cv::Point> contour;
    vector<cv::Vec4i> hierarchy;

    GetBiggestContour(mask, contour, hierarchy);

    
    // draw rect over the biggest contour
    if (contour.size() > 0)
    {
        cv::Rect rect = boundingRect(contour); // Find the bounding rectangle for biggest contour   
        rectangle(out, rect, Scalar(0, 255, 0), 1, 8, 0);
    }
}

void ColorDetector::GetMask(const Mat &input, const Scalar &lowColorEdge, const Scalar &highColorEdge, Mat &output)
{
    GaussianBlur(input, output, Size(11, 11), 1.5, 1.5);
    Mat3b hsv;
    cvtColor(output, hsv, COLOR_BGR2HSV/*COLOR_BGR2GRAY*/);
    inRange(hsv, lowColorEdge, highColorEdge, output);

    erode(output, output, Mat());
    dilate(output, output, Mat());
}

void ColorDetector::GetBiggestContour(const Mat &input, std::vector<cv::Point>& contour, std::vector<cv::Vec4i> &hierarchy)
{
    contour.clear();

    vector<vector<cv::Point> > contours;
    findContours(input, contours, hierarchy, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE, cv::Point(0, 0));

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
            contour = vector<cv::Point>(std::move(contours[largestContourIndex]));
        }
    }
}
