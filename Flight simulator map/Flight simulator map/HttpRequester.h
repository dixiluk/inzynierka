#pragma once
#include <iostream>
#include <winsock2.h>

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
	char* getSatelliteImageSource(Coordinate center, int zoom, int sizeX, int sizeY, std::string format, std::string* out_metadata = 0);

private:
	void receiveHeader();
	void receiveChunkSource();
	std::string getSatelliteImageMetadata(Coordinate center, int zoom, int sizeX, int sizeY, std::string format); // zwracam to jak podasz coœ w out_metadata

};

