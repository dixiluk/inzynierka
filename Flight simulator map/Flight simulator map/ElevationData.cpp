#include "ElevationData.h"
#include <string>

#include <sstream>
ElevationData::ElevationData(short rows, short cols, Coordinate southWest, Coordinate northEast, std::string source)
{
	this->rows = rows;
	this->cols = cols;
	this->southWest = southWest;
	this->northEast = northEast;
	this->heights = new float*[rows];
	for (int row = 0; row < rows; row++)
		this->heights[row] = new float[cols];
	size_t Position = source.find("elevations");
	source.erase(0, Position+13);
	Position = source.find("]");
	source.erase(Position, source.size() - Position);
	std::stringstream ss(source);

	for (int row = 0; row < rows; row++) {
		for (int col = 0; col < cols; col++) {
			std::getline(ss, source, ',');
			this->heights[row][col] = atof(source.c_str());
		}
	}
}



ElevationData::~ElevationData()
{
}

