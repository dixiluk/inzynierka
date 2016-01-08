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

int main(int argc, char * argv[]){

	float hight;
	Coordinate cameraCordinate;
	if (argc == 4){
		cameraCordinate = Coordinate(PI2*(atof(argv[1]) + 90) / 360, PI2*(atof(argv[2]) + 180) / 360);
		hight = atof(argv[3]);
	}
	else {
		hight = 2;
		cameraCordinate = Coordinate(PI2*(49.25 + 90) / 360, PI2*(19.95 + 180) / 360);
	}
	Sleep(5545);
	MapLoader::Instance = new MapLoader();

	WSADATA wsaData;
	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
		std::cout << "WSAStartup failed.\n";
		system("pause");
		exit(-1);
	}
	Config *tmp = new Config("config.txt");
	Chunk::loadLevelOfDetail();
	ElevationData::rows = Config::Instance->takeConfigDouble("elevationRows");
	ElevationData::cols = Config::Instance->takeConfigDouble("elevationCols");
	HttpRequester::prefImageSizeX = Config::Instance->takeConfigDouble("prefImageSizeX");
	HttpRequester::prefImageSizeY = Config::Instance->takeConfigDouble("prefImageSizeY");
	Chunk::saveDataOnDrive = Config::Instance->takeConfigBool("saveDataOnDrive");
	MapLoader::Instance->createThread((int)Config::Instance->takeConfigDouble("downloadThread"));
	Chunk::levelOfDetailCheckAccuracy = Config::Instance->takeConfigDouble("levelOfDetailCheckAccuracy");
	Chunk::levelOfDetailCheckPresent = 0;
	
	GraphicalEngine::Instance = new GraphicalEngine(GraphicalEngine::Resolution(1440, 900));
	GraphicalEngine::Init(argc, argv);
	
	Chunk::Shader = new ChunkShader();

	Camera *camera = new Camera(glm::vec3(
		(float)(earthRadius + hight)*sin(cameraCordinate.longitude)*sin(cameraCordinate.latitude),
		(float)(earthRadius + hight)*cos(cameraCordinate.longitude)*sin(cameraCordinate.latitude),
		(float)(earthRadius + hight)*cos(cameraCordinate.latitude)));
	camera->setActive();
	Camera::ActiveCamera->maxHight= Config::Instance->takeConfigDouble("maxCameraHight");
	Camera::ActiveCamera->minHight= Config::Instance->takeConfigDouble("minCameraHight");
	Camera::ActiveCamera->speed= Config::Instance->takeConfigDouble("cameraSpeed");
	MapLoader::createMap(); 

	glutMainLoop();

	return 0;
}