#pragma once
#include <iostream>
#include <winsock2.h>

#pragma comment(lib,"ws2_32.lib")
#include "ElevationData.h"
#include "Coordinate.h"
#include "SatelliteImage.h"
#include "SatelliteImageMetadata.h"

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

	ElevationData* getElevationData(Coordinate southWest, Coordinate northEast, short rows, short cols, std::string height);
	SatelliteImage* getSatelliteImageSource(Coordinate southWest, Coordinate northEast, int sizeX, int sizeY, std::string format);
	SatelliteImageMetadata* getSatelliteImageMetadata(Coordinate southWest, Coordinate northEast, int sizeX, int sizeY, std::string format); // zwracam to jak podasz co� w out_metadata
private:
	void receiveHeader();
	void receiveChunkSource();

};

