#include "stdafx.h"
#include "HoleSearcher.h"

HoleSearcher::HoleSearcher(void)
{
	differ = ImgDiffer();
}


HoleSearcher::~HoleSearcher(void)
{
	//����������� ������ �� �����������
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
	cv::Point2f cur_hole;

	for(int ctr_idx = 0; ctr_idx < contours.size(); 
		                                  ctr_idx++) {
		
		cv::minEnclosingCircle(contours[ctr_idx], 
								cur_hole, radius);
		holes.push_back(cur_hole);
	}
}

void HoleSearcher::findHoleContoursOnImage(
	                            Mat& image,
								vector<vector<Point>>& contours,
							    vector<cv::Vec4i>& hierarchy
					)
{
	//
	//��������������� ��������������, ���� � ����������
	//����� ��������������� ���������� �� ����
	//

	Mat binaryDifference = this->differ.getBinaryDifference(image);
	
	cv::findContours(binaryDifference, contours, hierarchy,
		              CV_RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE);

	//����������� ������
	binaryDifference.release();
}