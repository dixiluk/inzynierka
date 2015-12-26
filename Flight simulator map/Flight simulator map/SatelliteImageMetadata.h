#pragma once
#include <iostream>
#include "Coordinate.h"

class SatelliteImageMetadata
{
public:
	SatelliteImageMetadata(std::string source,int sourceSize);
	~SatelliteImageMetadata();
	int sourceSize;
	std::string source;
	void readPositions();
	void readImageSize();
	void readMarkers();
	short sizeX();
	short sizeY();
	short *markers[2];
	short markersCount;
	Coordinate southWest, northEast;
	short imageHeight, imageWidth;
	void saveOnDrive(std::string path);
	static SatelliteImageMetadata* readFromDrive(std::string path);
	
};

