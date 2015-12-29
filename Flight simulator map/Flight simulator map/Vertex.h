#pragma once
#include "Environment.h"

class Vertex
{
public:
	Vertex();
	~Vertex();
	glm::vec3 position;
	glm::vec2 textureCoord;
	Vertex(glm::vec3 position, glm::vec2 textureCoord);
};


