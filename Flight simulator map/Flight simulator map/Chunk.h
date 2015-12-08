#pragma once
#include "Coordinate.h"
#include "SatelliteImage.h"
#include "ElevationData.h"
#include "ChunkShader.h"

class Chunk
{
private:Chunk* parent;
	Chunk* child[4];
	bool childExist;
public:
	Chunk(Coordinate southWest, Coordinate northEast, Chunk* parent);
	~Chunk();

	
	Coordinate southWest, northEast;
	short elevationDataAccuracy;
	short satelliteImageAccuracy;	
	ElevationData *elevationData;
	SatelliteImage *satelliteImage;
	static ChunkShader* Shader;

	void draw();
	void createChild();
	static void loadHttpData();
};

