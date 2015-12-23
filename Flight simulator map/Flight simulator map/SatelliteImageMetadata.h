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
	void readMarkers();
	short sizeX();
	short sizeY();
	short *markers[2];
	short markersCount;
	Coordinate southWest, northEast;
	short imageHeight, imageWidth;
	
};

