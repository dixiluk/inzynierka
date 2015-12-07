#pragma once
#include <iostream>
#include <winsock2.h>
#include <windows.h>
#pragma comment(lib,"ws2_32.lib")
#include "ElevationData.h"
#include "Coordinate.h"

class HttpRequester
{
private:
	std::string key, server;
	SOCKET connectionSocket;
	std::string source;
	std::string header;
	char last[6];
	char* requestBuffer;
public:
	HttpRequester(std::string server, std::string key);
	~HttpRequester();
	
	ElevationData* getElevationData(Coordinate northEast, Coordinate westSouth, short rows, short cols, std::string height);
	void getSatelliteImage(Coordinate center, int zoom, int sizeX, int sizeY, std::string format, bool metadata);
};

