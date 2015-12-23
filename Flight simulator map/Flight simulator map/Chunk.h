#pragma once
#include "Coordinate.h"
#include "SatelliteImage.h"
#include "ElevationData.h"
#include "ChunkShader.h"
#include <atomic>
#include "HttpRequester.h"
#include "Vertex.h"

class Chunk
{

private:Chunk* parent;
	Vertex **vertices;
	Chunk* child[4];
	bool childExist;
public:
	bool isLoaded;
	std::atomic<bool> isDownloaded;
	Chunk(Coordinate southWest, Coordinate northEast, Chunk* parent);
	~Chunk();
	void test();

	Coordinate southWest, northEast;
	short elevationDataAccuracy;
	short satelliteImageAccuracy;	
	ElevationData *elevationData;
	SatelliteImage *satelliteImage;
	static ChunkShader* Shader;
	short elevationRows, elevationCols;

	void loadChunk();
	void downloadChunk(HttpRequester* httpRequester);
	void draw();
	void createChild();
	static void loadHttpData();
	bool isChildsLoaded();
};

