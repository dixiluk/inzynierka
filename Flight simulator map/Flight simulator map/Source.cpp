
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
	MapLoader::Instance = new MapLoader((int)Config::Instance->takeConfigDouble("downloadThread"));
	MapLoader::Instance->createThread();
	Chunk::levelOfDetailCheckAccuracy = Config::Instance->takeConfigDouble("levelOfDetailCheckAccuracy");
	Chunk::levelOfDetailCheckPresent = 0;	
	GraphicalEngine::Instance = new GraphicalEngine(GraphicalEngine::Resolution(1440, 900));
	GraphicalEngine::Init(argc, argv);
	Chunk::Shader = new ChunkShader();

	MapLoader::Instance->downloadMapMode(Coordinate(0, 0), Coordinate(0.5, 0.5), 17);

	float hight;
	Coordinate cameraCordinate;
	if (argc == 4){
		cameraCordinate = Coordinate(PI2*(atof(argv[1]) + 90) / 360, PI2*(atof(argv[2]) + 180) / 360);
		hight = atof(argv[3]);
	}

	if (argc == 6) {
		
		MapLoader::Instance->downloadMapMode(Coordinate(atof(argv[1]), atof(argv[2])), Coordinate(atof(argv[3]), atof(argv[4])), atof(argv[5]));
		return 0;
	}
	else {
		hight = 2;
		cameraCordinate = Coordinate(PI2*(54.36 + 90) / 360, PI2*(18.64 + 180) / 360);
	}



	Camera *camera = new Camera(glm::vec3(
		(float)(earthRadius + hight)*sin(cameraCordinate.longitude)*sin(cameraCordinate.latitude),
		(float)(earthRadius + hight)*cos(cameraCordinate.longitude)*sin(cameraCordinate.latitude),
		(float)(earthRadius + hight)*cos(cameraCordinate.latitude)));
	camera->setActive();
	Camera::ActiveCamera->maxHight= Config::Instance->takeConfigDouble("maxCameraHight");
	Camera::ActiveCamera->minHight= Config::Instance->takeConfigDouble("minCameraHight");
	Camera::ActiveCamera->speed= Config::Instance->takeConfigDouble("cameraSpeed");

	std::cout << "Pobieranie mapy.." << std::endl << "pozostalo: ";
	MapLoader::createMap(); 
	int taskListCout = 1;
	while (taskListCout != 0) {
		taskListCout = MapLoader::Instance->readTaskListCount();
		COORD pos = { 11, 1 };
		HANDLE output = GetStdHandle(STD_OUTPUT_HANDLE);
		SetConsoleCursorPosition(output, pos);
		std::cout << taskListCout << "     " << std::endl;
	}
	std::cout << "Pobrano mape.." << std::endl;
	glutMainLoop();

	return 0;
}