#include "stdafx.h"
#include "FileImgTaker.h"


FileImgTaker::FileImgTaker(void)
{

}


FileImgTaker::~FileImgTaker(void)
{

}

void FileImgTaker::setFileName(std::string &filename)
{
	this->fileName = filename;
}

Mat FileImgTaker::getImage()
{
	// получаем картинку
	Mat img = cv::imread(this->fileName, cv::IMREAD_GRAYSCALE);

	if(!img.data){
		throw ;
	}

	return img;
}
