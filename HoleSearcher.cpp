#include "stdafx.h"
#include "HoleSearcher.h"

HoleSearcher::HoleSearcher(void)
{
	differ = ImgDiffer();
}


HoleSearcher::~HoleSearcher(void)
{
	//освобождаем память от изображений
	this->sourceImage.release();
}


void HoleSearcher::setSourceImage(Mat& img)
{
	this->sourceImage = img;
	this->differ.setSourceImage(img);
}

Mat HoleSearcher::getSourceImage()
{
	return this->sourceImage;
}


void HoleSearcher::findHolesOnImage(
						vector<vector<Point>>& contours, 
	                    vector<cv::Point2f>& holes
					)
{
	float radius;

	for(int ctr_idx = 0; ctr_idx < contours.size(); 
		                                  ctr_idx++) {
		cv::minEnclosingCircle(contours[ctr_idx], 
			                   holes[ctr_idx], radius);
	}
}

void HoleSearcher::findHoleContoursOnImage(
	                            Mat& image,
								vector<vector<Point>>& contours,
							    vector<cv::Vec4i>& hierarchy
					)
{
	//
	//морфологические преобразования, блур и фильтрация
	//перед преобразованием избавиться от шума
	//

	Mat binaryDifference = this->differ.getBinaryDifference(image);
	
	cv::findContours(binaryDifference, contours, hierarchy,
		              CV_RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE);

	//освобождаем память
	binaryDifference.release();
}