#pragma once
#include "Environment.h"
#include "Shader.h"

class GraphicalObject
{
	friend class Engine;
	friend class StructureShader;
	friend class CarShader;
public:
	GraphicalObject();
	~GraphicalObject();
protected:
	glm::vec3 position;
	glm::vec3 rotationAxis;
	Shader* shader;

	virtual void compute();
	virtual void draw();
};

