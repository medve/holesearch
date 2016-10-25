#include "stdafx.h"
#include "CaliberGUI.h"

std::string CALIBER_WINDOW = "Калибровка"; 

CaliberGUI::CaliberGUI(void)
{
	this->exam = StrikeExam::getInstance();
	this->curPoint = 0;
	this->circlesNum = this->exam->getCirclesNum();
	this->pointsNum = this->circlesNum + 3;
	this->otherCircles = new Point2f[this->pointsNum];
}


CaliberGUI::~CaliberGUI(void)
{
	delete[] this->otherCircles;
	this->caliberSource.empty();
}

void CaliberGUI::startCaliber(int state, void* a)
{
	this->caliberSource = exam->getSourceImage();

	cv::namedWindow(CALIBER_WINDOW);

	this->refreshImage();

	//добавим событие по клику на картинку
	cv::setMouseCallback(CALIBER_WINDOW, addPoint, 0);

	//добавить кнопку отмены действия
	cv::createButton("Cancel step", cancelStep, NULL, CV_PUSH_BUTTON, 1);

	//добавить кнопку полной отмены
	cv::createButton("Cancel", cancelCaliber, NULL, CV_PUSH_BUTTON, 1);
}
void CaliberGUI::cancelCaliber(int state, void* a)
{
	this->curPoint = 0;
	cv::destroyWindow(CALIBER_WINDOW);
}

void CaliberGUI::saveCaliber()
{
	Triangle centerCircle;
	centerCircle.a = this->otherCircles[0];
	centerCircle.b = this->otherCircles[1];
	centerCircle.c = this->otherCircles[2];
	this->exam->caliberRater(centerCircle, this->otherCircles + 3);
	this->curPoint = 0;
	cv::destroyWindow(CALIBER_WINDOW);
}

void CaliberGUI::cancelStep(int state, void* a)
{
	if(this->curPoint > 0){
		this->curPoint --;
	}
	this->refreshImage();
}

void CaliberGUI::addPoint(int event, int x, int y, int , void* a)
{
	if(event != cv::EVENT_LBUTTONDOWN){
		return ;
	}

	if(this->curPoint < this->pointsNum )
	{
		cv::Point2f coords = cv::Point2f(x,y);

		this->otherCircles[this->curPoint] = coords;
		this->curPoint++;
		this->refreshImage();
	} else {
		this->saveCaliber();
	}
}

void CaliberGUI::refreshImage()
{
	Mat drawedImage;
	this->caliberSource.copyTo(drawedImage);

	//выводим точки на исходное изображение
	float radius = 3.0f;
	cv::Scalar color(30, 10, 100);
	int thickness = 1;
	int lineType = 8;
	for(int i = 0; i < this->curPoint; i++){
		cv::circle(drawedImage, 
					this->otherCircles[i],
					radius,
					color,
				    thickness,
				    lineType
			);
	}

	cv::imshow(CALIBER_WINDOW, drawedImage);
}


void CaliberGUI::showCaliberImage()
{
	// покажем изображение с кругами калибровки
	cv::imshow(CALIBER_WINDOW, exam->drawCaliber());

	//добавим кнопку калибровать
	cv::createButton("Caliber", startCaliber, NULL, CV_PUSH_BUTTON, 1);
}
