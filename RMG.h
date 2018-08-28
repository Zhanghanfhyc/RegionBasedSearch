#ifndef RMG_H
#define RMG_H
#include"Region.h"
#include<stack>
#include<map>
#include<fstream>

Scalar BGR2HSV(Scalar bgrColor);

class RMG
{
public:
	RMG(map<int, Region>* RAGMap, IplImage* preLabelMat, double threshold1, int maxLabel);
	~RMG();
	IplImage* newlabelMat;
	map<int, Region*> RMGMap;
	float distanceFunction(Region r1, Region r2);
	map<int, vector<Region>> binaryMergeMap;
	int maxLabel;
	int meanRegionSize;

};



#endif // !RMG_H