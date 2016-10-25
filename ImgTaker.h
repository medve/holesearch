#pragma once
#include <opencv2/core/core.hpp>

class ImgTaker
{
public:
	ImgTaker(void);
	~ImgTaker(void);

	virtual cv::Mat getImage() = 0;
};

