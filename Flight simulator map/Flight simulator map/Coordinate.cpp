#include "Coordinate.h"



Coordinate::Coordinate()
{
}

Coordinate::Coordinate(double latitude, double longitude) {
	this->latitude = latitude;
	this->longitude = longitude;
}

Coordinate::~Coordinate()
{
}


Coordinate Coordinate::calculateMidle(Coordinate cord1, Coordinate cord2) {	
	double newLatitude, newlongitude,smalerLatitude,biggerLatitude,smalerlongitude,biggerlongitude;
	if (cord1.latitude > cord2.latitude) {
		smalerLatitude = cord2.latitude;
		biggerLatitude = cord1.latitude;
	}
	else {
		smalerLatitude = cord1.latitude;
		biggerLatitude = cord2.latitude;
	}
	if (cord1.longitude > cord2.longitude) {
		smalerlongitude = cord2.longitude;
		biggerlongitude = cord1.longitude;
	}
	else {
		smalerlongitude = cord1.longitude;
		biggerlongitude = cord2.longitude;
	}

	return Coordinate(smalerLatitude + (biggerLatitude - smalerLatitude) / 2, smalerlongitude + (biggerlongitude - smalerlongitude) / 2);
}