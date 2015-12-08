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


int main(int argc, char * argv[])
{
	WSADATA wsaData;
	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
		std::cout << "WSAStartup failed.\n";
		system("pause");
		exit(-1);
	}
	Config *tmp = new Config("config.txt");
	std::cout << (tmp->takeConfigInt("par3"));
	
	GraphicalEngine::Instance = new GraphicalEngine(GraphicalEngine::Resolution(1440, 900));
	GraphicalEngine::Init(argc, argv);

	Chunk::Shader = new ChunkShader();

	GraphicalEngine::Instance->worldChunk = new Chunk(Coordinate(0, 0), Coordinate(5, 5),NULL);

	Scene* scene = new Scene();
	GraphicalEngine::Instance->activeScene = (Scene*)scene;
	Camera *camera = new Camera(glm::vec3(10, -1, -7));
	camera->setActive();

	glutMainLoop();

	HttpRequester *t = new HttpRequester("dev.virtualearth.net", "AvLyPxYc5C5cPPAwZdsrhI1c4sT9FJo1AUVym7tgs-IvZzo720jrDdn-ZG-0Jrb9");

	int x = 0;
	long int i = 0;

	int time1 = 0;
	int time2 = 0;
	for (int p = 0; p < 2000; p++) {
		i = GetTickCount();
		t->getElevationData(Coordinate(0, 0), Coordinate(5, 5), 32, 32, "sealevel");
		std::cout << "Time 1:" << GetTickCount() - i << std::endl;
		//Sleep(1000);

		i = GetTickCount();
		//t2->getSatelliteImage(Coordinate(49, -122), 7, 1000, 1000, "jpeg", 0);
		std::cout << "Time 1:" << GetTickCount() - i << std::endl;
		break;
	}

	return 0;
}