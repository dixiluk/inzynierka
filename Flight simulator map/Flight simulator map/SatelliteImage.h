#pragma once
#include "Texture.h"

class SatelliteImage
{
	friend class Chunk;
private:

public:
	char* source;
	Texture* texture;
	SatelliteImage();
	SatelliteImage(char* source);
	~SatelliteImage();
};

