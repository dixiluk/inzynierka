#include "SatelliteImage.h"



SatelliteImage::SatelliteImage()
{
	this->texture = new Texture();
}

SatelliteImage::SatelliteImage(char* source)
{
	this->texture = new Texture();
	this->source = source;
}

SatelliteImage::~SatelliteImage()
{
}
