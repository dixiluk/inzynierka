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

	float hight = 1;
	//Coordinate cameraCordinate = Coordinate(PI2*(52.23 + 90) / 360, PI2*(21 + 180) / 360);
	Coordinate cameraCordinate = Coordinate(PI2*(49.36 + 90) / 360, PI2*(20 + 180) / 360);
	//Coordinate cameraCordinate = Coordinate(PI2*(27.98327226 + 90) / 360, PI2*(86.93383789 + 180) / 360); //himalaje
	Camera *camera = new Camera(glm::vec3(
		(float)(earthRadius + hight)*sin(cameraCordinate.longtitude)*sin(cameraCordinate.latitude),
		(float)(earthRadius + hight)*cos(cameraCordinate.longtitude)*sin(cameraCordinate.latitude),
		(float)(earthRadius + hight)*cos(cameraCordinate.latitude)));
	camera->setActive();
	Camera::ActiveCamera->maxHight= Config::Instance->takeConfigDouble("maxCameraHight");
	Camera::ActiveCamera->minHight= Config::Instance->takeConfigDouble("minCameraHight");
	Camera::ActiveCamera->speed= Config::Instance->takeConfigDouble("cameraSpeed");
	MapLoader::createMap();

	glutMainLoop();

	return 0;
}