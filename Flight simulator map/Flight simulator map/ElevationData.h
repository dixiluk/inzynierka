#pragma once
#include "Coordinate.h"
#include <iostream>

class ElevationData
{
private:
	Coordinate southWest,northEast;

public:
	static short rows, cols;
	std::string source;
	float ** heights;
	int sourceSize;
	Coordinate ** coordinates;
	ElevationData(Coordinate southWest, Coordinate northEast, std::string source, int sourceSize);
	~ElevationData();
	void saveOnDrive(std::string path);
	static ElevationData* readFromDrive(Coordinate southWest, Coordinate northEast,std::string path);
};

