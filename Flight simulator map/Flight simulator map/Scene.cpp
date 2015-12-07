#include "Scene.h"


Scene::Scene()
{
}


Scene::~Scene()
{
}

void Scene::addGraphicalObject(GraphicalObject* object){
	this->graphicalObjects.push_back(object);
}
