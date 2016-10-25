#include "stdafx.h"
#include "CameraImageTaker.h"


CameraImageTaker::CameraImageTaker(void)
{
	this->cap = cv::VideoCapture(0);

	if(!this->cap.isOpened()){
		throw ;
	}
}


CameraImageTaker::~CameraImageTaker(void)
{
}

Mat CameraImageTaker::getImage()
{
	Mat frame;
	this->cap >> frame;

	return frame;
}

