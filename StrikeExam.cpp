#include "stdafx.h"
#include "StrikeExam.h"

StrikeExam* StrikeExam::instance = 0;
const int NUM_CIRCLES = 5;

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
	this->rater = HoleRater(NUM_CIRCLES);
	this->isSceneInited = false;
}

StrikeExam::StrikeExam()
{
	initStrike(this->strikesNum);
	cv::Scalar color(30, 10, 100);
	this->setContourColor(color);
}

StrikeExam::StrikeExam(int strikesNum)
{
	this->strikesNum = strikesNum;
	initStrike(this->strikesNum);
}

StrikeExam* StrikeExam::getInstance()
{
	if(instance == 0){
		instance = new StrikeExam;
	}
	return instance;
}

StrikeExam::~StrikeExam(void)
{
	this->wipeContours();
	this->wipePoints();
}

void StrikeExam::initStrikingSession(Mat& sourceImg)
{
	this->stopStrikingSession();

	//������� ������ ����������� � ������, ��� ������
	this->setSourceImage(sourceImg);

	//������������� ���� ������������� �����
	this->isSceneInited = true;
}

void StrikeExam::caliberRater(Triangle center_circle, Point2f* other_circles)
{
	//��������� �������
	this->rater.caliber(center_circle, other_circles);
}

int StrikeExam::getCirclesNum()
{
	return NUM_CIRCLES;
}

Mat StrikeExam::drawCaliber()
{
	Mat circledImage;
	this->sourceImage.copyTo(circledImage);
	int circlesNum = this->rater.getHolesMatchNum();
	float* circleRadiuses = new float[circlesNum];
	Point2f center = rater.getCenter();
	int thickness = 1;
	int lineType = 8;
	for(int i = 0; i < circlesNum;i++) {
		cv::circle(circledImage, 
		           center,
				   circleRadiuses[i],
				   this->getContourColor(),
				   thickness,
				   lineType);
	}
	delete[] circleRadiuses;
	return circledImage;
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

void StrikeExam::getResults(vector<float>& holeRates, int* match)
{
	//�������� ���� �� ������
	vector<Point2f> holes = this->getHolesBySession();
	
	//��������� ����
	this->rater.rateHoles(holes, holeRates);
	*match = this->rater.getMatch(holeRates);
}

void StrikeExam::findPoints()
{
	vector<Point2f> points; 
	this->searcher
		.findHolesOnImage(this->holeContours, points);

	//�������� ����� � ��������� �����
	for(int i = 0; i < points.size(); i++){
		this->holeCenters.push_back(points[i]);
	}
	points.clear();
}

void StrikeExam::findContours()
{
	vector<vector<Point>> contours;
	vector<cv::Vec4i> ch;

	this->searcher
		.findHoleContoursOnImage(
		              this->currentImage, 
					  contours, 
					  ch
			);

	//�������� ������� � ��������� ��������
	for(int i = 0; i < contours.size(); i++){
		this->holeContours.push_back(contours[i]);
	}

	for(int i = 0; i < ch.size(); i++){
		this->holeContoursHierarchy.push_back(ch[i]);
	}


	contours.clear();
	ch.clear();
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
	this->findPoints();
	this->strikesNum = this->holeCenters.size();

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

int StrikeExam::getStrikesNum()
{
	return this->strikesNum;
}

void StrikeExam::wipeContours()
{
	this->holeContours.clear();
	this->holeContoursHierarchy.clear();
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
