#include<opencv2\opencv.hpp>
#include <iostream>
#include <stdio.h>
class ColorDetector
{
	int maxDist;
	cv::Vec3b target;
	cv::Mat result;
	cv::Mat converted;
public:
	ColorDetector() :maxDist(100), target(0, 0, 0) {}
	void setTargetColor(cv::Vec3b color)
	{
		target = color;
	}
	void setTargetColor(uchar blue, uchar green, uchar red)
	{
		target = cv::Vec3b(blue, green, red);
	}
	cv::Vec3b getTargetColor() const
	{
		return target;

	}
	int getColorDistance(const cv::Vec3b& color1, const cv::Vec3b& color2)const
	{
		return abs(color1[0] - color2[0]) + abs(color1[1] - color2[1]) + abs(color1[2] - color2[2]);
	}
	int getDistanceToTargetColor(const cv::Vec3b& color)const
	{
		return getColorDistance(color, target);
	}
	cv::Mat process(const cv::Mat &image);
	void setColorDistanceThreshold(int distance)
	{
		if (distance<0)
		{
			distance = 0;
		}
		maxDist = distance;
	}
	int getColorDistanceThreshold()const
	{
		return maxDist;
	}
};

class ColorDetectController
{
private:
	ColorDetector *cdetect;
	cv::Mat image;
	cv::Mat result;
public:
	ColorDetectController()
	{
		cdetect = new ColorDetector;
	}
	~ColorDetectController()
	{
		delete cdetect;
	}
	void setColorDistanceThreshold(int distance) const
	{
		return  cdetect->setColorDistanceThreshold(distance);
	}
	int getColorDistaceThreshold() const
	{
		return cdetect->getColorDistanceThreshold();
	}
	void setTargetColor(unsigned char red,
		unsigned char green, unsigned char blue)
	{
		cv::Mat tmp(1, 1, CV_8UC3);
		tmp.at<cv::Vec3b>(0, 0) = cv::Vec3b(blue, green, red);
		cv::cvtColor(tmp, tmp, CV_BGR2Lab);
		target = tmp.at<cv::Vec3b>(0, 0);
	}
	void getTargetColor(unsigned char &red,
		unsigned char &green, unsigned char &blue)
	{
		cv::Vec3b color = cdetect->getTargetColor();
		red = color[2];
		green = color[1];
		blue = color[0];
	}
	bool setIuputImage(std::string filename)
	{
		image = cv::imread(filename);
		return !image.empty();
	}
	const cv::Mat getInputImage()const
	{
		return image;
	}
	void process()
	{
		result = cdetect->process(image);
	}
	const cv::Mat getLastResult() const
	{
		return result;
	}

};

int main()
{
	ColorDetector cdetect;
	cv::Mat image = cv::imread("dc.jpg");
	if (image.empty())
		return 0;
	cdetect.setTargetColor(230, 190, 130);
	cv::namedWindow("result");
	cv::imshow("result", cdetect.process(image));
	cv::waitKey();
	return 0;
}

cv::Mat ColorDetector::process(const cv::Mat &image)
{
	result.create(image.size(), CV_8U);
	cv::cvtColor(image, converted, CV_BGR2Lab);
	cv::Mat_<cv::Vec3b>::const_iterator it = image.begin<cv::Vec3b>();
	cv::Mat_<cv::Vec3b>::const_iterator itend = image.end<cv::Vec3b>();
	cv::Mat_<uchar>::iterator itout = result.begin<uchar>();

	for (; it != itend; ++it, ++itout)
	{
		if (getDistanceToTargetColor(*it) <= maxDist)
		{
			*itout = 255;
		}
		else
		{
			*itout = 0;
		}
	}
	return result;
}
