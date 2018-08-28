#pragma once
#include"Region.h"
class RegionTree {
public:
	map<int, Region> topLayer;
	map<int, Region>* pTopLayer;
	void addBottomLayer(map<int, Region>* pRAGMap);
	void addLayer(map<int, Region>* pRAGMap, map<int, vector<Region>> mergeMap);
	list<Region*> BreadthFirstSearch(CvScalar value);
	list<Region*> BreadthFirstSearchHSV(CvScalar value);
	list<Region*> BreadthFirstSearch(int value);
	list<Region*> BreadthFirstSearch(int value, int adjValue);
};

