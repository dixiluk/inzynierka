#pragma once
#include "Texture.h"
#include "SatelliteImageMetadata.h"

class SatelliteImage
{
	friend class Chunk;
private:

public:
	char* source;
	long unsigned int sourceSize;
	int width; int height;
	char* header;
	Texture* texture;
	SatelliteImageMetadata *metadata;
	SatelliteImage();
	SatelliteImage(char* source, int sourceSize , SatelliteImageMetadata* metadata);
	void saveOnDrive(std::string path);
	static SatelliteImage *readFromDrive(std::string path, std::string metadataPath);
	~SatelliteImage();
	bool CheckImage();
};

