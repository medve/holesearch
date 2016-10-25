#include "stdafx.h"
#include "HoleRater.h"

//TODO:asserts for empty tables

HoleRater::HoleRater(int circlesNum)
{
	this->setHolesMatchNum(circlesNum);
}


HoleRater::~HoleRater(void)
{
	if(this->holesMatchTable){
		delete[] this->holesMatchTable;
	}

	if(this->matchTable){
		delete[] this->matchTable;
	}
}

float HoleRater::getDistanceBetweenPoints(Point2f point1, Point2f point2)
{
	float x, y;
	x = (float)point1.x - point2.x;
	y = (float)point1.y - point2.y;
	
	return sqrt(x*x + y*y);
}

float HoleRater::getHoleToCenter(Point hole)
{
	Point2f center = this->getCenter();
	
	return this->getDistanceBetweenPoints(hole, center);
}

int HoleRater::rateHole(Point hole)
{
	return this->getMatchByTable(this->getHoleToCenter(hole), this->holesMatchTable, 
		                         this->holesMatchNum, this->holesMinMatch);
}

void HoleRater::rateHoles(vector<Point2f>& holesList, vector<float>& holes)
{
	for( int hole_idx = 0; hole_idx < holesList.size(); hole_idx++) {
		holes.push_back(rateHole(holesList[hole_idx]));
	}
}

void HoleRater::getMatchRateTable(float * matchTable)
{
	for(int i = 0; i < this->matchNum; i++) {
		matchTable[i] = this->matchTable[i];
	}
}

void HoleRater::getHoleRateTable(float * holesMatchTable)
{
	for(int i = 0; i < this->holesMatchNum; i++) {
		holesMatchTable[i] = this->holesMatchTable[i];
	}
}

void HoleRater::setMatchRateParams(float * matchTable, int matchNum, int minMatch)
{
	this->minMatch = minMatch;
	this->setMatchTable(matchTable, holesMatchNum);
}


void HoleRater::setMatchTable(float * matchTable, int matchNum)
{
	if(!this->matchTable){
		delete[] this->matchTable;
	}

	this->matchTable = matchTable;
	this->matchNum = matchNum;
}


void HoleRater::setMatchNum(int matchNum)
{
	this->matchNum = matchNum;
}

void HoleRater::setHolesMatchNum(int holesMatchNum)
{
	if(!this->holesMatchTable){
		delete[] this->holesMatchTable;
	}

	this->holesMatchTable = new float[holesMatchNum];
	this->holesMatchNum = holesMatchNum;
}

void HoleRater::setHolesMinMatch(int holesMinMatch)
{
	this->holesMinMatch = holesMinMatch;
}

void HoleRater::setMinMatch(int minMatch)
{
	this->minMatch = minMatch;
}

int HoleRater::getMatchNum()
{
	return this->matchNum;
}

int HoleRater::getHolesMatchNum()
{
	return this->holesMatchNum;
}

void HoleRater::setCenter(Point2f center)
{
	this->center = center;
}

Point2f HoleRater::getCenter()
{
	return this->center;
}

int HoleRater::getHolesSum(vector<float>& holes){
	//найдем сумму очков по дыркам
	float pointSum = 0;
	for( int i = 0; i < holes.size(); i++) {
		pointSum += holes[i];
	}
	
	return pointSum;
}

int HoleRater::getMatch(vector<float>& holes)
{
	int pointSum = this->getHolesSum(holes);
	return this->getMatchByTable(pointSum, this->matchTable, 
		                         this->matchNum, this->minMatch);
}

int HoleRater::getMatchByTable(int points, float* matchTable, int matchNum, int minMatch)
{
	//оценок мало, поэтому просто используем линейный поиск по 
	//таблице граничных значений для оценок
	int match = 0;
	for(int i = 0; i < matchNum; i++) {
		//находим максимальное значение оценки, которое соответствуем сумме очков
		if(points < matchTable[i]) {
			break;
		}

		match++;
	}

	//делаем так, чтобы оценки начинались с некого минимального значения
	match += minMatch;

	return match;
}

Point2f circleCenterByThreePoints(Triangle t)
{
	float ma = (t.a.y - t.b.y)/(t.a.x - t.b.x);

	float mb = (t.c.y - t.a.y)/(t.c.x - t.a.x);

	float x = (ma*mb*(t.b.y - t.c.y) + mb*(t.b.x + t.a.y) - ma*(t.a.x + t.c.x))/2.0f*(mb-ma);

	float y = -1.0f/mb*( x - (t.a.x + t.c.x)/2.0f) + (t.a.y + t.c.y)/2.0f;

	return Point2f(x, y); 
}

void HoleRater::caliber(Triangle center_circle, Point2f* other_circles)
{
	Point2f center = circleCenterByThreePoints(center_circle);
	this->setCenter(center);

	this->holesMatchTable[0] = this->getDistanceBetweenPoints(center, center_circle.a);
	for(int i = 1; i < this->holesMatchNum; i++){
		holesMatchTable[i] = this->getDistanceBetweenPoints(center, other_circles[i]);
	}
}
