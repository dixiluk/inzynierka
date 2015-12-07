#pragma once
#include "GraphicalObject.h"

class DynamicObject : public GraphicalObject
{
public:
	DynamicObject();
	~DynamicObject();
	void draw();
	void compute();
};

