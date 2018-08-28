#include "stdafx.h"
#include"RegionTree.h"
#include<queue>
#include"RMG.h"

void RegionTree::addBottomLayer(map<int, Region>* pRAGMap) {
	topLayer = *pRAGMap;
}
void RegionTree::addLayer(map<int, Region>* pRAGMap, map<int, vector<Region>> mergeMap) {
	pTopLayer = pRAGMap;
	for (map<int, vector<Region>>::iterator iter = mergeMap.begin(), end = mergeMap.end();iter != end;iter++) {
		Region* pRegion = &(pTopLayer->find(iter->first)->second);


		pRegion->parent1 = new Region(topLayer.find(iter->second[0].regionIndex)->second);

		pRegion->parent2 = new Region(topLayer.find(iter->second[1].regionIndex)->second);

	}
	topLayer = *pTopLayer;

}


list<Region*> RegionTree::BreadthFirstSearch(CvScalar value) {

	//=======================
	queue<Region*> q;
	int curLayer = topLayer.begin()->second.layer;
	list<Region*>targetList;

	//=======================
	if (topLayer.size() == 0)return targetList;
	for (map<int, Region>::iterator iter = topLayer.begin(), end = topLayer.end();iter != end;iter++) {
		q.push(&iter->second);
	}
	while (!q.empty()) {
		Region* r = q.front();

		int i = r->layer;
		cout << i;

		if (r->layer != curLayer) {
			if (targetList.size() != 0) {
				return targetList;
			}
			else curLayer = r->layer;
		}

		if (r->parent1 != nullptr) {
			q.push(r->parent1);
		}
		if (r->parent2 != nullptr) {
			q.push(r->parent2);
		}


		if (r->value.val[0] > (value.val[0] - 5) && r->value.val[0] < (value.val[0] + 5))
			if (r->value.val[1] > (value.val[1] - 5) && r->value.val[1] < (value.val[1] + 5))
				if (r->value.val[2] > (value.val[2] - 5) && r->value.val[2] < (value.val[2] + 5))
					targetList.push_back(r);

		q.pop();
	}
}

list<Region*> RegionTree::BreadthFirstSearch(int value) {

	//=======================
	queue<Region*> q;
	int curLayer = topLayer.begin()->second.layer;
	list<Region*>targetList;

	//=======================
	if (topLayer.size() == 0)return targetList;
	for (map<int, Region>::iterator iter = topLayer.begin(), end = topLayer.end();iter != end;iter++) {
		q.push(&iter->second);
	}
	while (!q.empty()) {
		Region* r = q.front();
		Scalar rHSV = BGR2HSV(r->value);
		int i = r->layer;
		cout << i;

		if (r->layer != curLayer) {
			if (targetList.size() != 0) {
				return targetList;
			}
			else curLayer = r->layer;
		}

		if (r->parent1 != nullptr) {
			q.push(r->parent1);
		}
		if (r->parent2 != nullptr) {
			q.push(r->parent2);
		}


		if (rHSV.val[0] > (value - 10) && rHSV.val[0] < (value + 10))
			targetList.push_back(r);

		q.pop();
	}
}

list<Region*> RegionTree::BreadthFirstSearch(int value,int adjValue) {

	//=======================
	queue<Region*> q;
	int curLayer = topLayer.begin()->second.layer;
	list<Region*>targetList;

	//=======================
	if (topLayer.size() == 0)return targetList;
	for (map<int, Region>::iterator iter = topLayer.begin(), end = topLayer.end();iter != end;iter++) {
		q.push(&iter->second);
	}
	while (!q.empty()) {
		Region* r = q.front();
		Scalar rHSV = BGR2HSV(r->value);
		int i = r->layer;
		cout << i;

		if (r->layer != curLayer) {
			if (targetList.size() != 0) {
				return targetList;
			}
			else curLayer = r->layer;
		}

		if (r->parent1 != nullptr) {
			q.push(r->parent1);
		}
		if (r->parent2 != nullptr) {
			q.push(r->parent2);
		}


		if (rHSV.val[0] > (value - 15) && rHSV.val[0] < (value + 15))
		{
			if(rHSV.val[0] > (adjValue - 15) && rHSV.val[0] < (adjValue + 15))
				targetList.push_back(r);
		}

		q.pop();
	}
}


