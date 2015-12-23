#pragma once

#include "Environment.h"

class Texture
{
public:
	Texture(char* source, void* tmp);
	Texture();
	~Texture();
	GLuint id;

	static fi_handle LoadAdress;

protected:
private:
	static std::list<Texture*> Textures;

};

