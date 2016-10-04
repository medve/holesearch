#include "stdafx.h"
#include "StrikeExam.h"

class SessionIsNotReadyException: public exception
{
	virtual const char* what() const throw()
	{
		return "Session is not ready!";
	}
};

void StrikeExam::initStrike(int strikesNum)
{
	this->searcher = HoleSearcher();
	this->rater = HoleRater();
	this->isSceneInited = false;
	this->results.holeRates = new int[strikesNum]; 
}

StrikeExam::StrikeExam()
{
	this->strikesNum = 3;
	initStrike(this->strikesNum);
	cv::Scalar color(30, 10, 100);
	this->setContourColor(color);
}

StrikeExam::StrikeExam(int strikesNum)
{
	this->strikesNum = strikesNum;
	initStrike(this->strikesNum);
}

StrikeExam::~StrikeExam(void)
{
	delete[] this->results.holeRates;
}

void StrikeExam::initStrikingSession(Mat& sourceImg)
{
	this->stopStrikingSession();

	//������� ������ ����������� � ������, ��� ������
	this->setSourceImage(sourceImg);

	//��������� �������
	this->rater.caliber(sourceImg);

	//������������� ���� ������������� �����
	this->isSceneInited = true;
}

void StrikeExam::stopStrikingSession()
{
	//�������� ���������� ���������� ��������
	//this->rater.wipeResults();

	//�������� ������ ����� � ��������
	this->wipeContours();
	this->wipePoints();

	//������������� ���� ������������� �����
	this->isSceneInited = false;
}

void StrikeExam::highlightHoles()
{
	Mat src = this->currentImage;

	//������� �������� ��������� ���� � �� �������
	Mat dst;
	src.copyTo(dst);

	for(int idx=0; idx >= 0; idx = this->holeContoursHierarchy[idx][0]) {
		cv::Scalar color = this->getContourColor();
		drawContours(dst, this->holeContours, idx, color, 
			                 CV_FILLED, 8, holeContoursHierarchy);
	}

	this->highlightedImage = dst;
}

void StrikeExam::getResults(int* holeRates, int* match)
{
	//�������� ���� �� ������
	vector<Point2f> holes = this->getHolesBySession();
	
	//��������� ����
	this->rater.rateHoles(holes, holeRates, this->strikesNum);
	*match = this->rater.getMatch(holeRates, this->strikesNum);
}

void StrikeExam::findPoints()
{
	this->searcher
		.findHolesOnImage(this->holeContours, this->holeCenters);
}

void StrikeExam::findContours()
{
	this->searcher
		.findHoleContoursOnImage(
		              this->currentImage, 
					  this->holeContours, 
					  this->holeContoursHierarchy
			);
}

void StrikeExam::newInputImage(Mat& inputImage)
{
	if(!isSceneInited){
		throw (new SessionIsNotReadyException());
	}

	//�������� ��������� �����������
	//������������� ��� � currentImage
	this->setCurrentImage(inputImage);

	//��������� ���������
	this->findContours();
	//this->findPoints();

	//������������ �����������
	this->highlightHoles();

	//������������� ����������� � sourceImage
	this->setSourceImage(inputImage);
}

void StrikeExam::setSourceImage(Mat& inputImage)
{
	this->sourceImage = inputImage;

	//������������� �������� ����������� � HoleSearcher
	this->searcher.setSourceImage(inputImage);
}

vector<cv::Point2f> StrikeExam::getHolesBySession()
{
	return this->holeCenters;
}

vector<vector<Point>> StrikeExam::getContoursBySession()
{
	return this->holeContours;
}

Mat StrikeExam::getSourceImage()
{
	return this->sourceImage;
}

Mat StrikeExam::getHighlightedImage()
{
	return this->highlightedImage;
}

Mat StrikeExam::getCurrentImage()
{
	return this->currentImage;
}

void StrikeExam::setCurrentImage(Mat& inputImage)
{
	this->currentImage = inputImage;
}

void StrikeExam::setStrikesNum(int strikesNum)
{
	this->strikesNum = strikesNum;
}

int StrikeExam::getStrikesNum()
{
	return this->strikesNum;
}

void StrikeExam::wipeContours()
{
	this->holeContours.clear();
}

void StrikeExam::wipePoints()
{
	this->holeCenters.clear();
}

cv::Scalar StrikeExam::getContourColor()
{
	return this->contourColor;
}

void StrikeExam::setContourColor(cv::Scalar& color)
{
	this->contourColor = color;
}

//void StrikeExam::setImageCenter(Point center)
//{
//
//}
//
//Point StrikeExam::getImageCenter()
//{
//
//}