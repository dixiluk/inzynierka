#pragma once
#include "Environment.h"
#include "GraphicalObject.h"

class Scene
{
public:
	std::list<GraphicalObject*> graphicalObjects;
	std::list<GraphicalObject*> kulki; //
	void addGraphicalObject(GraphicalObject* object);
	Scene();
	~Scene();
	
};

