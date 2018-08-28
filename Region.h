#pragma once
#pragma once
#ifndef REGION_H
#define REGION_H
#include"stdafx.h"
#include<iostream>
#include <string>
#include <queue>
#include"WatershedStructure.h"
#include"WatershedPixel.h"
#include<list>
#include<vector>
#include<map>



class Edge;
class Region;

class  Edge
{
public:
	int regionIndex;
	Region* edgeRegion;
	list<Point> washPoints;

	Edge(int index, Region* region) {
		regionIndex = index;
		edgeRegion = region;
	}

	void addWashPoint(Point newWashPoint) {

		washPoints.push_back(newWashPoint);
	}

};

class Region {
public:
	int regionIndex;//index in the database/picture tree
	int label;//pixel label of the final isMerged image
	list < cv::Point > pixels;
	list<Edge> edges;
	Scalar value = 0;
	int layer;
	bool isMerged = false;

	Region* parent1=nullptr; Region* parent2=nullptr;
	void addEdge(int index, Region* edgeRegion);
	Scalar getValue(IplImage* pSrc, int channels, int step);
	bool containEdge(int index);
	list<Edge>::iterator getEdge(int index);
	void mergePixels(list<Point> pixel2);

	Region(int x, int y, int label1,int layer1) {
		pixels.push_back(Point(x, y));
		regionIndex = label1;
		label = label1;
		layer = layer1;
	} 

};

#endif // !REG{ION_H