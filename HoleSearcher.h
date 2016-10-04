#pragma once


#include <opencv2/core/core.hpp>
#include <vector>
#include <string>
#include "ImgDiffer.h"

using std::string;
using std::vector;
using cv::Mat;
using cv::Point;

class HoleSearcher
{
private:
	Mat sourceImage;
	ImgDiffer differ;

public:

	void setSourceImage(Mat& img);
	Mat getSourceImage();
	void findHolesOnImage(vector<vector<Point>>& contours, 
		                  vector<cv::Point2f>& holes);
	void findHoleContoursOnImage(Mat& image,
		                         vector<vector<Point>>& contours,
							     vector<cv::Vec4i>& hierarchy);

	HoleSearcher(void);
	~HoleSearcher(void);
};
