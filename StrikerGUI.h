#pragma once

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <iostream>
#include <string>
#include "StrikeExam.h"
#include "FileImgTaker.h"

class StrikerGUI
{
private:
	void showHighlightedImage();
	void newInputImage();
	void showMatchAndHolesList(int state, void*);
	void deleteHole(int state, void*);
	void stopSession();
	void newSession(int state, void*);
	void caliber(int state, void*);

	StrikeExam* se;
	FileImgTaker taker;
	bool sessionStarted;
	
public:
	StrikerGUI(void);
	~StrikerGUI(void);

	void start();
	void stop();

};

