#pragma once

#include "Environment.h"

class Texture
{
public:
	Texture(char* source, long unsigned int size);
	Texture();
	~Texture();
	GLuint id;

protected:
private:
	static std::list<Texture*> Textures;

};

