#include "SatelliteImage.h"



SatelliteImage::SatelliteImage()
{
	this->texture = new Texture();
}

SatelliteImage::SatelliteImage(char* source, char* header, SatelliteImageMetadata* metadata)
{
	this->texture = new Texture();
	this->source = source;
	this->header = header;
	this->metadata = metadata;
}

SatelliteImage::~SatelliteImage()
{
}
