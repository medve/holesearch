#include "stdafx.h"
#include "ImgDiffer.h"
#include <opencv2/imgproc/imgproc.hpp>

#ifndef min
#define min(a,b)            (((a) < (b)) ? (a) : (b))
#endif

//TODO:оставлять нетронутым входное изображение

ImgDiffer::ImgDiffer(void)
{
	this->morph_elem = cv::MORPH_ELLIPSE;
	this->morph_size = 3;
	this->threshold = 4;
}

void ImgDiffer::countSourceSizes()
{
	// получаем количество столбцов и строк
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

	//вычисляет изображение с отличающимися пикселями

	if(inputImage.size[0]!=this->sourceImage.size[0] || 
		      inputImage.size[1]!=this->sourceImage.size[1]){
		throw (new InvalidInputImageException());
	}

	// создаём картинку для хранения разницы
	this->diffImage = Mat::zeros(this->sourceImage.size(), CV_8U);

	//обрабатываем все пиксели изображения
	uchar* sourcePtr, *inputPtr, *diffPtr;
	for(int row = 0; row < this->NRows; row++) {
		//получаем указатели на строки
		sourcePtr = this->sourceImage.ptr<uchar>(row);
		inputPtr = inputImage.ptr<uchar>(row);
		diffPtr = this->diffImage.ptr<uchar>(row);

		//для каждого пикселя
		//если пиксели не совпадают
		//делаем их белыми
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
	//приведем к оттенкам серого
	Mat greyMat;
	if(img.channels() == 3){
		cv::cvtColor(img, greyMat, CV_BGR2GRAY);
	} else {
		img.copyTo(greyMat);
	}

	//фильтруем изображение
	greyMat = filterImage(greyMat);
	
	return greyMat;
}

void ImgDiffer::setSourceImage(Mat& img)
{
	//проверим наличие данных
	if(!img.data){
		throw (new InvalidInputImageException());
	}

	//присвоим изображение свойству класса
	this->sourceImage = this->prepareImage(img);

	//рассчитаем значение количества столбцов и строк
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
	//подготавливаем изображение для обработки
	Mat filteredImage = this->prepareImage(inputImage);

	//сравниваем изображение с оригиналом
	Mat differenceImage = this->countDifference(filteredImage);

	//магическим образом удаляем шум и лишние контуры
	//посредством фильтрации и морфологических преобразований
	differenceImage = approveEdgesOfImage(differenceImage);
	differenceImage = filterImage(differenceImage, false);
	differenceImage = approveEdgesOfImage(differenceImage);

	//производим пороговое преобразование
	Mat binaryDifference = this->tresholdDifference(differenceImage);

	//освобождаем память
	filteredImage.release();
	differenceImage.release();

	return binaryDifference;
}