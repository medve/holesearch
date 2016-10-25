#pragma once

#include <opencv2/core.hpp>
#include <vector>

using std::vector;
using cv::Mat;
using cv::Point;
using cv::Point2f;

typedef struct {Point2f a;
				Point2f b;
				Point2f c;} Triangle;

class HoleRater
{
private:
	

	int matchNum;
	float * matchTable;
	int minMatch;

	int holesMatchNum;
	float * holesMatchTable;
	int holesMinMatch;

	float getHoleToCenter(Point hole);
	int getMatchByTable(int points, float* matchTable, int matchNum, int minMatch);
	float getDistanceBetweenPoints(Point2f point1, Point2f point2);

	void setMatchNum(int matchNum);
	void setHolesMatchNum(int holesMatchNum);

	int getHolesSum(vector<float>& holes);

	Point2f center;

public:
	int rateHole(Point hole);
	void rateHoles(vector<Point2f>& holesList, vector<float>& holes);

	void setMatchRateParams(float * matchTable, int matchNum, int minMatch);
	//void setHoleRateParams(float * holesMatchTable, int holesMatchNum, int holesMinMatch);

	//void getMatchRateParams(float * matchTable, int* matchNum, int* minMatch);

	void setMatchTable(float * matchTable, int matchNum);

	void setHolesMinMatch(int holesMinMatch);
	void setMinMatch(int minMatch);

	int getMatchNum();
	int getHolesMatchNum();


	void getHoleRateTable(float * holesMatchTable);
	void getMatchRateTable(float * matchTable);

	void setCenter(Point2f);
	Point2f getCenter();
	void caliber(Triangle center_circle, Point2f* other_circles);
	int getMatch(vector<float>& holes);


	HoleRater(const int circlesNum = 5);
	~HoleRater(void);
};

