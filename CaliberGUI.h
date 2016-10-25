#pragma once

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <vector>
#include <string>
#include "StrikeExam.h"

class CaliberGUI
{
private:
	StrikeExam* exam;
	cv::Point2f* points;
	int pointsNum;
	int curPoint;
	int circlesNum;
	cv::Point2f* otherCircles;
	Mat caliberSource;

	cv::ButtonCallback startCaliber(int state, void*);
	cv::ButtonCallback cancelCaliber(int state, void*);
	void saveCaliber();
	cv::ButtonCallback cancelStep(int state, void*);
	void addPoint(int event, int x, int y, int , void*);
	void refreshImage();
public:
	CaliberGUI(void);
	~CaliberGUI(void);

	void showCaliberImage();

};

