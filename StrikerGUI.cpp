#include "stdafx.h"
#include "StrikerGUI.h"

#include "FileImgTaker.h"
#include "CaliberGUI.h"
#include <Windows.h>

using namespace std;
using namespace cv;

string MAIN_WINDOW = "Стрельба 4680";
string MATCH_WINDOW = "Оценки за стрельбу";


StrikerGUI::StrikerGUI(void)
{
	this->se = StrikeExam::getInstance();
	this->taker = FileImgTaker();
}


StrikerGUI::~StrikerGUI(void)
{

}


void StrikerGUI::start()
{
	// создаем основное окно
	namedWindow(MAIN_WINDOW);
	//добавляем кнопки и события

	this->newSession(0, NULL);

	//добавляем события и кнопки
	//показываем результаты
	cv::createButton("Match", showMatchAndHolesList, NULL, CV_PUSH_BUTTON, 1);
	
	//калибровка
	cv::createButton("Caliber", caliber, NULL, CV_PUSH_BUTTON, 1);

	//new session
	cv::createButton("NewSession", newSession, NULL, CV_PUSH_BUTTON, 1);
}

void StrikerGUI::stop()
{
	//закрываем сессию
	this->se->stopStrikingSession();
	//удаляем окно
	destroyAllWindows();
}

void StrikerGUI::newInputImage()
{
	this->se->newInputImage(this->taker.getImage());
	imshow(MAIN_WINDOW, this->se->getHighlightedImage());
}

void StrikerGUI::showMatchAndHolesList(int state, void*)
{
	vector<float> holeRates;
	int match;
	this->se->getResults(holeRates, &match);
	for(int i = 0; i < holeRates.size(); i++){
		displayOverlay(MATCH_WINDOW, to_string(holeRates[i]),2);
		//TODO:событие удаления
		//deletehole
	}
	displayOverlay(MATCH_WINDOW, to_string(match));
}

void StrikerGUI::deleteHole(int state, void*)
{

}

void StrikerGUI::stopSession()
{
	this->se->stopStrikingSession();
	destroyWindow(MATCH_WINDOW);
}

void StrikerGUI::newSession(int state, void*)
{
	this->se->initStrikingSession(this->taker.getImage());
	imshow(MAIN_WINDOW, this->se->getSourceImage());
	this->sessionStarted = true;

	//каждый интервал времени получаем изображение, обрабатываем и показываем его
	//!!!!!!!!!!!!
	//TODO:таймер событие!!!
	//!!!!!!!!!!!
	while(this->sessionStarted){
		this->newInputImage();
		Sleep(5000);
	}
	
}

void StrikerGUI::caliber(int state, void*)
{
	CaliberGUI caliberController = CaliberGUI();
	caliberController.showCaliberImage();
}