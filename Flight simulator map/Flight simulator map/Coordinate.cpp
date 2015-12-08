#include "Coordinate.h"



Coordinate::Coordinate()
{
}

Coordinate::Coordinate(double latitude, double longtitude) {
	this->latitude = latitude;
	this->longtitude = longtitude;
}

Coordinate::~Coordinate()
{
}


Coordinate Coordinate::calculateMidle(Coordinate cord1, Coordinate cord2) {	
	double newLatitude, newLongtitude,smalerLatitude,biggerLatitude,smalerLongtitude,biggerLongtitude;
	if (cord1.latitude > cord2.latitude) {
		smalerLatitude = cord2.latitude;
		biggerLatitude = cord1.latitude;
	}
	else {
		smalerLatitude = cord1.latitude;
		biggerLatitude = cord2.latitude;
	}
	if (cord1.longtitude > cord2.longtitude) {
		smalerLongtitude = cord2.longtitude;
		biggerLongtitude = cord1.longtitude;
	}
	else {
		smalerLongtitude = cord1.longtitude;
		biggerLongtitude = cord2.longtitude;
	}

	return Coordinate(smalerLatitude + (biggerLatitude - smalerLatitude) / 2, smalerLongtitude + (biggerLongtitude - smalerLongtitude) / 2);
}