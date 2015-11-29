#pragma once
#include "Coordinate.h"

class ElevationData
{
private:
	short rows, cols;
	Coordinate southWest,northEast;

public:
	float ** heights;
	ElevationData(short rows,short cols, Coordinate westSouth, Coordinate northEast, char* source);
	~ElevationData();
};

