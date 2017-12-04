#include <opencv2\opencv.hpp>
#include <stdio.h>
#include <iostream>

void detectHScolor(const cv::Mat& image,
	double minHue, double maxHue,
	double minSat, double maxSat,
	cv::Mat& mask)
{
	cv::Mat hsv;
	cv::cvtColor(image, hsv, CV_BGR2HSV);
	std::vector<cv::Mat> channels;
	cv::split(hsv, channels);
	cv::Mat mask1;
	cv::threshold(channels[0], mask1, maxHue, 255, cv::THRESH_BINARY_INV);
	cv::Mat mask2;
	cv::threshold(channels[0], mask2, minHue, 255, cv::THRESH_BINARY);

	cv::Mat hueMask;
	if (minHue < maxHue)
		hueMask = mask1&mask2;
	else
		hueMask = mask1 | mask2;


	cv::threshold(channels[1], mask1, maxSat, 255, cv::THRESH_BINARY_INV);
	cv::threshold(channels[1], mask2, minSat, 255, cv::THRESH_BINARY);

	cv::Mat satMask;
	if (minSat< maxSat)
		satMask = mask1&mask2;
	else
		satMask = mask1 | mask2;

	mask = hueMask&satMask;
}