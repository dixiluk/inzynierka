#pragma once
#include "Texture.h"

class SatelliteImage
{
	friend class Chunk;
private:

public:
	Texture* texture;
	SatelliteImage();
	~SatelliteImage();
};

