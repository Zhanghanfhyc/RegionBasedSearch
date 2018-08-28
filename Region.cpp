#include "stdafx.h"
#include"Region.h"

bool Region::containEdge(int index) {


	for (list<Edge>::iterator iter = edges.begin(), end = edges.end();iter != end;iter++)
		if (iter->regionIndex == index)
			return true;

	return false;
}
list<Edge>::iterator Region::getEdge(int index) {

	for (list<Edge>::iterator iter = edges.begin(), end = edges.end();iter != end;iter++) {
		if (iter->regionIndex == index)
			return iter;
	}
	return edges.end();
}
Scalar Region::getValue(IplImage* pSrc, int channels, int step) {
	if (pixels.size() != 0) {
		char* p = pSrc->imageData;
		int sumB = 0, sumG = 0, sumR = 0;//Çórbg¾ùÖµ
		for (std::list<Point>::iterator it = pixels.begin(), end = pixels.end();it != end;it++) {
			int b = (uchar)p[it->x * channels + it->y*step];
			int g = (uchar)p[it->x * channels + it->y*step + 1];
			int r = (uchar)p[it->x * channels + it->y*step + 2];

			sumB += b;
			sumG += g;
			sumR += r;
		}
		Scalar rgbvalue(sumB / pixels.size(), sumG / pixels.size(), sumR / pixels.size());
		value = rgbvalue;
		return rgbvalue;
	}
	else {
		value = Scalar(0, 0, 0);
		return value;
	}

}

void Region::addEdge(int index, Region* edgeRegion) {

	if (!containEdge(index)) {
		edges.push_back(Edge(index, edgeRegion));
	}


}

void Region::mergePixels(list<Point> pixels2) {
	bool isContain = false;
	if (pixels2.size() != 0)
		for (list<Point>::iterator iter2 = pixels2.begin(), end2 = pixels2.end();iter2 != end2;iter2++) {
			isContain = false;
			for (list<Point>::iterator iter = pixels.begin(), end = pixels.end();iter != end;iter++)
				if (*iter2 == *iter) { isContain = true;break; }
			if (isContain == false)pixels.push_back(*iter2);
		}
}