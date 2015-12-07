#include "Environment.h"
#include "Engine.h"
#include "Environment.h"
#include "Camera.h"
#include <iostream>
#include "HttpRequester.h"
#include <Windows.h>

int main(int argc, char * argv[])
{
	Engine::Instance = new Engine(Engine::Resolution(1440, 900));

	Engine::Init(argc, argv);

	Scene* scene = new Scene();
	Engine::Instance->activeScene = (Scene*)scene;
	Camera *camera = new Camera(glm::vec3(10, -1, -7));
	camera->setActive();
	glutMainLoop();

	HttpRequester *t = new HttpRequester("dev.virtualearth.net", "AvLyPxYc5C5cPPAwZdsrhI1c4sT9FJo1AUVym7tgs-IvZzo720jrDdn-ZG-0Jrb9");
	HttpRequester *t2 = new HttpRequester("dev.virtualearth.net", "AvLyPxYc5C5cPPAwZdsrhI1c4sT9FJo1AUVym7tgs-IvZzo720jrDdn-ZG-0Jrb9");

	int x = 0;
	long int i = 0;

	int time1 = 0;
	int time2 = 0;
	for (int p = 0; p < 2000; p++) {
		i = GetTickCount();
		t->getElevationData(Coordinate(50, 50), Coordinate(51, 51), 32, 32, "sealevel");
		std::cout << "Time 1:" << GetTickCount() - i << std::endl;
		//Sleep(1000);

		i = GetTickCount();
		t2->getSatelliteImage(Coordinate(49, -122), 7, 1000, 1000, "jpeg", 0);
		std::cout << "Time 1:" << GetTickCount() - i << std::endl;
		break;
	}

	return 0;
}