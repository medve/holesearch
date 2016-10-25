#pragma once
#include "imgtaker.h"
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>

using cv::Mat;

class CameraImageTaker :
	public ImgTaker
{
private:
	cv::VideoCapture cap;

public:
	CameraImageTaker(void);
	~CameraImageTaker(void);

	virtual Mat getImage();
};

