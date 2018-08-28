#pragma once
#ifndef WATERSHEDALGORITHM_H
#define WATERSHEDALGORITHM_H

#include<iostream>
#include <opencv/cv.h>
#include <opencv2/highgui.hpp>
#include <string>
#include <queue>
#include"WatershedStructure.h"
#include"WatershedPixel.h"


class WatershedAlgorithm {
	static const int HMIN = 0;//��С��
	static const int HMAX = 256;//����
	


public:
	IplImage * labelMat;
	int curlab = 0;//��ǰlabel
	IplImage* pWS;
	 void run(IplImage* pSrc, const std::string& imgName);
};

#endif // 
