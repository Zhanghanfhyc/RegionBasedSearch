#pragma once
#pragma once

#ifndef REGIONADJACENCYGRAPH_H
#define WATERSHEDALGORITHM_H
#include "stdafx.h"

#include <string>
#include <queue>
#include"WatershedStructure.h"
#include"WatershedPixel.h"
#include"Region.h"
#include<list>
#include<vector>
#include<map>
#include<stack>
#include<cmath>




class RAG
{
public:
	RAG(IplImage* labelMat, IplImage* pSrc, string imgName);
	~RAG();

	void createRegionAdjList(IplImage* labelMat);

	std::map<int, Region> regionMap;
	int regionCount;
	static int layer;
	IplImage* RegionImg;
private:
	int getRegionLabel(int x, int y, IplImage* labelmat);
	void addAdj(int labA, int labB, stack<Point>* s_washPoints);
	string name;


};

#endif // !REGIONADJACENCYGRAPH_H


