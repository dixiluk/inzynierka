#include "Vertex.h"



Vertex::Vertex(glm::vec3 position, glm::vec2 textureCoord) {
	this->position = position;
	this->textureCoord = textureCoord;
}

Vertex::Vertex()
{
}


Vertex::~Vertex()
{
}
