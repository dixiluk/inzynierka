//#include "Environment.h"
//#include "Engine.h"
#include "Environment.h"
#include "Camera.h"
#include <iostream>
#include "HttpRequester.h"
#include "Chunk.h"
#include <Windows.h>
#include "Config.h"
#include <string>
#include "SatelliteImageMetadata.h"
#include "MapLoader.h"

int main(int argc, char * argv[])
{	
	MapLoader::Instance = new MapLoader();

	WSADATA wsaData;
	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
		std::cout << "WSAStartup failed.\n";
		system("pause");
		exit(-1);
	}
	
	Config *tmp = new Config("config.txt");
	Chunk::loadLevelOfDetail();
	ElevationData::rows = Config::Instance->takeConfigInt("elevationRows");
	ElevationData::cols = Config::Instance->takeConfigInt("elevationCols");
	HttpRequester::prefImageSizeX = Config::Instance->takeConfigInt("prefImageSizeX");
	HttpRequester::prefImageSizeY = Config::Instance->takeConfigInt("prefImageSizeY");
	Chunk::saveDataOnDrive = Config::Instance->takeConfigBool("saveDataOnDrive");
	Chunk::minHorizont = Config::Instance->takeConfigInt("minHorizont");
	MapLoader::Instance->createThread(1);
	
	GraphicalEngine::Instance = new GraphicalEngine(GraphicalEngine::Resolution(1440, 900));
	GraphicalEngine::Init(argc, argv);

	Chunk::Shader = new ChunkShader();

	GraphicalEngine::Instance->worldChunk1 = new Chunk(Coordinate(-80, 0), Coordinate(80, 180), NULL);
	GraphicalEngine::Instance->worldChunk2 = new Chunk(Coordinate(-80, -180), Coordinate(80, 0), NULL);

	//Camera *camera = new Camera(glm::vec3(-6578, 0, 0)); 

	double earthRadius = 63784.10;
	Camera *camera = new Camera(glm::vec3(
		(float)(earthRadius + 1000)*sin(52.25)*sin(21), (float)(earthRadius +1000)*cos(52.25)*sin(21), (float)(earthRadius + 1000)*cos(21)));
	camera->setActive();

	glutMainLoop();

	return 0;
}