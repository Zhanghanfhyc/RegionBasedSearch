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
	static const int HMIN = 0;//最小层
	static const int HMAX = 256;//最大层
	


public:
	IplImage * labelMat;
	int curlab = 0;//当前label
	IplImage* pWS;
	 void run(IplImage* pSrc, const std::string& imgName);
};

#endif // 
