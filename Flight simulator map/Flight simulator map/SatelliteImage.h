#pragma once
#include "Texture.h"
#include "SatelliteImageMetadata.h"

class SatelliteImage
{
	friend class Chunk;
private:

public:
	char* source;
	char* header;
	Texture* texture;
	SatelliteImageMetadata *metadata;
	SatelliteImage();
	SatelliteImage(char* source, char* header, SatelliteImageMetadata* metadata);
	~SatelliteImage();
};

