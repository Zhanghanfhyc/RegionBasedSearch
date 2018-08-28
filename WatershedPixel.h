#pragma once
#ifndef WATERSHEDPIXEL_H
#define WARERSHEDPIXEL_H

#include<vector>



class WatershedPixel {

	static const int INIT = -1;
	static const int MASK = -2;
	static const int WSHED = 0;
	static const int FICTITIOUS = -3;

	int x;
	int y;
	char height;//Gray value
	int label;
	int dist;//distance

	std::vector<WatershedPixel*> neighbours;

public:
	WatershedPixel(int x, int y, char height) {
		this->x = x;
		this->y = y;
		this->height = height;
		label = INIT;
		dist = 0;
		dist = 0;
		neighbours.reserve(8);//存储8领域联通的像素
	}

	WatershedPixel() {//虚拟像素点的构造函数
		label = FICTITIOUS;
	}

	void addNeighbour(WatershedPixel* neighbour) {//添加领域像素
		neighbours.push_back(neighbour);
	}
	std::vector<WatershedPixel*>& getNeighbours() {//获取邻域像素
		return neighbours;
	}

	//获取像素灰度和坐标
	char getHeight() const { return height; }
	int getIntHeight() const { return (int)height & 0xff; }
	int getX() const { return x; }
	int getY() const { return y; }

	//设置和获取标签
	void setLabel(int label) { this->label = label; }
	void setLabelToINIT() { label = INIT; }
	void setLabelToMASK() { label = MASK; }
	void setLabelToWSHED() { label = WSHED; }
	int getLabel() { return label; }

	// 判断当前标签状态
	bool isLabelINIT() { return label == INIT; }
	bool isLabelMASK() { return label == MASK; }
	bool isLabelWSHED() { return label == WSHED; }

	// 设置和获取距离
	void setDistance(int distance) { dist = distance; }
	int getDistance() { return dist; }

	// 判断是否为虚拟像素 
	bool isFICTITIOUS() { return label == FICTITIOUS; }

	//判断是否所有邻域像素为分水岭
	bool allNeighboursAreWSHED() {
		for (unsigned i = 0; i < neighbours.size(); i++) {
			WatershedPixel* r = neighbours.at(i);
			if (!r->isLabelWSHED()) return false;
		}
		return true;
	}
};
#endif // !WATERSHEDPIXEL_H
