#pragma once
#include "Coordinate.h"
#include "SatelliteImage.h"
#include "ElevationData.h"
#include "ChunkShader.h"
#include <atomic>
#include "HttpRequester.h"
#include "Vertex.h"
#include "Camera.h"

class Chunk
{

private:Chunk* parent;
	Vertex **vertices;
	Chunk* child[4];
	bool childExist;
public:
	static short levelOfDetailCheckAccuracy;
	static short levelOfDetailCheckPresent;
	short levelOfDetailCheck;
	bool isLoaded;
	std::atomic<bool> isDownloaded;
	Chunk(Coordinate southWest, Coordinate northEast, Chunk* parent);
	~Chunk();
	void test();
	static short LevelOfDetailCount;
	static double LevelOfDetail[];
	static void loadLevelOfDetail();
	Coordinate southWest, northEast;
	short elevationDataAccuracy;
	short satelliteImageAccuracy;	
	ElevationData *elevationData;
	SatelliteImage *satelliteImage;
	static ChunkShader* Shader;
	static bool saveDataOnDrive;
	short detailLevel;
	void loadChunk();
	void downloadChunk(HttpRequester* httpRequester);
	void draw();
	bool toRemove;
	static double minHorizont;
	bool visible;
	void createChild();
	void removeChild();
	static void loadHttpData();
	bool isChildsLoaded();
	void saveOnDrive(std::string pat);
	double distanceFromCamera();
	short calculatePrefDetailLevel();
	void calculateAllDetails();
};

