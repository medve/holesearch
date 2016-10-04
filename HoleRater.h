#pragma once

#include <opencv2/core.hpp>
#include <vector>

using std::vector;
using cv::Mat;
using cv::Point;
using cv::Point2f;

class HoleRater
{
private:
	float getHoleToCenter(Point hole);
	int getMatchByTable(int points, int* matchTable, int matchNum, int minMatch);

	int matchNum;
	int * matchTable;
	int minMatch;

	int holesMatchNum;
	int * holesMatchTable;
	int holesMinMatch;

	Point2f center;

public:
	int rateHole(Point hole);
	void rateHoles(vector<Point2f>& holesList, int* holes,
		                                   int strikesNum);

	void setMatchRateParams(int * matchTable, int matchNum, int minMatch);
	void setHoleRateParams(int * holesMatchTable, int holesMatchNum, int holesMinMatch);

	void getMatchRateParams(int * matchTable, int* matchNum, int* minMatch);
	void getHoleRateParams(int * holesMatchTable, int* holesMatchNum, int* holesMinMatch);

	void setHolesMatchTable(int * holesMatchTable, int holesMatchNum);
	void setMatchMatchTable(int * matchTable, int matchNum);

	void setHolesMinMatch(int holesMinMatch);
	void setMinMatch(int minMatch);

	void setMatchNum(int matchNum);
	void setHolesMatchNum(int holesMatchNum);

	int getMatchNum();
	int getHolesMatchNum();

	void getHoleRateTable(int * holesMatchTable);
	void getMatchRateTable(int * matchTable);

	void setCenter(Point2f);
	Point2f getCenter();
	void caliber(Mat& sourceImg);
	int getMatch(int* holes, int strikesNum);


	HoleRater(void);
	~HoleRater(void);
};

