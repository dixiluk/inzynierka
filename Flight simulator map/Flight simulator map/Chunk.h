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
	Chunk* child[4];
	bool childExist;
	std::atomic<bool> canHaveChildren;
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

	Chunk(Coordinate southWest, Coordinate northEast, Chunk* parent);
	~Chunk();

	
	static void loadLevelOfDetail();
	Coordinate southWest, northEast;

	ElevationData *elevationData;
	SatelliteImage *satelliteImage;
	static ChunkShader* Shader;
	static bool saveDataOnDrive;
	
	short detailLevel;
	void loadChunk();
	void downloadChunk(HttpRequester* httpRequester);
	void draw();
	bool visible;
	void createChild();
	void removeChild();
	static void loadHttpData();
	bool isChildrenDowdloaded();
	void saveOnDrive(std::string pat);
	double distanceFromCamera();
	short calculatePrefDetailLevel();
	void calculateAllDetails();
	bool Chunk::isChildrenToRemove();
	bool Chunk::isChildrenLoaded();
	void createBeginChunks();
	void loadBeginChunks();
	void loadChildren();
	//Zero
	GLuint vertexArrayId;
	float* textureCoordinants;
	float* vertices;
	int verticesCount;
	unsigned int* indices;
	int indicesSize;
	void calculateVertexArrayTextureCoordinants();
	void calculateVertexArrayVertices();
	void calculateVertexArrayIndices();
	void loadVertexArray();
};

