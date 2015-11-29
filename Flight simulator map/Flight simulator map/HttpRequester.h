#pragma once
#include "ElevationData.h"

class HttpRequester
{
public:
	HttpRequester();
	~HttpRequester();
	ElevationData getElevationData(Coordinate northEast, Coordinate southWest, short rows, short cols, std::string height, std::string key)

};

