#pragma once
class Coordinate
{
public:
	double latitude, longtitude;
	Coordinate();
	Coordinate(double latitude, double longtitude);
	static Coordinate calculateMidle(Coordinate cord1, Coordinate cord2);
	Coordinate size(Coordinate cord);
	~Coordinate();
};

