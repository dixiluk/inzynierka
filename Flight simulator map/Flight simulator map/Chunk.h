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

private:
	Chunk* parent;
	Chunk* children[4];
	bool childrenExist;
	bool isLoaded;
	bool toRemove;
	std::atomic<bool> isDownloaded;
	std::atomic<bool> isNowDownloading;
	static short LevelOfDetailCount;
	static double LevelOfDetail[];
public:
	static short levelOfDetailCheckAccuracy;
	static short levelOfDetailCheckPresent;
	short levelOfDetailCheck;


	Coordinate southWest, northEast;
	ElevationData *elevationData;
	SatelliteImage *satelliteImage;
	static ChunkShader* Shader;
	static bool saveDataOnDrive;
	short detailLevel;
	bool visible;

	Chunk(Coordinate southWest, Coordinate northEast, Chunk* parent);
	~Chunk();
	static void loadLevelOfDetail();
	void loadChunk();
	void downloadChunk(HttpRequester* httpRequester);
	void draw();
	void createChildren();
	void removeChildren();
	static void loadHttpData();
	bool isChildrenDowdloaded();
	GLuint vertexArrayId;
	float* textureCoordinants;
	float* vertices;
	int verticesCount;
	unsigned int* indices;
	int indicesSize;
	
	void saveOnDrive(std::string pat);
	double distanceFromCamera();
	short calculatePrefDetailLevel();
	void calculateAllDetails();
	bool Chunk::isChildrenToRemove();
	bool Chunk::isChildrenLoaded();
	void createBeginChunks();
	void loadBeginChunks();
	void downloadModecreateBeginChunks(Coordinate p1, Coordinate p2, short bestLevel);
	void downloadModeloadBeginChunks();
	void downloadModeloadClear();
	void loadChildren();
	void calculateVertexArrayTextureCoordinants();
	void calculateVertexArrayVertices();
	void calculateVertexArrayIndices();
	void loadVertexArray();
};

