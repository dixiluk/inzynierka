#include "ElevationData.h"
#include <string>

#include <sstream>
#include "Config.h"


#define PI2  6.28318531
short ElevationData::rows;
short ElevationData::cols;

ElevationData::ElevationData(Coordinate southWest, Coordinate northEast, std::string source, int sourceSize)
{
	this->source = source;
	this->sourceSize = sourceSize;
	this->southWest = southWest;
	this->northEast = northEast;
	this->heights = new float*[rows];
	for (int row = 0; row < rows; row++)
		this->heights[row] = new float[cols];
	this->coordinates = new Coordinate*[rows];
	for (int row = 0; row < rows; row++)
		this->coordinates[row] = new Coordinate[cols];
	size_t Position = source.find("elevations");
	source.erase(0, Position+13);
	Position = source.find("]");
	source.erase(Position, source.size() - Position);
	std::stringstream ss(source);
	double stepLatitude = (northEast.latitude-southWest.latitude)/(rows-1);
	double stepLongitude = (northEast.longtitude - southWest.longtitude) / (cols-1);
	Coordinate cord = southWest;

	for (int row = 0; row < rows; row++) {
		for (int col = 0; col < cols; col++) {
			
			std::getline(ss, source, ',');
			this->heights[row][col] = atof(source.c_str());
			this->coordinates[row][col] = Coordinate(PI2*(cord.latitude+90)/360, PI2*(cord.longtitude+180)/360);
			cord.longtitude += stepLongitude;
		}
		cord.longtitude = southWest.longtitude;
		cord.latitude += stepLatitude;
	}

	return;
}



ElevationData::~ElevationData()
{
}

void ElevationData::saveOnDrive(std::string path)
{
	FILE* file = fopen(path.c_str(), "wb");
	if (file == NULL)
		std::cout << "error" << std::endl;
	fwrite(this->source.c_str(), 1, this->sourceSize, file);
	fclose(file);
}

ElevationData* ElevationData::readFromDrive(Coordinate southWest, Coordinate northEast,std::string path)
{
	int lSize;
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


	fclose(file);
	return new ElevationData(southWest,northEast, buffer, lSize);
}