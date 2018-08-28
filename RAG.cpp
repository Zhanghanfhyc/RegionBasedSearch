#include "stdafx.h"
#include "RAG.h"


int RAG::layer = 0;




//成员函数实现
RAG::RAG(IplImage* labelMat, IplImage* pSrc, string imgName) {

	RegionImg = cvCreateImage(cvGetSize(pSrc), IPL_DEPTH_8U, 3);//watershed color image
	uchar* pColor = (uchar*)RegionImg->imageData;
	for(int y=0;y<RegionImg->height;y++)
		for (int x = 0;x < RegionImg->width;x++) {
			pColor[y*RegionImg->widthStep + x * 3] = 255;
			pColor[y*RegionImg->widthStep + x * 3+1] = 255;
			pColor[y*RegionImg->widthStep + x * 3+2] = 255;
		}
	regionCount = 0;

	unsigned short int* plabel = (unsigned short int*)labelMat->imageData;
	int step = labelMat->widthStep / sizeof(unsigned short int);


	for (int y = 0;y < labelMat->height;++y)
	{
		for (int x = 0;x <labelMat->width;++x) {
			//cout << "(" << x << "," << y << ")" << endl;
			if (plabel[x + y * step] != 0) {

				int lab = plabel[x + y * step];
				map<int, Region>::iterator iter = regionMap.find(lab);

				if (iter != regionMap.end())//如果map中已存在该label
					iter->second.pixels.push_back(Point(x, y));
				else//不存在
				{
					regionMap.insert(pair<int, Region>(lab, Region(x, y, plabel[x + y * step],layer)));
					regionCount++;
				}
			}
		}
	}
	
	//===================显示分水岭图像===============================

	for (map<int, Region>::iterator iter = regionMap.begin(), end = regionMap.end();iter != end;iter++) {
		iter->second.getValue(pSrc, pSrc->nChannels, pSrc->widthStep);
		for (list<Point>::iterator pixelIter = iter->second.pixels.begin(), pixelEnd = iter->second.pixels.end();pixelIter != pixelEnd;pixelIter++) {
			pColor[pixelIter->x*RegionImg->nChannels + pixelIter->y*RegionImg->widthStep] = iter->second.value[0];
			pColor[pixelIter->x*RegionImg->nChannels + pixelIter->y*RegionImg->widthStep + 1] = iter->second.value[1];
			pColor[pixelIter->x*RegionImg->nChannels + pixelIter->y*RegionImg->widthStep + 2] = iter->second.value[2];
		}
	}
	
	createRegionAdjList(labelMat);
	
	name = imgName + "_layer" + to_string(layer) + ".jpg"; cvSaveImage(name.c_str(), RegionImg);
	//cvShowImage(name.c_str(), RegionImg);
	layer++;

}

void RAG::createRegionAdjList(IplImage* labelMat) {

	unsigned short int* plabel = (unsigned short int*)labelMat->imageData;
	int step = labelMat->widthStep / sizeof(unsigned short int);
	int labA;int labB;
	stack<int> labels;
	unsigned short* curLabel = 0;
	unsigned short* preLabel = 0;
	stack<Point> stack_washPoints;
	for (int y = 0;y < labelMat->height;++y)//horizontal scan
	{
		preLabel = &plabel[y * step];
		while (!stack_washPoints.empty())stack_washPoints.pop();
		for (int x = 0;x < labelMat->width;++x) {
			if (plabel[x + y * step] == 0)
				if (stack_washPoints.size() <= 2)
					stack_washPoints.push(Point(x, y));
				else {
					while (!stack_washPoints.empty())
						stack_washPoints.pop();
					preLabel = &plabel[x + y * step];
				}
			else if (plabel[x + y * step] != 0)
			{
				if (*preLabel == 0) preLabel = &plabel[x + y * step];
				else if (*preLabel != plabel[x + y * step]) {
					addAdj(*preLabel, plabel[x + y * step], &stack_washPoints);
					preLabel = &plabel[x + y * step];
				}
			}
		}
	}

	for (int x = 0;x < labelMat->width;++x)//vertical scan
	{
		preLabel = &plabel[x];
		while (!stack_washPoints.empty())stack_washPoints.pop();
		for (int y = 0;y < labelMat->height;++y) {
			if (plabel[x + y * step] == 0)
				if (stack_washPoints.size() <= 2)
					stack_washPoints.push(Point(x, y));
				else {
					while (!stack_washPoints.empty())
						stack_washPoints.pop();
					preLabel = &plabel[x + y * step];
				}
			else if (plabel[x + y * step] != 0)
			{
				if (*preLabel == 0) preLabel = &plabel[x + y * step];
				else if (*preLabel != plabel[x + y * step]) {
					addAdj(*preLabel, plabel[x + y * step], &stack_washPoints);
					preLabel = &plabel[x + y * step];
				}
			}
		}
	}

}

void RAG::addAdj(int labA, int labB, stack<Point>* s_washPoints) {
	//Region：A, Edge:B
	Region* pa = &regionMap.find(labA)->second;
	Region* pb = &regionMap.find(labB)->second;
	pa->addEdge(labB, pb);
	while (!s_washPoints->empty()) {
		pa->getEdge(labB)->addWashPoint(s_washPoints->top());
		s_washPoints->pop();
	}
	//Region：B, Edge:A
	pb->addEdge(labA, pa);
	while (s_washPoints->size()>0) {
		pb->getEdge(labA)->addWashPoint(s_washPoints->top());
		s_washPoints->pop();
	}
}


int RAG::getRegionLabel(int x, int y, IplImage* labelmat) {
	unsigned short int* plabel = (unsigned short int*)labelmat->imageData;
	int step = labelmat->widthStep / sizeof(unsigned short int);
	return plabel[x + y * step];
}


RAG::~RAG()
{
}
