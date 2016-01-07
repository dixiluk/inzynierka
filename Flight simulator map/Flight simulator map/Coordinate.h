#pragma once
class Coordinate
{
public:
	double latitude, longitude;
	Coordinate();
	Coordinate(double latitude, double longitude);
	~Coordinate();
	static Coordinate calculateMidle(Coordinate cord1, Coordinate cord2);
};

