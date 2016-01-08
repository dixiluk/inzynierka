#include "ElevationData.h"
#include <string>
#include "Environment.h"
#include <sstream>
#include "Config.h"

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
		this->coordinates = new Coordinate*[rows];


	double stepLatitude = (northEast.latitude - southWest.latitude) / (rows - 1);
	double stepLongitude = (northEast.longitude - southWest.longitude) / (cols - 1);
	Coordinate cord = southWest;
	for (int row = 0; row < rows; row++) {
		this->coordinates[row] = new Coordinate[cols];
		for (int col = 0; col < cols; col++) {
			this->coordinates[row][col] = Coordinate(PI2*(cord.latitude + 90) / 360, PI2*(cord.longitude + 180) / 360);
			cord.longitude += stepLongitude;
		}
		cord.longitude = southWest.longitude;
		cord.latitude += stepLatitude;
	}

	if (sourceSize > 0) {
		size_t Position = source.find("elevations");
		source.erase(0, Position + 13);
		Position = source.find("]");
		source.erase(Position, source.size() - Position);
		std::stringstream ss(source);

		for (int row = 0; row < rows; row++) {
			this->heights[row] = new float[cols];
			for (int col = 0; col < cols; col++) {
				std::getline(ss, source, ',');
				this->heights[row][col] = atof(source.c_str());
			}
		}
	}
	else {
		for (int row = 0; row < rows; row++) {
			this->heights[row] = new float[cols];
			for (int col = 0; col < cols; col++) {
				this->heights[row][col] = 0;
			}
		}
	}
	return;
}



ElevationData::~ElevationData()
{
	for (int i = 0; i < this->rows; i++)
		delete[] this->heights[i];
	for (int i = 0; i < this->rows; i++)
		delete[] this->coordinates[i];
	
}

void ElevationData::saveOnDrive(std::string path)
{
	FILE* file = fopen(path.c_str(), "wb");
	if (file == NULL)
		std::cout << "error saveOnDrive" << std::endl;
	fwrite(this->source.c_str(), 1, this->sourceSize, file);
	fclose(file);
}

ElevationData* ElevationData::readFromDrive(Coordinate southWest, Coordinate northEast, std::string path)
{
	int lSize;
	char * buffer;
	size_t result;

	FILE* file = fopen(path.c_str(), "rb");

	if (file == NULL) {
		//std::cout << "nie odnaleziono pliku"+path << std::endl;
		return NULL;
	}

	fseek(file, 0, SEEK_END);
	lSize = ftell(file);
	rewind(file);


	buffer = (char*)malloc(lSize+1);
	

	result = fread(buffer, 1, lSize, file);
	if (result != lSize) {
		free(buffer);
		return NULL;
	}
	buffer[lSize] = 0;
	std::string copy = std::string(buffer);
	free(buffer);
	
	fclose(file);
	return new ElevationData(southWest, northEast, copy, lSize);
}