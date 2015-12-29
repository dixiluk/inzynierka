#include "SatelliteImageMetadata.h"
#include <sstream>
#include <list>
#include <Windows.h>

#include <errno.h>
SatelliteImageMetadata::SatelliteImageMetadata(std::string source, int sourceSize)
{
	this->source = source;
	this->readPositions();
	this->readImageSize();
	this->readMarkers();
	this->sourceSize = sourceSize;
}


SatelliteImageMetadata::~SatelliteImageMetadata()
{
}

void SatelliteImageMetadata::readPositions() {
	std::string source = this->source;
	size_t Position = source.find("bbox");
	source.erase(0, Position + 7);
	Position = source.find("]");
	source.erase(Position, source.size() - Position);
	std::stringstream ss(source);
	std::getline(ss, source, ',');
	this->southWest.latitude = atof(source.c_str());
	std::getline(ss, source, ',');
	this->southWest.longtitude = atof(source.c_str());
	std::getline(ss, source, ',');
	this->northEast.latitude = atof(source.c_str());
	std::getline(ss, source, ',');
	this->northEast.longtitude = atof(source.c_str());

}

void SatelliteImageMetadata::readImageSize() {
	std::string source = this->source;
	size_t Position = source.find("imageHeight");
	source.erase(0, Position + 14);
	Position = source.find("\"");
	source.erase(Position, source.size() - Position);
	this->imageHeight = atoi(source.c_str());
	source = this->source;
	Position = source.find("imageWidth");
	source.erase(0, Position + 13);
	Position = source.find("\"");
	source.erase(Position, source.size() - Position);
	this->imageWidth = atoi(source.c_str());

}

void SatelliteImageMetadata::readMarkers()
{
	std::string source = this->source;
	std::string sourceCopy = this->source;
	std::list<short> list1;
	std::list<short> list2;

	while(true) {
		source = sourceCopy;
		size_t Position = source.find("anchor");
		if (Position >= source.size())
			break;
		source.erase(0, Position + 14);
		sourceCopy = source;
		Position = source.find("\"");
		source.erase(Position, source.size() - Position);
		list1.push_back(atoi(source.c_str()));
		source = sourceCopy;
		Position = source.find("y");
		source.erase(0, Position + 4);
		Position = source.find("\"");
		source.erase(Position, source.size() - Position);
		list2.push_back(atoi(source.c_str()));
	}

	this->markers[0] = new short[list1.size()];
	this->markers[1] = new short[list1.size()];
	this->markersCount = list1.size();

	std::list<short>::iterator it = list1.begin();
	std::list<short>::iterator it2 = list2.begin();
	for (int i = 0; i < list1.size(); i++) {
		markers[0][i] = *it;
		markers[1][i] = *it2;
		it++;
		it2++;
	}


}

short SatelliteImageMetadata::sizeX()
{
	std::cout << this->markers[0][this->markersCount - 1] << std::endl;
	return (this->markers[0][this->markersCount-1]-this->markers[0][0])+1;
}

short SatelliteImageMetadata::sizeY()
{
	return this->markers[1][0] - this->markers[1][this->markersCount -1]+1;
}
void SatelliteImageMetadata::saveOnDrive(std::string path)
{
	FILE* file = fopen(path.c_str(), "wb");
	if (file == NULL)
		std::cout << "error" << std::endl;
	fwrite(this->source.c_str(), 1, this->sourceSize, file);
	fclose(file);
}

SatelliteImageMetadata* SatelliteImageMetadata::readFromDrive(std::string path)
{
	int lSize;
	char * buffer;
	size_t result;
	Sleep(5);
	FILE* file = fopen(path.c_str(), "rb");

	if (file == NULL) {
		std::cout << "error" << errno<< std::endl;
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

	fclose(file);
	return new SatelliteImageMetadata(buffer, lSize);
}