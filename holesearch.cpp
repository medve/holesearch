#include "stdafx.h"

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <iostream>
#include <string>
#include "StrikeExam.h"

using namespace cv;
using namespace std;

#ifndef min
#define min(a,b)            (((a) < (b)) ? (a) : (b))
#endif


int main(int argc, char* argv[])
{
	// дефолтные названия картинок для обработки
	string file1("C:\\Users\\Admin\\Documents\\Visual Studio 2012\\Projects\\holesearch\\Debug\\tbn.jpg");
	string file2("C:\\Users\\Admin\\Documents\\Visual Studio 2012\\Projects\\holesearch\\Debug\\tn.jpg");

	// получаем картинку
	Mat img1 = imread(file1, IMREAD_GRAYSCALE);
	cout<<"[i] first image: %s\n"<<file2;

	// получаем картинку
	Mat img2 = imread(file2, IMREAD_GRAYSCALE);
	cout<<"[i] second image: %s\n"<<file2;

	if(!img1.data){
		cout<<"[!] cant load image: %s\n"<< file1;
		return -1;
	}
	if(!img2.data){
		cout<<"[!] cant load image: %s\n"<<file2;
		return -1;
	}

	if(img1.size[0]!=img2.size[0] || img1.size[1]!=img2.size[1]){
		cout<<"[!] different image size!\n";
		return -2;
	}

	// покажем изображения
	namedWindow( "image1");
	imshow( "image1", img1 );
	namedWindow( "image2");
	imshow( "image2", img2 );

	StrikeExam se = StrikeExam();
	se.initStrikingSession(img1);
	se.newInputImage(img2);
	Mat diff = se.getHighlightedImage();

	// выводим результат
	namedWindow( "highlighted");
	imshow( "highlighted", diff );

	// ждём нажатия клавиши
	waitKey(0);

	//освобождаем ресурсы
	img1.release();
	img2.release();
	diff.release();

	destroyAllWindows();

	return 0;
}