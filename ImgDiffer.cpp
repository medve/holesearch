#include "stdafx.h"
#include "ImgDiffer.h"
#include <opencv2/imgproc/imgproc.hpp>

#ifndef min
#define min(a,b)            (((a) < (b)) ? (a) : (b))
#endif

//TODO:��������� ���������� ������� �����������

ImgDiffer::ImgDiffer(void)
{
	this->morph_elem = cv::MORPH_ELLIPSE;
	this->morph_size = 3;
	this->threshold = 4;
}

void ImgDiffer::countSourceSizes()
{
	// �������� ���������� �������� � �����
	this->NRows = this->sourceImage.rows;
	this->NCols = this->sourceImage.cols;

	if(this->sourceImage.isContinuous()) {
		this->NCols*=this->NRows;
		this->NRows = 1;
	}
}

Mat ImgDiffer::countDifference(Mat& inputImage)
{
	if(!this->sourceImage.data) {
		throw (new InvalidSourceImageException());
	}

	//��������� ����������� � ������������� ���������

	if(inputImage.size[0]!=this->sourceImage.size[0] || 
		      inputImage.size[1]!=this->sourceImage.size[1]){
		throw (new InvalidInputImageException());
	}

	// ������ �������� ��� �������� �������
	this->diffImage = Mat::zeros(this->sourceImage.size(), CV_8U);

	//������������ ��� ������� �����������
	uchar* sourcePtr, *inputPtr, *diffPtr;
	for(int row = 0; row < this->NRows; row++) {
		//�������� ��������� �� ������
		sourcePtr = this->sourceImage.ptr<uchar>(row);
		inputPtr = inputImage.ptr<uchar>(row);
		diffPtr = this->diffImage.ptr<uchar>(row);

		//��� ������� �������
		//���� ������� �� ���������
		//������ �� ������
		for(int col = 0; col < this->NCols; col++)
		{
			diffPtr[col] = sourcePtr[col] + inputPtr[col] 
			              - 2*min(sourcePtr[col], inputPtr[col]);
		}
	}

	return this->diffImage;
}

Mat ImgDiffer::approveEdgesOfImage(Mat& inputImage)
{
	cv::Size size = cv::Size(2*morph_size + 1, 2*morph_size + 1);
	cv::Size point = cv::Point(morph_size, morph_size);

	Mat element = cv::getStructuringElement(morph_elem, size, point);

	Mat outputImg;
	cv::morphologyEx(inputImage, outputImg, cv::MORPH_OPEN, element);
	return outputImg;
}

Mat ImgDiffer::filterImage(Mat& inputImage, bool gauss)
{
	Mat bluredImage;
	if(gauss) {
		GaussianBlur(inputImage, bluredImage, cv::Size(3,3),0);
	} else {
		blur(inputImage, bluredImage, cv::Size(5,5));
	}

	return bluredImage;
}

Mat ImgDiffer::tresholdDifference(Mat& inputImage)
{
	return inputImage > this->threshold;
}

Mat ImgDiffer::prepareImage(Mat& img)
{
	//�������� � �������� ������
	Mat greyMat;
	if(img.channels() == 3){
		cv::cvtColor(img, greyMat, CV_BGR2GRAY);
	} else {
		img.copyTo(greyMat);
	}

	//��������� �����������
	greyMat = filterImage(greyMat);
	
	return greyMat;
}

void ImgDiffer::setSourceImage(Mat& img)
{
	//�������� ������� ������
	if(!img.data){
		throw (new InvalidInputImageException());
	}

	//�������� ����������� �������� ������
	this->sourceImage = this->prepareImage(img);

	//���������� �������� ���������� �������� � �����
	countSourceSizes();
}
	
Mat ImgDiffer::getSourceImage()
{

	return this->sourceImage;
}

void ImgDiffer::setThreshold(int threshold)
{
	this->threshold = threshold;
}

int ImgDiffer::getThreshold()
{
	return this->threshold;
}

ImgDiffer::~ImgDiffer(void)
{
	this->diffImage.release();
	this->sourceImage.release();
}

Mat ImgDiffer::getBinaryDifference(Mat& inputImage)
{
	//�������������� ����������� ��� ���������
	Mat filteredImage = this->prepareImage(inputImage);

	//���������� ����������� � ����������
	Mat differenceImage = this->countDifference(filteredImage);

	//���������� ������� ������� ��� � ������ �������
	//����������� ���������� � ��������������� ��������������
	differenceImage = approveEdgesOfImage(differenceImage);
	differenceImage = filterImage(differenceImage, false);
	differenceImage = approveEdgesOfImage(differenceImage);

	//���������� ��������� ��������������
	Mat binaryDifference = this->tresholdDifference(differenceImage);

	//����������� ������
	filteredImage.release();
	differenceImage.release();

	return binaryDifference;
}