#include "stdafx.h"
#include "RMG.h"

RMG::RMG(map<int, Region>* RAGMap, IplImage* preLabelMat, double threshold, int Label)
{
	meanRegionSize = preLabelMat->height*preLabelMat->width / RAGMap->size();
	newlabelMat = cvCreateImage(cvGetSize(preLabelMat), IPL_DEPTH_16U, 1);
	cvZero(newlabelMat);
	maxLabel = Label;
	for (map<int, Region>::iterator mapIt = RAGMap->begin(), end = RAGMap->end();mapIt != end;mapIt++) {
		if (mapIt->second.isMerged == false)
		{
			list<Edge> edgeList = mapIt->second.edges;
			list<Edge>::iterator edgeListIt = edgeList.begin();
			int minIndex = 0, adjMinIndex = 0;
			float minDistance = 999, adjMinDistance = 999;
			while (edgeListIt != edgeList.end()) {
				float distance = distanceFunction(mapIt->second, *edgeListIt->edgeRegion);
				if (distance < threshold&&distance < minDistance) {
					minDistance = distance;
					minIndex = edgeListIt->regionIndex;
				}
				edgeListIt++;
			}//find the nearest edge

			if (minIndex != 0) {
				Region minRegion = RAGMap->find(minIndex)->second;
				if (minRegion.isMerged == false)
				{
					list<Edge> edgeList = minRegion.edges;
					if (edgeList.size() != 0) {
						for (list<Edge>::iterator edgeListIt = edgeList.begin(), end = edgeList.end();edgeListIt != end;edgeListIt++) {
							float distance = distanceFunction(mapIt->second, *edgeListIt->edgeRegion);
							if (distance < adjMinDistance) {
								adjMinDistance = distance;
								adjMinIndex = edgeListIt->regionIndex;
							}

						}
					}
				}
			}
			if (minIndex*adjMinIndex != 0 && adjMinIndex == mapIt->second.regionIndex) {
				maxLabel++;
				vector<Region> mergeRegions;
				mapIt->second.label = maxLabel;
				mapIt->second.isMerged = true;
				mapIt->second.getEdge(minIndex)->edgeRegion->label = maxLabel;
				mapIt->second.getEdge(minIndex)->edgeRegion->isMerged = true;

				mergeRegions.push_back(mapIt->second);
				mergeRegions.push_back(*mapIt->second.getEdge(minIndex)->edgeRegion);
				binaryMergeMap.insert(pair<int, vector<Region>>(maxLabel, mergeRegions));
			}
		}

	}

	//draw on newlabelmat
	unsigned short* pNewLabel = (unsigned short*)newlabelMat->imageData;
	int width = newlabelMat->width;
	int height = newlabelMat->height;
	int step = newlabelMat->widthStep / sizeof(unsigned short int);

	for (map<int, Region>::iterator iter = RAGMap->begin(), end = RAGMap->end();iter != end;iter++) {
		for (list<Point>::iterator pixelsIt = iter->second.pixels.begin(), end = iter->second.pixels.end();pixelsIt != end;pixelsIt++) {
			pNewLabel[pixelsIt->y*step + pixelsIt->x] = iter->second.label;
		}
	}
	//delate	unsigned short* preLabel = 0;
	unsigned short* preLabel = 0;
	stack<Point> stack_washPoints;
	for (int y = 0;y < height;++y)//horizontal scan
	{
		while (!stack_washPoints.empty())stack_washPoints.pop();
		preLabel = &pNewLabel[y * step];
		for (int x = 0;x < width;++x) {
			if (pNewLabel[x + y * step] == 0)
				if (stack_washPoints.size() < 2)
					stack_washPoints.push(Point(x, y));
				else
				{
					while (!stack_washPoints.empty())stack_washPoints.pop();
					preLabel = &pNewLabel[x + y * step];
				}

			else if (pNewLabel[x + y * step] != 0)
			{
				if (*preLabel == 0)
					preLabel = &pNewLabel[x + y * step];
				else if (*preLabel != pNewLabel[x + y * step]) {
					preLabel = &pNewLabel[x + y * step];
					while (!stack_washPoints.empty())stack_washPoints.pop();
				}
				else if (*preLabel == pNewLabel[x + y * step]) {
					while (!stack_washPoints.empty())
					{
						Point washPoint = stack_washPoints.top();
						pNewLabel[washPoint.x + washPoint.y*step] = pNewLabel[x + y * step];
						stack_washPoints.pop();
					}
				}
			}
		}
	}
	for (int x = 0;x < width;++x)//vertical scan
	{
		while (!stack_washPoints.empty())stack_washPoints.pop();
		preLabel = &pNewLabel[x];
		for (int y = 0;y < height;++y) {
			if (pNewLabel[x + y * step] == 0)
				if (stack_washPoints.size() < 2)
					stack_washPoints.push(Point(x, y));
				else
				{
					while (!stack_washPoints.empty())stack_washPoints.pop();
					preLabel = &pNewLabel[x + y * step];
				}

			else if (pNewLabel[x + y * step] != 0)
			{
				if (*preLabel == 0)
					preLabel = &pNewLabel[x + y * step];
				else if (*preLabel != pNewLabel[x + y * step]) {
					preLabel = &pNewLabel[x + y * step];
					while (!stack_washPoints.empty())stack_washPoints.pop();
				}
				else if (*preLabel == pNewLabel[x + y * step]) {
					while (!stack_washPoints.empty())
					{
						Point washPoint = stack_washPoints.top();
						pNewLabel[washPoint.x + washPoint.y*step] = pNewLabel[x + y * step];
						stack_washPoints.pop();
					}
				}
			}
		}
	}

}

float RMG::distanceFunction(Region r1, Region r2) {
	//float value = abs(valueA[0] + valueA[1] + valueA[2] - valueB[0] - valueB[1] - valueB[2]);
	Scalar valueA = BGR2HSV(r1.value);
	Scalar valueB = BGR2HSV(r2.value);
	float value;
	if (valueA[0] == valueB[0] && valueA[1] == valueB[1] && valueA[2] == valueB[2])
		value = 1;
	else value =sqrt(pow((valueA[0] - valueB[0]), 2) + pow((valueA[1] - valueB[1]), 2) + pow((valueA[2] - valueB[2]), 2));

	return value;
}



Scalar BGR2HSV(Scalar bgrColor) {
	float b = bgrColor[0];
	float g = bgrColor[1];
	float r = bgrColor[2];
	float max = (b > g) ? b : g;
	max = (max > r) ? max : r;
	float min = (b < g) ? b : g;
	min = (min < r) ? min : r;

	float h = 0;
	float s = 0;
	float v = 0;
	//h
	if (max == min)h = 0;
	else if (max == r) {
		if(g>b)h = 60 * (g - b) / (max - min); 
		else if (b > g)h = 60 * (g - b) / (max - min) + 360;
	}
	else if (max == g) { h = 60 * (b - r) / (max - min) + 120; }
	else if (max == b) { h = 60 * (r - g) / (max - min) + 240; }
	//if (h < 0)h = h + 360;
	//v
	v = max;
	//s
	s = ((max - min) / max) * 255;
	Scalar hsvScalar(h, s, v);
	return hsvScalar;
}

RMG::~RMG()
{
}