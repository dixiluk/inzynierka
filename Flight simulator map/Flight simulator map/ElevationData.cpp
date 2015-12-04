#include "ElevationData.h"

#include <iostream>


ElevationData::ElevationData(short rows, short cols, Coordinate southWest, Coordinate northEast, char* source)
{
	this->rows = rows;
	this->cols = cols;
	this->southWest = southWest;
	this->northEast = northEast;
	this->heights = new float*[rows];
	for (int row = 0; row < rows; row++)
		this->heights[row] = new float[cols];

	char * pch;
	pch = strtok(str, " ,.-");
	while (pch != NULL)
	{
		pch = strtok(NULL, " ,.-");
	}
	return 0;



	for (int row = 0;  row < rows; row++) {
		for (int col = 0; col < cols; col++) {
			this->heights[row][col] = 0;
		}
	}

}



ElevationData::~ElevationData()
{
}

