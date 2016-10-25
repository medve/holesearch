#pragma once
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <vector>
#include <string>
#include "ImgTaker.h"

using cv::Mat;

class FileImgTaker : public ImgTaker
{
private:
	std::string fileName;
public:
	FileImgTaker(void);
	~FileImgTaker(void);

	void setFileName(std::string &filename);

	virtual Mat getImage();
	
};

