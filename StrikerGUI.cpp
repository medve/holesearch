#include "stdafx.h"
#include "StrikerGUI.h"

#include "FileImgTaker.h"
#include "CaliberGUI.h"
#include <Windows.h>

using namespace std;
using namespace cv;

string MAIN_WINDOW = "�������� 4680";
string MATCH_WINDOW = "������ �� ��������";


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
	// ������� �������� ����
	namedWindow(MAIN_WINDOW);
	//��������� ������ � �������

	this->newSession(0, NULL);

	//��������� ������� � ������
	//���������� ����������
	cv::createButton("Match", showMatchAndHolesList, NULL, CV_PUSH_BUTTON, 1);
	
	//����������
	cv::createButton("Caliber", caliber, NULL, CV_PUSH_BUTTON, 1);

	//new session
	cv::createButton("NewSession", newSession, NULL, CV_PUSH_BUTTON, 1);
}

void StrikerGUI::stop()
{
	//��������� ������
	this->se->stopStrikingSession();
	//������� ����
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
		//TODO:������� ��������
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

	//������ �������� ������� �������� �����������, ������������ � ���������� ���
	//!!!!!!!!!!!!
	//TODO:������ �������!!!
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