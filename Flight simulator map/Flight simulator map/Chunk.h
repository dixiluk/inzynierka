#pragma once
#include "Coordinate.h"
#include "SatelliteImage.h"
#include "ElevationData.h"
class Chunk
{
public:
	Chunk();
	~Chunk();
	Coordinate southWest, northEast;
	short elevationDataAccuracy;
	short satelliteImageAccuracy;	
	ElevationData *elevationData;
	SatelliteImage *satelliteImage;
};

