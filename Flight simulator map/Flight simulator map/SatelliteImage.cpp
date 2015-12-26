#include "SatelliteImage.h"


SatelliteImage::SatelliteImage()
{
	this->texture = new Texture();
}

SatelliteImage::SatelliteImage(char* source, int sourceSize, SatelliteImageMetadata* metadata)
{
	this->texture = new Texture();
	this->source = source;
	this->sourceSize = sourceSize;
	this->metadata = metadata;
}

void SatelliteImage::saveOnDrive(std::string path)
{
	FILE* file = fopen(path.c_str(), "wb");
	if (file == NULL)
		std::cout << "error" << std::endl;
	fwrite(this->source, 1, this->sourceSize, file);
	fclose(file);
}
SatelliteImage *SatelliteImage::readFromDrive(std::string path, std::string metadataPath)
{

	long lSize;
	char * buffer;
	size_t result;

	FILE* file = fopen(path.c_str(), "rb");

	if (file == NULL) {
		std::cout << "error" << std::endl;
		return NULL;
	}

	fseek(file, 0, SEEK_END);
	lSize = ftell(file);
	rewind(file);


	buffer = (char*)malloc(sizeof(char)*lSize);
	if (buffer == NULL) {
		std::cout << "error" << std::endl;
		return NULL;
	}

	result = fread(buffer, 1, lSize, file);
	if (result != lSize) {
		std::cout << "error" << std::endl;
		return NULL;
	}



	SatelliteImageMetadata * tmp = SatelliteImageMetadata::readFromDrive(metadataPath);
	if (tmp == NULL)return NULL;
	return new SatelliteImage(buffer, lSize,tmp);
	
}



SatelliteImage::~SatelliteImage()
{
}
