#pragma once

#include <opencv2/core/core.hpp>
#include <vector>
#include <string>
#include "HoleSearcher.h"
#include "HoleRater.h"

using std::string;
using std::vector;
using cv::Mat;
using cv::Point;

typedef struct {
	int match;
	int* holeRates;
} StrikeResults;

class StrikeExam
{
private:
	void highlightHoles();
	Mat currentImage;
	Mat sourceImage;
	Mat highlightedImage;
	vector<cv::Point2f> holeCenters;
	vector<vector<Point>> holeContours;
	vector<cv::Vec4i> holeContoursHierarchy;

	vector<cv::Point2f> getHolesBySession();
	vector<vector<Point>> getContoursBySession();

	void wipeContours();
	void wipePoints();

	void setSourceImage(Mat& inputImage);
	void setCurrentImage(Mat& inputImage);

	void findPoints();
	void findContours();

	void initStrike(int strikesNum);

	HoleSearcher searcher;
	HoleRater rater;
	bool isSceneInited;
	int strikesNum;
	StrikeResults results;
	cv::Scalar contourColor;

public:
	//��������� ���������� ����������� � � ������ ��������� �����
	//�� ��������� ��� ���������
	void initStrikingSession(Mat& sourceImg);
	void stopStrikingSession();

	void setStrikesNum(int strikesNum);
	int getStrikesNum();

	cv::Scalar getContourColor();
	void setContourColor(cv::Scalar& color);
	
	Mat getSourceImage();
	Mat getCurrentImage();
	Mat getHighlightedImage();

	void newInputImage(Mat& inputImage);

	void getResults(int* holeRates, int* match);

	StrikeExam();
	StrikeExam::StrikeExam(int strikesNum);
	~StrikeExam(void);

	//void setImageCenter(Point center);
	//Point getImageCenter();
};

