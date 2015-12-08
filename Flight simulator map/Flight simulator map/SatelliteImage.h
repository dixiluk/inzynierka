#pragma once
#include "Texture.h"

class SatelliteImage
{
	friend class Chunk;
private:
	Texture* texture;
public:
	SatelliteImage();
	~SatelliteImage();
};

