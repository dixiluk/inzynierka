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
	if (file == NULL);
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
		return NULL;
	}

	fseek(file, 0, SEEK_END);
	lSize = ftell(file);
	rewind(file);


	buffer = (char*)malloc(sizeof(char)*lSize);

	result = fread(buffer, 1, lSize, file);
	if (result != lSize) {
		free(buffer);
		return NULL;
	}

	fclose(file);

	SatelliteImageMetadata * tmp = SatelliteImageMetadata::readFromDrive(metadataPath);
	if (tmp == NULL)
		return NULL;
	return new SatelliteImage(buffer, lSize,tmp);
	
}



SatelliteImage::~SatelliteImage()
{
	delete this->texture;
	delete this->metadata;

}

bool SatelliteImage::CheckImage() {
	int counter = 0;
	if (this->width < 0 || this->height < 0)
		return true;
	for (int i = 0; i < this->width*this->height * 4; i += 4) {
		if ((short)this->source[i] == -13 && (short)this->source[i + 1] == -14 && (short)this->source[i + 2] == -19)
			counter++;
	}
	double filled = (double)counter / (this->width*this->height);
	if(filled>0.1)
		return false;
	return true;
}