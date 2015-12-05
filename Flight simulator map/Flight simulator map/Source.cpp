#include "ElevationData.h"
#include "HttpRequester.h"
#include <iostream>

	using namespace std;
void main() {
	//ElevationData(4, 4, Coordinate(50, 100), Coordinate(60, 110), "teelevationsxt]1234");
	HttpRequester *tmp = new HttpRequester();
	tmp->getElevationData(Coordinate(1,1), Coordinate(2, 2), 5, 5, "sealevel", "AvLyPxYc5C5cPPAwZdsrhI1c4sT9FJo1AUVym7tgs-IvZzo720jrDdn-ZG-0Jrb9");

	cout << "tt";
}