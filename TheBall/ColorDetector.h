#pragma once
#include <opencv2/opencv.hpp>

class ColorDetector
{
public:
	ColorDetector(const cv::Mat &input);
	ColorDetector(cv::Mat &&input);

	ColorDetector& operator=(const ColorDetector& rhs) = delete;
	ColorDetector(const ColorDetector& rhs) = delete;

    // Detects the biggest segment of color in range between lowColorEdge and highColorEdge on target
    // and draws a bounding box over it
	void DetectByColor(const cv::Scalar &lowColorEdge, const cv::Scalar &highColorEdge, cv::Mat &target);

private:
	void GetMask(const cv::Mat &input, const cv::Scalar &lowColorEdge, const cv::Scalar &highColorEdge, cv::Mat &output);
	void GetBiggestContour(const cv::Mat &input, std::vector<cv::Point>& contour, std::vector<cv::Vec4i> &hierarchy);

	cv::Mat m_source;
};

