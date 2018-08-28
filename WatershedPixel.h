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
		neighbours.reserve(8);//�洢8������ͨ������
	}

	WatershedPixel() {//�������ص�Ĺ��캯��
		label = FICTITIOUS;
	}

	void addNeighbour(WatershedPixel* neighbour) {//�����������
		neighbours.push_back(neighbour);
	}
	std::vector<WatershedPixel*>& getNeighbours() {//��ȡ��������
		return neighbours;
	}

	//��ȡ���ػҶȺ�����
	char getHeight() const { return height; }
	int getIntHeight() const { return (int)height & 0xff; }
	int getX() const { return x; }
	int getY() const { return y; }

	//���úͻ�ȡ��ǩ
	void setLabel(int label) { this->label = label; }
	void setLabelToINIT() { label = INIT; }
	void setLabelToMASK() { label = MASK; }
	void setLabelToWSHED() { label = WSHED; }
	int getLabel() { return label; }

	// �жϵ�ǰ��ǩ״̬
	bool isLabelINIT() { return label == INIT; }
	bool isLabelMASK() { return label == MASK; }
	bool isLabelWSHED() { return label == WSHED; }

	// ���úͻ�ȡ����
	void setDistance(int distance) { dist = distance; }
	int getDistance() { return dist; }

	// �ж��Ƿ�Ϊ�������� 
	bool isFICTITIOUS() { return label == FICTITIOUS; }

	//�ж��Ƿ�������������Ϊ��ˮ��
	bool allNeighboursAreWSHED() {
		for (unsigned i = 0; i < neighbours.size(); i++) {
			WatershedPixel* r = neighbours.at(i);
			if (!r->isLabelWSHED()) return false;
		}
		return true;
	}
};
#endif // !WATERSHEDPIXEL_H
