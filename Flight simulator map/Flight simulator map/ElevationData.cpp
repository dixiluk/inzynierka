#include "ElevationData.h"
#include <string>

#include <sstream>
ElevationData::ElevationData(short rows, short cols, Coordinate southWest, Coordinate northEast, std::string source)
{
	//std::cout << source << std::endl << std::endl << std::endl;
	this->rows = rows;
	this->cols = cols;
	this->southWest = southWest;
	this->northEast = northEast;
	this->heights = new float*[rows];
	for (int row = 0; row < rows; row++)
		this->heights[row] = new float[cols];
	size_t Position = source.find("elevations");
	//std::cout << source << std::endl << std::endl << std::endl;
	source.erase(0, Position+13);
	//std::cout << source << std::endl << std::endl << std::endl;
	Position = source.find("]");
	source.erase(Position, source.size() - Position);
	//std::cout << source << std::endl << std::endl << std::endl;
	std::stringstream ss(source);

	for (int row = 0; row < rows; row++) {
		for (int col = 0; col < cols; col++) {
			std::getline(ss, source, ',');
			this->heights[row][col] = atof(source.c_str());
		}
	}

	for (int row = 0; row < rows; row++) {
		for (int col = 0; col < cols; col++) {
		//	std::cout << this->heights[row][col];
		}
	//	std::cout << std::endl;
	}
}



ElevationData::~ElevationData()
{
}

