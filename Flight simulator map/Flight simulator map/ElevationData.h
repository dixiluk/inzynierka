#pragma once
#include "Coordinate.h"
#include <iostream>

class ElevationData
{
private:
	short rows, cols;
	Coordinate southWest,northEast;

public:
	float ** heights;
	ElevationData(short rows,short cols, Coordinate southWest, Coordinate northEast, std::string source);
	~ElevationData();
};

