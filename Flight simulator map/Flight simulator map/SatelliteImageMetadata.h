#pragma once
#include <iostream>
#include "Coordinate.h"

class SatelliteImageMetadata
{
public:
	int sourceSize;
	std::string source;
	short *markers[2];
	short imageHeight, imageWidth;

	SatelliteImageMetadata(std::string source,int sourceSize);
	~SatelliteImageMetadata();
	short markersCount;
	Coordinate southWest, northEast;;
	void readImageSize();
	void readMarkers();
	short sizeX();
	short sizeY();
	void saveOnDrive(std::string path);

	void SatelliteImageMetadata::readPositions();
	static SatelliteImageMetadata* readFromDrive(std::string path);
	
};

