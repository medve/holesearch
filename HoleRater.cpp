#include "stdafx.h"
#include "HoleRater.h"


HoleRater::HoleRater(void)
{
	//заглушка
}


HoleRater::~HoleRater(void)
{
	//заглушка
}

float HoleRater::getHoleToCenter(Point hole)
{
	Point2f center = this->getCenter();
	float x, y;
	x = (float)hole.x - center.x;
	y = (float)hole.y - center.y;
	
	return sqrt(x*x + y*y);
}

int HoleRater::rateHole(Point hole)
{
	return this->getMatchByTable(this->getHoleToCenter(hole), this->holesMatchTable, 
		                         this->holesMatchNum, this->holesMinMatch);
}

void HoleRater::rateHoles(vector<Point2f>& holesList, int* holes,
		                                   int strikesNum)
{
	for( int hole_idx = 0; hole_idx < strikesNum; hole_idx++) {
		if(hole_idx < holesList.size()) {
			holes[hole_idx] = rateHole(holesList[hole_idx]);
		} else {
			holes[hole_idx] = 0;
		}
	}
}

void HoleRater::getMatchRateTable(int * matchTable)
{
	for(int i = 0; i < this->matchNum; i++) {
		matchTable[i] = this->matchTable[i];
	}
}

void HoleRater::getHoleRateTable(int * holesMatchTable)
{
	for(int i = 0; i < this->holesMatchNum; i++) {
		holesMatchTable[i] = this->holesMatchTable[i];
	}
}

void HoleRater::setMatchRateParams(int * matchTable, int matchNum, int minMatch)
{
	this->minMatch = minMatch;
	this->matchTable = matchTable;
	this->matchNum = matchNum;
}

void HoleRater::setHoleRateParams(int * holesMatchTable, int holesMatchNum, int holesMinMatch)
{
	this->holesMinMatch = holesMinMatch;
	this->holesMatchTable = holesMatchTable;
	this->holesMatchNum = holesMatchNum;
}

void HoleRater::setMatchMatchTable(int * matchTable, int matchNum)
{

	this->matchTable = matchTable;
	this->matchNum = matchNum;
}

void HoleRater::setHolesMatchTable(int * holesMatchTable, int holesMatchNum)
{
	this->holesMatchTable = holesMatchTable;
	this->holesMatchNum = holesMatchNum;
}

void HoleRater::setMatchNum(int matchNum)
{
	this->matchNum = matchNum;
}

void HoleRater::setHolesMatchNum(int holesMatchNum)
{
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

int HoleRater::getMatch(int* holes, int strikesNum)
{
	//найдем сумму очков по дыркам
	int pointSum = 0;
	for( int i = 0; i < strikesNum; i++) {
		pointSum += holes[i];
	}
	
	return this->getMatchByTable(pointSum, this->matchTable, 
		                         this->matchNum, this->minMatch);
}

int HoleRater::getMatchByTable(int points, int* matchTable, int matchNum, int minMatch)
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

void HoleRater::caliber(Mat& sourceImg)
{
	//заглушка
}
