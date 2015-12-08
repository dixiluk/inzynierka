#pragma once
#include "Coordinate.h"
#include "SatelliteImage.h"
#include "ElevationData.h"
#include "ChunkShader.h"

class Chunk
{
public:
	Chunk(Coordinate southWest, Coordinate northEast);
	~Chunk();
	static Chunk *worldChunk;

	Coordinate southWest, northEast;
	short elevationDataAccuracy;
	short satelliteImageAccuracy;	
	ElevationData *elevationData;
	SatelliteImage *satelliteImage;
	static ChunkShader* Shader;
	void draw();
};

