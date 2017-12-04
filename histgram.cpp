#include <opencv2\opencv.hpp>
#include <iostream>
#include <opencv\highgui.h>

class Histogram1D
{
public:
	Histogram1D();
	cv::Mat getHistogram(const cv::Mat &image)
	{
		cv::Mat hist;
		cv::calcHist(&image,
			1,
			channels,
			cv::Mat(),
			hist,
			1,
			histSize,
			ranges
		);
		return hist;
	}
	static cv::Mat getImageOfHistogram(const cv::Mat hist,int zoom)
	{
		double maxVal = 0;
		double minVal = 0;
		cv::minMaxLoc(hist, &minVal, &maxVal, 0, 0);

		int histSize = hist.rows;

		cv::Mat histImg(histSize*zoom, histSize*zoom, CV_8U, cv::Scalar(255));
		int hpt = static_cast<int>(0.9*histSize);
		for (int h = 0; h < histSize; h++)
		{
			float binVal = hist.at<float>(h);
			if (binVal > 0)
			{
				int intensity = static_cast<int>(binVal*hpt / maxVal);
				cv::line(histImg, cv::Point(h*zoom, histSize*zoom), cv::Point(h*zoom, (histSize - intensity)*zoom), cv::Scalar(0), zoom);
			}
		}
		return histImg;
	}
	cv::Mat getHistogramImage(const cv::Mat &image, int zoom = 1)
	{
		cv::Mat hist = getHistogram(image);
		return getImageOfHistogram(hist, zoom);
	}

private:
	int histSize[1];
	float hranges[2];
	const float* ranges[1];
	int channels[1];

};

Histogram1D::Histogram1D()
{
	histSize[0] = 256;
	hranges[0] = 0.0;
	hranges[1] = 256.0;
	ranges[0] = hranges;
	channels[0] = 0;
}



int main()
{
	cv::Mat image = cv::imread("niu.jpg", 0);
	Histogram1D h;
	cv::Mat histo = h.getHistogram(image);
	cv::namedWindow("histogram");
	cv::imshow("histogram", h.getHistogramImage(image));
	cv::Mat thresholded;
	cv::threshold(image, thresholded, 60, 255, cv::THRESH_BINARY);
	cv::imshow("thr", thresholded);
	cv::waitKey();
}