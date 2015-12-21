#include "ElevationData.h"
#include <string>

#include <sstream>


#define PI2  6.28318531

ElevationData::ElevationData(short rows, short cols, Coordinate southWest, Coordinate northEast, std::string source)
{
	this->rows = rows;
	this->cols = cols;
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

