#pragma once
#ifndef WATERSHEDSTRUCTURE_H
#define WATERSHEDSTRUCTURE_H

#include <algorithm>
#include "WatershedPixel.h"

class WatershedStructure {
	std::vector<WatershedPixel*> watershedStructure;

public:
	WatershedStructure(char* pixels, int width, int height);
	~WatershedStructure();
	int size();// number of pixels
	WatershedPixel* at(int i);// return pixel i;
};
#endif // !WATERSHEDSTRUCTURE_H

