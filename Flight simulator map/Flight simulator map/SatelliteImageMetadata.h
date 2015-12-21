#pragma once
#include <iostream>
#include "Coordinate.h"

class SatelliteImageMetadata
{
public:
	SatelliteImageMetadata(std::string data);
	~SatelliteImageMetadata();
	std::string data;
	void readPositions();
	void readImageSize();
	void readTwoMarkers();
	short firstMarkerx, firstMarkery, secondMarkerx, secondMarkery;
	Coordinate southWest, northEast;
	short imageHeight, imageWidth;
	
};

