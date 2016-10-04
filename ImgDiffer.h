#pragma once
#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>
#include <exception>

using std::exception;

class InvalidInputImageException: public exception
{
	virtual const char* what() const throw()
	{
		return "Invalid Input Image!";
	}
};

class InvalidSourceImageException: public exception
{
	virtual const char* what() const throw()
	{
		return "Invalid Source Image!";
	}
};

using cv::Mat;
using cv::Point;
class ImgDiffer
{
private:
	Mat diffImage;
	Mat sourceImage;
	int NRows;
	int NCols;
	int threshold;
	int morph_elem;
	int morph_size;
		
	void countSourceSizes();
	Mat prepareImage(Mat& image);

public:
	void setSourceImage(Mat& img);
	Mat getSourceImage();
	void setThreshold(int threshold);
	int getThreshold();
	Mat countDifference(Mat& inputImage);
	Mat approveEdgesOfImage(Mat& inputImage);
	Mat filterImage(Mat& inputImage, bool gauss = true);
	Mat tresholdDifference(Mat& inputImage);
	Mat getBinaryDifference(Mat& inputImage);

	ImgDiffer(void);
	~ImgDiffer(void);
};

