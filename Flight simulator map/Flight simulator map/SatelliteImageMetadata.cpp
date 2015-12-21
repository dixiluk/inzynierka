#include "SatelliteImageMetadata.h"
#include <sstream>



SatelliteImageMetadata::SatelliteImageMetadata(std::string data)
{
	this->data = data;
	this->readPositions();
	this->readImageSize();
	this->readTwoMarkers();
}


SatelliteImageMetadata::~SatelliteImageMetadata()
{
}

void SatelliteImageMetadata::readPositions() {
	std::string source = this->data;
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
	std::string source = this->data;
	size_t Position = source.find("imageHeight");
	source.erase(0, Position + 14);
	Position = source.find("\"");
	source.erase(Position, source.size() - Position);
	this->imageHeight = atoi(source.c_str());
	source = this->data;
	Position = source.find("imageWidth");
	source.erase(0, Position + 13);
	Position = source.find("\"");
	source.erase(Position, source.size() - Position);
	this->imageWidth = atoi(source.c_str());

}

void SatelliteImageMetadata::readTwoMarkers()
{
	std::string source = this->data; 
	std::string sourceCopy = this->data;
	size_t Position = source.find("anchor");
	source.erase(0, Position + 14);
	sourceCopy = source;
	Position = source.find("\"");
	source.erase(Position, source.size() - Position);
	this->firstMarkerx = atoi(source.c_str());
	source = sourceCopy;
	Position = source.find("y");
	source.erase(0, Position + 4);
	Position = source.find("\"");
	source.erase(Position, source.size() - Position);
	this->firstMarkery = atoi(source.c_str());


	source = sourceCopy;
	Position = source.find("anchor");
	source.erase(0, Position + 14);
	sourceCopy = source;
	Position = source.find("\"");
	source.erase(Position, source.size() - Position);
	this->secondMarkerx = atoi(source.c_str());
	source = sourceCopy;
	Position = source.find("y");
	source.erase(0, Position + 4);
	Position = source.find("\"");
	source.erase(Position, source.size() - Position);
	this->secondMarkery = atoi(source.c_str());
}
