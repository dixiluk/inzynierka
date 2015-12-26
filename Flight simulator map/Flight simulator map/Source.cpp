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

	ElevationData::rows = Config::Instance->takeConfigInt("elevationRows");
	ElevationData::cols = Config::Instance->takeConfigInt("elevationCols");
	HttpRequester::prefImageSizeX = Config::Instance->takeConfigInt("prefImageSizeX");
	HttpRequester::prefImageSizeY = Config::Instance->takeConfigInt("prefImageSizeY");
	Chunk::saveDataOnDrive = Config::Instance->takeConfigBool("saveDataOnDrive");

	MapLoader::Instance->createThread(200);
	//std::cout << (tmp->takeConfigInt("par3"));

	/*for (int i = 1; i < 10; i++) {
		for (int j = 1; j < 10; j++)
		SatelliteImage *tmpstring = t->getSatelliteImageSource(Coordinate(0, 0), Coordinate((double)(0.5/i), (double)(0.5/j)), 800, 800, "png");
	}*/
//	new SatelliteImageMetadata(tmpstring);
	
	GraphicalEngine::Instance = new GraphicalEngine(GraphicalEngine::Resolution(1440, 900));
	GraphicalEngine::Init(argc, argv);

	Chunk::Shader = new ChunkShader();

	GraphicalEngine::Instance->worldChunk1 = new Chunk(Coordinate(-80, 0), Coordinate(80, 180), NULL);
	//GraphicalEngine::Instance->worldChunk2 = new Chunk(Coordinate(-80, -180), Coordinate(80, 0),NULL);
	//GraphicalEngine::Instance->worldChunk1 = new Chunk(Coordinate(-50, 0), Coordinate(-40, 11.25), NULL);
	//GraphicalEngine::Instance->worldChunk1 = new Chunk(Coordinate(10, 133), Coordinate(60, 178), NULL);

	Scene* scene = new Scene();
	GraphicalEngine::Instance->activeScene = (Scene*)scene;
	Camera *camera = new Camera(glm::vec3(-20, -1, -13));
	camera->setActive();

	glutMainLoop();

	return 0;
}